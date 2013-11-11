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
* Description:   class for receiving IAP change event 
*
*/


#ifndef EPOS_COMASUPLIAPCHANGENOTIFIER_H
#define EPOS_COMASUPLIAPCHANGENOTIFIER_H

// INCLUDES
#include <e32def.h>
#include <e32base.h>
#include <commdb.h>

//Forward declaration
class COMASuplTrace;
class CCommsDatabase;
/**
*  Observer class for listening to Etel Events
*  @lib eposomasuplprotocolhandler.lib
*  @since S60 v3.1u
*/

class MOMASuplIapChangeNotifierObserver
	{
	public:
		// IAP change Notifier invoked for IAP settings change 
		virtual void HandleIapSettingsChangeL(TInt aErrorCode) = 0;
	};

/**
*  Class for getting the network change info 
*  @lib eposomasuplprotocolhandler.lib
*  @since S60 v3.2
*/

class COMASuplIapNotifier : public CActive
    {
    public:            
    public:  // Constructors and destructor
        
   		 /**
        * Performs construction of an object
        * @since S60  v3.1u
        * @param None
        * @return None
        */
   		static COMASuplIapNotifier* NewL(MOMASuplIapChangeNotifierObserver& aObserver);
   		
        /**
        * Performs destructor
        * @since S60  v3.1u
        * @param None
        * @return None
        */
        ~COMASuplIapNotifier();

        void NotifyIapSettingsChange();
        
     private:
     
       /**
        * Performs construction of an object
        * @since S60  v3.1u
        * @param aObserver, MOMASuplIapChangeNotifierObserver observer for request completion.
        * @return None
        */
        COMASuplIapNotifier(MOMASuplIapChangeNotifierObserver& aObserver);

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
  
      	//For access to CommsDB
        CCommsDatabase* iCommDb; 

		//Trace Utility
		COMASuplTrace* iTrace;
		
        //Observer for request completion
        MOMASuplIapChangeNotifierObserver& iObserver;

    };

#endif      // EPOS_COMASUPLETELNOTIFIER_H
            
// End of File
