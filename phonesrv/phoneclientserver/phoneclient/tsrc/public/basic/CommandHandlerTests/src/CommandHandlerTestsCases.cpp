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
* Description:  Implementation of CCommandHandlerTests.
*
*/



// INCLUDE FILES
#include <e32math.h>
#include "CommandHandlerTests.h"
#include <cphcltcommandhandler.h>
// EXTERNAL DATA STRUCTURES
//extern  ?external_data;

// EXTERNAL FUNCTION PROTOTYPES  
//extern ?external_function( ?arg_type,?arg_type );

// CONSTANTS
//const ?type ?constant_var = ?constant;
_LIT(KPhoneNumber1, "111"); 
// MACROS
//#define ?macro ?macro_def

// LOCAL CONSTANTS AND MACROS
//const ?type ?constant_var = ?constant;
//#define ?macro_name ?macro_def

// MODULE DATA STRUCTURES
//enum ?declaration
//typedef ?declaration

// LOCAL FUNCTION PROTOTYPES
//?type ?function_name( ?arg_type, ?arg_type );

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;

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
// CCommandHandlerTests::Case
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
const TCaseInfo CCommandHandlerTests::Case ( 
    const TInt aCaseNumber ) const 
     {

    /**
    * To add new test cases, implement new test case function and add new 
    * line to KCases array specify the name of the case and the function 
    * doing the test case
    * In practice, do following
    * 1) Make copy of existing test case function and change its name
    *    and functionality. Note that the function must be added to 
    *    CommandHandlerTests.cpp file and to CommandHandlerTests.h 
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
        // CCommandHandlerTests::PrintTest. Otherwise the compiler
        // gives errors.
        
        //FUNCENTRY( CCommandHandlerTests::PrintTest ),
        ENTRY( "CPhCltCommandHandler::NewL", Construction ),
        ENTRY( "CPhCltCommandHandler::Atd", Atd ),
        ENTRY( "CPhCltCommandHandler::Chup", Chup ),
        ENTRY( "CPhCltCommandHandler::Ata", Ata ),
        ENTRY( "CPhCltCommandHandler::Vts", Vts ),
        ENTRY( "CPhCltCommandHandler::MuteRingingTone", MuteRingingTone ),
         
        // Example how to use OOM functionality
        //OOM_ENTRY( "Loop test with OOM", CCommandHandlerTests::LoopTest, ETrue, 2, 3),
        //OOM_FUNCENTRY( CCommandHandlerTests::PrintTest, ETrue, 1, 3 ),
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
// CCommandHandlerTests::PrintTest
// Simple printing to UI test.
// -----------------------------------------------------------------------------
//
TInt CCommandHandlerTests::PrintTest( 
    TTestResult& aResult )
    {
     /* Simple print test */
    _LIT( KPrintTest, "PrintTest" );
    _LIT( KEnter, "Enter" );
    _LIT( KOnGoing, "On-going" );
    _LIT( KExit, "Exit" );

    TestModuleIf().Printf( 0, KPrintTest, KEnter );
           
    TestModuleIf().Printf( 1, KPrintTest, KOnGoing );
    
    TestModuleIf().Printf( 0, KPrintTest, KExit );

    // Test case passed

    // Sets test case result and description(Maximum size is KStifMaxResultDes)
    _LIT( KDescription, "PrintTest passed" );
    aResult.SetResult( KErrNone, KDescription );

    // Case was executed
    return KErrNone;

    }

// -----------------------------------------------------------------------------
// CCommandHandlerTests::LoopTest
// Another printing to UI test.
// -----------------------------------------------------------------------------
//
TInt CCommandHandlerTests::LoopTest( TTestResult& aResult )
    {

    /* Simple print and wait loop */
    _LIT( KState, "State" );
    _LIT( KLooping, "Looping" );

    TestModuleIf().Printf( 0, KState, KLooping );

    _LIT( KRunning, "Running" );
    _LIT( KLoop, "%d" );
    for( TInt i=0; i<10; i++)
        {
        TestModuleIf().Printf( 1, KRunning, KLoop, i);
        User::After( 1000000 );
        }

    _LIT( KFinished, "Finished" );
    TestModuleIf().Printf( 0, KState, KFinished );

    // Test case passed

    // Sets test case result and description(Maximum size is KStifMaxResultDes)
    _LIT( KDescription, "LoopTest passed" );
    aResult.SetResult( KErrNone, KDescription );

    // Case was executed
    return KErrNone;

    }

