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
* Description:  Implementation of CUssdTests class.
*
*/



// INCLUDE FILES
#include <e32math.h>
#include <cphcltussd.h>

#include "UssdTests.h"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CUssdTests::Case
// Returns a test case by number.
//
// This function contains an array of all available test cases 
// i.e pair of case name and test function. If case specified by parameter
// aCaseNumber is found from array, then that item is returned.
// 
// The reason for this rather complicated function is to specify all the
// test cases only in one place. It is not necessary to understand how
// function pointers to class member functions works when adding new test
// cases. See function body for instructions how to add new test case.
// -----------------------------------------------------------------------------
//
const TCaseInfo CUssdTests::Case ( 
    const TInt aCaseNumber ) const 
     {

    /**
    * To add new test cases, implement new test case function and add new 
    * line to KCases array specify the name of the case and the function 
    * doing the test case
    * In practice, do following
    * 1) Make copy of existing test case function and change its name
    *    and functionality. Note that the function must be added to 
    *    UssdTests.cpp file and to UssdTests.h 
    *    header file.
    *
    * 2) Add entry to following KCases array either by using:
    *
    * 2.1: FUNCENTRY or ENTRY macro
    * ENTRY macro takes two parameters: test case name and test case 
    * function name.
    *
    * FUNCENTRY macro takes only test case function name as a parameter and
    * uses that as a test case name and test case function name.
    *
    * Or
    *
    * 2.2: OOM_FUNCENTRY or OOM_ENTRY macro. Note that these macros are used
    * only with OOM (Out-Of-Memory) testing!
    *
    * OOM_ENTRY macro takes five parameters: test case name, test case 
    * function name, TBool which specifies is method supposed to be run using
    * OOM conditions, TInt value for first heap memory allocation failure and 
    * TInt value for last heap memory allocation failure.
    * 
    * OOM_FUNCENTRY macro takes test case function name as a parameter and uses
    * that as a test case name, TBool which specifies is method supposed to be
    * run using OOM conditions, TInt value for first heap memory allocation 
    * failure and TInt value for last heap memory allocation failure. 
    */ 

    static TCaseInfoInternal const KCases[] =
        {
        // To add new test cases, add new items to this array
        
        // NOTE: When compiled to GCCE, there must be Classname::
        // declaration in front of the method name, e.g. 
        // CUssdTests::PrintTest. Otherwise the compiler
        // gives errors.
        
        FUNCENTRY( CUssdTests::ConstructionTestL ),
        FUNCENTRY( CUssdTests::SendUssdUnicodeL ),
        FUNCENTRY( CUssdTests::SendUssdL ),
        FUNCENTRY( CUssdTests::SendUssdWithDCSL ),
        FUNCENTRY( CUssdTests::TestCanclingSendingOfUssdL ),
        FUNCENTRY( CUssdTests::TestLaunchingEditorL ),
        
        // Example how to use OOM functionality
        //OOM_ENTRY( "Loop test with OOM", CUssdTests::LoopTest, ETrue, 2, 3),
        //OOM_FUNCENTRY( CUssdTests::PrintTest, ETrue, 1, 3 ),
        };

    // Verify that case number is valid
    if( (TUint) aCaseNumber >= sizeof( KCases ) / 
                               sizeof( TCaseInfoInternal ) )
        {
        // Invalid case, construct empty object
        TCaseInfo null( (const TText*) L"" );
        null.iMethod = NULL;
        null.iIsOOMTest = EFalse;
        null.iFirstMemoryAllocation = 0;
        null.iLastMemoryAllocation = 0;
        return null;
        } 

    // Construct TCaseInfo object and return it
    TCaseInfo tmp ( KCases[ aCaseNumber ].iCaseName );
    tmp.iMethod = KCases[ aCaseNumber ].iMethod;
    tmp.iIsOOMTest = KCases[ aCaseNumber ].iIsOOMTest;
    tmp.iFirstMemoryAllocation = KCases[ aCaseNumber ].iFirstMemoryAllocation;
    tmp.iLastMemoryAllocation = KCases[ aCaseNumber ].iLastMemoryAllocation;
    return tmp;

    }

// -----------------------------------------------------------------------------
// Tests construction
// -----------------------------------------------------------------------------
//
TInt CUssdTests::ConstructionTestL( 
    TTestResult& /*aResult*/ )
    {
    CActiveScheduler* scheduler = new( ELeave )CActiveScheduler();
    CActiveScheduler::Install( scheduler );
    CleanupStack::PushL( scheduler );
    CPhCltUssd* ussd = CPhCltUssd::NewL( EFalse );
    delete ussd;
    CleanupStack::PopAndDestroy( scheduler );
    return KErrNone;
    }
    
