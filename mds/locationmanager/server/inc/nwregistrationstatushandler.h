/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Helper class to retrieve UE network registration status
*
*/

#ifndef __NW_REGISTRATION_STATUS_HANDLER_H__
#define __NW_REGISTRATION_STATUS_HANDLER_H__


// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <etelmm.h>

NONSHARABLE_CLASS(CNwRegistrationStatusHandler) : public CActive
{
    
public:

	/**
	  * 1st phase constructor
	  * @param aMobilePhone an instance of the RMobilePhone to get the current network
	  */
     static CNwRegistrationStatusHandler* NewL(RMobilePhone& aMobilePhone); 

	/**
	  * destructor
	  */	
     ~CNwRegistrationStatusHandler();

	/**
	  * Register for getting the n/w registration change notification
	  * starts notifying the current network changes
	  */	
     void StartNotifier();

	 /**
	   * Retrieve network registration status
	   * @return the mobilephone reg status
	   */

     RMobilePhone::TMobilePhoneRegistrationStatus GetNetworkRegistrationStatus() const;
     
protected:

	/**
	  * RunL
	  * from CActive
	  */	
      void RunL();

	/**
	  * DoCancel
	  * from CActive
	  */	
      void DoCancel();
 private:

	/**
	  * Second phase construction
	  */	
      void ConstructL();
      CNwRegistrationStatusHandler(RMobilePhone& aMobilePhone);
      


private:     
    typedef enum 
        {
        ERegStatusOptStateNone = 0x00,
        ERegStatusOptStateGet,
        ERegStatusOptStateNotify
        }TRegStatusOptState;
    
	 /**
	   * An instance of the RMobilePhone to get the current network
	   */
      RMobilePhone& iMobilePhone;

	  /**
	     * Indicates the network registration status
	     */
      RMobilePhone::TMobilePhoneRegistrationStatus iRegistrationStatus;
      TRegStatusOptState iState;
};


#endif /*__NW_REGISTRATION_STATUS_HANDLER_H__*/

// End of file

