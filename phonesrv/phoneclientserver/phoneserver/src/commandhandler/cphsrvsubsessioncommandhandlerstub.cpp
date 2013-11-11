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
* Description:  Sub implemetation for Sub Session Command handler.
*
*/



// INCLUDE FILES
#include "cphsrvsubsessionbase.h" 
#include "cphsrvsession.h" 
#include "cphsrvserver.h" 
#include <phclttypes.h> 
#include "mphsrvcomhandnegotiators.h" 
#include "cphsrvsubsessioncommandhandler.h" 



// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhSrvSubSessionCommandHandler::CPhSrvSubSessionCommandHandler
// 
// Constructor.
// -----------------------------------------------------------------------------
//
CPhSrvSubSessionCommandHandler::CPhSrvSubSessionCommandHandler( 
    CPhSrvSession& aSession )
:   CPhSrvSubSessionBase( aSession, EPhSrvSubSessionTypeComHand )
    {
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionCommandHandler::NewL
// 
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPhSrvSubSessionBase* CPhSrvSubSessionCommandHandler::NewL(
    CPhSrvSession& aSession )
    {
    CPhSrvSubSessionBase* self = 
        new( ELeave ) CPhSrvSubSessionCommandHandler( aSession );
    
    return self;
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionCommandHandler::PhSrvMessageDecoderCanProcessMessage
// 
// Framework function. Returns ETrue for the op codes that
// this subsession can handle.
// -----------------------------------------------------------------------------
//
TBool CPhSrvSubSessionCommandHandler::PhSrvMessageDecoderCanProcessMessage(
    TInt aFunction )
    {
    switch( aFunction )
        {
        case EPhoneServerComHandSubSessionOpen:
        case EPhoneServerComHandSubSessionClose:
        case EPhoneServerComHandSubSessionAtd:
        case EPhoneServerComHandSubSessionAta:
        case EPhoneServerComHandSubSessionChld:
        case EPhoneServerComHandSubSessionChup:
        case EPhoneServerComHandSubSessionVts:
        case EPhoneServerComHandSubSessionCancel:
        case EPhoneServerComHandSubSessionMuteMic:
        case EPhoneServerComHandSubSessionMuteRingingTone:
            return ETrue;
            
        default:
            return EFalse;
        }
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionCommandHandler::PhSrvMessageProcessorHandleMessageL
// 
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionCommandHandler::PhSrvMessageProcessorHandleMessageL(
    const RMessage2& /* aMessage */ )
    {
    // Stub implementation, i.e. functionality not supported.
    User::Leave( KErrNotSupported );
    }


// End of File
