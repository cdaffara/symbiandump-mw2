// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "test_scheduledentry_step.h"
#include <msvapi.h>
#include <msvuids.h>
#include <msvids.h>



/**
CDummyMsvScheduledEntry
*/	
CDummyMsvScheduledEntry* CDummyMsvScheduledEntry::NewL(const TMsvEntry& aMsvEntry)
	{
	CDummyMsvScheduledEntry* self = new (ELeave) CDummyMsvScheduledEntry(aMsvEntry);
	return self;
	}

CDummyMsvScheduledEntry::CDummyMsvScheduledEntry(const TMsvEntry& aMsvEntry)
	: CMsvScheduledEntry(aMsvEntry)
	{
	}	
	
/**
CMsvScheduledEntryStep
*/
	
CMsvScheduledEntryStep::CMsvScheduledEntryStep()
	{
	SetTestStepName(KMsvScheduledEntryStep);
	}	

void CMsvScheduledEntryStep::HandleSessionEventL(TMsvSessionEvent, TAny*, TAny*, TAny*)
	{
	//do nothing
	}
					
TVerdict CMsvScheduledEntryStep::doTestStepPreambleL()
	{
	__UHEAP_MARK;
	iScheduler = new (ELeave) CActiveScheduler();
	CActiveScheduler::Install(iScheduler);
	iSession = CMsvSession::OpenSyncL(*this);	
	InitializeEntryL();
	
	return TestStepResult();
	}			

void CMsvScheduledEntryStep::InitializeEntryL()
	{
	iEntry.iType = KUidMsvMessageEntry;
	iEntry.iServiceId = KMsvLocalServiceIndexEntryId;
	iEntry.iMtm.iUid = 0x1;
	TTime scheduleTime;
	scheduleTime.HomeTime();
	iEntry.iDate = scheduleTime.Int64();
	iEntry.SetScheduled(ETrue);
	iEntry.SetSendingState(KMsvSendStateResend);
	iEntry.iError = -1;
	iEntry.SetOffPeak(EFalse);
	iEntry.SetFailed(EFalse);
	iEntry.SetPendingConditions(ETrue);

	iScheduledEntry = CDummyMsvScheduledEntry::NewL(iEntry);
	iScheduledEntry->iData.iTaskId = 1;
	iScheduledEntry->iData.iRef.iHandle = 2;
	iScheduledEntry->iData.IncreaseRetries();
	}
					
TVerdict CMsvScheduledEntryStep::doTestStepL()
	{
	//Test Constructor
	TestGetValues();
	
	//Test Setters and Getters	
	TestSetValues();
		
	//Test store and restore	
	TestStoreRestoreL();
	
	//Test Entry(TMsvEntry&) method
	TestEntry();
	return TestStepResult();
	}			
				
TVerdict CMsvScheduledEntryStep::doTestStepPostambleL()
	{
	delete iScheduledEntry;
	delete iSession;
	CActiveScheduler::Install(NULL);
	delete iScheduler;
	__UHEAP_MARKEND;
	return TestStepResult();
	}


void CMsvScheduledEntryStep::TestGetValues()
	{
	if(iScheduledEntry->ScheduleDate() != iEntry.iDate ||
		iScheduledEntry->Scheduled() != iEntry.Scheduled() ||	
		(TUint)iScheduledEntry->SendingState() != iEntry.SendingState() ||
		iScheduledEntry->Error() != iEntry.iError ||
		iScheduledEntry->Id() != iEntry.Id() ||
		iScheduledEntry->OffPeak() != iEntry.OffPeak() ||
		iScheduledEntry->Failed() != iEntry.Failed() ||
		iScheduledEntry->PendingConditions() != iEntry.PendingConditions() ||
		iScheduledEntry->Mtm() != iEntry.iMtm)
		{
		INFO_PRINTF1(_L("CMsvScheduledEntry: Initialization error"));
		SetTestStepResult(EFail);
		}
	}	

void CMsvScheduledEntryStep::TestSetValues()
	{
	TTime scheduleTime;
	scheduleTime.HomeTime();
	iScheduledEntry->SetScheduleDate(scheduleTime);
	iScheduledEntry->SetScheduled(EFalse);
	iScheduledEntry->SetSendingState(KMsvSendStateSuspended);
	iScheduledEntry->SetFailed(ETrue);
	iScheduledEntry->SetPendingConditions(EFalse);
	
	if(iScheduledEntry->ScheduleDate() != scheduleTime ||
		iScheduledEntry->SendingState() != KMsvSendStateSuspended ||
		iScheduledEntry->Scheduled() ||
		iScheduledEntry->PendingConditions() ||
		!iScheduledEntry->Failed())
		{
		INFO_PRINTF1(_L("CMsvScheduledEntry:: setters wrong!"));
		SetTestStepResult(EFail);
		}
	}	

void CMsvScheduledEntryStep::TestStoreRestoreL()
	{
	CMsvEntry* cEntry = iSession->GetEntryL(KMsvGlobalOutBoxIndexEntryId);
	CleanupStack::PushL(cEntry);
	
	cEntry->CreateL(iEntry);
	cEntry->Session().CleanupEntryPushL(iEntry.Id());
	cEntry->SetEntryL(iEntry.Id());
	
	CMsvStore* store = cEntry->EditStoreL();
	CleanupStack::PushL(store);
	
	iScheduledEntry->StoreL(*store);
	
	TMsvEntry anotherEntry;
	CMsvScheduledEntry* anotherScheduledEntry = CDummyMsvScheduledEntry::NewL(anotherEntry);
	CleanupStack::PushL(anotherScheduledEntry);
	anotherScheduledEntry->RestoreL(*store);
	
	if(iScheduledEntry->iData.iTaskId != anotherScheduledEntry->iData.iTaskId ||
		iScheduledEntry->iData.iRef.iHandle != anotherScheduledEntry->iData.iRef.iHandle ||
		iScheduledEntry->iData.Retries() != anotherScheduledEntry->iData.Retries())
		{
		INFO_PRINTF1(_L("CMsvScheduledEntry::Store/Restore wrong"));
		SetTestStepResult(EFail);
		}
	
	CleanupStack::PopAndDestroy(anotherScheduledEntry);
	CleanupStack::PopAndDestroy(store);
	cEntry->Session().CleanupEntryPop();
	cEntry->SetEntryL(KMsvGlobalOutBoxIndexEntryId);
	cEntry->DeleteL(iEntry.Id());
	CleanupStack::PopAndDestroy(cEntry);			
	}

void CMsvScheduledEntryStep::TestEntry()
	{
	TMsvEntry copyEntry;
	iScheduledEntry->Entry(copyEntry);
	//compare
	if(copyEntry.iDate != iScheduledEntry->ScheduleDate())
		{
		SetTestStepResult(EFail);
		}
	if(	copyEntry.Scheduled() != iScheduledEntry->Scheduled())
		{
		SetTestStepResult(EFail);
		}
	if(	copyEntry.SendingState() != (TUint)iScheduledEntry->SendingState() )
		{
		SetTestStepResult(EFail);
		}
	
	if(	copyEntry.Failed() != iScheduledEntry->Failed())
		{	
		SetTestStepResult(EFail);
		}
	
	if(TestStepResult() == EFail)
		{
		INFO_PRINTF1(_L("CMsvScheduledEntry::Entry() wrong"));
		}
	}
	
