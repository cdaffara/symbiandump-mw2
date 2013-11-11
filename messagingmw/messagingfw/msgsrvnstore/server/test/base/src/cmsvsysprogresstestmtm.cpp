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
// MSVSYSPROGRESSTESTMTM.cpp
// 
//

#include <msvids.h>
#include <msventry.h>
#include "cmsvsysprogresstestmtm.h"
#include <msvuids.h>
#include <tmsvschedulesettingsutils.h>
#include <tmsvsystemprogress.h>

const TInt KBulkEntriesToCreate=5;

CMsvSysProgressTestMtm::CMsvSysProgressTestMtm(CRegisteredMtmDll& aRegisteredMtmDll, CMsvServerEntry* aServerEntry) : CBaseServerMtm(aRegisteredMtmDll, aServerEntry)
	{
	}

CMsvSysProgressTestMtm::~CMsvSysProgressTestMtm()
	{	
	}

void CMsvSysProgressTestMtm::ConstructL()
	{	
	// Get the entry id for the bulkcommit service entry.
	User::LeaveIfError(iServerEntry->SetEntry(KMsvRootIndexEntryId));
	CMsvEntrySelection* sel = new (ELeave) CMsvEntrySelection();
	CleanupStack::PushL(sel);
	User::LeaveIfError(iServerEntry->GetChildrenWithMtm(KUidSysProgressTestMtm, *sel));
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
		serviceEntry.iMtm = KUidSysProgressTestMtm;

		User::LeaveIfError(iServerEntry->CreateEntry(serviceEntry));
		iServiceId = serviceEntry.Id();		
		}
	else
		{
		iServiceId = sel->At(0);
		}
	CleanupStack::PopAndDestroy(sel);	
	}

EXPORT_C CMsvSysProgressTestMtm* CMsvSysProgressTestMtm::NewL(CRegisteredMtmDll& aRegisteredMtmDll, CMsvServerEntry& aServerEntry)
	{
	CMsvSysProgressTestMtm* self = new (ELeave) CMsvSysProgressTestMtm(aRegisteredMtmDll, &aServerEntry);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}
	
