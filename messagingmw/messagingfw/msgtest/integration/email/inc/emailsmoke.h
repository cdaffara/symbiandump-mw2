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

#include <e32test.h>
#include <csdprog.h>
#include "emailtestutils.h"
#include "testframeutils.h"

#ifndef EMAILSMOKE_H
#define EMAILSMOKE_H


#define KInvalidState	-99  //Invalid progress state
#define KInvalidIAP	-99 
//
//
// MBaseTestState
//

class MBaseTestState
	{
public:
	virtual void StartL(TRequestStatus& aStatus) = 0;
	virtual ~MBaseTestState() {};

	void SetDebugInfo(const TTestDebugInfo& aDebugInfo) {iDebugInfo = aDebugInfo;};
	const TTestDebugInfo& GetDebugInfo() const {return iDebugInfo;};

protected:
	TTestDebugInfo iDebugInfo;
	};

class TTestDebugInfo;

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

	virtual void LogComment(const TDesC& aError);
	
	// From CActive
	void RunL();
	void DoCancel();
	TInt RunError(TInt aError);
	RTest& TestConsole();

protected:
	virtual void ConstructL();
	CBaseTestHarness(RTest& aTest);

	TInt iCurrentState;
	TRequestStatus* iReportStatus;

	RTest& iTest;
	CArrayPtrFlat<MBaseTestState>* iStateList;

private:
//	CTestScript& iScript;
	};


//
//
// CMainTestHarness
//

class CMainTestHarness : public CBaseTestHarness
	{
public:
	static CMainTestHarness* NewL(RTest& aTest);
	virtual void StartL(TRequestStatus& aStatus);

protected:
	virtual void ConstructL();
	CMainTestHarness(RTest& aTest);
	};


//
//
// CMsvClientTest
//

class CMsvClientTest : public CBaseTestHarness
	{
public:
	static CMsvClientTest* NewL(CEmailTestUtils& aTestUtils, RTest& aTest);
	static CMsvClientTest* NewL(CEmailTestUtils* aTestUtils, RTest& aTest);
	virtual ~CMsvClientTest();
	CEmailTestUtils& MsvTestUtils();
	void DoCancel();
	void SetCurrentOperation(CMsvOperation* aOperation);
	CMsvOperation& CurrentOperation();

protected:
	virtual void ConstructL();
	CMsvClientTest(CEmailTestUtils& aMsvTestUtils, RTest& aTest);
	CMsvClientTest(CEmailTestUtils* aMsvTestUtils, RTest& aTest);

public:
	CMsvEntrySelection* iCurrentSelection;
	TMsvId iSelectedFolder;
	TMsvId iDestinationFolder;
	TMsvId iCurrentServiceId;

protected:
	CEmailTestUtils& iMsvTestUtils;

private:
	CMsvTestUtils* iOwnedMsvTestUtils;
	CMsvOperation* iOperation;
	};



//
//
// CEmailClientTest
//

class CEmailClientTest : public CMsvClientTest
	{
public:
	static CEmailClientTest* NewL(CEmailTestUtils& aTestUtils, RTest& aTest);
	static CEmailClientTest* NewL(CEmailTestUtils* aTestUtils, RTest& aTest);
	virtual ~CEmailClientTest();

	void LogComment(const TDesC& aError);

	CEmailTestUtils& EmailTestUtils();
protected:
	virtual void ConstructL();
	CEmailClientTest(CEmailTestUtils& aTestUtils, RTest& aTest);
	CEmailClientTest(CEmailTestUtils* aTestUtils, RTest& aTest);

protected:
	CEmailTestUtils& iEmailTestUtils;

private:
	CEmailTestUtils* iOwnedEmailTestUtils;
	};

#endif
