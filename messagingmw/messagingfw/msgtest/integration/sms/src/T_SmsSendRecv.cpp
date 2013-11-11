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

#include "T_SmsSendRecv.h"
#include <watcher.h>
#include <smuthdr.h>
#include <txtrich.h>
#include <bioscmds.h>
#include "biosmtm.h"
#include <e32std.h>

CEntryObserver* CEntryObserver::NewL(CSmsTestUtils& aTestUtils, const TInt& aCurrentMessageNum)
	{
	CEntryObserver* self = new (ELeave) CEntryObserver(aTestUtils, aCurrentMessageNum);
	CleanupStack::PushL(self);
	self->iReceived = new (ELeave) CMsvEntrySelection();
	CleanupStack::Pop(self);
	return self;
	}

CEntryObserver::CEntryObserver(CSmsTestUtils& aTestUtils, const TInt& aCurrentMessageNum)
: CBase(), iCurrentMessageNum(aCurrentMessageNum), iSmsTest(aTestUtils)
	{
	}


CEntryObserver::~CEntryObserver() 
	{
	delete iReceived;
	}

void CEntryObserver::HandleEntryEventL(TMsvEntryEvent aEvent, TAny* aArg1, TAny* /*aArg2*/, TAny* /*aArg3*/)
	{
	if (aEvent == EMsvNewChildren && iCurrentMessageNum > -1)
		{
		CMsvEntrySelection* msgs = (CMsvEntrySelection*)aArg1;

		TInt count = msgs->Count();

		TInt msgRecv = 0;

		while (count--)
			{
			TMsvId id = (*msgs)[count];

			CSmsHeader* header = iSmsTest.GetHeaderLC(id);

			if (header->Type() == CSmsPDU::ESmsDeliver)
				{
				iReceived->AppendL(id);
				msgRecv++;
				}

			CleanupStack::PopAndDestroy(header);
			}

		if (msgRecv)	
			iSmsTest.Printf(_L("%d Messages Received in inbox\n"), msgRecv);
		}
	}

CSmsSendRecvTest::~CSmsSendRecvTest()
	{
	delete iEntryObserver;
	delete iEntryForObserver;
	}

CSmsSendRecvTest* CSmsSendRecvTest::NewLC(CSmsTestUtils& aSmsTest, const TDesC& aScriptFile, TInt& aNextTest)
	{
	CSmsSendRecvTest* self = new (ELeave) CSmsSendRecvTest(aSmsTest, aScriptFile, aNextTest);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}


void CSmsSendRecvTest::TestSendingL()
	{
	iSmsTest.Test().Next(_L("Sending"));

	TTime now;
	now.HomeTime();
	now += (TTimeIntervalSeconds) 5;

	iSelection->Reset();

	iSmsTest.DeleteSmsMessagesL(KMsvGlobalOutBoxIndexEntryId);

	TBool read = EFalse;
	iSmsTest.Printf(_L("Creating msgs in outbox from script:"));
	iSmsTest.Printf(iScriptFile);
	TRAPD(err, read = iSmsTest.ReadScriptL(iScriptFile, KMsvGlobalOutBoxIndexEntryId, *iSelection, now));

	iSmsTest.Test()(!err && read);

	iSmsTest.Printf(_L("Send and Receive %d messages...\n"), iSelection->Count());

	TInt count = iSelection->Count();
	iTotal = 0;
	iSent = 0;
	while (count--)
		{
		iTotal += CountRecipientsL(iSelection->At(count));
		}

	iCurrentMessageNum = -1;
	}

TInt CSmsSendRecvTest::CountRecipientsL(TMsvId aId)
	{
	CSmsHeader* header = iSmsTest.GetHeaderLC(aId);
	const TInt count = header->Recipients().Count();
	CleanupStack::PopAndDestroy(header);
	return count;
	}

