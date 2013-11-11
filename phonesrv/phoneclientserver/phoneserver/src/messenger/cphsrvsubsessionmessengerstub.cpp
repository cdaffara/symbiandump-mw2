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
* Description:  Sub Session Messenger.
*                The stub version of CPhSrvSubSessionMessenger.cpp
*
*/



// INCLUDE FILES
#include "cphsrvsubsessionmessenger.h" 
#include "phcltclientserver.h" 

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhSrvSubSessionMessenger::CPhSrvSubSessionMessenger
// 
// Constructor.
// -----------------------------------------------------------------------------
//
CPhSrvSubSessionMessenger::CPhSrvSubSessionMessenger( 
    CPhSrvSession& aSession )
:   CPhSrvSubSessionBase( aSession, EPhSrvSubSessionTypeMessenger )
    {
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionMessenger::~CPhSrvSubSessionMessenger
// 
// Destructor.
// -----------------------------------------------------------------------------
//
CPhSrvSubSessionMessenger::~CPhSrvSubSessionMessenger()
    {
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionMessenger::NewL
// 
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPhSrvSubSessionBase* CPhSrvSubSessionMessenger::NewL(
    CPhSrvSession& /*aSession*/ )
    {
    // Stub version, so not supported.
    User::Leave( KErrNotSupported );

    return NULL;
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionMessenger::PhSrvMessageDecoderCanProcessMessage
// 
// Framework function. Returns ETrue for the op codes that
// this subsession can handle.
// -----------------------------------------------------------------------------
//
TBool CPhSrvSubSessionMessenger::PhSrvMessageDecoderCanProcessMessage( 
    TInt aFunction )
    {
    switch( aFunction )
        {
        case EPhoneServerMessengerSubSessionOpen:
        case EPhoneServerMessengerSubSessionClose:
        case EPhoneServerMessengerSubSessionConstruct:
        case EPhoneServerMessengerSubSessionReceive:
        case EPhoneServerMessengerSubSessionSend:
        case EPhoneServerMessengerSubSessionSkip:
        case EPhoneServerMessengerSubSessionCancel:
            return ETrue;
            
        default:
            return EFalse;
        }
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionMessenger::PhSrvMessageProcessorHandleMessageL
// 
// Framework function. Handle any op-codes here.
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionMessenger::PhSrvMessageProcessorHandleMessageL( 
    const RMessage2& /*aMessage*/ )
    {
    // Stub implementation, i.e. functionality not supported.
    User::Leave( KErrNotSupported );
    }


// End of File
