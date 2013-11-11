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

#include <smsuaddr.h>

class CEntryObserver : public CBase, public MMsvEntryObserver
	{
public:
	void HandleEntryEventL(TMsvEntryEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3);
	~CEntryObserver();
public:
	CEntryObserver(CSmsTestUtils& aTestUtils, CMsvEntrySelection& aReceived, const TInt& aCurrentMessageNum);
	CMsvEntrySelection* iNew;
	CSmsTestUtils& iTestUtils;
	CMsvEntrySelection& iReceived;
	const TInt& iCurrentMessageNum;
	};

class CSendReceive;
class CParserTest;
class CTestIap;
class CTestWap;
class CWatcher;
class CWatcherWait;
class CBioSmokeTest : public CBase
	{
public:
	static CBioSmokeTest* NewL(RTest& aTest);
	void StartL();

private:
	CBioSmokeTest();
	~CBioSmokeTest();

	void ConstructL(RTest& aTest);

	void StartWatcherL();
	void CancelWatcher();

	CMsvSession& Session()	{return *iSmsTestUtils->iMsvSession;}
	CMsvEntry& MsvEntry()	{return *iSmsTestUtils->iMsvEntry;}
	CSmsClientMtm& Client()	{return *iSmsTestUtils->iSmsClientMtm;}

private:

	CSmsTestUtils*		iSmsTestUtils;

	CSendReceive*		iSendReceive;
	CParserTest*		iParserTest;

	CEntryObserver*		iEntryObserver;

	CMsvEntrySelection* iReceived;
	TInt				iCountWaits;

	CTestUtilsWatcherStarter* iWatcher;
	CMsvEntry*			iEntryForObserver;
	TInt				iCurrentMessageNum;

	CTestTimer*			iTimer;

	enum TBioSmokeState
		{
		EBioSmokeSendRecv,
		EBioSmokeParseProcess,
		EBioSmokeTestIap,
		EBioSmokeTestWap
		} iBioSmokeState;

	RSocketServ iSocketServ;	//< A Socket Server
	RSocket iSocket;			//< A Socket
	};
