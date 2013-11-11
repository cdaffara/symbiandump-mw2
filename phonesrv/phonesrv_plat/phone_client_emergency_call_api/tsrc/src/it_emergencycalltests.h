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

#ifndef __IT_EMERGENCYCALLTESTS_H__
#define __IT_EMERGENCYCALLTESTS_H__

// INCLUDES
#include <CEUnitTestSuiteClass.h>
#include <EUnitDecorators.h>

#include <mphcltemergencycallobserver.h>

// FORWARD DECLARATIONS
class CPhCltEmergencyCall;
class CEtel;

// CLASS DEFINITION
/**
 * Generated EUnit test suite class.
 */
NONSHARABLE_CLASS( IT_EmergencyCallTests )
	: public CEUnitTestSuiteClass, private MPhCltEmergencyCallObserver
    {
    public:  // Constructors and destructor

        static IT_EmergencyCallTests* NewL();
        static IT_EmergencyCallTests* NewLC();
        ~IT_EmergencyCallTests();

    private: // Constructors

        IT_EmergencyCallTests();
        void ConstructL();
        
        void HandleEmergencyDialL( const TInt aStatus );

    private: // New methods
        
         void DoTestValidEmergencyNumberL( const TDesC& aNumber );
         void DoTestInvalidEmergencyNumberL( const TDesC& aNumber );
         
         void DoTestFindValidEmergencyNumberL( const TDesC& aNumber, 
             const TDesC& aExpectedFoundEmergencyNumber );
         void DoTestFindInvalidEmergencyNumberL( const TDesC& aNumber );

         void SetupL();
         void SetupWithEtelL();
         void Teardown();     
         
         void TestConstructionL();
         void Test112L();
         void TestHash31Hash112L();
         void TestStar31Hash112L();
         void Test112WithDtmfL();
         void Test112WithDtmf_2L();
         void TestPlusSignBefore112L();
         void TestCLIR112DTMFL();
         
         void TestInvalidEmergencyNumber_1L();
         void TestInvalidEmergencyNumber_2L();
         void TestInvalidEmergencyNumber_3L();
         void TestInvalidEmergencyNumber_4L();
         void TestInvalidEmergencyNumber_5L();
         void TestInvalidEmergencyNumber_6L();
         void TestInvalidEmergencyNumber_7L();
         void TestInvalidEmergencyNumber_8L();
         void TestInvalidEmergencyNumber_9L();
         void TestInvalidEmergencyNumber_10L();
         void TestInvalidEmergencyNumber_11L();
         void TestInvalidEmergencyNumber_12L();
         void TestInvalidEmergencyNumber_13L();
         void TestInvalidEmergencyNumber_14L();
         void TestInvalidEmergencyNumber_15L();
         void TestInvalidEmergencyNumber_16L();
         void TestInvalidEmergencyNumber_17L();
         void TestInvalidEmergencyNumber_18L();
         
         void TestFindingEmergencyNumber_1L();
         void TestFindingEmergencyNumber_2L();
         void TestFindingEmergencyNumber_3L();
         
         void TestCallingEmergencyCallWithInvalidEmergencyNumberL();
         void TestEmergencyCallL();

    private: // Data
        
        CPhCltEmergencyCall* iEmergency;
        CEtel* iEtel;
        
        TInt iReceivedDialStatus;

        EUNIT_DECLARE_TEST_TABLE;

    };

#endif      //  __IT_EMERGENCYCALLTESTS_H__
