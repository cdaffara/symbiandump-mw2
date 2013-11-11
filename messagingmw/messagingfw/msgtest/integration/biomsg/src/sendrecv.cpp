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

#include "sendrecv.h"
#include <smuthdr.h>

#include <txtrich.h>
#include <txtfmlyr.h>

#include <bitsids.h>

const TTimeIntervalMicroSeconds32 KWaitForReceiveIteration = 2000000;
const TTimeIntervalMicroSeconds32 KMaxWaitForMsg = 60000000;

CSendReceive::~CSendReceive()
	{
	delete iEntryForObserver;

	iReceived = NULL;
	delete iReceived;
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
	}


CSendReceive* CSendReceive::NewL(CSmsTestUtils& aTestUtils)
	{
	CSendReceive* self = new (ELeave) CSendReceive(aTestUtils);
	CleanupStack::PushL(self);

	self->ConstructL();

	CleanupStack::Pop();
	return self;
	}


void CSendReceive::TestSendingL()
//
//
//
	{
	iSmsTest.Test().Next(_L("Sending"));

	TTime now;
	now.HomeTime();
	now += (TTimeIntervalSeconds) 5;

	iSelection->Reset();

	iSmsTest.DeleteSmsMessagesL(KMsvGlobalOutBoxIndexEntryId);

	TBool read = EFalse;
	iSmsTest.Printf(_L("Creating msgs in outbox from script:"));
	iSmsTest.Printf(KSendRecvScript);
	TRAPD(err, read = iSmsTest.ReadScriptL(KSendRecvScript, KMsvGlobalOutBoxIndexEntryId, *iSelection, now));

	iSmsTest.Test()(!err && read);

	// read any test messages from file
//	CreateWappMessagesL();

	iSmsTest.Printf(_L("Send and Receive %d messages...\n"), iSelection->Count());

	iCurrentMessageNum = -1;
	}
