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
* Description:  Stub implementation for Sub Session Command handler notify.
*
*/



// INCLUDE FILES
#include "cphsrvsubsessionbase.h" 
#include "cphsrvsession.h" 
#include "cphsrvserver.h" 
#include <phclttypes.h> 
#include "mphsrvcomhandnegotiators.h" 
#include "cphsrvsubsessioncommandhandlernotify.h" 



// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CPhSrvSubSessionCommandHandlerNotify::
//     CPhSrvSubSessionCommandHandlerNotify
// 
// Constructor.
// -----------------------------------------------------------------------------
//
CPhSrvSubSessionCommandHandlerNotify::CPhSrvSubSessionCommandHandlerNotify( 
    CPhSrvSession& aSession )
:   CPhSrvSubSessionBase( aSession, EPhSrvSubSessionTypeComHandNotify )
    {
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionCommandHandlerNotify::NewL
// 
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPhSrvSubSessionBase* 
CPhSrvSubSessionCommandHandlerNotify::NewL(
    CPhSrvSession& aSession )
    {
    CPhSrvSubSessionBase* self = 
        new( ELeave ) CPhSrvSubSessionCommandHandlerNotify( aSession );
    
    return self;
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionCommandHandlerNotify::~CPhSrvSubSessionCommandHandlerNotify
// 
// Destructor.
// -----------------------------------------------------------------------------
//
CPhSrvSubSessionCommandHandlerNotify::~CPhSrvSubSessionCommandHandlerNotify()
    {
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionCommandHandlerNotify::PhSrvMessageDecoderCanProcessMessage
// 
// Framework function. Returns ETrue for the op codes that
// this subsession can handle.
// -----------------------------------------------------------------------------
//
TBool
CPhSrvSubSessionCommandHandlerNotify::PhSrvMessageDecoderCanProcessMessage(
    TInt aFunction )
    {
    switch( aFunction )
        {
        case EPhoneServerComHandNotifySubSessionClose:
        case EPhoneServerComHandNotifySubSessionComHandRequest:
        case EPhoneServerComHandNotifySubSessionComHandRequestCancel:
        case EPhoneServerComHandNotifySubSessionReportComHandResult:
            return ETrue;
            
        default:
            return EFalse;
        }
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionCommandHandlerNotify::PhSrvMessageProcessorHandleMessageL
// 
// Framework function. Handle any op-codes here.
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionCommandHandlerNotify::PhSrvMessageProcessorHandleMessageL(
    const RMessage2& /* aMessage */ )
    {
    // Stub implementation, i.e. functionality not supported.
    User::Leave( KErrNotSupported );
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionCommandHandlerNotify::InitiatorMakeAsynchronousComHandRequest
// 
// Called by the command handler negotiator when it wants this object to 
// initiate a command handler request (on behalf of the external interface).
// -----------------------------------------------------------------------------
//
void 
CPhSrvSubSessionCommandHandlerNotify::InitiatorMakeAsynchronousComHandRequest(
    const TPhCltComHandCommandParameters& /* aParams */ )
    {
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionCommandHandlerNotify::InitiatorIsReadyToPerformRequest
// 
// Does this session have a pending request?
// -----------------------------------------------------------------------------
//
TBool
CPhSrvSubSessionCommandHandlerNotify::InitiatorIsReadyToPerformRequest() const
    {
    return EFalse;
    }




// End of File
