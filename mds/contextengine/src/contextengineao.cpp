/*
* Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:
*
*/


#include <sbdefs.h>
#include <e32property.h>

#include "contextengineao.h"
#include "contextengine.h"
#include "contextsnapshotitem.h"
#include "harvesterlog.h"
#include "mdsutils.h"
#include "harvesterdata.h"
#include "propertywatcher.h" 
#include "harvestercommon.h"

// ---------------------------------------------------------------------------
// Default constructor for first phase construction.
// ---------------------------------------------------------------------------
//
CContextEngineAO::CContextEngineAO( 
		MContextInitializationObserver* aInitObserver,
		MContextSnapshotObserver* aSnapshotObserver ) : 
		CActive( KHarvesterPriorityContextEngine ), 
		iInitializationObserver( aInitObserver ), 
		iContextSnapshotObserver ( aSnapshotObserver ),
		iUninitializedPluginsCount( 0 ),
		iPluginSnapshotsLacking( 0 ),
		iItem( NULL ),
	    iState( EInitialization ),
	    iInitializationError( KErrNone ),
	    iSnapshotError( KErrNone ),
	    iMdESession( NULL ),
	    iPropertyWatcher( NULL ),
	    iLastPlugin( EFalse )
    {
    WRITELOG( "CContextEngineAO::CContextEngineAO" ); // DEBUG INFO
    }