void CMsvSysProgressTestMtm::CreateBulkEntries(TUint aEntryCount, TMsvId aServiceId)
	{	
	TMsvEntry newEntry;
	for (TInt i = 0; i < aEntryCount; ++i)
		{
		newEntry.iType = KUidMsvMessageEntry;
		newEntry.iMtm = KUidSysProgressTestMtm;
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
Creates two folders and then creates entries under each folder.
After System Progress test mtm, creates an entry to let the client know about 
bulk commit is done.
@param aEntryCount - Bulk entries to be created under each root entry
*/
void CMsvSysProgressTestMtm::CreateBulkEntryWithMultipleParentsL(TUint aEntryCount)
	{	
	iServerEntry->SetEntry(iServiceId);
	
	TMsvEntry folderEntry1, folderEntry2;
	//create the first folder
	folderEntry1.iType=KUidMsvFolderEntry;
	folderEntry1.iMtm = KUidSysProgressTestMtm;
	folderEntry1.iServiceId = iServiceId;	
	iServerEntry->CreateEntry(folderEntry1);
	
	//create the second folder entry
	folderEntry2.iType=KUidMsvFolderEntry;
	folderEntry2.iMtm = KUidSysProgressTestMtm;
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
	//Let the client know that the System Progress test mtm is done
	TMsvEntry childEntry;	
	childEntry.iType = KUidMsvMessageEntry;
	childEntry.iMtm = KUidSysProgressTestMtm;
	childEntry.iServiceId = iServiceId;		
	childEntry.SetVisible(ETrue);
	childEntry.SetReadOnly(EFalse);
	childEntry.SetUnread(ETrue);
	childEntry.iDescription.Set(_L("SysProgress testMtm Done"));
	childEntry.SetSendingState(KMsvSendStateNotApplicable);	
	iServerEntry->CreateEntry(childEntry);
	
	//Set entry to NULL entry to avoid locking	
	iServerEntry->SetEntry(KMsvNullIndexEntryId);		
	}
	
const TDesC8& CMsvSysProgressTestMtm::Progress()
	{
	return iProgress;
	}
	
void CMsvSysProgressTestMtm::GetSystemProgress(TMsvSystemProgress& aOutSysProg)
	{
	aOutSysProg.iErrorCode = 123456;
	}

EXPORT_C TInt CMsvSysProgressTestMtm::Extension_(TUint aExtensionId, TAny *&a0, TAny *a1)	
	{
	if (aExtensionId == KUIDMsgMsvSystemProgress)
		{
		TMsvSystemProgress* systemProgress = reinterpret_cast<TMsvSystemProgress*>(a1);
		GetSystemProgress(*systemProgress);
		return KErrNone;
		}
	
	// Chain to base class
	return CBaseServerMtm::Extension_(aExtensionId, a0, a1);
	}
	
void CMsvSysProgressTestMtm::StartCommandL(CMsvEntrySelection& aSelection, TInt aCommand, const TDesC8& aParameter, TRequestStatus& aStatus)
	{
//	iProgress().SetCommand(aCommand);
	const TMsvId id = iServerEntry->Entry().Id();	
	
	TRAPD(err, DoStartCommandL(aSelection, aCommand, aParameter, aStatus));
	iSysProgress.iErrorCode = err;
	
	TRequestStatus* bStatus = &aStatus;
	User::RequestComplete(bStatus, iSysProgress.iErrorCode);
	iServerEntry->SetEntry(id); 
	}

void CMsvSysProgressTestMtm::DoStartCommandL(CMsvEntrySelection& /*aSelection*/, TInt aCommand, const TDesC8& /*aParameter*/, TRequestStatus& /*aStatus*/)
	{
	switch(aCommand)
		{
		case 0:
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

void CMsvSysProgressTestMtm::CopyToLocalL(const CMsvEntrySelection& , long , TRequestStatus& )
	{
	}
	
void CMsvSysProgressTestMtm::CopyFromLocalL(const CMsvEntrySelection& /*aSelection*/, long /*aLong*/, TRequestStatus& /*aStatus*/)
	{
	}

void CMsvSysProgressTestMtm::CopyWithinServiceL(const CMsvEntrySelection& /*aSelection*/, long /*aLong*/, TRequestStatus& /*aStatus*/)
	{
	}

void CMsvSysProgressTestMtm::DeleteAllL(const CMsvEntrySelection& /*aSelection*/, TRequestStatus& /*aStatus*/)
	{
	}

void CMsvSysProgressTestMtm::CreateL(TMsvEntry /*aEntry*/, TRequestStatus& /*aStatus*/)
	{
	}
	
void CMsvSysProgressTestMtm::ChangeL(TMsvEntry /*aEntry*/, TRequestStatus& /*aStatus*/)
	{
	}
	
TBool CMsvSysProgressTestMtm::CommandExpected()
	{
	return EFalse;
	}
		
void CMsvSysProgressTestMtm::MoveToLocalL(const CMsvEntrySelection& /*aSelection*/, long /*aLong*/, TRequestStatus& /*aStatus*/)
	{
	}
	
void CMsvSysProgressTestMtm::MoveFromLocalL(const CMsvEntrySelection& /*aSelection*/, long /*aLong*/, TRequestStatus& /*aStatus*/)
	{
	}

void CMsvSysProgressTestMtm::MoveWithinServiceL(const CMsvEntrySelection& /*aSelection*/, long /*aLong*/, TRequestStatus& /*aStatus*/)
	{
	}

void CMsvSysProgressTestMtm::DoCancel()
	{
	}
	
void CMsvSysProgressTestMtm::DoRunL()
	{
	}

void CMsvSysProgressTestMtm::DoComplete(int /*i*/)
	{
	}
