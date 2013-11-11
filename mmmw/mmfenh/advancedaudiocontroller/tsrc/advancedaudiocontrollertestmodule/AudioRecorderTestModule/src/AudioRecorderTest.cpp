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
* Description:  AudioRecorder Test
*
*/



// INCLUDE FILES
#include <Stiftestinterface.h>
#include "AudioRecorderTest.h"
#include "RecordClip.h"
#include "RecPositionClip.h"
#include "RecordTimeAvailable.h"
#include "OpenClip.h"
#include "RecordMicGain.h"
#include "RecordBalance.h"
#include "RecordBySize.h"
#include "RecordMetaData.h"
#include "RecordPriority.h"
#include "PlayBalance.h"
#include "RecordFormats.h"
#include "PlayClip.h"
#include "PlayVolume.h"
#include "PlayRamp.h"
#include "PlayWindow.h"
#include "RecordCropClip.h"
#include "RecordBitRate.h"
#include "RecordSampleRate.h"
#include "RecordDataType.h"
#include "RecordDataFormat.h"
#include "RecordChannel.h"
#include "RecordMaxLength.h"
#include "ThreadPriority.h"
#include "AudioResourceNotification.h"
#include "AudioLoadingProgress.h"
#include "CropFromBeginning.h"
#include "MetaDataEntry.h"
#include "CustomCommand.h"
#include "CustomCommandAsync.h"
#include "RecordOpen.h"
#include "AudioPreempionARN.h"



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
// CAudioRecorderTest::CAudioRecorderTest
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CAudioRecorderTest::CAudioRecorderTest()
{
	iLogger = NULL;
	for (TInt i = 0; i < (KNumberOfRecordTests+1); i++)
		counterArray[i] = 0;

}

// -----------------------------------------------------------------------------
// CAudioRecorderTest::ConstructL
// Symbian 2nd phase constructor can leave.
// Note: If OOM test case uses STIF Logger, then STIF Logger must be created
// with static buffer size parameter (aStaticBufferSize). Otherwise Logger
// allocates memory from heap and therefore causes error situations with OOM
// testing. For more information about STIF Logger construction, see STIF Users
// Guide.
// -----------------------------------------------------------------------------
//
void CAudioRecorderTest::ConstructL()
{

}

// -----------------------------------------------------------------------------
// CAudioRecorderTest::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CAudioRecorderTest* CAudioRecorderTest::NewL()
{
    CAudioRecorderTest* self = new (ELeave) CAudioRecorderTest;

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;

}

// Destructor
CAudioRecorderTest::~CAudioRecorderTest()
{
    delete iLogger;
}

// -----------------------------------------------------------------------------
// CAudioRecorderTest::InitL
// InitL is used to initialize the Test Module.
// -----------------------------------------------------------------------------
//
TInt CAudioRecorderTest::InitL( TFileName& aIniFile, TBool /*aFirstTime*/ )
{
	TBool logCreated = false;

	CStifParser *parser = CStifParser::NewL(KLibPath, aIniFile);
	CleanupStack::PushL(parser);

	if (parser)
	{
		CStifSectionParser *section;
		TInt counter = 1;
		TBool moreSections = true;
		TInt result;

		while (moreSections && !logCreated)
		{
			section = parser->SectionL(KInitSectionStart, KInitSectionEnd, counter);
			CleanupStack::PushL(section);

			if (section)
			{
				TPtrC logPath;
				result = section->GetLine(KLogPath, logPath, ENoTag);
				if (result == KErrNone)
				{
					TPtrC logFileName;
					result = section->GetLine(KLogFileName, logFileName, ENoTag);
					if (result == KErrNone)
					{
						iLogger = CStifLogger::NewL(logPath, logFileName, CStifLogger::ETxt, CStifLogger::EFile, EFalse);//, ETrue, ETrue, ETrue, ETrue, EFalse);
						if (iLogger != NULL)
						{
							iLogger->Log(_L("Logger created at specified location"));
							logCreated = true;
						}
					}
				}
			}
			else
				moreSections = false;

			CleanupStack::PopAndDestroy();	//section
			counter++;
		}
	}

	CleanupStack::PopAndDestroy();	//parser

	if (!logCreated)
	{
		iLogger = CStifLogger::NewL(KDefaultLogPath, KDefaultLogFileName, CStifLogger::ETxt, CStifLogger::EFile, EFalse);//, ETrue, ETrue, ETrue, ETrue, EFalse);
		iLogger->Log(_L("Logger created at default location"));
	}

    return KErrNone;
}

