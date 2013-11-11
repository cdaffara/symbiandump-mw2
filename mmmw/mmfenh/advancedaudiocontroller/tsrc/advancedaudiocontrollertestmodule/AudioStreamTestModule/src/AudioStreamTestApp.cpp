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
* Description:  AudioInputStreamTestApp class member functions
*
*/



// INCLUDE FILES
#include <Stiftestinterface.h>
#include "AudioStreamTestApp.h"

// EXTERNAL DATA STRUCTURES
//extern  ?external_data;

// EXTERNAL FUNCTION PROTOTYPES  
//extern ?external_function( ?arg_type,?arg_type );

// CONSTANTS
//const ?type ?constant_var = ?constant;

// Input Stream Test Cases
const TInt CAudioStreamTestApp::KTestCaseInputNew=301;
const TInt CAudioStreamTestApp::KTestCaseInputNewWithParam=302;
const TInt CAudioStreamTestApp::KTestCaseInputSetAudioProperties=303;
const TInt CAudioStreamTestApp::KTestCaseInputSetGain=304;
const TInt CAudioStreamTestApp::KTestCaseInputSetBalance=305;
const TInt CAudioStreamTestApp::KTestCaseInputSetPriority=306;
const TInt CAudioStreamTestApp::KTestCaseInputSetDataType=307;
const TInt CAudioStreamTestApp::KTestCaseInputOpen=308;
const TInt CAudioStreamTestApp::KTestCaseInputRecord=309;
const TInt CAudioStreamTestApp::KTestCaseInputBitRate=310;

// Output Stream Test Cases
const TInt CAudioStreamTestApp::KTestCaseOutputNew=351;
const TInt CAudioStreamTestApp::KTestCaseOutputNewWithParam=352;
const TInt CAudioStreamTestApp::KTestCaseOutputSetAudioProperties=353;
const TInt CAudioStreamTestApp::KTestCaseOutputOpen=354;
const TInt CAudioStreamTestApp::KTestCaseOutputSetVolume=355;
const TInt CAudioStreamTestApp::KTestCaseOutputSetPriority=356;
const TInt CAudioStreamTestApp::KTestCaseOutputPlay=357;
const TInt CAudioStreamTestApp::KTestCaseOutputSetBalance=358;
const TInt CAudioStreamTestApp::KTestCaseOutputSetDataType=359;
const TInt CAudioStreamTestApp::KTestCaseOutputStop=360;


const TInt CAudioStreamTestApp::MAX_TESTCASE_ID=360;

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
// CAudioStreamTestApp::CAudioStreamTestApp
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CAudioStreamTestApp::CAudioStreamTestApp()
    {
	iErrName[0].Copy(_L("KErrNone"));
	iErrName[1].Copy(_L("KErrNotFound"));
	iErrName[2].Copy(_L("KErrGeneral"));
	iErrName[3].Copy(_L("KErrCancel"));
	iErrName[4].Copy(_L("KErrNoMemory"));
	iErrName[5].Copy(_L("KErrNotSupported"));
	iErrName[6].Copy(_L("KErrArgument"));
	iErrName[7].Copy(_L("KErrTotalLossOfPrecision"));
	iErrName[8].Copy(_L("KErrBadHandle"));
	iErrName[9].Copy(_L("KErrOverflow"));
	iErrName[10].Copy(_L("KErrUnderflow"));
	iErrName[11].Copy(_L("KErrAlreadyExists"));
	iErrName[12].Copy(_L("KErrPathNotFound"));
	iErrName[13].Copy(_L("KErrDied"));
	iErrName[14].Copy(_L("KErrInUse"));
	iErrName[15].Copy(_L("KErrServerTerminated"));
	iErrName[16].Copy(_L("KErrServerBusy"));
	iErrName[17].Copy(_L("KErrCompletion"));
	iErrName[18].Copy(_L("KErrNotReady"));
	iErrName[19].Copy(_L("KErrUnknown"));
	iErrName[20].Copy(_L("KErrCorrupt"));
	iErrName[21].Copy(_L("KErrAccessDenied"));
	iErrName[22].Copy(_L("KErrLocked"));
	iErrName[23].Copy(_L("KErrWrite"));
	iErrName[24].Copy(_L("KErrDisMounted"));
	iErrName[25].Copy(_L("KErrEof"));
	iErrName[26].Copy(_L("KErrDiskFull"));
	iErrName[27].Copy(_L("KErrBadDriver"));
	iErrName[28].Copy(_L("KErrBadName"));
	iErrName[29].Copy(_L("KErrCommsLineFail"));
	iErrName[30].Copy(_L("KErrCommsFrame"));
	iErrName[31].Copy(_L("KErrCommsOverrun"));
	iErrName[32].Copy(_L("KErrCommsParity"));
	iErrName[33].Copy(_L("KErrTimedOut"));
	iErrName[34].Copy(_L("KErrCouldNotConnect"));
	iErrName[35].Copy(_L("KErrCouldNotDisconnect"));
	iErrName[36].Copy(_L("KErrDisconnected"));
	iErrName[37].Copy(_L("KErrBadLibraryEntryPoint"));
	iErrName[38].Copy(_L("KErrBadDescriptor"));
	iErrName[39].Copy(_L("KErrAbort"));
	iErrName[40].Copy(_L("KErrTooBig"));
	iErrName[41].Copy(_L("KErrDivideByZero"));
	iErrName[42].Copy(_L("KErrBadPower"));
	iErrName[43].Copy(_L("KErrDirFull"));
	iErrName[44].Copy(_L("KErrHardwareNotAvailable"));
	iErrName[45].Copy(_L("KErrSessionClosed"));
	iErrName[46].Copy(_L("KErrPermissionDenied"));
    }

