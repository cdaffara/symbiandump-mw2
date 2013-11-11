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
* Description:  Sub Session Command handler notify.
*                The stub version of this file can be found from file
*                CPhSrvSubSessionCommandHandlerNotifyStub.cpp
*
*/



// INCLUDE FILES
#include    "cphsrvsubsessioncommandhandlernotify.h" 
#include    "cphsrvsession.h" 
#include    "cphsrvserver.h" 
#include    <phclttypes.h> 
#include    "mphsrvcomhandnegotiators.h" 




// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CPhSrvSubSessionCommandHandlerNotify::CPhSrvSubSessionCommandHandlerNotify
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
    // Just to make sure that the destructor does not leave.
    aSession.PhoneServer().ComHandNegotiatorL();

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
    // Inform that we are not ready to process requests anymore.
    if ( PhoneSession().PhoneServer().IsComHandNegotiator() )
        {
        PhoneSession().
            PhoneServer().
                ComHandNegotiator().
                    SetNegotiatorReadyRequestCancel( *this );
        }
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
    const RMessage2& aMessage )
    {
    switch( aMessage.Function() )
        {
        case EPhoneServerComHandNotifySubSessionClose:
            CmdSubSessionClose( aMessage );
            break;

        case EPhoneServerComHandNotifySubSessionComHandRequest:
            CmdSubSessionComHandNotifyRequestL( aMessage );
            break;

        case EPhoneServerComHandNotifySubSessionComHandRequestCancel:
            CmdSubSessionComHandNotifyRequestCancelL( aMessage );
            break;

        case EPhoneServerComHandNotifySubSessionReportComHandResult:
            CmdSubSessionComHandNotifyReportL( aMessage );
            break;
            
        default:
            PhoneSession().PanicClient(
                aMessage,
                EPhCltServerInitiatedPanicInvalidRequest );
            break;
        }
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionCommandHandlerNotify::InitiatorMakeAsynchronousComHandRequest
// 
// Called by the command handler negotiator when it wants this object to 
// initiate a command handler request (on behalf of the external interface).
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionCommandHandlerNotify::
    InitiatorMakeAsynchronousComHandRequest( 
    const TPhCltComHandCommandParameters& aParams )
    {
    // Package up the data to send
    TPckgC< TPhCltComHandCommandParameters > 
        comHandDetails( aParams );

    // Write to command handler notifier's address space
    PhoneSession().Write( 
        iPendingComHandRequestPointer,
        0,
        comHandDetails );

    // Indicate that the command handler notifier should try to perform the 
    // request.
    if ( !iPendingComHandRequestPointer.IsNull() )
        {
        iPendingComHandRequestPointer.Complete( KErrNone );
        }
    iHavePendingComHandRequestPointer = EFalse;
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionCommandHandlerNotify::InitiatorIsReadyToPerformRequest
// 
// Does this session have a pending request?
// -----------------------------------------------------------------------------
//
TBool CPhSrvSubSessionCommandHandlerNotify::InitiatorIsReadyToPerformRequest() 
    const
    {
    return iHavePendingComHandRequestPointer;
    }



// -----------------------------------------------------------------------------
// CPhSrvSubSessionCommandHandlerNotify::CmdSubSessionClose
// 
// Close the sub-session.
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionCommandHandlerNotify::CmdSubSessionClose(
    const RMessage2& aMessage )
    {
    PhoneSession().CloseSubSession( aMessage );
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionCommandHandlerNotify::CmdSubSessionComHandNotifyRequestL
// 
// Handle command handler notifier's request.
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionCommandHandlerNotify::CmdSubSessionComHandNotifyRequestL( 
    const RMessage2& aMessage )
    {
    // Update our pending request pointer if it's not already set
    if  ( iHavePendingComHandRequestPointer )
        {
        PhoneSession().PanicClient(
            aMessage,
            EPhCltServerInitiatedPanicRequestAlreadyActive );
        }
    else
        {
        iPendingComHandRequestPointer = aMessage;
        iHavePendingComHandRequestPointer = ETrue;

        // The phone app engine is ready to process a command handler request.
        // Tell the negotiator that we're ready to process any pending 
        // requests.
        MPhSrvComHandNegotiator& comHandNegotiator = 
            PhoneSession().PhoneServer().ComHandNegotiatorL();
        comHandNegotiator.SetNegotiatorReadyRequestL( *this );
        }
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionCommandHandlerNotify::
// CmdSubSessionComHandNotifyRequestCancelL
// 
// Handle command handler notifier's request cancel.
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionCommandHandlerNotify::
    CmdSubSessionComHandNotifyRequestCancelL( 
        const RMessage2& aMessage )
    {
    if  ( iHavePendingComHandRequestPointer )
        {
        iPendingComHandRequestPointer.Complete( KErrCancel );
        iHavePendingComHandRequestPointer = EFalse;
        }

    aMessage.Complete( KErrNone );
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionCommandHandlerNotify::CmdSubSessionComHandNotifyReportL
// 
// Tell the result of the request execution to the command handler client.
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionCommandHandlerNotify::CmdSubSessionComHandNotifyReportL( 
    const RMessage2& aMessage )
    {
    // Get result
    const TPhCltPhoneResults result = 
        static_cast< TPhCltPhoneResults >( aMessage.Int0() );

    // Tell command handler negotiator of result
    MPhSrvComHandNegotiator& comHandNegotiator = 
        PhoneSession().PhoneServer().ComHandNegotiatorL();
    comHandNegotiator.SetNegotiatorAttemptedRequestResponseValue( result );

    // Complete the request
    aMessage.Complete( KErrNone );
    }


// End of File
