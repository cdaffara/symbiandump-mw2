/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Call Request Manager.
*
*/



// INCLUDES
#include "cphsrvcallrequestmanager.h" 
#include "cphsrvcallrequest.h" 
#include "cphsrvsubsessionbase.h" 
#include "phsrvutils.h" 


// CONSTANTS

// The size of the Notification manager storage.
const TInt KPhSrvNotificationManagerPendingCallGranularity = 4;


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhSrvCallRequestManager::CPhSrvCallRequestManager
// 
// C++ constructor.
// -----------------------------------------------------------------------------
//
CPhSrvCallRequestManager::CPhSrvCallRequestManager()
    {
    }


// -----------------------------------------------------------------------------
// CPhSrvCallRequestManager::~CPhSrvCallRequestManager
// 
// C++ destructor.
// -----------------------------------------------------------------------------
//
CPhSrvCallRequestManager::~CPhSrvCallRequestManager()
    {
    if ( iOutstandingRequestsCall )
        {
        iOutstandingRequestsCall->ResetAndDestroy();
        iOutstandingRequestsCall->Close();
        }
    delete iOutstandingRequestsCall;
    }


// -----------------------------------------------------------------------------
// CPhSrvCallRequestManager::ConstructL
// 
// Complete construction.
// -----------------------------------------------------------------------------
//
void CPhSrvCallRequestManager::ConstructL()
    {
    iOutstandingRequestsCall = new ( ELeave ) RPointerArray< 
        CPhSrvCallRequest >( KPhSrvNotificationManagerPendingCallGranularity );

    CPhSrvCallRequest* emergencyCallRequest = CPhSrvCallRequest::NewL();
    CleanupStack::PushL( emergencyCallRequest );
    User::LeaveIfError( 
        iOutstandingRequestsCall->Append( emergencyCallRequest ) );
    CleanupStack::Pop(); // emergencyCallRequest
    }


// -----------------------------------------------------------------------------
// CPhSrvCallRequestManager::NegotiatorDialL
// 
// Initiate a dial request - called by the external call
// interface.
// -----------------------------------------------------------------------------
//
void CPhSrvCallRequestManager::NegotiatorDialL( 
    const RMessage2& aMessage, 
    CPhCltExtPhoneDialData& aArgs,
    CPhSrvSubSessionBase& aSubSession )
    {
    // If we have no registered initiator then we can't make a call.
    if ( !iCallInitiator )
        {
        User::Leave( KErrNotReady );
        }
    
    if (  aArgs.TelephoneNumber() == KPhSrvUsedEmergencyCallNumber && 
         ( *iOutstandingRequestsCall )[0]->SubSessionHandle() == 0 )
        {
        ( *iOutstandingRequestsCall )[0]->UpdateEmercgencyCall( 
            aMessage, 
           aArgs,
            aSubSession.SubSessionUniqueHandle() );         
        }
    else
        {
        // Create pending request
        CPhSrvCallRequest* request = CPhSrvCallRequest::NewLC( 
            aMessage, 
            aArgs,
            aSubSession.SubSessionUniqueHandle() );

        User::LeaveIfError( iOutstandingRequestsCall->Append( request ) );
        CleanupStack::Pop( request );
        }

    // Now see if the call notifier (initiator) is ready 
    if ( iCallInitiator->InitiatorIsReadyToMakeCall() )
        {
        // Ask the call initiator to make the call (basically completes the
        //phone app engine's outstanding call notifier request).
        iCallInitiator->InitiatorMakeAsynchronousCallL( aArgs );
        }
    }


// -----------------------------------------------------------------------------
// CPhSrvCallRequestManager::NegotiatorDialCancel
// 
// Cancel a pending dial request.
// -----------------------------------------------------------------------------
//
void CPhSrvCallRequestManager::NegotiatorDialCancelL( 
    CPhSrvSubSessionBase& aSubSession )
    {
    // Obtain the subsessions handle
    const TInt handle = aSubSession.SubSessionUniqueHandle();

    // Now look for any pending requests that match.
    const TInt count = iOutstandingRequestsCall->Count();
    for( TInt i = 0; i < count; i++ )
        {
        CPhSrvCallRequest* entry = ( *iOutstandingRequestsCall )[i];
        if ( entry->SubSessionHandle() == handle )
            {
            // Found matching entry, so cancel it and clean up
            if ( i == 0 ) // 0 stands for emergency call
                {
                entry->Cancel();
                entry->ClearEmergencyCall();
                }
            else
                {
                iOutstandingRequestsCall->Remove( i );
                entry->Cancel();
                delete entry;
                }

            // If we have more than one item in the queue, then we
            // try and fire of the next item here.
            if ( iCallInitiator->InitiatorIsReadyToMakeCall() && 
                 ( iOutstandingRequestsCall->Count() > 1 ||
                 ( *iOutstandingRequestsCall )[0]->SubSessionHandle() != 0 ) )
                {
                // Get the call at the top of the list (head of the queue)
                CPhSrvCallRequest* head = NULL;
                if ( ( *iOutstandingRequestsCall )[0]->SubSessionHandle() != 0 )
                    {
                    head = ( *iOutstandingRequestsCall )[0];
                    }
                else
                    {
                    head = ( *iOutstandingRequestsCall )[1];
                    }


                iCallInitiator->InitiatorMakeAsynchronousCallL( 
                    head->CallArguments() );
                }

            return;
            }
        }
    }