//-----------------------------------------------------------------------------
// CCommandHandlerTests::Construction
//-----------------------------------------------------------------------------
// 
TInt CCommandHandlerTests::Construction( TTestResult& aResult )
    {
    TPhCltComHandCommandParameters Param;
    Param.iTelNumber = KPhoneNumber1;
    TRequestStatus iStatus;
    
    CActiveScheduler* scheduler = new( ELeave )CActiveScheduler();
    CActiveScheduler::Install( scheduler );
    CleanupStack::PushL( scheduler );
    
    CPhCltCommandHandler* handler = CPhCltCommandHandler::NewL();
    delete handler;
    CleanupStack::PopAndDestroy( scheduler );
    
    _LIT( KDescription, "NewL Done" );
    aResult.SetResult( KErrNone, KDescription );
    
    return KErrNone;
        
    }    

//-----------------------------------------------------------------------------
// CCommandHandlerTests::Atd
//-----------------------------------------------------------------------------
// 
TInt CCommandHandlerTests::Atd( TTestResult& aResult )
    {
    TPhCltComHandCommandParameters Param;
    Param.iTelNumber = KPhoneNumber1;
    TRequestStatus iStatus;
    
    CActiveScheduler* scheduler = new( ELeave )CActiveScheduler();
    CActiveScheduler::Install( scheduler );
    CleanupStack::PushL( scheduler );
    
    CPhCltCommandHandler* handler = CPhCltCommandHandler::NewL();
    
    handler->Atd( iStatus, Param.iTelNumber );        
    User::WaitForRequest( iStatus );
    
    delete handler;
    CleanupStack::PopAndDestroy( scheduler );
    
    _LIT( KDescription, "Atd Done" );
    aResult.SetResult( iStatus.Int(), KDescription );
    
    return iStatus.Int();
        
    }

//-----------------------------------------------------------------------------
// CCommandHandlerTests::Chup
//-----------------------------------------------------------------------------
// 
TInt CCommandHandlerTests::Chup( TTestResult& aResult )
    {
    TPhCltComHandCommandParameters Param;
    Param.iTelNumber = KPhoneNumber1;
    TRequestStatus iStatus;
    
    CActiveScheduler* scheduler = new( ELeave )CActiveScheduler();
    CActiveScheduler::Install( scheduler );
    CleanupStack::PushL( scheduler );
    
    CPhCltCommandHandler* handler = CPhCltCommandHandler::NewL();
    
    handler->Chup( iStatus );        
    User::WaitForRequest( iStatus );
    
    delete handler;
    CleanupStack::PopAndDestroy( scheduler );
    
    _LIT( KDescription, "Chup Done" );
    aResult.SetResult( iStatus.Int(), KDescription );
    
    return iStatus.Int();
        
    }    

//-----------------------------------------------------------------------------
// CCommandHandlerTests::Ata
//-----------------------------------------------------------------------------
// 
TInt CCommandHandlerTests::Ata( TTestResult& aResult )
    {
    
    TRequestStatus iStatus;
    CActiveScheduler* scheduler = new( ELeave )CActiveScheduler();
    CActiveScheduler::Install( scheduler );
    CleanupStack::PushL( scheduler );
    
    CPhCltCommandHandler* handler = CPhCltCommandHandler::NewL();
    
    handler->Ata( iStatus );        
    User::WaitForRequest( iStatus );
  
    delete handler;
    CleanupStack::PopAndDestroy( scheduler );
    
    _LIT( KDescription, "Ata Done" );
    aResult.SetResult( iStatus.Int(), KDescription );
    
    return iStatus.Int();
        
    }
    
