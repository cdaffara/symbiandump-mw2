/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Active Scheduler Test Module
*
*/


// INCLUDE FILES
#include <StifTestInterface.h>
#include "ActiveScheduledTestModuleBase.h"

// EXTERNAL DATA STRUCTURES
// None

// EXTERNAL FUNCTION PROTOTYPES
// None

// CONSTANTS
// None

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

// ==================== LOCAL FUNCTIONS =======================================
// None


/*-------------------------------------------------------------------------------

    DESCRIPTION

    This module contains the implementation of ActiveScheduledTestModuleBase class
	member functions.

-------------------------------------------------------------------------------*/

// ================= MEMBER FUNCTIONS =========================================

/*-------------------------------------------------------------------------------

    Class: CActiveScheduledTestModuleBase

    Method: CActiveScheduledTestModuleBase

    Description: Default constructor

    C++ default constructor can NOT contain any code, that might leave.

    Parameters:	None
    Return Values: None
    Errors/Exceptions: None
    Status: Draft

------------------------------------------------------------------------------- */
CActiveScheduledTestModuleBase::CActiveScheduledTestModuleBase(TInt aMinTestCaseId, TInt aMaxTestCaseId, const TDesC &aTitleFormat)
	:	iTitleFormat(aTitleFormat) ,
		iOutputPrinter(0, iLogger, KDefaultConsoleTraceDescription, KDefaultTracePrefix) ,
		iMaxTestCaseId(aMaxTestCaseId) ,
		iMinTestCaseId(aMinTestCaseId)
    {}

CActiveScheduledTestModuleBase::CActiveScheduledTestModuleBase
	(
		TInt aMinTestCaseId,
		TInt aMaxTestCaseId,
		const TDesC &aTitleFormat,
		const TDesC &aConsoleTraceDescription,
		const TDesC &aTracePrefix, TInt aPrintPriority
	)
	:	iTitleFormat(aTitleFormat) ,
		iOutputPrinter(0, iLogger, aConsoleTraceDescription, aTracePrefix, aPrintPriority) ,
		iMaxTestCaseId(aMaxTestCaseId) ,
		iMinTestCaseId(aMinTestCaseId)
    {}

	/* -------------------------------------------------------------------------------
    Class: CActiveScheduledTestModuleBase
    Method: ~CActiveScheduledTestModuleBase
    Description: Destructor
    Parameters:	None
    Return Values: None
    Errors/Exceptions: None
    Status: Draft
------------------------------------------------------------------------------- */
CActiveScheduledTestModuleBase::~CActiveScheduledTestModuleBase()
    {
		delete iScheduler;
		delete iLogger;
    }

/* -------------------------------------------------------------------------------
    Class: CActiveScheduledTestModuleBase
    Method: ConstructL
    Description: Symbian OS second phase constructor
    Symbian OS default constructor can leave.
    Parameters:	None
    Return Values: None
    Errors/Exceptions: <description how errors and exceptions are handled>
    Status: Draft
------------------------------------------------------------------------------- */
void CActiveScheduledTestModuleBase::ConstructL()
    {
	iOutputPrinter.SetTestModule(this);
	if ( ! iScheduler)
		{
		iScheduler = new (ELeave) CActiveScheduler;
		CActiveScheduler::Install(iScheduler); // install as active scheduler
		}
    }

