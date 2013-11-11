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
* Description:  Sub Session Ext Call.
*
*/


// INCLUDE FILES
#include "cphsrvsubsessionextcall.h" 
#include "cphsrvsession.h" 
#include "cphsrvserver.h" 
#include "mphsrvobjectnegotiators.h" 

// Hash mark
_LIT( KPhoneHash, "#" );

// dtmf chars
_LIT( KPhoneDtmfNumberChars, "pw" );

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhSrvSubSessionExtCall::CPhSrvSubSessionExtCall
// 
// C++ Constructor
// -----------------------------------------------------------------------------
//
CPhSrvSubSessionExtCall::CPhSrvSubSessionExtCall(CPhSrvSession& aSession)
:    CPhSrvSubSessionBase(aSession, EPhSrvSubSessionTypeExternalCall)
    {
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionExtCall::PhSrvMessageDecoderCanProcessMessage
// 
// Framework function. Returns ETrue for the op codes that
// this subsession can handle.
// -----------------------------------------------------------------------------
//
TBool CPhSrvSubSessionExtCall::PhSrvMessageDecoderCanProcessMessage( 
    TInt aFunction )
    {
    switch( aFunction )
        {
        case EPhoneServerExtCallSubSessionClose:
        case EPhoneServerExtCallSubSessionMakeCall:
        case EPhoneServerExtCallSubSessionMakeCallCancel:
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
void CPhSrvSubSessionExtCall::PhSrvMessageProcessorHandleMessageL( 
    const RMessage2& aMessage )
    {
    TBool permissionStatus ( EFalse );
    TBool dtmfNumber( EFalse ); //contains 'p' or 'w'  
    TBool isSSUssd( EFalse ); //is USSD/SS string
    TBool hasUssdSSCaps( EFalse ); //has USSD/SS string correct capabilities
     
    TBool hasCapNetworkServices( EFalse );
    TBool hasCapNetworkControlAndServices( EFalse );
    TBool hasCapWriteAndReadDeviceData ( EFalse );
    // Capabilities needed if NOT USSD/SS string                                                         
    hasCapNetworkServices = aMessage.HasCapability( ECapabilityNetworkServices );                                                                                                                   
      
    switch( aMessage.Function() )
        {
        case EPhoneServerExtCallSubSessionClose:
            CmdSubSessionCloseL( aMessage );
            permissionStatus = ETrue;
            break;
        case EPhoneServerExtCallSubSessionMakeCall:
            CreateCallArgsL( aMessage );
            // Checks if telephone number contains dtmf string
            dtmfNumber = IsDtmfPhoneNumber( iCallArgs->TelephoneNumber());
            if ( !dtmfNumber )
                {
                // Checks if telephone number is USSD/SS string
                isSSUssd = ( iCallArgs->TelephoneNumber().Right( KPhoneHash().Length() ) == KPhoneHash );
                if ( isSSUssd )
                    { 
                    // Capabilities needed if USSD/SS string
                    hasCapNetworkControlAndServices = aMessage.HasCapability( ECapabilityNetworkControl, 
                                                              ECapabilityNetworkServices );
                    hasCapWriteAndReadDeviceData = aMessage.HasCapability( ECapabilityReadDeviceData, ECapabilityWriteDeviceData );
                    hasUssdSSCaps =  hasCapNetworkControlAndServices && hasCapWriteAndReadDeviceData;
                    }   
                }
           if ( ( hasCapNetworkServices && !isSSUssd ) ||  hasUssdSSCaps )
                {
                CmdSubSessionMakeCallL( aMessage );
                permissionStatus = ETrue;
                }
            break;
        case EPhoneServerExtCallSubSessionMakeCallCancel:
            if ( hasCapNetworkServices )
                {
                CmdSubSessionMakeCallCancelL( aMessage );
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
// CPhSrvSubSessionExtCall::CmdSubSessionCloseL
// 
// Close the sub-session
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionExtCall::CmdSubSessionCloseL( const RMessage2& aMessage )
    {
    delete iCallArgs;
    iCallArgs =NULL;
    PhoneSession().CloseSubSession( aMessage );
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionExtCall::CmdSubSessionMakeCallL
// 
// Setup an asynchronous request to handle external interface
// call requests.
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionExtCall::CmdSubSessionMakeCallL( 
    const RMessage2& aMessage )
    {               

    // Submit a new request to the call manager
    MPhSrvNegotiatorCall& callNegotiator = PhoneSession().PhoneServer(
        ).CallNegotiatorL();
    callNegotiator.NegotiatorDialL( aMessage, *iCallArgs, *this );
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionExtCall::CmdSubSessionMakeCallCancelL
// 
// Cancel an outstanding request to handle call initiations.
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionExtCall::CmdSubSessionMakeCallCancelL( 
    const RMessage2& aMessage )
    {
    // Ask the negotiator to delete any pending call request for this 
    // subsession. It will panic the client if one can't be found.
    MPhSrvNegotiatorCall& callNegotiator = PhoneSession().PhoneServer(
        ).CallNegotiatorL();

    callNegotiator.NegotiatorDialCancelL( *this );
    aMessage.Complete( KErrNone );
    }

// -----------------------------------------------------------------------------
// CPhSrvSubSessionExtCall::CreateCallArgs
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionExtCall::CreateCallArgsL( const RMessage2& aMessage )
    {
    HBufC8* package = HBufC8::NewLC( aMessage.Int1() ); 
    TPtr8 calldata( package->Des() );
    PhoneSession().Read(
            aMessage,
            0,
            calldata );
                                             
    iCallArgs = CPhCltExtPhoneDialData::NewL( *package );
    CleanupStack::PopAndDestroy( package );
    }
    
// -----------------------------------------------------------------------------
// CPhSrvSubSessionExtCall::IsDtmfPhoneNumber
// -----------------------------------------------------------------------------
//
TBool CPhSrvSubSessionExtCall::IsDtmfPhoneNumber(
        const TDesC& aString )
    {
    

    // String must contain 'p' or 'w'.
    TBool must = EFalse;

        {
        TPtrC chs( KPhoneDtmfNumberChars );
        const TInt length = chs.Length();
        TInt index;

        for ( index = 0; index < length; index++ )
            {
            if ( aString.Locate( chs[ index ] ) != KErrNotFound )
                {
                must = ETrue;
                break;
                }
            }
        }

    return must; 
    }
// End of File
