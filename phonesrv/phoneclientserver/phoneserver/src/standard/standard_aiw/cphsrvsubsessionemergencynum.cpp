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
* Description:  Sub Session Emergency Num.
*
*/



// INCLUDE FILES
#include "cphsrvsubsessionemergencynum.h" 
#include <s32mem.h>
#include <phclttypes.h> 
#include "cphsrvsession.h" 
#include "cphsrvserver.h" 
#include "mphsrvphoneinterface.h" 
#include "mphsrvemergencynumbermanager.h" 
#include "mphsrvemergencynegotiators.h" 
#include "phsrvdebuginfo.h" 

// CONSTANTS



// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhSrvSubSessionEmergencyNum::CPhSrvSubSessionEmergencyNum
// 
// C++ Constructor
// -----------------------------------------------------------------------------
//
CPhSrvSubSessionEmergencyNum::CPhSrvSubSessionEmergencyNum( 
    CPhSrvSession& aSession )
:   CPhSrvSubSessionBase( aSession, EPhSrvSubSessionTypeEmergencyNumber )
    {
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionEmergencyNum::ConstructL
// 
// Symbian OS 2nd phase constructor
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionEmergencyNum::ConstructL()
    {
    CPhSrvSubSessionBase::ConstructL();
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionEmergencyNum::PhSrvMessageDecoderCanProcessMessage
// 
// Framework function. Returns ETrue for the op codes that
// this subsession can handle.
// -----------------------------------------------------------------------------
//
TBool CPhSrvSubSessionEmergencyNum::PhSrvMessageDecoderCanProcessMessage( 
    TInt aFunction )
    {
    switch(aFunction)
        {
        case EPhoneServerEmergencyNumberSubSessionClose:
        case EPhoneServerEmergencyNumberSubSessionIsEmergencyNumber:
        case EPhoneServerEmergencyNumberSubsessionEmergencyDial:
        case EPhoneServerEmergencyNumberSubsessionEmergencyDialCancel:
            return ETrue;

        default:
            return EFalse;
        }
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionEmergencyNum::PhSrvMessageProcessorHandleMessageL
// 
// Framework function. Handle any op-codes here.
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionEmergencyNum::PhSrvMessageProcessorHandleMessageL( 
    const RMessage2& aMessage )
    {
    TBool permissionStatus ( EFalse );
     
    // Check needed capabilities
    TBool hasCapNetworkControlAndServices ( EFalse );
    hasCapNetworkControlAndServices = aMessage.HasCapability( ECapabilityNetworkControl,
                                                              ECapabilityNetworkServices );
    switch( aMessage.Function() )
        {
    case EPhoneServerEmergencyNumberSubSessionClose:
        CmdSubSessionCloseL( aMessage );
        permissionStatus = ETrue;
        break;
    case EPhoneServerEmergencyNumberSubSessionIsEmergencyNumber:
        CmdSubSessionIsEmergencyNumberL( aMessage );
        permissionStatus = ETrue;
        break;
    case EPhoneServerEmergencyNumberSubsessionEmergencyDial:
        if ( hasCapNetworkControlAndServices )
            {
            CmdSubSessionDialEmergencyCallL( aMessage );
            permissionStatus = ETrue;
            }
        break;
     case EPhoneServerEmergencyNumberSubsessionEmergencyDialCancel:
        if ( hasCapNetworkControlAndServices )
            {
            CmdSubSessionDialEmergencyCallCancelL( aMessage );
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
// CPhSrvSubSessionEmergencyNum::CmdSubSessionCloseL
// 
// Close the sub-session
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionEmergencyNum::CmdSubSessionCloseL( 
    const RMessage2& aMessage )
    {
    PhoneSession().CloseSubSession( aMessage );
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionEmergencyNum::CmdSubSessionIsEmergencyNumberL
// 
// Check if the specified number is an emergency number
//
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionEmergencyNum::CmdSubSessionIsEmergencyNumberL( 
    const RMessage2& aMessage )
    {
    _DPRINT( 4, "PhSrv.CPhSrvSubSessionEmergencyNum:Start");  
    // Read from client-side
    TPhCltTelephoneNumber number;
    TPckg< TPhCltTelephoneNumber > pNumber( number );    
    Read(
        aMessage,
        0,
        pNumber );

    const TPhCltEmergencyNumberQueryMode mode = 
        static_cast< TPhCltEmergencyNumberQueryMode >( aMessage.Int2() );

    // Is this an emergency number?
    const TBool isEmergencyNumber = 
        PhoneSession().PhoneServer().EmergencyNumberManagerL().
            EmergencyNumberManagerIsEmergencyNumberL( number, mode );
    const TPckg< TBool > pIsEmergencyNumber( isEmergencyNumber );
    
    // Write back to client    
    if ( mode == EPhCltEmergencyNumberQueryModeAdvanced && isEmergencyNumber ||
        mode == EPhCltEmergencyNumberQueryModeNormalWriteback && isEmergencyNumber )
        {        
        TPckg< TPhCltTelephoneNumber > pNumber2( number );    
        Write(
            aMessage,
            0,
            pNumber2          // write emergency number back.
             );
        }    

    Write(
        aMessage,
        1,              // write emergency number check result back.
        pIsEmergencyNumber );

    aMessage.Complete( KErrNone );
_DPRINT( 4, "PhSrv.CPhSrvSubSessionEmergencyNum.End" );       // debug print
    }
    
// -----------------------------------------------------------------------------
// CPhSrvSubSessionEmergencyNum::CmdSubSessionDialEmergencyCall
// 
// Initiates emergency call
//
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionEmergencyNum::CmdSubSessionDialEmergencyCallL( 
    const RMessage2& aMessage )
    {
    _DPRINT( 4, "PhSrv.CPhSrvSubSessionEmergencyNum.DialEM" );       // debug print
    // Submit a new request to the call manager
    MPhSrvEmergencyNegotiatorCall& callNegotiator = 
        PhoneSession().PhoneServer().EmergencyCallNegotiatorL();
    callNegotiator.EmergencyDialL( aMessage );
    }
    
// -----------------------------------------------------------------------------
// CPhSrvSubSessionEmergencyNum::CmdSubSessionDialEmergencyCallCancelL
// 
// Cancel emergency call
//
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionEmergencyNum::CmdSubSessionDialEmergencyCallCancelL( 
    const RMessage2& aMessage )
    {
    _DPRINT( 4, "PhSrv.CPhSrvSubSessionEmergencyNum.EMCancel" );       // debug print
    // Submit a new request to the call manager
    MPhSrvEmergencyNegotiatorCall& callNegotiator = 
        PhoneSession().PhoneServer().EmergencyCallNegotiatorL();
    callNegotiator.EmergencyDialCancel();
    aMessage.Complete( KErrNone );
    }

// End of File
