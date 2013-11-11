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
#include <e32uid.h>
#include <e32std.h>
#include <smuthdr.h>
#include <smscmds.h>
#include "T_SmsEnum.h"
#include <txtrich.h>

void CSmsEnumTest::TestCopyFromSimL()
	{
	iSmsTest.Test().Next(_L("Copy From SIM"));
	iState = EStateCopyFromSim;

	MsvEntry().SetEntryL(iSmsTest.iProgress.iEnumerateFolder);

	delete iSelection;
	iSelection = MsvEntry().ChildrenL();
	iSelection->InsertL(0,iSmsTest.iSmsServiceId);

	TPckgBuf<TMsvId> pkg(KMsvGlobalInBoxIndexEntryId);

	delete iOperation;
	iOperation = Session().TransferCommandL(*iSelection, ESmsMtmCommandCopyFromPhoneStore, pkg, iStatus);
	SetActive();
	CActiveScheduler::Start();
	}

void CSmsEnumTest::TestMoveFromSimL()
	{
	iSmsTest.Test().Next(_L("Move From SIM"));
	iState = EStateMoveFromSim;

	MsvEntry().SetEntryL(iSmsTest.iProgress.iEnumerateFolder);

	delete iSelection;
	iSelection = MsvEntry().ChildrenL();
	iSelection->InsertL(0,iSmsTest.iSmsServiceId);

	TPckgBuf<TMsvId> pkg(KMsvGlobalInBoxIndexEntryId);

	delete iOperation;
	iOperation = Session().TransferCommandL(*iSelection, ESmsMtmCommandMoveFromPhoneStore, pkg, iStatus);
	SetActive();
	CActiveScheduler::Start();
	}

void CSmsEnumTest::TestDeleteFromSimL()
	{
	iSmsTest.Test().Next(_L("Delete From SIM"));
	iState = EStateDeleteFromSim;

	MsvEntry().SetEntryL(iSmsTest.iProgress.iEnumerateFolder);

	delete iSelection;
	iSelection = MsvEntry().ChildrenL();
	iSelection->InsertL(0,iSmsTest.iSmsServiceId);

	delete iOperation;
	iOperation = Session().TransferCommandL(*iSelection, ESmsMtmCommandDeleteFromPhoneStore, TPtrC8(), iStatus);
	SetActive();
	CActiveScheduler::Start();
	}

void CSmsEnumTest::TestCopyToSimL()
	{
	iSmsTest.Test().Next(_L("Copy to SIM"));
	iState = EStateCopyToSim;

	iSelection->Reset();

	TTime now;
	now.HomeTime();
	iSelection->Reset();
	iSmsTest.ReadScriptL(iScriptFile, KMsvGlobalOutBoxIndexEntryId, *iSelection, now);
	iSelection->InsertL(0,iSmsTest.iSmsServiceId);

	iSmsTest(iSelection->Count());

	delete iOperation;
	iOperation = Session().TransferCommandL(*iSelection, ESmsMtmCommandCopyToPhoneStore, TPtrC8(), iStatus);
	SetActive();
	CActiveScheduler::Start();
	}

void CSmsEnumTest::TestEnumerateL()
	{
	iSmsTest.Test().Next(_L("Enumerate SIM"));
	iState = EStateEnumerating;

	iSelection->Reset();
	iSelection->AppendL(iSmsTest.iSmsServiceId);

	delete iOperation;
	iOperation = Session().TransferCommandL(*iSelection, ESmsMtmCommandEnumeratePhoneStores, TPtrC8(), iStatus);
	SetActive();
	CActiveScheduler::Start();
	}

CSmsEnumTest::CSmsEnumTest(CSmsTestUtils& aSmsTest, const TDesC& aScriptFile, TInt& aNextTest)
: CSmsTestBase(aSmsTest, aScriptFile, aNextTest), iState(EStateWaiting)
	{
	}