TBool CSmsSendRecvTest::SendNextMessageL()
	{
	iCurrentMessageNum++;
	const TInt count = iSelection->Count();

	if (iCurrentMessageNum < count)
		{
		iSmsTest.Printf(_L("Sending message %d of %d\n"), iCurrentMessageNum + 1, count);


		delete iOperation;
		iOperation = NULL;

		CMsvEntrySelection* sel = new (ELeave) CMsvEntrySelection();
		CleanupStack::PushL(sel);
		const TMsvId id = iSelection->At(iCurrentMessageNum);
		sel->AppendL(id);
		iSent += CountRecipientsL(id);
		iSmsTest.SetEntryL(KMsvGlobalOutBoxIndexEntryId);
		iOperation = MsvEntry().CopyL(*sel, iSmsTest.iSmsServiceId, iStatus);
		CleanupStack::PopAndDestroy(); //sel
		
		iState = EStateSending;
		SetActive();
		CActiveScheduler::Start();
		return ETrue;
		}

	return EFalse;
	}

void CSmsSendRecvTest::TestWaitForReceiveL()
	{
	iSmsTest.Test().Next(_L("Waiting to receive message"));
	iState = EStateWaitingToReceive;
	TRequestStatus* status = &iStatus;
	User::RequestComplete(status, KErrNone);
	SetActive();
	}

CSmsSendRecvTest::CSmsSendRecvTest(CSmsTestUtils& aSmsTest, const TDesC& aScriptFile, TInt& aNextTest)
: CSmsTestBase(aSmsTest, aScriptFile, aNextTest), iCurrentMessageNum(-1), iState(EStateWaiting)
	{
	TBuf<256> command;
	User::CommandLine(command);
	command.Trim();
	iParse = (command.FindF(KParseAfterReceive) != KErrNotFound);
	iSaveToFile = (command.FindF(KSaveAfterReceive) != KErrNotFound);
	iTestBifs = (command.FindF(KTestBifs) != KErrNotFound);
	iWaitForever = (command.FindF(KWaitForever) != KErrNotFound);
	}

void CSmsSendRecvTest::RunAutoL()
	{	
	if (!iSmsTest.iSmokeTest && iTestBifs)
		{
		iSmsTest.TestStart(++iNextTest, _L("Add Remove Bifs"));
		TestBifsL();
		User::LeaveIfError(iStatus.Int());
		iSmsTest.TestFinish(iNextTest, KErrNone);
		}

	iSmsTest.TestStart(++iNextTest, _L("Read Script"));
	TestSendingL();
	User::LeaveIfError(iStatus.Int());
	iSmsTest.TestFinish(iNextTest, KErrNone);

	FOREVER
		{
		iSmsTest.TestStart(++iNextTest, _L("Send Message"));
		TBool sendNext = SendNextMessageL();
		if (sendNext)
			{
			User::LeaveIfError(iStatus.Int());
			iSmsTest.TestFinish(iNextTest, KErrNone);
			}
		else
			{
			iSmsTest.Printf(_L("No more messages to send!"));
			iSmsTest.TestFinish(iNextTest, KErrNone);
			break;
			}
		}

	iSmsTest.TestStart(++iNextTest, _L("Wait to receive extra message(s)"));
	WaitForExtraL();
	User::LeaveIfError(iStatus.Int());
	iSmsTest.TestFinish(iNextTest, KErrNone);

	iSmsTest.Printf(_L("\nDisplay Final Log:\n"));
	DisplayLogL();
	}

void CSmsSendRecvTest::ConstructL()
	{
	SetTestNameL(KSmsSendRecvTestName);
	iSmsTest.SetLogToFile();

	TBuf<256> command;
	User::CommandLine(command);
	command.Trim();
	iSmsTest.Printf(_L("Command Line: \"%S\" (Parse %d, Save %d, Wait %d, Bifs %d)\n"), &command, iParse, iSaveToFile, iWaitForever, iTestBifs);

	iSelection = new (ELeave) CMsvEntrySelection();
	iTimer = CTestTimer::NewL();
	CActiveScheduler::Add(this);

	iSmsTest.SetCommDbServiceCenterFromScriptL(iScriptFile);

  	iEntryObserver = CEntryObserver::NewL(iSmsTest, iCurrentMessageNum);
  	iEntryForObserver = Session().GetEntryL(KMsvGlobalInBoxIndexEntryId);
  	iEntryForObserver->AddObserverL(*iEntryObserver);

	ClearLogL();
	}

