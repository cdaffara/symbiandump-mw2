/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This module contains the implementation of 
*               CNWDynamicCapsMonitor
*
*/

//  INCLUDE FILES
#include    <pcktcs.h>                         // for EPacketGetDynamicCaps
#include    <etelpckt.h>                       // for RPacketService
#include    "CNWDynamicCapsMonitor.h"
#include    "CNWMessageHandler.h"
#include    "NWLogger.h"

// ================= MEMBER FUNCTIONS =======================================

// -----------------------------------------------------------------------------
// CNWDynamicCapsMonitor::CNWDynamicCapsMonitor
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CNWDynamicCapsMonitor::CNWDynamicCapsMonitor( 
        CNWMessageHandler& aOwner, // ref. to owner class
        RMobilePhone& aPhone,      // ref. to phone
        TNWInfo& aNetworkInfo,     // ref. to network info struct 
        RMmCustomAPI& aCustomAPI ) //ref. to customAPI
         : CNWNetworkMonitorBase( aOwner,
                                  aPhone,
                                  aNetworkInfo,
                                  aCustomAPI )
    {
    NWLOGSTRING( KNWOBJECT, "NW: CNWDynamicCapsMonitor::CNWDynamicCapsMonitor -START" );
    CActiveScheduler::Add( this );
    }
    
    
// -----------------------------------------------------------------------------
// CNWDynamicCapsMonitor::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CNWDynamicCapsMonitor::ConstructL()
    {
    NWLOGSTRING( KNWOBJECT, "NW: CNWDynamicCapsMonitor::ConstructL -START" );
    User::LeaveIfError( iPacketService.Open( iPhone ) );
    // Assign lower priority than other networkhandling monitors.
    // (Default in CNWNetworkMonitorBase is EPriorityStandard)
    SetPriority( EPriorityIdle );
    
    NWLOGSTRING( KNWOBJECT, "NW: CNWDynamicCapsMonitor::ConstructL -END" );
    }

// -----------------------------------------------------------------------------
// CNWDynamicCapsMonitor::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CNWDynamicCapsMonitor* CNWDynamicCapsMonitor::NewL( 
        CNWMessageHandler& aOwner, 
        RMobilePhone& aPhone, 
        TNWInfo& aNetworkInfo, 
        RMmCustomAPI& aCustomAPI )
    {
    NWLOGSTRING( KNWOBJECT, "NW: CNWDynamicCapsMonitor::NewL -START" );
    CNWDynamicCapsMonitor* self = 
        new( ELeave ) CNWDynamicCapsMonitor( aOwner,
                                             aPhone, 
                                             aNetworkInfo,
                                             aCustomAPI );
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    NWLOGSTRING( KNWOBJECT, "NW: CNWDynamicCapsMonitor::NewL -END" );
    return self;
    }
    

// Destructor
CNWDynamicCapsMonitor::~CNWDynamicCapsMonitor()
    {
    Cancel();    
    iPacketService.Close();
    
    NWLOGSTRING( KNWOBJECT, "NW: CNWDynamicCapsMonitor::~CNWDynamicCapsMonitor -END" );
    }

// -----------------------------------------------------------------------------
// CNWDynamicCapsMonitor::DoCancel
// Cancels the pending async. command.
// -----------------------------------------------------------------------------
//
void CNWDynamicCapsMonitor::DoCancel()
    {
    NWLOGSTRING( KNWREQEND, "NW: CNWDynamicCapsMonitor::DoCancel: Request CancelAsyncRequest( EPacketNotifyDynamicCapsChange )" );
    iPacketService.CancelAsyncRequest( EPacketNotifyDynamicCapsChange );
    }

// -----------------------------------------------------------------------------
// CNWDynamicCapsMonitor::RunL
// Receives notification that the dynamic packet data capabilities changed.
// Saves new value in TNWInfo object. (This will be used by active idle to
// hide the alpha tag if CS registration is unsuccessful.)
// -----------------------------------------------------------------------------
//
void CNWDynamicCapsMonitor::RunL()
    {
    NWLOGSTRING2( KNWREQEND,
        "NW: CNWDynamicCapsMonitor::RunL, iStatus = %d", iStatus.Int() );
    
    if( iStatus.Int() == KErrNone )
        {
        if ( iNWNetworkInfo.iDynamicCapsFlags != iDynamicCapsFlags )
            {
            // Update dynamic caps info
            UpdateDynamicCapsFlags( iDynamicCapsFlags );
            iOwner.SendMessage( 
                MNWMessageObserver::ENWMessageDynamicCapsChange );
            }
        IssueRequest(); 
        }
        // Check if monitor can be restarted.
    else if ( iStatus != KErrCancel && iStatus != KErrServerTerminated )
        {
        NWLOGSTRING2( KNWERROR,
            "NW: CNWDynamicCapsMonitor::RunL, Unknown error situation, iStatus = %d", iStatus.Int() );
        IssueRequest();
        }
    else
        {
        NWLOGSTRING2( KNWERROR,
            "NW: CNWDynamicCapsMonitor::RunL, Server Terminated or request Cancelled, iStatus = %d", iStatus.Int() );
        }
     }


// -----------------------------------------------------------------------------
// CNWDynamicCapsMonitor::IssueRequest
// Request a notification when the dynamic packet data capabilities change.
// -----------------------------------------------------------------------------
//
void CNWDynamicCapsMonitor::IssueRequest()
    {
    NWLOGSTRING( KNWOBJECT, "NW: CNWDynamicCapsMonitor::IssueRequest() -START" );
    
    if( !IsActive() )
        {
        NWLOGSTRING( KNWREQIN, 
                        "NW:CNWDynamicCapsMonitor::IssueRequest > RPacketService::NotifyDynamicCapsChange" );
        iPacketService.NotifyDynamicCapsChange( iStatus, iDynamicCapsFlags );
        SetActive();
        }
    else 
        {
        NWLOGSTRING( KNWERROR, 
            "NW: CNWDynamicCapsMonitor::IssueRequest: Already active" );
        }
    
    NWLOGSTRING( KNWOBJECT, "NW: CNWDynamicCapsMonitor::IssueRequest() -END" );
    }
 

// -----------------------------------------------------------------------------
// CNWDynamicCapsMonitor::UpdateDynamicCapsFlags()
// Update the value of iNWNetworkInfo.iDynamicCapsFlags
// -----------------------------------------------------------------------------
//
void CNWDynamicCapsMonitor::UpdateDynamicCapsFlags( 
        const RPacketService::TDynamicCapsFlags aDynamicCapsFlags )
    {
    NWLOGSTRING( KNWOBJECT, "NW: CNWDynamicCapsMonitor::UpdateDynamicCapsFlags()" );
    iNWNetworkInfo.iDynamicCapsFlags = aDynamicCapsFlags;
    }


// -----------------------------------------------------------------------------
// CNWDynamicCapsMonitor::RunError()
// Processes any errors.
// -----------------------------------------------------------------------------
//
TInt CNWDynamicCapsMonitor::RunError( TInt /*aError*/ )
    {
    return KErrNone;    // Handled the error fully
    }

// end of file
