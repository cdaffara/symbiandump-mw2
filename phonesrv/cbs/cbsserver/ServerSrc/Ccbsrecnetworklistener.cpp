/*
* Copyright (c) 2003-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This module contains the implementation of CCbsRecNetworkListener class 
*                member functions.   
*
*/



//  INCLUDES
#include <e32base.h>
#include <e32svr.h>
#include <etelmm.h>
#include "CCbsRecNetworkListener.h"
#include "CbsLogger.h"
 
// CONSTANTS

/// Network listener's priority in active scheduler.
const TInt KCbsRecNetworkListenerPriority = CActive::EPriorityStandard + 5;

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CCbsRecNetworkListener::CCbsRecNetworkListener
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CCbsRecNetworkListener::CCbsRecNetworkListener( 
    RMobilePhone& aPhone )
    : CActive( KCbsRecNetworkListenerPriority ), 
      iNetworkInfoRetrieved( EFalse ), 
      iPhone( aPhone ),
      iNetworkInfoPckg( iNetworkInfo ),
      iGetCurrentNetworkActive( EFalse ),
      iNotifyNwRegChange( EFalse ) 
    {
    }

// -----------------------------------------------------------------------------
// CCbsRecNetworkListener::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCbsRecNetworkListener::ConstructL()
    {
    CActiveScheduler::Add( this );
    IssueGetCurrentNwRequest();
    }

// -----------------------------------------------------------------------------
// CCbsRecNetworkListener::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCbsRecNetworkListener* CCbsRecNetworkListener::NewL( 
    RMobilePhone& aPhone )
    {
    CCbsRecNetworkListener* self = 
        new (ELeave) CCbsRecNetworkListener( aPhone );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// Destructor
CCbsRecNetworkListener::~CCbsRecNetworkListener()
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsRecNetworkListener::~CCbsRecNetworkListener()");
    Cancel();
    CBSLOGSTRING("CBSSERVER: <<< CCbsRecNetworkListener::~CCbsRecNetworkListener()");
    }

// -----------------------------------------------------------------------------
// CCbsRecMessage::GetCurrentNetworkInfo
// Fetches the current networking info.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CCbsRecNetworkListener::GetCurrentNetworkInfo(
    RMobilePhone::TMobilePhoneNetworkInfoV1& aNetworkInfo, 
    RMobilePhone::TMobilePhoneLocationAreaV1& aArea )
    {
    TInt errorCode;
    if ( !iNetworkInfoRetrieved )
        {
        // No network state change notification arrived yet
        errorCode = KErrNotFound;
        }
    else
        {
        // Get both infos from member variables        
        aNetworkInfo = iNetworkInfo;
        aArea = iArea;
        errorCode = KErrNone;
        }
    return errorCode;
    }

// -----------------------------------------------------------------------------
// CCbsRecMessage::RunL
// Called after NotifyCurrentNetworkChange is completed by EPOC Telephony Server.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsRecNetworkListener::RunL()
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsRecNetworkListener::RunL()");
    if ( iGetCurrentNetworkActive )
        {   
        // Current request is getting current active network
        HandleGetCurrentNetworkResult();
        }
    else
        {
        if ( iNotifyNwRegChange )
            {
            // Current request is notifying network registeration change
            HandleNotifyNetworkRegistrationStatusChangeResult();
            }
        else
            {
            // Current request is notifying current network state change
            HandleNotifyCurrentNetworkChangeResult();
            }
        }
    CBSLOGSTRING("CBSSERVER: <<< CCbsRecNetworkListener::RunL()");
    }

// -----------------------------------------------------------------------------
// CCbsRecMessage::DoCancel
// Called whenever the listener has been requested to cancel.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsRecNetworkListener::DoCancel()
    {
    // Cancel current request. Ongoing request can be:
    // GetCurrentNetwork, NotifyNetworkRegistrationStatusChange and 
    // NotifyCurrentNetworkChange.
    
    // Check if the initialization phase was going on
    if ( iGetCurrentNetworkActive )
        {   
        iPhone.CancelAsyncRequest( EMobilePhoneGetCurrentNetwork );
        iGetCurrentNetworkActive = EFalse;
        }
    else
        {
        // Check if the notification of network registration state change
        if ( iNotifyNwRegChange )
            {
            iPhone.CancelAsyncRequest( EMobilePhoneNotifyNetworkRegistrationStatusChange );
            iNotifyNwRegChange = EFalse;
            }
        else
            {
            // Current request is notification of current network change
            iPhone.CancelAsyncRequest( EMobilePhoneNotifyCurrentNetworkChange );
            }    
        }
    }

// -----------------------------------------------------------------------------
// CCbsRecNetworkListener::IssueGetCurrentNwRequest
// Requests getting current network information from EPOC Telephony Server.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsRecNetworkListener::IssueGetCurrentNwRequest()
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsRecNetworkListener::\
                  IssueGetCurrentNwRequest()");
    if ( !IsActive() )
        {
        iPhone.GetCurrentNetwork( iStatus, iNetworkInfoPckg, iArea );
        SetActive();
        iGetCurrentNetworkActive = ETrue;
        }
    else
        {
        CBSLOGSTRING("CBSSERVER: CCbsRecNetworkListener::\
                     IssueGetCurrentNwRequest(), Aready active");
        }
    CBSLOGSTRING("CBSSERVER: <<< CCbsRecNetworkListener::\
                  IssueGetCurrentNwRequest()");
    }

