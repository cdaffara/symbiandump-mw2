/*
* Copyright (c) 2003-2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Command Handler Request Manager.
*
*/



// INCLUDES
#include "cphsrvcomhandrequestmanager.h" 
#include "cphsrvcomhandrequest.h" 
#include "cphsrvsubsessionbase.h" 
#include "phsrvutils.h" 
#include "cphsrvserver.h" 
#include "mphsrvemergencynumbermanager.h" 


// CONSTANTS

// The size of the Notification manager storage.
const TInt KPhSrvComHandManagerPendingCallGranularity = 4;


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhSrvComHandRequestManager::CPhSrvComHandRequestManager
// 
// Constructor.
// -----------------------------------------------------------------------------
//
CPhSrvComHandRequestManager::CPhSrvComHandRequestManager( 
    CPhSrvServer& aServer )
:   iServer( aServer )
    {
    }


// -----------------------------------------------------------------------------
// CPhSrvComHandRequestManager::~CPhSrvComHandRequestManager
// 
// Destructor.
// -----------------------------------------------------------------------------
//
CPhSrvComHandRequestManager::~CPhSrvComHandRequestManager()
    {
    if ( iOutstandingComHandRequests )
        {
        iOutstandingComHandRequests->ResetAndDestroy();
        iOutstandingComHandRequests->Close();

        delete iOutstandingComHandRequests;
        }
    }


// -----------------------------------------------------------------------------
// CPhSrvComHandRequestManager::ConstructL
// 
// Complete construction.
// -----------------------------------------------------------------------------
//
void CPhSrvComHandRequestManager::ConstructL()
    {
    iOutstandingComHandRequests = new ( ELeave ) RPointerArray< 
        CPhSrvComHandRequest >( KPhSrvComHandManagerPendingCallGranularity );

    CPhSrvComHandRequest* emergencyRequest = CPhSrvComHandRequest::NewL();
    CleanupStack::PushL( emergencyRequest );
    User::LeaveIfError( 
        iOutstandingComHandRequests->Append( emergencyRequest ) );
    CleanupStack::Pop(); // emergencyRequest
    }


// -----------------------------------------------------------------------------
// CPhSrvComHandRequestManager::NegotiatorRequestL
// 
// Initiate a command handler request - called by the external command handler
// interface.
// -----------------------------------------------------------------------------
//
void CPhSrvComHandRequestManager::NegotiatorRequestL( 
    const RMessage2& aMessage, 
    const TPhCltComHandCommandParameters& aParams, 
    CPhSrvSubSessionBase& aSubSession )
    {
    // If we have no registered initiator then we can't perform the request.
    if ( !iComHandInitiator )
        {
        User::Leave( KErrNotReady );
        }

    // If the request is DTMF stop, it is handled as emergency request.
    TBool isDtmfCancel = 
        ( 
            ( aParams.iCommandHandlerCommand == EPhCltCommandVts ) &&
            ( aParams.iDtmfAction == EPhCltDtmfStop ) 
        );

    // If the request is dial and the number is emergencynumber, it is
    // handled as emergency request.
    TBool isEmergencyCall = 
        ( aParams.iCommandHandlerCommand == EPhCltCommandAtd );
    if( isEmergencyCall )
        {
        isEmergencyCall = iServer.EmergencyNumberManagerL().
        EmergencyNumberManagerIsEmergencyNumberL(
            ( TDes16& ) aParams.iDialData.iTelNumber, 
            EPhCltEmergencyNumberQueryModeNormal );
        }

    // Panic if this is not the only request from this subsession.
    if ( RequestAlreadyExists( aSubSession ) )
        {
        // Cancel all ongoing requests first.
        NegotiatorRequestCancel( aSubSession );

        PhoneServerUtils::PanicClient( 
            EPhoneServerPanicTooManyRequestsOngoing,
            aMessage );
        }

    // Normal request handling, request is ok.
    else
        {
        CPhSrvComHandRequest* temp = NULL;

        // Emergency request handling.
        if ( ( isDtmfCancel || isEmergencyCall ) && 
             ( *iOutstandingComHandRequests )[0]->SubSessionHandle() == 0 )
            {
            ( *iOutstandingComHandRequests )[0]->UpdateEmergencyRequest( 
                aMessage, 
                aParams,
                aSubSession.SubSessionUniqueHandle() );
            temp = ( *iOutstandingComHandRequests )[0];
            }
        else
            {
            // Create pending request
            CPhSrvComHandRequest* request = CPhSrvComHandRequest::NewLC( 
                aMessage, 
                aParams,
                aSubSession.SubSessionUniqueHandle() );

            User::LeaveIfError( 
                iOutstandingComHandRequests->Append( request ) );
            CleanupStack::Pop( request );
            temp = request;
            }

        // Now see if the command handler notifier (initiator) is ready 
        if ( iComHandInitiator->InitiatorIsReadyToPerformRequest() )
            {
            // Ask the call initiator to perform the request 
            // (basically completes the phone app engine's outstanding 
            // ComHand Notifier request).
            iComHandInitiator->InitiatorMakeAsynchronousComHandRequest( 
                aParams );

            iActiveRequest = temp;
            }
        }
    }


