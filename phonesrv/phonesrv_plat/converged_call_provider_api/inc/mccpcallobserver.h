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
* Description:  CCP Call observer
*
*/


#ifndef MCCPCALLOBSERVER_H
#define MCCPCALLOBSERVER_H

// INCLUDES
#include <e32base.h>
#include "ccpdefs.h" // error defenitions

class MCCPCall;

/**
* CCP API Call observer class
*
*  @since S60 3.2
*/
class MCCPCallObserver
    {
public:
    /** Call states */
    enum TCCPCallState
        {
        /** Indicates initial call state after call creation. Basic state. */
        ECCPStateIdle,
        /** Indicates that the MT call is ringing but not answered yet by the local user. Basic state.*/
        ECCPStateRinging,
        /** MO Call: the network notifies to the MS that the remote party is now ringing. Basic state.*/
        ECCPStateConnecting,
        /** Indicates that call is connected and active. Basic state.*/
        ECCPStateConnected,
        /** Indicates that call is disconnecting. Basic state.*/
        ECCPStateDisconnecting,
        /** Indicates that the call is connected but on hold. Basic state.*/
        ECCPStateHold,

        /** Call is transfering. Optional state.*/
        ECCPStateTransferring,
        /** MO call is being forwarded at receiver end. Optional state. */
        ECCPStateForwarding,
        /** Call is queued locally. Optional state. */
        ECCPStateQueued,

        /** Indicates that the local user has answered the MT call but the network has not
        acknowledged the call connection yet.  Must be sent after MCCPCall::Answer method has been completed. */
        ECCPStateAnswering,
   
        /** Indicates that the call is dialing. Must be sent after MCCPCall::Dial method has been completed. */
        ECCPStateDialling,
        
        };

    /** Describes the possible call events. */
    enum TCCPCallEvent
        {
        /** Call has been placed on hold as a result of a local action. */
        ECCPLocalHold,
        /** Call has been placed on hold by the remote connected party. */
        ECCPRemoteHold,
        /** Call has been resumed as a result of a local action.    */
        ECCPLocalResume,
        /** Call has been resumed by the remote connected party. */
        ECCPRemoteResume,
        /** Outgoing call has been barred by the local party. */
        ECCPLocalBarred,
        /** Outgoing call has been barred by the remote party. */
        ECCPRemoteBarred,
        /** Call is waiting at the remote end. Obsolote, use supplementary service ESsCallWaiting */
        ECCPRemoteWaiting,
        /** Call has been terminated by the remote party. */
        ECCPRemoteTerminated,
        /** Call is in queue */
        ECCPQueued,

        /** VoIP specific */
        ECCCSPEarlyMediaStarted,

        /** Call is secure */
        ECCPSecureCall,
        /** Call is not secure */
        ECCPNotSecureCall,
        
        /** VoIP specific
        User has called with address containing "SIPS:" prefix. This event notifies that
        secure call signaling is not possible. **/
        ECCPNotSecureSessionWithSips,
       
        /** Call has been added to remote conference call at remote end */
        ECCPRemoteConferenceCreate,
       
        ////////////////////////////
        // CS call specific events
        ///////////////////////////        
       
        /** CS call specific event */
        ECCPCSDataPortLoaned,
        /** CS call specific event */
        ECCPCSDataPortRecovered,

        /** One or more of following information has been changed
         *  - Remote party name
         *  - Remote party number/address
         *  - Call parameters ( ie. service id )
         */     
        ECCPNotifyRemotePartyInfoChange,

        /** Call secure status cannot be determined */
        ECCPSecureNotSpecified
        };


    /** Flags indicating all the possible call control capabilities. */
    enum TCCPCallControlCaps
        {
        /** Call supports data calls. */
        ECCPCapsData           =   0x00000001,
        /** Call supports fax calls. */
        ECCPCapsFax            =   0x00000002,
        /** Call supports voice calls. */
        ECCPCapsVoice          =   0x00000004,
        /** The Dial request is currently a valid action. */
        ECCPCapsDial           =   0x00000008,
        /** The Connect request is currently a valid action. */
        ECCPCapsConnect        =   0x00000010,
        /** The Hang Up request is currently a valid action. */
        ECCPCapsHangUp         =   0x00000020,
        /** The Answer request is currently a valid action. */
        ECCPCapsAnswer         =   0x00000040,
        /** The Loan Data Port request is currently a valid action. */
        ECCPCapsLoanDataPort   =   0x00000080,
        /** The Recover Data Port request is currently a valid action. */
        ECCPCapsRecoverDataPort=   0x00000100,
        /** Indicates that this call can be put on hold. This implies that the call is currently
        active and that there is no other held call. */
        ECCPCapsHold           =   0x00000200,
        /** Indicates that this call can be resumed. This implies that the call is currently
        on hold and that there is no other active call. */
        ECCPCapsResume         =   0x00000400,
        /** Indicates that this call's state can be swapped to the opposite state. This
        implies that this call is either active or held. There may be another call
        in the opposite state and if this is the case then both calls will be simultaneously
        swapped to their opposite state. */
        ECCPCapsSwap           =   0x00000800,
        /** Indicates that this incoming call can be deflected to another destination.*/
        ECCPCapsDeflect        =   0x00001000,
        /** Indicates that this call can be transferred to the remote party of another call.
        For this capability to be set there must be one active call and one held or
        dialing call.*/
        ECCPCapsTransfer       =   0x00002000,
        /** Indicates that this call can be joined either to another single call to form
        a conference call or to an existing conference call.*/
        ECCPCapsJoin           =   0x00004000,
        /** Indicates that this call (within a conference call) can go into One to One mode.
        This action would put the rest of the conference call on hold. */
        ECCPCapsOneToOne       =   0x00008000,
        /** Indicates that the user can activate a CCBS request on this failed call set-up.*/
        ECCPCapsActivateCCBS   =   0x00010000,
        /** Indicates that this call is part of an alternating call and the client can call
        SwitchAlternatingCall() on this call in order to switch it to its other mode.*/
        ECCPCapsSwitchAlternatingCall = 0x00020000,
        /** Indicates that this call can use/create a new bearer. This means that multiple
        calls (multicall) can be active at the same time using no more that 7 bearers
        simultaneously. The other option is to swap with an active call, therefore
        it becomes held and shares the same bearer.*/
        ECCPCapsMulticall      =   0x00040000,
        /** Indicates that this call is an packet switched call */
        ECCPCapsPSCall       =   0x00080000,
        /** Indicates that this call supports unattended transfer method and that this call
        is currently active */
        ECCPCapsUnattendedTransfer = 0x00100000,
        /** Indicates that a conference call can be created. */
        KCCPCapsCreate = 0x00200000
        };


protected:
    /** 
    * Protects the observer being deleted through the observer interface 
    * @since S60 3.2
    */
    virtual inline ~MCCPCallObserver() {};
    
public:
    /**
    * An error has occurred concerning a specific call.
    * @since S60 3.2
    * @param aError Error code.
    * @param aCall Pointer to used ccp call, can be NULL in emergency call
    * @return none
    */
    virtual void ErrorOccurred( const TCCPError aError, MCCPCall* aCall ) = 0; 

    /**
    * The state of the call has changed.
    * @since S60 3.2
    * @param aState Call state.
    * @param aCall Pointer to used ccp call, can be NULL in emergency call
    * @return none
    */
    virtual void CallStateChanged( const MCCPCallObserver::TCCPCallState aState, 
                                   MCCPCall* aCall) = 0; 

    /**
    * The state of the call has changed with inband tone, meaning network is playing 
    * the tone relating to the state.
    * @since S60 3.2
    * @param aState Call state.
    * @param aCall Pointer to used ccp call, can be NULL in emergency call
    * @return none
    */
    virtual void CallStateChangedWithInband( const MCCPCallObserver::TCCPCallState aState, 
                                             MCCPCall* aCall ) = 0; 

    /**
    * An event has occurred concerning a specific call.
    * @since S60 3.2
    * @param aEvent Occurred event.
    * @param aCall Pointer to used ccp call, can be NULL in emergency call
    * @return none
    */
    virtual void CallEventOccurred( const MCCPCallObserver::TCCPCallEvent aEvent, 
                                    MCCPCall* aCall ) = 0; 

    /**
    * Notifies changed call capabilities.
    * @since S60 3.2
    * @param aCapsFlags New capabilities for the call
    * @param aCall Pointer to used ccp call, can be NULL in emergency call
    * @return none
    */
    virtual void CallCapsChanged( const TUint32 aCapsFlags, 
                                  MCCPCall* aCall ) = 0; 
    };

#endif // MCCPCALLOBSERVER_H

// End of File
