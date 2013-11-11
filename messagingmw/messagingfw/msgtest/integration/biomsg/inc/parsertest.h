// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "smstestutils.h"

#include <msvapi.h>

class TestUiTimer : public CTimer
	{
public:
	static TestUiTimer* NewLC(CSmsTestUtils& aTestUtils);
	~TestUiTimer();

	void RunL();
	void DoCancel();
	void ConstructL();
	void IssueRequest();
	void SetOperation(CMsvOperation*);
	TTimeIntervalMicroSeconds32 period;
protected:
	TestUiTimer(CSmsTestUtils& aTestUtils);
	
	CMsvOperation*				iOperation;
	CSmsTestUtils&				iSmsTest;
	};

class CMsvEntrySelection;
class CBIOClientMtm;
class CInternetAccessTester;
//class CWapAccessTester;
class CParserTest : public CBase
	{
public:
	static CParserTest* NewL(CSmsTestUtils& aTestUtils);
	void StartL(CMsvEntrySelection* aReceived);
	~CParserTest();
private:
	CParserTest(CSmsTestUtils& aTestUtils);
	
	virtual void ConstructL();

	CMsvSession& Session()	{return *iSmsTest.iMsvSession;}
	CMsvEntry& MsvEntry()	{return *iSmsTest.iMsvEntry;}
	CSmsClientMtm& Client()	{return *iSmsTest.iSmsClientMtm;}

	void CreateParserL();
	void ReParseL();
	void ReProcessL();
	void ProcessError(TInt& aError);

	void CheckMessageTypeL();

	void TestInternetAccessL();
	void TestWapAccessL();

	CMsvEntrySelection*		iBioMessages;
	CMsvEntrySelection*		iSelection;

	TInt					iCurrentMessageNum;

	CSmsTestUtils&			iSmsTest;
	CMsvOperation*			iOperation;
	CBIOClientMtm*			iBioClientMtm;
	TestUiTimer*			iTimer;

	TInt					iCurrentTest;
	TBool					iAllTestsSuccessful;

	CInternetAccessTester*	iIapTester;
//	CWapAccessTester*		iWapTester;

	CClientMtmRegistry*		iClientMtmRegistry;

	enum TSmsSendTestState
		{
		EGetMessages,
		ECreateParser,
		EParseAndProcess,
		ETryReparse,
		ETryReprocess
		} iState;
	};