void CSmsSendRecvTest::ShowMenuL()
	{
	iSmsTest.ResetMenu();

	iSmsTest.AppendToMenuL(_L("Start"));
	iSmsTest.AppendToMenuL(_L("Display Log"));
	iSmsTest.AppendToMenuL(_L("Clear Log"));

	TInt result = iSmsTest.DisplayMenu(_L("SMS Send Receive Test"));

	if (result <= 0)
		return;

	switch (result)
		{
		case 1:
			RunAutoL();
			return;
		case 2:
			DisplayLogL();
			break;
		case 3:
			ClearLogL();
			break;
		default:
			User::Leave(KErrArgument);
			break;
		}

	ShowMenuL();
	}

void CSmsSendRecvTest::RunComplete()
	{
	if (!IsActive())
		CActiveScheduler::Stop();
	}

void CSmsSendRecvTest::RunL()
	{
	switch (iState)
		{
		case EStateSending:
			{
			DoRunSendingL();
			delete iOperation;
			iOperation = NULL;
			break;
			}
		case EStateWaitingToReceive:
			{
			DoRunWaitingToReceiveL();
			break;
			}
		case EStateWaitingToReceiveAnyExtra:
			{
			DoRunWaitForExtraL();
			break;
			}
		case EStateMessageReceived:
			{
			TestParseL();
			break;
			}
		case EStateParsing:
			{
			DoRunTestParseL();
			break;
			}
		default:
			{
			break;
			}
		}

	RunComplete();
	}

TInt CSmsSendRecvTest::RunError(TInt aError)
	{
	aError = CSmsTestBase::RunError(aError);
	RunComplete();
	return aError;
	}

void CSmsSendRecvTest::DoRunTestParseL()
	{
	TBioProgress progress;
	TBioProgressBuf progressBuf;
	progressBuf.Copy(iOperation->ProgressL());
	progress = progressBuf();

	if (iStatus == KErrNone)
		iStatus = progress.iErrorCode;

	delete iOperation;
	iOperation = NULL;
	}

void CSmsSendRecvTest::TestParseL()
	{
	const TMsvId id = iEntryObserver->LastReceived();

	iSmsTest.SetEntryL(id);

	if (iSaveToFile)
		{
		iSmsTest.Printf(_L("Saving BIO message to file\n"));
		iSmsTest.SaveBodyToFileL(id);
		}

	CSmsHeader* header = iSmsTest.GetHeaderLC(id);
 	CSmsDeliver& pdu = header->Deliver();

	TBool found = EFalse;
	TInt destination, originator, is16Bit;
	destination = originator = is16Bit = 0;
	if (pdu.UserDataPresent())
		{
		const CSmsUserData& userData = pdu.UserData();
		TInt count = userData.NumInformationElements();
		iSmsTest.Printf(_L("Pdu contains %d information elements\n"), count);
		while (!found && count--)
			{
			CSmsInformationElement& nextIE=userData.InformationElement(count);
			switch(nextIE.Identifier())
				{
				case CSmsInformationElement::ESmsIEIApplicationPortAddressing8Bit:
				case CSmsInformationElement::ESmsIEIApplicationPortAddressing16Bit:
					{
					pdu.ApplicationPortAddressing(destination, originator, &is16Bit);
					found = ETrue;
					break;
					}
				default:
					break;
				}
			}
		}

	CleanupStack::PopAndDestroy(header);

	if (found)
		iSmsTest.Printf(_L("Application Port Addressing: Dest %d, Origin %d, is16Bit %d\n"), destination, originator, is16Bit);
	else
		iSmsTest.Printf(_L("Application Port Addressing: Not Found\n"));

	if (!iParse)
		return;

	if (!iSmsTest.Entry().iBioType)
		return;

	iState = EStateParsing;
	
	iSmsTest.Printf(_L("Parsing BIO message\n"));

	CMsvEntrySelection* sel = new (ELeave) CMsvEntrySelection();
	CleanupStack::PushL(sel);

	sel->AppendL(id);

	delete iOperation;
	iOperation = NULL;
	
	iOperation = Session().TransferCommandL(*sel, KBiosMtmParse, KNullDesC8, iStatus);
	SetActive();

	CleanupStack::PopAndDestroy(sel);
	}

