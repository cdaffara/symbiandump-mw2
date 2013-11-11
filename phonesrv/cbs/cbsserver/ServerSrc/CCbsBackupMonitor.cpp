/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This module contains the implementation of CCbsBackupMonitor class 
*                member functions.    
*
*/


// INCLUDE FILES
#include "CCbsBackupMonitor.h"
#include <connect/sbdefs.h>
#include "CbsLogger.h"

// CONSTANTS
const TInt KCbsBackupMask = 0xF;

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CCbsBackupMonitor::CCbsBackupMonitor
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CCbsBackupMonitor::CCbsBackupMonitor( CCbsDbImp& aDbImp )
    : CActive( EPriorityStandard ),
    iDbImp( aDbImp )
    {
    }

// -----------------------------------------------------------------------------
// CCbsBackupMonitor::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCbsBackupMonitor::ConstructL()
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsBackupMonitor::ConstructL()");
    
    CActiveScheduler::Add( this );
    
    // Attach to backup property    
    iProperty.Attach( KUidSystemCategory, conn::KUidBackupRestoreKey );    
    IssueRequest();
    
    CBSLOGSTRING("CBSSERVER: <<< CCbsBackupMonitor::ConstructL()");
    }

// -----------------------------------------------------------------------------
// CCbsBackupMonitor::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCbsBackupMonitor* CCbsBackupMonitor::NewL( CCbsDbImp& aDbImp )
    {
    CCbsBackupMonitor* self = 
        new ( ELeave ) CCbsBackupMonitor( aDbImp );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }
    
// Destructor
CCbsBackupMonitor::~CCbsBackupMonitor()
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsBackupMonitor::~CCbsBackupMonitor()");
    Cancel();
	iProperty.Close();
	CBSLOGSTRING("CBSSERVER: <<< CCbsBackupMonitor::~CCbsBackupMonitor()");
    }

// -----------------------------------------------------------------------------
// CCbsBackupMonitor::IssueRequest
// Requests that we are notified when the system state changes.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsBackupMonitor::IssueRequest() 
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsBackupMonitor::IssueRequest()");
    
    if ( !IsActive() )
        {
        // Subscribe again
    	iProperty.Subscribe( iStatus );
	    SetActive();
	    CBSLOGSTRING("CBSSERVER: CCbsBackupMonitor::IssueRequest(): New Subscribe() set active.");
        }
    CBSLOGSTRING("CBSSERVER: <<< CCbsBackupMonitor::IssueRequest()");
    }

// -----------------------------------------------------------------------------
// CCbsBackupMonitor::RunL
// Becomes active when the system state changes.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsBackupMonitor::RunL()
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsBackupMonitor::RunL()");
    
    if ( iStatus == KErrNone )
        {        
        // Resubscribe before processing new value to prevent missing updates        
        IssueRequest();
        
        // Property updated, get new value
    	TInt state( 0 );
    	TInt backupState( 0 );
    	TInt error( KErrNone );
    	error = iProperty.Get( state );
    	CBSLOGSTRING2("CBSSERVER: CCbsBackupMonitor::RunL(): error: %d", error );
    	
    	if ( !error )
    	    {    	       	    
    	    backupState = ( state &= KCbsBackupMask );
    	    
        	// States are from sbdefs.h
        	switch ( backupState )
        	    {
        	    // No backup or restore ongoing
        	    case conn::EBURUnset:
        	    case conn::EBURNormal:
        	        {
        	        CBSLOGSTRING("CBSSERVER: CCbsBackupMonitor::RunL(): EBURUnset or EBURNormal" );
        	        iDbImp.ChangeFileLocksL( ECbsNoBackup );
        	        }    	        
        	        break;
        	    // Backup, release file locks
        	    case conn::EBURBackupFull:
        	    case conn::EBURBackupPartial:
        	        {
        	        CBSLOGSTRING("CBSSERVER: CCbsBackupMonitor::RunL(): EBURBackupFull or EBURBackupPartial" );
        	        iDbImp.ChangeFileLocksL( ECbsBackup );
        	        }    	        
        	        break;
        	    // Restore, release file locks
        	    case conn::EBURRestoreFull:
        	    case conn::EBURRestorePartial:
        	        {
        	        CBSLOGSTRING("CBSSERVER: CCbsBackupMonitor::RunL(): EBURRestoreFull or EBURRestorePartial" );
        	        iDbImp.ChangeFileLocksL( ECbsRestore );
        	        }
        	        break;
        	    default:
        	        CBSLOGSTRING("CBSSERVER: CCbsBackupMonitor::RunL(): Default" );
        	        break;
        	    }
    	    }
        }
    CBSLOGSTRING("CBSSERVER: <<< CCbsBackupMonitor::RunL()");
    }

// -----------------------------------------------------------------------------
// CCbsBackupMonitor::DoCancel
// Cancels an outstanding Subscribe request.  
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsBackupMonitor::DoCancel()
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsBackupMonitor::DoCancel()");
    iProperty.Cancel();   
    CBSLOGSTRING("CBSSERVER: <<< CCbsBackupMonitor::DoCancel()");
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File  
