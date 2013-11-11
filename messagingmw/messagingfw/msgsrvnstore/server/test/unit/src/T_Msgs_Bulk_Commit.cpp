// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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
#include "T_MSGS_Bulk_Commit.h"
#include "Testsetup.h"

const TUid KUidBulkCommitTestMtm = {0x10273EDD}; //KUidRajaSekharChowdary 
RTest test(_L("BulkCommit with multiple parents Testing"));

CMsvEntry* theEntry;
CMsvEntrySelection* theSelection;
CMsvSession* theSession;

CBulkCommitTest::CBulkCommitTest() : CActive(EPriorityStandard)
	{
	CActiveScheduler::Add(this);
	}

CBulkCommitTest* CBulkCommitTest::NewL()
	{		
	CBulkCommitTest* self = new(ELeave) CBulkCommitTest();	
	return self;
	}
	
CBulkCommitTest::~CBulkCommitTest()
	{
	//cancel any outstanding requests
	Cancel();	
	delete iOperation;		
	delete theSession;				
	}
	
LOCAL_C void TestStartL()
	{
	CBulkCommitTest* bulkTest = CBulkCommitTest::NewL();
	CleanupStack::PushL(bulkTest);
	//create client session
	theSession = CMsvSession::OpenSyncL(*bulkTest);
	
	theEntry = CMsvEntry::NewL(*theSession, KMsvRootIndexEntryId, TMsvSelectionOrdering());	
	CleanupStack::PushL(theEntry);
	
	theSelection = new(ELeave) CMsvEntrySelection;
	CleanupStack::PushL(theSelection);
	
	bulkTest->TestBulkCommitL();	
	CActiveScheduler::Start();

	CleanupStack::PopAndDestroy(3, bulkTest); //theSelection, theEntry, bulkTest
	}
	
void CBulkCommitTest::TestBulkCommitL()
	{
	theUtils->WriteComment(_L("Creating service Entry"));
		
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
	SetActive();
	}

void CBulkCommitTest::HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* /*aArg3*/)
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
				
				//Check whether this is the last entry createe,
				if(tempEntry.iDescription == _L("Bulk Commit Done"))
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

void CBulkCommitTest::RunL()
	{
	switch(iState)
		{
		case ECreate:
			{
			iStatus = KRequestPending;
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

void CBulkCommitTest::DoCancel()
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
