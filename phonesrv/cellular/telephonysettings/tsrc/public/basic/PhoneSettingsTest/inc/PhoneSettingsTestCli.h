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
* Description: Declaration of CPhoneSettingsTestCli class.
*
*/



#ifndef PHONESETTINGSTESTCLI_H
#define PHONESETTINGSTESTCLI_H

#include <e32base.h>
#include "PhoneSettingsTestParser.h"

// Forward declarations 
class CPhoneSettingsTestCliObserver;
class CPsetCli; 

// CLASS DECLARATION

NONSHARABLE_CLASS(CPhoneSettingsTestCli) : public CBase 
    
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CPhoneSettingsTestCli* NewL(
            CPhoneSettingsTestParser& aPhoneSettingsTestParser,
            MPhoneSettingsTestMessageHandler& aPhoneSettingsTestMessageHandler );

        /**
        * Destructor.
        */
        virtual ~CPhoneSettingsTestCli();

    private:

        /**
        * C++ default constructor.
        */
        CPhoneSettingsTestCli(
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
        * Test CPsetCli construction and destruction 
        * Function type: Library
        */        
        TInt TestCPsetCliConstructAndDestructL(TTestResult& aResult);

        /**
        * Test CPsetCli::CancelAll
        * Function type: Library
        */        
        TInt TestCancelAll(TTestResult& aResult);

        /**
        * Test CPsetCli::GetClipModeL
        * Function type: Library
        */        
        TInt TestGetClipModeL(TTestResult& aResult);

        /**
        * Test CPsetCli::GetClirModeL
        * Function type: Library
        */        
        TInt TestGetClirModeL(TTestResult& aResult);

        /**
        * Test CPsetCli::GetCnapL
        * Function type: Library
        */        
        TInt TestGetCnapL(TTestResult& aResult);

        /**
        * Test CPsetCli::GetColpModeL
        * Function type: Library
        */        
        TInt TestGetColpModeL(TTestResult& aResult);

        /**
        * Test CPsetCli::GetColrModeL
        * Function type: Library
        */        
        TInt TestGetColrModeL(TTestResult& aResult);

        /**
        * Test CPsetCli::SetRequestObserver
        * Function type: Library
        */        
        TInt TestSetRequestObserver(TTestResult& aResult);
        
    private: // Data
        
        // Reference to test data parsing interface object
        CPhoneSettingsTestParser& iTestParser; 

        // Reference to logging message interface object        
        MPhoneSettingsTestMessageHandler& iTestMessageHandler; 
        
        // Pointer to Phone Settings Cli Observer object 
        CPhoneSettingsTestCliObserver* iPhoneSettingsTestCliObserver;

        // Pointer to Phone Settings Request Observer object 
        CPhoneSettingsTestPsetRequestObserver* iPhoneSettingsTestPsetRequestObserver;	    
	            
        // Pointer to Phone Settings Container object 
        CPsetContainer* iPsetContainer; 
        
        // Pointer to Phone Settings Cli object 
        CPsetCli* iPsetCli; 
        
        // RMobilePhone object 
        RMobilePhone iPhone; 
    	
    	// RTelServer object     	
    	RTelServer iTelServer; 
    	
    	// RMobileLine object 
    	RMobileLine iLine; 	  
    	
    	// Instance of scheduler wait class        
        CActiveSchedulerWait iWaitTimer;               
    };

#endif      // PHONESETTINGSTESTCLI_H

// End of File