/*-------------------------------------------------------------------------------

    Class: CActiveScheduledTestModuleBase

    Method: InitL
    Description: Initialization function.
    Init is used to initialize the Test Module.
    Parameters:	TName& aIniFile: in: Filename:
                    Configuration file for the test module

    Return Values: KErrNone: No error
                   <value_1: Description
		            value_n: Description line 1
			                 description line 2>

    Errors/Exceptions: None
    Status: Draft

------------------------------------------------------------------------------- */
TInt CActiveScheduledTestModuleBase::InitL( TFileName& aIniFile , TBool /*aFirstTime*/ )
    {
	CStifParser *parser=0;

	TParsePtrC fp(aIniFile);
	TRAPD(parserError, parser = CStifParser::NewL(fp.DriveAndPath() , fp.NameAndExt() )   );
	if (!parserError && parser)
		{
		CleanupStack::PushL(parser);

		if (parser)
			{
			CStifSectionParser *section;

			section = parser->SectionL(KSectionStart, KSectionEnd);
			CleanupStack::PushL(section);
			if (section)
				{
				TPtrC logFileName;
				TPtrC logPath;

				if ( (section->GetLine(KLogFileTag, logFileName, ENoTag) == KErrNone)  && (section->GetLine(KLogPathTag, logPath, ENoTag) == KErrNone) )
					{
					InitLoggerL(logPath, logFileName);
					}
				}
			if (!iLogger)
				{
				InitLoggerL();
				}
			iOutputPrinter.Printf(KLoggerStarted, &aIniFile);
			CleanupStack::PopAndDestroy(section);
			}

		CleanupStack::PopAndDestroy(parser);
		}
	else
		{
		iLogger = 0;
		}
	iOutputPrinter.SetLogger(iLogger);

    TInt error = iLogger ? KErrNone : KErrNotFound;
	return error;
	}

/* -------------------------------------------------------------------------------

    Class: CActiveScheduledTestModuleBase
    Method: InitLoggerL
    Description: Initialize the logger for this test module
    Parameters:	File name where to append data
    Return Values: Error value
    Errors/Exceptions: <description how errors and exceptions are handled>
    Status: Draft

------------------------------------------------------------------------------- */
void CActiveScheduledTestModuleBase::InitLoggerL( const TFileName& aLogPath , const TFileName& aLogFile)
    {
		//iLogger = CStifLogger::NewL(aLogPath, aLogFile, ETxt, EFile, EFalse);	//, ETrue, ETrue, ETrue, ETrue, EFalse);
		iLogger = CStifLogger::NewL(aLogPath, aLogFile, CStifLogger::ETxt, CStifLogger::EFile, EFalse) ;
		if (  ! iLogger  )
			{
			InitLoggerL();
			}
    }

void CActiveScheduledTestModuleBase::InitLoggerL()
	{
	iLogger = CStifLogger::NewL(KDefLogPath, KDefLogFile, CStifLogger::ETxt, CStifLogger::EFile, EFalse);
	}


/* -------------------------------------------------------------------------------
    Class: CActiveScheduledTestModuleBase
    Method: GetTestCasesL
    Description: Get test cases.
    GetTestCases is used to inquire test cases from the Test Module. Test
    cases are stored to array of test cases. The Test Framework will be
    the owner of the data in the RPointerArray after GetTestCases return
    and it does the memory deallocation.

    Parameters:	const TFileName& aConfig: in: Test case file (optional)
                RPointerArray<RTestEngine::TTestCaseInfo>& aTestCases: out:
                    Array of TestCases

    Return Values: KErrNone: No error
                   <value_1: Description
		            value_n: Description line 1
			                 description line 2>
    Errors/Exceptions: None
------------------------------------------------------------------------------- */
TInt CActiveScheduledTestModuleBase::GetTestCasesL( const TFileName& aConfig , RPointerArray<TTestCaseInfo>& aTestCases )
    {

	//Get a parser for the config file
	iOutputPrinter.Printf(KOpeningConfig, &aConfig);

	if (! aConfig.Length() ) {
		iOutputPrinter.Printf(KConfigRequired() );
		return KErrConfigInvalid;
	}

	TParsePtrC fp(aConfig);
	CStifParser *parser = CStifParser::NewL(fp.DriveAndPath() , fp.NameAndExt() );
	CleanupStack::PushL(parser);

	if (!parser)
		{
		iOutputPrinter.Printf(KConfigRequired() );
		return KErrPathNotFound;
		}

	//Get a section parser
	CStifSectionParser *section;
	TInt x = 0;

	//Read all cases in config file
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
				if (!ErrorResult) {iOutputPrinter.Printf( _L("Adding case: (%d) %S") , id, &TestCaseTitle ); }
				}
			else
				{
				id = -1;
				}
			CleanupStack::PopAndDestroy(item);
			}

		//Read test case id
		//Read test case title
		//if id is valid then add the test case
		if (iMinTestCaseId<=id && id<=iMaxTestCaseId)
			{
			TTestCaseInfo* testCase = new ( ELeave ) TTestCaseInfo();
			CleanupStack::PushL( testCase );
			testCase->iCaseNumber = x++;

			testCase->iTitle.Format(iTitleFormat, id, &TestCaseTitle);	//testCase->iTitle.Copy( KTestCaseTitleFormat() );

			User::LeaveIfError( aTestCases.Append( testCase ) );

			CleanupStack::Pop( testCase );
			}
		else
			{
			iOutputPrinter.Printf(_L("Skipping test with Id (%d)") , id);
			}

		CleanupStack::PopAndDestroy(section);
		}

	CleanupStack::PopAndDestroy(parser);

    return KErrNone;
    }

