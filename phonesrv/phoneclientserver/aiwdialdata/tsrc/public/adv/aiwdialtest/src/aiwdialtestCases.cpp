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
* Description:  Implementation of Caiwdialtest class.
*
*/



// INCLUDE FILES
#include <e32math.h>
#include <aiwdialdata.h>
#include <AiwCommon.h>
#include <AiwServiceHandler.h>

#include "aiwdialtest.h"



// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// Caiwdialtest::Case
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
const TCaseInfo Caiwdialtest::Case ( 
    const TInt aCaseNumber ) const 
     {

    /**
    * To add new test cases, implement new test case function and add new 
    * line to KCases array specify the name of the case and the function 
    * doing the test case
    * In practice, do following
    * 1) Make copy of existing test case function and change its name
    *    and functionality. Note that the function must be added to 
    *    aiwdialtest.cpp file and to aiwdialtest.h 
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
        // Caiwdialtest::PrintTest. Otherwise the compiler
        // gives errors.
        
        FUNCENTRY( Caiwdialtest::DialTestL ),
        // Example how to use OOM functionality
        //OOM_ENTRY( "Loop test with OOM", Caiwdialtest::LoopTest, ETrue, 2, 3),
        //OOM_FUNCENTRY( Caiwdialtest::PrintTest, ETrue, 1, 3 ),
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
//  Test dialing a call.
// -----------------------------------------------------------------------------
//
TInt Caiwdialtest::DialTestL( TTestResult& /*aResult*/ ) 
    {
    CAiwDialData* dialData = CAiwDialData::NewLC();
    dialData->SetPhoneNumberL( *iPhoneNumber );
    
    CAiwGenericParamList* list = CAiwGenericParamList::NewLC();
    dialData->FillInParamListL( *list );
    
    CActiveScheduler* scheduler = new( ELeave )CActiveScheduler();
    CleanupStack::PushL( scheduler );
    CActiveScheduler::Install( scheduler );
   
    
    RCriteriaArray criterias;
    _LIT8( KContentType, "*" );
    CAiwCriteriaItem* criteria = CAiwCriteriaItem::NewLC( KAiwCmdCall, KAiwCmdCall, KContentType );
    TUid uid;
	uid.iUid = KAiwClassBase;
	criteria->SetServiceClass( uid );
    criterias.AppendL( criteria );
    CleanupClose<RCriteriaArray>::PushL( criterias );
    
    CAiwServiceHandler* service = CAiwServiceHandler::NewLC();
    service->AttachL( criterias );
    service->ExecuteServiceCmdL( KAiwCmdCall, *list, service->OutParamListL() );
    
    CleanupStack::PopAndDestroy( 6, dialData );
    return KErrNone;
    }

//  End of File