// -----------------------------------------------------------------------------
// CAudioStreamTestApp::ConstructL
// Symbian 2nd phase constructor can leave.
// Note: If OOM test case uses STIF Logger, then STIF Logger must be created
// with static buffer size parameter (aStaticBufferSize). Otherwise Logger 
// allocates memory from heap and therefore causes error situations with OOM 
// testing. For more information about STIF Logger construction, see STIF Users 
// Guide.
// -----------------------------------------------------------------------------
//
void CAudioStreamTestApp::ConstructL()
    {
	if ( ! iScheduler) 
		{
		iScheduler = new (ELeave) CActiveScheduler;
		CActiveScheduler::Install(iScheduler); // install as active scheduler
		}
    }

// -----------------------------------------------------------------------------
// CAudioStreamTestApp::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CAudioStreamTestApp* CAudioStreamTestApp::NewL()
    {
    CAudioStreamTestApp* self = new (ELeave) CAudioStreamTestApp;

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;

    }

// Destructor
CAudioStreamTestApp::~CAudioStreamTestApp()
    { 
    delete iLogger;
	delete iScheduler;
    }

// -----------------------------------------------------------------------------
// CAudioStreamTestApp::InitL
// InitL is used to initialize the Test Module.
// -----------------------------------------------------------------------------
//
TInt CAudioStreamTestApp::InitL( 
    TFileName& aIniFile, 
    TBool /*aFirstTime*/ )
    {
	CStifParser *parser;
	RDebug::Print(_L("CAudioStreamTestApp::InitL:"));
	TParsePtrC fp(aIniFile);
	//fp.Set();
	parser = CStifParser::NewL(fp.DriveAndPath() , fp.NameAndExt() );
	CleanupStack::PushL(parser);

	if (parser) 
		{
		RDebug::Print(_L("CAudioStreamTestApp::InitL: parser is not null"));
		CStifSectionParser *section;

		section = parser->SectionL(KSectionStart, KSectionEnd);
		CleanupStack::PushL(section);
		TPtrC logFileName;
		TPtrC logPath;
		if (section) 
			{
			RDebug::Print(_L("CAudioStreamTestApp::InitL: section is not null"));


			if ( (section->GetLine(KLogFileTag, logFileName, ENoTag) == KErrNone)  && (section->GetLine(KLogPathTag, logPath, ENoTag) == KErrNone) ) 
				{
				RDebug::Print(_L("CAudioStreamTestApp::InitL: InitLoggerL(logPath, logFileName);"));
				InitLoggerL(logPath, logFileName);
				}			
			}
		if (!iLogger) 
			{
			RDebug::Print(_L("CAudioStreamTestApp::InitL: InitLoggerL(); again"));
			InitLoggerL();
			}
		iLogger->Log(KOpeningLogFilepath, &logPath);
		iLogger->Log(KLoggerStarted, &aIniFile);
		CleanupStack::PopAndDestroy(section);	//section
		}

	CleanupStack::PopAndDestroy(parser);	//parser

    return KErrNone;
    }