// -----------------------------------------------------------------------------
// CAudioRecorderTest::GetTestCasesL
// GetTestCases is used to inquire test cases from the Test Module. Test
// cases are stored to array of test cases. The Test Framework will be
// the owner of the data in the RPointerArray after GetTestCases return
// and it does the memory deallocation.
// -----------------------------------------------------------------------------
//
TInt CAudioRecorderTest::GetTestCasesL( const TFileName& aConfig,
                                        RPointerArray<TTestCaseInfo>& aTestCases )
{

	CStifParser *parser = CStifParser::NewL(KLibPath, aConfig);
	CleanupStack::PushL(parser);

	if (parser)
	{
		CStifSectionParser *section;
		TInt counter = 1;
		TBool moreSections = true;
		TInt result;

		while (moreSections)
		{
			section = parser->SectionL(KSectionStart, KSectionEnd, counter);
			CleanupStack::PushL(section);

			if (section)
			{
				TPtrC name;
				result = section->GetLine(KTestName, name, ENoTag);
				if (result == KErrNone)
				{
					TTestCaseInfo* testCase = new ( ELeave ) TTestCaseInfo();
					CleanupStack::PushL( testCase );

					testCase->iCaseNumber = counter;
					testCase->iTitle.Copy(name);

					User::LeaveIfError( aTestCases.Append( testCase ) );
					CleanupStack::Pop( testCase );
				}
			}
			else
				moreSections = false;

			CleanupStack::PopAndDestroy();	//section
			counter++;
		}
	}

	CleanupStack::PopAndDestroy();	//parser

    return KErrNone;

}

// -----------------------------------------------------------------------------
// CAudioRecorderTest::RunTestCaseL
// RunTestCaseL is used to run an individual test case specified
// by aTestCase. Test cases that can be run may be requested from
// Test Module by GetTestCases method before calling RunTestCase.
// -----------------------------------------------------------------------------
//
TInt CAudioRecorderTest::RunTestCaseL( const TInt aCaseNumber,
                                       const TFileName& aConfig,
                                       TTestResult& aResult )
{
	TInt localError = KErrSyntax;
	CStifParser *parser = CStifParser::NewL(KLibPath, aConfig);
	CleanupStack::PushL(parser);

	if (parser)
	{
		CStifSectionParser *section = parser->SectionL(KSectionStart, KSectionEnd, aCaseNumber);
		CleanupStack::PushL(section);

		if (section)
		{
			CStifItemParser* item = section->GetItemLineL(KTestId);
			CleanupStack::PushL(item);

			if (item)
			{
				TInt id;
				if (item->GetInt(KTestId,id) == KErrNone)
				{
					TInt err = KErrNone;
					TInt execErr = KErrNone;
					localError = KErrNone;

					//Add TRAPD
					TRAPD(trapErr, execErr = ExecuteTestL(id,section,err));
					if (trapErr != KErrNone)
					{
						iLogger->Log(_L("Leave error: %d"),trapErr);
						CleanupStack::PopAndDestroy(3);
						return trapErr;
					}
					REComSession::FinalClose(); 
					if (execErr == KErrNone)
					{
						aResult.iResult = err;
						if (err == KErrNone)
						{
							aResult.iResultDes.Copy( _L("Test passed") );
							iLogger->Log(_L("Test passed"));
						}
						else
						{
							aResult.iResultDes.Copy( _L("Test failed") );
							iLogger->Log(_L("Test failed"));
						}
					}
					else
					{
						iLogger->Log(_L("Invalid test id"));
						localError = KErrInvalidId;
					}
				}
			}

			CleanupStack::PopAndDestroy();	//item
		}

		CleanupStack::PopAndDestroy();	//section
	}

	CleanupStack::PopAndDestroy();	//parser
    return localError;
}

