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
* Description:  CCP call class, Contains methods common for a call.
*
*/


#ifndef MCCPCALL_H
#define MCCPCALL_H

//  INCLUDES
#include <e32base.h>
#include <mccpcallobserver.h>

#include <ccpdefs.h>
#include <cccpcallparameters.h>

class MCCPConferenceCall;
class MCCPConferenceCallObserver;

class MCCPTransferProvider;
class MCCPTransferObserver;

class MCCPForwardProvider;
class MCCPForwardObserver;


/**
* All common methods for different calls.
* Events, errors, status notifications etc are received via MCCPCallObserver class
*
*  @since S60 3.2
*/
class MCCPCall
    {
protected:
    /** 
    * Protected destructor. Object cannot be deleted from plug-in client (CCE).
    * @since S60 3.2
    */
    virtual inline ~MCCPCall() {};

public:
    /**
    * Answer an incoming call. 
    * Returns any error asyncronous via MCCPCallObserver.
    * If the request cannot be started then an error will be returned immediately as return value. 
    * In succesfull case KErrNone will be returned and the requested action has been started.
    * @param None
    * @return KErrNone if request was started succesfully. In that case plug-in must return 
    * MCCPCallObserver::ECCPStateAnswering.
    * @return KErrNotReady if call is not in ringing or queued state.
    * @return KErrNotSupported if call does not support queueing. Plug-in will disconnect the call in this case.
    * @pre Call state is MCCPCallObserver::ECCPStateRinging or MCCPCallObserver::ECCPStateQueued.
    * @since S60 3.2
    */
    virtual TInt Answer() = 0;
      
    /**
    * Reject an incoming call.
    * If the request cannot be started then an error will be returned immediately as return value. 
    * In succesfull case KErrNone will be returned and the requested action has been started.
    * @param none
    * @return KErrNone if request was started succesfully. 
    * @return KErrNotReady if call is not in ringing or queued state.
    * @pre Call state is MCCPCallObserver::ECCPStateRinging or MCCPCallObserver::ECCPStateQueued.
    * @since S60 3.2
    */
    virtual TInt Reject() = 0;

    /**
    * Reguests plug-in to queue the call. 
    * If the request cannot be started then an error will be returned immediately as return value. 
    * In succesfull case KErrNone will be returned and the requested action has been started.
    * In case call waiting is temporarily not supported when called returns not ready indication.
    * @param None
    * @return KErrNone if request was started succesfully. 
    * @return KErrNotReady if call is not in ringing state or call waiting is not supported at the moment.
    * @pre Call state is MCCPCallObserver::ECCPStateIdle.
    * @post When succesfull, call state is changed to MCCPCallObserver::ECCPStateQueued.
    * @post If queueing is not supported by the plug-in call is disconnected by the plug-in.
    * @since S60 3.2
    */
    virtual TInt Queue() = 0;

    /**
    * Starts dialing to recipient. Recipient was set at call creation.
    * If the request cannot be started then an error will be returned immediately as return value. 
    * In succesfull case KErrNone will be returned and the requested action has been started.
    * @param None
    * @return KErrNone if request was started succesfully. In this case plug-in must return 
    * MCCPCallObserver::ECCPStateDialling.
    * @return KErrNotReady if call is not in idle state.
    * @return KErrNotSupported If call is not mobile originated.
    * @pre Call state is MCCPCallObserver::ECCPStateIdle and call type is MO.
    * @since S60 3.2
    */
    virtual TInt Dial() = 0;

    /**
    * Indicate call is ringning.
    * If the request cannot be started then an error will be returned immediately as return value. 
    * In succesfull case KErrNone will be returned and the requested action has been started.
    * Optionally ringing state can be set for MO call too.
    * @param None
    * @return KErrNone if request was started succesfully.
    * @return KErrNotReady if call is not in idle state.
    * @pre Call state is MCCPCallObserver::ECCPStateIdle.
    * @since S60 3.2
    */
    virtual TInt Ringing() = 0;
      
    /**
    * Ends an ongoing call. 
    * Does not delete the call. Call is deleted when CConvergedCallProvider::ReleaseCall is called.
    * If the request cannot be started then an error will be returned immediately as return value. 
    * In succesfull case KErrNone will be returned and the requested action has been started.
    * @param None.
    * @return KErrNone if request was started succesfully.
    * @return KErrAlreadyExists if call is already in idle state.
    * @pre Call state is not MCCPCallObserver::ECCPStateIdle.
    * @since S60 3.2
    */
    virtual TInt HangUp() = 0;
      
    /**
    * Called by CCE instead of HangUp when Dial has been called and state change to 
    * MCCPCallObserver::ECCPStateConnecting has not been received. 
    * Cancels the last ongoing request. Used for Dial only. Not supported for other actions.
    * If the request cannot be started then an error will be returned immediately as return value. 
    * In succesfull case KErrNone will be returned and the requested action has been started.
    * @param None.
    * @return KErrNone if request was started succesfully. 
    * @since S60 3.2
    * @pre MCCPCall::Dial has been called and call state is MCCPCallObserver::ECCPIdle 
    */
    virtual TInt Cancel() = 0;
      
    /**
    * Puts call on hold.
    * If the request cannot be started then an error will be returned immediately as return value. 
    * In succesfull case KErrNone will be returned and the requested action has been started.
    * @param None.
    * @return KErrNone if request was started succesfully. 
    * @return KErrNotReady if call is not in connected state.
    * @return KErrAlreadyExists if call is already held (MCCPCallObserver::ECCPStateHold).
    * @pre Call state is MCCPCallObserver::ECCPStateConnected or MCCPCallObserver::ECCPStateTransferring.
    * Transferring state is allowed only if previous state before transferring state was on-hold.
    * @since S60 3.2
    */
    virtual TInt Hold() = 0;
      
    /**
    * Resume previously held call.
    * If the request cannot be started then an error will be returned immediately as return value. 
    * In succesfull case KErrNone will be returned and the requested action has been started.
    * @param None.
    * @return KErrNone if request was started succesfully.
    * @return KErrNotReady if call is not in on-hold state.
    * @return KErrAlreadyExists if call is already in connected state MCCPCallObserver::ECCPStateConnected.
    * @pre Call state is MCCPCallObserver::ECCPStateHold.
    * @since S60 3.2
    */
    virtual TInt Resume() = 0;
      
    /**
    * Swaps the call between Hold and Resume. 
    * If the request cannot be started then an error will be returned immediately as return value. 
    * In succesfull case KErrNone will be returned and the requested action has been started.
    * @param None.
    * @return KErrNone if request was started succesfully.
    * @return KErrNotReady if call is not on in holded or connected state.
    * @pre Call state is MCCPCallObserver::ECCPStateHold or MCCPCallObserver::ECCPStateConnected.
    * @since S60 3.2
    */
    virtual TInt Swap() = 0;
      
    /**
    * Returns the remote party number or address of the call. 
    * In case returned remote party value is KNullDesC it means the remote party is
    * regarded as anonymous.
    * @since S60 3.2
    * @param None
    * @return Remote party address or number
    */
    virtual const TDesC& RemoteParty() const = 0;

    /**
    * Returns the remote party name of the call. 
    * @since S60 5.0
    * @param None
    * @return Remote party name. KNullDesC if not available.
    */
    virtual const TDesC& RemotePartyName() = 0;

    /**
    * Returns the original dialled party. The original number (e.g. including DTMF) dialled for 
    * an outgoing call.
    * Can be different than returned from MCCPCall::RemoteParty().
    * @since S60 3.2
    * @param None
    * @return Dialled party,
    */
    virtual const TDesC& DialledParty() const = 0;
      
    /**
    * Return boolean value if the call is forwarded or not.
    * @since S60 3.2
    * @param None.
    * @return ETrue if call is forwarded, else EFalse.
    */
    virtual TBool IsCallForwarded() const = 0;
      
    /**
    * Returns boolean value if the call is mobile originated or not.
    * @since S60 3.2
    * @param None.
    * @return ETrue if call is mobile originated, otherwise EFalse.
    */
    virtual TBool IsMobileOriginated() const = 0;
      
    /**
    * Returns the state of the call.
    * @since S60 3.2
    * @param None.
    * @return The current state of the call (MCCPCallObserver::TCCPCallState).
    */
    virtual MCCPCallObserver::TCCPCallState State() const = 0;
      
    /**
    * Returns UID of the plug-in used for this call
    * @since S60 3.2
    * @param None.
    * @return TUid Implementation UID of the Plug-In
    */
    virtual TUid Uid() const = 0;
      
    /**
    * Returns call control caps for this call.
    * This tells for CCE what call can currently do.
    * @since S60 3.2
    * @param None.
    * @return Call control caps defined in MCCPCallObserver::TCCPCallControlCaps.
    */
    virtual MCCPCallObserver::TCCPCallControlCaps Caps() const = 0;      

    /**
    * Return if call secure status is specified or not.
    * @since Series 60 5.0
    * @param None.
    * @return TBool ETrue if security is specified, else EFalse.
    */
    virtual TBool SecureSpecified() const = 0;
    
    /**
    * Get call's secure status.
    * @since Series 60 3.2
    * @param None.
    * @return TBool ETrue if security is enabled, else EFalse.
    */
    virtual TBool IsSecured() const = 0;

    /**
    * Set call parameters.
    * @since S60 3.2
    * @param aNewParams New call paramater information.
    * @return none
    */
    virtual void SetParameters( const CCCPCallParameters& aNewParams ) = 0;

    /**
    * Cet call parameters.
    * @since S60 3.2
    * @param None
    * @return Current call paramater information.
    */
    virtual const CCCPCallParameters& Parameters() const = 0;

    /**
    * Returns the inband tone associated to an error condition or a state transition of a call.
    * See defenition for TCCPTone.
    * @since S60 3.2
    * @param None.
    * @return TCCPTone Used tone.
    */
    virtual TCCPTone Tone() const = 0;
      
    /**
    * Get forward provider. Must return the already created object if called 
    * again after initial creation.
    * @since Series 60 3.2
    * @param aObserver Observer class for forward events.
    * @return Pointer to MCCPForwardProvider if forwarding is supported for the call.
    * In unsupported case does not leave but returns NULL. 
    * @leave On error case, system error code.
    */
    virtual MCCPForwardProvider* ForwardProviderL( const MCCPForwardObserver& aObserver ) = 0;

    /**
    * Get transfer provider. Must return the already created object if called 
    * again after initial creation.
    * @since Series 60 3.2
    * @param aObserver Observer class for transfer events
    * @return Pointer to MCCPTransferProvider if transfer is supported for the call. 
    * In unsupported case does not leave but returns NULL. 
    * otherwise returns NULL but does not leave.in case forwarding is not supported for the call.
    * @leave On error case, system error code.
    */
    virtual MCCPTransferProvider* TransferProviderL( const MCCPTransferObserver& aObserver ) = 0;
      
    /**
    * Adds a new observer for MCCECall object. Called in MT call case. In MO 
    * case observer will be given at call construction. 
    * @since S60 v3.2
    * @param aObserver Observer
    * @return None.
    * @leave system error if observer adding fails.
    */
    virtual void AddObserverL( const MCCPCallObserver& aObserver ) = 0;

    /**
    * Remove an observer.
    * @since S60 v3.2
    * @param aObserver Observer
    * @return KErrNone if removed succesfully. 
    * @return KErrNotFound if observer was not found.
    */
    virtual TInt RemoveObserver( const MCCPCallObserver& aObserver ) = 0;
    };

#endif // MCCPCALL_H

// End of File