/* -------------------------------------------------------------------------------

    Class: CMmfTsPlay
    Method: InitLoggerL
    Description: Initialize the logger for this test module
    Parameters:	File name where to append data
    Return Values: Error value
    Errors/Exceptions: <description how errors and exceptions are handled>
    Status: Draft
	
------------------------------------------------------------------------------- */
void CAudioStreamTestApp::InitLoggerL( const TFileName& aLogPath , const TFileName& aLogFile)
    {
		iLogger = CStifLogger::NewL(aLogPath, aLogFile, CStifLogger::ETxt, CStifLogger::EFile, EFalse) ;
		if (  ! iLogger  ) 
			{
			RDebug::Print(_L("CAudioStreamTestApp::InitL: InitLoggerL(); with filename failed"));
			InitLoggerL();
			}
    }

void CAudioStreamTestApp::InitLoggerL() 
	{
	RDebug::Print(_L("CAudioStreamTestApp::InitL: InitLoggerL(); with default"));
	iLogger = CStifLogger::NewL(KDefLogPath, KDefLogFile, CStifLogger::ETxt, CStifLogger::EFile, EFalse);
	}

// -----------------------------------------------------------------------------
// CAudioStreamTestApp::GetTestCasesL
// GetTestCases is used to inquire test cases from the Test Module. Test
// cases are stored to array of test cases. The Test Framework will be 
// the owner of the data in the RPointerArray after GetTestCases return
// and it does the memory deallocation. 
// -----------------------------------------------------------------------------
//
TInt CAudioStreamTestApp::GetTestCasesL( 
    const TFileName& aConfig, 
    RPointerArray<TTestCaseInfo>& aTestCases )
    {
   //Get a parser for the config file
	iLogger->Log(KOpeningConfig, &aConfig);

	if (! aConfig.Length() ) {
		iLogger->Log(KConfigRequired() );
		return KErrConfigInvalid;
	}

	TParsePtrC fp(aConfig);
	CStifParser *parser = CStifParser::NewL(fp.DriveAndPath() , fp.NameAndExt() );
	CleanupStack::PushL(parser);

	//Get a section parser
	CStifSectionParser *section;
	TInt x = 0;

	//Read all cases in config file
	//while ( section = parser->SectionL(KTestCaseConfigStart , KTestCaseConfigEnd, x++) )
	
	while ( (section = parser->NextSectionL(KTestCaseConfigStart , KTestCaseConfigEnd) ),section  )	// use comma to avoid warning
		{
		CleanupStack::PushL(section);
		TInt id = -1;
		TPtrC TestCaseTitle;
		TInt ErrorResult = KErrNone;

		CStifItemParser *item;
		item = section->GetItemLineL(KTagId);
		if (item) 
			{
			CleanupStack::PushL(item);

			if (!item->GetInt(KTagId, id)) 
				{
				ErrorResult = section->GetLine(KTagTitle, TestCaseTitle, ENoTag);
				if (!ErrorResult) {iLogger->Log( _L("Adding case: (%d) %S") , id, &TestCaseTitle ); }
				}
			else
				{
				id = -1;
				}
			CleanupStack::PopAndDestroy(item);	//item
			}

		if (0<id && id<=MAX_TESTCASE_ID)
			{
			TTestCaseInfo* testCase = new ( ELeave ) TTestCaseInfo();
			CleanupStack::PushL( testCase );
			testCase->iCaseNumber = x++;
			
			testCase->iTitle.Format(KTestCaseTitleFormat, id, &TestCaseTitle);	//testCase->iTitle.Copy( KTestCaseTitleFormat() );
    
			User::LeaveIfError( aTestCases.Append( testCase ) );
    
			CleanupStack::Pop( testCase );	//testCase
			}
		
		CleanupStack::PopAndDestroy(section);	//section
		}

	CleanupStack::PopAndDestroy(parser);	//parser

    return KErrNone;

    }

