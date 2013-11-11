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

#include <smsclnt.h>
#include <smuthdr.h>
#include "smstestbase.h"

_LIT(KSmsEnumTestName, "Enumerate and Write to SIM");

class CSmsEnumTest : public CSmsTestBase
	{
public:
	static CSmsEnumTest* NewLC(CSmsTestUtils& aSmsTest, const TDesC& aScriptFile, TInt& aNextTest);
	~CSmsEnumTest();

private:
	CSmsEnumTest(CSmsTestUtils& aSmsTest, const TDesC& aScriptFile, TInt& aNextTest);
	void ConstructL();

	void RunL();
	void DoCancel();

	void TestEnumerateL();
	void TestCopyFromSimL();
	void TestMoveFromSimL();
	void TestDeleteFromSimL();

	void TestCopyToSimL();
	void TestDeleteRemoteEntryL();

	void ShowMenuL();
	void RunAutoL();

	void DoRunEnumerateL();
	void DoRunCopyToSimL();
	void DoRunCopyMoveDeleteFromSimL();

	void ShowMenuAfterEnumerateL();

private:
	enum TSchSmsTestState
		{
		EStateWaiting,
		EStateEnumerating,
		EStateCopyFromSim,
		EStateMoveFromSim,
		EStateDeleteFromSim,
		EStateCopyToSim
		} iState;
	};