/*
-------------------------------------------------------------------------------

    Class: CActiveScheduledTestModuleBase

    Method: RunTestCaseL

    Description: Run a specified testcase.

    RunTestCaseL is used to run an individual test case specified
    by aTestCase. Test cases that can be run may be requested from
    Test Module by GetTestCases method before calling RunTestCase.

    Parameters:	const TInt aCaseNumber: in: case number
                const TFileName& aConfig: in: Test case file (optional)
                TTestResult& aResult: out: Test case result

    Return Values: KErrNone: Test case is started
                   KErrNotFound: Test case not available
                   <value_1: Description
		            value_n: Description line 1
			                 description line 2>

    Errors/Exceptions: None

    Status: Draft

-------------------------------------------------------------------------------
*/
TInt CActiveScheduledTestModuleBase::RunTestCaseL( const TInt aCaseNumber , const TFileName& aConfig , TTestResult& aResult )
    {
	TInt id=-1;

	//Get a parser for the config file
	iOutputPrinter.Printf(KOpeningConfig, &aConfig);

	if (! aConfig.Length() ) {
		iOutputPrinter.Printf(KConfigRequired() );
		aResult.iResultDes.Copy(KConfigRequired());
		aResult.iResult = KErrNotExecuted;
		return KErrConfigInvalid;
	}

	TParsePtrC fp(aConfig);
	iOutputPrinter.Printf(_L("Opening configuration file [%S], case #%d"), &aConfig, aCaseNumber);
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
				if (TitleErrorResult) { iOutputPrinter.Printf( _L("Executing test case (%d)") , id ); }
				else { iOutputPrinter.Printf( _L("Executing test case (%d) %S") , id, &TestCaseTitle ); }

				//Call Funtion to perform test
				__UHEAP_MARK;
				TRAPD(error, retVal = RunTestCaseFunctionL(id, section, aResult););
				__UHEAP_MARKEND;

				iOutputPrinter.Printf( _L("Finished Executing test case (%d), LeaveError=(%d)\r\n") , id, error);

				if (error)
					{
					retVal = KErrLeft;
					aResult.iResult=error;
					aResult.iResultDes.Copy( KTestCaseResultLeft() );
					}
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
	iOutputPrinter.Printf(KConfigInvalid() );
	aResult.iResultDes.Copy(KConfigInvalid());
	aResult.iResult = KErrNotFound;
	return KErrNotFound;
    }

TTimeIntervalMicroSeconds CActiveScheduledTestModuleBase::GetTimeIntervalL(CStifSectionParser *aSection, const TDesC &aTag, TBool &aUsingDefaults, const TTimeIntervalMicroSeconds &aDefaultInterval, TBool aContinue)
	{
	TInt TempInterval1=0 , TempInterval2=0;
	TInt64 tmp=0;
	TInt ParseError=KErrNotFound;
	TTimeIntervalMicroSeconds ReturnValue = aDefaultInterval;
	aUsingDefaults = ETrue;

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
		ParseError = item->GetInt(aTag, TempInterval1) ;
		if (!ParseError)
			{
			//First delay found.
			aUsingDefaults = EFalse;
			//tmp = TempInterval1*1000000;	//convert from seconds to microseconds
			tmp = TempInterval1;
			tmp*= 1000000;

			if ( ! item->GetNextInt(TempInterval2) )
				{
				//tmp += TempInterval2*1000;	//add miliseconds
				TInt64 tmp2 = TempInterval2;
				tmp2 *= 1000;
				tmp += tmp2;

				if ( ! item->GetNextInt(TempInterval2) )
					{
					tmp += TempInterval2;
					}
				}
			ReturnValue = tmp;
			}
		CleanupStack::PopAndDestroy(item);
		}
	if (ParseError) {iOutputPrinter.Printf(_L("Error retrieving interval"));}
	iOutputPrinter.Printf(_L("Interval retrieved: (%d)") , ReturnValue);
	return ReturnValue;
	}

