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
#include <logcli.h>
#include <smscmds.h>
#include <txtrich.h> 
#include <smsclnt.h>

EXPORT_C CSmsTestBase::~CSmsTestBase()
	{
	if (iOperation)
		{
		iOperation->Cancel();
		delete iOperation;
		iOperation = NULL;
		}

	delete iSelection;
	iSelection = NULL;

	if (iTimer)
		{
		iTimer->Cancel();
		delete iTimer;
		iTimer = NULL;
		}

	delete iTestName;
	iTestName = NULL;
	}

EXPORT_C CSmsTestBase::CSmsTestBase(CSmsTestUtils& aSmsTest, const TDesC& aScriptFile, TInt& aNextTest, TInt aPriority)
: CActive(aPriority), iComplete(EFalse), iError(KErrNone), iScriptFile(aScriptFile), iSmsTest(aSmsTest), iNextTest(aNextTest)
	{
	}

EXPORT_C void CSmsTestBase::DisplayLogL()
	{
	CLogFilter* filter = CLogFilter::NewL();
	CleanupStack::PushL(filter);

	iSmsTest.DisplayLogL(*filter, ETrue);

	CleanupStack::PopAndDestroy(); //filter

	if (!iSmsTest.RunAuto())
		{
		iSmsTest.Test().Printf(_L("\nPress any key to continue...\n"));
		iSmsTest.Test().Getch();
		}
	}

EXPORT_C void CSmsTestBase::ClearLogL()
	{
	CLogFilter* filter = CLogFilter::NewL();
	CleanupStack::PushL(filter);

	iSmsTest.ClearLogL(*filter);

	CleanupStack::PopAndDestroy(); //filter

	if (!iSmsTest.RunAuto())
		{
		iSmsTest.Test().Printf(_L("\nPress any key to continue...\n"));
		iSmsTest.Test().Getch();
		}
	}

EXPORT_C TInt CSmsTestBase::RunError(TInt aError)
	{
	if (iStatus == KErrNone)
		iStatus = aError;

	return KErrNone;
	}

EXPORT_C void CSmsTestBase::DoTestForwardMsgL(TMsvId aId)
// Test forwarding this message (ie Create a forwarded SMS, 
// place it in the drafts folder & then send it).
	{
	// Load the SMS Client MTM if not already done so
	if (! iSmsTest.iSmsClientMtm)
		iSmsTest.InstantiateClientMtmsL();

	// Get the SMS Client MTM to use & load this message
	Client().SwitchCurrentEntryL(aId);
	Client().LoadMessageL();

	// Forward the message & store it in the Drafts folder
	TMsvPartList part=0;
	TRequestStatus status(0);
	CMsvOperation* op = Client().ForwardL(KMsvDraftEntryId, part, status);
	CleanupStack::PushL(op);

	TMsvId temp;	
	TPckgC<TMsvId> paramPack(temp);
	const TDesC8& progBuf2 = op->ProgressL();	
	paramPack.Set(progBuf2);
	TMsvId progress=paramPack();
	
	CleanupStack::PopAndDestroy(); //op
	op = NULL;

	Client().SwitchCurrentEntryL(progress);
	Client().LoadMessageL();

	// Add additional text & an Addressee to the Forwarded Message
	Client().Body().InsertL(0,_L("This is a forwarded Msg. "));
	Client().AddAddresseeL(KScriptItemRecipientsDef);

	// Change the description of the new message
	CMsvEntry& cEntry = Client().Entry();
	TMsvEntry tEntry = cEntry.Entry();
	tEntry.iDescription.Set(_L("Forwarded Msg"));
	cEntry.ChangeL(tEntry);

	// Get the ID of the new Forwarded Message
	TMsvId newEntryId = cEntry.EntryId();

	// Save the new message
	Client().SaveMessageL();

	// Now send the Forwarded Message that we have just created
	DoTestSendMesssageL(newEntryId);
	}