// -----------------------------------------------------------------------------
// CAudioStreamTestApp::RunTestCaseL
// RunTestCaseL is used to run an individual test case specified 
// by aTestCase. Test cases that can be run may be requested from 
// Test Module by GetTestCases method before calling RunTestCase.
// -----------------------------------------------------------------------------
//
TInt CAudioStreamTestApp::RunTestCaseL(
    const TInt aCaseNumber,
    const TFileName& aConfig,
    TTestResult& aResult )
    {
	TInt id=-1;

	//Get a parser for the config file
	iLogger->Log(KOpeningConfig, &aConfig);

	if (! aConfig.Length() ) {
		iLogger->Log(KConfigRequired() );
		aResult.iResultDes.Copy(KConfigRequired());
		aResult.iResult = KErrNotExecuted;
		return KErrConfigInvalid;
	}

	TParsePtrC fp(aConfig);
	iLogger->Log(_L("Opening configuration file [%S], case #%d"), &aConfig, aCaseNumber);
	CStifParser *parser = CStifParser::NewL(fp.DriveAndPath() , fp.NameAndExt() );
	CleanupStack::PushL(parser);

	//Get a section parser
	TInt retVal=0;
	CStifSectionParser *section=0;
	CStifItemParser *item=0;
	section = parser->NextSectionL(KTestCaseConfigStart , KTestCaseConfigEnd, aCaseNumber+1);
	
	if (section) {
		CleanupStack::PushL(section);
		item = section->GetItemLineL(KTagId);

		if (item) {
			CleanupStack::PushL(item);

			if (!item->GetInt(KTagId, id)) {
				TPtrC TestCaseTitle;

				TInt TitleErrorResult = section->GetLine(KTagTitle, TestCaseTitle, ENoTag);
				if (!TitleErrorResult) {iLogger->Log( _L("Executing test case (%d) %S") , id, &TestCaseTitle ); }

				//Call Funtion to perform test
				__UHEAP_MARK;
				TRAPD(error, retVal = RunTestCaseFunctionL(id, section, aResult););
                                REComSession::FinalClose();
				__UHEAP_MARKEND;
				
				if (error)
					{
					retVal = KErrLeft;
					aResult.iResult=error;
					aResult.iResultDes.Copy( KTestCaseResultLeft() );
					}				

				iLogger->Log( _L("Finished Executing test case (%d)"), id);
				iLogger->Log( _L("  Error code (%d)"), retVal);
				iLogger->Log( _L("  aResult.iResult=(%d)") , aResult.iResult);
				iLogger->Log( _L("  aResult.iResultDes=(%S)\r\n") , &aResult.iResultDes);

			} else {
				id=-1;
			}
			CleanupStack::PopAndDestroy(item);
		}		
		CleanupStack::PopAndDestroy(section);
	}
	CleanupStack::PopAndDestroy(parser);

	if (-1!=id) 
	{
		return retVal;
	}
	iLogger->Log(KConfigInvalid() );
	aResult.iResultDes.Copy(KConfigInvalid());
	aResult.iResult = KErrNotFound;
    return KErrNotFound;
    }

TInt CAudioStreamTestApp::RunTestCaseFunctionL(TInt id, CStifSectionParser *section, TTestResult &aResult) 
	{
	TInt retValue=KErrConfigInvalid;
	aResult.iResult=KErrNotExecuted;

	switch (id)
		{
		// Audio Input Stream Test Cases
		case KTestCaseInputNew:
		    retValue = TestCaseInputNewL(section, aResult);
		    break;
		case KTestCaseInputNewWithParam:
		    retValue = TestCaseInputNewWithParamL(section, aResult);
		    break;
		case KTestCaseInputSetAudioProperties:
		    retValue = TestCaseInputSetPropL(section, aResult);
		    break;
		case KTestCaseInputSetGain:
		    retValue = TestCaseInputSetGainL(section, aResult);
		    break;
		case KTestCaseInputSetBalance:
		    retValue = TestCaseInputSetBalanceL(section, aResult);
		    break;
		case KTestCaseInputSetPriority:
		    retValue = TestCaseInputSetPriorityL(section, aResult);
		    break;
		case KTestCaseInputSetDataType:
		    retValue = TestCaseInputSetDataTypeL(section, aResult);
		    break;
		case KTestCaseInputOpen:
		    retValue = TestCaseInputOpenL(section, aResult);
		    break;
		case KTestCaseInputRecord:
		    retValue = TestCaseInputRecordL(section, aResult);
		    break;
		case KTestCaseInputBitRate:
		    retValue = TestCaseInputBitRateL(section, aResult);
		    break;

		// Audio Output Stream Test Cases
		case KTestCaseOutputNew:
		    retValue = TestCaseOutputNewL(section, aResult);
		    break;
		case KTestCaseOutputNewWithParam:
		    retValue = TestCaseOutputNewWithParamL(section, aResult);
		    break;
		case KTestCaseOutputSetAudioProperties:
		    retValue = TestCaseOutputSetPropL(section, aResult);
		    break;
		case KTestCaseOutputOpen:
		    retValue = TestCaseOutputOpenL(section, aResult);
		    break;
		case KTestCaseOutputSetVolume:
		    retValue = TestCaseOutputSetVolumeL(section, aResult);
		    break;
		case KTestCaseOutputSetPriority:
		    retValue = TestCaseOutputSetPriorityL(section, aResult);
		    break;
		case KTestCaseOutputPlay:
		    retValue = TestCaseOutputPlayL(section, aResult);
		    break;
		case KTestCaseOutputSetBalance:
		    retValue = TestCaseOutputSetBalanceL(section, aResult);
		    break;
		case KTestCaseOutputSetDataType:
		    retValue = TestCaseOutputSetDataTypeL(section, aResult);
		    break;
		case KTestCaseOutputStop:
		    retValue = TestCaseOutputStopL(section, aResult);
		    break;

		default:
			aResult.iResultDes.Copy(KConfigInvalid());
			aResult.iResult = KErrNotExecuted;
		}

	return retValue;
	}