TTimeIntervalMicroSeconds32 CActiveScheduledTestModuleBase::GetTimeIntervalL(CStifSectionParser *aSection, const TDesC &aTag, TBool &aUsingDefault, const TTimeIntervalMicroSeconds32 &aDefaultDelay, TBool aContinue)
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
		if (ParseError) {iOutputPrinter.Printf(_L("Error retrieving report delay"));}
	return ReturnValue;
	}

HBufC8 *CActiveScheduledTestModuleBase::GetDescriptor8LC(const TFileName &aFileName)
	{
	TInt FileError, SoundSize=0;
	RFile file;
	RFs FsSession;
	HBufC8 *SoundFile;

	iOutputPrinter.Printf(_L("Reading descriptor from file [%S]") , &aFileName );
	iOutputPrinter.Printf(_L("Connecting to File Server Session") );
	FileError = FsSession.Connect();
	if (FileError)
		{
		iOutputPrinter.Printf(_L("File error (%d)") , FileError);
		User::LeaveIfError(FileError);
		}

	iOutputPrinter.Printf(_L("Opening file to get size") );
	FileError = file.Open(FsSession, aFileName, EFileStream);
	if (FileError)
		{
		iOutputPrinter.Printf(_L("File error (%d)") , FileError);
		User::LeaveIfError(FileError);
		}
	FileError = file.Size(SoundSize);
	iOutputPrinter.Printf(_L("File Size (%d)") , SoundSize);
	if (FileError)
		{
		iOutputPrinter.Printf(_L("Error getting size") );
		User::LeaveIfError(FileError);
		}

	SoundFile = HBufC8::NewLC(SoundSize);
	TPtr8 helper = SoundFile->Des();
	FileError = file.Read( helper );

	iOutputPrinter.Printf(_L("Descriptor length [%d]") , SoundFile->Size() );
	file.Close();
	FsSession.Close();
	return SoundFile;
	}

TInt CActiveScheduledTestModuleBase::GetIntL(CStifSectionParser *aSection, const TDesC &aTag, TBool &aUsingDefault, const TInt &aDefaultInt, TBool aContinue)
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

	if (ParseError) {iOutputPrinter.Printf(_L("Error retrieving integer, [%S] not found"), &aTag);}
	iOutputPrinter.Printf(_L("Integer read, value (%d)") , ReturnValue);
	return ReturnValue;
	}

TBool CActiveScheduledTestModuleBase::IsDurationInRange
	(	TInt aRepeatTimes, TTimeIntervalMicroSeconds aToneDuration, TTimeIntervalMicroSeconds aRepeatSilence,
		TTimeIntervalMicroSeconds aErrorRange , TTime aStartTime , TTime aEndTime )
	{
	TInt64 tmp=aToneDuration.Int64();
	TInt64 tmp2=aRepeatSilence.Int64();

	tmp*=(aRepeatTimes+1);  tmp2*=(aRepeatTimes+1);
	TTimeIntervalMicroSeconds ExpectedDuration = tmp+tmp2;

	TTime EndTimeRange1 = aStartTime;
	EndTimeRange1+= ExpectedDuration;
	TTime EndTimeRange2 = EndTimeRange1;
	EndTimeRange1-=aErrorRange;
	EndTimeRange2+=aErrorRange;

	iOutputPrinter.Printf(_L("Play duration (%d)") , aEndTime.MicroSecondsFrom(aStartTime) );
	iOutputPrinter.Printf(_L("Expected duration (%d)") , ExpectedDuration);
	return (EndTimeRange1<aEndTime && aEndTime<EndTimeRange2) ;
	}
