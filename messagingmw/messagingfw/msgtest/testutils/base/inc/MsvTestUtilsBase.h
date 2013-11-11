// Copyright (c) 2000-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
//

#ifndef MSV_TEST_UTILS_BASE_H_
#define MSV_TEST_UTILS_BASE_H_

#include <f32file.h>
#include <e32test.h>
#include <msventry.h>
#include <msvapi.h>
#include <flogger.h>
#include <cntdef.h>
#include <simtsy.h>

//	Forward Declarations

class RResourceFile;
class CLogFilter;
class CLogEvent;
class CMsvSession;
class CMsvEntry;
class CContactItem;

//	Constants

//Test Harness Logging

#define KLogsDir				_L("c:\\logs\\")
#define KMsgLogsDirName			_L("MsgTest")

#define KTestHeader				_L("Test Results")
#define KTestStarting			_L("Test %d Starting")
#define KTestStartingWithDesc	_L("Test %d Starting (%S)")
#define KTestPassed				_L("Test %d OK")
#define KTestFailed				_L("Test %d FAILED (error = %d)")
#define KTestHarnessCompleted	_L("Tests Completed")
#define KTestHarnessFailed		_L("Tests FAILED (error = %d)")
#define KTestCommentPrepend		_L("    ")

#define KTestShowMenu			_L("/manual")

#define KPeriod				10000	// period of timer

#define KUidSASimTsyTestNumberValue (KUidSASimTsyValue+1)			// < UID value for test number.
const TUid KUidSASimTsyTestNumber = {KUidSASimTsyTestNumberValue};	// < UID for test number.

//
//
//	TMsvDummyObserver
//
//

class TMsvDummyObserver : public MMsvSessionObserver
	{

public: 
	void HandleSessionEventL(TMsvSessionEvent, TAny*, TAny*, TAny*)
		{}
	};


//
//
// CTestActive
//
//

class CTestActive : public CActive
	{
public:
	IMPORT_C CTestActive();
	IMPORT_C ~CTestActive();
	IMPORT_C void StartL();
protected:
	IMPORT_C void DoCancel();
	IMPORT_C void RunL();
	};


//
//
// CTestTimer
//
//

class CTestTimer : public CTimer
	{
public:
	IMPORT_C static CTestTimer* NewL();
	IMPORT_C void AfterReq(TTimeIntervalMicroSeconds32 aSec, TRequestStatus& aStatus);
protected:
	IMPORT_C CTestTimer();
	IMPORT_C void RunL();

private:
	TRequestStatus* iReport;
	};

//
//
// CTestUtils
//
//

