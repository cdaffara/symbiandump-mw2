/*
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Observes Backup & Restore's state key and status -*
*/


// SYSTEM INCLUDE
#include <e32property.h>
#include <sbdefs.h>

//USER INCLUDE
#include "backupsubscriber.h"
#include "harvesterlog.h"
#include "mdsactivescheduler.h"
#include "propertywatcher.h"

// ---------------------------------------------------------------------------
// CBackupSubscriber::CBackupSubscriber
// Default constructor.
// ---------------------------------------------------------------------------
//
CBackupSubscriber::CBackupSubscriber( MBackupRestoreObserver& aObserver )
	: iPropertyWatcher( NULL ), iObserver( aObserver ) 
    {
    // No implementation required.
    }

// ---------------------------------------------------------------------------
// CBackupSubscriber::NewL
// Standard NewL for first phase construction.
// ---------------------------------------------------------------------------
//
CBackupSubscriber* CBackupSubscriber::NewL( MBackupRestoreObserver& aObserver )
    {
    WRITELOG( "CBackupSubscriber::NewL - begin" ); 

    CBackupSubscriber* self = new ( ELeave ) CBackupSubscriber( aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    
    WRITELOG( "CBackupSubscriber::NewL - end" );
    return self;
    }

// ---------------------------------------------------------------------------
// CBackupSubscriber::ConstructL
// 2nd phase construction.
// ---------------------------------------------------------------------------
//
void CBackupSubscriber::ConstructL()
    {
    WRITELOG( "CBackupSubscriber::ConstructL - begin" ); 
    
	iPropertyWatcher = CPropertyWatcher::GetInstanceL();
    
	// Want to listen when backup/restore starts.
	// Calls NotifyKeyL when key's state has changed. 
	iPropertyWatcher->ListenKeyAndStatusChangesL( 
			KUidSystemCategory,
			conn::KUidBackupRestoreKey,
			this
			);
	
    WRITELOG( "CBackupSubscriber::ConstructL - end" ); 
    }

// ---------------------------------------------------------------------------
// CBackupSubscriber::~CBackupSubscriber
// Destructor.
// ---------------------------------------------------------------------------
//
CBackupSubscriber::~CBackupSubscriber()
    {
    WRITELOG( "CBackupSubscriber::~CBackupSubscriber - begin" ); 
    
	if( iPropertyWatcher )
		{
		iPropertyWatcher->StopListeningKeyAndStatusChanges( 
				KUidSystemCategory,
				conn::KUidBackupRestoreKey, this );
		
		iPropertyWatcher->Delete(); // Release connection to TLS object.
		}
    
	WRITELOG( "CBackupSubscriber::~CBackupSubscriber - end" );
    }

// -----------------------------------------------------------------------------
// CBackupSubscriber::NotifyKeyAndStatusL
// CPropertyWatcher's callback.
// -----------------------------------------------------------------------------
//
void CBackupSubscriber::NotifyKeyAndStatusL( 
		const TInt aKeyValue,
		TRequestStatus& aStatus,
		const TUid aPropertyCategory,
		const TUint aKey )
	{
	WRITELOG("CBackupSubscriber::NotifyKeyAndStatusL() - begin");
	
	if( aPropertyCategory == KUidSystemCategory &&
			aKey == conn::KUidBackupRestoreKey )
		{
		CheckBackupState( aKeyValue,  aStatus );
		}
	
	WRITELOG("CBackupSubscriber::NotifyKeyAndStatusL() - end");
	}


// ---------------------------------------------------------------------------
// CBackupSubscriber::CheckBackupState
// Check Backup & Restore's state key status and act accordingly. 
// ---------------------------------------------------------------------------
//
void CBackupSubscriber::CheckBackupState( 
		const TInt aKeyValue,
		TRequestStatus& aStatus )
    {
    WRITELOG( "CBackupSubscriber::CheckBackupState, begin" );
    
    TInt backupStateValue = aKeyValue;
	backupStateValue &= conn::KBURPartTypeMask;
	WRITELOG1( "CBackupSubscriber::CheckBackupState(), backupStateValue = %d", backupStateValue ); 	
	
	switch ( backupStateValue )
		{
		case conn::EBURBackupFull:
		case conn::EBURBackupPartial:
		case conn::EBURRestoreFull:
		case conn::EBURRestorePartial:
			{
			iObserver.BackupRestoreStart();
			CMdsActiveScheduler* mdsScheduler = static_cast<CMdsActiveScheduler*> (CActiveScheduler::Current());
   			mdsScheduler->SetAllowedAOStatus( aStatus );
			break;
			}
		case conn::EBURNormal:
		case conn::EBURUnset:
		default:
			{
			// backup or restore is completed, so resume normal operation.
       		iObserver.BackupRestoreReady();
			CMdsActiveScheduler* mdsScheduler = static_cast<CMdsActiveScheduler*> (CActiveScheduler::Current());
       		mdsScheduler->RemoveAllowedAOStatus();
			}
		}

    WRITELOG( "CBackupSubscriber::CheckBackupState, end" ); 
    }

// End of file.
