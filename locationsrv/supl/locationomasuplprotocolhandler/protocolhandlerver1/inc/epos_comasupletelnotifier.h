/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   class for receiving network change event 
*
*/


#ifndef EPOS_COMASUPLETELNOTIFIER_H
#define EPOS_COMASUPLETELNOTIFIER_H

// INCLUDES
#include <e32def.h>
#include <e32base.h>
#include <etelmm.h>

//Forward declaration
class COMASuplLocationId;
class RMobilePhone;
class COMASuplTrace;

/**
*  Observer class for listening to Etel Events
*  @lib eposomasuplprotocolhandler.lib
*  @since S60 v3.1u
*/

class MOMASuplETelNotifierObserver
	{
	public:
		// ETel Notifier invoked for current network change 
		virtual void HandleCurrentNetworkChangeL(TInt aErrorCode, TBool aHomeNw) = 0;

		virtual void HandleRoamingCheckCompleteL(TInt aErrorCode, TBool aHomeNw) = 0;
	};

/**
*  Class for getting the network change info 
*  @lib eposomasuplprotocolhandler.lib
*  @since S60 v3.2
*/

class COMASuplETelNotifier : public CActive
    {
    public:            
		enum TCellReqType
			{
				EOMASuplNone = 0,			
				EOMASuplCellIdHome,			
				EOMASuplCellIdCurrent,
                EOMASuplCellIdNotify
			};   
    public:  // Constructors and destructor
        
   		 /**
        * Performs construction of an object
        * @since S60  v3.1u
        * @param None
        * @return None
        */
   		static COMASuplETelNotifier* NewL(MOMASuplETelNotifierObserver& aObserver,RMobilePhone& aPhone);
   		
        /**
        * Performs destructor
        * @since S60  v3.1u
        * @param None
        * @return None
        */
        ~COMASuplETelNotifier();
        
        /**
        * Checks whether the terminal is in the roming mode or not 
        * @since S60  v3.2
        * @param None
        * @return None
        */        
        void CheckForRoaming();

        void GetCurrentNetwork();

        void NotifyCurrentNetworkChange();
        
        
     private:
     
        void CompareForRoamingL();

        void CompeteRoamingCheckL();
     	
     private:
     
       /**
        * Performs construction of an object
        * @since S60  v3.1u
        * @param aObserver, MOMASuplETelNotifierObserver observer for request completion.
        * @param aPhone,RMobilePhone 
        * @return None
        */
        COMASuplETelNotifier(MOMASuplETelNotifierObserver& aObserver,RMobilePhone& aPhone);

     	/**
        * Performs Second phase constrcution
        * @since S60  v3.1u
        * @param None
        * @return None
        */
        void ConstructL();

    protected:  // CActive        
    	 /**
        * Performs cancelling of asynchronour request
        * @since S60  v3.1u
        * @param None
        * @return None
        */ 
        void DoCancel();

        /**
        * Performs RunL
        * @since S60  v3.1u
        * @param None
        * @return None
        */ 
        void RunL();  
        
        /**
        * Performs RunError
        * @since S60  v3.1u
        * @param aError, Error code containing leave reason
        * @return TInt , informs to activescheduler about handling of leave.
        */ 
        TInt RunError(TInt aError);  
        
    private:    // Data
  
      	//For access to a mobile phone
        RMobilePhone& iPhone; 
        
        // For Network information 
        RMobilePhone::TMobilePhoneNetworkInfoV1 iNetworkInfo;
        
        //for network information
        RMobilePhone::TMobilePhoneNetworkInfoV1Pckg iNetworkInfoPkg;
        
     	 // For location information
        RMobilePhone::TMobilePhoneLocationAreaV1 iLocationInfo;

        // For Network information 
        RMobilePhone::TMobilePhoneNetworkInfoV1 iHomeNetworkInfo;
        
        //for network information
        RMobilePhone::TMobilePhoneNetworkInfoV1Pckg iHomeNetworkInfoPkg;
        
     	 // For location information
        RMobilePhone::TMobilePhoneLocationAreaV1 iHomeLocationInfo;

		//Trace Utility
		COMASuplTrace* iTrace;
		
        //Observer for request completion
        MOMASuplETelNotifierObserver& iObserver;

		// For type of request... home or current 
		TCellReqType iCellMode;
    };

#endif      // EPOS_COMASUPLETELNOTIFIER_H
            
// End of File
