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

#ifndef SMS_TEST_BASE_H_
#define SMS_TEST_BASE_H_

#include "smstestutils.h"

_LIT(KDefaultSmsTestName, "SMS Test Harness");

class CSmsTestBase : public CActive
	{
public:
	//StartL() calls RunAutoL if iSmsTest.RunAuto() is True,
	//otherwise calls ShowMenuL().
	//Traps any errors.
	//Calls iSmsTest.TestHarnessFailed() if there is an error,
	//otherwise calls iSmsTest.TestHarnessComplete().
	IMPORT_C virtual void StartL();
	IMPORT_C ~CSmsTestBase();

	inline TInt Error() {return iError;}
	inline TBool Complete() {return iComplete;}

	inline void SetTestNameL(const TDesC& aTestName);
	inline const TDesC& TestName() const;
	inline CSmsTestUtils& SmsTest() const {return iSmsTest;}

protected:

	IMPORT_C CSmsTestBase(CSmsTestUtils& aSmsTest, const TDesC& aScriptFile, TInt& aCurrentTest, TInt aPriority = EPriorityStandard);

	virtual void ShowMenuL() = 0;
	virtual void RunAutoL() = 0;

	IMPORT_C void DisplayLogL();
	IMPORT_C void ClearLogL();

	IMPORT_C TInt RunError(TInt aError);

	CMsvSession& Session()	{return *iSmsTest.iMsvSession;}
	CMsvEntry& MsvEntry()	{return *iSmsTest.iMsvEntry;}
	CSmsClientMtm& Client()	{return *iSmsTest.iSmsClientMtm;}

	IMPORT_C virtual void DoTestSendMesssageL(TMsvId aMsgId);
	IMPORT_C virtual void DoTestForwardMsgL(TMsvId aId);
	IMPORT_C virtual void DoTestSendingL(const TDesC& aScriptFile, TBool aMove = EFalse);
	IMPORT_C virtual void DoTestSchedulingL(const TDesC& aScriptFile, TBool aMove = EFalse, TTimeIntervalSeconds aFromNow = 5);

	inline TBool CompareBools(TBool aLeft, TBool aRight) {return (!aLeft && !aRight) || (aLeft && aRight);}

protected:

	CMsvOperation* iOperation;
	CMsvEntrySelection* iSelection;
	CTestTimer* iTimer;
	HBufC* iTestName;

	TBool iComplete;
	TInt iError;
	TFileName iScriptFile;
	CSmsTestUtils& iSmsTest;
	TInt& iNextTest;
	};

#define SMSTEST_PANIC _L("Sms_Test")

inline void CSmsTestBase::SetTestNameL(const TDesC& aTestName)
	{
	delete iTestName;
	iTestName = NULL;
	iTestName = aTestName.AllocL();
	}

inline const TDesC& CSmsTestBase::TestName() const
	{
	if (iTestName)
		return *iTestName;
	else
		return KDefaultSmsTestName;
	}

#endif