//-----------------------------------------------------------------------------
// CCommandHandlerTests::Chld
//-----------------------------------------------------------------------------
// 
TInt CCommandHandlerTests::Chld( TTestResult& aResult )
    {
    
    TRequestStatus iStatus;
    CActiveScheduler* scheduler = new( ELeave )CActiveScheduler();
    CActiveScheduler::Install( scheduler );
    CleanupStack::PushL( scheduler );
    
    CPhCltCommandHandler* handler = CPhCltCommandHandler::NewL();
    
    handler->Chld( iStatus, EPhCltChldZero, 0 );        
    User::WaitForRequest( iStatus );
  
    delete handler;
    CleanupStack::PopAndDestroy( scheduler );
    
    _LIT( KDescription, "Chld Done" );
    aResult.SetResult( iStatus.Int(), KDescription );
    
    return iStatus.Int();
        
    }    
 
//-----------------------------------------------------------------------------
// CCommandHandlerTests::Vts
//-----------------------------------------------------------------------------
//

TInt CCommandHandlerTests::Vts( TTestResult& aResult ) 
    {
    
    TPhCltComHandCommandParameters Param;
    
    TRequestStatus iStatus;
    CActiveScheduler* scheduler = new( ELeave )CActiveScheduler();
    CActiveScheduler::Install( scheduler );
    CleanupStack::PushL( scheduler );
    
    CPhCltCommandHandler* handler = CPhCltCommandHandler::NewL();
    
    handler->Vts(iStatus, Param.iDtmfTone, EPhCltDtmfNotUsed );      
    User::WaitForRequest( iStatus );
  
    delete handler;
    CleanupStack::PopAndDestroy( scheduler );
    
    _LIT( KDescription, "Chld Done" );
    aResult.SetResult( iStatus.Int(), KDescription );
    
    return iStatus.Int();
         
    }         
          
//-----------------------------------------------------------------------------
// TestCPhCltCommandHandler::MuteMicrophone
//-----------------------------------------------------------------------------
// 
TInt CCommandHandlerTests::MuteMicrophone( TTestResult& aResult )
    {
    
    // TBD, somehow does not work in this environment, 
    // ->User::WaitForRequest( iStatus ) jams
    return KErrNone;       
    }

//-----------------------------------------------------------------------------
// TestCPhCltCommandHandler::CancelAsyncRequest
//-----------------------------------------------------------------------------
// 
TInt CCommandHandlerTests::CancelAsyncRequest( TTestResult& aResult )
    {
    // TBD
    return KErrNone;
    }
 
//-----------------------------------------------------------------------------
// CCommandHandlerTests::MuteRingingTone
//-----------------------------------------------------------------------------
// 
TInt CCommandHandlerTests::MuteRingingTone( TTestResult& aResult )
    {
   
    TRequestStatus iStatus;
    CActiveScheduler* scheduler = new( ELeave )CActiveScheduler();
    CActiveScheduler::Install( scheduler );
    CleanupStack::PushL( scheduler );
    
    CPhCltCommandHandler* handler = CPhCltCommandHandler::NewL();
    
    handler->MuteRingingTone( iStatus );
          
    User::WaitForRequest( iStatus );
  
    delete handler;
    CleanupStack::PopAndDestroy( scheduler );
    
    _LIT( KDescription, "MuteRingingTone Done" );
    aResult.SetResult( iStatus.Int(), KDescription );
    
    return iStatus.Int();        
    }


// -----------------------------------------------------------------------------
// ?classname::?member_function
// ?implementation_description
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
/*
?type ?classname::?member_function(
   ?arg_type arg,
   ?arg_type arg )
   {

   ?code

   }
*/

// ========================== OTHER EXPORTED FUNCTIONS =========================

// -----------------------------------------------------------------------------
// ?function_name implements...
// ?implementation_description.
// Returns: ?value_1: ?description
//          ?value_n: ?description
//                    ?description
// -----------------------------------------------------------------------------
//
/*
?type  ?function_name(
    ?arg_type arg,  // ?description
    ?arg_type arg )  // ?description
    {

    ?code

    }
*/
//  End of File
