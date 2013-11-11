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

#include "t_schsend1.h"
#include <msvstd.h>
#include <msvids.h>
#include <msvuids.h>
#include "pigeonservermtm.h"

CTrapCleanup* theCleanup;

void doMainL(RTest& aTest)
	{
	CActiveScheduler* scheduler = new (ELeave) CActiveScheduler;
	CleanupStack::PushL(scheduler);
	CActiveScheduler::Install( scheduler );

	CSchSendTest* schSendTest = CSchSendTest::NewL(aTest);
	CleanupStack::PushL(schSendTest);

	schSendTest->StartL();

	CleanupStack::PopAndDestroy(2); //schSendTest, scheduler

	aTest.Printf(_L("\nWaiting 5 seconds before closing...\n"));
	User::After(5000000);
	}

GLDEF_C TInt E32Main()
	{
	RTest test(_L("Scheduled Sending Test Harness"));
	__UHEAP_MARK;
	test.Title();
	test.Start(_L("Scheduled Sending Test Harness"));
	theCleanup = CTrapCleanup::New();
	test(theCleanup !=NULL);
	TRAPD(error,doMainL(test));
	test(error==KErrNone);
	delete theCleanup;
	test.Printf(_L("Completed with return code %d"),error);
	test.End();
	test.Close();
	__UHEAP_MARKEND;
	return KErrNone;
	}

CSchSendTest* CSchSendTest::NewL(RTest& aTest)
	{
	CSchSendTest* self = new (ELeave) CSchSendTest(aTest);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(); //self
	return self;
	}

CSchSendTest::CSchSendTest(RTest& aTest)
: CMsvTestUtils(aTest)
	{
	}

void CSchSendTest::InstallMtmGroupsL()
	{
	}

void CSchSendTest::CreateServerMtmRegsL()
	{
//CreateServerMtmRegL(KPigeonMtmUid, _L("Pig"), KUidMsvMtmServerDLL, KUidMsvMtmClientDLL, TMsvTestDllInfo(KUidMsvMtmUiDLL,2), TMsvTestDllInfo(KUidMsvMtmUiDLL,1), KPigeonMtmUid, KDataComponentFileName);
	}

void CSchSendTest::ConstructL()
	{
	CTestUtils::ConstructL();
	CreateAllTestDirectories();
	GoClientSideL();

	InstallMtmGroupL(_L("c:\\system\\mtm\\pigmtmu.dat"));

	iSelection = new(ELeave) CMsvEntrySelection;
	iSchTestActive = CScheduleTestActive::NewL(*this);
	}

void CSchSendTest::ShowMenuL()
	{
	RunAutoL();
	}

CSchSendTest::~CSchSendTest()
	{
	delete iSchTestActive;
	delete iSelection;
	}

void CSchSendTest::Panic(TInt aPanic)
	{
	User::Panic(_L("SchSend Test"), aPanic);
	}

void CSchSendTest::StartL()
	{
	TRAPD(err, RunAutoL());

	if (err)
		{
		if (iCurrentTest)
			{
			TestFinish(iCurrentTest, err);
			}

		TestHarnessFailed(err);
		}
	else
		{
		TestHarnessCompleted();
		}
	}

void CSchSendTest::RunAutoL()
	{
	TestSchedulingMessageL();
	}

TMsvEntry CSchSendTest::CreateMessageLC(TMsvId aDestFolder, TTime aSendTime, TBool aOffPeak)
	{
	SetEntryL(aDestFolder);
	TMsvEntry entry;
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm = KPigeonMtmUid;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	entry.iDate = aSendTime;
	entry.SetOffPeak(aOffPeak);
	CreateEntryL(entry);
	iMsvSession->CleanupEntryPushL(entry.Id());
	SetEntryL(entry.Id());
	return entry;
	}

void CSchSendTest::TestSchedulingMessageL()
	{
	TTime now;
	now.HomeTime();
	TMsvEntry entry = CreateMessageLC(KMsvGlobalOutBoxIndexEntryId, now);

	iSelection->Reset();
	iSelection->AppendL(entry.Id());

	iSchTestActive->TransferCommandSyncL(CPigeonServerMtm::EScheduleAllL, *iSelection);

	CleanupStack::PopAndDestroy(); //entry
	}

