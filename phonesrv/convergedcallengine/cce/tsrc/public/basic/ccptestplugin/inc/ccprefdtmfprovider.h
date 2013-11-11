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


#ifndef CCPREFDTMFPROVIDER_H
#define CCPREFDTMFPROVIDER_H

//  INCLUDES
#include <e32base.h>
#include <mccpdtmfprovider.h>


class MCCPDTMFObserver;
class CCPRefConvergedCallProvider;

/**
*
*/
class CCPRefDTMFProvider : public CBase,
                           public MCCPDTMFProvider
	{
public:

    /*
    * @param aProvider provider that owns this object.
	* @return created CCPRefDTMFProvider object
    */
    static CCPRefDTMFProvider* NewL( const CCPRefConvergedCallProvider& aProvider );

    /*
    * dtor
    */
    ~CCPRefDTMFProvider();    

	/**
	* Cancels asynchronous DTMF string sending
	* @param none
	* @return KErrNone if succesfull, otherwise a system wide error code
	* @since Series 60 3.2
	*/
	TInt CancelDtmfStringSending();

	/**
	* Starts the transmission of a single DTMF tone across a
	* connected and active call.
	* @since S60 3.2
	* @param aTone Tone to be played.
	* @return KErrNone if succesfull, otherwise a system wide error code
	*/
	TInt StartDtmfTone( const TChar aTone );

	/**
	* Stops playing current DTMF tone.
	* @since S60 3.2
	* @return KErrNone if succesfull, otherwise a system wide error code
	*/
	TInt StopDtmfTone();

	/**
	* Plays DTMF string.
	* @since S60 3.2
	* @param aString This is the string to be played.
	* @return KErrNone if succesfull, otherwise another system wide error code
	* KErrArgument if the specified string contains illegal DTMF characters
	*/
	TInt SendDtmfToneString( const TDesC& aString );

	/**
	* Continue or cancel sending DTMF string which was stopped with 'w'-character
	* in string.
	* @since S60 3.2
	* @param aContinue ETrue if sending of the DTMF string should continue,
	* EFalse if the rest of the DTMF string is to be discarded.
	* @return KErrNone if succesfull, otherwise another system wide error code
	*/
	TInt ContinueDtmfStringSending( const TBool aContinue );
	
    /**
    * Add an observer for dtmf related events.
    * Plug-in dependent feature if duplicates or more than one observers 
    * are allowed or not. Currently CCE will set only one observer.
    * @since S60 v3.2
    * @param aObserver Observer
    * @return none
    * @leave system error if observer adding fails
    */
    void AddObserverL( const MCCPDTMFObserver& aObserver );

    /**
    * Remove an observer.
    * @since S60 v3.2
    * @param none
    * @param aObserver Observer
    * @return KErrNone if removed succesfully. KErrNotFound if observer was not found.
    * Any other system error depending on the error.
    */
    TInt RemoveObserver( const MCCPDTMFObserver& aObserver );
    
    void ClearDtmfString();
	
protected:

	void InitializeStringL(const TDesC& aStringNotSent);

private:

    /**
    * constructor.
    */
    CCPRefDTMFProvider( const CCPRefConvergedCallProvider& aProvider );

    /**
    * By default Symbian 2nd phase constructor is private.
    */
    void ConstructL();
	
    mutable CCPRefConvergedCallProvider& iProvider;
    const MCCPDTMFObserver* iObserver;
    
    /**
     * Dtmf string (tones not sent).
     */
    HBufC* iStringNotSent;
	};


#endif // CCPREFDTMFPROVIDER_H

