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


#ifndef MCCPREFEMERGENCYCALL_H
#define MCCPREFEMERGENCYCALL_H

//  INCLUDES
#include <e32base.h>
#include <mccpemergencycall.h>

class CCPRefConvergedCallProvider;
class MCCPCallObserver;


/**
* Emergency call class
*/
class CCPRefEmergencyCall : public CBase,
                            public MCCPEmergencyCall
	{
public:

    /*
    * @param aProvider provider that owns this object.
    * @param aRecipient call recipient
    * @param aObserver call observer 
    * @return created CCPRefEmergencyCall object
    */
    static CCPRefEmergencyCall* NewL( const CCPRefConvergedCallProvider* aProvider,
                                      const TDesC& aRecipient,
			                          MCCPCallObserver* aObserver );

    /*
    * dtor
    */
    ~CCPRefEmergencyCall();

 	/**
	* Creates a Emergency call with the recipient.
	* @since S60 3.2
	*/
	TInt Dial( const TDesC& aRecipient=KNullDesC );
	/**
	 * Cancels ongoing dial request
	 * @since S60 3.2
	 */
	 TInt Cancel();

	/**
	* Ends an ongoing call.
	* @since S60 3.2
	*/
	TInt HangUp();

	/**
	* Answers to an incoming call.
	* @since S60 3.2
	*/
	TInt Answer();

	/**
	* Set observer. Not in API. 
	* @since S60 3.2
	*/
	void SetObserver( MCCPCallObserver* aObserver);


private:
    /**
    * constructor.
    */
    CCPRefEmergencyCall( const CCPRefConvergedCallProvider* aProvider );

    /**
    * By default Symbian 2nd phase constructor is private.
    */
    void ConstructL( const TDesC& aRecipient,
 		            MCCPCallObserver* aObserver);

    mutable CCPRefConvergedCallProvider* iProvider;
    HBufC* iRecipient;
    MCCPCallObserver* iObserver;
	};


#endif // MCCPREFEMERGENCYCALL_H