// -----------------------------------------------------------------------------
// CPhSrvComHandRequestManager::NegotiatorRequestCancel
// 
// Cancel a pending command handler request.
// -----------------------------------------------------------------------------
//
void CPhSrvComHandRequestManager::NegotiatorRequestCancel( 
    CPhSrvSubSessionBase& aSubSession )
    {
    // Obtain the subsessions handle
    const TInt handle = aSubSession.SubSessionUniqueHandle();

    // Now look for any pending requests that match.
    const TInt count = iOutstandingComHandRequests->Count();
    for( TInt i = 0; i < count; i++ )
        {
        CPhSrvComHandRequest* entry = ( *iOutstandingComHandRequests )[i];
        if ( entry->SubSessionHandle() == handle )
            {
            if ( entry == iActiveRequest )
                {
                // This request is active, so inform that it is deleted.
                iActiveRequest = NULL;
                }

            // Found matching entry, so cancel it and clean up
            if ( i == 0 ) // 0 stands for emergency request
                {
                entry->Cancel();
                entry->ClearEmergencyRequest();
                }
            else
                {
                iOutstandingComHandRequests->Remove( i );
                entry->Cancel();
                delete entry;
                }
            }
        }
    UpdateQueue();
    }


// -----------------------------------------------------------------------------
// CPhSrvComHandRequestManager::SetNegotiatorReadyRequestL
// 
// Updates the negotiator so that it knows if the notifier
// session is able to perform the request.
// Does not leave.
// -----------------------------------------------------------------------------
//
void CPhSrvComHandRequestManager::SetNegotiatorReadyRequestL(
    MPhSrvComHandInitiator& aInitiator )
    {
    // There mustn't already be a command handler initiator, or the existing 
    // initiator must be the same as the new one (otherwise, panic).
    __ASSERT_ALWAYS( !iComHandInitiator || &aInitiator == iComHandInitiator, 
        PhoneServerUtils::Panic( 
        EPhoneServerPanicExistingComHandInitiatorFault ) );

    // Update our pointer
    iComHandInitiator = &aInitiator;

    UpdateQueue();
    }


// -----------------------------------------------------------------------------
// CPhSrvComHandRequestManager::SetNegotiatorReadyRequestCancel
// 
// Indicates that the initiator is no longer available.
// -----------------------------------------------------------------------------
//
void CPhSrvComHandRequestManager::SetNegotiatorReadyRequestCancel( 
    MPhSrvComHandInitiator& aInitiator )
    {
    if ( iComHandInitiator && iComHandInitiator == &aInitiator )
        {
        iComHandInitiator = NULL;
        }
    }

// -----------------------------------------------------------------------------
// CPhSrvComHandRequestManager::SetNegotiatorAttemptedRequestResponseValue
// 
// Handle a response from the phone app engine - this is the 
// result of attempting to perform a command handler request 
// (based upon a previous clients external command handler request).
// -----------------------------------------------------------------------------
//
void CPhSrvComHandRequestManager::SetNegotiatorAttemptedRequestResponseValue( 
    TPhCltPhoneResults aResult )
    {
    // Handle response event only if there is request that waits response.
    if ( iActiveRequest )
        {
        // Get the first item in the list - this is the oldest, and has the
        // details of the command handler request we had previously initiated.
        CPhSrvComHandRequest* head = NULL;
        if ( ( *iOutstandingComHandRequests )[0]->SubSessionHandle() != 0 )
            {
            head = ( *iOutstandingComHandRequests )[0];
            }
        else
            {
            head = ( *iOutstandingComHandRequests )[1];
            }
        // Complete the request so that the external command handler client 
        // knows the result of the request attempt
        head->InformOfComHandRequestAttemptResult( aResult );

        // Delete the pending request (now that it's been processed) and tidy
        // up
        if ( ( *iOutstandingComHandRequests )[0]->SubSessionHandle() != 0 )
            {
            ( *iOutstandingComHandRequests )[0]->ClearEmergencyRequest();
            }
        else
            {
            iOutstandingComHandRequests->Remove( 1 );
            delete head;
            }

        // Reset the iActiveRequest variable.
        iActiveRequest = NULL;
        }
    }


// -----------------------------------------------------------------------------
// CPhSrvComHandRequestManager::UpdateQueue
// 
// Update the request queue.
// -----------------------------------------------------------------------------
//
void CPhSrvComHandRequestManager::UpdateQueue()
    {
    // If we have no registered initiator then we do nothing here.
    if ( iComHandInitiator )
        {
        // If we have more than one item in the queue, then we
        // try and fire of the next item here.
        if ( iComHandInitiator->InitiatorIsReadyToPerformRequest() && 
            ( iOutstandingComHandRequests->Count() > 1 ||
            ( *iOutstandingComHandRequests )[0]->SubSessionHandle() != 0 ) )
            {
            // Get the request at the top of the list (head of the queue)
            CPhSrvComHandRequest* head = NULL;
            if ( ( *iOutstandingComHandRequests )[0]->SubSessionHandle() != 0 )
                {
                head = ( *iOutstandingComHandRequests )[0];
                }
            else
                {
                head = ( *iOutstandingComHandRequests )[1];
                }

            iComHandInitiator->InitiatorMakeAsynchronousComHandRequest(
                head->ComHandParameters() );

            iActiveRequest = head;
            }
        }
    }


// -----------------------------------------------------------------------------
// CPhSrvComHandRequestManager::RequestAlreadyExists
// 
// Check whether there already is request(s) for the subsession given as 
// parameter.
// -----------------------------------------------------------------------------
//
TBool CPhSrvComHandRequestManager::RequestAlreadyExists( 
    CPhSrvSubSessionBase& aSubSession )
    {
    TBool isThereRequest = EFalse;

    // Obtain the subsessions handle
    const TInt handle = aSubSession.SubSessionUniqueHandle();

    // Look for any pending requests that match the handle.
    const TInt count = iOutstandingComHandRequests->Count();
    TInt i = 0;
    for( i = 0; i < count; i++ )
        {
        CPhSrvComHandRequest* entry = 
            ( *iOutstandingComHandRequests )[i];
        if ( entry->SubSessionHandle() == handle )
            {
            isThereRequest = ETrue;
            }
        }
    return isThereRequest;
    }

// End of File
