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

//#define NO_SIM_PARAMS_TEST
_LIT(KSmsScTestName, "Read and Write Service Centre");


class CSmsScTest : public CSmsTestBase
	{
public:
	static CSmsScTest* NewLC(CSmsTestUtils& aSmsTest, const TDesC& aScriptFile, TInt& aNextTest);
	~CSmsScTest();

private:
	CSmsScTest(CSmsTestUtils& aSmsTest, const TDesC& aScriptFile, TInt& aNextTest);

#ifndef NO_SIM_PARAMS_TEST
	void TestReadSimParamsL();
	void DoRunReadSimParamsL();
#endif

	void ConstructL();

	void RunL();
	void DoCancel();

	void ShowMenuL();
	void RunAutoL();

private:
	enum TSmsSendTestState
		{
		EStateWaiting,
#ifndef NO_SIM_PARAMS_TEST
		EStateReadingSimParams
#endif
		} iState;
	};
