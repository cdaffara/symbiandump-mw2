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
* Description:
*
*/

#ifndef __UT_CALLINFOS_H__
#define __UT_CALLINFOS_H__

// INCLUDES
#include <CEUnitTestSuiteClass.h>
#include <EUnitDecorators.h>



// FORWARD DECLARATIONS
class CCallInfos;

// CLASS DEFINITION
/**
 * Generated EUnit test suite class.
 */
NONSHARABLE_CLASS( ut_callinfos )
	: public CEUnitTestSuiteClass
    {
    public:  // Constructors and destructor

        static ut_callinfos* NewL();
        static ut_callinfos* NewLC();
        ~ut_callinfos();

    private: // Constructors

        ut_callinfos();
        void ConstructL();

    private: // New methods
        
         void AddInfoL( TInt aCallIndex );

         void SetupL();
        
         void Teardown();
        
         void TestEmptyInfoL();
         void TestInfoHasContentL();
        

    private: // Data

        CCallInfos* iInfos;

        EUNIT_DECLARE_TEST_TABLE;

    };

#endif      //  __UT_CALLINFOS_H__