CScheduleTestActive::CScheduleTestActive(CMsvTestUtils& aTest)
: CActive(EPriorityStandard), iTest(aTest)
	{
	CActiveScheduler::Add(this);
	}

CScheduleTestActive* CScheduleTestActive::NewL(CMsvTestUtils& aTest)
	{
	CScheduleTestActive* self = new (ELeave) CScheduleTestActive(aTest);
	CleanupStack::PushL(self);
	self->iSession = CMsvSession::OpenSyncL(*self);
	CleanupStack::Pop(); //self
	return self;
	}

CScheduleTestActive::~CScheduleTestActive()
	{
	Cancel();
	delete iSelection;
	delete iSession;
	}

//Set aStopAtRunL to true to stop the active scheduler. 
//False will make us wait for someone else to stop it
void CScheduleTestActive::TransferCommandSyncL(CPigeonServerMtm::TPigOperation aCommand, const CMsvEntrySelection& aSelection, TBool aStopAtRunL)
	{
	iTest.Printf(_L("TransferCommandSyncL Command %d Count %d StopAtRunL %d\n"), aCommand, aSelection.Count(), aStopAtRunL);

	if (IsActive())
		User::Leave(KErrInUse);

	delete iSelection;
	iSelection = NULL;
	iSelection = aSelection.CopyL();
	iCommand = aCommand;
	iStopAtRunL = aStopAtRunL;
	iStatus = KRequestPending;
	SetActive();

	CMsvOperation*  opert = iSession->TransferCommandL (*iSelection, 
				aCommand,
				_L8("aaaa"), 
				iStatus);

	CleanupStack::PushL(opert);
	CActiveScheduler::Start(); // will not return from this until the command is transferred

	if(iStatus.Int()!=KErrNone)
		User::Leave(iStatus.Int());

	CleanupStack::PopAndDestroy(); //opert
	}

void CScheduleTestActive::RunL()
	{
	iTest.Printf(_L("CScheduleTestActive::RunL() iStatus %d\n"), iStatus.Int());

	if(iStopAtRunL) 
		{
		CActiveScheduler::Stop();
		return;
		}

	switch (iCommand)
		{
		case CPigeonServerMtm::EScheduleAllL:
		case CPigeonServerMtm::EReScheduleAllL:
			{		
			for(int i = 0; i < iSelection->Count(); i++)
				{
				CMsvEntry* entry = iSession->GetEntryL(iSelection->At(i));
				CleanupStack::PushL(entry);
				TInt state = entry->Entry().SendingState();
				switch(state)
					{
					case KMsvSendStateScheduled:
					case KMsvSendStateResend:
						break;
					default: // The message failed
						CActiveScheduler::Stop();
						break;
					}
				CleanupStack::PopAndDestroy(); //entry
				}
		
			break;
			}
		default:
			CActiveScheduler::Stop(); //Added by AA 9/6/2000. Is this correct?
			break;
		}
	}

void CScheduleTestActive::HandleSessionEventL(TMsvSessionEvent eventType, TAny* p1, TAny*, TAny*)
	{
	if (iStopAtRunL) return;
	if (eventType != EMsvEntriesCreated) return;

	CMsvEntrySelection* entries = STATIC_CAST(CMsvEntrySelection*, p1);
	if(entries->Count() == 0) 
		User::Panic(_L("sch send test"), 23);
	CMsvEntry* cEntry = iSession->GetEntryL((*entries)[0]);
	CleanupStack::PushL(cEntry);

	TMsvEntry entry = cEntry->Entry();
	
	if(entry.iDetails == _L("sch send test"))
		{
		iTest.Printf(_L("Scheduled message has been sent\n"));

		//We found the right message!
		CActiveScheduler::Stop();

		//Delete the new message
		CMsvOperationWait* wait = CMsvOperationWait::NewLC();
		wait->Start();
		cEntry->SetEntryL(entry.Parent());
		CMsvOperation* op = cEntry->DeleteL(entry.Id(), wait->iStatus);
		CActiveScheduler::Start();
		delete op;
		CleanupStack::PopAndDestroy(); //wait
		}

	CleanupStack::PopAndDestroy(); //cEntry
	}
