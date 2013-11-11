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
* Description: This file contains hardcoded module implementation.
*
*/


// [INCLUDE FILES] - do not remove
#include <e32math.h>
#include "CSatelliteUITest.h"

#include <EPos_MPosModulesObserver.h> 
#include <EPos_CPosModuleIdList.h>
#include <EPos_CPosModules.h>
#include <EPos_CPosModuleUpdate.h>


// EXTERNAL DATA STRUCTURES
//extern  ?external_data;

// EXTERNAL FUNCTION PROTOTYPES  
//extern ?external_function( ?arg_type,?arg_type );

// CONSTANTS
//const ?type ?constant_var = ?constant;

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
// CCSatelliteUITest::Case
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
const TCaseInfo CCSatelliteUITest::Case ( 
    const TInt aCaseNumber ) const 
     {

    /**
    * To add new test cases, implement new test case function and add new 
    * line to KCases array specify the name of the case and the function 
    * doing the test case
    * In practice, do following
    * 1) Make copy of existing test case function and change its name
    *    and functionality. Note that the function must be added to 
    *    CSatelliteUITest.cpp file and to CSatelliteUITest.h 
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
        // [test cases entries] - do not remove
        
        // NOTE: When compiled to GCCE, there must be Classname::
        // declaration in front of the method name, e.g. 
        // CCSatelliteUITest::PrintTest. Otherwise the compiler
        // gives errors.
        ENTRY( "TC_CreateInstance", CCSatelliteUITest::CreateInstance ),
        ENTRY( "TC_FirmamentView", CCSatelliteUITest::SetandExecuteFirmament ),
        ENTRY( "TC_CompassView", CCSatelliteUITest::SetandExecuteCompass ),
        ENTRY( "TC_SignalStrengthView", CCSatelliteUITest::SetandExecuteSignalStrength ),
        ENTRY( "TC_HandleFGEvent", CCSatelliteUITest::HandleFGEvent),
        ENTRY( "TC_HandleBGEvent", CCSatelliteUITest::HandleBGEvent),
        ENTRY( "TC_BoundryValue_ExecuteLD1", CCSatelliteUITest::ExecuteBVA1),
        ENTRY( "TC_BoundryValue_ExecuteLD2", CCSatelliteUITest::ExecuteBVA2),
        ENTRY( "TC_BoundryValue_ExecuteLD3", CCSatelliteUITest::ExecuteBVA3),
        ENTRY( "TC_BoundryValue_ExecuteLD4", CCSatelliteUITest::ExecuteBVA4),



        

        // Example how to use OOM functionality
        //OOM_ENTRY( "Loop test with OOM", CCSatelliteUITest::LoopTest, ETrue, 2, 3),
        //OOM_FUNCENTRY( CCSatelliteUITest::PrintTest, ETrue, 1, 3 ),
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
// CCSatelliteUITest::CreateInstance
// To test the create instance for CSatelliteUI.
// -----------------------------------------------------------------------------
//
TInt CCSatelliteUITest::CreateInstance( TTestResult& aResult )
    {
	mTCPassed = EFalse;
    TInt32 error = KErrNone;
    error = Initialize();
    if(error != KErrNone)
    	{
    	iLog->Log(_L("FAILED: Test Case fails"));
		mTCPassed = EFalse;
    	}
    else
    	mTCPassed = ETrue;
    Finalize();
	SetResult(aResult);
    return KErrNone;
    }
// -----------------------------------------------------------------------------
// CCSatelliteUITest::SetandExecuteFirmament
// To test the create instance for CSatelliteUI.
// -----------------------------------------------------------------------------
//
TInt CCSatelliteUITest::SetandExecuteFirmament( TTestResult& aResult )
    {
	mTCPassed = EFalse;
    TInt32 error = KErrNone;
    error = Initialize();
    if(error != KErrNone)
    	{
    	iLog->Log(_L("FAILED: Initialization failed"));
    	}
    else
    	{
    	SetView(CASE_FIRMAMENT_VIEW);
    	if(error != KErrNone)
    		{
    		iLog->Log(_L("FAILED: Setting View Failed"));
    		}
    	else
    		{
    		TRAPD(retCode ,error = ExecuteViewL());
    		if(error!= KErrNone || retCode != KErrNone)
    			{
    			iLog->Log(_L("Test Case Fails retCode: %d, error: %d"), retCode, error);
    			}
    		else
    			{
    			mTCPassed = ETrue;
    			iLog->Log(_L("Test Case Passed"));
    			}
    		}
	}
    Finalize();
	SetResult(aResult);
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CCSatelliteUITest::SetandExecuteCompass
// To test Setting and lauching of Satellite info in Compass view.
// -----------------------------------------------------------------------------
//
TInt CCSatelliteUITest::SetandExecuteCompass( TTestResult& aResult )
    {
	mTCPassed = EFalse;
    TInt32 error = KErrNone;
    error = Initialize();
    if(error != KErrNone)
    	{
    	iLog->Log(_L("FAILED: Initialization failed"));
    	}
    else
    	{
    	SetView(CASE_COMPASS_VIEW);
    	if(error != KErrNone)
    		{
    		iLog->Log(_L("FAILED: Setting View Failed"));
    		}
    	else
    		{
    		TRAPD(retCode ,error = ExecuteViewL());
    		if(error!= KErrNone || retCode != KErrNone)
    			{
    			iLog->Log(_L("Test Case Fails retCode: %d, error: %d"), retCode, error);
    			}
    		else
    			{
    			iLog->Log(_L("Test Case Passed"));
    			mTCPassed = ETrue;
    			}
    		}
    	}
    	
    Finalize();
	SetResult(aResult);
    return KErrNone;
    }
// -----------------------------------------------------------------------------
// CCSatelliteUITest::SetandExecuteCompass
// To test Setting and lauching of Satellite info in SignalStrength view.
// -----------------------------------------------------------------------------
//
TInt CCSatelliteUITest::SetandExecuteSignalStrength( TTestResult& aResult )
    {
	mTCPassed = EFalse;
    TInt32 error = KErrNone;
    error = Initialize();
    if(error != KErrNone)
    	{
    	iLog->Log(_L("FAILED: Initialization failed"));
    	}
    else
    	{
    	SetView(CASE_SIGNALSTRENGTH_VIEW);
    	if(error != KErrNone)
    		{
    		iLog->Log(_L("FAILED: Setting View Failed"));
    		}
    	else
    		{
    		TRAPD(retCode ,error = ExecuteViewL());
    		if(error!= KErrNone || retCode != KErrNone)
    			{
    			iLog->Log(_L("Test Case Fails retCode: %d, error: %d"), retCode, error);
    			}
    		else
    			{
    			iLog->Log(_L("Test Case Passed"));
    			mTCPassed = ETrue;
    			}
    		}
    	}
    Finalize();
	SetResult(aResult);
    return KErrNone;
    }
// -----------------------------------------------------------------------------
// CCSatelliteUITest::HandleFGEvent
// To test HandleForegroundEventL.
// -----------------------------------------------------------------------------
//
TInt CCSatelliteUITest::HandleFGEvent( TTestResult& aResult )
    {
	mTCPassed = EFalse;
    TInt error = KErrNone;
    error = Initialize();
    if(error == KErrNone)
    	{
		TRAPD(errorCode , HandleFGEventL(ETrue));
		if(errorCode != KErrNone)
			{
				iLog->Log(_L("FAILED: Initialization failed"));
			}
				
		else
			mTCPassed = ETrue;
    	}
    Finalize();
	SetResult(aResult);
    return KErrNone;
    }
// -----------------------------------------------------------------------------
// CCSatelliteUITest::HandleBGEvent
// To test HandleForegroundEventL.
// -----------------------------------------------------------------------------
//
TInt CCSatelliteUITest::HandleBGEvent( TTestResult& aResult )
    {
	mTCPassed = EFalse;
    TInt error = KErrNone;
    error = Initialize();
    if(error == KErrNone)
    	{
		TRAPD(errorCode , HandleFGEventL(EFalse));
		if(errorCode != KErrNone)
			{
				iLog->Log(_L("FAILED: Initialization failed"));
			}
				
		else
			mTCPassed = ETrue;
    	}
    Finalize();
	SetResult(aResult);
    return KErrNone;
    }
// -----------------------------------------------------------------------------
// CCSatelliteUITest::SetandExecuteCompass
// To test Setting and lauching of Satellite info in SignalStrength view.
// -----------------------------------------------------------------------------
//
//TInt CCSatelliteUITest::CreateInstance_OOM( TTestResult& aResult )
//    {
//    TInt32 error = KErrNone;
//	mTCPassed = ETrue;
//    error = Initialize(2);
//    if(error != KErrNoMemory)
//    	{
//    	iLog->Log(_L("Test Case fails for low memory condition"));
//    	mTCPassed = EFalse;
//    	}
//    Finalize();
//	SetResult(aResult);
//    return KErrNone;
//    }
// -----------------------------------------------------------------------------
// CCSatelliteUITest::ExecuteBVA1
// Boundry Value Analysis for ExecuteLD().
// -----------------------------------------------------------------------------
//
TInt CCSatelliteUITest::ExecuteBVA1( TTestResult& aResult )
    {
	mTCPassed = EFalse;
    TInt32 error = KErrNone;
    error = Initialize();
    if(error != KErrNone)
    	{
    	iLog->Log(_L("FAILED: Initialization failed"));
    	}
    else
    	{
    	SetView(CASE_FIRMAMENT_VIEW);
    	if(error != KErrNone)
    		{
    		iLog->Log(_L("FAILED: Setting View Failed"));
    		}
    	else
    		{
    		TRAPD(retCode , error= ExecuteViewL(STRING_LENGTH_256));
    		if(retCode!= KErrArgument)
    			{
    			iLog->Log(_L("Test Case Fails retCode: %d, error: %d"), retCode, error);
    			}
    		else
    			{
    			iLog->Log(_L("Test Case Passed"));
    			mTCPassed = ETrue;
    			}
    		}
	}
    if(mExecuteDialog)
    	{
    	delete mExecuteDialog;
    	mExecuteDialog = NULL;
    	}
    if(mEnv)
    	{
    	mEnv->PrepareToExit();
    	mEnv->DestroyEnvironment();
    	mEnv = NULL;
    	}
	SetResult(aResult);
    TestModuleIf().SetBehavior( CTestModuleIf::ETestLeaksHandles );       
    User::After( 20000 );
    return KErrNone;
    }
// -----------------------------------------------------------------------------
// CCSatelliteUITest::ExecuteBVA2
// Boundry Value Analysis for ExecuteLD().
// -----------------------------------------------------------------------------
//
TInt CCSatelliteUITest::ExecuteBVA2( TTestResult& aResult )
    {
	mTCPassed = EFalse;
    TInt32 error = KErrNone;
    error = Initialize();
    if(error != KErrNone)
    	{
    	iLog->Log(_L("FAILED: Initialization failed"));
    	}
    else
    	{
    	SetView(CASE_FIRMAMENT_VIEW);
    	if(error != KErrNone)
    		{
    		iLog->Log(_L("FAILED: Setting View Failed"));
    		}
    	else
    		{
    		TRAPD(retCode ,error = ExecuteViewL(STRING_LENGTH_255));
    		if(error!= KErrNone || retCode != KErrNone)
    			{
    			iLog->Log(_L("Test Case Fails retCode: %d, error: %d"), retCode, error);
    			}
    		else
    			{
    			iLog->Log(_L("Test Case Passed"));
    			mTCPassed = ETrue;
    			}
    		}
	}
    Finalize();
	SetResult(aResult);
    return KErrNone;
    }
// -----------------------------------------------------------------------------
// CCSatelliteUITest::ExecuteBVA3
// Boundry Value Analysis for ExecuteLD().
// -----------------------------------------------------------------------------
//
TInt CCSatelliteUITest::ExecuteBVA3( TTestResult& aResult )
    {
	mTCPassed = EFalse;
    TInt32 error = KErrNone;
    error = Initialize();
    if(error != KErrNone)
    	{
    	iLog->Log(_L("FAILED: Initialization failed"));
    	}
    else
    	{
    	SetView(CASE_FIRMAMENT_VIEW);
    	if(error != KErrNone)
    		{
    		iLog->Log(_L("FAILED: Setting View Failed"));
    		}
    	else
    		{
    		TRAPD(retCode ,error = ExecuteViewL(STRING_LENGTH_254));
    		if(error!= KErrNone || retCode != KErrNone)
    			{	
					iLog->Log(_L("Test Case Fails retCode: %d, error: %d"), retCode, error);
    			}
    		else
    			{
				iLog->Log(_L("Test Case Passed"));
				mTCPassed = ETrue;
    			}
    		}
    	}
    Finalize();
	SetResult(aResult);
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CCSatelliteUITest::ExecuteBVA4
// Boundry Value Analysis for ExecuteLD().
// -----------------------------------------------------------------------------
//
TInt CCSatelliteUITest::ExecuteBVA4( TTestResult& aResult )
    {
	mTCPassed = EFalse;
    TInt32 error = KErrNone;
    error = Initialize();
    if(error != KErrNone)
    	{
    	iLog->Log(_L("FAILED: Initialization failed"));
    	}
    else
    	{
    	SetView(CASE_FIRMAMENT_VIEW);
    	if(error != KErrNone)
    		{
    		iLog->Log(_L("FAILED: Setting View Failed"));
    		}
    	else
    		{
    		TRAPD(retCode , error= ExecuteViewL(NULL_STRING));
    		if(retCode!= KErrArgument)
    			{
    			iLog->Log(_L("Test Case Fails retCode: %d, error: %d"), retCode, error);
    			}
    		else
    			{
    			iLog->Log(_L("Test Case Passed"));
    			mTCPassed = ETrue;
    			}
    		}
	}
    if(mExecuteDialog)
    	{
    	delete mExecuteDialog;
    	mExecuteDialog = NULL;
    	}
    if(mEnv)
    	{
    	mEnv->PrepareToExit();
    	mEnv->DestroyEnvironment();
    	mEnv = NULL;
    	}
	SetResult(aResult);
    TestModuleIf().SetBehavior( CTestModuleIf::ETestLeaksHandles );       
    User::After( 20000 );
    return KErrNone;
    }




//------------------------------------------------------------------------------
//-------------------------------HELPER FUNCTIONS-------------------------------
//------------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// CCSatelliteUITest::Initialize
// Sets up the environment and creates the object of CSatelliteInfoUI
// -----------------------------------------------------------------------------
//
TInt CCSatelliteUITest::Initialize(TInt aCase)
    {
    // This part of the code creates the environment
    TInt32 returnCode = KErrNone;
    mEnv= CEikonEnv::Static();
	if( NULL == mEnv )
		{
		mEnv = new CEikonEnv();
		if( mEnv )
			{
			TRAPD( error, mEnv->ConstructL( ETrue ) );
			if( error != KErrNone )
				{
				iLog->Log(_L("Environment Creation failed!!"));
				returnCode = KErrGeneral;
				}
			else
				iLog->Log(_L("Environment Creation passed!!"));
	
			}
		else
			{
			iLog->Log(_L("FAILED: No Memory to create the environment"));
			returnCode = KErrNoMemory;
			}
		}

	//This part of the code creates appui.
	
    if( mEnv->AppUi() == NULL )
    {
        appUi = new CAppUi();
        if ( appUi )
        {
            TRAPD( err, appUi->ConstructL());
            if( err != KErrNone )
            	{
            	iLog->Log(_L("Appui creation failed!!"));
            	return KErrGeneral; 
            	}
            else
            	{
				iLog->Log(_L("Appui creation success!!"));
				mEnv->SetAppUi( appUi );
            	}
        }
        else
        {   
			iLog->Log(_L("FAILED: No Memory to create appui"));
			return KErrNoMemory;            
        }
    }
    //This creates an object of CExecuteDialog:CActive
    mExecuteDialog = new CExecuteDialog();
    
    TRAPD(retCode, EnableSimPSYL());
    if (retCode != KErrNone)
    	iLog->Log(_L("Enabling simlutator method on the device failed"));
	
	// This part of the code creates the object of CSatelliteInfoUI
    switch(aCase)
    	{
    	case 1:
			TRAPD(error, mSatelliteInfo = CSatelliteInfoUI::NewL());
			if (!mSatelliteInfo||error != KErrNone)
				{
				iLog->Log(_L("FAILED: Createinstance for CreateCSatelliteInfo failed"));
				returnCode = KErrGeneral;
				}
			else
				{
				iLog->Log(_L("Passed: Createinstance for CreateCSatelliteInfo success"));
				}
			break;
			//This part of the code tests the creation of instance in low memory condition
    	case 2:
    		DrainMemory();
			TRAPD(errorCode, mSatelliteInfo = CSatelliteInfoUI::NewL());
			iLog->Log(_L("Creating instance in low memory condition retunrs %d"),error);
			RestoreMemory();
			returnCode = errorCode;
			break;
    	}
    return returnCode;		
    }

// -----------------------------------------------------------------------------
// CCSatelliteUITest::Finalize
// Release the resources
// -----------------------------------------------------------------------------
//
void CCSatelliteUITest::Finalize()
    {
    if(mExecuteDialog)
    	{
    	delete mExecuteDialog;
    	mExecuteDialog = NULL;
    	}
	
	if(mSatelliteInfo)
		{
		delete mSatelliteInfo;
		mSatelliteInfo = NULL;
		}
	
    if(mEnv)
    	{
    	mEnv->PrepareToExit();
    	mEnv->DestroyEnvironment();
    	mEnv = NULL;
    	}
    
    TestModuleIf().SetBehavior( CTestModuleIf::ETestLeaksHandles );       
    User::After( 20000 );
    }

// -----------------------------------------------------------------------------
// CCSatelliteUITest::SetView
// Sets the View for the Dialog
// -----------------------------------------------------------------------------
//
void CCSatelliteUITest::SetView(TInt aViewCase)
    {
    if(mSatelliteInfo)
    	{
    	switch(aViewCase)
    		{
    		default:
    		case CASE_FIRMAMENT_VIEW:
    			mSatelliteInfo->SetLaunchView(CSatelliteInfoUI::ESatelliteFirmamentView);
    			break;
    		case CASE_COMPASS_VIEW:
    			mSatelliteInfo->SetLaunchView(CSatelliteInfoUI::ESatelliteCompassView);
    			break;
    		case CASE_SIGNALSTRENGTH_VIEW:
    			mSatelliteInfo->SetLaunchView(CSatelliteInfoUI::ESatelliteSignalStrengthView);
    			break;
    		case 4:
    			//mSatelliteInfo->SetLaunchView(NULL);
    			break;
    		}
    	}
   }
// -----------------------------------------------------------------------------
// CCSatelliteUITest::SetView
// Sets the View for the Dialog
// -----------------------------------------------------------------------------
//
TInt CCSatelliteUITest::ExecuteViewL(TInt aCase)
    {
    TInt error = KErrNone;
    char *MyString = "";
    //This part of the code generates the resource string
    switch(aCase)
    	{
    	case DEFAULTSTRING:
    		MyString = "Satellite Info";
    		break;
    	case STRING_LENGTH_256:
			//Testing BV
			// 256 char string
			MyString = "1234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678";
			break;
    	case STRING_LENGTH_255:
			// 255 char string
			MyString = "123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567";
			break;
    	case STRING_LENGTH_254:
			// 254 char string
			MyString = "12345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456";
			break;
    	case NULL_STRING:
			// NUll String
			break;
    	}
    
    TBufC8<260> buf8((unsigned char*)MyString);
    TBuf16<260> buf16;
    buf16.Copy(buf8);
    HBufC *hb; 
    hb = buf16.AllocLC();
    iLog->Log(_L("Length of the string %d"), buf16.Length());
    //This part of the code simluates the exit key to exit the dialog box
    mExecuteDialog->SimulateExitKeyL();    
    if(mSatelliteInfo)
			error = mSatelliteInfo->ExecuteLD(* hb);     

    CleanupStack::PopAndDestroy(hb);
    mSatelliteInfo = NULL;
    
    return error;
    }
// -----------------------------------------------------------------------------
// CCSatelliteUITest::DrainMemory
// Eats the memory
// -----------------------------------------------------------------------------
//
void CCSatelliteUITest::DrainMemory()
	{
    for( TInt i = 0; i < MAX_EXPONENT1; i++ )
    {
        cells[i] = NULL;
    }
    // consume all available heap memory
    TInt j=0;
   	for (TInt i = KMaxTInt/2-1; i > 0; )
	{
		cells[j] = User::Alloc(i);
		if( !( cells[j] ) )
		{
		    i/=2;
		}
		else
		{
		    j++;
		}
	}
	}
// -----------------------------------------------------------------------------
// CCSatelliteUITest::RestoreMemory
// Restores the memory
// -----------------------------------------------------------------------------
//
void CCSatelliteUITest::RestoreMemory()
	{
 	for ( TInt cellIndex = 0; cellIndex < MAX_EXPONENT1; cellIndex++ )
		{
			 if ( cells[cellIndex] )
				{
				   User::Free( cells[cellIndex] );
			   }
		 }
	}
// -----------------------------------------------------------------------------
// Function:    CCSatelliteUITest::SetResult
// Description: Helperfunction which writes information about the testcase
//              into provided structure and stiflogger.
// Returns:     None
// -----------------------------------------------------------------------------
    
void CCSatelliteUITest::SetResult(TTestResult& aResult)
    {
    if( mTCPassed )
    {
        aResult.SetResult( KErrNone, _L ( "passed" ) );
        iLog->Log( CStifLogger::EGreen, _L( "TESTCASE PASSED" ) );
    }
    else
    {
        aResult.SetResult( KErrGeneral, _L ( "error" ) );
        iLog->Log( CStifLogger::ERed, _L( "TESTCASE FAILED" ) );
    }
// -----------------------------------------------------------------------------
// Function:    CCSatelliteUITest::HandleForegroundEventL
// Description: Helperfunction to handle FG Events
// Returns:     None
// -----------------------------------------------------------------------------   
    }
void CCSatelliteUITest::HandleFGEventL(TBool aForeground)
	{
    //CAknAppUi::HandleForegroundEventL( aForeground );  

	if(mSatelliteInfo)
		{
		// SatelliteInfo To Handle Foreground Event
		mSatelliteInfo->HandleForegroundEventL(aForeground);
		}
	}

// -----------------------------------------------------------------------------
// Function:    CCSatelliteUITest::EnableSimPSYL
// Description: Helperfunction to enable simPSY
// Returns:     None
// -----------------------------------------------------------------------------   
     
void CCSatelliteUITest::EnableSimPSYL() 
    {
   CPosModules* db = CPosModules::OpenL();
	CleanupStack::PushL( db );

	// List entries
	CPosModuleIdList* idList = db->ModuleIdListLC();
	CPosModuleUpdate* updateParams = CPosModuleUpdate::NewLC();

	// Get the display name and status of each installed positioning plug-in
	for (TInt i = 0; i < idList->Count(); i++)
    {
    // get PSY info
    TPositionModuleInfo moduleInfo;
    db->GetModuleInfoL( (*idList)[i], moduleInfo );
	
    if ( moduleInfo.IsAvailable() )
        {
        // read PSY’s name
        TBuf<KPositionMaxModuleName> moduleName;
        moduleInfo.GetModuleName( moduleName );
        TPositionModuleId id = moduleInfo.ModuleId();
        
		updateParams->SetUpdateAvailability( EFalse );
		db->UpdateModuleL( id, *updateParams );

        }
    }
    TPositionModuleId id2 = {0x101F7A81};
	updateParams->SetUpdateAvailability( ETrue );
	db->UpdateModuleL( id2, *updateParams );

    CleanupStack::PopAndDestroy( updateParams );

    CleanupStack::PopAndDestroy( idList );
	CleanupStack::PopAndDestroy( db );
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
//  [End of File] - do not remove
