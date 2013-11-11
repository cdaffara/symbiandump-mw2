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
* Description:  Interface for CCE call object
*
*/


#ifndef MCCECALL_H
#define MCCECALL_H

#include <e32base.h>
#include <e32std.h> // TTimeIntervalSeconds
#include <e32cmn.h> // TName
#include <badesca.h>

#include "ccpdefs.h"
#include "mccecallobserver.h"
#include "mccecscall.h"
#include "mccetransferinterface.h"
#include "mcceforwardinterface.h"
#include "mcceextensioninterface.h"
#include "cccecallparameters.h" // For CCCECallParameters

#include <ccpdefs.h>
#include <mccpcall.h> // For CCCPCallParameters




/**
 *  CCE call interface
 *  @lib cce.dll
 *  @since S60 v3.2
 */
class MCCECall : public MCCETransferInterface,
                 public MCCEForwardInterface,
                 public MCCECSCall
    {
public:




    /**
    * Set new MCCECallObserver observer to MCCECall object.
    * Called in case of incoming call. In that case call has no observer yet when it 
    * is created. In MO case observer is set when call is created. Repleces current observer 
    * if called at any other time.
    * @since S60 v3.2
    * @param aCallObserver New observer. TODO const
    * @return none
    */
    virtual void SetObserver( const MCCECallObserver& aObserver ) = 0;

/************************************************************************/
/*                             Basic call functions                     */
/************************************************************************/
   
    /**
    * Attempts to establish a connection using the address/number given .
    * in CCCE::OpenNewCallL.
    * @since S60 v3.2
    * @param aCallParams The call parameters used by the TSY (a TCallParamsPckg object. 
    * CS call specific). 
    * @return KErrNone if request was started succesfully. 
    * @return KErrNotReady if call is not in idle state
    * @return KErrNotSupported If call is not mobile originated
    * @pre Call state is MCCECallObserver::EStateIdle and call type is MO
    */
    virtual TInt Dial( const TDesC8& aDialParams ) = 0;

    /**
    * Puts a call on hold.
    * Operation results will come via MCCECallObserver. 
    * @since S60 v3.2
    * @param none
    * @return KErrNone if request was started succesfully. 
    * @return KErrNotReady if call is not in connected state
    * @return KErrAlreadyExists if call is already on hold
    * @pre Call state is MCCECallObserver::EStateConnected or MCCECallObserver::ECCPStateTransferring
    * Transferring state is allowed only if previous state before transferring state was on-hold.
    */
    virtual TInt Hold() = 0;

    /**
    * Resumes the call that is on hold.
    * Operation results will come via MCCECallObserver. 
    * @since S60 v3.2
    * @param none
    * @return KErrNone if request was started succesfully. 
    * @return KErrAlreadyExists if call is already on connected state
    * @return KErrNotReady if call is not in on-hold state
    * @pre Call state is MCCECallObserver::EStateHold 
    */
    virtual TInt Resume() = 0;
    
    /**
    * Swaps a connected call to holded state and resumes held call.
    * Currently called only for active call. 
    * For CS call calls Swap in CCP interface. 
    * For other calls holds the call first and then resumes the other call.
    * @since S60 v3.2
    * @param none
    * @return KErrNone if request was started succesfully. 
    * @return KErrNotReady if call is not on in connected state.
    * @pre Call is in connected state.
    * @pre Active and held call exist.
    */
    virtual TInt Swap() = 0;
    
    /**
    * Terminates a call.
    *
    * @since S60 v3.2
    * @param none
    * @return KErrNone if request was started succesfully. 
    * @return KErrAlreadyExists if call is already in idle state
    * @pre Call state is not MCCECallObserver::EStateIdle
    */
    virtual TInt HangUp() = 0;

    /**
    * Answer an incoming call
    *
    * @since S60 v3.2
    * @param none
    * @return KErrNone if request was started succesfully.
    * @return KErrNotReady if call is not in ringing or queued state.
    * @return KErrInUse connected and held call already exist.
    * @pre Call state is MCCECallObserver::EStateRinging or MCCECallObserver::EStateQueued
    */
    virtual TInt Answer() = 0;

    /**
    * Releases the call object.
    * This must be called to free all the resources reserved for the call.
    * After this call PE MUST not be access the call objectanymore and observer 
    * for the class must not be used in CCE.
    * This operation MUST always succeed. Client has to be sure call is  
    * cleared from the system and all resuorces are returned to the state before calling 
    * CCCE::OpenNewCallL.
    * It is CCE's internal issue how it handles the actual call resources etc, and no 
    * notification will result back from this operation to client. The operation means the same 
    * as deleting an object pointer in code from PE point of view.
    * @since S60 v3.2
    * @param none
    * @return KErrNone
    * @pre None, can always be called.
    * @post CCE must quarantee all resources for the call will be cleaned up. Call MUST not 
    * be referenced anymore from PE and CCE must not call MCCECallObserver after this call!
    */
    virtual TInt Release() = 0;

    /**
    * Replaces active call with incoming call
    * @since S60 3.2
    * @param none
    * @return KErrNone if request was started succesfully.
    * @return KErrNotSupported in case when request could not be executed
    */
    virtual TInt ReplaceActive() = 0;

/************************************************************************/
/*                             Util                                    */
/************************************************************************/
    /**
    * Returns the service-id used for the call.
    * @since S60 3.2
    * @param none
    * @return TUint32 Service-id
    */
    virtual TUint32 ServiceId() const = 0;

    /**
    * Returns duration of a call. 
    * CCE will count duration from connected state to idle state.
    *
    * @since S60 v3.2
    * @param none
    * @return On return contains the call duration in seconds.
    */
    virtual TTimeIntervalSeconds CallDuration() const = 0;

    /**
    * Returns start time of the call.
    *
    * @since S60 v3.2
    * @param none
    * @return On return contains the call start time.
    */
    virtual TDateTime StartTime() const = 0;
    
    /**
    * Return call control capabilities defined in MCCECallObserver::TCCECallControlCaps.
    *
    * @since S60 v3.2
    * @param none
    * @return Call control capabilities
    */
    virtual MCCECallObserver::TCCECallControlCaps Caps() const = 0;

    /**
    * Return current call state.
    *
    * @since S60 v3.2
    * @param none
    * @return Current call state
    */
    virtual CCPCall::TCallState State() const = 0;

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
    * Returns if call is an emergency call or not.
    * @since Series 60 3.2
    * @param None.
    * @return TBool ETrue call is emergency call else EFalse.
    */
    virtual TBool IsEmergency() const = 0;

    /**
    * Returns the remote party number or address of the call. 
    * In case returned remote party value is KNullDesC it means the remote party is
    * regarded as anonymous.
    * Can be different than returned from MCCECall::DialledParty().
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
    virtual const TDesC& RemotePartyName() const = 0;

    /**
    * Returns the original dialled party. The original number (e.g. including DTMF) dialled 
    * for an outgoing call.
    * Can be different than returned from MCCECall::RemoteParty().
    * @since S60 3.2
    * @param None
    * @return Dialled party,
    */
    virtual const TDesC& DialledParty() const = 0;

    /**
    * Set call parameters.
    * @since S60 3.2
    * @param aNewParams New call paramater information.
    * @return none
    */
    virtual void SetParameters( const CCCECallParameters& aNewParams ) = 0;

    /**
    * Get call parameters.
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
    * Get call information. NOTE: will be removed shortly. Stop using this call use relevant getters instead!
    * TODO this needs to be rechecked if params are to be defined in CCE as a struct for example and to return that!
    * @since S60 3.2
    * @param aCallInfo TMobileCallInfoV3 - TODO check this
    * @return none
    */
    virtual void GetMobileCallInfo( TDes8& aCallInfo ) const = 0;
 
/************************************************************************/
/*                             Emergency call                           */
/************************************************************************/
    /**
    * Places an emergency call request. When starting the request firrst terminates 
    * all but ringing and queued calls.
    * @since S60 v3.2
    * @param aNumber The emergency number / address
    * @return none
    */
    virtual void DialEmergencyCall( const TDesC& aNumber ) = 0;
    
    /**
     * Returns the call index of the call. The call index is valid
     * only for calls, which are in non idle states.
     * @since S60 5.2
     * @return Call index
     */
     virtual TInt CallIndex() const = 0;
    };

#endif // MCCECALL_H

