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

#define KSendRecvScript _L("biotest.script")

class CMsvEntrySelection;
class CSendReceive : public CActive
	{
public:
	static CSendReceive* NewL(CSmsTestUtils& aTestUtils);
	void StartL(CMsvEntrySelection* aReceived);
	~CSendReceive();
private:
	CSendReceive(CSmsTestUtils& aTestUtils);

	void TestSendingL();
	TBool SendNextMessageL();
	void TestWaitForReceiveL();
	void TestParseL();
	void DoRunTestParseL();

	virtual void ConstructL();

	void DoRunL();
	void RunL();
	void DoCancel();

	void DoRunSendingL();
	void DoRunWaitingToReceiveL();
	void DoWaitL();

	void WaitForExtraL();
	void DoRunWaitForExtraL();

//	void CreateWappMessagesL();
//	HBufC* ReadWappMessageFromFileL(const TDesC& aFile);
//	void CreateMessageL(const TDesC& aFile);

	CMsvSession& Session()	{return *iSmsTest.iMsvSession;}
	CMsvEntry& MsvEntry()	{return *iSmsTest.iMsvEntry;}
	CSmsClientMtm& Client()	{return *iSmsTest.iSmsClientMtm;}

	CMsvEntrySelection* iReceived;
	TInt iCountWaits;


	CMsvEntry* iEntryForObserver;
	TInt iCurrentMessageNum;

	CSmsTestUtils&		iSmsTest;
	CMsvEntrySelection*	iSelection;
	CMsvOperation*		iOperation;
	CTestTimer*			iTimer;

	TInt				iCurrentTest;

	CDir*				iDir;

	enum TSmsSendTestState
		{
		EStateWaiting,
		EStateSending,
		EStateWaitingToReceive,
		EStateMessageReceived,
		EStateWaitingToReceiveAnyExtra
		} iState;
	};
