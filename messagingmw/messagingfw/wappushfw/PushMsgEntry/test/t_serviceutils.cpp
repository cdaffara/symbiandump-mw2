// Copyright (c) 2000-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// source file for CWapPushServiceFinder - basic function is to locate a service Id, and if none exists
// then find one. 
// 
//

#include <msvids.h>
#include <msvuids.h>

#include <pushentry.h>
#include <push/cwappushmsgutils.h>
#include  <push/sislpushmsgutils.h>
#include "t_serviceutils.h"



CWapPushTestMsgUtils*  CWapPushTestMsgUtils::NewL()
	{
	CWapPushTestMsgUtils* self = new (ELeave) CWapPushTestMsgUtils();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

CWapPushTestMsgUtils::~CWapPushTestMsgUtils()
	{
	delete iMsvEntry;
	delete iMsvSession;
	}

void CWapPushTestMsgUtils::ConstructL()
	{
	iMsvSession = CMsvSession::OpenSyncL(*this);
	iMsvEntry = iMsvSession->GetEntryL(KMsvRootIndexEntryId);
	}


void CWapPushTestMsgUtils::PushServiceIdL(TMsvId& rFirstId, CMsvEntrySelection* rServiceIds)
	{
    //Returns the Service IDs of MTM aMtm, or Null value if none exist

	rFirstId = KMsvNullIndexEntryId;

	iMsvEntry->SetEntryL(KMsvRootIndexEntryId);

	TMsvSelectionOrdering order;
	order.SetShowInvisibleEntries(ETrue);
	iMsvEntry->SetSortTypeL(order);

	//Get the children on the Root Index Entry
	CMsvEntrySelection* selection = iMsvEntry->ChildrenWithTypeL(KUidMsvServiceEntry);
	CleanupStack::PushL(selection);

	TInt count = selection->Count();

	//Find an entry for MTM aMtm
	for (TInt curChild = 0; curChild < count && (rFirstId == KMsvNullIndexEntryId || rServiceIds); curChild++)
		{
		iMsvEntry->SetEntryL(selection->At(curChild));

		if (iMsvEntry->Entry().iMtm == KUidMtmWapPush)
			{
			TMsvId id = iMsvEntry->Entry().Id();

			if (rFirstId == KMsvNullIndexEntryId)
				rFirstId = id;

			if (rServiceIds)
				rServiceIds->AppendL(id);
			}
		}

	CleanupStack::Pop(); //selection
	delete selection;
	}



void CWapPushTestMsgUtils::InstallMtmGroupL(const TDesC& aDatFile)
	{
	TInt err = KErrNone;
	err = iMsvSession->InstallMtmGroup(aDatFile);
	if (err != KErrAlreadyExists)
		User::LeaveIfError(err);
	}



void CWapPushTestMsgUtils::RemoveServiceEntryChildrenL(TMsvId aServiceId)
	{
	CMsvEntrySelection* inboxMessageSel;

	TMsvSelectionOrdering sort;
	sort.SetShowInvisibleEntries(ETrue);
	iMsvEntry->SetSortTypeL(sort);

	iMsvEntry->SetEntryL(aServiceId);
	inboxMessageSel = iMsvEntry->ChildrenL();
	CleanupStack::PushL(inboxMessageSel);
	
	DeleteEntriesSychronouslyL(inboxMessageSel);
	
	CleanupStack::PopAndDestroy();	// inboxMessageSel

	iMsvEntry->SetEntryL(KMsvRootIndexEntryId);

	}


// Cleans the message folder of all entries with the specified Mtm Type. Get the Inbox Entry, and then all the 
// children of this entry.Then iterate through the selection deleting each entry with chosesn Mtm Uid.
void CWapPushTestMsgUtils::RemoveEntriesFromLocalServiceFolderL(TMsvId aPushFolderId, TUid aMtm)
	{
	CMsvEntrySelection* inboxMessageSel;

	TMsvSelectionOrdering sort;
	sort.SetShowInvisibleEntries(ETrue);
	iMsvEntry->SetSortTypeL(sort);
	
	iMsvEntry->SetEntryL(aPushFolderId);
	__ASSERT_ALWAYS(iMsvEntry->Entry().iType == KUidMsvFolderEntry,
		User::Panic(_L("CWapPushTestMsgUtils"), 0));
	inboxMessageSel = iMsvEntry->ChildrenWithMtmL(aMtm);
	CleanupStack::PushL(inboxMessageSel);

	for (TInt count =0; count < inboxMessageSel->Count(); count++)
		{
		iMsvEntry->DeleteL(inboxMessageSel->At(count));
		}

	iMsvEntry->SetEntryL(KMsvRootIndexEntryId);
	CleanupStack::PopAndDestroy();	// inboxMessageSel
	}



void CWapPushTestMsgUtils::DeleteEntriesSychronouslyL(CMsvEntrySelection* aSelection)
	{
	if (aSelection->Count() != 0)
		{
		// iMsvEntry should be set to the parent of the entries to delete, otherwise delete leaves
		iMsvEntry->SetEntryL(aSelection->At(0));
		iMsvEntry->SetEntryL(iMsvEntry->Entry().Parent());
		CMsvOperationWait* wait=CMsvOperationWait::NewLC(CActive::EPriorityStandard);
		CMsvOperation* op=iMsvEntry->DeleteL(*aSelection, wait->iStatus);
		CleanupStack::PushL(op);
		wait->Start();
		CActiveScheduler::Start();

		const TInt err=op->iStatus.Int();
		TPushMTMProgressBuf progressBuf;
		progressBuf.Copy(op->ProgressL());
		TPushMTMProgress progress = progressBuf();
		CleanupStack::PopAndDestroy(2); // op, wait

		User::LeaveIfError(err);
		User::LeaveIfError(progress.iError);
    	}
	}

TMsvId CWapPushTestMsgUtils::CreateServiceL()
	{
	_LIT(KServicedescription, "WAP Push Service Entry");
	TMsvId id = 0;
	TMsvEntry entry;
	entry.iMtm = KUidMtmWapPush;
	entry.iType = KUidMsvServiceEntry;
	entry.SetReadOnly(ETrue);
	entry.SetVisible(ETrue);
	entry.iDetails.Set(KServicedescription);
	
	iMsvEntry->SetEntryL(KMsvRootIndexEntryId);
	iMsvEntry->CreateL(entry);
	id = entry.Id();
	iMsvEntry->SetEntryL(id);
	return id;
	}


void CWapPushTestMsgUtils::PushFolderIdL(TMsvId& rFirstFolderId, CMsvEntrySelection* rFolderIds)
	{
    //Returns the Service IDs of MTM aMtm, or Null value if none exist

	rFirstFolderId = KMsvNullIndexEntryId;

	iMsvEntry->SetEntryL(KMsvLocalServiceIndexEntryId);

	TMsvSelectionOrdering order;
	order.SetShowInvisibleEntries(ETrue);
	iMsvEntry->SetSortTypeL(order);

	//Get the children on the Root Index Entry
	CMsvEntrySelection* selection = iMsvEntry->ChildrenWithTypeL(KUidMsvFolderEntry);
	CleanupStack::PushL(selection);

	TInt count = selection->Count();

	//Find an entry for Push MTM 
	for (TInt curChild = 0; curChild < count ; curChild++)
		{
		iMsvEntry->SetEntryL(selection->At(curChild));

		if (iMsvEntry->Entry().iMtm == KUidMtmWapPush)
			{
			TMsvId id = iMsvEntry->Entry().Id();

			if (rFirstFolderId == KMsvNullIndexEntryId)
				rFirstFolderId = id;

			if (rFolderIds)
				rFolderIds->AppendL(id);
			}
		}

	CleanupStack::Pop(); //selection
	delete selection;

	
	
	}

TMsvId CWapPushTestMsgUtils::CreatePushMsgFolderL()
	{
	TMsvId id = 0;
	TMsvEntry entry;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	entry.iMtm = KUidMtmWapPush;
	entry.iType = KUidMsvFolderEntry;
	entry.SetReadOnly(ETrue);
	entry.SetVisible(EFalse);
	entry.iDetails.Set(KPushFolderDescription);
	
	iMsvEntry->SetEntryL(KMsvLocalServiceIndexEntryId);
	iMsvEntry->CreateL(entry);
	id = entry.Id();
	iMsvEntry->SetEntryL(id);
	return id;
	}
	

// Clears any existing Push Service entries - test if creation works
void CWapPushTestMsgUtils::RemoveAllPushServiceEntriesL()
	{
	iMsvEntry->SetEntryL(KMsvRootIndexEntryId);

	TMsvSelectionOrdering order;
	order.SetShowInvisibleEntries(ETrue);
	iMsvEntry->SetSortTypeL(order);

	CMsvEntrySelection* selection = iMsvEntry->ChildrenWithTypeL(KUidMsvServiceEntry);
	CleanupStack::PushL(selection);

	TInt count = selection->Count();
	TMsvEntry entry;

	for (TInt currentEntry = 0; currentEntry < count; currentEntry++)
		{
		iMsvEntry->SetEntryL(selection->At(currentEntry));
		entry = iMsvEntry->Entry();
		if (entry.iMtm == KUidMtmWapPush && entry.iType == KUidMsvServiceEntry)
			{
			iMsvEntry->SetEntryL(KMsvRootIndexEntryId);
			iMsvEntry->DeleteL(entry.Id());
			}
		}
	CleanupStack::PopAndDestroy();//selection
	}


void CWapPushTestMsgUtils::RemoveAllPushFolderEntriesL()
	{
	iMsvEntry->SetEntryL(KMsvLocalServiceIndexEntryId);

	TMsvSelectionOrdering order;
	order.SetShowInvisibleEntries(ETrue);
	iMsvEntry->SetSortTypeL(order);

	CMsvEntrySelection* selection = iMsvEntry->ChildrenWithTypeL(KUidMsvFolderEntry);
	CleanupStack::PushL(selection);

	TInt count = selection->Count();
	TMsvEntry folderEntry;

	for (TInt current = 0; current <count; current++)
		{
		iMsvEntry->SetEntryL(selection->At(current));
		folderEntry = iMsvEntry->Entry();

		if (folderEntry.iMtm == KUidMtmWapPush && folderEntry.iType == KUidMsvFolderEntry)
			{
//			if (iMsvEntry->Entry().ReadOnly())
//			{
//			iMsvEntry->Entry().SetReadOnly(False);
//			iMsvEntry->ChangeL(iMsvEntry->Entry().Id());
//			}
			iMsvEntry->SetEntryL(KMsvLocalServiceIndexEntryId);
			iMsvEntry->DeleteL(folderEntry.Id());
			}
		}
	CleanupStack::PopAndDestroy();
	}
