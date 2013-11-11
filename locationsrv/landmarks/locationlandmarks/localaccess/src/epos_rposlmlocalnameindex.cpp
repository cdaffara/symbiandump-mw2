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
* Description: This class is a handle to server-side landmark name index
*
*
*/


#include <e32base.h>
#include <s32mem.h>
#include <epos_poslmservercommon.h>

#include "EPos_RPosLmLocalAccessSubsession.h"
#include "epos_rposlmlocalnameindex.h"

const TInt KInitialBufferSize = 8 * 1024; // 8K

// ================= MEMBER FUNCTIONS =======================

//--------------------------------------------------------------------
// RPosLmLocalNameIndex::CIndexItem
//--------------------------------------------------------------------
//
RPosLmLocalNameIndex::CIndexItem::CIndexItem()
: iId ( KPosLmNullItemId )
    {
    }

RPosLmLocalNameIndex::CIndexItem::~CIndexItem()
    {
    delete iName;
    }

RPosLmLocalNameIndex::CIndexItem* RPosLmLocalNameIndex::CIndexItem::NewL( 
    TPosLmItemId aId, const TDesC& aName )
    {
    CIndexItem* self = new (ELeave) CIndexItem;
    CleanupStack::PushL( self );
    self->iId = aId;
    self->iName = aName.AllocL();
    CleanupStack::Pop( self );
    return self;
    }

