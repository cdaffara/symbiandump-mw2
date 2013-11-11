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


#ifndef CCPREFCONFERENCECALL_H
#define CCPREFCONFERENCECALL_H

//  INCLUDES
#include <e32base.h>
#include <mccpconferencecall.h>

// Forward declarations

class CCPRefCall;
class MCCPCall;
class MCCPConferenceCallObserver;
class CCPRefConvergedCallProvider;

/**
*
*/
class CCPRefConferenceCall : public CBase,
                             public MCCPConferenceCall
	{
public:

    /*
	* @param aMyCall call that owns this object.
    */
    CCPRefConferenceCall( CCPRefConvergedCallProvider& aProvider );
    
    /*
    * dtor
    */
    ~CCPRefConferenceCall();

    /**
    * Returns the service-id used for the call.
    * @since S60 3.2
    * @param None.
    * @return Service-id of the call.
    */
    TUint32 ServiceId() const;
    
    /*
    * Ends an ongoing conference call.
    * Does not delete the call. Call deletion is requested Release will be called.
    * In case of request cannot be triggered or an immediate error is noticed return
    * error immediately using return value. In that case the request has not been started.
    * Plug-in may notify an error via observer class too.
    * @param none
    * @return In case of an immediate error returns system error code
    * @since S60 3.2
    */
    TInt HangUp();

    /**
    * Puts conference call on hold.
    * In case of request cannot be triggered or an immediate error is noticed return
    * error immediately using return value. In that case the request has not been started.
    * Plug-in may notify an error via observer class too.
    * @param none
    * @return In case of an immediate error returns system error code
    * @since S60 3.2
    */
    TInt Hold();
      
    /**
    * Resumes previously held conference call.
    * In case of request cannot be triggered or an immediate error is noticed return
    * error immediately using return value. In that case the request has not been started.
    * Plug-in may notify an error via observer class too.
    * @param none
    * @return In case of an immediate error returns system error code
    * @since S60 3.2
    */
    TInt Resume();

    /**
    * Swaps a connected conference call between Hold to Resume. 
    * In case of request cannot be triggered or an immediate error is noticed return
    * error immediately using return value. In that case the request has not been started.
    * Plug-in may notify an error via observer class too.
    * @param none
    * @return In case of an immediate error returns system error code
    * @since S60 3.2
    */
    TInt Swap();

	/**
	* Adds new call to conference call.
	* @since S60 3.2
	* @param aCall Address of the call to be added to conference
	*/
	void AddCallL( MCCPCall* aCall );

	/**
	* Adds new call to conference call.
	* @since S60 3.2
	* @param aCall Address of the call to be added to conference
	*/
	void RemoveCallL( MCCPCall* aCall );

	/**
	* Returns number of calls in conference call.
	* @since S60 3.2
	* @param aCallCount Number of calls in conference.
	* @return KErrNone if succesfull, otherwise another system wide error code
	*/
	TInt CallCount() const;
   /**
	* Private call with one call from conference call.
	* @since S60 3.2
	* @param aCall
	*/
	void GoOneToOneL( MCCPCall& aCall );

    /**
	* Add all current calls in plug-in to conference.
	* @since S60 3.2
	* @param none
	* @return In error case leaves. 
	* KErrArgument, KErrNotFound
	*/
	void CurrentCallsToConferenceL();

    /**
    * Add an observer for forward related events.
    * Plug-in dependent feature if duplicates or more than one observers 
    * are allowed or not. Currently CCE will set only one observer.
    * @since S60 v3.2
    * @param aObserver Observer
    * @return none
    * @leave system error if observer adding fails
    */
    void AddObserverL( const MCCPConferenceCallObserver& aObserver );

    /**
    * Remove an observer.
    * @since S60 v3.2
    * @param none
    * @param aObserver Observer
    * @return KErrNone if removed succesfully. KErrNotFound if observer was not found.
    * Any other system error depending on the error.
    */
    TInt RemoveObserver( const MCCPConferenceCallObserver& aObserver );
    
    TInt GetCallArray( RPointerArray<MCCPCall>& aCallArray );


     // Not in API. Has Release been called or not
    TBool IsReleased();
    
   	/* Remove conference call from array
   	*/
    void RemoveConfCall( CCPRefCall& aCall );
    
    void SetCallState(const MCCPCallObserver::TCCPCallState aNewState );
    
private: // functions
    
    void ConferenceActive();
    
public: // data
    // conference call pointers
    RPointerArray<MCCPCall> iConfCallArray;
private: // data
    
    CCPRefConvergedCallProvider* iProvider;
    MCCPConferenceCallObserver* iObserver;

    // has Release been called or not    
    TBool iIsReleased;
    
    TBool iIsOnHold; // is conference on hold
    TBool iIsActive; // is conference ongoing
	};


#endif // CCPREFCONFERENCECALL_H