// ---------------------------------------------------------------------------
// Standard NewL.
// ---------------------------------------------------------------------------
//
CContextEngineAO* CContextEngineAO::NewL(
    MContextInitializationObserver* aInitObserver,
    MContextSnapshotObserver* aSnapshotObserver )
    {
    WRITELOG( "CContextEngineAO::NewL" ); // DEBUG INFO

    CContextEngineAO* self = new (ELeave) CContextEngineAO( aInitObserver, aSnapshotObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// 2nd phase construction.
// ---------------------------------------------------------------------------
//
void CContextEngineAO::ConstructL()
    {
    WRITELOG( "CContextEngineAO::ConstructL" ); // DEBUG INFO

    CActiveScheduler::Add( this );

    iPropertyWatcher = CPropertyWatcher::GetInstanceL();
      
  	// Want to listen when backup/restore starts.
  	// Calls NotifyKeyL when key's state has changed. 
  	iPropertyWatcher->ListenKeyChangesL( 
  			KUidSystemCategory,
  			conn::KUidBackupRestoreKey, this );
      
    
    if ( !iInitializationObserver )
        {
        InitializeL();
        iState = EReady;
        }
    else  // same thing asyncronously
        {
        iState = EInitialization;
        SetActive();
        TRequestStatus* status = &iStatus;
        User::RequestComplete( status, KErrNone ); // "kick-start" this AO
        }
    }
// ---------------------------------------------------------------------------
// Destructor.
// ---------------------------------------------------------------------------
//
CContextEngineAO::~CContextEngineAO()
    {
    WRITELOG( "CContextEngineAO::~CContextEngineAO" ); // DEBUG INFO

    Cancel();

    MdsUtils::CleanupPtrArray<CContextPlugin>( &iPlugins );
	
    if( iPropertyWatcher )
		{
		
	 	iPropertyWatcher->StopListeningKeyChanges( 
	  			KUidSystemCategory,
	  			conn::KUidBackupRestoreKey, this );
		
		iPropertyWatcher->Delete(); // Release connection to TLS object.
		}
    }

// ---------------------------------------------------------------------------
// Set MdeSession for context engine plugins.
// ---------------------------------------------------------------------------
//
void CContextEngineAO::SetMdeSession( CMdESession* aSession )
    {
    iMdESession = aSession;

    const TInt count( iPlugins.Count() );
    for ( TInt i = 0; i < count; ++i )
        {
        iPlugins[i]->SetMdeSession( *aSession );
        }
    }

// ---------------------------------------------------------------------------
// Start taking a context snapshot (for multiple objects).
// ---------------------------------------------------------------------------
//
void CContextEngineAO::StartSnapshot( CContextSnapshotItem* aItem )
    {
    WRITELOG( "CContextEngineAO::StartSnapshot" ); // DEBUG INFO
    iItem = aItem;
    if ( !aItem ) return;
    if ( !aItem->GetItem() && !aItem->GetItemArray() )
        {
        // no items to do the snapshot for... stop.
        aItem->GetObserver()->ContextSnapshotStatus( KErrNone );
        }

    iSnapshotError = KErrNone;
    iPluginSnapshotsLacking = iPlugins.Count(); // reset, no snapshots taken yet

    if ( iPluginSnapshotsLacking <= 0 )
    	{
    	iState = EReady;
    	iContextSnapshotObserver->ContextSnapshotStatus( KErrNone );    
    	}
    iLastPlugin = EFalse;
    
    iState = ESnapshot;
    SetActive();
    TRequestStatus* status = &iStatus;
    User::RequestComplete( status, KErrNone ); // "kick-start" this AO
    }

// ---------------------------------------------------------------------------
// Return loaded plugin count.
// ---------------------------------------------------------------------------
//
TInt CContextEngineAO::PluginCount()
    {
    return iPlugins.Count();
    }

// ---------------------------------------------------------------------------
// From CActive.
// Start/continue taking a context snapshot (for multiple objects).
// Ask a snapshot from one plugin at a time.
// ---------------------------------------------------------------------------
//
void CContextEngineAO::RunL()
	{
#ifdef _DEBUG
    WRITELOG1( "CContextEngineAO::RunL iStatus: %d", iStatus.Int() ); // DEBUG INFO
#endif

    if ( iState == EInitialization )
        {
        InitializeL();
        }
    else if ( iState == ESnapshot )
        {
        if ( iStatus.Int() != KErrNone )
            {
            return;
            }

        if ( iPluginSnapshotsLacking > 0 )
            {
            TInt i = iPlugins.Count() - iPluginSnapshotsLacking;
            if( iPluginSnapshotsLacking == 1 )
                {
                iLastPlugin = ETrue;
                }
            if ( iItem->GetItemArray() ) 
                {
                iPlugins[i]->ContextSnapshot( *this, *iItem->GetItemArray() );  
                }                               // CContextEngineAO is instance of
            else                                // MContextPluginObserver
                {
                iPlugins[i]->ContextSnapshot( *this, *iItem->GetItem() );
                }
            iPluginSnapshotsLacking--;
            }
        }
    }

// ---------------------------------------------------------------------------
// From CActive.
// ---------------------------------------------------------------------------
//
#ifdef _DEBUG
TInt CContextEngineAO::RunError( TInt aError )
#else
TInt CContextEngineAO::RunError( TInt )
#endif
    {
    WRITELOG1( "CContextEngineAO::RunError with error code: %d", aError ); // DEBUG INFO
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// From CActive. 
// ---------------------------------------------------------------------------
//
void CContextEngineAO::DoCancel()
    {
    WRITELOG( "CContextEngineAO::DoCancel" ); // DEBUG INFO

    iPluginSnapshotsLacking = 0;
    iState = EReady;
    }

// ---------------------------------------------------------------------------
// From MContextPluginObserver.
// ---------------------------------------------------------------------------
//
void CContextEngineAO::PluginInitializationStatus( TInt aErrorCode )
    {
    WRITELOG1( "CContextEngineAO::PluginInitializationStatus with error code: %d", aErrorCode ); // DEBUG INFO

    if ( aErrorCode != KErrNone && iInitializationError != KErrNone )
        {
        iInitializationError = aErrorCode;
        }

    iUninitializedPluginsCount--;
    if ( iUninitializedPluginsCount <= 0 )
        {
        iState = EReady;
        // notify the client application by using MContextSnapshotObserver's
        // method ContextInitializationStatus()
        // Notice! Error code of the last plugin is returned...
        if ( iInitializationObserver )
            {
            iInitializationObserver->ContextInitializationStatus( iInitializationError );
            }
        }
    }

// ---------------------------------------------------------------------------
// From MContextPluginObserver.
// ---------------------------------------------------------------------------
//
void CContextEngineAO::PluginSnapshotStatus( CHarvesterData* aHD )
    {
    TInt errorCode = aHD->ErrorCode();
    WRITELOG1( "CContextEngineAO::PluginSnapshotStatus with error code: %d", errorCode ); // DEBUG INFO

    if ( errorCode != KErrNone && iSnapshotError != KErrNone )
        {
        aHD->SetErrorCode( errorCode );
        }
    
    if( iLastPlugin )
        {
        // Notice! Error code of the last plugin is returned...
        iContextSnapshotObserver->ContextSnapshotStatus( aHD );
        }

    if ( iPluginSnapshotsLacking <= 0 )
        {
        iState = EReady;
        }
    else 
        {  // do this again for the next plugin/harvest data object
        if( !IsActive() )
            {
            SetActive();
            TRequestStatus* status = &iStatus;
            User::RequestComplete( status, KErrNone ); // "kick-start" this AO
            }
        }
    }

// ---------------------------------------------------------------------------
// Called by CBlacklistBackupSubscriberAO when
// Backup&Restore is backing up or restoring.
// ---------------------------------------------------------------------------
//
void CContextEngineAO::BackupRestoreStart()
    {
    // stop context plugins
    WRITELOG( "CContextEngineAO::BackupRestoreStart" );
 
    iPlugins.ResetAndDestroy(); // remove any existing plugins, just in case
    }

// ---------------------------------------------------------------------------
// Called by CBlacklistBackupSubscriberAO when
// Backup&Restore has finished backup or restore.
// ---------------------------------------------------------------------------
//
void CContextEngineAO::BackupRestoreReady()
    {
    // restart plugins
    WRITELOG( "CContextEngineAO::BackupRestoreReady" );
    
    TRAP_IGNORE( InitializeL() );
    
    if ( iMdESession )
    	{
    	SetMdeSession( iMdESession );
    	}
    
    iState = EReady;
    
    }

// private methods

// ---------------------------------------------------------------------------
// Load context plugins to the "iPlugins" array.
// ---------------------------------------------------------------------------
//
void CContextEngineAO::LoadPluginsL()
    {
    WRITELOG( "CContextEngineAO::LoadPluginsL" ); // DEBUG INFO

    if ( iPlugins.Count() > 0 )
        {
        iPlugins.ResetAndDestroy(); // remove any existing plugins, just in case
        }

    // read info about all implementations into infoArray
    RImplInfoPtrArray infoArray;
    TCleanupItem cleanupItem( MdsUtils::CleanupEComArray, &infoArray );
    CleanupStack::PushL( cleanupItem );
    
    CContextPlugin::ListImplementationsL( infoArray );

    CContextPlugin* plugin = NULL;
    TInt err = KErrNone;
    for ( TInt i = 0; i < infoArray.Count(); i++ )
        {
        TUid uid = infoArray[i]->ImplementationUid(); 
        TRAP( err, plugin = CContextPlugin::NewL( uid ) ); // create the plug-ins
        if ( err == KErrNone && plugin )
            {
            CleanupStack::PushL( plugin );
            iPlugins.AppendL( plugin );        // and add them to an array
            CleanupStack::Pop( plugin );
            }
        }

	CleanupStack::PopAndDestroy( &infoArray ); // infoArray, results in a call to CleanupEComArray		
    }

// ---------------------------------------------------------------------------
// Initialize this object. Thus load and initialize plugins.
// ---------------------------------------------------------------------------
//
void CContextEngineAO::InitializeL()
    {
    WRITELOG( "CContextEngineAO::InitializeL" ); // DEBUG INFO

    iInitializationError = KErrNone;
    LoadPluginsL();
    iUninitializedPluginsCount = iPlugins.Count(); // set this so we can later be sure that all
                                                   // plug-ins are initialized.
    if ( iUninitializedPluginsCount <= 0 ) // if no plugins were loaded
        {
        if ( iInitializationObserver )
            {
            iInitializationObserver->ContextInitializationStatus( KErrNone );
            }
        }

    const TInt KCount = iUninitializedPluginsCount;
    for ( TInt i = 0; i < KCount; i++ )
        {
        iPlugins[i]->Init( *this ); // CContextEngineAO is instance of MContextPluginObserver
        }
    }

// -----------------------------------------------------------------------------
// CRestoreWatcher::NotifyKeyL
// CPropertyWatcher's callback. From 
// -----------------------------------------------------------------------------
//
void CContextEngineAO::NotifyKeyL( 
		const TInt aKeyValue,
		const TUid aPropertyCategory,
		const TUint aKey )
	{
	WRITELOG("CContextEngineAO::NotifyKeyL() - begin");
	
	if( aPropertyCategory == KUidSystemCategory &&
			aKey == conn::KUidBackupRestoreKey ) 
		{
		HandleBackupRestoreKeyAction( aKeyValue );
		}
	
	
	WRITELOG("CContextEngineAO::NotifyKeyL() - end");
	}

// -----------------------------------------------------------------------------
// CContextEngineAO::HandleBackupKeyAction
// When user starts restore sets flag on.
// -----------------------------------------------------------------------------
//
void CContextEngineAO::HandleBackupRestoreKeyAction( const TUint aKeyValue )
	{	
	WRITELOG("CRestoreWatcher::HandleBackupKeyAction() - begin");
	
	TInt backupStateValue = aKeyValue;
	backupStateValue &= conn::KBURPartTypeMask;
	
	switch ( backupStateValue )
		{
		case conn::EBURBackupFull:
		case conn::EBURBackupPartial:
		case conn::EBURRestoreFull:
		case conn::EBURRestorePartial:
			{
			BackupRestoreStart();
			break;
			}
		
		case conn::EBURNormal:
		case conn::EBURUnset:
		default:
			{
			// backup or restore is completed, so resume normal operation.
       		BackupRestoreReady();
			}
		}	
	
	WRITELOG("CRestoreWatcher::HandleBackupKeyAction() - end");
	}

