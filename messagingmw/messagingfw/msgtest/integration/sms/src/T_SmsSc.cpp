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

#include "T_SmsSc.h"
#include <smscmds.h>
#include <smutsimparam.h>
#include <smsclnt.h>

#include <mmlist.h>

//#ifndef NO_SIM_PARAMS_TEST
//#include <etelagsm.h>
//#endif

CSmsScTest::~CSmsScTest()
	{
	}

#ifndef NO_SIM_PARAMS_TEST
void CSmsScTest::TestReadSimParamsL()
	{
	iSmsTest.Test().Next(_L("Read Service Centres from Sim Params"));
	iState = EStateReadingSimParams;
	delete iOperation;
	iOperation = Client().ReadSimParamsL(iStatus);
	SetActive();
	CActiveScheduler::Start();
	}
#endif

CSmsScTest* CSmsScTest::NewLC(CSmsTestUtils& aSmsTest, const TDesC& aScriptFile, TInt& aNextTest)
	{
	CSmsScTest* self = new (ELeave) CSmsScTest(aSmsTest, aScriptFile, aNextTest);
	CleanupStack::PushL(self);

	self->ConstructL();
	return self;
	}



CSmsScTest::CSmsScTest(CSmsTestUtils& aSmsTest, const TDesC& aScriptFile, TInt& aNextTest)
: CSmsTestBase(aSmsTest, aScriptFile, aNextTest), iState(EStateWaiting)
	{
	}

void CSmsScTest::RunAutoL()
	{
#ifndef NO_SIM_PARAMS_TEST
	iSmsTest.TestStart(++iNextTest, _L("Read Service Centres from Sim Params"));
	TestReadSimParamsL();
	User::LeaveIfError(iStatus.Int());
	iSmsTest.TestFinish(iNextTest, KErrNone);
#endif
	}

void CSmsScTest::ConstructL()
	{
	SetTestNameL(KSmsScTestName);
	iSmsTest.InstantiateClientMtmsL();

	iSmsTest.SetLogToFile();

	iSelection = new (ELeave) CMsvEntrySelection();
	iTimer = CTestTimer::NewL();
	CActiveScheduler::Add(this);
	}

void CSmsScTest::ShowMenuL()
	{
	iSmsTest.ResetMenu();

	iSmsTest.AppendToMenuL(_L("Read Sim Params"));
	iSmsTest.AppendToMenuL(_L("Run Auto"));

	TInt result = iSmsTest.DisplayMenu(_L("SMS Service Centre Test"));

	if (result <= 0)
		return;

	switch (result)
		{
#ifndef NO_SIM_PARAMS_TEST
		case 1:
			TestReadSimParamsL();
			break;
		case 2:
#else
		case 1:
#endif
			iSmsTest.SetRunAuto(ETrue);
			RunAutoL();
			break;
		default:
			User::Leave(KErrArgument);
			break;
		}

	ShowMenuL();
	}

void CSmsScTest::RunL()
	{
	CActiveScheduler::Stop();

	switch (iState)
		{
#ifndef NO_SIM_PARAMS_TEST
		case EStateReadingSimParams:
			{
			DoRunReadSimParamsL();
			break;
			}
#endif
		default:
			{
			break;
			}
		}
	}

#ifndef NO_SIM_PARAMS_TEST
void CSmsScTest::DoRunReadSimParamsL()
	{
	if (iStatus == KErrNone)
		{
		TIntBuf progressBuf;
		progressBuf.Copy(iOperation->ProgressL());
		iStatus = progressBuf();
		}

	iSmsTest.Printf(_L("ReadSimParams completed with error %d\n"), iStatus);
	iSmsTest(iStatus == KErrNone || iStatus == KErrNotFound || iStatus == KErrNotSupported);

	if (iStatus != KErrNone)
		iSmsTest.Printf(_L("Error %d expected\n"), iStatus);

	if (iStatus == KErrNone)
		{
		CSmsSimParamOperation* op = (CSmsSimParamOperation*) iOperation;

		TInt count = 0;

		count = op->SmspList().Enumerate();

		iSmsTest.Printf(_L("ReadSimParams found:\n\t%d SMS params on SIM\n"), count);
		iSmsTest(count);

		CMobilePhoneSmspList* sc = op->ServiceCentersLC();

		count = sc->Enumerate();

		iSmsTest.Printf(_L("\t%d Service Centre(s)\n"), count);

		while (count--)
			{
			RMobileSmsMessaging::TMobileSmspEntryV1 smspEntry;
			smspEntry = sc->GetEntryL(count);
			iSmsTest.Printf(_L("\t\tService Centre \"%S\", Name \"%S\"\n"), &smspEntry.iServiceCentre.iTelNumber, &smspEntry.iText);
			}

		CleanupStack::PopAndDestroy(); //sc
		}

	iStatus = KErrNone;

	delete iOperation;
	iOperation = NULL;

	if (!iSmsTest.RunAuto())
		{
		iSmsTest.Printf(_L("\nPress any key to continue...\n"));
		iSmsTest.Test().Getch();
		}
	}
#endif

void CSmsScTest::DoCancel()
	{
	if (iOperation)
		{
		iOperation->Cancel();
		iSmsTest.Printf(_L("Operation Cancelled!\n"));
		}
	delete iOperation;
	iOperation = NULL;
	}
