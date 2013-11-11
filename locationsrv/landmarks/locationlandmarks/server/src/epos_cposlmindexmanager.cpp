/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description: provides handling of indexing in database
*
*
*/


#include <e32base.h>
#include <epos_cposlmlocaldbaccess.h>
#include <epos_poslmlandmarkhandler.h>
#include <epos_poslmdatabaseutility.h>

#include "EPos_CPosLmServer.h"
#include "EPos_CPosLmNameIndex.h"
#include "epos_lmdebug.h"
#include "epos_poslmserverutility.h"
#include "epos_cposlmindexmanager.h"

const TTimeIntervalMicroSeconds KPosLmMaxIndexOlderThanDb = 1*1000*1000; // 1 sec
const TTimeIntervalMicroSeconds KPosLmMaxIndexNewerThanDb = 60*1000*1000; // 1 min

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmIndexManager::CItem::~CItem()
    {
    delete iDbAccess;
    delete iIndex;
    delete iDbUri;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmIndexManager::CPosLmIndexManager( CPosLmServer& aServer )
: 	iServer( aServer )
    {
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmIndexManager::~CPosLmIndexManager()
    {
    ASSERT( iIndexes.Count() == 0 ); // all indexes should be already disconnected
    iIndexes.ResetAndDestroy();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmIndexManager* CPosLmIndexManager::NewL( CPosLmServer& aServer )
    {
    CPosLmIndexManager* self = new (ELeave) CPosLmIndexManager( aServer );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmIndexManager::ConstructL()
    {
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmIndexManager::AddIndexUserL( const TDesC& aUri )
    {
    TInt pos = Find( aUri );
    if ( pos != KErrNotFound )
        {
        CItem* item = iIndexes[pos];
        item->iRefCount++;
        }
    else
        {
        LoadIndexL( aUri );
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmIndexManager::RemoveIndexUser( const TDesC& aUri )
    {
    TInt pos = Find( aUri );
    if ( pos != KErrNotFound )
        {
        CItem* item = iIndexes[pos];
        if ( --(item->iRefCount) <= 0 )
            {
            iIndexes.Remove( pos );
            ASSERT( !item->iHasWriteLock && !item->iHasReadLock );
            item->iHasWriteLock = ETrue;
#ifdef _DEBUG
            TRAPD( err, SaveIndexL( *item ) );
            LOG2( "IndexManager: Saved index for db '%S', err %d", &aUri, err );
#else
            TRAP_IGNORE( SaveIndexL( *item ) );
#endif            
            delete item;
            }
        }
    }

// -----------------------------------------------------------------------------
// This one should always succeed, because any client should first register
// for the URI before asking index
// -----------------------------------------------------------------------------
//
CPosLmNameIndex& CPosLmIndexManager::Index( const TDesC& aUri )
    {
    TInt pos = Find( aUri );
    ASSERT( pos >= 0 ); 
    return *(iIndexes[pos]->iIndex); // there is always valid index pointer
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TInt CPosLmIndexManager::Find( const TDesC& aUri ) const
    {
    for ( int i = 0; i < iIndexes.Count(); ++i ) 
        {
        if ( iIndexes[i]->iDbUri->CompareF( aUri ) == 0 )
            {
            return i;
            }
        }
    return KErrNotFound;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmIndexManager::LoadIndexL( const TDesC& aUri )
    {
    CItem* item = new (ELeave) CItem();
    CleanupStack::PushL( item );

    item->iDbUri = aUri.AllocL();
    item->iRefCount = 1;
    
    CPosLmLocalDbAccess* dbAccess = CPosLmLocalDbAccess::NewL();
    item->iDbAccess = dbAccess;
    TInt err = dbAccess->OpenDatabaseL( aUri );
    User::LeaveIfError( err );

    item->iIndex = CPosLmNameIndex::NewL( *dbAccess );
    if ( item->iIndex->Status() == KErrNone && 
         PosLmServerUtility::TotalLandmarkCountL( *dbAccess ) > 0 )
        {
        // advanced check: 
        // verify that index timestamp is not older
        // than timestamp of the database
        if ( !IsIndexValidL( DatabaseStampL( aUri ), item->iIndex->TimeStamp() ) )
            {
            item->iIndex->Reset();
            }
        }
    
    iIndexes.AppendL( item );
    CleanupStack::Pop( item );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool CPosLmIndexManager::HasWriteLock( const TDesC& aUri ) const
    {
    TInt pos = Find( aUri );
    if ( pos != KErrNotFound )
        {
        CItem* item = iIndexes[pos];
        if ( item->iHasWriteLock )
            {
            return ETrue;
            }
        }

    return EFalse;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool CPosLmIndexManager::HasAnyLock( const TDesC& aUri ) const
    {
    TInt pos = Find( aUri );
    if ( pos != KErrNotFound )
        {
        CItem* item = iIndexes[pos];
        if ( item->iHasWriteLock || item->iHasReadLock )
            {
            return ETrue;
            }
        }

    return EFalse;
    }

// -----------------------------------------------------------------------------
// RDb does not expose any method to detect database modification timestamp
// or any automatic timestamp. This is a workaround.
// -----------------------------------------------------------------------------
//
TTime CPosLmIndexManager::DatabaseStampL( const TDesC& aDbUri ) const 
    {
    LOG1("IndexManager::DatabaseStamp: dbUri '%S'", &aDbUri );

    TFileName dbPath( aDbUri );
    TPtrC path( dbPath );
    PosLmDatabaseUtility::RemoveProtocolFromUriL( path );

    _LIT( KDatabaseFilePath, "\\private\\100012a5\\DBS_%X_" );
    TFileName filename;
    // drive
    TInt delimPos = path.Locate(':'); 
    filename.Copy( path.Left( delimPos + 1 ) );
    filename.AppendFormat( KDatabaseFilePath(), KPosLmDbSecureUid );
    filename.Append( path.Mid( delimPos + 1 ) );
    
    LOG1("IndexManager::DatabaseStamp: file '%S'", &filename );

    TTime modified;
    TInt err = iServer.FileSession().Modified( filename, modified );
#ifdef _DEBUG    
    TBuf<64> mtime;
    modified.FormatL( mtime, KPosLmTimeFormat );
    LOG2("IndexManager::DatabaseStamp: file modtime %S, error %d", &mtime, err );
#endif    
    if ( err ) 
        {
        modified.UniversalTime();
        modified += TTimeIntervalDays( 1 );
        }
    
    return modified;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmIndexManager::SaveIndexL( CItem& aItem )
    {
    TChar drive = (*(aItem.iDbUri))[KPosLocalDbFileProtocol().Length()];
    aItem.iIndex->SaveL( drive );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool CPosLmIndexManager::IsIndexValidL( const TTime& aDbStamp, const TTime& aIndexStamp ) const
    {
    TTime now;
    now.UniversalTime();
    
#ifdef _DEBUG
    TBuf<64> dtime, itime;
    aDbStamp.FormatL( dtime, KPosLmTimeFormat );
    aIndexStamp.FormatL( itime, KPosLmTimeFormat );
    LOG2("IndexManager: timestamps: index %S, db %S", &itime, &dtime );

    TTimeIntervalMicroSeconds diff = aIndexStamp.MicroSecondsFrom( aDbStamp );
#endif 

    if ( aDbStamp > now || aIndexStamp > now )
        {
        LOG("IndexManager: index and/or db time is in future, index invalid");
        return EFalse;
        }
    else if ( aIndexStamp + KPosLmMaxIndexOlderThanDb < aDbStamp )
        {
        LOG1("IndexManager: database newer than index, diff %ld us, index invalid", diff.Int64() );
        return EFalse;
        }
    else if ( aIndexStamp - KPosLmMaxIndexNewerThanDb > aDbStamp )
        {
        LOG1("IndexManager: index is too much newer than database, diff %ld us, index invalid", diff.Int64() );
        return EFalse;
        }
    else
        {
        return ETrue;
        }
    }

