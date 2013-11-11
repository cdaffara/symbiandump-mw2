// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __HARNESS_H
#define __HARNESS_H

#include <e32test.h>
#include <badesca.h>
#include <pop3set.h>
#include <smtpset.h>
#include <imapset.h>
#include <smutset.h>
#include "msvtestutils.h"
#include "testframeutils.h"
#include "biotestutils.h"

// forward declaration
class CEmailAccounts;

//
//
// MBaseTestState
//

class MBaseTestState
{
public:
	virtual void StartL(TRequestStatus& aStatus) = 0;
	virtual ~MBaseTestState() {};

	inline void SetDebugInfo(const TTestDebugInfo& aDebugInfo) { iDebugInfo = aDebugInfo; } 
	const TTestDebugInfo& GetDebugInfo() const {return iDebugInfo;};

protected:
	TTestDebugInfo iDebugInfo;
};

//
//
// CBaseTestHarness
//

class CBaseTestHarness : public CActive, public MBaseTestState
{
public:
	virtual ~CBaseTestHarness();
	virtual void StartL(TRequestStatus& aStatus);
	virtual void AddStateL(MBaseTestState* aTestState, TTestDebugInfo aDebugInfo);

	inline virtual void SetHeapFailureTesting(TBool aTest) { iTestHeapFailure = aTest; }
	inline virtual void LogCommentL(const TDesC& /*aComment*/) {};
	inline RTest& TestConsole() { return iTest; }

	inline void SetExpectedError(TInt aError) { iExpectedError = aError; }

	//
	// From CActive
	virtual void RunL();
	inline void DoCancel() {}
	TInt RunError(TInt aError);

protected:
	virtual void ConstructL();
	inline CBaseTestHarness(RTest& aTest, TBool aMarkTestStarts) : CActive(0), iOldCurrentState(-1), iTest(aTest), iMarkTestStarts(aMarkTestStarts)  {}

	//
	// Mark test start/end - default imp does nothing
	inline virtual void TestStart(TInt& /*aState*/) {}
	inline virtual void TestFinish(TInt& /*aState*/, TRequestStatus& /*aStatus*/) {}

	//
	// Called when an error occurs
	virtual void OnStateError();

	//
	// Used to log error
	TInt LogStateError();

protected:
	TInt iCurrentState;
	TInt iOldCurrentState;
	TRequestStatus* iReportStatus;
	RTest& iTest;
	CArrayPtrFlat<MBaseTestState>* iStateList;
	TBool iTestHeapFailure;
	TInt iExpectedError;

private:
	TInt iHeapFailureCount;
	TBool iMarkTestStarts;
};


//
//
// CMainTestHarness
//
class CBioTestUtils;
class CMainTestHarness : public CBaseTestHarness
{
public:
	~CMainTestHarness();
	static CMainTestHarness* NewL(RTest& aTest);
	inline CBioTestUtils& TestUtils() { return *iBioUtils; }

protected:
	virtual void ConstructL();
	inline CMainTestHarness(RTest& aTest) : CBaseTestHarness(aTest, ETrue) {}

	//
	// Test start/end marking
	virtual void TestStart(TInt& aState);
	virtual void TestFinish(TInt& aState, TRequestStatus& aStatus);

protected:
	CBioTestUtils* iBioUtils;
	TBool iAllTestsSuccessful;
};

//
//
// CClientTestHarness
//
class CClientTestHarness : public CBaseTestHarness
{
public:
	static CClientTestHarness* NewL(CBioTestUtils& aTestUtils, RTest& aTest);
	inline CBioTestUtils& TestUtils() { return iBioUtils; }
	virtual void LogCommentL(const TDesC& aComment);
	inline const TMsvId& CurrentBioMsg() { return iBioMessageData; }
	inline void SetCurrentBioMsg(const TMsvId& aBioMsg) { iBioMessageData = aBioMsg; }
	inline ~CClientTestHarness() {delete iPop3Settings;delete iSmtpSettings;delete iImapSettings;delete iSmsSettings;delete iSQL;}
	
	void UnInstantiatePop3SettingsL();
	void UnInstantiateSmtpSettingsL();
	void UnInstantiateImapSettingsL();
	void InstantiateNamedPop3SettingsL(const TDesC& aName);
	void InstantiateNamedSmtpSettingsL(const TDesC& aName);
	void InstantiateNamedImapSettingsL(const TDesC& aName);
	void InstantiatePop3SettingsL();
	void InstantiateSmtpSettingsL();
	void InstantiateImapSettingsL();
	void InstantiateSmsSettingsL();
	void InstantiateClientMtmL();
	void InstantiateServerMtmL();

	//
	// Customized error handeling
	virtual void OnStateError();

protected:
	void ConstructL();
	inline CClientTestHarness(CBioTestUtils& aTestUtils, RTest& aTest) : CBaseTestHarness(aTest, EFalse), iBioUtils(aTestUtils) {}
private:
	TMsvId GetImapServiceL(CEmailAccounts& aAccounts);
	TMsvId GetPopServiceL(CEmailAccounts& aAccounts);

protected:
	CBioTestUtils& iBioUtils;

public:
	CImPop3Settings* iPop3Settings;
	CImSmtpSettings* iSmtpSettings;
	CImImap4Settings* iImapSettings;
	CSmsSettings* iSmsSettings;
	HBufC*		iSQL;

	//
	// Stores the curent BIO message.  All operations that
	// check bio messages etc will be checking this.
	TMsvId iBioMessageData;
};
#endif
