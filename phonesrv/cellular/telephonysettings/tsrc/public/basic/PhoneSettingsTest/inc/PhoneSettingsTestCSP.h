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
* Description: Declaration of CPhoneSettingsTestCSP class.
*
*/



#ifndef PHONESETTINGSTESTCSP_H
#define PHONESETTINGSTESTCSP_H

#include <e32base.h>
#include "PhoneSettingsTestParser.h"

// Forward declarations 
class CPsetCustomerServiceProfile; 

// CLASS DECLARATION

NONSHARABLE_CLASS(CPhoneSettingsTestCSP) : public CBase     
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CPhoneSettingsTestCSP* NewL(
            CPhoneSettingsTestParser& aPhoneSettingsTestParser,
            MPhoneSettingsTestMessageHandler& aPhoneSettingsTestMessageHandler);

        /**
        * Destructor.
        */
        virtual ~CPhoneSettingsTestCSP();

    private:

        /**
        * C++ default constructor.
        */
        CPhoneSettingsTestCSP(
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
        * Test CPsetCustomerServiceProfile construction and destruction 
        * Function type: Library
        */        
        TInt TestCPsetCustomerServiceProfileConstructAndDestructL(TTestResult& aResult);

        /**
        * Test CPsetCustomerServiceProfile::IsALSSupported
        * Function type: Library
        */        
        TInt TestIsALSSupported(TTestResult& aResult);

        /**
        * Test CPsetCustomerServiceProfile::IsCBSupported
        * Function type: Library
        */        
        TInt TestIsCBSupported(TTestResult& aResult);

        /**
        * Test CPsetCustomerServiceProfile::IsCFSupported
        * Function type: Library
        */        
        TInt TestIsCFSupported(TTestResult& aResult);

        /**
        * Test CPsetCustomerServiceProfile::IsCWSupported
        * Function type: Library
        */        
        TInt TestIsCWSupported(TTestResult& aResult);

        /**
        * Test CPsetCustomerServiceProfile::IsNetworkSelectionSupported
        * Function type: Library
        */        
        TInt TestIsNetworkSelectionSupported(TTestResult& aResult);

        /**
        * Test CPsetCustomerServiceProfile::OpenCSProfileL
        * Function type: Library
        */        
        TInt TestOpenCSProfileL(TTestResult& aResult);

        /**
        * Test CPsetCustomerServiceProfile::CheckIfCWSupported
        * Function type: Library
        */        
        TInt TestCheckIfCWSupported(TTestResult& aResult);

        /**
        * Test CPsetCustomerServiceProfile::CheckIfCBSupported
        * Function type: Library
        */        
        TInt TestCheckIfCBSupported(TTestResult& aResult);

        /**
        * Test CPsetCustomerServiceProfile::CheckIfCFSupported
        * Function type: Library
        */        
        TInt TestCheckIfCFSupported(TTestResult& aResult);

        /**
        * Test CPsetCustomerServiceProfile::CheckIfAlsSupported
        * Function type: Library
        */        
        TInt TestCheckIfAlsSupported(TTestResult& aResult);

        /**
        * Test CPsetCustomerServiceProfile::CheckIfNetworkSelectionSupported
        * Function type: Library
        */        
        TInt TestCheckIfNetworkSelectionSupported(TTestResult& aResult);

    private: // Data
        
        // Reference to test data parsing interface object
        CPhoneSettingsTestParser& iTestParser; 

        // Reference to logging message interface object        
        MPhoneSettingsTestMessageHandler& iTestMessageHandler; 
        
        // Pointer to Phone Settings CSP object 
        CPsetCustomerServiceProfile* iPsetCustomerServiceProfile; 

        // Instance of scheduler wait class        
        CActiveSchedulerWait iWaitTimer;            
    };

#endif      // PHONESETTINGSTESTCSP_H

// End of File