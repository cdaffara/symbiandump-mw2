/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Sub Session Notifier.
*
*/



// INCLUDE FILES
#include <phclttypes.h> 
#include "cphsrvsubsessionnotifier.h" 
#include "cphsrvsession.h" 
#include "cphsrvserver.h" 
#include "mphsrvemergencynegotiators.h" 
#include "mphsrvobjectnegotiators.h" 
#include "mphsrvcomhandnegotiators.h" 

// CONSTANTS
// None.

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhSrvSubSessionNotifier::CPhSrvSubSessionNotifier
// 
// C++ Constructor
// -----------------------------------------------------------------------------
//
CPhSrvSubSessionNotifier::CPhSrvSubSessionNotifier( CPhSrvSession& aSession )
    :   CPhSrvSubSessionBase( aSession, EPhSrvSubSessionTypeCallNotifier ),
        iTelNumberPckg( iTelNumber )
    {
    
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionNotifier::~CPhSrvSubSessionNotifier
// 
// C++ Destructor
// -----------------------------------------------------------------------------
//
CPhSrvSubSessionNotifier::~CPhSrvSubSessionNotifier()
    {
    if ( PhoneSession().PhoneServer().IsCallNegotiator() )
        {
        PhoneSession().
            PhoneServer().
                CallNegotiator().
                    SetNegotiatorReadyCallCancel( *this ); 
        }   
    if ( PhoneSession().PhoneServer().IsEmergencyCallNegotiator() )
        {
        PhoneSession().
            PhoneServer().
                EmergencyCallNegotiator().
                    SetNegotiatorReadyCallCancel( *this );
        }
    if ( PhoneSession().PhoneServer().IsComHandNegotiator() )
        {
        PhoneSession().
            PhoneServer().
                ComHandNegotiator().
                    SetNegotiatorReadyRequestCancel( *this );
        
        }
    delete iDialData;
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionNotifier::ConstructL
// 
// Symbian OS 2nd phase constructor
// -----------------------------------------------------------------------------
//
/*****************************************************
*   Series 60 Customer / ETel
*   Series 60  ETel API
*****************************************************/
void CPhSrvSubSessionNotifier::ConstructL()
    {
    CPhSrvSubSessionBase::ConstructL();
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionNotifier::PhSrvMessageDecoderCanProcessMessage
// 
// Framework function. Returns ETrue for the op codes that
// this subsession can handle.
// -----------------------------------------------------------------------------
//
TBool CPhSrvSubSessionNotifier::PhSrvMessageDecoderCanProcessMessage(
        TInt aFunction )
    {
    switch( aFunction )
        {
        case EPhoneServerNotifySubSessionClose:
        case EPhoneServerNotifySubSessionNotifyCallRequest:
        case EPhoneServerNotifySubSessionNotifyCallRequestCancel:
        case EPhoneServerNotifySubSessionReportCallResult:
        case EPhoneServerNotifySubSessionDialData:

        case EPhoneServerNotifyEmergencySubSessionNotifyCallRequest:
        case EPhoneServerNotifyEmergencySubSessionNotifyCallRequestCancel:
        case EPhoneServerNotifyEmergencySubSessionReportCallResult:

        case EPhoneServerComHandNotifySubSessionComHandRequest:
        case EPhoneServerComHandNotifySubSessionComHandRequestCancel:
        case EPhoneServerComHandNotifySubSessionReportComHandResult:

            return ETrue;

        default:
            return EFalse;
        }   
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionNotifier::PhSrvMessageProcessorHandleMessageL
// 
// Framework function. Handle any op-codes here.
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionNotifier::PhSrvMessageProcessorHandleMessageL(
    const RMessage2& aMessage )
    {
    switch( aMessage.Function() )
        {
        case EPhoneServerNotifySubSessionClose:
            CmdSubSessionCloseL( aMessage );
            break;
        case EPhoneServerNotifySubSessionNotifyCallRequest:
            CmdSubSessionNotifyCallRequestL( aMessage );
            break;
        case EPhoneServerNotifySubSessionNotifyCallRequestCancel:
            CmdSubSessionNotifyCallRequestCancelL( aMessage );
            break;
        case EPhoneServerNotifySubSessionReportCallResult:
            CmdSubSessionReportCallResultL( aMessage );
            break;
        case EPhoneServerNotifySubSessionDialData:
            CmdSubSessionDialDataL( aMessage );
            break;
        case EPhoneServerNotifyEmergencySubSessionNotifyCallRequest:
            CmdSubSessionNotifyEmergencyL( aMessage );
            break;
        case EPhoneServerNotifyEmergencySubSessionNotifyCallRequestCancel:
            CmdSubSessionNotifyEmergencyCancel( aMessage );
            break;
        case EPhoneServerNotifyEmergencySubSessionReportCallResult:
            CmdSubSessionReportEmergencyCallResultL( aMessage );
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
// CPhSrvSubSessionNotifier::InitiatorMakeAsynchronousCall
// 
// Called by the phone call negotiator when it wants this
// object to initiate a call (on behalf of the external
// interface).
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionNotifier::InitiatorMakeAsynchronousCallL(
      CPhCltExtPhoneDialData& aArgs )
    {
    // data length to send
    const TInt dataLength = aArgs.CalculatePackageSize();
    
    const TPckg< TInt > pDataLength( dataLength );
    //save dial data  
    CmdSubSessionCloneDialDataL( aArgs );

    PhoneSession().Write(
    iPendingDialRequestPointer,
    0,
    pDataLength );   

    // Indicate that the phone notifier should try and make the call.
    if ( !iPendingDialRequestPointer.IsNull() )
        {
        iPendingDialRequestPointer.Complete( KErrNone );
        }
    iHavePendingDialRequestPointer = EFalse;
    
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionNotifier::InitiatorIsReadyToMakeCall
// 
// Does this session have a pending request?
// -----------------------------------------------------------------------------
//
TBool CPhSrvSubSessionNotifier::InitiatorIsReadyToMakeCall() const
    {
    return iHavePendingDialRequestPointer;
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionNotifier::CmdSubSessionCloseL
// 
// Close the sub-session
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionNotifier::CmdSubSessionCloseL(
    const RMessage2& aMessage )
    {
    PhoneSession().CloseSubSession( aMessage );
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionNotifier::CmdSubSessionNotifyCallRequestL
// 
// Setup an asynchronous request to handle external interface
// call requests.
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionNotifier::CmdSubSessionNotifyCallRequestL(
    const RMessage2& aMessage )
    {
    // Update our pending request pointer if it's not already set
    if  ( iHavePendingDialRequestPointer )
        {
        PhoneSession().PanicClient(
            aMessage,
            EPhCltServerInitiatedPanicRequestAlreadyActive );
        }
    else
        {
        iPendingDialRequestPointer = aMessage;
        iHavePendingDialRequestPointer = ETrue;

        // The phone app engine is ready to process a call request. 
        // Tell the negotiator that we're ready to process any pending 
        // requests.
        MPhSrvNegotiatorCall& callNegotiator = 
            PhoneSession().PhoneServer().CallNegotiatorL();
        callNegotiator.SetNegotiatorReadyCallL( *this );
        }
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionNotifier::CmdSubSessionNotifyCallRequestCancelL
// 
// Cancel an outstanding request to handle call initiations.
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionNotifier::CmdSubSessionNotifyCallRequestCancelL(
    const RMessage2& aMessage )
    {
    if  ( !iHavePendingDialRequestPointer )
        {
        PhoneSession().PanicClient(
            aMessage,
            EPhCltServerInitiatedPanicRequestNotActive );
        }
    else
        {
        if ( !iPendingDialRequestPointer.IsNull() )
            {
            iPendingDialRequestPointer.Complete( KErrCancel );
            }
        iHavePendingDialRequestPointer = EFalse;
        }

    aMessage.Complete( KErrNone );
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionNotifier::CmdSubSessionReportCallResultL
// 
// Tell the external call interface client the result of 
// dialing a call.
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionNotifier::CmdSubSessionReportCallResultL(
    const RMessage2& aMessage )
    {
    // Get result
    const TPhCltPhoneResults result = 
        static_cast< TPhCltPhoneResults >( aMessage.Int0() );

    // Tell call negotiator of result
    MPhSrvNegotiatorCall& callNegotiator = 
        PhoneSession().PhoneServer().CallNegotiatorL();
    callNegotiator.SetNegotiatorAttemptedCallResponseValue( result );

    // Complete the request
    aMessage.Complete( KErrNone );
    }

// -----------------------------------------------------------------------------
// CPhSrvSubSessionNotifier::CmdSubSessionCloneDialData
// 
// Clones call arguments to object
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionNotifier::CmdSubSessionCloneDialDataL(
 CPhCltExtPhoneDialData& aArgs )
    {
    iDialData = CPhCltExtPhoneDialData::NewL();
    iDialData->SetTelephoneNumber( aArgs.TelephoneNumber() );
    iDialData->SetCallType( aArgs.CallType() );
    iDialData->SetNameL( aArgs.Name() );
    iDialData->SetWindowGroup( aArgs.WindowGroup() );
    iDialData->SetRedial( aArgs.Redial() );
    iDialData->SetRedialMaximumDuration( aArgs.RedialMaximumDuration() );
    iDialData->SetContactLinkL( aArgs.ContactLink() );
    iDialData->SetSubAddressL( aArgs.SubAddress() );
    iDialData->SetShowNumber( aArgs.ShowNumber() );
    iDialData->SetAllowMatch( aArgs.AllowMatch() );
    iDialData->SetSATCall( aArgs.SATCall() );
    iDialData->SetEndOtherCalls( aArgs.EndOtherCalls() );
    iDialData->SetBearerL( aArgs.Bearer() );
    iDialData->SetSendKeyPressed( aArgs.SendKeyPressed() );
    iDialData->SetInitiateCall( aArgs.InitiateCall() );
    iDialData->SetServiceId( aArgs.ServiceId() );
    iDialData->SetUUIL( aArgs.UUI() );
    }
    
// -----------------------------------------------------------------------------
// CPhSrvSubSessionNotifier::CmdSubSessionDialDataL
// 
// Tell the external call interface client the result of 
// dialing a call.
// -----------------------------------------------------------------------------
//  
void CPhSrvSubSessionNotifier::CmdSubSessionDialDataL ( 
    const RMessage2& aMessage )
    {
    HBufC8* package = NULL;
    package = iDialData->PackLC();
    if ( package )
        {
        PhoneSession().Write(
        aMessage,
        0,
       *package );
        aMessage.Complete( KErrNone );   
        CleanupStack::PopAndDestroy( package );
        }
    delete iDialData;
    iDialData = NULL;
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionNotifier::InitiatorMakeAsynchronousCall
// 
// Called by the phone call negotiator when it wants this
// object to initiate a call (on behalf of the external
// interface).
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionNotifier::InitiatorMakeEmergencyCall( const RMessage2& aMessage )
    {
    
    // Write to phone notifier's address space
    PhoneSession().Read(
        aMessage,
        0,
         iTelNumberPckg );
    
    PhoneSession().Write(
        iPendingEmergencyRequestPointer,
        0,
        iTelNumberPckg );
        
    // Indicate that the phone notifier should try and make the call.
    if ( !iPendingEmergencyRequestPointer.IsNull() )
        {
        iPendingEmergencyRequestPointer.Complete( KErrNone );
        }
    iHavePendingEmergencyRequestPointer = EFalse;
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionNotifier::InitiatorIsReadyToMakeCall
// 
// Does this session have a pending request?
// -----------------------------------------------------------------------------
//
TBool CPhSrvSubSessionNotifier::InitiatorIsReadyToMakeEmergencyCall() const
    {
    return iHavePendingEmergencyRequestPointer;
    }

// -----------------------------------------------------------------------------
// CPhSrvSubSessionNotifier::CmdSubSessionNotifyCallRequestL
// 
// Setup an asynchronous request to handle external interface
// call requests.
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionNotifier::CmdSubSessionNotifyEmergencyL(
    const RMessage2& aMessage )
    {
    // Update our pending request pointer if it's not already set
    if  ( iHavePendingEmergencyRequestPointer )
        {
        PhoneSession().PanicClient(
            aMessage,
            EPhCltServerInitiatedPanicRequestAlreadyActive );
        }
    else
        {
        iPendingEmergencyRequestPointer = aMessage;
        iHavePendingEmergencyRequestPointer = ETrue;

        // The phone app engine is ready to process a call request. 
        // Tell the negotiator that we're ready to process any pending 
        // requests.
        MPhSrvEmergencyNegotiatorCall& emergencyNegotiator = 
            PhoneSession().PhoneServer().EmergencyCallNegotiatorL();
        emergencyNegotiator.SetNegotiatorReadyCall( *this );
        }
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionNotifier::CmdSubSessionNotifyCallRequestCancelL
// 
// Cancel an outstanding request to handle call initiations.
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionNotifier::CmdSubSessionNotifyEmergencyCancel(
    const RMessage2& aMessage )
    {
    if  ( !iHavePendingEmergencyRequestPointer )
        {
        PhoneSession().PanicClient(
            aMessage,
            EPhCltServerInitiatedPanicRequestNotActive );
        }
    else
        {
        if ( !iPendingEmergencyRequestPointer.IsNull() )
            {
            iPendingEmergencyRequestPointer.Complete( KErrCancel );
            }
        iHavePendingEmergencyRequestPointer = EFalse;
        }

    aMessage.Complete( KErrNone );
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionNotifier::CmdSubSessionReportEmergencyCallResultL
// 
// Tell the external call interface client the result of 
// dialing a call.
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionNotifier::CmdSubSessionReportEmergencyCallResultL(
    const RMessage2& aMessage )
    {
    // Get result
    const TPhCltPhoneResults result = 
        static_cast< TPhCltPhoneResults >( aMessage.Int0() );

    // Tell call negotiator of result
    MPhSrvEmergencyNegotiatorCall& emergencyNegotiator = 
        PhoneSession().PhoneServer().EmergencyCallNegotiatorL();
    emergencyNegotiator.SetNegotiatorAttemptedCallResponseValue( result );

    // Complete the request
    aMessage.Complete( KErrNone );
    }

// -----------------------------------------------------------------------------
// CPhSrvSubSessionNotifier::InitiatorMakeAsynchronousComHandRequest
// 
// Called by the command handler negotiator when it wants this object to 
// initiate a command handler request (on behalf of the external interface).
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionNotifier::
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
// CPhSrvSubSessionNotifier::InitiatorIsReadyToPerformRequest
// 
// Does this session have a pending request?
// -----------------------------------------------------------------------------
//
TBool CPhSrvSubSessionNotifier::InitiatorIsReadyToPerformRequest() 
    const
    {
    return iHavePendingComHandRequestPointer;
    }

// -----------------------------------------------------------------------------
// CPhSrvSubSessionNotifier::CmdSubSessionComHandNotifyRequestL
// 
// Handle command handler notifier's request.
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionNotifier::CmdSubSessionComHandNotifyRequestL( 
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
// CPhSrvSubSessionNotifier::
// CmdSubSessionComHandNotifyRequestCancelL
// 
// Handle command handler notifier's request cancel.
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionNotifier::
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
// CPhSrvSubSessionNotifier::CmdSubSessionComHandNotifyReportL
// 
// Tell the result of the request execution to the command handler client.
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionNotifier::CmdSubSessionComHandNotifyReportL( 
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
