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

#ifndef __SMTPTESTS_H
#define __SMTPTESTS_H

#include "emailsmoke.h"
#include "emailsmokecommands.h"

//
//
// CSmtpClientTest
//

class CSmtpClientTest : public CEmailClientTest
	{
public:
	static CSmtpClientTest* NewL(CEmailTestUtils& aTestUtils, RTest& aTest);
	virtual ~CSmtpClientTest();

protected:
	virtual void ConstructL();
	CSmtpClientTest(CEmailTestUtils& aTestUtils, RTest& aTest);
	};


//
//
// CCreateSmtpClientMtm
//

class CCreateSmtpClientMtm : public CBase, public MBaseTestState
	{
public:
	void StartL(TRequestStatus& aStatus);
	CCreateSmtpClientMtm(CEmailClientTest& aTest);

private:
	CEmailClientTest& iTest;
	};




//
//
// CCreateSmtpService
//

class CCreateSmtpService : public CCreateEmailService
	{
public:
	static CCreateSmtpService* NewL(CEmailClientTest& aTestHarness);
	void StartL(TRequestStatus& aStatus);
	~CCreateSmtpService();
	CImSmtpSettings& Settings();

private:
	void ConstructL();
	CCreateSmtpService(CEmailClientTest& aTestHarness);

private:
	CEmailClientTest& iTestHarness;
	CImSmtpSettings* iSettings;
	};


//
//
// CCheckSmtpOperation
//

class CCheckSmtpOperation : public CBase, public MBaseTestState
	{
public:
	CCheckSmtpOperation(TInt aExpectedStatus, CSmtpClientTest& aTestHarness);
	void StartL(TRequestStatus& aStatus);

private:
	CSmtpClientTest& iTestHarness;
	TInt iExpectedStatus;
	};


//
//
// CSmtpUseService
//

class CSmtpUseService : public CBase, public MBaseTestState
	{
public:
	CSmtpUseService(CSmtpClientTest& aTestHarness);
	void StartL(TRequestStatus& aStatus);

private:
	CSmtpClientTest& iTestHarness;
	};


//
//
// CImapConnectAndMonitor
//

class CCopySelectionAndMonitor : public CActive, public MBaseTestState
	{
public:
	CCopySelectionAndMonitor(CEmailClientTest& aParentTestHarness, TBool aSynchronise = EFalse);
	void StartL(TRequestStatus& aStatus);
	void RunL();
	void DoCancel();
	~CCopySelectionAndMonitor();

private:
	void LogCommentFormat(TRefByValue<const TDesC> format,...);

private:
	CEmailClientTest& iParentTestHarness;
	TRequestStatus* iReportStatus;
	TBool iSynchronise;
	TBool iConnectionError;
	};



#endif
