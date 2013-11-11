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
* Description:  Tests for domain AIWDialData API.
*
*/



// INCLUDE FILES
#include "AiwDialDataTests.h"

#include <e32math.h>
#include <aiwdialdata.h>
#include <aiwdialdataext.h>
#include <aiwgenericparam.h>
// ============================= LOCAL FUNCTIONS ===============================

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CAiwDialDataTests::Case
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
const TCaseInfo CAiwDialDataTests::Case ( 
    const TInt aCaseNumber ) const 
     {

    /**
    * To add new test cases, implement new test case function and add new 
    * line to KCases array specify the name of the case and the function 
    * doing the test case
    * In practice, do following
    * 1) Make copy of existing test case function and change its name
    *    and functionality. Note that the function must be added to 
    *    AiwDialDataTests.cpp file and to AiwDialDataTests.h 
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
        // CAiwDialDataTests::PrintTest. Otherwise the compiler
        // gives errors.
        
        FUNCENTRY( CAiwDialDataTests::ConstructionTestL ),
        FUNCENTRY( CAiwDialDataTests::SetAndGetTestL ),
        FUNCENTRY( CAiwDialDataTests::ExtConstructionTestL ),
        FUNCENTRY( CAiwDialDataTests::ExtSetAndGetTestL ),
        // Example how to use OOM functionality
        //OOM_ENTRY( "Loop test with OOM", CAiwDialDataTests::LoopTest, ETrue, 2, 3),
        //OOM_FUNCENTRY( CAiwDialDataTests::PrintTest, ETrue, 1, 3 ),
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
// Tests CAiwDialData construction
// -----------------------------------------------------------------------------
//
TInt CAiwDialDataTests::ConstructionTestL( TTestResult& /*aResult*/ )
    {
    CAiwDialData* dialData = CAiwDialData::NewL();
    delete dialData;
    
    dialData = CAiwDialData::NewLC();
    
    TL(  dialData->PhoneNumber() == KNullDesC() );
    T1L( dialData->CallType(), CAiwDialData::EAIWVoice );
    T1L( dialData->WindowGroup(), 0 );
    
    CAiwGenericParamList* list = CAiwGenericParamList::NewLC();
    dialData->FillInParamListL( *list );
    
    CleanupStack::PopAndDestroy( 2, dialData );

    return KErrNone;
    }
    
// -----------------------------------------------------------------------------
// Tests CAiwDialData setting and getting values.
// -----------------------------------------------------------------------------
//
TInt CAiwDialDataTests::SetAndGetTestL( TTestResult& /*aResult*/ )
    {
    _LIT( KPhoneNumber, "1234567890");
    
    CAiwDialData* dialData = CAiwDialData::NewLC();
    
    dialData->SetPhoneNumberL( KPhoneNumber );
    dialData->SetCallType( CAiwDialData::EAIWVideo );
    dialData->SetWindowGroup( 1 );
    
    TL(  dialData->PhoneNumber() == KPhoneNumber() );
    T1L( dialData->CallType(), CAiwDialData::EAIWVideo );
    T1L( dialData->WindowGroup(), 1 );
    
    CAiwGenericParamList* list = CAiwGenericParamList::NewLC();
    dialData->FillInParamListL( *list );
    
    CleanupStack::PopAndDestroy( 2, dialData );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// Tests CAiwDialDataExt construction
// -----------------------------------------------------------------------------
//    
TInt CAiwDialDataTests::ExtConstructionTestL( TTestResult& /*aResult*/ )
    {
    CAiwDialDataExt* dialData = CAiwDialDataExt::NewL();
    delete dialData;
    
    dialData = CAiwDialDataExt::NewLC();
    
    TL(  dialData->PhoneNumber() == KNullDesC() );
    T1L( dialData->CallType(), CAiwDialData::EAIWVoice );
    T1L( dialData->WindowGroup(), 0 );
    TL( !dialData->InitiateCall() );
    TL( dialData->Name() == KNullDesC() );
    T1L( dialData->Redial(), AIWDialDataExt::KAIWRedialDefault );
    TL( dialData->ShowNumber() );        
    TL( dialData->ContactLink() == KNullDesC8() );
    
    CAiwGenericParamList* list = CAiwGenericParamList::NewLC();
    dialData->FillInParamListL( *list );
    
    CleanupStack::PopAndDestroy( 2, dialData );

    return KErrNone;

    }
    
// -----------------------------------------------------------------------------
// Tests CAiwDialDataExt setting and getting values.
// -----------------------------------------------------------------------------
//
TInt CAiwDialDataTests::ExtSetAndGetTestL( TTestResult& /*aResult*/ )
    {
    _LIT( KPhoneNumber, "1234567890");
    _LIT8( KContactLink, "ContactLink");
    _LIT( KName, "Mr Smith" );
    
    CAiwDialDataExt* dialData = CAiwDialDataExt::NewLC();
    
    dialData->SetPhoneNumberL( KPhoneNumber );
    dialData->SetCallType( CAiwDialData::EAIWVideo );
    dialData->SetWindowGroup( 1 );
    dialData->SetInitiateCall( ETrue );
    dialData->SetNameL( KName );
    
    dialData->SetRedial( AIWDialDataExt::KAIWRedialOff );
    dialData->SetShowNumber( EFalse );
    dialData->SetContactLinkL( KContactLink );
    
    TL(  dialData->PhoneNumber() == KPhoneNumber() );
    T1L( dialData->CallType(), CAiwDialData::EAIWVideo );
    T1L( dialData->WindowGroup(), 1 );
    TL( dialData->InitiateCall() );
    TL( dialData->Name() == KName() );
    T1L( dialData->Redial(), AIWDialDataExt::KAIWRedialOff );
    TL( !dialData->ShowNumber() );        
    TL( dialData->ContactLink() == KContactLink() );
    
    
    CAiwGenericParamList* list = CAiwGenericParamList::NewLC();
    dialData->FillInParamListL( *list );
    
    CleanupStack::PopAndDestroy( 2, dialData );

    return KErrNone;
    }


//  End of File