// -----------------------------------------------------------------------------
// CAudioStreamTestApp::OOMTestQueryL
// Used to check if a particular test case should be run in OOM conditions and 
// which memory allocations should fail.    
//
// NOTE: This method is virtual and must be implemented only if test case
// should be executed using OOM conditions.  
// -----------------------------------------------------------------------------
//
TBool CAudioStreamTestApp::OOMTestQueryL( 
                                const TFileName& /* aTestCaseFile */, 
                                const TInt /* aCaseNumber */, 
                                TOOMFailureType& /* aFailureType */, 
                                TInt& /* aFirstMemFailure */, 
                                TInt& /* aLastMemFailure */ ) 
    {
    //_LIT( KOOMTestQueryL, "CAudioStreamTestApp::OOMTestQueryL" );
    //iLogger->Log( KOOMTestQueryL ); 

    return EFalse;

    }

// -----------------------------------------------------------------------------
// CAudioStreamTestApp::OOMTestInitializeL
// Used to perform the test environment setup for a particular OOM test case. 
// Test Modules may use the initialization file to read parameters for Test 
// Module initialization but they can also have their own configure file or 
// some other routine to initialize themselves.  
//
// NOTE: This method is virtual and must be implemented only if test case
// should be executed using OOM conditions.  
// -----------------------------------------------------------------------------
//
void CAudioStreamTestApp::OOMTestInitializeL( 
                                const TFileName& /* aTestCaseFile */, 
                                const TInt /* aCaseNumber */ )
    {
    }

// -----------------------------------------------------------------------------
// CAudioStreamTestApp::OOMHandleWarningL
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
void CAudioStreamTestApp::OOMHandleWarningL( 
                                const TFileName& /* aTestCaseFile */,
                                const TInt /* aCaseNumber */, 
                                TInt& /* aFailNextValue */ )
    {
    }

// -----------------------------------------------------------------------------
// CAudioStreamTestApp::OOMTestFinalizeL
// Used to perform the test environment cleanup for a particular OOM test case.
//
// NOTE: This method is virtual and must be implemented only if test case
// should be executed using OOM conditions.  
// -----------------------------------------------------------------------------
//                  
void CAudioStreamTestApp::OOMTestFinalizeL( 
                                const TFileName& /* aTestCaseFile */, 
                                const TInt /* aCaseNumber */ )
    {
    }


TMdaPriorityPreference CAudioStreamTestApp::IntToPriorityPreference (TInt aPriorityPreference) 
	{
	switch (aPriorityPreference) 
		{
		case 0: 
			return EMdaPriorityPreferenceNone; break;
		case 1:
			return EMdaPriorityPreferenceTime; break;
		case 2: 
			return EMdaPriorityPreferenceQuality; break;
		case 3:
		default:
			return EMdaPriorityPreferenceTimeAndQuality; break;
		}
	}