// -----------------------------------------------------------------------------
// CPhSrvCallRequestManager::SetNegotiatorReadyCall
// 
// Updates the negotiator so that it knows if the notifier
// session is able to make a call.
// -----------------------------------------------------------------------------
//
void CPhSrvCallRequestManager::SetNegotiatorReadyCallL(
    MPhSrvInitiatorCall& aInitiator )
    {
    // There mustn't already be a call initiator, or the existing call
    // initiator must be the same as the new one (otherwise, panic).
    __ASSERT_ALWAYS( !iCallInitiator || &aInitiator == iCallInitiator, 
        PhoneServerUtils::Panic( 
        EPhoneServerPanicExistingCallInitiatorFault ) );

    // Update our pointer
    iCallInitiator = &aInitiator;

    // If we have more than one item in the queue, then we
    // try and fire of the next item here.
    if ( iCallInitiator->InitiatorIsReadyToMakeCall() && 
         ( iOutstandingRequestsCall->Count() > 1 ||
           ( *iOutstandingRequestsCall )[0]->SubSessionHandle() != 0 ) )
        {
        // Get the call at the top of the list (head of the queue)
        CPhSrvCallRequest* head = NULL;
        if ( ( *iOutstandingRequestsCall )[0]->SubSessionHandle() != 0 )
            {
            head = ( *iOutstandingRequestsCall )[0];
            }
        else
            {
            head = ( *iOutstandingRequestsCall )[1];
            }

        iCallInitiator->InitiatorMakeAsynchronousCallL(
            head->CallArguments() );
        }
    }


// -----------------------------------------------------------------------------
// CPhSrvCallRequestManager::SetNegotiatorReadyCallCancel
// 
// Indicates that the initiator is no longer available
// -----------------------------------------------------------------------------
//
void CPhSrvCallRequestManager::SetNegotiatorReadyCallCancel( 
    MPhSrvInitiatorCall& aInitiator )
    {
    if ( iCallInitiator && iCallInitiator == &aInitiator )
        {
        iCallInitiator = NULL;
        }
    }

// -----------------------------------------------------------------------------
// CPhSrvCallRequestManager::SetNegotiatorAttemptedCallResponseValue
// 
// Handle a response from the phone app engine - this is the 
// result of attempting to make a call (based upon a previous
// clients external call request)
// -----------------------------------------------------------------------------
//
void CPhSrvCallRequestManager::SetNegotiatorAttemptedCallResponseValue( 
    TPhCltPhoneResults aResult )
    {
    if ( iOutstandingRequestsCall->Count() > 1 ||
         ( *iOutstandingRequestsCall )[0]->SubSessionHandle() != 0 )
        {
        // Get the first item in the list - this is the oldest, and has the
        // details of the call we had previously initiated.
        CPhSrvCallRequest* head = NULL;
        if ( ( *iOutstandingRequestsCall )[0]->SubSessionHandle() != 0 )
            {
            head = ( *iOutstandingRequestsCall )[0];
            }
        else
            {
            head = ( *iOutstandingRequestsCall )[1];
            }
        // Complete the request so that the external call client knows the
        // result of the call attempt
        head->InformOfCallAttemptResult( aResult );

        // Delete the pending request (now that it's been processed) and tidy
        // up
        if ( ( *iOutstandingRequestsCall )[0]->SubSessionHandle() != 0 )
            {
            ( *iOutstandingRequestsCall )[0]->ClearEmergencyCall();
            }
        else
            {
            iOutstandingRequestsCall->Remove( 1 );
            delete head;
            }
        }
    }


// End of File
