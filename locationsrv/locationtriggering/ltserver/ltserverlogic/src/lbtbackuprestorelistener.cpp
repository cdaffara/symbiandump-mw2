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
* Description:  Implementation of Backup and Restore event listener
*
*/


// INCLUDE FILES
#include <e32property.h>
#include <startupdomainpskeys.h>
#include <sbdefs.h>
#include "lbtbackuprestorelistener.h"
#include "lbtbackuprestoreobserver.h"
#include "lbtlogger.h"

using namespace conn;

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CLbtBackupRestoreListener::NewL
// Symbian Two - phase constructor
// ---------------------------------------------------------------------------
//
CLbtBackupRestoreListener* CLbtBackupRestoreListener::NewL(MLbtBackupRestoreObserver& aObserver)
    {
    FUNC_ENTER("CLbtBackupRestoreListener::NewL");
    CLbtBackupRestoreListener* self = new ( ELeave ) CLbtBackupRestoreListener(aObserver);
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// ---------------------------------------------------------------------------
// CLbtBackupRestoreListener::~CLbtBackupRestoreListener
// Destructor
// ---------------------------------------------------------------------------
//
CLbtBackupRestoreListener::~CLbtBackupRestoreListener()
	{
	FUNC_ENTER("CLbtBackupRestoreListener::~CLbtBackupRestoreListener");	
	Cancel();
	iBackupRestoreListener.Close();	
	}

// ---------------------------------------------------------------------------
// CLbtBackupRestoreListener::IsBackupRestoreOperationInProgress
// ---------------------------------------------------------------------------
//
TBool CLbtBackupRestoreListener::IsBackupRestoreOperationInProgress()
	{
	FUNC_ENTER("CLbtBackupRestoreListener::IsBackupRestoreOperationInProgress");
	TInt status = 0;
    iBackupRestoreListener.Get( status );
    
    if(status == EBURNormal || status == EBURUnset)
    	{
    	// No operations in progress
    	return EFalse;
    	}
    	
    // Backup or restore operation is in progress
    return ETrue;
	}

// ---------------------------------------------------------------------------
// CLbtBackupRestoreListener::CLbtBackupRestoreListener
// Default C++ Constructor
// ---------------------------------------------------------------------------
//
CLbtBackupRestoreListener::CLbtBackupRestoreListener(MLbtBackupRestoreObserver& aObserver)
		:CActive( EPriorityStandard ), iObserver(aObserver)
	{
	FUNC_ENTER("CLbtBackupRestoreListener::CLbtBackupRestoreListener");
	CActiveScheduler::Add( this );
	}

// ---------------------------------------------------------------------------
// CLbtBackupRestoreListener::ConstructL()
// 2nd phase constuctor for instantiating member variables
// ---------------------------------------------------------------------------
//
void CLbtBackupRestoreListener::ConstructL()
	{
	FUNC_ENTER("CLbtBackupRestoreListener::ConstructL");
	User::LeaveIfError( iBackupRestoreListener.Attach( TUid::Uid(KUidSystemCategoryValue),
                                                 	   KUidBackupRestoreKey, EOwnerThread ) );
	}

// ---------------------------------------------------------------------------
// CLbtBackupRestoreListener::DoCancel()
// Cancels any outstanding request
// ---------------------------------------------------------------------------
//
void CLbtBackupRestoreListener::DoCancel()
	{
	FUNC_ENTER("CLbtBackupRestoreListener::DoCancel");
	iBackupRestoreListener.Cancel();
	}

// ---------------------------------------------------------------------------
// CLbtBackupRestoreListener::StartToListenL()
// Starts to Listen to Backup and restore events
// ---------------------------------------------------------------------------
//
void CLbtBackupRestoreListener::StartToListenL()
	{
	FUNC_ENTER("CLbtBackupRestoreListener::StartToListenL");	
    iBackupRestoreListener.Subscribe( iStatus );
    SetActive();
    
    TInt status = 0;
    User::LeaveIfError( iBackupRestoreListener.Get( status ) );
    
    if(status & EBURBackupFull || 
       status & EBURRestoreFull ||
       status & EBURRestorePartial ||
       status & EBURBackupPartial)
    	{
    	// If backup or restore operation is going on 
    	// then inform observer immediately
    	iObserver.HandleBackupRestoreOperationL(MLbtBackupRestoreObserver::EInProgress);
    	}
	}

// ---------------------------------------------------------------------------
// CLbtBackupRestoreListener::RunL()
// Request completion event handler
// ---------------------------------------------------------------------------
//
void CLbtBackupRestoreListener::RunL()
	{
	FUNC_ENTER("CLbtBackupRestoreListener::RunL");
	iBackupRestoreListener.Subscribe( iStatus );
    SetActive();
    
    TInt status = 0;
    User::LeaveIfError( iBackupRestoreListener.Get( status ) );
    
    if( status & EBURBackupFull || 
        status & EBURBackupPartial ||
        status & EBURRestoreFull ||
    	status & EBURRestorePartial)
    	{
    	// If backup operation is going on then inform
    	// observer immediately
    	iObserver.HandleBackupRestoreOperationL(MLbtBackupRestoreObserver::EStarted);
    	}
    else
    	{
    	// Backup or restore operation has ended so infrom observer
    	iObserver.HandleBackupRestoreOperationL(MLbtBackupRestoreObserver::EEnded);
    	}
	}
// ---------------------------------------------------------------------------
// CLbtBackupRestoreListener::RunError()
// ---------------------------------------------------------------------------
//
TInt CLbtBackupRestoreListener::RunError( TInt /*aError*/ )
    {
    return KErrNone;
    }
//end of file