HBufC8 *CAudioStreamTestApp::GetDescriptor8LC(const TFileName &aFileName)
	{
	TInt FileError, SoundSize=0;
	RFile file;
	RFs FsSession;
	HBufC8 *SoundFile;

	iLogger->Log(_L("Reading descriptor from file [%S]") , &aFileName );
	iLogger->Log(_L("Connecting to File Server Session") );
	FileError = FsSession.Connect();
	if (FileError)
		{
		iLogger->Log(_L("File error (%d)") , FileError);
		User::LeaveIfError(FileError);
		}

	iLogger->Log(_L("Opening file to get size") );
	FileError = file.Open(FsSession, aFileName, EFileStream);
	if (FileError)
		{
		iLogger->Log(_L("File error (%d)") , FileError);
		User::LeaveIfError(FileError);
		}
	FileError = file.Size(SoundSize);
	iLogger->Log(_L("File Size (%d)") , SoundSize);
	if (FileError)
		{
		iLogger->Log(_L("Error getting size") );
		User::LeaveIfError(FileError);
		}
	
	SoundFile = HBufC8::NewLC(SoundSize);
	TPtr8 helper = SoundFile->Des();
	FileError = file.Read( helper );

	iLogger->Log(_L("Descriptor length [%d]") , SoundFile->Size() );
	file.Close();
	FsSession.Close();
	return SoundFile;
	}

TInt CAudioStreamTestApp::GetIntL(CStifSectionParser *aSection, 
                                  const TDesC &aTag, TBool &aUsingDefault, 
                                  const TInt &aDefaultInt, 
                                  TBool aContinue)
	{
	TInt ParseError=KErrNotFound;
	TInt TempDelay=0;
	TInt ReturnValue=aDefaultInt;

	CStifItemParser *item;
	if (aContinue)
		{
		item = aSection->GetNextItemLineL(aTag);
		}
	else
		{
		 item = aSection->GetItemLineL(aTag);
		}
	if (item)
		{
		CleanupStack::PushL(item);
		ParseError = item->GetInt(aTag, TempDelay) ;
		CleanupStack::PopAndDestroy(item);
		}

	if (ParseError) 
		{	//Integer not found, using defaults
		aUsingDefault=ETrue;
		}
	else
		{	//Integer found.
		aUsingDefault=EFalse;
		ReturnValue=TempDelay;
		}

	if (ParseError) {iLogger->Log(_L("Error retrieving integer, [%S] not found"), &aTag);}
	iLogger->Log(_L("Integer read, value (%d)") , ReturnValue);
	return ReturnValue;
	}

TTimeIntervalMicroSeconds32 CAudioStreamTestApp::GetTimeIntervalL(CStifSectionParser *aSection, 
																	   const TDesC &aTag, 
																	   TBool &aUsingDefault, 
																	   const TTimeIntervalMicroSeconds32 &aDefaultDelay, 
																	   TBool aContinue) 
	{
	TInt ParseError=KErrNotFound;
	TInt TempDelay=0;
	TTimeIntervalMicroSeconds32 ReturnValue=aDefaultDelay;
	CStifItemParser *item;
	if (aContinue)
		{
		item = aSection->GetNextItemLineL(aTag);
		}
	else
		{
		item = aSection->GetItemLineL(aTag);
		}
	if (item)
		{
		CleanupStack::PushL(item);
		ParseError = item->GetInt(aTag, TempDelay) ;
		if (ParseError) 
			{	//Delay not found, using defaults
			aUsingDefault=ETrue;
			}
		else
			{	//Delay found.
			aUsingDefault=EFalse;
			ReturnValue=TempDelay;
			}
			CleanupStack::PopAndDestroy(item);
			}
		if (ParseError) {iLogger->Log(_L("Error retrieving report delay"));}
	return ReturnValue;
	}

TDesC& CAudioStreamTestApp::GetErrorDes(TInt aErr)
{
	TInt err = Abs(aErr);
	if ( (err < 0) || (err > 46) )
	{
		iErrBuf = KTestCaseResultFail;
		return iErrBuf;
	}
	
	iErrBuf = iErrName[err];
	return iErrBuf;
}


// ========================== OTHER EXPORTED FUNCTIONS =========================

// -----------------------------------------------------------------------------
// LibEntryL is a polymorphic Dll entry point
// Returns: CTestModuleBase*: Pointer to Test Module object
// -----------------------------------------------------------------------------
//
EXPORT_C CTestModuleBase* LibEntryL()
    {
    return CAudioStreamTestApp::NewL();

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
