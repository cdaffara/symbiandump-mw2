/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This module contains the implementation of CNWNetworkFailureMonitor
*                class member functions.
*
*/



// INCLUDE FILES
#include    "CNWNetworkFailureMonitor.h"
#include    "CNWGsmMessageHandler.h"
#include    "NWLogger.h"


// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// CNWNetworkFailureMonitor::CNWNetworkFailureMonitor
// C++ default constructor can NOT contain any code, that
// might leave.
// ----------------------------------------------------------------------------
//
CNWNetworkFailureMonitor::CNWNetworkFailureMonitor(
        CNWGsmMessageHandler& aOwner,
        RMobilePhone& aPhone,
        TNWInfo& aNetworkInfo,
        RMmCustomAPI& aCustomAPI )
        : CNWNetworkMonitorBase( aOwner,
                                 aPhone,
                                 aNetworkInfo,
                                 aCustomAPI )
    {
    NWLOGSTRING( KNWOBJECT, "NW CNWNetworkFailureMonitor::\
            CNWNetworkFailureMonitor() Begin " );
    
    CActiveScheduler::Add( this );
    
    NWLOGSTRING( KNWOBJECT, "NW CNWNetworkFailureMonitor::\
        CNWNetworkFailureMonitor() End " );
    }

    
// Destructor
CNWNetworkFailureMonitor::~CNWNetworkFailureMonitor()
    {
    NWLOGSTRING( KNWOBJECT, "NW: CNWNetworkFailureMonitor::\
        ~CNWNetworkFailureMonitor start." );
    
    Cancel();
    
    NWLOGSTRING( KNWOBJECT, "NW: CNWNetworkFailureMonitor::\
        ~CNWNetworkFailureMonitor complete." );
    }


// ----------------------------------------------------------------------------
// CNWNetworkFailureMonitor::DoCancel
// Cancels the pending async. command. 
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void CNWNetworkFailureMonitor::DoCancel()
    {
    NWLOGSTRING( KNWREQEND, "NW CNWNetworkFailureMonitor::DoCancel() Begin" );
    NWLOGSTRING( KNWREQEND, "NW RMobilePhone::CancelAsyncRequest\
        ( EMobilePhoneNotifyModeChange )" );
        
    iCustomAPI.CancelAsyncRequest( ECustomNotifyNetworkConnectionFailureIPC  );
    
    NWLOGSTRING( KNWREQEND, "NW CNWNetworkFailureMonitor::DoCancel() End" );
    }

// ----------------------------------------------------------------------------
// CNWNetworkFailureMonitor::RunL
// Receives the completion of the pending async. command, saves possible values
// from async. function call and sends notification to client object.
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void CNWNetworkFailureMonitor::RunL()
    {
    NWLOGSTRING2( KNWREQEND,
        "NW: CNWNetworkFailureMonitor::RunL() Begin, iStatus = %d",
        iStatus.Int());

    if ( iStatus == KErrNone )
        {
        iOwner.SendMessage ( 
            MNWMessageObserver::ENWMessageNetworkConnectionFailure );
        }
    else
        {    
        NWLOGSTRING2( KNWERROR,
            "NW: CNWNetworkFailureMonitor::RunL, Unknown error \
            situation, iStatus = %d", iStatus.Int());
        }

    NWLOGSTRING( KNWREQEND, "NW CNWNetworkFailureMonitor::RunL() End" );
    }


// ----------------------------------------------------------------------------
// CNWNetworkFailureMonitor::IssueRequest
// Executes the async. NotifyNetworkConnectionFailure
// Initialised the network failure monitor, if is not yet initialised.
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void CNWNetworkFailureMonitor::IssueRequest()
    {
    NWLOGSTRING( KNWREQEND, 
        "NW CNWNetworkFailureMonitor::IssueRequest() Begin " );
    if ( !IsActive() )
        {
        // Start monitoring network status changes.
        NWLOGSTRING( KNWREQOUT,
            "NW: CNWNetworkFailureMonitor::IssueRequest: \
            Request RMobilePhone::NotifyModeChange" );
        iCustomAPI.NotifyNetworkConnectionFailure( iStatus );
        SetActive();
        }
    else 
        {
        NWLOGSTRING( KNWERROR,
            "NW: CNWNetworkFailureMonitor::IssueRequest: Already active" );
        }
    
    NWLOGSTRING( KNWREQEND, 
        "NW CNWNetworkFailureMonitor::IssueRequest() End " );
    }


//  End of File  
