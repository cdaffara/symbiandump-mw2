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
#include "cphsrvemergencyrequestmanager.h" 
#include "cphsrvsubsessionbase.h" 
#include "phsrvutils.h" 


// CONSTANTS

// The size of the Notification manager storage.
//const TInt KPhSrvNotificationManagerPendingCallGranularity = 4;


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhSrvEmergencyRequestManager::CPhSrvEmergencyRequestManager
// 
// C++ constructor.
// -----------------------------------------------------------------------------
//
CPhSrvEmergencyRequestManager::CPhSrvEmergencyRequestManager()
    : iIsRequestPending( EFalse )
    {
    }


// -----------------------------------------------------------------------------
// CPhSrvEmergencyRequestManager::~CPhSrvEmergencyRequestManager
// 
// C++ destructor.
// -----------------------------------------------------------------------------
//
CPhSrvEmergencyRequestManager::~CPhSrvEmergencyRequestManager()
    {
    }


// -----------------------------------------------------------------------------
// CPhSrvEmergencyRequestManager::ConstructL
// 
// Complete construction.
// -----------------------------------------------------------------------------
//
void CPhSrvEmergencyRequestManager::ConstructL()
    {
    }


// -----------------------------------------------------------------------------
// CPhSrvEmergencyRequestManager::EmergencyDialL
// 
// Initiate a dial request - called by the external call
// interface. Checks if emergency initiator is ready and sets request pending if 
// emergency initiator hasn't been registered yet 
// -----------------------------------------------------------------------------
//
void CPhSrvEmergencyRequestManager::EmergencyDialL( 
    const RMessage2& aMessage )
    {
    // If we have no registered initiator then we can't make a call.
    if ( !iEmergencyInitiator )
        {
        if( iIsRequestPending )
            {
            User::Leave( KErrAlreadyExists );
            }
        else
            {
            iPendingRequest = aMessage;
            iIsRequestPending = ETrue;
            }
        }
    else
        {
        DoMakeEmergencyDial( aMessage );
        }
    }


// -----------------------------------------------------------------------------
// CPhSrvEmergencyRequestManager::DoMakeEmergencyDial
// 
// Do the actual emergency dial request for emergency initiator.
// -----------------------------------------------------------------------------
//
void CPhSrvEmergencyRequestManager::DoMakeEmergencyDial( const RMessage2& aMessage )
    {
    iPendingRequestPointer= aMessage;
    
    // Clear pending request status 
    iIsRequestPending = EFalse;
    
    // Now see if the call notifier (initiator) is ready 
    if ( iEmergencyInitiator ->InitiatorIsReadyToMakeEmergencyCall() )
        {
        // Ask the call initiator to make the call (basically completes the
        //phone app engine's outstanding call notifier request).
        iEmergencyInitiator ->InitiatorMakeEmergencyCall( aMessage );
        }
    }


// -----------------------------------------------------------------------------
// CPhSrvEmergencyRequestManager::EmergencyDialCancel
// 
// Cancel a pending dial request.
// -----------------------------------------------------------------------------
//
void CPhSrvEmergencyRequestManager::EmergencyDialCancel()
    {
    // Clear pending request status 
    iIsRequestPending = EFalse;
    
    if ( !iPendingRequestPointer.IsNull() )
        {
        iPendingRequestPointer.Complete( KErrCancel );
        }
    }


// -----------------------------------------------------------------------------
// CPhSrvEmergencyRequestManager::SetNegotiatorReadyCall
// 
// Updates the negotiator so that it knows if the notifier
// session is able to make a call.
// -----------------------------------------------------------------------------
//
void CPhSrvEmergencyRequestManager::SetNegotiatorReadyCall(
    MPhSrvEmergencyCall& aInitiator )
    {
    // There mustn't already be a call initiator, or the existing call
    // initiator must be the same as the new one (otherwise, panic).
    __ASSERT_ALWAYS( !iEmergencyInitiator || &aInitiator == iEmergencyInitiator, 
        PhoneServerUtils::Panic( 
        EPhoneServerPanicExistingCallInitiatorFault ) );

    // Update our pointer
    iEmergencyInitiator  = &aInitiator;
    
    if( iIsRequestPending )
        {
        DoMakeEmergencyDial( iPendingRequest );
        }
    }


// -----------------------------------------------------------------------------
// CPhSrvEmergencyRequestManager::SetNegotiatorReadyCallCancel
// 
// Indicates that the initiator is no longer available
// -----------------------------------------------------------------------------
//
void CPhSrvEmergencyRequestManager::SetNegotiatorReadyCallCancel( 
    MPhSrvEmergencyCall& aInitiator )
    {
    if ( iEmergencyInitiator && iEmergencyInitiator == &aInitiator )
        {
        iEmergencyInitiator = NULL;
        }
    }


// -----------------------------------------------------------------------------
// CPhSrvEmergencyRequestManager::SetNegotiatorAttemptedCallResponseValue
// 
// Handle a response from the phone app engine - this is the 
// result of attempting to make a call (based upon a previous
// clients external call request)
// -----------------------------------------------------------------------------
//
void CPhSrvEmergencyRequestManager::SetNegotiatorAttemptedCallResponseValue( 
    TPhCltPhoneResults aResult )
    {
    if ( !iPendingRequestPointer.IsNull() )
        {
        iPendingRequestPointer.Complete( aResult );
        }
    }


// End of File