EXPORT_C void CSmsTestBase::DoTestSendMesssageL(TMsvId aMsgId)
// Test Sending this message  
	{
	iSmsTest.Test().Next(_L("Sending"));
	iSmsTest.Printf(_L("Sending %d messages...\n"), 1);
	iSmsTest.DisplayMessageL(aMsgId);

	// Get the Parent Id of the message we want to send & 
	// then set the entry to this parent
	iSmsTest.SetEntryL(aMsgId);
	TMsvEntry entryToSend = iSmsTest.Entry();
	TMsvId parentId = entryToSend.Parent();
	iSmsTest.SetEntryL(parentId);

	delete iOperation;
	iOperation = NULL;

	// Send the message, by copying it to the SMS Service.
	iOperation = MsvEntry().CopyL(aMsgId, iSmsTest.iSmsServiceId, iStatus);

	SetActive();
	}

EXPORT_C void CSmsTestBase::DoTestSendingL(const TDesC& aScript, TBool aMove)
//
//
//
	{
	iSmsTest.Test().Next(_L("Sending"));

	TTime now;
	now.HomeTime();
	now += (TTimeIntervalSeconds) 5;

	iSmsTest.DeleteSmsMessagesL(KMsvGlobalOutBoxIndexEntryId);
	iSelection->Reset();

	// Create the test messages specified in the script file and place in the Outbox.
	iSmsTest.Printf(_L("Creating msgs in outbox from script %S\n"), &aScript);
	TBool read = EFalse;
	TRAPD(err, read = iSmsTest.ReadScriptL(aScript, KMsvGlobalOutBoxIndexEntryId, *iSelection, now));

	iSmsTest.Test()(!err && read);

	iSmsTest.Printf(_L("Sending %d messages...\n"), iSelection->Count());
	iSmsTest.DisplayMessagesL(*iSelection);
	iSmsTest.SetEntryL(KMsvGlobalOutBoxIndexEntryId);

	delete iOperation;

	// Send the message, by copying it to the SMS Service.
	if (aMove)
		{
		iOperation = MsvEntry().MoveL(*iSelection, iSmsTest.iSmsServiceId, iStatus);
		}
	else
		{
		iOperation = MsvEntry().CopyL(*iSelection, iSmsTest.iSmsServiceId, iStatus);
		}

	SetActive();
	}

EXPORT_C void CSmsTestBase::DoTestSchedulingL(const TDesC& aScript, TBool aMove, TTimeIntervalSeconds aFromNow)
	{
	iSmsTest.Test().Next(_L("Scheduling"));
	iSmsTest.DeleteSmsMessagesL(KMsvGlobalOutBoxIndexEntryId);
	iSelection->Reset();

	TTime now;
	now.HomeTime();
	now += aFromNow;

	iSmsTest.Printf(_L("Creating msgs in outbox from script %S\n"), &aScript);
	TBool read = EFalse;
	TRAPD(err, read = iSmsTest.ReadScriptL(aScript, KMsvGlobalOutBoxIndexEntryId, *iSelection, now));

	iSmsTest.Test()(!err && read);

	iSmsTest.Printf(_L("Scheduled %d messages...\n"), iSelection->Count());
	iSmsTest.DisplayMessagesL(*iSelection);
	MsvEntry().SetEntryL(KMsvGlobalOutBoxIndexEntryId);

	delete iOperation;

	TInt cmd = aMove ? ESmsMtmCommandScheduleMove : ESmsMtmCommandScheduleCopy;

	iOperation = Session().TransferCommandL(*iSelection, cmd, TPtrC8(), iStatus);
	SetActive();
	}

EXPORT_C void CSmsTestBase::StartL()
	{
	TRAP(iError, iSmsTest.RunAuto() ? RunAutoL() : ShowMenuL());

	if (iError)
		{
		if (iNextTest)
			{
			iSmsTest.TestFinish(iNextTest, iError);
			}

		iSmsTest.TestHarnessFailed(iError);
		}
	else
		{
		iSmsTest.TestHarnessCompleted();
		iComplete = ETrue;
		}
	}
