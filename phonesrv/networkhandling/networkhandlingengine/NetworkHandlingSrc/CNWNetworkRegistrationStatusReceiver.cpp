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
* Description:  This module contains the implementation of CNWNetworkRegistrationStatusReceiver 
*                class member functions.
*
*/


//  INCLUDE FILES
#include    "CNWNetworkRegistrationStatusReceiver.h"
#include    "NWPanic.pan"
#include    "NWLogger.h"
#include    "CNWNetworkRegistrationStatusMonitor.h"


// ================= MEMBER FUNCTIONS =========================================

// ----------------------------------------------------------------------------
// CNWNetworkRegistrationStatusReceiver::CNWNetworkRegistrationStatusReceiver
// C++ default constructor can NOT contain any code, that
// might leave.
// ----------------------------------------------------------------------------
//
CNWNetworkRegistrationStatusReceiver::CNWNetworkRegistrationStatusReceiver( 
        CNWNetworkRegistrationStatusMonitor& aOwner, // ref. to owner class
        RMobilePhone& aPhone,  // ref. to mobile phone
        RMobilePhone::TMobilePhoneRegistrationStatus& aRegistrationStatus ) // ref. to network info struct 
         : CActive( EPriorityStandard ),
           iOwner( aOwner ),
           iPhone( aPhone ),
           iRegistrationStatus( aRegistrationStatus )
    {
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkRegistrationStatusReceiver::\
        CNWNetworkRegistrationStatusReceiver() Begin." );
    
    CActiveScheduler::Add( this );
    
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkRegistrationStatusReceiver::\
        CNWNetworkRegistrationStatusReceiver() End." );
    }
    
    
// ----------------------------------------------------------------------------
// CNWNetworkRegistrationStatusReceiver::ConstructL
// Symbian 2nd phase constructor can leave.
// ----------------------------------------------------------------------------
//
void CNWNetworkRegistrationStatusReceiver::ConstructL()
    {
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkRegistrationStatusReceiver::ConstructL() Called." );
    }

// ----------------------------------------------------------------------------
// CNWNetworkRegistrationStatusReceiver::NewL
// Two-phased constructor.
// ----------------------------------------------------------------------------
//
CNWNetworkRegistrationStatusReceiver* CNWNetworkRegistrationStatusReceiver::NewL( 
        CNWNetworkRegistrationStatusMonitor& aOwner, 
        RMobilePhone& aPhone, 
        RMobilePhone::TMobilePhoneRegistrationStatus& aRegistrationStatus 
        )
    {
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkRegistrationStatusReceiver::NewL() Begin." );

    CNWNetworkRegistrationStatusReceiver* self = 
        new( ELeave ) CNWNetworkRegistrationStatusReceiver( aOwner,
                                                           aPhone, 
                                                           aRegistrationStatus );
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkRegistrationStatusReceiver::NewL() End." );
    
    return self;
    }
    

// Destructor
CNWNetworkRegistrationStatusReceiver::~CNWNetworkRegistrationStatusReceiver()
    {
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkRegistrationStatusReceiver::\
        ~CNWNetworkRegistrationStatusReceiver() Begin." );
    
    Cancel();
    
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkRegistrationStatusReceiver::\
        ~CNWNetworkRegistrationStatusReceiver() End." );
    }

// ----------------------------------------------------------------------------
// CNWNetworkRegistrationStatusReceiver::DoCancel
// Cancels the pending async. command.
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void CNWNetworkRegistrationStatusReceiver::DoCancel()
    {
    NWLOGSTRING( KNWREQEND, 
        "NW: CNWNetworkRegistrationStatusReceiver::DoCancel() Begin,\
        Request CancelAsyncRequest( EMobilePhoneNotifyNetworkRegistra\
        tionStatusChange )" );
    
    iPhone.CancelAsyncRequest( EMobilePhoneGetNetworkRegistrationStatus );
    
    NWLOGSTRING( KNWREQEND, 
        "NW: CNWNetworkRegistrationStatusReceiver::DoCancel() End" );
    }

// ----------------------------------------------------------------------------
// CNWNetworkRegistrationStatusMonitor::RunL
// Receives the completion of the pending async. command,
// saves possible values from async.
// Function call and sends notification to CNWSession object.
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void CNWNetworkRegistrationStatusReceiver::RunL()
    {
    NWLOGSTRING2( KNWREQEND,
        "NW: CNWNetworkRegistrationStatusReceiver::RunL, \
        iStatus = %d", iStatus.Int() );

    if ( iStatus == KErrNone )
        {
        if ( !iRegistrationStatus == RMobilePhone::ERegistrationUnknown )
            {
            // Cast MMETel types to NW types.
            iOwner.UpdateNetworkInfo(); 
            // Update P&S property with registration status
            iOwner.UpdateRegistrationStatusProperty();
            
            NWLOGSTRING2( KNWINT, 
                "NW: CNWNetworkRegistrationStatusReceiver::RunL, \
                iRegistrationStatus = %d (=ETel)", 
                iRegistrationStatus );
            }
        }
    // Check if monitor can be restarted.
    else if ( iStatus != KErrCancel 
              && iStatus != KErrServerTerminated
              && iStatus != KErrNotSupported )
        {
        NWLOGSTRING2( KNWERROR,
            "NW: CNWNetworkRegistrationStatusReceiver::RunL, Unknown \
            error situation, iStatus = %d", iStatus.Int() );
        IssueRequest();
        }
    else
        {
        NWLOGSTRING2( KNWERROR,
            "NW: CNWNetworkRegistrationStatusReceiver::RunL, Server \
            Terminated or request Cancelled, iStatus = %d", iStatus.Int() );
        }
    
    NWLOGSTRING( KNWREQEND,
        "NW: CNWNetworkRegistrationStatusReceiver::RunL() End" );
    }


// ----------------------------------------------------------------------------
// CNWNetworkRegistrationStatusReceiver::IssueRequest
// Executes the async. NotifyNetworkRegistrationStatusChange function.
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void CNWNetworkRegistrationStatusReceiver::IssueRequest()
    {
    NWLOGSTRING( KNWOBJECT,
        "NW: CNWNetworkRegistrationStatusReceiver::IssueRequest() Begin");
    
    if ( !IsActive() )
        {
        NWLOGSTRING( KNWREQIN, 
                "NW:CNWNetworkRegistrationStatusReceiver::IssueRequest \
                >RMobilePhone::GetNetworkRegistrationStatus" );
        iPhone.GetNetworkRegistrationStatus( iStatus, iRegistrationStatus );
        SetActive();
        }
    else 
        {
        NWLOGSTRING( KNWERROR, 
            "NW: CNWNetworkRegistrationStatusReceiver::IssueRequest: \
            Already active" );
        }

    NWLOGSTRING( KNWOBJECT,
        "NW: CNWNetworkRegistrationStatusReceiver::IssueRequest() End");
    }



//  End of File 