TInt CAudioRecorderTest::ExecuteTestL(const TInt testId, CStifSectionParser* section, TInt & err)
{
	TPtrC name;
	section->GetLine(KTestName, name, ENoTag);

	iLogger->Log(_L("\n"));
	iLogger->Log(_L("Running test: %S"),&name);
	TInt retVal = KErrNone;
	switch (testId)
	{
		case 201: err = CRecordFormats::RunTestL(&TestModuleIf(), iLogger, section,&counterArray[1]);
				break;
		case 202: err = COpenClip::RunTestL(&TestModuleIf(), iLogger, section,&counterArray[2]);
				break;
		case 205: err = CRecordClip::RunTestL(&TestModuleIf(), iLogger, section,&counterArray[5]);
				break;
		case 206: err = CRecordBySize::RunTestL(&TestModuleIf(), iLogger, section,&counterArray[6]);
				break;
		case 207: err = CRecPositionClip::RunTestL(&TestModuleIf(), iLogger, section,&counterArray[7]);
				break;
		case 208: err = CRecordTimeAvailable::RunTestL(&TestModuleIf(), iLogger, section,&counterArray[8]);
				break;
		case 209: err = CRecordMicGain::RunTestL(&TestModuleIf(), iLogger, section,&counterArray[9]);
				break;
		case 210: err = CRecordMetaData::RunTestL(&TestModuleIf(), iLogger, section,&counterArray[10]);
				break;
		case 211: err = CRecordPriority::RunTestL(&TestModuleIf(), iLogger, section,&counterArray[11]);
				break;
		case 212: err = CRecordBalance::RunTestL(&TestModuleIf(), iLogger, section,&counterArray[12]);
				break;
		case 213: err = CPlayVolume::RunTestL(&TestModuleIf(), iLogger, section,&counterArray[18]);
				break;
		case 214: err = CPlayRamp::RunTestL(&TestModuleIf(), iLogger, section,&counterArray[18]);
				break;
		case 215: err = CPlayWindow::RunTestL(&TestModuleIf(), iLogger, section,&counterArray[18]);
				break;
		case 217: err = CPlayClip::RunTestL(&TestModuleIf(), iLogger, section,&counterArray[18]);
				break;
		case 218: err = CPlayBalance::RunTestL(&TestModuleIf(), iLogger, section,&counterArray[18]);
				break;
		case 219: err = CRecordCropClip::RunTestL(&TestModuleIf(), iLogger, section,&counterArray[18]);
				break;
		case 221: err = CRecordBitRate::RunTestL(&TestModuleIf(), iLogger, section,&counterArray[18]);
				break;
		case 222: err = CRecordSampleRate::RunTestL(&TestModuleIf(), iLogger, section,&counterArray[18]);
				break;
		case 223: err = CRecordDataType::RunTestL(&TestModuleIf(), iLogger, section,&counterArray[18]);
				break;
		case 224: err = CRecordDataFormat::RunTestL(&TestModuleIf(), iLogger, section,&counterArray[18]);
				break;
		case 225: err = CRecordChannel::RunTestL(&TestModuleIf(), iLogger, section,&counterArray[18]);
				break;
		case 226: err = CRecordMaxLength::RunTestL(&TestModuleIf(), iLogger, section,&counterArray[18]);
				break;
		case 227: err = CThreadPriority::RunTestL(&TestModuleIf(), iLogger, section,&counterArray[18]);
				break;
		case 228: err = CAudioResourceNotification::RunTestL(&TestModuleIf(), iLogger, section,&counterArray[18]);
				break;
		case 229: err = CAudioLoadingProgress::RunTestL(&TestModuleIf(), iLogger, section,&counterArray[18]);
				break;
		case 230: err = CCropFromBeginning::RunTestL(&TestModuleIf(), iLogger, section,&counterArray[18]);
				break;
		case 231: err = CMetaDataEntry::RunTestL(&TestModuleIf(), iLogger, section,&counterArray[18]);
				break;
		case 232: err = CCustomCommand::RunTestL(&TestModuleIf(), iLogger, section,&counterArray[18]);
				break;
		case 233: err = CCustomCommandAsync::RunTestL(&TestModuleIf(), iLogger, section,&counterArray[18]);
				break;
		case 234: err = CRecordOpen::RunTestL(&TestModuleIf(), iLogger, section,&counterArray[18]);
				break;

// wav cr 
        case 235: err = CAudioPreEmptionARN235::RunTestL(&TestModuleIf(), iLogger, section,&counterArray[18]);
                break;
        case 236: err = CAudioPreEmptionARN236::RunTestL(&TestModuleIf(), iLogger, section,&counterArray[18]);
                break;
        case 237: err = CAudioPreEmptionARN237::RunTestL(&TestModuleIf(), iLogger, section,&counterArray[18]);
                break;				
			//	CTestMmfAclntARN8112::DoTestL()

		default:retVal = KErrInvalidId;
	}
	return retVal;
}

