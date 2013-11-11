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

#include "smstestbase.h"

#include <msvapi.h>

_LIT(KSmsSendRecvTestName, "Send and Receive");

class CEntryObserver : public CBase, public MMsvEntryObserver
	{
public:
	void HandleEntryEventL(TMsvEntryEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3);
	~CEntryObserver();
public:

	static CEntryObserver* NewL(CSmsTestUtils& aTestUtils, const TInt& aCurrentMessageNum);
	const CMsvEntrySelection& Received() const {return *iReceived;}
	TMsvId Received(TInt aItem) const {return iReceived->At(aItem);}
	TInt Count() const {return iReceived->Count();}
	TMsvId LastReceived() const {return Count() ? Received(Count() - 1) : KErrNotFound;}

private:

	CEntryObserver(CSmsTestUtils& aTestUtils, const TInt& aCurrentMessageNum);
	CMsvEntrySelection* iReceived;
	const TInt& iCurrentMessageNum;
	CSmsTestUtils& iSmsTest;
	};

const TTimeIntervalMicroSeconds32 KWaitForReceiveIteration = 5000000;
const TTimeIntervalMicroSeconds32 KMaxWaitForMsg = 30000000;

//Command Line Switches
_LIT(KParseAfterReceive, "/p");
_LIT(KSaveAfterReceive, "/s");
_LIT(KWaitForever, "/w");
_LIT(KTestBifs, "/b");

//Test Bifs
_LIT(KSendRecvScript, "sendrecv.script");
_LIT(KTestBif1, "Koala.rsc");
_LIT(KTestBif2, "Giraffe.rsc");
_LIT(KBifDir, "c:\\system\\bif\\");
const TTimeIntervalMicroSeconds32 KBifWait = 4000000;

//Forward declarations
class CSmsSendRecvTest : public CSmsTestBase
	{
public:
	static CSmsSendRecvTest* NewLC(CSmsTestUtils& aSmsTest, const TDesC& aScriptFile, TInt& aNextTest);
	~CSmsSendRecvTest();

private:
	CSmsSendRecvTest(CSmsTestUtils& aSmsTest, const TDesC& aScriptFile, TInt& aNextTest);

	void TestSendingL();
	TBool SendNextMessageL();
	void TestWaitForReceiveL();
	void TestParseL();
	void DoRunTestParseL();

	void TestBifsL();
	void TestAddBifL(const TDesC& aBifName);
	void TestRemoveBifL(const TDesC& aBifName);

	void ConstructL();

	void RunL();
	void DoCancel();

	TInt RunError(TInt aError);
	void RunComplete();

	void ShowMenuL();
	void RunAutoL();

	void DoRunSendingL();
	void DoRunWaitingToReceiveL();
	void DoWaitL();

	void WaitForExtraL();
	void DoRunWaitForExtraL();

	TInt CountRecipientsL(TMsvId aId);

	CEntryObserver* iEntryObserver;

	TInt iSent;
	TInt iTotal;

	TInt iCountWaits;

	CMsvEntry* iEntryForObserver;
	TInt iCurrentMessageNum;
	TBool iParse;
	TBool iSaveToFile;
	TBool iWaitForever;

	enum TSmsSendTestState
		{
		EStateWaiting,
		EStateSending,
		EStateWaitingToReceive,
		EStateParsing,
		EStateMessageReceived,
		EStateWaitingToReceiveAnyExtra
		} iState;

	TBool iTestBifs;
	};