void CSmsEnumTest::RunAutoL()
	{
	iSmsTest.TestStart(++iNextTest, _L("Copy to SIM"));
	TestCopyToSimL();
	User::LeaveIfError(iStatus.Int());
	iSmsTest.TestFinish(iNextTest, KErrNone);

	iSmsTest.TestStart(++iNextTest, _L("Enumerate"));
	TestEnumerateL();
	User::LeaveIfError(iStatus.Int());
	iSmsTest.TestFinish(iNextTest, KErrNone);

	if (iSelection->Count())
		{
		iSmsTest.TestStart(++iNextTest, _L("Delete Remote Entries"));
		TestDeleteRemoteEntryL();
		User::LeaveIfError(iStatus.Int());
		iSmsTest.TestFinish(iNextTest, KErrNone);
		}

	iSmsTest.TestStart(++iNextTest, _L("Enumerate"));
	TestEnumerateL();
	User::LeaveIfError(iStatus.Int());
	iSmsTest.TestFinish(iNextTest, KErrNone);

	if (iSelection->Count())
		{
		iSmsTest.TestStart(++iNextTest, _L("Move from SIM"));
		TestCopyFromSimL();
		User::LeaveIfError(iStatus.Int());
		iSmsTest.TestFinish(iNextTest, KErrNone);
		}

	iSmsTest.TestStart(++iNextTest, _L("Enumerate"));
	TestEnumerateL();
	User::LeaveIfError(iStatus.Int());
	iSmsTest.TestFinish(iNextTest, KErrNone);

	if (iSelection->Count())
		{
		iSmsTest.TestStart(++iNextTest, _L("Move from SIM"));
		TestMoveFromSimL();
		User::LeaveIfError(iStatus.Int());
		iSmsTest.TestFinish(iNextTest, KErrNone);
		}
	}

void CSmsEnumTest::ConstructL()
	{
	SetTestNameL(KSmsEnumTestName);
	iSmsTest.SetLogToFile();

	iSelection = new (ELeave) CMsvEntrySelection();
	iTimer = CTestTimer::NewL();
	CActiveScheduler::Add(this);
	}

void CSmsEnumTest::ShowMenuL()
	{
	iSmsTest.ResetMenu();

	iSmsTest.AppendToMenuL(_L("Enumerate SIM"));
	iSmsTest.AppendToMenuL(_L("Copy To SIM"));
	iSmsTest.AppendToMenuL(_L("Display Log"));
	iSmsTest.AppendToMenuL(_L("Clear Log"));

	TInt result = iSmsTest.DisplayMenu(_L("SMS Enumerate Sim Test"));

	if (result <= 0)
		return;

	switch (result)
		{
		case 1:
			TestEnumerateL();
			break;
		case 2:
			TestCopyToSimL();
			break;
		case 3:
			DisplayLogL();
			break;
		case 4:
			ClearLogL();
			break;
		default:
			User::Leave(KErrArgument);
			break;
		}

	ShowMenuL();
	}

CSmsEnumTest::~CSmsEnumTest()
	{
	}

CSmsEnumTest* CSmsEnumTest::NewLC(CSmsTestUtils& aSmsTest, const TDesC& aScriptFile, TInt& aNextTest)
	{
	CSmsEnumTest* self = new (ELeave) CSmsEnumTest(aSmsTest, aScriptFile, aNextTest);
	CleanupStack::PushL(self);

	self->ConstructL();
	return self;
	}

void CSmsEnumTest::RunL()
	{
	CActiveScheduler::Stop();

	iSmsTest(iStatus.Int() == KErrNone);

	iSmsTest.SetProgressL(*iOperation);

	switch (iState)
		{
		case EStateEnumerating:
			{
			DoRunEnumerateL();
			break;
			}
		case EStateCopyFromSim:
		case EStateMoveFromSim:
		case EStateDeleteFromSim:
			{
			DoRunCopyMoveDeleteFromSimL();
			break;
			}
		case EStateCopyToSim:
			{
			DoRunCopyToSimL();
			break;
			}
		default:
			{
			break;
			}
		}

	delete iOperation;
	iOperation = NULL;
	}

void CSmsEnumTest::DoRunCopyToSimL()
	{
	TInt err = iStatus.Int();

	if (err == KErrNone)
		{
		err = iSmsTest.iProgress.iError;
		}

	iSmsTest.Printf(_L("Copy To SIM completed with error %d\n"), err);

	iState = EStateWaiting;

	if (!iSmsTest.RunAuto())
		{
		iSmsTest.Printf(_L("\nPress any key to continue...\n"));
		iSmsTest.Test().Getch();
		}
	}

