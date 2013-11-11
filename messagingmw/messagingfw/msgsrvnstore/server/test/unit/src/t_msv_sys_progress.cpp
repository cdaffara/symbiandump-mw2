// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <e32base.h>
#include <e32test.h>
#include <msvuids.h>
#include <msvids.h>
#include "t_msv_sys_progress.h"
#include "Testsetup.h"

const TUid KUidSysProgressTestMtm = {0x1028308F};
const TUid KUidBulkCommitTestMtm = {0x10273EDD};
RTest test(_L("System Progress test with multiple parents"));

CMsvEntry* theEntry;
CMsvEntrySelection* theSelection;
CMsvSession* theSession;

CSysProgressTest::CSysProgressTest() : CActive(EPriorityStandard)
	{
	CActiveScheduler::Add(this);
	}

CSysProgressTest* CSysProgressTest::NewL()
	{		
	CSysProgressTest* self = new(ELeave) CSysProgressTest();	
	return self;
	}
	
CSysProgressTest::~CSysProgressTest()
	{
	//cancel any outstanding requests
	Cancel();	
	delete iOperation;		
	delete theSession;				
	}
	
LOCAL_C void TestStartL()
	{
	CSysProgressTest* progressTest = CSysProgressTest::NewL();
	CleanupStack::PushL(progressTest);
	//create client session
	theSession = CMsvSession::OpenSyncL(*progressTest);
	
	theEntry = CMsvEntry::NewL(*theSession, KMsvRootIndexEntryId, TMsvSelectionOrdering());	
	CleanupStack::PushL(theEntry);
	
	theSelection = new(ELeave) CMsvEntrySelection;
	CleanupStack::PushL(theSelection);
	
	progressTest->TestBulkCommitProgressL();
//	progressTest->TestSysProgressL();
	CActiveScheduler::Start();
	CleanupStack::PopAndDestroy(3, progressTest); //theSelection, theEntry, progressTest
	}
	
void CSysProgressTest::TestSysProgressL()
	{
	theUtils->WriteComment(_L("Creating service Entry for SysProgress test"));
		
	TMsvEntry indexEntry;
	indexEntry.iType = KUidMsvServiceEntry;
	indexEntry.iMtm = KUidSysProgressTestMtm;
	indexEntry.SetReadOnly(EFalse);
	indexEntry.SetVisible(ETrue);
	indexEntry.SetUnread(ETrue);
	indexEntry.SetSendingState(KMsvSendStateNotApplicable);
	indexEntry.iDescription.Set(_L("Client Service Entry"));
	indexEntry.iDate.HomeTime();
	
	theEntry->SetEntryL(KMsvRootIndexEntryId);
	theEntry->CreateL(indexEntry);	
	
	TMsvId paramServiceId;
	paramServiceId = indexEntry.Id();
			
	theUtils->WriteComment(_L("Create 5 entries under two different root entries for SysProgress test"));	
	theSelection->AppendL(paramServiceId);
		
	// Dummy aParameter value passed as MsvSystemProgressTest MTM does not make use of it.
	iOperation = theSession->TransferCommandL(*theSelection, 0, KNullDesC8(), iStatus);	

	TMsvSystemProgress testclass;
	TPckgC<TMsvSystemProgress> progressPack(testclass);
	progressPack.Set(iOperation->ProgressL());
	TMsvSystemProgress TestMtmProgress = progressPack();
	
	//Test that ret returns KErrNone when SystemProgress is run successfully,
	//and systemProgress Error code was set to be 123456 by the test MTM.
	TMsvSystemProgress systemProgress;
	TInt ret = iOperation->SystemProgress(systemProgress);
	test(ret == 0);
	test(systemProgress.iErrorCode == 123456);
	TMsvId id = systemProgress.iId;
	}