void CSmsSendRecvTest::DoCancel()
	{
	if (iOperation)
		{
		iOperation->Cancel();
		iSmsTest.Printf(_L("Operation Cancelled!\n"));
		}
	delete iOperation;
	iOperation = NULL;
	}

void CSmsSendRecvTest::DoRunSendingL()
	{
	if (iOperation)
		iSmsTest.SetProgressL(*iOperation);

	TInt count = iSelection->Count();
	TBool wait = EFalse;

	while (iStatus == KErrNone && count--)
		{
		TRAPD(err, iSmsTest.SetEntryL(iSelection->At(count)));

		if (err == KErrNone)
			{
			TMsvEntry entry = MsvEntry().Entry();
			iSmsTest.SetEntryL(KMsvGlobalOutBoxIndexEntryId);

			TBuf<0x100> output;
			TBuf<0x50>  nowBuf;

			switch (entry.SendingState())
				{
				case KMsvSendStateSending:
					output.AppendFormat(_L("Message %d Sending with error %d\n"), entry.Id(), entry.iError);
					wait = ETrue;
					break;
				case KMsvSendStateResend:
					output.AppendFormat(_L("Message %d Resend at "), entry.Id());
					entry.iDate.FormatL(nowBuf, _L("%-B%:0%J%:1%T%:2%S%:3%+B"));
					output.Append(nowBuf);
					output.Append(_L("\n"));
					wait = ETrue;
					break;
				case KMsvSendStateScheduled:
					output.AppendFormat(_L("Message %d Scheduled at "), entry.Id());					entry.iDate.FormatL(nowBuf, _L("%-B%:0%J%:1%T%:2%S%:3%+B"));
					entry.iDate.FormatL(nowBuf, _L("%-B%:0%J%:1%T%:2%S%:3%+B"));
					output.Append(nowBuf);
					output.Append(_L("\n"));
					wait = ETrue;
					break;
				case KMsvSendStateFailed:
					output.AppendFormat(_L("Message %d Failed with error %d\n"), entry.Id(), entry.iError);
					if (entry.iError)
						iStatus = entry.iError;
					else
						iStatus = KErrGeneral;

					break;
				case KMsvSendStateWaiting:
					output.AppendFormat(_L("Message %d Waiting with error %d\n"), entry.Id(), entry.iError);
					wait = ETrue;
					break;
				case KMsvSendStateSent:
				default:
					break;
					//nothing
				}

			iSmsTest.Printf(output);
			}
		}

	if (wait)
		{
		iTimer->AfterReq(10000000, iStatus);
		SetActive();
		iState = EStateSending;
//		CActiveScheduler::Start();
		}
	else
		{
		iSmsTest.Printf(_L("Sending completed with %d\n"), iStatus);
		
		if (iStatus == KErrNone)
			{
			TestWaitForReceiveL();
			}
		else
			{
			iState = EStateWaiting;
			}
		}
	}

void CSmsSendRecvTest::WaitForExtraL()
	{
	iSmsTest.Printf(_L("Waiting for any extra to arrive...\n"));
	iWaitForever = EFalse;
	iCountWaits = 0;
	iState = EStateWaitingToReceiveAnyExtra;
	TRequestStatus* status = &iStatus;
	User::RequestComplete(status, KErrNone);
	SetActive();
	CActiveScheduler::Start();
	}

void CSmsSendRecvTest::DoRunWaitForExtraL()
	{
	iCountWaits++;

	TInt recvCount = iEntryObserver->Count();

	if (recvCount > iSent)
		{
		iSmsTest.Printf(_L("Too many messages received:\nSent %d Rcvd %d\n"), iSent, recvCount);
		iStatus = KErrOverflow;
		}
	else
		DoWaitL();
	}

void CSmsSendRecvTest::DoRunWaitingToReceiveL()
	{
	iCountWaits++;

	const TInt recvCount = iEntryObserver->Count();

	if (recvCount == iSent)
		{
		//Message received
		if (!iSmsTest.AreMessagesSameTextL((*iSelection)[iCurrentMessageNum], iEntryObserver->Received(iSent - 1), ETrue))
			{
			//Do something?
			}
		}

	if (recvCount > iSent)
		{
		iSmsTest.Printf(_L("Too many messages received:\nSent %d Rcvd %d\n"), iSent, recvCount);
		iStatus = KErrOverflow;
		}
	else 
		{
		if (iTotal == recvCount || recvCount == iSent)
			{
			if (iTotal == recvCount)
				iSmsTest.Printf(_L("All Messages Received!!\n"));

			//Message received
			iState = EStateMessageReceived;
			iCountWaits = 0;
			TRequestStatus* status = &iStatus;
			User::RequestComplete(status, KErrNone);
			SetActive();
			}
		else
			{
			DoWaitL();
			}
		}
	}

