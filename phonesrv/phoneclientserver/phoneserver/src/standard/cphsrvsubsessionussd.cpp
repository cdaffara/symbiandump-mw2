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
* Description:  Sub Session Ussd.
*
*/


// INCLUDE FILES
#include "cphsrvsubsessionussd.h" 

#include    <etelmm.h>            // ETel

#include "phsrvutils.h" 
#include "cphsrvsession.h" 
#include "cphsrvserver.h" 

#include "cphsrvussdmanager.h" 

#include <phclttypes.h> 


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhSrvSubSessionUssd::CPhSrvSubSessionUssd
// 
// C++ Constructor
// -----------------------------------------------------------------------------
//
CPhSrvSubSessionUssd::CPhSrvSubSessionUssd( CPhSrvSession& aSession )
:    CPhSrvSubSessionBase( aSession, EPhSrvSubSessionTypeUssd )
    {
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionUssd::PhSrvMessageDecoderCanProcessMessage
// 
// Framework function. Returns ETrue for the op codes that
// this subsession can handle.
// -----------------------------------------------------------------------------
//
TBool CPhSrvSubSessionUssd::PhSrvMessageDecoderCanProcessMessage( 
    TInt aFunction )
    {
    switch( aFunction )
        {
        case EPhoneServerUSSDSubSessionClose:
        case EPhoneServerUSSDSubSessionSendMessage:
        case EPhoneServerUSSDSubSessionSendMessageCancel:
        case EPhoneServerUSSDSubSessionStartEditing:
        case EPhoneServerUSSDSubSessionAppStarting:
        case EPhoneServerUSSDSubSessionAppTerminating:
        case EPhoneServerUSSDSubSessionAppToForeground:
        case EPhoneServerUSSDSubSessionAppToBackground:
        case EPhoneServerUSSDSubSessionStartSAT:
        case EPhoneServerUSSDSubSessionStopSAT:
            return ETrue;
            
        default:
            return EFalse;
        }
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionUssd::PhSrvMessageProcessorHandleMessageL
// 
// Framework function. Handle any op-codes here.
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionUssd::PhSrvMessageProcessorHandleMessageL( 
    const RMessage2& aMessage )
    {
    TBool permissionStatus ( EFalse );
    
    // Check needed capabilities    
    TBool hasCapNetworkControlAndServices ( EFalse );
    TBool hasCapWriteDeviceData ( EFalse );
    hasCapNetworkControlAndServices = aMessage.HasCapability( ECapabilityNetworkControl, 
                                                              ECapabilityNetworkServices );
    hasCapWriteDeviceData = aMessage.HasCapability( ECapabilityWriteDeviceData );
    // Check Security ID     
    TBool hasUssdSID = ( aMessage.SecureId().iId == KUssdSecureId.iUid ) ? ETrue : EFalse; 
    TBool hasSatSID  = ( aMessage.SecureId().iId == KSatSecureId.iUid ) ? ETrue : EFalse; 

    switch( aMessage.Function() )
        {
        case EPhoneServerUSSDSubSessionClose:
            CmdSubSessionCloseL( aMessage );
            permissionStatus = ETrue;
            break;
        case EPhoneServerUSSDSubSessionSendMessage:
            if ( hasCapNetworkControlAndServices && hasCapWriteDeviceData  )
                {
                CmdSubSessionSendMessageL( aMessage );
                permissionStatus = ETrue;
                }
            break;
        case EPhoneServerUSSDSubSessionSendMessageCancel:
            if ( hasCapNetworkControlAndServices && hasCapWriteDeviceData )
                {        
                CmdSubSessionSendMessageCancelL( aMessage );
                permissionStatus = ETrue;
                }
            break;
        case EPhoneServerUSSDSubSessionStartEditing:
            CmdSubSessionStartEditingL( aMessage );
            permissionStatus = ETrue;
            break;
        case EPhoneServerUSSDSubSessionAppStarting:
            if ( hasUssdSID )
                {
                CmdSubSessionAppStartingL( aMessage );
                permissionStatus = ETrue;
                }
            break;
        case EPhoneServerUSSDSubSessionAppTerminating:
            if ( hasUssdSID )
                {
                CmdSubSessionAppTerminatingL( aMessage );
                permissionStatus = ETrue;               
                }
            break;
        case EPhoneServerUSSDSubSessionAppToForeground:
            if ( hasUssdSID )
                {
                CmdSubSessionAppToForegroundL( aMessage );
                permissionStatus = ETrue;
                }
             break;
        case EPhoneServerUSSDSubSessionAppToBackground:
            if ( hasUssdSID )
                {
                CmdSubSessionAppToBackgroundL( aMessage );
                permissionStatus = ETrue;                
                }
             break;   
        case EPhoneServerUSSDSubSessionStartSAT:
            if ( hasSatSID )
                {
                CmdSubSessionStartSATL( aMessage );
                permissionStatus = ETrue;    
                }
             break;
        case EPhoneServerUSSDSubSessionStopSAT:
            if ( hasSatSID )
                {
                CmdSubSessionStopSATL( aMessage );
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
// CPhSrvSubSessionUssd::UssdMessageSentObserverHandleResult
// 
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionUssd::UssdMessageSentObserverHandleResult( TInt aError )
    {
    __ASSERT_DEBUG( iHavePendingSendMessagePointer, 
        PhoneServerUtils::Panic( EPhoneServerPanicNoPendingSendRequestFault ) );
    
    // Notify observer
    if ( iHavePendingSendMessagePointer )
        {
        if ( !iPendingSendMessagePointer.IsNull() )
            {
            iPendingSendMessagePointer.Complete( aError );
            }
        iHavePendingSendMessagePointer = EFalse;
        }
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionUssd::CmdSubSessionCloseL
// 
// Close the sub-session
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionUssd::CmdSubSessionCloseL( const RMessage2& aMessage )
    {
    PhoneSession().CloseSubSession( aMessage );
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionUssd::CmdSubSessionSendMessageL
// 
// -----------------------------------------------------------------------------
//
/*****************************************************
*   Series 60 Customer / ETel
*   Series 60  ETel API
*****************************************************/
void CPhSrvSubSessionUssd::CmdSubSessionSendMessageL( 
    const RMessage2& aMessage )
    {
    if (iHavePendingSendMessagePointer)
        {
        PhoneSession().PanicClient(
            aMessage,
            EPhCltServerInitiatedPanicRequestAlreadyActive );
        }
    else
        {
        // Get text buffer, 0 buffer, 1 length, 2 attributes
        HBufC8* message = HBufC8::NewLC( aMessage.Int1() );
        TPtr8 pMessage(message->Des());
        PhoneSession().Read(
            aMessage,
            0,
            pMessage );

        RMobileUssdMessaging::TMobileUssdAttributesV1* attributes = 
            new ( ELeave ) RMobileUssdMessaging::TMobileUssdAttributesV1;
        CleanupStack::PushL( attributes );
        RMobileUssdMessaging::TMobileUssdAttributesV1Pckg 
            pAttributes( *attributes );
        PhoneSession().Read(
            aMessage,
            2,
            pAttributes );
     
        // Try and send the message
        PhoneSession().PhoneServer().UssdManagerL().SendUssdL( 
            *message, 
            *attributes, 
            *this );
        CleanupStack::PopAndDestroy(2); // attributes, message
        
        iHavePendingSendMessagePointer = ETrue;
        iPendingSendMessagePointer = aMessage;
        }
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionUssd::CmdSubSessionSendMessageCancelL
// 
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionUssd::CmdSubSessionSendMessageCancelL( 
    const RMessage2& aMessage )
    {
    // Cancel Send
    PhoneSession().PhoneServer().UssdManagerL().SendUssdCancel();

    // Complete old request
    if ( iHavePendingSendMessagePointer )
        {
        if ( !iPendingSendMessagePointer.IsNull() )
            {
            iPendingSendMessagePointer.Complete( KErrCancel );
            }
        iHavePendingSendMessagePointer = EFalse;
        }
    aMessage.Complete( KErrNone );
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionUssd::CmdSubSessionStartEditingL
// 
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionUssd::CmdSubSessionStartEditingL( 
    const RMessage2& aMessage )
    {

    PhoneSession().PhoneServer().UssdManagerL().RequestStartEditingL();
    aMessage.Complete( KErrNone );
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionUssd::CmdSubSessionAppStartingL
// 
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionUssd::CmdSubSessionAppStartingL( 
    const RMessage2& aMessage )
    {
    PhoneSession().PhoneServer().UssdManagerL().InformUssdApplicationStarting();
    aMessage.Complete( KErrNone );
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionUssd::CmdSubSessionAppTerminatingL
// 
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionUssd::CmdSubSessionAppTerminatingL( 
    const RMessage2& aMessage )
    {
    PhoneSession().PhoneServer().UssdManagerL(
        ).InformUssdApplicationTerminatingL( aMessage );

    // Complete the message.
    aMessage.Complete( KErrNone );
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionUssd::CmdSubSessionAppToForegroundL
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionUssd::CmdSubSessionAppToForegroundL( 
    const RMessage2& aMessage )
    {
    TInt ret = PhoneSession().PhoneServer().UssdManagerL(
        ).InformUssdApplicationToForeground();
    aMessage.Complete( ret );
    }


// -----------------------------------------------------------------------------
// CPhSrvSubSessionUssd::CmdSubSessionAppToBackgroundL
// 
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionUssd::CmdSubSessionAppToBackgroundL( 
    const RMessage2& aMessage )
    {    
    PhoneSession().PhoneServer().UssdManagerL(
        ).InformUssdApplicationToBackground();
    aMessage.Complete( KErrNone );
    }

// -----------------------------------------------------------------------------
// CPhSrvSubSessionUssd::CmdSubSessionStartSATL
// 
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionUssd::CmdSubSessionStartSATL( const RMessage2& aMessage )
    {
    PhoneSession().PhoneServer().UssdManagerL().InformStartSAT( aMessage );
    }

// -----------------------------------------------------------------------------
// CPhSrvSubSessionUssd::CmdSubSessionStartSATL
// 
// -----------------------------------------------------------------------------
//
void CPhSrvSubSessionUssd::CmdSubSessionStopSATL( const RMessage2& aMessage )
    {
    PhoneSession().PhoneServer().UssdManagerL().InformStopSAT();
    aMessage.Complete( KErrNone );
    }


// End of File
