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
// Pop3Tests.h
// 
//

#ifndef __POP3TESTS_H
#define __POP3TESTS_H

#include "emailsmoke.h"
#include "emailsmokecommands.h"
#include <cemailaccounts.h>

//
//
// CPop3ClientTest
//

class CPop3ClientTest : public CEmailClientTest
	{
public:
	static CPop3ClientTest* NewL(CEmailTestUtils& aTestUtils, RTest& aTest);
	virtual ~CPop3ClientTest();

protected:
	virtual void ConstructL();
	CPop3ClientTest(CEmailTestUtils& aTestUtils, RTest& aTest);
	};


//
//
// CCreatePop3ClientMtm
//

class CCreatePop3ClientMtm : public CBase, public MBaseTestState
	{
public:
	void StartL(TRequestStatus& aStatus);
	CCreatePop3ClientMtm(CEmailClientTest& aTest);

private:
	CEmailClientTest& iTest;
	};




//
//
// CCreatePop3Service
//

class CCreatePop3Service : public CCreateEmailService
	{
public:
	static CCreatePop3Service* NewL(CEmailClientTest& aTestHarness);
	void StartL(TRequestStatus& aStatus);
	~CCreatePop3Service();
	CImPop3Settings& Settings();

private:
	void ConstructL();
	CCreatePop3Service(CEmailClientTest& aTestHarness);

private:
	CEmailClientTest& iTestHarness;
	CEmailAccounts* iAccount;
	CImPop3Settings* iSettings;
	CImIAPPreferences* iPopIAP;
	};


//
//
// CPop3Connect
//

class CPop3Connect : public CActive, public MBaseTestState
	{
public:
	CPop3Connect(CEmailClientTest& aParentTestHarness);
	void StartL(TRequestStatus& aStatus);
	void RunL();
	void DoCancel();
	~CPop3Connect();

private:
	CEmailClientTest& iParentTestHarness;
	TRequestStatus* iReportStatus;
	};


//
//
// CPop3ConnectAndMonitor
//

class CPop3ConnectAndMonitor : public CActive, public MBaseTestState
	{
public:
	CPop3ConnectAndMonitor(CEmailClientTest& aParentTestHarness, TBool aSynchronise = EFalse);
	void StartL(TRequestStatus& aStatus);
	void RunL();
	void DoCancel();
	~CPop3ConnectAndMonitor();

private:
	void LogCommentFormat(TRefByValue<const TDesC> format,...);

private:
	CEmailClientTest& iParentTestHarness;
	TRequestStatus* iReportStatus;
	TBool iSynchronise;
	TBool iConnectionError;
	};
//
//
// CPopDisconnect
//

class CPop3Disconnect : public CActive, public MBaseTestState
	{
public:
	CPop3Disconnect(CEmailClientTest& aParentTestHarness);
	void StartL(TRequestStatus& aStatus);
	void RunL();
	void DoCancel();
	~CPop3Disconnect();

private:
	CEmailClientTest& iParentTestHarness;
	TRequestStatus* iReportStatus;
	};



//
//
// CCheckPop3Operation
//

class CCheckPop3Operation : public CBase, public MBaseTestState
	{
public:
	CCheckPop3Operation(TInt aExpectedStatus, CPop3ClientTest& aTestHarness);
	void StartL(TRequestStatus& aStatus);

private:
	CPop3ClientTest& iTestHarness;
	TInt iExpectedStatus;
	};


//
//
// CPop3UseService
//

class CPop3UseService : public CBase, public MBaseTestState
	{
public:
	CPop3UseService(CPop3ClientTest& aTestHarness);
	void StartL(TRequestStatus& aStatus);

private:
	CPop3ClientTest& iTestHarness;
	};



//
//
// CPop3SetSyncLimits
//

class CPop3SetSyncLimits : public CBase, public MBaseTestState
	{
	public:
		CPop3SetSyncLimits(TInt aInboxSyncLimit, CPop3ClientTest& aTestHarness);
		void StartL(TRequestStatus& aStatus);

	private:
		void LogCommentFormat(TRefByValue<const TDesC> format,...);

	private:
		CPop3ClientTest&    iTestHarness;
		TInt				iInboxSyncLimit;
	};

#endif
