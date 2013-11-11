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
* Description: Declaration of CPhoneSettingsTestSettingsContainer class.
*
*/



#ifndef PHONESETTINGSTESTSETTINGSCONTAINER_H
#define PHONESETTINGSTESTSETTINGSCONTAINER_H

#include <e32base.h>
#include "PhoneSettingsTestParser.h"

// Forward declarations 
class CPsetContainer; 

// CLASS DECLARATION

NONSHARABLE_CLASS(CPhoneSettingsTestSettingsContainer) : public CBase 
    
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CPhoneSettingsTestSettingsContainer* NewL(
            CPhoneSettingsTestParser& aPhoneSettingsTestParser,
            MPhoneSettingsTestMessageHandler& aPhoneSettingsTestMessageHandler);

        /**
        * Destructor.
        */
        virtual ~CPhoneSettingsTestSettingsContainer();

    private:

        /**
        * C++ default constructor.
        */
        CPhoneSettingsTestSettingsContainer(
                    CPhoneSettingsTestParser& aPhoneSettingsTestParser,
                    MPhoneSettingsTestMessageHandler& aPhoneSettingsTestMessageHandler);

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    public: // Test functions 
    
        /**
        * Test CPsetContainer construction and destruction 
        * Tested function(s): NewL, ~CPsetContainer
        * Function type: Library
        */        
        TInt TestCPsetContainerConstructAndDestructL(TTestResult& aResult);

        /**
        * Test CPsetContainer::CreateCBObjectL
        * Function type: Library
        */        
        TInt TestCreateCBObjectL(TTestResult& aResult);
        
        /**
        * Test CPsetContainer::CreateCFObjectL
        * Function type: Library
        */        
        TInt TestCreateCFObjectL(TTestResult& aResult);
        
        /**
        * Test CPsetContainer::CreateCWObjectL
        * Function type: Library
        */        
        TInt TestCreateCWObjectL(TTestResult& aResult);
        
        /**
        * Test CPsetContainer::CreateCliObjectL
        * Function type: Library
        */        
        TInt TestCreateCliObjectL(TTestResult& aResult);

        /**
        * Test CPsetContainer::CreateNetworkModeObjectL
        * Function type: Library
        */        
        TInt TestCreateNetworkModeObjectL(TTestResult& aResult);

        /**
        * Test CPsetContainer::CreateNetworkObjectL
        * Function type: Library
        */        
        TInt TestCreateNetworkObjectL(TTestResult& aResult);

        /**
        * Test CPsetContainer::CreateRefreshHandlerL
        * Function type: Library
        */        
        TInt TestCreateRefreshHandlerL(TTestResult& aResult);

    private: // Data 
    
        // Reference to test data parsing object
        CPhoneSettingsTestParser& iTestParser; 

        // Reference to logging message object        
        MPhoneSettingsTestMessageHandler& iTestMessageHandler; 
                
        // Pointer to CPsetContainer object 
        CPsetContainer* iPsetContainer; 
    };

#endif      // PHONESETTINGSTESTSETTINGSCONTAINER_H

// End of File