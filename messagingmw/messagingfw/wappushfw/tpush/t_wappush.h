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

#ifndef __T_WAPPUSH_H__
#define __T_WAPPUSH_H__

#include <e32std.h>
#include <e32base.h>
#include <e32cons.h>
#include "testlog.h"

_LIT(KTextFinishedTest,"Finished all tests successfully - Press any key");
_LIT(KTextFailedTest,"!!! TEST FAILED !!! - Press any key");
const TInt KPushLogBuffer=256;

// forward declarations
class CWapPushTestEngine;
class CPushMessage;



/**
 * This class represents a single test case to be run
 */
class CWapPushTest : public CActive, public MWapPushLog
	{
public:
	CWapPushTest() : CActive(CActive::EPriorityStandard) {};
	//
	inline void SetEngine(CWapPushTestEngine* aEngine)	{ iEngine = aEngine; }
	void BeginTest();
	void Printf(const TDesC& aString);
	void Test(TInt aTestResult,TInt aLine);
public:
	// from MWapPushLog
	virtual void WPLPrintf(const TDesC& aDescription);
	virtual void WPLPrintfL(CPushMessage& aMessage);
	virtual void WPLLogBinaryAsHex(const TDesC& aDescription);
	virtual void WPLLogError(const TDesC& aDescription,TInt aError);
	virtual const TDesC& TestName()=0;
protected:
	CWapPushTestEngine* iEngine;

	};





/**
 * This class drives CWapPushTest derived test objects
 * 
 */
class CWapPushTestEngine : public CActive
	{
public:
	static CWapPushTestEngine* NewL();
	CWapPushTestEngine() : CActive(CActive::EPriorityUserInput) {}
	virtual ~CWapPushTestEngine();
	virtual void TestCompleted(TInt aResult);
	void Printf(const TDesC& aString);
	void Printf(const TDesC& aName, const TDesC& aString);
	TKeyCode Getch();
	CConsoleBase& Console() const;
protected:
	// from CActive
	virtual void DoCancel();
	virtual void RunL();
	void RunNextTestL();
	void ConstructL();
	void InitialiseMsgServiceL();
	
	enum TState { EIdle=0, ERunningTest, EShuttingDown};
	enum TTest	{
					EUnknownMessage,
					ESLMessage, 
					ESIMessage,
					ESICMessage,
					ESLCMessage,
					EMultipartRelated,
					EMultipartAlternative,
					EMultipartMixed,
					ESIAMessage,
					EReleaseThree,
					EClientMtmFind,
					EFailMessageTest,		
					EINC081489,
					EDRMMessage,
					EInvalidDTDMessage,
					ECorruptMsgs,
					EReleaseTwo,
					EFinished
					};

	TState				iState;
	TTest				iTest;
	CWapPushTest*		iCurrentTest;
	CConsoleBase*		iConsole;
	};

class CWapPushOOMTestEngine : public CWapPushTestEngine
	{
public:
	static CWapPushOOMTestEngine* NewL ();
	~CWapPushOOMTestEngine ();
	void TestCompleted ( TInt aResult );
	TTest NextTest () const;
	void SetNextTest ( TInt aTest );
	TInt Error () const;
	void SetError ( TInt aErr );
	private:
	void DoCancel ();
	void RunL ();
		
private:
	TInt				iError;
	};



#endif
