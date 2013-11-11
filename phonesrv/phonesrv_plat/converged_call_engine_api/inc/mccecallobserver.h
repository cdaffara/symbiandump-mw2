/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Observer interface for CCE call object
*
*/


#ifndef MCCECALLOBSERVER_H
#define MCCECALLOBSERVER_H

#include <e32std.h>
#include "ccpdefs.h" // error defenitions


/**
 *  Call observer for MCCECall class.
 *
 *  @lib cce.dll
 *  @since S60 v3.2
 */
class MCCECallObserver
    {
public:
    /** Tone to play enumeration. Used in MCCECallObserver::PlayInbandTone*/
    enum TCCETone
        {
        ECCENoTone,
        ECCEUserBusy,
        ECCERadioPathNotAvailable,
        ECCECongestion,
        ECCESpecialInformation,
        ECCEReorder,
        ECCERemoteAlerting,
        ECCEDataCallTone,
        ECCENoSoundSequence,
        ECCEBeepSequence, /** ENoSoundSequence handled the same way */
        ECCECallWaiting
        };
   
 
    
    /** MCCE call events*/
    enum TCCECallEvent
        {
        /** The call has been placed on hold as a result of a local action.     */
        ELocalHold,
        /** The call has been placed on hold by the remote connected party. */
        ERemoteHold,

        /** The call has been resumed as a result of a local action. */
        ELocalResume,
        /** The call has been resumed by the remote connected party. */
        ERemoteResume,

        /** The call has been deflected to another remote party as a result of a local action. */
        ELocalDeflectCall,

        /** The call has been transferred to another remote party as a result of a local action. */
        ELocalTransfer,
        /** */
        ERemoteTransferring,
        /** */
        ERemoteTransferAlerting,

        /** The call has been terminated by the remote party. */
        ERemoteTerminated,

        /** The call has been joined by the remote connected party to other call(s) to create/add 
         * to a conference call. */     
        /* Call has been added to remote conference call at remote end */
        ERemoteConferenceCreate,

        /** The outgoing call has been barred by the remote party. */
        ERemoteBarred,
        /** The call is being forwarded by the remote party. */
        ERemoteForwarding,
        /** Call is waiting at the remote end. Obsolote, use supplementary service ESsCallWaiting */
        ERemoteWaiting,
        /** The outgoing call has been barred by the local party. */
        ELocalBarred,
        /** Indicates that the connected call is secure */
        ESecureSession,
        /** Indicates that the connected call is not secure */
        ENotSecureSession,

        /** VoIP specific */
        EVoIPEventNotSecureSessionWithSips,
        /** VoIP specific: Indicates that the early media playing will be done in MCC component */
        EVoIPEventEarlyMediaStarted,
        /** VoIP specific */
        EVoIPEventMultipleChoices,
        /** VoIP specific */
        EVoIPEventMovedPermanently,
        
        ////////////////////////////
        // CS call specific events
        ///////////////////////////        
       
        /** Call has been deflected to another remote party as a result of a local action.  */
        ECCECSLocalDeflectCall,
        /** CS call specific event */
        ECCECSDataPortLoaned, 
        /** CS call specific event */
        ECCECSDataPortRecovered,

        /** One or more of following information has been changed
         *  - Remote party name
         *  - Remote party number/address
         *  - Call parameters ( ie. service id )
         */ 
        ECCENotifyRemotePartyInfoChange,

        /** Call secure status cannot be determined */
        ECCESecureNotSpecified
        };
    
     // Flags indicating all the possible call control capabilities.
     enum TCCECallControlCaps 
        {
        /** The call supports data calls. */
        ECCECapsData           =   0x00000001,
        /** The call supports fax calls. */
        ECCECapsFax            =   0x00000002,
        /** The call supports voice calls. */
        ECCECapsVoice          =   0x00000004,
        /** The Dial request is currently a valid action. */
        ECCECapsDial           =   0x00000008,
        /** The Connect request is currently a valid action. */
        ECCECapsConnect        =   0x00000010,
        /** The Hang Up request is currently a valid action. */
        ECCECapsHangUp         =   0x00000020,
        /** The Answer request is currently a valid action. */
        ECCECapsAnswer         =   0x00000040,
        /** The Loan Data Port request is currently a valid action. */
        ECCECapsLoanDataPort   =   0x00000080,
        /** The Recover Data Port request is currently a valid action. */
        ECCECapsRecoverDataPort=   0x00000100,  
        /** Indicates that this call can be put on hold. This implies that the call is currently 
        active and that there is no other held call. */
        ECCECapsHold           =   0x00000200,
        /** Indicates that this call can be resumed. This implies that the call is currently 
        on hold and that there is no other active call. */ 
        ECCECapsResume         =   0x00000400,
        /** Indicates that this call's state can be swapped to the opposite state. This
        implies that this call is either active or held. There may be another call 
        in the opposite state and if this is the case then both calls will be simultaneously 
        swapped to their opposite state. */
        ECCECapsSwap           =   0x00000800,
        /** Indicates that this incoming call can be deflected to another destination.*/    
        ECCECapsDeflect        =   0x00001000,
        /** Indicates that this call can be transferred to the remote party of another call. 
        For this capability to be set there must be one active call and one held or 
        dialing call.*/ 
        ECCECapsTransfer       =   0x00002000,
        /** Indicates that this call can be joined either to another single call to form 
        a conference call or to an existing conference call.*/
        ECCECapsJoin           =   0x00004000,
        /** Indicates that this call (within a conference call) can go into One to One mode. 
        This action would put the rest of the conference call on hold. */
        ECCECapsOneToOne       =   0x00008000,
        /** Indicates that the user can activate a CCBS request on this failed call set-up.*/   
        ECCECapsActivateCCBS   =   0x00010000,
        /** Indicates that this call is part of an alternating call and the client can call 
        SwitchAlternatingCall() on this call in order to switch it to its other mode.*/
        ECCECapsSwitchAlternatingCall = 0x00020000,
        /** Indicates that this call can use/create a new bearer. This means that multiple 
        calls (multicall) can be active at the same time using no more that 7 bearers 
        simultaneously. The other option is to swap with an active call, therefore 
        it becomes held and shares the same bearer.*/
        ECCECapsMulticall      =   0x00040000,
        /** Indicates that this call is an packet swiched call */
        ECCECapsPSCall       =   0x00080000,
        /** Indicates that this call supports unattended transfer method and that this call 
        is currently active */
        ECCECapsUnattendedTransfer = 0x00100000
        };
    
public:

    /**
    * Call errors are notified using this interface. 
    *
    * @since S60 v3.2
    * @param aError Occurred error as integer,
    * return none
    */
    virtual void ErrorOccurred( const TCCPError aError ) = 0;
    
    /**
    * Call state notificfation. 
    * The observer need to verify the correctness of given state.
    * CCE will do at least basic call state change checking but also the client 
    * needs to verify the correctness.
    *
    * @since S60 v3.2
    * @param aState New state of the call
    * return none
    */
    virtual void CallStateChanged( const CCPCall::TCallState aState ) = 0;

    /**
    * The state of the call has changed with inband tone, meaning network is playing 
    * the tone relating to the state.
    * @since S60 3.2
    * @param aState Call state.
    * @return none
    */
    virtual void CallStateChangedWithInband( const CCPCall::TCallState aState ) = 0;

    /**
    * An event for the call has occurred. 
    * The observer need to verify event correcness.
    *
    * @since S60 v3.2
    * @param aEvent Occurred call event
    * return none
    */
    virtual void CallEventOccurred( const MCCECallObserver::TCCECallEvent aEvent ) = 0;

    /**
    * Notifies observer the change in call duration.
    *
    * @since S60 v3.2
    * @param aDuration Duration of the call in seconds
    * return none
    */
    virtual void CallDurationChanged( const TTimeIntervalSeconds aDuration ) = 0;
    
    /**
    * Notifies observer about the changes in call control caps.
    *
    * @since S60 v3.2
    * @param aCaps Contains updated call control capability flags
    * return none
    */
    virtual void CallCapsChanged( const MCCECallObserver::TCCECallControlCaps aCaps ) = 0;
    
    /**
    * Notifies observer about transfer made by remote end
    *
    * @since S60 v3.2
    * @param aAttended ETrue: if transfer type is attended, EFalse: type is unattended
    * @param aDestination Address where the call is to be transferred. Given as received in CCE 
    * observer needs to verify the address, e.g. it can be KNullDesC.
    * A-B -> B-C, C has requested anonymity in network???
    */
    virtual void HandleTransfer( const TBool aAttended, 
                                 const TDesC& aDestination ) = 0;
    };


#endif // MCCECALLOBSERVER_H
