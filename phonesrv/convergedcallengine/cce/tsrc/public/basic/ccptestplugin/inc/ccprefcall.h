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
* Description:  CCP test plug-in implementation.
*
*/


#ifndef CCPREFCALL_H
#define CCPREFCALL_H

//  INCLUDES
#include <e32base.h>
#include <e32cmn.h>
#include <mccpcall.h>
#include <mccpcallobserver.h>
#include <mccpcscall.h>

// Forward declarations

class MCCPCall;
class MCCPCSCall;
class CCPRefConvergedCallProvider;
class CCPRefForwardProvider;
class MCCPForwardObserver;
class CCPRefTransferProvider;
class MCCPTransferObserver;
class MCCPObserver;

/**
*
*/
class CCPRefCall : public CBase,
                   public MCCPCSCall
    {
    
public:

    enum TCCPRefCallAction
        {
        ECCPRefCallActionDial,
        ECCPRefCallActionAnswer,
        ECCPRefCallActionReject,
        ECCPRefCallActionHold,
        ECCPRefCallActionResume,
        ECCPRefCallActionQueue,
        ECCPRefCallActionRinging,
        ECCPRefCallActionTransfer,
        ECCPRefCallActionForward
        };

    /*
    * @param aProvider provider that owns this object.
    * @param aServiceId Service id used
    * @param aRecipient call recipient
    * @param aObserver call observer 
    * @return created CCPRefCall object
    */
    static CCPRefCall* NewL( CCPRefConvergedCallProvider* aProvider,
                             const TDesC& aRecipient,
		                     MCCPCallObserver* aObserver,
		                     TUid aType );

    /*
    * dtor
    */
    ~CCPRefCall();
      
    /**
    * Answers to an incoming call.
    * @since S60 3.2
    */
    TInt Answer();
      
    /**
    * Rejects an incoming call.
    * @since S60 3.2
    */
    TInt Reject();

    /**
    * Reguests plug-in to queue the call. 
    * @since S60 3.2
    */
    TInt Queue();

    /**
    * Starts dialing to recipient
    * @since S60 3.2
    */
    TInt Dial();

    /**
    * Call is ringing
    * @since S60 3.2
    */
    TInt Ringing();
      
    /**
    * Ends an ongoing call.
    * @since S60 3.2
    */
    TInt HangUp();
      
    /**
    * Cancels the ongoing request
    * @since S60 3.2
    */
    TInt Cancel();
      
    /**
    * Puts the specified call on hold.
    * @since S60 3.2
    */
    TInt Hold();
      
    /**
    * Resumes the call.
    * @since S60 3.2
    */
    TInt Resume();
      
    /**
    * Swaps a connected call to its opposite state, either active or on hold
    * @since S60 3.2
    */
    TInt Swap();

    /*** CS specific methods ***/
    TInt Dial( const TDesC8& aCallParams );
    TInt GetMobileCallInfo( TDes8& aCallInfo ) const;
    TInt SwitchAlternatingCall();
    TInt GetMobileDataCallCaps( TDes8& aCaps ) const;
    void NoFDNCheck();
    TBool LogDialedNumber() const;
    
      
    /**
    * Returns the recipient of the call.
    * @since S60 3.2
    * @return recipient address,
    */  
    const TDesC& RemoteParty() const;
    
    /** @see MCCPCall::RemotePartyName */
    const TDesC& RemotePartyName();
    
    /**
    * Returns the original dialled party. The original number (e.g. including DTMF) dialled for 
    * an outgoing call.
    * Can be different than returned from MCCPCall::RemoteParty().
    * @since S60 3.2
    * @param None
    * @return Dialled party,
    */
    const TDesC& DialledParty() const;
      
    /**
    * Return boolean value if the incoming call is forwarded or not.
    * @since S60 3.2
    * @return ETrue if call is forwarded, else EFalse
    */
    TBool IsCallForwarded() const;
     
    /**
    * Returns boolean value if the call is mobile originated or not.
    * @since S60 3.2
    * @return ETrue if call is mobile originated, otherwise EFalse
    */
    TBool IsMobileOriginated() const;
      
    /**
    * Returns the state of the call.
    * @since S60 3.2
    * @return TCCPCallState The current state of the call
    */
    MCCPCallObserver::TCCPCallState State() const;
      
    /**
    * Returns service-id
    * @since S60 3.2
    * @return TUint32 service-id
    */
    
    // poistettu metodi
    //TUint32 ServiceId() const;
      
    /**
    * Returns TUid of the plug-in used for this call
    * @since S60 3.2
    * @return TUid Implementation UID of the Plug-In
    */
    TUid Uid() const;
      
    /**
    * Returns call control caps for this call.
    * This tells for CCE what call can currently do.
    * @since S60 3.2
    * @param None.
    * @return Call control caps defined in MCCPCallObserver::TCCPCallControlCaps.
    */
    MCCPCallObserver::TCCPCallControlCaps Caps() const;     
    /**
    * Get call's secure status.
    * @since Series 60 3.2
    * @return TBool ETrue if security is enabled, else EFalse.
    */
    TBool IsSecured() const;
    
    /**
    * Return if call secure status is specified or not.
    * @since Series 60 5.0
    * @param None.
    * @return TBool ETrue if security is specified, else EFalse.
    */
    TBool SecureSpecified() const;
    
    /**
	* Set call parameters.
	* @since S60 3.2
	* @param aNewParams New call paramater information.
	* @return none
	*/
    void SetParameters( const CCCPCallParameters& aNewParams );
    
    /**
    * Returns the inband tone associated to an error condition or a state transition of a call.
	* See defenition for TCCPTone.
    * @since S60 3.2
    * @param None.
    * @return TCCPTone Used tone.
    */
    TCCPTone Tone() const;

    /**
	* Cet call parameters.
	* @since S60 3.2
	* @param None
	* @return Current call paramater information.
	*/
    const CCCPCallParameters& Parameters() const;
          
	/**
	* Get Forward provider
	* @since Series 60 3.2
    * @param aObserver - observer class
	* @return Pointer to MCCPForwardProvider if succesfull, NULL if not available
    * @leave on error case system error code
	*/
	MCCPForwardProvider* ForwardProviderL( const MCCPForwardObserver& aObserver );

	/**
    * Get Call transfer provider
    * @since Series 60 3.2
    * @param aObserver - observer class
    * @return Pointer to MCCPTransferProvider if succesfull, NULL if not available
    * @leave on error case system error code
    */
	MCCPTransferProvider* TransferProviderL( const MCCPTransferObserver& aObserver );
      
    /**
    * Adds a new observer for MCCECall object. Called in MT call case. In MO 
    * case observer will be given at call construction.
    * Plug-in dependent feature if duplicates or more than one observers 
    * are allowed or not. Currently CCE will set only one observer.
    * @since S60 v3.2
    * @param none
    * @param aObserver Observer
    * @return none
    * @leave system error if observer adding fails
    */
    void AddObserverL( const MCCPCallObserver& aObserver );

    /**
    * Remove an observer.
    * @since S60 v3.2
    * @param none
    * @param aObserver Observer
    * @return KErrNone if removed succesfully. KErrNotFound if observer was not found.
    * Any other system error depending on the error.
    */
    TInt RemoveObserver( const MCCPCallObserver& aObserver );
    
	// Not in API
    /**
    * Get observer
    */
    MCCPCallObserver* Observer() const;

    /**
    * A central place for checking if the requested action ok to perform
    */
    TInt IsActionOK( const CCPRefCall::TCCPRefCallAction aAction );
    
private: // functions

    /**
    * constructor.
    */
    CCPRefCall( CCPRefConvergedCallProvider* aProvider, TUid aType );

    /**
    * By default Symbian 2nd phase constructor is private.
    */
    void ConstructL( const TDesC& aRecipient,
 	                 MCCPCallObserver* aObserver);
    
    void IncomingTransfer();
        
    TReal ReadNumber(TPtrC& aInput);
    
    void DeleteSelf();
    
    TPtrC16 ConvertLowerCase(TDes* aDesc);
    
    void Delay( TInt delay );
    
    
    
public:	// functions

	/**
    * Set and notify new call state
    */
    void SetNewState( const MCCPCallObserver::TCCPCallState aNewState );
    
    /* Used no disable event sending when internally changing states
    *  with 
    */
    void DontNotifyNext();
    
    static TInt CheckForwardRequest(HBufC* aInput);
    
    void SetMobileOriginated(TBool aIsMobileOriginated);
    
    TBool IsInConference( CCPRefCall& aCall );
    
    TBool IsCallActive();

	CCPRefConvergedCallProvider& Provider();
            
private: // data

    TBool iIsOnHold;
    TBool iIsForwared;
    TBool iIsQueued;
    TBool iIsMobileOriginated;
    TBool iIsSecured;
    
    MCCPCallObserver::TCCPCallControlCaps iCaps;
    TUid iType;
    HBufC* iRecipient;
    TBool iNotifyAction;	// see DontNotifyNext()
    TInt iForwardAction;
    TCCPTone iTone;
         
    CCPRefConvergedCallProvider& iProvider;
    
    CCPRefForwardProvider* iForwardProvider;
    
    CCCPCallParameters* iCallParameters;
    
public:	// data

	TBool iIsCallActive;

	MCCPCallObserver::TCCPCallState iCallState;
	mutable MCCPCallObserver* iCallObserver;
    
    CCPRefTransferProvider* iTransferProvider;


    };

#endif      // CCPREFCALL_H

// End of File
