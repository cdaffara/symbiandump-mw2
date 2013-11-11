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
* Description:  This module contains the implementation of CNWNetworkCurrentNetworkReceiver 
*                class member functions.
*
*/


//  INCLUDE FILES
#include    "cnwnetworkcurrentnetworkreceiver.h"
#include    "NWPanic.pan"
#include    "NWLogger.h"
#include    "CNWNetworkCurrentNetworkMonitor.h"


// ================= MEMBER FUNCTIONS =========================================

// ----------------------------------------------------------------------------
// CNWNetworkCurrentNetworkReceiver::CNWNetworkCurrentNetworkReceiver
// C++ default constructor can NOT contain any code, that
// might leave.
// ----------------------------------------------------------------------------
//
CNWNetworkCurrentNetworkReceiver::CNWNetworkCurrentNetworkReceiver( 
        CNWNetworkCurrentNetworkMonitor& aOwner, // ref. to owner class
        RMobilePhone& aPhone,
        RMobilePhone::TMobilePhoneNetworkInfoV2Pckg& aNetworkInfoPckg,
        RMobilePhone::TMobilePhoneLocationAreaV1& aLocationArea ) // ref. to mobile phone
         : CActive( EPriorityStandard ),
           iOwner( aOwner ),
           iPhone( aPhone ),
           iNetworkInfoPckg( aNetworkInfoPckg ),//lint !e1403
           iLocationArea( aLocationArea )
    {
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkCurrentNetworkReceiver::\
        CNWNetworkCurrentNetworkReceiver() Begin." );
    
    CActiveScheduler::Add( this );
    
    NWLOGSTRING( KNWOBJECT, 
        "CNWNetworkCurrentNetworkReceiver::\
        CNWNetworkCurrentNetworkReceiver() End" );
    }

// Destructor
CNWNetworkCurrentNetworkReceiver::~CNWNetworkCurrentNetworkReceiver()
    {
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkCurrentNetworkReceiver::\
        ~CNWNetworkCurrentNetworkReceiver() Begin." );
    
    Cancel();    
    
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkCurrentNetworkReceiver::\
        ~CNWNetworkRegistrationStatusMonitor() End." );
    }

// ----------------------------------------------------------------------------
// CNWNetworkCurrentNetworkReceiver::DoCancel
// Cancels the pending async. command.
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void CNWNetworkCurrentNetworkReceiver::DoCancel()
    {
    NWLOGSTRING( KNWREQEND, 
        "NW: CNWNetworkCurrentNetworkReceiver::DoCancel: \
        Request CancelAsyncRequest( \
        EMobilePhoneNotifyNetworkRegistrationStatusChange )" );
    
    iPhone.CancelAsyncRequest( EMobilePhoneGetCurrentNetwork );
    
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkCurrentNetworkReceiver::\
        DoCancel() End." );
    }

// ----------------------------------------------------------------------------
// CNWNetworkRegistrationStatusMonitor::RunL
// Receives the completion of the pending async. command
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void CNWNetworkCurrentNetworkReceiver::RunL()
    {
    NWLOGSTRING2( KNWREQEND,
        "NW: CNWNetworkCurrentNetworkReceiver::RunL() Begin, \
        iStatus = %d", iStatus.Int() );

    if ( iStatus == KErrNone )
        {
        // Cast MMETel types to NW types.
        iOwner.UpdateNetworkInfo(); 
        }
    else
        {
        NWLOGSTRING2( KNWERROR,
            "NW: CNWNetworkCurrentNetworkReceiver::RunL, Server Terminated \
            or request Cancelled, iStatus = %d", iStatus.Int() );
        }
    
    NWLOGSTRING( KNWREQEND,
        "NW: CNWNetworkCurrentNetworkReceiver::RunL() End" );
    }


// ----------------------------------------------------------------------------
// CNWNetworkCurrentNetworkReceiver::IssueRequest
// Executes the async. NotifyNetworkRegistrationStatusChange function.
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void CNWNetworkCurrentNetworkReceiver::IssueRequest()
    {
    NWLOGSTRING( KNWOBJECT,
        "NW: CNWNetworkCurrentNetworkReceiver::IssueRequest() Begin" );
    
    if ( !IsActive() )
        {
         NWLOGSTRING( KNWREQOUT, 
                "NW: CNWNetworkCurrentNetworkMonitor::IssueRequest \
                > RMobilePhone::GetCurrentNetwork" );
        iPhone.GetCurrentNetwork( iStatus, iNetworkInfoPckg, iLocationArea );
        SetActive();
        }
    else 
        {
        NWLOGSTRING( KNWERROR, 
            "NW: CNWNetworkCurrentNetworkReceiver::IssueRequest: Already active" );
        }
    
    NWLOGSTRING( KNWOBJECT,
        "NW: CNWNetworkCurrentNetworkReceiver::IssueRequest() End" );
    }
