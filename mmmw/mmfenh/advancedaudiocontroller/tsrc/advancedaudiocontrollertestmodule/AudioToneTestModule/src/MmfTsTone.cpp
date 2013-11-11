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
* Description:  MmfTsTone class member functions
*
*/



// INCLUDE FILES
#include <Stiftestinterface.h>
#include "MmfTsTone.h"

// EXTERNAL DATA STRUCTURES
// None

// EXTERNAL FUNCTION PROTOTYPES  
// None

// CONSTANTS
const TInt CMmfTsTone::KToneMinCaseId=501;



const TInt CMmfTsTone::KToneMaxCaseId=520;

// MACROS
// None

// LOCAL CONSTANTS AND MACROS
// None

// MODULE DATA STRUCTURES
// None

// LOCAL FUNCTION PROTOTYPES
// None

// FORWARD DECLARATIONS
// None

// ============================= LOCAL FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// ?function_name ?description.
// ?description
// Returns: ?value_1: ?description
//          ?value_n: ?description_line1
//                    ?description_line2
// -----------------------------------------------------------------------------
//
/*
?type ?function_name(
    ?arg_type arg,  // ?description
    ?arg_type arg)  // ?description
    {

    ?code  // ?comment

    // ?comment
    ?code
    }
*/

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMmfTsTone::CMmfTsTone
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMmfTsTone::CMmfTsTone() :CActiveScheduledTestModuleBase(KToneMinCaseId,KToneMaxCaseId, KTestCaseTitleFormat, KDefaultConsoleTraceDescription, KToneTracePrefix) {}


// -----------------------------------------------------------------------------
// CMmfTsTone::ConstructL
// Symbian 2nd phase constructor can leave.
// Note: If OOM test case uses STIF Logger, then STIF Logger must be created
// with static buffer size parameter (aStaticBufferSize). Otherwise Logger 
// allocates memory from heap and therefore causes error situations with OOM 
// testing. For more information about STIF Logger construction, see STIF Users 
// Guide.
// -----------------------------------------------------------------------------
//
void CMmfTsTone::ConstructL()
    {
	CActiveScheduledTestModuleBase::ConstructL();
    }

// -----------------------------------------------------------------------------
// CMmfTsTone::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CMmfTsTone* CMmfTsTone::NewL()
    {
    CMmfTsTone* self = new (ELeave) CMmfTsTone;

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;

    }

// Destructor
CMmfTsTone::~CMmfTsTone()
    { 
    }

// -----------------------------------------------------------------------------
// CMmfTsTone::InitL
// InitL is used to initialize the Test Module.
// -----------------------------------------------------------------------------
//
//TInt CMmfTsTone::InitL( 
  //  TFileName& /*aIniFile*/, 
  //  TBool /*aFirstTime*/ )
  //  {
  //  return KErrNone;

 //   }

// -----------------------------------------------------------------------------
// CMmfTsTone::GetTestCasesL
// GetTestCases is used to inquire test cases from the Test Module. Test
// cases are stored to array of test cases. The Test Framework will be 
// the owner of the data in the RPointerArray after GetTestCases return
// and it does the memory deallocation. 
// -----------------------------------------------------------------------------
//
//TInt CMmfTsTone::GetTestCasesL( 
   // const TFileName& /*aConfig*/, 
  //  RPointerArray<TTestCaseInfo>& aTestCases )
   // {

    /**
    * A sample code for testcase description allocation, filling and adding 
    * to test case array. This is probably done somehow in a loop for 
    * every testcase and information may be fetched from file 
    * given as parameter or it can be hardcoded to the testmodule. 
    * Another example can be found from DemoModule included 
    * also in STIF Test Framework release. 

    // Allocate new TTestCaseInfo from heap.
    // It MUST be allocated from heap.
    TTestCaseInfo* testCase = new ( ELeave ) TTestCaseInfo();

    // Sample how to use logging
    _LIT( KLogStart, "Logging starts!" );
    iLog->Log( KLogStart );

    // PushL TTestCaseInfo to CleanupStack.    
    CleanupStack::PushL( testCase );

    // Set number for the testcase.
    // When the testcase is run, this comes as a parameter to RunTestCaseL.
    testCase->iCaseNumber = 1;

    // Set title for the test case. This is shown in UI to the user.
    _LIT( KTestOne, "Test one" );
    testCase->iTitle.Copy( KTestOne );

    // Append TTestCaseInfo to the testcase array. After appended succesfully 
    // the TTestCaseInfo object is owned (and freed) by the TestServer. 
    User::LeaveIfError( aTestCases.Append( testCase ) );

    // Pop TTestCaseInfo from the CleanupStack.
    CleanupStack::Pop( testCase );

    */

   // return KErrNone;

  //  }

// -----------------------------------------------------------------------------
// CMmfTsTone::RunTestCaseL
// RunTestCaseL is used to run an individual test case specified 
// by aTestCase. Test cases that can be run may be requested from 
// Test Module by GetTestCases method before calling RunTestCase.
// -----------------------------------------------------------------------------
//
//TInt CMmfTsTone::RunTestCaseL(
 //   const TInt /* aCaseNumber */,
  //  const TFileName& /*aConfig*/,
   // TTestResult& aResult )
  //  {
    // Sets test case result and description(Maximum size is KStifMaxResultDes)
  //  _LIT( KDescription, "Test passed" );
   // aResult.SetResult( KErrNone, KDescription );

    // Test case was executed
  //  return KErrNone;
  //  }

