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
* Description:  Declaration of CPhoneSettingsTestCallBarring class.
*
*/



#ifndef PHONESETTINGSTESTCALLBARRING_H
#define PHONESETTINGSTESTCALLBARRING_H

#include <e32base.h>
#include "PhoneSettingsTestParser.h"

// Forward declarations 
class CPsetContainer; 
class CPsetCallBarring;
class CPhoneSettingsTestCallBarringObserver; 
class CPhoneSettingsTestPsetRequestObserver;

// CLASS DECLARATION

NONSHARABLE_CLASS(CPhoneSettingsTestCallBarring) : public CBase     
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CPhoneSettingsTestCallBarring* NewL(
            CPhoneSettingsTestParser& aPhoneSettingsTestParser,
            MPhoneSettingsTestMessageHandler& aPhoneSettingsTestMessageHandler);

        /**
        * Destructor.
        */
        virtual ~CPhoneSettingsTestCallBarring();

    private:

        /**
        * C++ default constructor.
        */
        CPhoneSettingsTestCallBarring(
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
        * Test CPsetCallBarring construction and destruction 
        */        
        TInt TestCPsetCallBarringConstructAndDestructL(TTestResult& aResult);
    
        /**
        * Test CPsetCallBarring::ChangePasswordL
        */        
        TInt TestChangePasswordL(TTestResult& aResult);
		
        /**
        * Test CPsetCallBarring::TestSetBarringL 
        */        
	    TInt TestSetBarringL(TTestResult& aResult);
	
        /**
        * Test CPsetCallBarring::GetBarringStatusL
        */        
	    TInt TestGetBarringStatusL(TTestResult& aResult);

        /**
        * Test CPsetCallBarring::SetRequestObserver
        */        
	    TInt TestSetRequestObserver(TTestResult& aResult);

        /**
        * Test CPsetCallBarring::CancelCurrentRequest
        */        
	    TInt TestCancelCurrentRequest(TTestResult& aResult);

        /**
        * Test CPsetCallBarring::NewL
        */        
	    TInt TestCPsetCallBarringNewL(TTestResult& aResult);

        /**
        * Test CPsetCallBarring::PhoneSettingChanged
        */        
	    TInt TestPhoneSettingChanged(TTestResult& aResult);

    private: // Data
        
        // Reference to test data parsing object
        CPhoneSettingsTestParser& iTestParser; 

        // Reference to logging message object        
        MPhoneSettingsTestMessageHandler& iTestMessageHandler; 

        // Pointer to barring observer object
        CPhoneSettingsTestCallBarringObserver* iPhoneSettingsTestCallBarringObserver; 

        // Pointer to request observer object
	    CPhoneSettingsTestPsetRequestObserver* iPhoneSettingsTestPsetRequestObserver;        
        
        // Phone Settings Call Container 
        CPsetContainer* iPsetContainer;
        
        // Phone Settings Call Barring 
        CPsetCallBarring* iPsetCallBarring;
        
        // Instance of scheduler wait class        
        CActiveSchedulerWait iWaitTimer;         
    };


#endif      // PHONESETTINGSTESTCALLBARRING_H

// End of File