void CSmsEnumTest::DoRunCopyMoveDeleteFromSimL()
	{
	TInt err = iStatus.Int();

	if (err == KErrNone)
		{
		err = iSmsTest.iProgress.iError;
		}

	switch (iState)
		{
		case EStateCopyFromSim:
			iSmsTest.Printf(_L("Copy From SIM completed with error %d\n"), err);
			break;
		case EStateMoveFromSim:
			iSmsTest.Printf(_L("Move From SIM completed with error %d\n"), err);
			break;
		case EStateDeleteFromSim:
			iSmsTest.Printf(_L("Delete From SIM completed with error %d\n"), err);
			break;
		default:
			break;
		}

	iState = EStateWaiting;

	if (!iSmsTest.RunAuto())
		{
		iSmsTest.Printf(_L("\nPress any key to continue...\n"));
		iSmsTest.Test().Getch();
		}
	}

void CSmsEnumTest::DoRunEnumerateL()
	{
	TInt err = iStatus.Int();

	if (err == KErrNone)
		{
		err = iSmsTest.iProgress.iError;
		iStatus = err;

		if (err == KErrNone)
			{
			MsvEntry().SetEntryL(iSmsTest.iProgress.iEnumerateFolder);
			delete iSelection;
			iSelection = MsvEntry().ChildrenL();

			iSmsTest.Printf(_L("%d message(s) on SIM. %d message(s) now contained in folder %d.\n"), iSmsTest.iProgress.iMsgCount, iSelection->Count(), iSmsTest.iProgress.iEnumerateFolder);
			iSmsTest.DisplayMessagesL(*iSelection);
			}
		}

	iSmsTest.Printf(_L("Enumerate SIM completed with %d\n"), err);
	iState = EStateWaiting;

	delete iOperation;
	iOperation = NULL;

	if (!iSmsTest.RunAuto())
		{
		iSmsTest.Printf(_L("\nPress any key to continue...\n"));
		iSmsTest.Test().Getch();

		if  (iSmsTest.iProgress.iMsgCount)
			ShowMenuAfterEnumerateL();
		}
	}

void CSmsEnumTest::ShowMenuAfterEnumerateL()
	{
	iSmsTest.ResetMenu();

	iSmsTest.AppendToMenuL(_L("Copy From Sim To Inbox"));
	iSmsTest.AppendToMenuL(_L("Move From Sim To Inbox"));
	iSmsTest.AppendToMenuL(_L("Delete From Sim"));

	TInt result = iSmsTest.DisplayMenu(_L("What to do with enumerated messages?"));

	if (result <= 0)
		return;

	switch (result)
		{
		case 1:
			TestCopyFromSimL();
			break;
		case 2:
			TestMoveFromSimL();
			break;
		case 3:
			TestDeleteFromSimL();
			break;
		default:
			User::Leave(KErrArgument);
			break;
		}
	}

void CSmsEnumTest::DoCancel()
	{
	if (iOperation)
		{
		iOperation->Cancel();
		iSmsTest.Printf(_L("Operation Cancelled!\n"));
		}
	else
		{
		iSmsTest.Printf(_L("No operation to cancel!\n"));
		}

	delete iOperation;
	iOperation = NULL;
	}

void CSmsEnumTest::TestDeleteRemoteEntryL()
	{
	iSmsTest.Test().Next(_L("Test Delete Remote Entry\n"));

	CMsvOperationWait* wait = CMsvOperationWait::NewLC();
	wait->Start();

	iSmsTest.SetEntryL(iSmsTest.iProgress.iEnumerateFolder);
	iSmsTest.SetEntryL(iSmsTest.Entry().Parent());

	CMsvOperation* op = iSmsTest.iMsvEntry->DeleteL(iSmsTest.iProgress.iEnumerateFolder, wait->iStatus);
	CleanupStack::PushL(op);

	CActiveScheduler::Start();

	iSmsTest.Printf(_L("Remote DeleteL() completed with %d\n"), wait->iStatus.Int());

	User::LeaveIfError(wait->iStatus.Int());

	iSmsTest.SetProgressL(*op);

	iSmsTest.Printf(_L("Operation completed with %d, msg count %d\n"), iSmsTest.iProgress.iError, iSmsTest.iProgress.iMsgCount);
	User::LeaveIfError(iSmsTest.iProgress.iError);

	CleanupStack::PopAndDestroy(2); //wait, op
	}