RPosLmLocalNameIndex::CIndexItem* RPosLmLocalNameIndex::CIndexItem::NewL( 
    RReadStream& aStream )
    {
    CIndexItem* self = new (ELeave) CIndexItem;
    CleanupStack::PushL( self );

    self->iId = aStream.ReadUint32L();
    TUint32 len = aStream.ReadUint32L();
    self->iName = HBufC::NewL( len );
    TPtr ptr = self->iName->Des();
    aStream.ReadL( ptr, len );
    
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
RPosLmLocalNameIndex::RPosLmLocalNameIndex( RPosLmLocalAccessSubsession& aSession )
:   iSession( aSession ), iBuffer( NULL )
    {
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TInt RPosLmLocalNameIndex::Open()
    {
    iBuffer = HBufC8::New( KInitialBufferSize );
    if ( !iBuffer )
        {
        return KErrNoMemory;
        }
    return iSession.SendReceive( EPosLmServerOpenNameIndex );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void RPosLmLocalNameIndex::Close()
    {
    iSession.SendReceive( EPosLmServerCloseNameIndex );
    delete iBuffer;
    iBuffer = NULL;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RPosLmLocalNameIndex::Count() const
    {
    TInt result = iSession.SendReceive( EPosLmServerNameIndexStatus );
    if ( result >= 0 )
        return result;
    else
        return 0;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RPosLmLocalNameIndex::Status() const
    {
    TInt result = iSession.SendReceive( EPosLmServerNameIndexStatus );
    if ( result < 0 )
        return result;
    else
        return KErrNone;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RPosLmLocalNameIndex::ReadSortedIds( 
    TPosLmItemId* aIdArray, TInt aStartPos, TInt aCount, TInt& aRemainder )
    {
    TPosLmServerReadArrayParam param;
    param.iFirst = aStartPos;
    param.iCount = aCount;

    TPckg<TPosLmServerReadArrayParam> pack( param );
    TPtr8 data( (TUint8*) aIdArray, sizeof( TPosLmItemId ) * aCount );
    TInt err = iSession.SendReceive( EPosLmServerReadSortedIds, TIpcArgs( &pack, &data ));
    if ( err >= 0 )
        {
        aRemainder = param.iRemainder;
        return param.iActualCount;
        }
    else
        {
        return err;
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void RPosLmLocalNameIndex::AddL( const CPosLandmark& aLandmark )
    {
    RPointerArray<CPosLandmark> landmarks; // does not own items
    CleanupClosePushL( landmarks );

    landmarks.AppendL( &aLandmark );
    AddL( landmarks );
    
    CleanupStack::PopAndDestroy( &landmarks );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void RPosLmLocalNameIndex::AddL( RPointerArray<CPosLandmark>& aLandmarks )
    {
    SendLandmarkDataL( EPosLmServerAddLandmarks, aLandmarks );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void RPosLmLocalNameIndex::UpdateL( const CPosLandmark& aLandmark )
    {
    RPointerArray<CPosLandmark> landmarks; // does not own items
    CleanupClosePushL( landmarks );

    landmarks.AppendL( &aLandmark );
    UpdateL( landmarks );
    
    CleanupStack::PopAndDestroy( &landmarks );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void RPosLmLocalNameIndex::UpdateL( RPointerArray<CPosLandmark>& aLandmarks )
    {
    SendLandmarkDataL( EPosLmServerUpdateLandmarks, aLandmarks );
    }


// -----------------------------------------------------------------------------
// Server does not necessarily return all requested items at once
// it depends on size of actual data. Client will need to repeat request
// to get all needed landmarks.
// -----------------------------------------------------------------------------
//
EXPORT_C void RPosLmLocalNameIndex::ReadSortedLandmarksL( 
    TInt aFirst, TInt aCount,
    RPointerArray<CIndexItem>& aLandmarks,
    TInt& aRemainder)
    {
    TPosLmServerReadArrayParam params;
    params.iFirst = aFirst;
    params.iCount = aCount;
    params.iActualCount = 0;
    params.iRemainder = 0;
    params.iMinBufferNeeded = 0;
    TPckg<TPosLmServerReadArrayParam> packParams( params );

    TPtr8 buf = iBuffer->Des();
    buf.SetMax();
    
    User::LeaveIfError( iSession.SendReceive( 
        EPosLmServerReadNameIndex, TIpcArgs( &packParams, &buf ) ) );
        
    if ( params.iActualCount )
        {
        RDesReadStream readStream;
        readStream.Open( *iBuffer );
        CleanupClosePushL( readStream );
    
        for ( TInt i = 0; i < params.iActualCount; i++ )
            {
            CIndexItem* item = CIndexItem::NewL( readStream );
            CleanupStack::PushL( item );
            aLandmarks.AppendL( item );
            CleanupStack::Pop( item );
            }
        CleanupStack::PopAndDestroy( &readStream );
        }

    // check if server requests bigger buffer for next landmark
    if ( params.iMinBufferNeeded > 0 ) 
        {
        iBuffer = iBuffer->ReAllocL(iBuffer->Length() + params.iMinBufferNeeded );
        }
    
    aRemainder = params.iRemainder;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void RPosLmLocalNameIndex::RemoveL( TPosLmItemId aLmid )
    {
    RArray<TPosLmItemId> ids;
    CleanupClosePushL( ids );

    ids.AppendL( aLmid );
    RemoveL( ids );
    
    CleanupStack::PopAndDestroy( &ids );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void RPosLmLocalNameIndex::RemoveL( RArray<TPosLmItemId>& aLmids )
    {
    TInt lmCount = aLmids.Count();
    if ( lmCount < 1 )
        return;
    
    TPosLmItemId* ids = new (ELeave) TPosLmItemId[ lmCount ];
    
    for ( TUint32 i = 0; i < lmCount; i++ )
        {
        *(ids + i) = aLmids[i];
        }
        
    TPtrC8 idsDes( reinterpret_cast<const TUint8*>( ids ), sizeof( TPosLmItemId ) * lmCount );

    TInt err = iSession.SendReceive( EPosLmServerUpdateNameIndex, 
        TIpcArgs( EPosLmServerRemoveLandmarks, &idsDes ) );
        delete[] ids;
    User::LeaveIfError( err );
    }


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TInt RPosLmLocalNameIndex::BeginTransaction()
    {
    return iSession.SendReceive( 
        EPosLmServerIndexTransaction, TIpcArgs( EPosLmServerTransactionStart ) );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TInt RPosLmLocalNameIndex::CommitTransaction()
    {
    return iSession.SendReceive( 
        EPosLmServerIndexTransaction, TIpcArgs( EPosLmServerTransactionCommit ) );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TInt RPosLmLocalNameIndex::RollbackTransaction()
    {
    return iSession.SendReceive( 
        EPosLmServerIndexTransaction, TIpcArgs( EPosLmServerTransactionRollback ) );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TInt RPosLmLocalNameIndex::CalculateLandmarkBasicDataSize( 
    RPointerArray<CPosLandmark>& aLandmarks )
    {
    TInt dataSize = 0;
    for (int i = 0; i < aLandmarks.Count(); ++i)
        {
        TPtrC name;
        aLandmarks[i]->GetLandmarkName( name );
        dataSize += name.Size() 
            + sizeof( TPosLmItemId ) 
            + sizeof( TInt32 ); // + ID + name length
        }
    return dataSize + sizeof( TInt32 ); // + total count
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void RPosLmLocalNameIndex::SendLandmarkDataL( 
    TInt aFunction,
    RPointerArray<CPosLandmark>& aLandmarks )
    {
    if ( aLandmarks.Count() < 1 )
        return;
    
    TInt dataSize = CalculateLandmarkBasicDataSize( aLandmarks );
    
    CBufFlat* lmBuffer = CBufFlat::NewL( dataSize );
    CleanupStack::PushL( lmBuffer );

    RBufWriteStream writeStream;
    writeStream.Open( *lmBuffer );
    CleanupClosePushL( writeStream );

    writeStream.WriteInt32L( aLandmarks.Count() );

    for ( TInt i = 0; i < aLandmarks.Count(); i++ )
        {
        TPtrC name;
        aLandmarks[i]->GetLandmarkName( name );
        
        writeStream.WriteInt32L( aLandmarks[i]->LandmarkId() );
        writeStream.WriteInt32L( name.Length() );
        writeStream.WriteL( name );
        }
    writeStream.CommitL();
    CleanupStack::PopAndDestroy( &writeStream );

    TPtr8 lmBufferPtr( lmBuffer->Ptr( 0 ) );
    User::LeaveIfError( iSession.SendReceive( 
        EPosLmServerUpdateNameIndex, 
        TIpcArgs( aFunction, &lmBufferPtr ) ) );

    CleanupStack::PopAndDestroy( lmBuffer );
    }