// -----------------------------------------------------------------------------
// CCbsRecNetworkListener::IssueNotifyNwRegChangeRequest
// Requests notification of network registration state change 
// from EPOC Telephony Server.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsRecNetworkListener::IssueNotifyNwRegChangeRequest()
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsRecNetworkListener::\
                  IssueNotifyNwRegChangeRequest()");
    if ( !IsActive() )
        {
        iPhone.NotifyNetworkRegistrationStatusChange( iStatus, iRegistration );
        SetActive();
        iNotifyNwRegChange = ETrue;
        }
    else
        {
        CBSLOGSTRING("CBSSERVER: CCbsRecNetworkListener::\
                     IssueNotifyNwRegChangeRequest(), Aready active");
        }
    CBSLOGSTRING("CBSSERVER: <<< CCbsRecNetworkListener::\
                  IssueNotifyNwChangeRequest()");
    }

// -----------------------------------------------------------------------------
// CCbsRecNetworkListener::IssueNotifyNwChangeRequest
// Requests notification of network state change from EPOC Telephony Server.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsRecNetworkListener::IssueNotifyNwChangeRequest()
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsRecNetworkListener::\
                  IssueNotifyNwChangeRequest");
    if ( !IsActive() )
        {
        iPhone.NotifyCurrentNetworkChange( iStatus, iNetworkInfoPckg, iArea );
        SetActive();
        }
    else
        {
        CBSLOGSTRING("CBSSERVER: CCbsRecNetworkListener::\
                     IssueNotifyNwChangeRequest(), Aready active");
        }
    CBSLOGSTRING("CBSSERVER: <<< CCbsRecNetworkListener::\
                  IssueNotifyNwChangeRequest()");
    }


// -----------------------------------------------------------------------------
// When the current request is GetCurrentNetwork, handle the return result
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsRecNetworkListener::HandleGetCurrentNetworkResult()
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsRecNetworkListener::\
                  HandleGetCurrentNetworkResult");
    iGetCurrentNetworkActive = EFalse;

    if ( KErrNone == iStatus.Int() )
        {
        // Get the network information successfully
        // Submit the notify current network change request.
        iNetworkInfoRetrieved = ETrue;
        IssueNotifyNwChangeRequest();
        }
    else
        {
        // Run into some problems to get network information,
        // submit the request of notification of network registration
        // state change
        CBSLOGSTRING2("CBSSERVER: CCbsRecNetworkListener::\
        HandleGetCurrentNetworkResult, iStatus is %d", iStatus.Int());
        IssueNotifyNwRegChangeRequest();
        } 
    CBSLOGSTRING("CBSSERVER: <<< CCbsRecNetworkListener::\
                  HandleGetCurrentNetworkResult");
    }

// -----------------------------------------------------------------------------
// When the current request is NotifyNetworkRegistrationStatusChange, 
// handle the return result
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsRecNetworkListener::HandleNotifyNetworkRegistrationStatusChangeResult()
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsRecNetworkListener::\
                  HandleNotifyNetworkRegistrationStatusChangeResult()");

    if ( KErrNone == iStatus.Int() )
        {
        CBSLOGSTRING2("CBSSERVER: CCbsRecNetworkListener::\
                       HandleNotifyNetworkRegistrationStatusChangeResult(), \
                       registration status is %d", iRegistration);
  
        if ( ( RMobilePhone::ERegisteredOnHomeNetwork == 
               iRegistration ) ||
             ( RMobilePhone::ERegisteredRoaming == iRegistration ) )
            {
            // ME successfully registered on network, submit GetCurrentNetwork 
            // request to get current network information
            iNotifyNwRegChange = EFalse;
            IssueGetCurrentNwRequest();
            }
        else
            {
            // ME didn't successfully registered on network.
            // Resubmit this request.
            IssueNotifyNwRegChangeRequest();
            }
        }
    else
        {
        CBSLOGSTRING2("CBSSERVER: CCbsRecNetworkListener::\
                       HandleNotifyNetworkRegistrationStatusChangeResult, \
                       iStatus is %d", iStatus.Int() );
        IssueNotifyNwRegChangeRequest();
        }

    CBSLOGSTRING("CBSSERVER: <<< CCbsRecNetworkListener::\
                  HandleNotifyNetworkRegistrationStatusChangeResult()");
    }

// -----------------------------------------------------------------------------
// When the current request is NotifyCurrentNetworkChange, 
// handle the return result
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsRecNetworkListener::HandleNotifyCurrentNetworkChangeResult()
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsRecNetworkListener::\
                  HandleNotifyCurrentNetworkChangeResult()");

    CBSLOGSTRING2("CBSSERVER: CCbsRecNetworkListener::\
                   HandleNotifyCurrentNetworkChangeResult, \
                   iStatus is %d", iStatus.Int());
    iNetworkInfoRetrieved = ( KErrNone == iStatus.Int() ) ? ETrue : EFalse;
    IssueNotifyNwChangeRequest();

    CBSLOGSTRING("CBSSERVER: <<< CCbsRecNetworkListener::\
                  HandleNotifyCurrentNetworkChangeResult()");
    }

//  End of File  
