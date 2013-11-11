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

#include <msvids.h>
#include <msventry.h>
#include "cbulkcommitservermtm.h"
#include <msvuids.h>
#include <tmsvschedulesettingsutils.h>

const TInt KBulkEntriesToCreate=5;

CBulkCommitServerMtm::CBulkCommitServerMtm(CRegisteredMtmDll& aRegisteredMtmDll, CMsvServerEntry* aServerEntry) : CBaseServerMtm(aRegisteredMtmDll, aServerEntry)
	{
	}

CBulkCommitServerMtm::~CBulkCommitServerMtm()
	{	
	}

void CBulkCommitServerMtm::ConstructL()
	{	
	// Get the entry id for the bulkcommit service entry.
	User::LeaveIfError(iServerEntry->SetEntry(KMsvRootIndexEntryId));
	CMsvEntrySelection* sel = new (ELeave) CMsvEntrySelection();
	CleanupStack::PushL(sel);
	User::LeaveIfError(iServerEntry->GetChildrenWithMtm(KUidBulkCommitTestMtm, *sel));
	TInt count = sel->Count();
	if( count > 1 )	// should only be one service entry
		{
		User::Leave(KErrCorrupt);
		}
	if( count == 0 )
		{
		// Create the settings
		TMsvEntry serviceEntry;
		serviceEntry.iType= KUidMsvServiceEntry;
		serviceEntry.iMtm = KUidBulkCommitTestMtm;

		User::LeaveIfError(iServerEntry->CreateEntry(serviceEntry));
		iServiceId = serviceEntry.Id();		
		}
	else
		{
		iServiceId = sel->At(0);
		}
	CleanupStack::PopAndDestroy(sel);	
	}

EXPORT_C CBulkCommitServerMtm* CBulkCommitServerMtm::NewL(CRegisteredMtmDll& aRegisteredMtmDll, CMsvServerEntry& aServerEntry)
	{
	CBulkCommitServerMtm* self = new (ELeave) CBulkCommitServerMtm(aRegisteredMtmDll, &aServerEntry);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}
	
void CBulkCommitServerMtm::CreateBulkEntries(TUint aEntryCount, TMsvId aServiceId)
	{	
	TMsvEntry newEntry;
	for (TInt i = 0; i < aEntryCount; ++i)
		{
		newEntry.iType = KUidMsvMessageEntry;
		newEntry.iMtm = KUidBulkCommitTestMtm;
		newEntry.iServiceId = aServiceId;		
		newEntry.SetVisible(ETrue);
		newEntry.SetReadOnly(EFalse);
		newEntry.SetUnread(ETrue);
		newEntry.iDescription.Set(_L("Bulk Entry"));
		newEntry.SetSendingState(KMsvSendStateNotApplicable);
		
		iServerEntry->CreateEntryBulk(newEntry);	
		}
	}

/*
Creates two folders and then creates bulk entries under each folder.
After bulk commit, creates an entry to let the client know about 
bulk commit is done.
@param aEntryCount - Bulk entries to be created under each root entry
*/
void CBulkCommitServerMtm::CreateBulkEntryWithMultipleParentsL(TUint aEntryCount)
	{	
	iServerEntry->SetEntry(iServiceId);
	
	TMsvEntry folderEntry1, folderEntry2;
	//create the first folder
	folderEntry1.iType=KUidMsvFolderEntry;
	folderEntry1.iMtm = KUidBulkCommitTestMtm;
	folderEntry1.iServiceId = iServiceId;	
	iServerEntry->CreateEntry(folderEntry1);
	
	//create the second folder entry
	folderEntry2.iType=KUidMsvFolderEntry;
	folderEntry2.iMtm = KUidBulkCommitTestMtm;
	folderEntry2.iServiceId = iServiceId;	
	iServerEntry->CreateEntry(folderEntry2);
	
	iServerEntry->SetEntry(folderEntry1.Id());
	//Create a series of entries using the bulk API under the first folder
	CreateBulkEntries(aEntryCount, folderEntry1.Id());
	
	iServerEntry->SetEntry(folderEntry2.Id());
	//Create a series of entries using the bulk API under the second folder
	CreateBulkEntries(aEntryCount, folderEntry2.Id());
	
	//commit the bulk entries
	iServerEntry->CompleteBulk();
	
	iServerEntry->SetEntry(iServiceId);
	//Let the client know that the bulk commit is done
	TMsvEntry childEntry;	
	childEntry.iType = KUidMsvMessageEntry;
	childEntry.iMtm = KUidBulkCommitTestMtm;
	childEntry.iServiceId = iServiceId;		
	childEntry.SetVisible(ETrue);
	childEntry.SetReadOnly(EFalse);
	childEntry.SetUnread(ETrue);
	childEntry.iDescription.Set(_L("Bulk Commit Done"));
	childEntry.SetSendingState(KMsvSendStateNotApplicable);	
	iServerEntry->CreateEntry(childEntry);
	
	//Set entry to NULL entry to avoid locking	
	iServerEntry->SetEntry(KMsvNullIndexEntryId);		
	}
	
