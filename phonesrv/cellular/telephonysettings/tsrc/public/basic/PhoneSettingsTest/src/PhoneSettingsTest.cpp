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
* Description:  CPhoneSettingsTest class member functions
*
*/



// INCLUDE FILES
#include <Stiftestinterface.h>
#include <StifParser.h>
#include "PhoneSettingsTest.h"
#include "PhoneSettingsTestParser.h"
#include "PhoneSettingsTestCallBarring.h" 
#include "PhoneSettingsTestCallDiverting.h" 
#include "PhoneSettingsTestCallWaiting.h" 
#include "PhoneSettingsTestCli.h" 
#include "PhoneSettingsTestNetwork.h" 
#include "PhoneSettingsTestCSP.h" 
#include "PhoneSettingsTestSAObserver.h"
#include "PhoneSettingsTestSettingsContainer.h" 

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::CPhoneSettingsTest
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPhoneSettingsTest::CPhoneSettingsTest()
    {
	iMessageOption = EMessagesLogAndUi; 
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::ConstructL
// Symbian 2nd phase constructor can leave.
//
// Note: If OOM test case uses STIF Logger, then STIF Logger must be created
// with static buffer size parameter (aStaticBufferSize). Otherwise Logger 
// allocates memory from heap and therefore causes error situations with OOM 
// testing. For more information about STIF Logger construction, see STIF Users 
// Guide.
// -----------------------------------------------------------------------------
//
void CPhoneSettingsTest::ConstructL()
    {
    iLogger = CStifLogger::NewL( 
    	KPhoneSettingsTestLogPath, 
    	KPhoneSettingsTestLogFile );    
    
	iTestScheduler = new (ELeave) CActiveScheduler;	
	CActiveScheduler::Install( iTestScheduler );	
	
    iTestParser = CPhoneSettingsTestParser::NewL( *this );
    
    // Construct test objects 
    iPhoneSettingsTestCallBarring = CPhoneSettingsTestCallBarring::NewL( 
        *iTestParser, *this );
    iPhoneSettingsTestCallDiverting = CPhoneSettingsTestCallDiverting::NewL( 
        *iTestParser, *this );
    iPhoneSettingsTestCallWaiting = CPhoneSettingsTestCallWaiting::NewL( 
        *iTestParser, *this );    
    iPhoneSettingsTestCli = CPhoneSettingsTestCli::NewL( 
        *iTestParser, *this );    
    iPhoneSettingsTestNetwork = CPhoneSettingsTestNetwork::NewL( 
        *iTestParser, *this );    
    iPhoneSettingsTestCSP = CPhoneSettingsTestCSP::NewL( 
        *iTestParser, *this );    
    iPhoneSettingsTestSAObserver = CPhoneSettingsTestSAObserver::NewL( 
        *iTestParser, *this );    
    iPhoneSettingsTestContainer = CPhoneSettingsTestSettingsContainer::NewL( 
        *iTestParser, *this );    
    } 

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPhoneSettingsTest* CPhoneSettingsTest::NewL()
    {
    CPhoneSettingsTest* self = new (ELeave) CPhoneSettingsTest; 
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// Destructor
CPhoneSettingsTest::~CPhoneSettingsTest()
    {          		
	delete iPhoneSettingsTestContainer; 
	delete iPhoneSettingsTestSAObserver; 
	delete iPhoneSettingsTestCSP; 
	delete iPhoneSettingsTestNetwork; 
	delete iPhoneSettingsTestCli; 
	delete iPhoneSettingsTestCallWaiting; 	
	delete iPhoneSettingsTestCallDiverting;
	delete iPhoneSettingsTestCallBarring; 
	delete iTestParser;  	
	delete iTestScheduler; 
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::InitL
// InitL is used to initialize the Test Module.
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::InitL( 
    TFileName& /*aIniFile*/, 
    TBool /*aFirstTime*/)
    {        
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::GetTestCasesL
// GetTestCases is used to inquire test cases from the Test Module. Test
// cases are stored to array of test cases. The Test Framework will be 
// the owner of the data in the RPointerArray after GetTestCases return
// and it does the memory deallocation. 
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::GetTestCasesL( 
    const TFileName& /*aConfig*/, 
    RPointerArray<TTestCaseInfo>& aTestCases)
    {
    // Loop through all test cases and create new
    // TTestCaseInfo items and append items to aTestCase array    
    for( TInt i = 0; Case(i).iMethod != NULL; i++ )
        {
        // Allocate new TTestCaseInfo from heap for a testcase definition.
        TTestCaseInfo* newCase = new( ELeave ) TTestCaseInfo();

        // PushL TTestCaseInfo to CleanupStack.    
        CleanupStack::PushL( newCase );

        // Set number for the testcase.
        // When the testcase is run, this comes as a parameter to RunTestCaseL.
        newCase->iCaseNumber = i;

        // Set title for the test case. This is shown in UI to user.
        newCase->iTitle.Copy( Case(i).iCaseName );

        // Append TTestCaseInfo to the testcase array. After appended 
        // successfully the TTestCaseInfo object is owned (and freed) 
        // by the TestServer. 
        User::LeaveIfError(aTestCases.Append ( newCase ) );

        // Pop TTestCaseInfo from the CleanupStack.
        CleanupStack::Pop( newCase );
        }

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::RunTestCaseL
// RunTestCaseL is used to run an individual test case specified 
// by aTestCase. Test cases that can be run may be requested from 
// Test Module by GetTestCases method before calling RunTestCase.
// -----------------------------------------------------------------------------
//
TInt CPhoneSettingsTest::RunTestCaseL( 
    const TInt aCaseNumber,   
    const TFileName& /*aConfig*/,
    TTestResult& aResult)
    {	
    // Return value
    TInt execStatus = KErrNone;

    // Get the pointer to test case function
    TCaseInfo tmp = Case ( aCaseNumber );

	// Write test case start messages to log    
    iLogger->WriteDelimiter(); 
    _LIT( KLogStartTC, "Starting testcase [%S]" );
    iLogger->Log( KLogStartTC, &tmp.iCaseName);

    // Check that case number was valid
    if ( tmp.iMethod != NULL )
        {
        // Valid case was found, call it via function pointer
        iMethod = tmp.iMethod;        
        execStatus  = ( this->*iMethod )( aResult );
        }
    else
        {
        // Valid case was not found, return error.
        execStatus = KErrNotFound;
        }
	
	// Print result to log/Stif UI
	if( KErrNone == aResult.iResult )
		{
		PrintMessage( KResultPASS() );	
		}
	else
		{
		PrintMessage( KResultFAIL() );	
		TBuf<KMessageHandlerMaxMessageSize> buffer;
		buffer.Copy( KErrorCode );	
		buffer.AppendNum( aResult.iResult );			
		PrintMessage( buffer );	
		}
	
	// If STIF Ui printing is used then a UI flush is needed after each test case 
	if( (EMessagesUi==iMessageOption)||(EMessagesLogAndUi==iMessageOption) )
		{
		TestModuleIf().Printf( iStifUiLineCounter, KNullDesC, KNullDesC ); 		
		iStifUiLineCounter = 0; 		
		}
		
    // Return case execution status (not the result of the case execution)
    return execStatus;
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::OOMTestQueryL
// Used to check if a particular test case should be run in OOM conditions and 
// which memory allocations should fail.    
// -----------------------------------------------------------------------------
//
TBool CPhoneSettingsTest::OOMTestQueryL( 
    const TFileName& /* aTestCaseFile */, 
    const TInt aCaseNumber, 
    TOOMFailureType& /* aFailureType */, 
    TInt& aFirstMemFailure, 
    TInt& aLastMemFailure) 
    {
    aFirstMemFailure = Case( aCaseNumber ).iFirstMemoryAllocation;
    aLastMemFailure = Case( aCaseNumber ).iLastMemoryAllocation;

    return Case( aCaseNumber ).iIsOOMTest;
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::OOMTestInitializeL
// Used to perform the test environment setup for a particular OOM test case. 
// Test Modules may use the initialization file to read parameters for Test 
// Module initialization but they can also have their own configure file or 
// some other routine to initialize themselves.  
//
// NOTE: User may add implementation for OOM test environment initialization.
// Usually no implementation is required.
// -----------------------------------------------------------------------------
//
void CPhoneSettingsTest::OOMTestInitializeL( 
    const TFileName& /*aTestCaseFile*/, 
    const TInt /*aCaseNumber*/)
    {
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::OOMHandleWarningL
// In some cases the heap memory allocation should be skipped, either due to
// problems in the OS code or components used by the code being tested, or even 
// inside the tested components which are implemented this way on purpose (by 
// design), so it is important to give the tester a way to bypass allocation 
// failures.
//
// NOTE: User may add implementation for OOM test warning handling. Usually no
// implementation is required.
// -----------------------------------------------------------------------------
//
void CPhoneSettingsTest::OOMHandleWarningL( 
    const TFileName& /*aTestCaseFile*/,
    const TInt /*aCaseNumber*/, 
    TInt& /*aFailNextValue*/ )
    {
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::OOMTestFinalizeL
// Used to perform the test environment cleanup for a particular OOM test case.
//
// NOTE: User may add implementation for OOM test environment finalization.
// Usually no implementation is required.
// -----------------------------------------------------------------------------
//
void CPhoneSettingsTest::OOMTestFinalizeL( 
    const TFileName& /* aTestCaseFile */, 
    const TInt /* aCaseNumber */)
    {
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::PrintMessage
// Implements MPhoneSettingsTestMessageHandler messaging interface. 
// Message is output to file log or to STIF UI. 
// -----------------------------------------------------------------------------
//
void CPhoneSettingsTest::PrintMessage(const TDesC& aMessage)
	{
	switch( iMessageOption )
		{
		case EMessagesLog: 
			iLogger->Log( aMessage );			
			break;
		case EMessagesUi: 
			TestModuleIf().Printf( iStifUiLineCounter, KNullDesC, aMessage );
			iStifUiLineCounter++; 			
			break;
		case EMessagesLogAndUi: 
			iLogger->Log( aMessage );			
			TestModuleIf().Printf( iStifUiLineCounter, KNullDesC, aMessage );
			iStifUiLineCounter++; 
			break;
		case EMessagesOff: 			
			break;			
		default: 
		 	break; 		
		}
	}

// -----------------------------------------------------------------------------
// CPhoneSettingsTest::PrintMessage
// Implements MPhoneSettingsTestMessageHandler messaging interface. 
// Message is output to file log or to STIF UO. 
// -----------------------------------------------------------------------------
//
void CPhoneSettingsTest::PrintMessage(TRefByValue<const TDesC> aMessage,...)
	{
	switch( iMessageOption )
		{
		case EMessagesLog: 
			iLogger->Log( aMessage );			
			break;
		case EMessagesUi: 
			TestModuleIf().Printf( iStifUiLineCounter, KNullDesC, aMessage );
			iStifUiLineCounter++; 			
			break;
		case EMessagesLogAndUi: 
			iLogger->Log( aMessage );			
			TestModuleIf().Printf( iStifUiLineCounter, KNullDesC, aMessage );
			iStifUiLineCounter++; 
			break;		
			
        case EMessagesOff: 									
		default: 
		 	break; 		
		}
	}

// ========================== OTHER EXPORTED FUNCTIONS =========================

// -----------------------------------------------------------------------------
// LibEntryL is a polymorphic Dll entry point
// Returns: CTestModuleBase*: Pointer to Test Module object
// -----------------------------------------------------------------------------
//
EXPORT_C CTestModuleBase* LibEntryL()
    {
    return CPhoneSettingsTest::NewL();
    }

// -----------------------------------------------------------------------------
// SetRequirements handles test module parameters(implements evolution
// version 1 for test module's heap and stack sizes configuring).
// Returns: TInt: Symbian error code.
// -----------------------------------------------------------------------------
//
EXPORT_C TInt SetRequirements( 
	CTestModuleParam*& /*aTestModuleParam*/, 
    TUint32& /*aParameterValid*/)
    {
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