/*
void CSendReceive::CreateWappMessagesL()
	{

	TBufC<KMaxFileName> currentFile;

	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection();
	CleanupStack::PushL(selection);

	TInt err = Session().FileSession().GetDir(KMessageFileDirectory, KEntryAttMatchMask, ESortByName, iDir);
	if (err == KErrPathNotFound)
		{
		TInt makeDirErr = Session().FileSession().MkDirAll(KMessageFileDirectory);	
		makeDirErr==KErrNone ? User::Leave(KErrNotFound): User::Leave(makeDirErr);
		}
	else if (err!=KErrNone)
		{
		User::Leave(err);
		}

	// Set the session path for the RFs
	Session().FileSession().SetSessionPath(KMessageFileDirectory);
	if (iDir) 
		delete iDir;
	User::LeaveIfError(Session().FileSession().GetDir(_L("wapp*.txt"), KEntryAttNormal, ESortByName, iDir));
    TInt count = iDir->Count();
    
	TBuf<60> outputBuf;

	for(TInt loop = 0; loop < count; loop++)
		{
		currentFile=( (*iDir)[loop].iName );
		// Not processed all the messages - so keep the current state
		
		}
	CleanupStack::Pop(); // selection
	}

HBufC* CSendReceive::ReadWappMessageFromFileL(const TDesC& aFile)
	{
	RFile           file;
	TBuf8<1024>     lineBuffer;   // Buffer for Read function.
	TInt			err=KErrNone;

	err = file.Open(Session().FileSession(),aFile,EFileStreamText|EFileRead|EFileShareAny);

    if(err != KErrNone)  // Didn't find the file, so leave - should only get valid filenames!
        {
        User::Leave(KErrNotFound);
		}

	// In real life this is set by the Socket Port Observer, or the SMS server.
	// So,in this test code we must do it by creating a random  telephone number.
	//	iMessageDetails = CreateDummyPhoneNumberL();

    HBufC*	bioBuf=HBufC::NewLC(1024); // Create a new descriptor on the heap.
	HBufC*	copyBuffer=HBufC::NewLC(1024);

    do // Read in the text from file, and also check if there is a name field:
		{
		err = file.Read(lineBuffer);// Read upto 256 chars, '\n' and all...
		//err = ReadLineL(file,lineBuffer);
		if (err==KErrNone)				// Made a valid read,
			if (lineBuffer.Length()==0) // but read 0 chars
				err=KErrEof;			// so set err value to end processing

		if(err == KErrNone)
			{
			copyBuffer->Des().Copy(lineBuffer);// Copy, and overwrite existing text
			if( (bioBuf->Length() + copyBuffer->Length()) > bioBuf->Des().MaxLength() )
					{
					bioBuf = bioBuf->ReAllocL(bioBuf->Length() + copyBuffer->Length());
					}
			bioBuf->Des().Append(*copyBuffer);
            //bioBuf->Des().Append(_L("\n"));
			}
		}
    while(err != KErrEof);

	CleanupStack::PopAndDestroy(); // Destroy the copyBuffer.
	CleanupStack::Pop();// Remove the bioBuf.
    
	file.Close();
	return bioBuf;  // Transfer ownership and responsibility of the buffer.
	}

void CSendReceive::CreateMessageL(const TDesC& aFile)
	{
	CParaFormatLayer* paraFormat = CParaFormatLayer::NewL();
	CleanupStack::PushL(paraFormat);
	CCharFormatLayer* charFormat = CCharFormatLayer::NewL();
	CleanupStack::PushL(charFormat);
	CRichText* richText = CRichText::NewL(paraFormat, charFormat);
	CleanupStack::PushL(richText);

	HBufC* buffer = ReadWappMessageFromFileL(aFile);
	CleanupStack::PushL(richText);
	
	richText->InsertL(0,*buffer);

	CSmsHeader* header = CSmsHeader::NewL(CSmsPDU::ESmsSubmit, *richText);
	CleanupStack::PushL(header);

	TTime now;
    now.HomeTime();

	TMsvEntry entry;
	entry.SetSendingState(KMsvSendStateUponRequest);
	entry.iBioType = KUidBIOWAPAccessPointMsg;

	iSmsTest.CreateMessageL(KMsvGlobalInBoxIndexEntryId,entry,*header,*richText,now);

	delete buffer;
	CleanupStack::PopAndDestroy(5);
	}
*/
TBool CSendReceive::SendNextMessageL()
	{
	iCurrentMessageNum++;
	const TInt count = iSelection->Count();

	if (iCurrentMessageNum < count)
		{
		iSmsTest.Printf(_L("Sending message %d of %d\n"), iCurrentMessageNum + 1, count);

		iSmsTest.SetEntryL(KMsvGlobalOutBoxIndexEntryId);

		delete iOperation;
		iOperation = NULL;

		CMsvEntrySelection* sel = new (ELeave) CMsvEntrySelection();
		CleanupStack::PushL(sel);
		sel->AppendL(iSelection->At(iCurrentMessageNum));
		iOperation = MsvEntry().CopyL(*sel, iSmsTest.iSmsServiceId, iStatus);
		CleanupStack::PopAndDestroy(); //sel
		
		iState = EStateSending;
		SetActive();
		CActiveScheduler::Start();
		return ETrue;
		}

	return EFalse;
	}

void CSendReceive::TestWaitForReceiveL()
	{
	iSmsTest.Test().Next(_L("Waiting to receive message"));
	iState = EStateWaitingToReceive;
	TRequestStatus* status = &iStatus;
	User::RequestComplete(status, KErrNone);
	SetActive();
	}

CSendReceive::CSendReceive(CSmsTestUtils& aTestUtils)
: CActive(EPriorityStandard),iCurrentMessageNum(-1),iSmsTest(aTestUtils),iState(EStateWaiting)
	{
	}