// -----------------------------------------------------------------------------
// CMmfTsTone::OOMTestQueryL
// Used to check if a particular test case should be run in OOM conditions and 
// which memory allocations should fail.    
//
// NOTE: This method is virtual and must be implemented only if test case
// should be executed using OOM conditions.  
// -----------------------------------------------------------------------------
//
TBool CMmfTsTone::OOMTestQueryL( 
                                const TFileName& /* aTestCaseFile */, 
                                const TInt /* aCaseNumber */, 
                                TOOMFailureType& /* aFailureType */, 
                                TInt& /* aFirstMemFailure */, 
                                TInt& /* aLastMemFailure */ ) 
    {
    _LIT( KOOMTestQueryL, "CMmfTsTone::OOMTestQueryL" );
    //iLogger->Log( KOOMTestQueryL ); 

    return EFalse;

    }

// -----------------------------------------------------------------------------
// CMmfTsTone::OOMTestInitializeL
// Used to perform the test environment setup for a particular OOM test case. 
// Test Modules may use the initialization file to read parameters for Test 
// Module initialization but they can also have their own configure file or 
// some other routine to initialize themselves.  
//
// NOTE: This method is virtual and must be implemented only if test case
// should be executed using OOM conditions.  
// -----------------------------------------------------------------------------
//
void CMmfTsTone::OOMTestInitializeL( 
                                const TFileName& /* aTestCaseFile */, 
                                const TInt /* aCaseNumber */ )
    {
    }

// -----------------------------------------------------------------------------
// CMmfTsTone::OOMHandleWarningL
// In some cases the heap memory allocation should be skipped, either due to
// problems in the OS code or components used by the code being tested, or even 
// inside the tested components which are implemented this way on purpose (by 
// design), so it is important to give the tester a way to bypass allocation 
// failures.
//
// NOTE: This method is virtual and must be implemented only if test case
// should be executed using OOM conditions.  
// -----------------------------------------------------------------------------
//
void CMmfTsTone::OOMHandleWarningL( 
                                const TFileName& /* aTestCaseFile */,
                                const TInt /* aCaseNumber */, 
                                TInt& /* aFailNextValue */ )
    {
    }

// -----------------------------------------------------------------------------
// CMmfTsTone::OOMTestFinalizeL
// Used to perform the test environment cleanup for a particular OOM test case.
//
// NOTE: This method is virtual and must be implemented only if test case
// should be executed using OOM conditions.  
// -----------------------------------------------------------------------------
//                  
void CMmfTsTone::OOMTestFinalizeL( 
                                const TFileName& /* aTestCaseFile */, 
                                const TInt /* aCaseNumber */ )
    {
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================

// -----------------------------------------------------------------------------
// LibEntryL is a polymorphic Dll entry point
// Returns: CTestModuleBase*: Pointer to Test Module object
// -----------------------------------------------------------------------------
//
EXPORT_C CTestModuleBase* LibEntryL()
    {
    return CMmfTsTone::NewL();

    }

// -----------------------------------------------------------------------------
// SetRequirements handles test module parameters(implements evolution
// version 1 for test module's heap and stack sizes configuring).
// Returns: TInt: Symbian error code.
// -----------------------------------------------------------------------------
//
EXPORT_C TInt SetRequirements( CTestModuleParam*& /*aTestModuleParam*/, 
                                TUint32& /*aParameterValid*/ )
    {

    /* --------------------------------- NOTE ---------------------------------
    USER PANICS occurs in test thread creation when:
    1) "The panic occurs when the value of the stack size is negative."
    2) "The panic occurs if the minimum heap size specified is less
       than KMinHeapSize".
       KMinHeapSize: "Functions that require a new heap to be allocated will
       either panic, or will reset the required heap size to this value if
       a smaller heap size is specified".
    3) "The panic occurs if the minimum heap size specified is greater than
       the maximum size to which the heap can grow".
    Other:
    1) Make sure that your hardware or Symbian OS is supporting given sizes.
       e.g. Hardware might support only sizes that are divisible by four.
    ------------------------------- NOTE end ------------------------------- */

    // Normally STIF uses default heap and stack sizes for test thread, see:
    // KTestThreadMinHeap, KTestThreadMinHeap and KStackSize.
    // If needed heap and stack sizes can be configured here by user. Remove
    // comments and define sizes.

/*
    aParameterValid = KStifTestModuleParameterChanged;

    CTestModuleParamVer01* param = CTestModuleParamVer01::NewL();
    // Stack size
    param->iTestThreadStackSize= 16384; // 16K stack
    // Heap sizes
    param->iTestThreadMinHeap = 4096;   // 4K heap min
    param->iTestThreadMaxHeap = 1048576;// 1M heap max

    aTestModuleParam = param;
*/
    return KErrNone;

    }

// -----------------------------------------------------------------------------
// E32Dll is a DLL entry point function
// Returns: KErrNone: No error
// -----------------------------------------------------------------------------
//
#ifndef EKA2 // Hide Dll entry point to EKA2
GLDEF_C TInt E32Dll(
    TDllReason /*aReason*/) // Reason
    {
    return(KErrNone);

    }
#endif // EKA2

//  End of File
