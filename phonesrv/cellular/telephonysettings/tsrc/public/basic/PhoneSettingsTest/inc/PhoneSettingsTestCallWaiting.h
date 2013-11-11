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
* Description: Declaration of CPhoneSettingsTestCallWaiting class.
*
*/



#ifndef PHONESETTINGSTESTCALLWAITING_H
#define PHONESETTINGSTESTCALLWAITING_H

#include <e32base.h>
#include "PhoneSettingsTestParser.h"

// Forward declarations 
class CPsetCallWaiting; 
class CPhoneSettingsTestCallWaitingObserver; 

// CLASS DECLARATION

NONSHARABLE_CLASS(CPhoneSettingsTestCallWaiting) : public CBase 
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CPhoneSettingsTestCallWaiting* NewL(
            CPhoneSettingsTestParser& aPhoneSettingsTestParser,
            MPhoneSettingsTestMessageHandler& aPhoneSettingsTestMessageHandler);

        /**
        * Destructor.
        */
        virtual ~CPhoneSettingsTestCallWaiting();

    private:

        /**
        * C++ default constructor.
        */
        CPhoneSettingsTestCallWaiting(
                    CPhoneSettingsTestParser& aPhoneSettingsTestParser,
                    MPhoneSettingsTestMessageHandler& aPhoneSettingsTestMessageHandler);

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();
        
    private: // Functions for handling CActiveSchedulerWait 
    
        /**
        * CActiveSchedulerWait callback function  
        */        
        static TInt DoCallBackL(TAny* aAny);
         
        /**
        * CActiveSchedulerWait wait function 
        */        
        void WaitTimer(TInt aWaitTime);
        
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
        * Test CPsetCallWaiting construction and destruction 
        * Function type: Library
        */        
        TInt TestCPsetCallWaitingConstructAndDestructL(TTestResult& aResult);

        /**
        * Test CPsetCallWaiting::SetCallWaitingL
        * Function type: Library
        */        
        TInt TestSetCallWaitingL(TTestResult& aResult);

        /**
        * Test CPsetCallWaiting::GetCallWaitingStatusL
        * Function type: Library
        */        
        TInt TestGetCallWaitingStatusL(TTestResult& aResult);

        /**
        * Test CPsetCallWaiting::CancelProcess
        * Function type: Library
        */        
        TInt TestCancelProcess(TTestResult& aResult);

        /**
        * Test CPsetCallWaiting::SetRequestObserver
        * Function type: Library
        */        
        TInt TestSetRequestObserver(TTestResult& aResult);

        /**
        * Test CPsetCallWaiting::PhoneSettingChanged
        * Function type: Library
        */        
        TInt TestPhoneSettingChanged(TTestResult& aResult);

    private: // Data
        
        // Reference to test data parsing interface object
        CPhoneSettingsTestParser& iTestParser; 

        // Reference to logging message interface object        
        MPhoneSettingsTestMessageHandler& iTestMessageHandler; 
        
        // Pointer to Phone Settings Call Waiting object 
        CPsetCallWaiting* iPsetCallWaiting; 

        // Pointer to Phone Settings Container object 
        CPsetContainer* iPsetContainer;
        
        // Pointer to Phone Settings Call Waiting Observer object 
        CPhoneSettingsTestCallWaitingObserver* iPhoneSettingsTestCallWaitingObserver; 

        // Pointer to Phone Settings Request Observer object 
        CPhoneSettingsTestPsetRequestObserver* iPhoneSettingsTestPsetRequestObserver;
        
        // RMobilePhone object 
        RMobilePhone iPhone; 
    	
    	// RTelServer object     	
    	RTelServer iTelServer; 
    	
    	// RMobileLine object 
    	RMobileLine iLine; 	  

        // Instance of scheduler wait class        
        CActiveSchedulerWait iWaitTimer;        	      
    };

#endif      // PHONESETTINGSTESTCALLWAITING_H

// End of File