// -----------------------------------------------------------------------------
// CAudioRecorderTest::OOMTestQueryL
// Used to check if a particular test case should be run in OOM conditions and
// which memory allocations should fail.
//
// NOTE: This method is virtual and must be implemented only if test case
// should be executed using OOM conditions.
// -----------------------------------------------------------------------------
//
TBool CAudioRecorderTest::OOMTestQueryL( const TFileName& /* aTestCaseFile */,
                                         const TInt /* aCaseNumber */,
                                         TOOMFailureType& /* aFailureType */,
                                         TInt& /* aFirstMemFailure */,
                                         TInt& /* aLastMemFailure */ )
{
    //_LIT( KOOMTestQueryL, "CAudioRecorderTest::OOMTestQueryL" );
    //iLog->Log( KOOMTestQueryL );

    return EFalse;

}

// -----------------------------------------------------------------------------
// CAudioRecorderTest::OOMTestInitializeL
// Used to perform the test environment setup for a particular OOM test case.
// Test Modules may use the initialization file to read parameters for Test
// Module initialization but they can also have their own configure file or
// some other routine to initialize themselves.
//
// NOTE: This method is virtual and must be implemented only if test case
// should be executed using OOM conditions.
// -----------------------------------------------------------------------------
//
void CAudioRecorderTest::OOMTestInitializeL( const TFileName& /* aTestCaseFile */,
                                             const TInt /* aCaseNumber */ )
{
}

// -----------------------------------------------------------------------------
// CAudioRecorderTest::OOMHandleWarningL
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
void CAudioRecorderTest::OOMHandleWarningL( const TFileName& /* aTestCaseFile */,
                                            const TInt /* aCaseNumber */,
                                            TInt& /* aFailNextValue */ )
{
}

// -----------------------------------------------------------------------------
// CAudioRecorderTest::OOMTestFinalizeL
// Used to perform the test environment cleanup for a particular OOM test case.
//
// NOTE: This method is virtual and must be implemented only if test case
// should be executed using OOM conditions.
// -----------------------------------------------------------------------------
//
void CAudioRecorderTest::OOMTestFinalizeL( const TFileName& /* aTestCaseFile */,
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
    return CAudioRecorderTest::NewL();

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
GLDEF_C TInt E32Dll( TDllReason /*aReason*/) // Reason
{
    return(KErrNone);

}
#endif // EKA2

//  End of File