void CSendReceive::StartL(CMsvEntrySelection* aReceived)
	{
	iCurrentTest = 0;
	iReceived = aReceived;
	
	iSmsTest.TestStart(++iCurrentTest, _L("Read Script"));
	TestSendingL();
	User::LeaveIfError(iStatus.Int());
	iSmsTest.TestFinish(iCurrentTest, KErrNone);

	FOREVER
		{
		iSmsTest.TestStart(++iCurrentTest, _L("Send Message"));
		TBool sendNext = SendNextMessageL();
		if (sendNext)
			{
			User::LeaveIfError(iStatus.Int());
			iSmsTest.TestFinish(iCurrentTest, KErrNone);
			}
		else
			{
			iSmsTest.Printf(_L("No more messages to send!"));
			iSmsTest.TestFinish(iCurrentTest, KErrNone);
			break;
			}
		}

	iSmsTest.TestStart(++iCurrentTest, _L("Wait to receive extra message(s)"));
	User::LeaveIfError(iStatus.Int());
	iSmsTest.TestFinish(iCurrentTest, KErrNone);
	}

void CSendReceive::ConstructL()
	{
	iSelection = new (ELeave) CMsvEntrySelection();
	iTimer = CTestTimer::NewL();
	CActiveScheduler::Add(this);

	iSmsTest.SetCommDbServiceCenterFromScriptL(KSendRecvScript);
	}

void CSendReceive::RunL()
	{
	TRAPD(err, DoRunL());
	if (err && iStatus == KErrNone)
		{
		iStatus = err;
		}
	if(!IsActive())
		CActiveScheduler::Stop();
	}

void CSendReceive::DoRunL()
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
			break;
			}
		default:
			{
			break;
			}
		}
	}

void CSendReceive::DoCancel()
	{
	if (iOperation)
		{
		iOperation->Cancel();
		iSmsTest.Printf(_L("Operation Cancelled!\n"));
		}
	delete iOperation;
	iOperation = NULL;
	}

void CSendReceive::DoRunSendingL()
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
		}
	else
		{
		iSmsTest.Printf(_L("Sending completed with error %d\n"), iStatus);
		
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

void CSendReceive::WaitForExtraL()
	{
	iSmsTest.Printf(_L("Waiting for any extra to arrive...\n"));
	iCountWaits = 0;
	iState = EStateWaitingToReceiveAnyExtra;
	TRequestStatus* status = &iStatus;
	User::RequestComplete(status, KErrNone);
	SetActive();
	}

void CSendReceive::DoRunWaitForExtraL()
	{
	iCountWaits++;

	TInt totalSentCount = iSelection->Count();
	TInt recvCount = iReceived->Count();

	if (recvCount > totalSentCount)
		{
		iSmsTest.Printf(_L("Too many messages received:\nSent %d Rcvd %d\n"), totalSentCount, recvCount);
		iStatus = KErrOverflow;
		}
	else
		DoWaitL();
	}

void CSendReceive::DoRunWaitingToReceiveL()
	{
	iCountWaits++;

	TInt totalSentCount = iSelection->Count();
	TInt recvCount = iReceived->Count();

	if (recvCount > totalSentCount || recvCount > (iCurrentMessageNum + 1))
		{
		iSmsTest.Printf(_L("Too many messages received:\nSent %d Rcvd %d\n"), totalSentCount, recvCount);
		iStatus = KErrOverflow;
		}
	else 
		{
		if (totalSentCount == recvCount)
			{
			iSmsTest.Printf(_L("All Messages Received!!\n"));
			}
		else if (recvCount == iCurrentMessageNum + 1)
			{
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

void CSendReceive::DoWaitL()
	{
	TInt totalSentCount = iSelection->Count();
	TInt recvCount = iReceived->Count();

	TInt maxCount = KMaxWaitForMsg.Int() / KWaitForReceiveIteration.Int();
	TInt waitSecs = KWaitForReceiveIteration.Int() * (maxCount - iCountWaits) / 1000000;
	iSmsTest.Printf(_L("Waiting %d secs to recv msg\n"), waitSecs);

	if (iCountWaits < maxCount)
		{
		iTimer->AfterReq(KWaitForReceiveIteration, iStatus);
		SetActive();
		}
	else
		{
		iSmsTest.Printf(_L("Time Out: Message NOT Received\n"));

		if (totalSentCount != recvCount)
			{
			iStatus = KErrTimedOut;
			}
		}
	}
