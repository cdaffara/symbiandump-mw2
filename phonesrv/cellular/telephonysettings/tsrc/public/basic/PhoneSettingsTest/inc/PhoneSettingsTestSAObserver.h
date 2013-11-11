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
* Description: Declaration of CPhoneSettingsTestSAObserver class.
*
*/



#ifndef PHONESETTINGSTESTSAOBSERVER_H
#define PHONESETTINGSTESTSAOBSERVER_H

#include <e32base.h>
#include "PhoneSettingsTestParser.h"

// Forward declarations 
class CPsetSAObserver; 

// CLASS DECLARATION

NONSHARABLE_CLASS(CPhoneSettingsTestSAObserver) : public CBase     
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CPhoneSettingsTestSAObserver* NewL(
            CPhoneSettingsTestParser& aPhoneSettingsTestParser,
            MPhoneSettingsTestMessageHandler& aPhoneSettingsTestMessageHandler);

        /**
        * Destructor.
        */
        virtual ~CPhoneSettingsTestSAObserver();

    private:

        /**
        * C++ default constructor.
        */
        CPhoneSettingsTestSAObserver(
                    CPhoneSettingsTestParser& aPhoneSettingsTestParser,
                    MPhoneSettingsTestMessageHandler& aPhoneSettingsTestMessageHandler);

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    public: // Test functions 
    
        /**
        * Test CPsetSAObserver construction and destruction 
        * Function type: Library
        */        
        TInt TestCPsetSAObserverConstructAndDestructL(TTestResult& aResult);

        /**
        * Test CPsetSAObserver::GetCurrentDivertStatus
        * Function type: Library
        */        
        TInt TestGetCurrentDivertStatus(TTestResult& aResult);

        /**
        * Test CPsetSAObserver::IsCallActive
        * Function type: Library
        */        
        TInt TestIsCallActive(TTestResult& aResult);

        /**
        * Test CPsetSAObserver::IsGPRSConnected
        * Function type: Library
        */        
        TInt TestIsGPRSConnected(TTestResult& aResult);

        /**
        * Test CPsetSAObserver::NotifyDivertChange
        * Function type: Library
        */        
        TInt TestNotifyDivertChange(TTestResult& aResult);

        /**
        * Test CPsetSAObserver::SetNetObserver
        * Function type: Library
        */        
        TInt TestSetNetObserver(TTestResult& aResult);

        /**
        * Test CPsetSAObserver::Initialize
        * Function type: Library
        */        
        TInt TestInitialize(TTestResult& aResult);

    private: // Data
        
        // Reference to test data parsing interface object
        CPhoneSettingsTestParser& iTestParser; 

        // Reference to logging message interface object        
        MPhoneSettingsTestMessageHandler& iTestMessageHandler; 
        
        // Pointer to Phone Settings SAObserver object 
        CPsetSAObserver* iPsetSAObserver; 
    };

#endif      // PHONESETTINGSTESTSAOBSERVER_H

// End of File