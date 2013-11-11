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
* Description:  This module contains the implementation of CCbsDiskSpaceMonitor class 
*                member functions.    
*
*/


// INCLUDE FILES
#include <e32property.h>
#include "CCbsDiskSpaceMonitor.h"
#include <UikonInternalPSKeys.h>    // KPSUidUikon, KUikFFSFreeLevel
#include "CCbsDbImp.H"
#include "CbsLogger.h"

// CONSTANTS

// Value to receive from critical level notification for "OK"
const TInt KCriticalLevelDiskOk = 0;


// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CCbsDiskSpaceMonitor::CCbsDiskSpaceMonitor
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CCbsDiskSpaceMonitor::CCbsDiskSpaceMonitor( CCbsDbImp& aDbImp )
    : CActive( EPriorityStandard ),
    iDbImp( aDbImp )
    {
    }

// -----------------------------------------------------------------------------
// CCbsDiskSpaceMonitor::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCbsDiskSpaceMonitor::ConstructL()
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsDiskSpaceMonitor::ConstructL()");
    
    CActiveScheduler::Add( this );
    
    // Attach to disk space property
    iProperty.Attach( KPSUidUikon, KUikFFSFreeLevel );
    
    CBSLOGSTRING("CBSSERVER: <<< CCbsDiskSpaceMonitor::ConstructL()");
    }

// -----------------------------------------------------------------------------
// CCbsDiskSpaceMonitor::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCbsDiskSpaceMonitor* CCbsDiskSpaceMonitor::NewL( CCbsDbImp& aDbImp )
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsDiskSpaceMonitor::NewL()");
    
    CCbsDiskSpaceMonitor* self = 
        new ( ELeave ) CCbsDiskSpaceMonitor( aDbImp );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    
    CBSLOGSTRING("CBSSERVER: <<< CCbsDiskSpaceMonitor::NewL(), returning self.");
    return self;
    }
    
// Destructor
CCbsDiskSpaceMonitor::~CCbsDiskSpaceMonitor()
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsDiskSpaceMonitor::~CCbsDiskSpaceMonitor()");
    Cancel();
	iProperty.Close();
	CBSLOGSTRING("CBSSERVER: <<< CCbsDiskSpaceMonitor::~CCbsDiskSpaceMonitor()");
    }

// -----------------------------------------------------------------------------
// CCbsDiskSpaceMonitor::IssueRequest
// Requests that we are notified when the system state changes.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDiskSpaceMonitor::IssueRequest() 
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsDiskSpaceMonitor::IssueRequest()");
    
    if ( !IsActive() )
        {        
        // Subscribe again
    	iProperty.Subscribe( iStatus );
	    SetActive();
	    CBSLOGSTRING("CBSSERVER: CCbsDiskSpaceMonitor::IssueRequest(): New Subscribe() set active.");
        }
        
    CBSLOGSTRING("CBSSERVER: <<< CCbsDiskSpaceMonitor::IssueRequest()");
    }

// -----------------------------------------------------------------------------
// CCbsDiskSpaceMonitor::RunL
// Becomes active when the system state changes.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDiskSpaceMonitor::RunL()
    {
    CBSLOGSTRING2("CBSSERVER: >>> CCbsDiskSpaceMonitor::RunL(): iStatus: %d", iStatus.Int() );
    
    if ( iStatus == KErrNone )
        {        
        // Property updated, get new value
    	TInt state( 0 );
    	TInt error( KErrNone );
    	error = iProperty.Get( state );
    	CBSLOGSTRING2("CBSSERVER: CCbsDiskSpaceMonitor::RunL(): property.Get() error: %d", error );
    	CBSLOGSTRING2("CBSSERVER: CCbsDiskSpaceMonitor::RunL(): property.Get() state: %d", state );
    	
    	// If we received DISK_SPACE_OK, try to create the database
        // components.
        if ( state == KCriticalLevelDiskOk )
            {
            CBSLOGSTRING("CBSSERVER: CCbsDiskSpaceMonitor::RunL(): Calling CreateComponentsL()..." );
            iDbImp.CreateComponentsL();
            CBSLOGSTRING("CBSSERVER: CCbsDiskSpaceMonitor::RunL(): CreateComponentsL() finished." );
            }
        // To prevent ARMV5 compiler error
        if ( error )
            {
            error = KErrNone;
            }            
        }
    CBSLOGSTRING("CBSSERVER: <<< CCbsDiskSpaceMonitor::RunL()");        
    }

// -----------------------------------------------------------------------------
// CCbsDiskSpaceMonitor::DoCancel
// Cancels an outstanding Subscribe request.  
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDiskSpaceMonitor::DoCancel()
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsDiskSpaceMonitor::DoCancel()");
    iProperty.Cancel();  
    CBSLOGSTRING("CBSSERVER: <<< CCbsDiskSpaceMonitor::DoCancel()"); 
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File  