void CSysProgressTest::TestBulkCommitProgressL()
	{
	theUtils->WriteComment(_L("Creating service Entry for Bulk Commit MTM test"));
		
	TMsvEntry indexEntry;
	indexEntry.iType = KUidMsvServiceEntry;
	indexEntry.iMtm = KUidBulkCommitTestMtm;
	indexEntry.SetReadOnly(EFalse);
	indexEntry.SetVisible(ETrue);
	indexEntry.SetUnread(ETrue);
	indexEntry.SetSendingState(KMsvSendStateNotApplicable);
	indexEntry.iDescription.Set(_L("Client Service Entry"));
	indexEntry.iDate.HomeTime();
	
	theEntry->SetEntryL(KMsvRootIndexEntryId);
	theEntry->CreateL(indexEntry);	
	
	TMsvId paramServiceId;
	paramServiceId = indexEntry.Id();
			
	theUtils->WriteComment(_L("Create 5 bulk entries under two different root entries"));	
	theSelection->AppendL(paramServiceId);
		
	// Dummy aParameter value passed as Bulkcommit MTM does not make use of it.
	// Command 0 will be evalueated to ECreateBulk
	iOperation = theSession->TransferCommandL(*theSelection, 0, KNullDesC8(), iStatus);	

	TMsvSystemProgress testclass;
	TPckgC<TMsvSystemProgress> progressPack(testclass);
	progressPack.Set(iOperation->ProgressL());
	TMsvSystemProgress TestMtmProgress = progressPack();
	
	//Test that the bulkcommit mtm will return KErrExtensionNotSupported as it does not
	//have an extension_ method.
	TMsvSystemProgress systemProgress;
	TInt ret = iOperation->SystemProgress(systemProgress);
	test(ret == KErrExtensionNotSupported);
	theUtils->WriteComment(_L("Bulk Commit MTM test passed"));
	SetActive();
	}

void CSysProgressTest::HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* /*aArg3*/)
	{
	TMsvId parentId = *(static_cast<TMsvId*>(aArg2));
				
	switch(aEvent)
		{
		case EMsvEntriesCreated:
			{							
			CMsvEntrySelection *sel = static_cast<CMsvEntrySelection*>(aArg1);	
			if(sel->Count() > 0)
				{				
				TMsvId id = sel->At(0);	
				TMsvId service;
				TMsvEntry tempEntry;
				theSession->GetEntry(id, service, tempEntry);
				
				//Check the state of the test.
				if(tempEntry.iDescription == _L("Bulk Commit Done"))
					{
					theUtils->WriteComment(_L("bulk commit done"));					
					TRequestStatus *status = &iStatus;				
					User::RequestComplete(status, KErrNone);
					//Set the status. Used to call the next test "BulkCommit Test"
					iState = ESysProgress;
					}
				else if(tempEntry.iDescription == _L("SysProgress testMtm Done"))
					{					
					TRequestStatus *status = &iStatus;				
					User::RequestComplete(status, KErrNone);
					//Set the status. Used to stop the scheduler
					iState = EDone;
					}
				}
			test(parentId != 0);			
			}				
			break;
		default:
			break;	
			}
	}

void CSysProgressTest::RunL()
	{
	switch(iState)
		{
		case ECreate:
			{
			iStatus = KRequestPending;
			SetActive();								
			}
			break;
		case ESysProgress:
			{
			theUtils->WriteComment(_L("prepare ESysProgress"));	
			delete iOperation;
			iOperation = NULL;
			theSelection->Reset();
			TestSysProgressL();
			SetActive();
			}
			break;
		case EDone:
			{
			delete iOperation;
			iOperation = NULL;	
			CActiveScheduler::Stop();										
			}
			break;	
		}
	}

void CSysProgressTest::DoCancel()
	{
	if(iOperation)
		{
		iOperation->Cancel();		
		}
	}

void doTestsL()
	{
	theUtils->Start(_L("Testing Bulk Commit with multiple parents"));
	TestStartL();	
	theUtils->Complete();
	}