class CTestUtils : public CBase
	{
public:
//File Logging

	IMPORT_C void TestStart(TInt aTest);
	IMPORT_C void TestStart(TInt aTest, const TDesC& aTestDescription);
	IMPORT_C void TestFinish(TInt aTest, TInt aError = KErrNone);
	IMPORT_C void TestHarnessCompleted();
	IMPORT_C void TestHarnessFailed(TInt aError);
	IMPORT_C void WriteComment(const TDesC& aComment);
	IMPORT_C TInt SeekLogFile(TSeek aMode,TInt& aPos);

//Test Utils
	
	IMPORT_C virtual void CreateAllTestDirectories();
	IMPORT_C void DisplayLogL(const CLogFilter& aFilter, TBool aForward = ETrue);
	IMPORT_C void ClearLogL(const CLogFilter& aFilter);

	// File location
	
	IMPORT_C TInt ResolveFile(const TDesC& aComponent, const TDesC& aFileName, TParse& aParseOut);
	IMPORT_C TInt ResolveLogFile(const TDesC& aFileName, TParse& aParseOut);

	//File writing
//	inline void Write(const TDesC& aTxt, RFile& aFile);
	IMPORT_C TInt DisplayFile(const TDesC& aFileName); //not tested
	void AppendEol(TDes& aTxt, TBool aToFile = ETrue);
	
	inline RFs& FileSession();

	//Display a menu.
	//NOTE: These functions should not be used by most test harnesses
	//      as test harnesses should run through automatically.
	IMPORT_C TInt DisplayMenu(const TDesC& aTitle, TInt aMaxAttempts = 3, TBool aAllowExit = ETrue, TBool aClearScreenBefore = ETrue, TBool aClearScreenAfter = ETrue);
	IMPORT_C void ResetMenu();
	IMPORT_C void AppendToMenuL(const TDesC& aOption);
	IMPORT_C TInt MenuCount() const;
	IMPORT_C void ClearScreen(TBool aClearScreen = ETrue);

	IMPORT_C void Printf(TRefByValue<const TDesC> aFmt,...); //prints to RTest and LogFile, depending on iLogToConsole and iLogToFile

	IMPORT_C TBool SetLogToConsole(TBool aLogToConsole = ETrue);
	IMPORT_C TBool SetLogToFile(TBool aLogToFile = ETrue);

	inline RTest& Test() const;
	inline void operator()(TInt aResult,TInt aLineNum);
	inline void operator()(TInt aResult);

	IMPORT_C TBool RunAuto() const;
	IMPORT_C void SetRunAuto(TBool aRunAuto = ETrue);

	//Add item to the contacts DB
	IMPORT_C TContactItemId CreateContactL(const TDesC& aGivenName, const TDesC& aFamilyName, const TDesC& aPhoneNumber, const TDesC& aEmailAddress, TBool aReplaceContactsDb = EFalse);
	IMPORT_C void SetNameL(CContactItem& aItem,TUid aType,const TDesC& aName, TBool aAddField);
	IMPORT_C void DeleteContactsFileL();

	IMPORT_C HBufC8* ReadFileLC(const TDesC& aFile);

	IMPORT_C void PrintHex(const TDesC& aDes);
	IMPORT_C void PrintHex(const TDesC8& aDes);

	IMPORT_C void OpenResourceFileL(const TDesC& aResFileName, RResourceFile& aResFile);
	IMPORT_C void ReadResourceStringL(RResourceFile& aResFile, TInt aId, TDes& rString);
	IMPORT_C void ReadResourceStringL(const TDesC& aResFileName, TInt aId, TDes& rString);

	IMPORT_C static void SetSimTsyTestNumberL(TInt aTestNumber);
	IMPORT_C static void CopyToSimTsyConfigFileL(RFs& aFs, const TDesC& aFilename);

public:

protected:
	IMPORT_C virtual void ConstructL();
	IMPORT_C CTestUtils(RTest& aRTest);
	IMPORT_C ~CTestUtils();

	IMPORT_C void DoBuf(TDes8& buf, const TDesC& label, const TDesC& data);
	IMPORT_C void DoBuf(TDes8& buf, const TDesC& label, const TDesC8& data);

	IMPORT_C void AppendText(const TDesC& aToken, TDes& aTxt, TInt aMaxTokenLength);
	IMPORT_C void WriteToFileL(RFile& aFile, TDesC8& aInputLine);
	IMPORT_C void WriteToFileL(RFile& aFile, const TDesC& aInputLine);
	IMPORT_C void RemoveRichtextFormating(TDes& aSourceLine);

	void ConstructKeepLogFileL();

	virtual void DisplayLogEventData(const CLogEvent& /*aEvent*/) {};
	void GetEventType(TUid aEventType, TDes& rOutput);
	virtual void Panic(TInt aPanic) = 0;

	void AppendTestName(TDes &aFileName);
	void AppendVariantName(TDes& aFileName);

	TInt OpenMainLogL();
	TInt AppendMainLogL();
	void CloseMainLog();
	void WriteToLog(TRefByValue<const TDesC> aFmt,...);


private:
	TBool CheckLogStatus(const TBool aEventViewRet, const TRequestStatus& aStatus);
	TInt DoDisplayMenu(const TDesC& aTitle, TInt& aMaxAttempts, TBool aAllowExit, TBool aClearScreenBefore = ETrue, TBool aClearScreenAfter = ETrue);

protected:

	CArrayPtrFlat<HBufC>*	iMenu;

	RFs						iFs;
	RFile					iFile;
	RTest&					iRTest;

	TFileName				iTestLogFile;
	TBool					iLogToConsole;
	TBool					iLogToFile;

	TBool					iRunAuto;
	HBufC*					iLogBuf;
	HBufC8*					iLogBuf8;
	};

class TTestOverflow : public TDes16Overflow
	{
public:
	TTestOverflow(CTestUtils& aTest)
		: iTest(aTest) {};

	// TDes16Overflow pure virtual
	virtual void Overflow(TDes16& /*aDes*/) {iTest.Printf(_L("ERROR: Printf Overflow\n")); iTest.Test()(EFalse);};

private:
	CTestUtils& iTest;
	};


//
// CTestUtilsWatcherStarter 
//

const TInt KTestUtilsWatcherStarterMin = 5000000;
class CWatcher;

class CTestUtilsWatcherStarter : public CActive
	{
	public:
		IMPORT_C static CTestUtilsWatcherStarter* NewL(TRequestStatus& aStatus, TTimeIntervalMicroSeconds32 aWaitToStart = KTestUtilsWatcherStarterMin, TInt aPriority = EPriorityStandard);
		IMPORT_C static CTestUtilsWatcherStarter* NewL(TInt aPriority = EPriorityStandard);
		IMPORT_C ~CTestUtilsWatcherStarter();

	private:
		CTestUtilsWatcherStarter(TInt aPriority);
		void RunL();
		void DoCancel();
		void ConstructL();
		void ConstructL(TRequestStatus& aStatus, TTimeIntervalMicroSeconds32 aWaitToStart);

	private:
		RTimer iTimer;
		
		RProcess iWatcher;
		TBool iConstructed;
		TRequestStatus* iObserver;
	};


#include <msvtestutilsbase.inl>

#endif
