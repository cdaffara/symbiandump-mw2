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


#ifndef MCCPREFFORWARDPROVIDER_H
#define MCCPREFFORWARDPROVIDER_H

//  INCLUDES
#include <e32base.h>
#include <badesca.h> // CDesC8Array
#include <mccpforwardprovider.h>

class CCPRefCall;
class MCCPForwardObserver;

/**
* Call forward methods.
*/
class CCPRefForwardProvider : public CBase,
                              public MCCPForwardProvider
	{
public:

    /*
    * @param aMyCall call that owns this object.
	* @return created CCPRefForwardProvider object
    */
    static CCPRefForwardProvider* NewL( const CCPRefCall& aMyCall );

    /*
    * dtor
    */
    ~CCPRefForwardProvider();    
	

	/**
	* Returns pointer to array containing addresses received with 300
	* multiple choices response
	* @since Series 60 3.2
	* @param aCCPCall reference of call which notified about multipe choices
	* @return Reference to array containing addresses
	*/
	const CDesC8Array& GetForwardAddressChoicesL();

	/**
	* Notification from UI to Multiple choices query
	* multiple choices response
	* @since Series 60 3.2
	* @param aIndex index of address where the call is to be forwarded
	*/
	void ForwardToAddressL( const TInt aIndex );

    /**
    * Add an observer for forward related events.
    * Plug-in dependent feature if duplicates or more than one observers 
    * are allowed or not. Currently CCE will set only one observer.
    * @since S60 v3.2
    * @param aObserver Observer
    * @return none
    * @leave system error if observer adding fails
    */
    void AddObserverL( const MCCPForwardObserver& aObserver );

    /**
    * Remove an observer.
    * @since S60 v3.2
    * @param none
    * @param aObserver Observer
    * @return KErrNone if removed succesfully. KErrNotFound if observer was not found.
    * Any other system error depending on the error.
    */
    TInt RemoveObserver( const MCCPForwardObserver& aObserver );


private:
    /**
    * constructor.
    */
    CCPRefForwardProvider( const CCPRefCall& aMyCall );

    /**
    * By default Symbian 2nd phase constructor is private.
    */
    void ConstructL();
    
    void SendForwardEvents();
	

private:
	/*const*/ CCPRefCall& iMyCall;

    // forward choice list
    CDesC8Array *iMultipleChoicesArrayPtr;
    
public:
    MCCPForwardObserver* iObserver;
	};


#endif //MCCPREFFORWARDPROVIDER_H
