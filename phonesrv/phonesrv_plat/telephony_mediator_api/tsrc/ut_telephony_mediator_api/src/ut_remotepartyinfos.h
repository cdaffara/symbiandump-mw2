/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: RemotePartyInformation unit tests.
*
*/

#ifndef __UT_REMOTEPARTYINFOS_H__
#define __UT_REMOTEPARTYINFOS_H__

// INCLUDES
#include <CEUnitTestSuiteClass.h>
#include <EUnitDecorators.h>

// FORWARD DECLARATIONS
class CCallRemotePartyInfos;

// CLASS DEFINITION
/**
 * Generated EUnit test suite class.
 */
NONSHARABLE_CLASS( ut_remotepartyinfos )
	: public CEUnitTestSuiteClass
    {
    public:  // Constructors and destructor

        static ut_remotepartyinfos* NewL();
        static ut_remotepartyinfos* NewLC();
        ~ut_remotepartyinfos();

    private: // Constructors

        ut_remotepartyinfos();
        void ConstructL();

    private: // New methods
        
         void AddInfoL( const TDesC& aMatchedName,
                        const TDesC& aNameFromNetwork,
                        const TDesC& aCompanyName,
                        const TDesC& aAddress,
                        TInt aCallIndex );
        
         void SetupL();
        
         void Teardown();
        
         void TestEmptyInfoL();
         void TestInfoHasContentL();
         void TestInternalizingOverExistingDataL(); 
         void TestAddingNullL();
        

    private: // Data

        CCallRemotePartyInfos* iInfos;

        EUNIT_DECLARE_TEST_TABLE;

    };

#endif      //  __UT_REMOTEPARTYINFOS_H__
