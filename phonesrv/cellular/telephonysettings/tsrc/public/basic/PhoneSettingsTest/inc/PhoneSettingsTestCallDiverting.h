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
* Description: Declaration of CPhoneSettingsTestCallDiverting class.
*
*/



#ifndef PHONESETTINGSTESTCALLDIVERTING_H
#define PHONESETTINGSTESTCALLDIVERTING_H

#include <e32base.h>
#include "PhoneSettingsTestParser.h"

// Forward declarations 
class CPhoneSettingsTestCallDivertingObserver;
class CPsetCallDiverting; 

// CLASS DECLARATION

NONSHARABLE_CLASS(CPhoneSettingsTestCallDiverting) : 
    public CBase 
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CPhoneSettingsTestCallDiverting* NewL(
            CPhoneSettingsTestParser& aPhoneSettingsTestParser,
            MPhoneSettingsTestMessageHandler& aPhoneSettingsTestMessageHandler);

        /**
        * Destructor.
        */
        virtual ~CPhoneSettingsTestCallDiverting();

    private:

        /**
        * C++ default constructor.
        */
        CPhoneSettingsTestCallDiverting(
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
        static TInt DoCallBackL( TAny* aAny );
         
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
        * Test CPsetCallDiverting construction and destruction 
        * Function type: Library
        */        
        TInt TestCPsetCallDivertingConstructAndDestruct(TTestResult& aResult);

        /**
        * Test CPsetCallDiverting::GetUsedDataNumberLC
        * Function type: Library
        */        
        TInt TestGetUsedDataNumberLC(TTestResult& aResult);

        /**
        * Test CPsetCallDiverting::GetUsedFaxNumberLC
        * Function type: Library 
        */        
        TInt TestGetUsedFaxNumberLC(TTestResult& aResult);        
    
        /**
        * Test CPsetCallDiverting::SetUsedDataNumberL
        * Function type: Library 
        */   	 
	    TInt TestSetUsedDataNumberL(TTestResult& aResult);        
	    
        /**
        * Test CPsetCallDiverting::SetUsedFaxNumberL
        * Function type: Library 
        */   	 
	    TInt TestSetUsedFaxNumberL(TTestResult& aResult);        

        /**
        * Test CPsetCallDiverting::SetTimerValueL
        * Function type: Library 
        */   	 
	    TInt TestSetTimerValueL(TTestResult& aResult);        

        /**
        * Test CPsetCallDiverting::GetTimerValueL
        * Function type: Library 
        */   	 
	    TInt TestGetTimerValueL(TTestResult& aResult);        

        /**
        * Test CPsetCallDiverting::SetNewDefaultNumberL
        * Function type: Library 
        */   	 
	    TInt TestSetNewDefaultNumberL(TTestResult& aResult);        

        /**
        * Test CPsetCallDiverting::GetDefaultNumbersL
        * Function type: Library 
        */   	 
	    TInt TestGetDefaultNumbersL(TTestResult& aResult);    
	    	        
        /**
        * Test CPsetCallDiverting::CancelCurrentRequest
        * Function type: Library 
        */   	 
	    TInt TestCancelCurrentRequest(TTestResult& aResult);    

        /**
        * Test CPsetCallDiverting::SetDivertingL
        * Function type: Library 
        */   	 
	    TInt TestSetDivertingL(TTestResult& aResult);        
	    
        /**
        * Test CPsetCallDiverting::GetDivertingStatusL
        * Function type: Library 
        */   	 
	    TInt TestGetDivertingStatusL(TTestResult& aResult);        

        /**
        * Test CPsetCallDiverting::SetRequestObserver
        */        
	    TInt TestSetRequestObserver(TTestResult& aResult);

        /**
        * Test CPsetCallDiverting::SwapDefaultNumberL
        */        
	    TInt TestSwapDefaultNumberL(TTestResult& aResult);

        /**
        * Test CPsetCallDiverting::VoiceMailQueryL
        */        
	    TInt TestVoiceMailQueryL(TTestResult& aResult);

        /**
        * Test CPsetCallDiverting::VideoMailQueryL
        */        
	    TInt TestVideoMailQueryL(TTestResult& aResult);

        /**
        * Test TCallDivertSetting::Initialize
        * (from NWDefs.h) 
        */        
	    TInt TestTCallDivertSettingInitialize(TTestResult& aResult);

    private: // Data
        
        // Reference to test data parsing interface object
        CPhoneSettingsTestParser& iTestParser; 

        // Reference to logging message interface object        
        MPhoneSettingsTestMessageHandler& iTestMessageHandler; 
        
        // Pointer to Phone Settings Call Diverting Observer object 
        CPhoneSettingsTestCallDivertingObserver* iPhoneSettingsTestCallDivertingObserver;
        
        // Pointer to Phone Settings Call Diverting object 
        CPsetCallDiverting* iPsetCallDiverting; 
        
        // Pointer to Phone Settings Container object 
        CPsetContainer* iPsetContainer;
        
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

#endif      // PHONESETTINGSTESTCALLDIVERTING_H

// End of File