void CSmsSendRecvTest::DoWaitL()
	{
	TInt recvCount = iEntryObserver->Count();

	TInt maxCount = KMaxWaitForMsg.Int() / KWaitForReceiveIteration.Int();
	TInt waitSecs = KWaitForReceiveIteration.Int() * (maxCount - iCountWaits) / 1000000;

	if (iWaitForever && iCountWaits <= 1)
		{
		iSmsTest.Printf(_L("Waiting forever to recv msg\n"));
		}
	else if (!iWaitForever)
		{
		iSmsTest.Printf(_L("Waiting %d secs to recv msg\n"), waitSecs);
		}

	if (iWaitForever || iCountWaits < maxCount)
		{
		iTimer->AfterReq(KWaitForReceiveIteration, iStatus);
		SetActive();
		}
	else
		{
		if (iSent != recvCount)
			{
			iSmsTest.Printf(_L("Time Out: Message NOT Received\n"));
			iStatus = KErrTimedOut;
			}
		}
	}

void CSmsSendRecvTest::TestBifsL()
	{
	iSmsTest.Printf(_L("Remove 2 Bifs if already exist\n"));	
	TestRemoveBifL(KTestBif1);
	TestRemoveBifL(KTestBif2);
	
	//Test add bif
	iSmsTest.Printf(_L("Test Add 2 Bifs\n"));	
	TestAddBifL(KTestBif1);
	TestAddBifL(KTestBif2);

	//Test remove bif
	iSmsTest.Printf(_L("Test Remove 2 Bifs\n"));	
	TestRemoveBifL(KTestBif1);
	TestRemoveBifL(KTestBif2);

	//Test change bif
	iSmsTest.Printf(_L("Test Change 2 Bifs\n"));	
	TestAddBifL(KTestBif1);
	TestAddBifL(KTestBif2);
	TestAddBifL(KTestBif1);
	TestAddBifL(KTestBif2);

	//Test remove bif
	iSmsTest.Printf(_L("Test Remove 2 Bifs\n"));	
	TestRemoveBifL(KTestBif1);
	TestRemoveBifL(KTestBif2);
	}

void CSmsSendRecvTest::TestAddBifL(const TDesC& aBifName)
	{
	iSmsTest.Printf(_L("\tAdd BIF: %S\n"), &aBifName);	
	
	RFs& fs = iSmsTest.FileSession();
	TInt err = fs.MkDir(KBifDir);

	if (err != KErrAlreadyExists)
		User::LeaveIfError(err);

	TParse fileOut;
	err = iSmsTest.ResolveFile(KSmsComponent, aBifName, fileOut);

	if (err)
		{
		iSmsTest.Printf(_L("Test BIF %S not found! Continuing without this test\n"), &aBifName);
		return;
		}

	CFileMan* fileMan = CFileMan::NewL(fs);
	CleanupStack::PushL(fileMan);

	err = fileMan->Copy(fileOut.FullName(), KBifDir);

	if (err != KErrAlreadyExists)
		User::LeaveIfError(err);

	CleanupStack::PopAndDestroy(fileMan);

	User::After(KBifWait);
	}

void CSmsSendRecvTest::TestRemoveBifL(const TDesC& aBifName)
	{
	iSmsTest.Printf(_L("\tRemove Bif: %S\n"), &aBifName);	

	RFs& fs = iSmsTest.FileSession();
	TFileName fileName(KBifDir);
	fileName.Append(aBifName);

	TInt err = fs.Delete(fileName);

	if (err)
		{
		iSmsTest.Printf(_L("\t\tfs.Delete() returned %d\n"), err);	
		
		if (err != KErrNotFound && err != KErrPathNotFound)
			User::Leave(err);
		}

	User::After(KBifWait);
	}

