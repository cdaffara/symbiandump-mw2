/*
* Copyright (c) 2003-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Sub Session Command handler.
*                The stub version of this file can be found from file
*                CPhSrvSubSessionCommandHandlerStub.cpp
*
*/



// INCLUDE FILES
#include    "cphsrvsubsessioncommandhandler.h" 
#include    "cphsrvsession.h" 
#include    "cphsrvserver.h" 
#include    <phclttypes.h> 
#include    "mphsrvcomhandnegotiators.h" 




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
// Framework function. Handle any op-codes here.
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionCommandHandler::PhSrvMessageProcessorHandleMessageL( 
    const RMessage2& aMessage )
    {
    TBool permissionStatus ( EFalse );
    
    // Check needed capabilities    
    TBool hasCapNetworkControlAndServices( EFalse );
    TBool hasCapNetworkServices( EFalse );
    TBool hasCapWriteDeviceData( EFalse );
    hasCapNetworkControlAndServices = aMessage.HasCapability( ECapabilityNetworkControl, 
                                                              ECapabilityNetworkServices  );
    hasCapNetworkServices = aMessage.HasCapability( ECapabilityNetworkServices );
    hasCapWriteDeviceData = aMessage.HasCapability( ECapabilityWriteDeviceData );
    
    switch( aMessage.Function() )
        {
        case EPhoneServerComHandSubSessionClose:
            CmdSubSessionClose( aMessage );
            permissionStatus = ETrue;
            break;

        case EPhoneServerComHandSubSessionAtd:
            if ( hasCapNetworkControlAndServices )
                {        
                CmdSubSessionComHandAtdL( aMessage );
                permissionStatus = ETrue;
                }
            break;

        case EPhoneServerComHandSubSessionAta:
            if ( hasCapNetworkControlAndServices )
                {        
                CmdSubSessionComHandAtaL( aMessage );
                permissionStatus = ETrue;
                }
            break;

        case EPhoneServerComHandSubSessionChld:
            if ( hasCapNetworkServices )
                {
                CmdSubSessionComHandChldL( aMessage );
                permissionStatus = ETrue;
                }    
             break;

        case EPhoneServerComHandSubSessionChup:
            if ( hasCapNetworkServices )
                {
                CmdSubSessionComHandChupL( aMessage );
                permissionStatus = ETrue;
                }        
             break;

        case EPhoneServerComHandSubSessionVts:
            if ( hasCapNetworkServices )
                {
                CmdSubSessionComHandVtsL( aMessage );
                permissionStatus = ETrue;
                }        
            break;
                  
        case EPhoneServerComHandSubSessionCancel:
                    
            if ( IsCancelAllowed( aMessage ) )
                {
                CmdSubSessionComHandCancelL( aMessage );
                permissionStatus = ETrue;
                }
            break;

        case EPhoneServerComHandSubSessionMuteMic:
            if ( hasCapWriteDeviceData )
                {
                CmdSubSessionComHandMuteMicL( aMessage );
                permissionStatus = ETrue;
                }        
            break;
        
        case EPhoneServerComHandSubSessionMuteRingingTone:
            if ( hasCapWriteDeviceData )
                {
                CmdSubSessionComHandMuteRingingToneL( aMessage );
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
    if( !permissionStatus )
        {
        aMessage.Complete( KErrPermissionDenied );
        }        
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionIhf::CmdSubSessionClose
// 
// Close the sub-session.
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionCommandHandler::CmdSubSessionClose( 
    const RMessage2& aMessage )
    {
    PhoneSession().CloseSubSession( aMessage );
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionCommandHandler::CmdSubSessionComHandAtdL
// 
// Handle command handler client's Atd request.
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionCommandHandler::CmdSubSessionComHandAtdL( 
    const RMessage2& aMessage )
    {
    CmdSubSessionComHandForwardRequestL( aMessage );
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionCommandHandler::CmdSubSessionComHandAtaL
// 
// Handle command handler client's Ata request.
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionCommandHandler::CmdSubSessionComHandAtaL( 
    const RMessage2& aMessage )
    {
    CmdSubSessionComHandForwardRequestL( aMessage );
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionCommandHandler::CmdSubSessionComHandChldL
// 
// Handle command handler client's Chld request.
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionCommandHandler::CmdSubSessionComHandChldL( 
    const RMessage2& aMessage )
    {
    CmdSubSessionComHandForwardRequestL( aMessage );
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionCommandHandler::CmdSubSessionComHandChupL
// 
// Handle command handler client's Chup request.
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionCommandHandler::CmdSubSessionComHandChupL( 
    const RMessage2& aMessage )
    {
    CmdSubSessionComHandForwardRequestL( aMessage );
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionCommandHandler::CmdSubSessionComHandVtsL
// 
// Handle command handler client's Vts request.
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionCommandHandler::CmdSubSessionComHandVtsL( 
    const RMessage2& aMessage )
    {
    CmdSubSessionComHandForwardRequestL( aMessage );
    }

// -----------------------------------------------------------------------------
// CPhSrvSubSessionCommandHandler::CmdSubSessionComHandCancelL
// 
// Handle command handler client's cancel request.
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionCommandHandler::CmdSubSessionComHandCancelL( 
    const RMessage2& aMessage )
    {
    // Ask the command handler negotiator to delete any pending request for 
    // this subsession. It will panic the client if one can't be found.
    MPhSrvComHandNegotiator& comHandNegotiator = 
        PhoneSession().PhoneServer().ComHandNegotiatorL();

    comHandNegotiator.NegotiatorRequestCancel( *this );
    aMessage.Complete( KErrNone );
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionCommandHandler::CmdSubSessionComHandForwardRequestL
// 
// Setup an asynchronous request to handle external interface
// command handler requests.
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionCommandHandler::CmdSubSessionComHandForwardRequestL(
    const RMessage2& aMessage )
    {
    // Read parameters from client's address space.
    TPhCltComHandCommandParameters comHandParams;
    TPckg< TPhCltComHandCommandParameters > 
        pComHandParams( comHandParams );
    Read(
        aMessage,
        0,
        pComHandParams );

    // Submit a new request to the command handler manager.
    MPhSrvComHandNegotiator& comHandNegotiator = 
        PhoneSession().PhoneServer().ComHandNegotiatorL();

    comHandNegotiator.NegotiatorRequestL( aMessage, comHandParams, *this );
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionCommandHandler::CmdSubSessionComHandMuteMicL
// 
// Handle command handler client's mute mic request.
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionCommandHandler::CmdSubSessionComHandMuteMicL( 
    const RMessage2& aMessage )
    {
    CmdSubSessionComHandForwardRequestL( aMessage );
    }

// -----------------------------------------------------------------------------
// CPhSrvSubSessionCommandHandler::CmdSubSessionComHandMuteRingingToneL
// 
// Handle command handler client's mute ringing tone request.
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionCommandHandler::CmdSubSessionComHandMuteRingingToneL( 
    const RMessage2& aMessage )
    {
    CmdSubSessionComHandForwardRequestL( aMessage );
    }

// -----------------------------------------------------------------------------
// CPhSrvSubSessionCommandHandler::IsCancelAllowed
// 
// Handle cancel command capability check
// -----------------------------------------------------------------------------
//    
TBool CPhSrvSubSessionCommandHandler::IsCancelAllowed(
    const RMessage2& aMessage ) 
    {
    TBool permissionStatus ( EFalse );
    
    TBool hasCapNetworkControlAndServices( EFalse );
    TBool hasCapNetworkServices( EFalse );
    hasCapNetworkControlAndServices = aMessage.HasCapability( ECapabilityNetworkControl, 
                                                              ECapabilityNetworkServices  );
    hasCapNetworkServices = aMessage.HasCapability( ECapabilityNetworkServices );
 
    
    // Read parameters from client's address space.
    TPhCltComHandCommandParameters comHandParams;
    TPckg< TPhCltComHandCommandParameters > 
        pComHandParams( comHandParams );
    Read(
        aMessage,
        0,
        pComHandParams );
        
    switch ( comHandParams.iCommandHandlerCommand )    
        {
        case EPhCltCommandAtd:
        case EPhCltCommandAta:
            if ( hasCapNetworkControlAndServices )
                {
                permissionStatus = ETrue;
                }
            break;
        case EPhCltCommandChld:
        case EPhCltCommandChup:
        case EPhCltCommandVts:
            if ( hasCapNetworkServices )
                {
                permissionStatus = ETrue;
                }
            break;
        default:
            break;
        }
    
    return permissionStatus;
    }    

// End of File
