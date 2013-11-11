/*
* Copyright (c) 2004-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
*                The stub version of this file can be found from file
*                CPhSrvSubSessionMessengerStub.cpp
*
*/



// INCLUDE FILES
#include    "cphsrvsubsessionmessenger.h" 
#include    "cphsrvsession.h" 
#include    "cphsrvserver.h" 
#include    <phclttypes.h> 
#include    "mphsrvmessengernegotiators.h" 
#include    "phsrvmessengertypes.h" 



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
    Clean();
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionMessenger::NewL
// 
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPhSrvSubSessionBase* CPhSrvSubSessionMessenger::NewL(
    CPhSrvSession& aSession )
    {
    // Just to make sure that the negotiator will exist.
    aSession.PhoneServer().MessengerNegotiatorL();

    CPhSrvSubSessionBase* self = 
        new( ELeave ) CPhSrvSubSessionMessenger( aSession );
    
    return self;
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
    const RMessage2& aMessage )
    {
    TBool permissionStatus ( EFalse );
     
    // Check needed capabilities    
    TBool hasCapWriteDeviceData( EFalse );                  
    hasCapWriteDeviceData = aMessage.HasCapability( ECapabilityWriteDeviceData );            

    switch( aMessage.Function() )
        {
        case EPhoneServerMessengerSubSessionClose:
            CmdSubSessionClose( aMessage );
            permissionStatus = ETrue;
            break;

        case EPhoneServerMessengerSubSessionConstruct:
            CmdSubSessionMessengerConstructL( aMessage );
            permissionStatus = ETrue;
            break;

        case EPhoneServerMessengerSubSessionReceive:
            if ( hasCapWriteDeviceData )
                {  
                CmdSubSessionMessengerReceive( aMessage );
                permissionStatus = ETrue;
                }
            break;
        case EPhoneServerMessengerSubSessionSend:
            if ( hasCapWriteDeviceData )
                {  
                CmdSubSessionMessengerSend( aMessage );
                permissionStatus = ETrue;
                }
            break;
        case EPhoneServerMessengerSubSessionSkip:
            if ( hasCapWriteDeviceData )
                {  
                CmdSubSessionMessengerSkip( aMessage );
                permissionStatus = ETrue;
                }
            break;
        case EPhoneServerMessengerSubSessionCancel:
            if ( hasCapWriteDeviceData )
                {  
                CmdSubSessionMessengerCancel( aMessage );
                permissionStatus = ETrue;
                }
            break;

        default:
            PhoneSession().PanicClient(
                aMessage,
                EPhCltServerInitiatedPanicInvalidRequest );
            break;
        }
        
    // If caps are not ok
    if ( !permissionStatus )
        {        
        aMessage.Complete( KErrPermissionDenied );
        }         
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionMessenger::CmdSubSessionClose
// 
// Close the sub-session.
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionMessenger::CmdSubSessionClose( 
    const RMessage2& aMessage )
    {
    Clean();

    PhoneSession().CloseSubSession( aMessage );
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionMessenger::CmdSubSessionMessengerConstructL
// 
// 
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionMessenger::CmdSubSessionMessengerConstructL( 
    const RMessage2& aMessage )
    {
    TPhCltMessengerParameters parameters;

    // Check the Construct parameters.
    TPckg< TPhCltMessengerParameters > recParPckg( 
        parameters );
    Read(
        aMessage,
        0,
        recParPckg );

    // Ask the messenger negotiator to reserve all needed space
    // for this subsession.
    MPhSrvMessengerNegotiator& messengerNegotiator = GetMessengerNegotiator();

    messengerNegotiator.NegotiatorRequestConstructL( 
        *this,
        parameters,
        aMessage );
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionMessenger::CmdSubSessionMessengerReceive
// 
// 
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionMessenger::CmdSubSessionMessengerReceive( 
    const RMessage2& aMessage )
    {

    TPhCltMessengerParameters parameters;

    // Check the Receive parameters.
    TPckg< TPhCltMessengerParameters > recParPckg( 
        parameters );
    Read(
        aMessage,
        0,
        recParPckg );

    // This is Receive request.
    parameters.iMessengerCommand = EPhCltMesCommandReceive;

    // Ask the messenger negotiator to handle this request.
    ForwardRequest( 
        *this,
        parameters,
        aMessage );
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionMessenger::CmdSubSessionMessengerSend
// 
// 
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionMessenger::CmdSubSessionMessengerSend( 
    const RMessage2& aMessage )
    {
    TPhCltMessengerParameters parameters;

    // Check the Send parameters.
    TPckg< TPhCltMessengerParameters > sendParPckg( 
        parameters );
    Read(
        aMessage,
        0,
        sendParPckg );

    // This is Send request.
    parameters.iMessengerCommand = EPhCltMesCommandSend;

    // Ask the messenger negotiator to handle this request.
    ForwardRequest( 
        *this,
        parameters,
        aMessage );
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionMessenger::CmdSubSessionMessengerSkip
// 
// 
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionMessenger::CmdSubSessionMessengerSkip( 
    const RMessage2& aMessage )
    {
    TPhCltMessengerParameters parameters;

    // This is Skip request.
    parameters.iMessengerCommand = EPhCltMesCommandSkip;

    // Ask the messenger negotiator to handle this request.
    ForwardRequest( 
        *this,
        parameters,
        aMessage );
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionMessenger::CmdSubSessionMessengerCancel
// 
// Handle command handler client's cancel request.
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionMessenger::CmdSubSessionMessengerCancel( 
    const RMessage2& aMessage )
    {
    TInt returnValue = KErrArgument;

    TPhCltMessengerParameters parameters;

    // Check the Cancel parameters.
    TPckg< TPhCltMessengerParameters > recParPckg( 
        parameters );
    Read(
        aMessage,
        0,
        recParPckg );

    TPhCltMessengerRequest request = parameters.iCancelRequest;


    TPhCltMessengerCommand mesCommand = EPhCltMesCommandUnknown;
    if ( request == EPhCltMesRequestReceive)
        {
        mesCommand = EPhCltMesCommandReceive;
        }
    else if ( request == EPhCltMesRequestSend )
        {
        mesCommand = EPhCltMesCommandSend;
        }

    // Perform cancel only if that was valid request.
    if ( mesCommand != EPhCltMesCommandUnknown )
        {
        // Ask the messenger negotiator to cancel the pending request
        // for this subsession.
        MPhSrvMessengerNegotiator& messengerNegotiator = 
            GetMessengerNegotiator();

        messengerNegotiator.NegotiatorRequestCancel( *this, mesCommand );

        returnValue = KErrNone;
        }
    aMessage.Complete( returnValue );
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionMessenger::GetMessengerNegotiator
// 
// Return the messenger negotiator.
// -----------------------------------------------------------------------------
//
MPhSrvMessengerNegotiator& CPhSrvSubSessionMessenger::GetMessengerNegotiator()
    {
    // Messenger negotiator created already in NewL.
    return PhoneSession().PhoneServer().MessengerNegotiator();
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionMessenger::ForwardRequest
// 
// Forward request to negotiator.
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionMessenger::ForwardRequest(
    CPhSrvSubSessionBase& aSubSession,
    const TPhCltMessengerParameters& aParameters,
    const RMessage2& aMessage )
    {
    // Ask the messenger negotiator to handle this request.
    MPhSrvMessengerNegotiator& messengerNegotiator = GetMessengerNegotiator();

    messengerNegotiator.NegotiatorRequest( 
        aSubSession,
        aParameters,
        aMessage );
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionMessenger::Clean
// 
// Clean the negotiator side.
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionMessenger::Clean()
    {
    // Ask the messenger negotiator to release all reserved space
    // for this subsession.
    MPhSrvMessengerNegotiator& messengerNegotiator = GetMessengerNegotiator();

    messengerNegotiator.NegotiatorRequestDelete( *this );
    }


// End of File