const TDesC8& CBulkCommitServerMtm::Progress()
	{
	return iProgress;
	}
	
void CBulkCommitServerMtm::StartCommandL(CMsvEntrySelection& aSelection, TInt aCommand, const TDesC8& aParameter, TRequestStatus& aStatus)
	{
	iProgress().SetCommand(aCommand);
	const TMsvId id = iServerEntry->Entry().Id();	
	
	TRAP(iProgress().iError, DoStartCommandL(aSelection, iProgress().Command(), aParameter, aStatus));
	
	TRequestStatus* bStatus = &aStatus;
	User::RequestComplete(bStatus, iProgress().iError);
	iServerEntry->SetEntry(id); 
	}

void CBulkCommitServerMtm::DoStartCommandL(CMsvEntrySelection& /*aSelection*/, TBulkCommitTestOperation aCommand, const TDesC8& /*aParameter*/, TRequestStatus& /*aStatus*/)
	{
	switch(aCommand)
		{
		case ECreateBulk:
			{
			//Create bulk entries				
			CreateBulkEntryWithMultipleParentsL(KBulkEntriesToCreate);
			} 
			break;		
		default:
			User::Panic(_L("BulkCommit server"), 1);
			break;
		}
	}

void CBulkCommitServerMtm::CopyToLocalL(const CMsvEntrySelection& , long , TRequestStatus& )
	{
	}
	
void CBulkCommitServerMtm::CopyFromLocalL(const CMsvEntrySelection& /*aSelection*/, long /*aLong*/, TRequestStatus& /*aStatus*/)
	{
	}

void CBulkCommitServerMtm::CopyWithinServiceL(const CMsvEntrySelection& /*aSelection*/, long /*aLong*/, TRequestStatus& /*aStatus*/)
	{
	}

void CBulkCommitServerMtm::DeleteAllL(const CMsvEntrySelection& /*aSelection*/, TRequestStatus& /*aStatus*/)
	{
	}

void CBulkCommitServerMtm::CreateL(TMsvEntry /*aEntry*/, TRequestStatus& /*aStatus*/)
	{
	}
	
void CBulkCommitServerMtm::ChangeL(TMsvEntry /*aEntry*/, TRequestStatus& /*aStatus*/)
	{
	}
	
TBool CBulkCommitServerMtm::CommandExpected()
	{
	return EFalse;
	}
		
void CBulkCommitServerMtm::MoveToLocalL(const CMsvEntrySelection& /*aSelection*/, long /*aLong*/, TRequestStatus& /*aStatus*/)
	{
	}
	
void CBulkCommitServerMtm::MoveFromLocalL(const CMsvEntrySelection& /*aSelection*/, long /*aLong*/, TRequestStatus& /*aStatus*/)
	{
	}

void CBulkCommitServerMtm::MoveWithinServiceL(const CMsvEntrySelection& /*aSelection*/, long /*aLong*/, TRequestStatus& /*aStatus*/)
	{
	}

void CBulkCommitServerMtm::DoCancel()
	{
	}
	
void CBulkCommitServerMtm::DoRunL()
	{
	}

void CBulkCommitServerMtm::DoComplete(int /*i*/)
	{
	}