// -----------------------------------------------------------------------------
// Tests sending ussd.
// No need to start the scheduler, because CPhCltUssd uses CActiveSchedulerWait
// to synchronise the request.
// -----------------------------------------------------------------------------
//
TInt CUssdTests::SendUssdUnicodeL( 
    TTestResult& /*aResult*/ )
    {
    CActiveScheduler* scheduler = new( ELeave )CActiveScheduler();
    CActiveScheduler::Install( scheduler );
    CleanupStack::PushL( scheduler );
    CPhCltUssd* ussd = CPhCltUssd::NewL( EFalse );
    CleanupStack::PushL( ussd );
    
    _LIT( KUnicodeUSSD, "Ussd msg");
    T1L( ussd->SendUssd( KUnicodeUSSD ), KErrNone );
    
    CleanupStack::PopAndDestroy( 2, scheduler );
    return KErrNone;
    }
    
// -----------------------------------------------------------------------------
// Tests sending ussd.
// No need to start the scheduler, because CPhCltUssd uses CActiveSchedulerWait
// to synchronise the request.
// -----------------------------------------------------------------------------
//
TInt CUssdTests::SendUssdL( 
    TTestResult& /*aResult*/ )
    {
    CActiveScheduler* scheduler = new( ELeave )CActiveScheduler();
    CActiveScheduler::Install( scheduler );
    CleanupStack::PushL( scheduler );
    CPhCltUssd* ussd = CPhCltUssd::NewL( EFalse );
    CleanupStack::PushL( ussd );
    
    ussd->SetDCS( KPhCltDcsUnknown );
    _LIT8( KMsg, "*#123#" 
    );
    T1L( ussd->SendUssd( KMsg ), KErrNone );
    
    CleanupStack::PopAndDestroy( 2, scheduler );
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// Tests sending ussd.
// No need to start the scheduler, because CPhCltUssd uses CActiveSchedulerWait
// to synchronise the request.
// -----------------------------------------------------------------------------
//    
TInt CUssdTests::SendUssdWithDCSL( 
    TTestResult& /*aResult*/ )
    {
    CActiveScheduler* scheduler = new( ELeave )CActiveScheduler();
    CActiveScheduler::Install( scheduler );
    CleanupStack::PushL( scheduler );
    CPhCltUssd* ussd = CPhCltUssd::NewL( EFalse );
    CleanupStack::PushL( ussd );
    
    _LIT8( KMsg, "*#123#" );
    T1L( ussd->SendUssd( KMsg, KPhCltDcsUnknown ), KErrNone );
    
    CleanupStack::PopAndDestroy( 2, scheduler );
    return KErrNone;
    }
    
// -----------------------------------------------------------------------------
// Tests canceling of sending ussd
// No need to start the scheduler, because CPhCltUssd uses CActiveSchedulerWait
// to synchronise the request.
// -----------------------------------------------------------------------------
//    
TInt CUssdTests::TestCanclingSendingOfUssdL( 
    TTestResult& /*aResult*/ )
    {
    CActiveScheduler* scheduler = new( ELeave )CActiveScheduler();
    CActiveScheduler::Install( scheduler );
    CleanupStack::PushL( scheduler );
    CPhCltUssd* ussd = CPhCltUssd::NewL( EFalse );
    CleanupStack::PushL( ussd );
    
    _LIT8( KMsg, "Ussd msg");
    T1L( ussd->SendUssd( KMsg ), KErrNone );
    ussd->SendUssdCancel();
    
    CleanupStack::PopAndDestroy( 2, scheduler );
    return KErrNone;
    }
    
// -----------------------------------------------------------------------------
// Tests canceling of sending ussd
// No need to start the scheduler, because CPhCltUssd uses CActiveSchedulerWait
// to synchronise the request.
// -----------------------------------------------------------------------------
//    
TInt CUssdTests::TestLaunchingEditorL( 
    TTestResult& /*aResult*/ )
    {
    CActiveScheduler* scheduler = new( ELeave )CActiveScheduler();
    CActiveScheduler::Install( scheduler );
    CleanupStack::PushL( scheduler );
    CPhCltUssd* ussd = CPhCltUssd::NewL( EFalse );
    CleanupStack::PushL( ussd );
    
    T1L( ussd->StartUssdEditor(), KErrNone );
    
    CleanupStack::PopAndDestroy( 2, scheduler );
    return KErrNone;
    }


//  End of File

