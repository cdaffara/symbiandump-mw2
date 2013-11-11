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
* Description:  This module contains the implementation of CCbsShutdownMonitor class 
*                member functions.    
*
*/


// INCLUDE FILES
#include <e32property.h>
#include "CCbsShutdownMonitor.h"
#include "CCbsServer.h"
#include <startupdomainpskeys.h>
#include "CbsLogger.h"

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CCbsShutdownMonitor::CCbsShutdownMonitor
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CCbsShutdownMonitor::CCbsShutdownMonitor( CCbsServer& aServer )
    : CActive( EPriorityStandard ),
    iServer( aServer )
    {
    }

// -----------------------------------------------------------------------------
// CCbsShutdownMonitor::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCbsShutdownMonitor::ConstructL()
    {
    CActiveScheduler::Add( this );
    
    // Attach to startup property
    iProperty.Attach( KPSUidStartup, KPSGlobalSystemState );    
    IssueRequest();
    }

// -----------------------------------------------------------------------------
// CCbsShutdownMonitor::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCbsShutdownMonitor* CCbsShutdownMonitor::NewL( CCbsServer& aServer )
    {
    CCbsShutdownMonitor* self = 
        new ( ELeave ) CCbsShutdownMonitor( aServer );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }
    
// Destructor
CCbsShutdownMonitor::~CCbsShutdownMonitor()
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsShutdownMonitor::~CCbsShutdownMonitor()"); 
    Cancel();
	iProperty.Close();
	CBSLOGSTRING("CBSSERVER: <<< CCbsShutdownMonitor::~CCbsShutdownMonitor()"); 
    }

// -----------------------------------------------------------------------------
// CCbsShutdownMonitor::IssueRequest
// Requests that we are notified when the system state changes.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsShutdownMonitor::IssueRequest() 
    {
    if ( !IsActive() )
        {
        // Subscribe again
    	iProperty.Subscribe( iStatus );
	    SetActive();
        }
    }

// -----------------------------------------------------------------------------
// CCbsShutdownMonitor::RunL
// Becomes active when the system state changes.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsShutdownMonitor::RunL()
    {
    if ( iStatus == KErrNone )
        {
        // Resubscribe before processing new value to prevent missing updates        
        IssueRequest();
        
        // Property updated, get new value
    	TInt state( 0 );
    	TInt error( KErrNone );
    	error = iProperty.Get( state );
    	
    	if ( !error )
    	    {
#ifdef RD_STARTUP_CHANGE
        	if ( state == ESwStateShuttingDown )
        		{
        		iServer.Shutdown();
        		}
#endif   	    
    	    }
        }        
    }

// -----------------------------------------------------------------------------
// CCbsShutdownMonitor::DoCancel
// Cancels an outstanding Subscribe request.  
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsShutdownMonitor::DoCancel()
    {
    iProperty.Cancel();   
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File  
