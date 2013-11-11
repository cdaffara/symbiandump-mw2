/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Declaration of CPhoneSettingsTestNetwork class.
*
*/



#ifndef PHONESETTINGSTESTNETWORK_H
#define PHONESETTINGSTESTNETWORK_H

#include <e32base.h>
#include "PhoneSettingsTestParser.h"

// Forward declarations 
class CPhoneSettingsTestNetworkInfoObserver;
class CPhoneSettingsTestNetworkModeObserver;
class CPsetNetwork; 

// CLASS DECLARATION

NONSHARABLE_CLASS(CPhoneSettingsTestNetwork) : public CBase     
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CPhoneSettingsTestNetwork* NewL(
            CPhoneSettingsTestParser& aPhoneSettingsTestParser,
            MPhoneSettingsTestMessageHandler& aPhoneSettingsTestMessageHandler );

        /**
        * Destructor.
        */
        virtual ~CPhoneSettingsTestNetwork();

    private:

        /**
        * C++ default constructor.
        */
        CPhoneSettingsTestNetwork(
                    CPhoneSettingsTestParser& aPhoneSettingsTestParser,
                    MPhoneSettingsTestMessageHandler& aPhoneSettingsTestMessageHandler );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private: // Functions for handling CActiveSchedulerWait 
    
        /**
        * CActiveSchedulerWait callback function  
        */        
        static TInt DoCallBackL( TAny* aAny );
         
        /**
        * CActiveSchedulerWait wait function 
        */        
        void WaitTimer( TInt aWaitTime );
        
        /**
        * CActiveSchedulerWait stop function 
        */        
        void Stop();

    public: // Test functions 
    
        /**
        * SetupL 
        * Set up tester 
        */        
        void SetupL();
        
        /**
        * TearDownL
        * Cleanup tester 
        */        
        void TearDownL();
            
        /**
        * Test CPsetNetwork construction and destruction 
        * Function type: Library
        */        
        TInt TestCPsetNetworkConstructAndDestructL(TTestResult& aResult);

        /**
        * Test CPsetNetwork::GetCurrentNetworkInfo
        * Function type: Library
        */        
        TInt TestGetCurrentNetworkInfo(TTestResult& aResult);
        
        /**
        * Test CPsetNetwork::GetNetworkSelectMode
        * Function type: Library
        */        
        TInt TestGetNetworkSelectMode(TTestResult& aResult);
        
        /**
        * Test CPsetNetwork::IsCallActive
        * Function type: Library
        */        
        TInt TestIsCallActive(TTestResult& aResult);
        
        /**
        * Test CPsetNetwork::IsGPRSConnected
        * Function type: Library
        */        
        TInt TestIsGPRSConnected(TTestResult& aResult);
        
        /**
        * Test CPsetNetwork::ResetNetworkSearch
        * Function type: Library
        */        
        TInt TestResetNetworkSearch(TTestResult& aResult);
        
        /**
        * Test CPsetNetwork::CancelProcess
        * Function type: Library
        */        
        TInt TestCancelProcess(TTestResult& aResult);
        
        /**
        * Test CPsetNetwork::GetAvailableNetworksL
        * Function type: Library
        */        
        TInt TestGetAvailableNetworksL(TTestResult& aResult);
        
        /**
        * Test CPsetNetwork::GetCurrentNetworkModeSelectionL
        * Function type: Library
        */        
        TInt TestGetCurrentNetworkModeSelectionL(TTestResult& aResult);
        
        /**
        * Test CPsetNetwork::SelectNetworkL
        * Function type: Library
        */        
        TInt TestSelectNetworkL(TTestResult& aResult);
        
        /**
        * Test CPsetNetwork::SetNetSAObserver
        * Function type: Library
        */        
        TInt TestSetNetSAObserver(TTestResult& aResult);

        /**
        * Test CPsetNetwork::SetNetworkModeObserver
        * Function type: Library
        */        
        TInt TestSetNetworkModeObserver(TTestResult& aResult);

        /**
        * Test CPsetNetwork::SetNetworkModeSelectionL
        * Function type: Library
        */        
        TInt TestSetNetworkModeSelectionL(TTestResult& aResult);
        
    private: // Data
        
        // Reference to test data parsing interface object
        CPhoneSettingsTestParser& iTestParser; 

        // Reference to logging message interface object        
        MPhoneSettingsTestMessageHandler& iTestMessageHandler; 
        
        // Pointer to Phone Settings Network Info Observer object 
        CPhoneSettingsTestNetworkInfoObserver* iPhoneSettingsTestNetworkInfoObserver;

        // Pointer to Phone Settings Network Mode Observer object 
        CPhoneSettingsTestNetworkModeObserver* iPhoneSettingsTestNetworkModeObserver;
        
        // Phone Settings Call Container 
        CPsetContainer* iPsetContainer;        
        
        // Pointer to Phone Settings Network object 
        CPsetNetwork* iPsetNetwork; 
        
        // RMobilePhone object 
        RMobilePhone iPhone; 
    	
    	// RTelServer object     	
    	RTelServer iTelServer; 
    	
    	// RMobileLine object 
    	RMobileLine iLine; 	  
    	
        // Instance of scheduler wait class        
        CActiveSchedulerWait iWaitTimer;        	       
    };

#endif      // PHONESETTINGSTESTNETWORK_H

// End of File