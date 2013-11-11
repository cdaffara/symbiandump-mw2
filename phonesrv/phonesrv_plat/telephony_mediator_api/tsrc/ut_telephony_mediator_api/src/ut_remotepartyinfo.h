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

#ifndef __UT_REMOTEPARTYINFO_H__
#define __UT_REMOTEPARTYINFO_H__

// INCLUDES
#include <CEUnitTestSuiteClass.h>
#include <EUnitDecorators.h>

// FORWARD DECLARATIONS
class CCallRemotePartyInfo;

// CLASS DEFINITION
/**
 * Generated EUnit test suite class.
 */
NONSHARABLE_CLASS( ut_remotepartyinfo )
	: public CEUnitTestSuiteClass
    {
    public:  // Constructors and destructor

        static ut_remotepartyinfo* NewL();
        static ut_remotepartyinfo* NewLC();
        ~ut_remotepartyinfo();

    private: // Constructors

        ut_remotepartyinfo();
        void ConstructL();

    private: // New methods
        
         void SetupL();
        
         void Teardown();
        
         void TestEmptyInfoL();
         void TestInternalizeExternalizeEmptyInfoL();
         void TestInfoL();
         void TestComparisonOperatorL();
         void TestComparisonOperatorNoMatchL();
         void TestInternalizeExternalizeInfoL();
        

    private: // Data

        CCallRemotePartyInfo* iInfo;

        EUNIT_DECLARE_TEST_TABLE;

    };

#endif      //  __UT_REMOTEPARTYINFO_H__
