// Copyright (c) 1998-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "MSVIDS.H"
#include "MSVCOPY.H"
#include "MSVSERV.H"
#include "MSVUTILS.H"

#include "MSVPANIC.H"

#include "CCopyOneFile.h"
#include "CCopyFiles.h"
#include "msvindexadapter.h"
#include "msvcacheentry.h"

//**********************************
// CMsvCopyEntry
//**********************************

// static
CMsvCopyEntry* CMsvCopyEntry::NewL(CMsvServer& aServer)
	{
	CMsvCopyEntry* self = new(ELeave) CMsvCopyEntry(aServer);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}


CMsvCopyEntry::CMsvCopyEntry(CMsvServer& aServer)
: CActive(EPriorityStandard), iServer(aServer)
	{}


CMsvCopyEntry::~CMsvCopyEntry()
	{
	Cancel();
	delete iCopyOneFile;
	delete iCopyFiles;
	delete iFromName;
	delete iToName;
	}

void CMsvCopyEntry::ConstructL()
	{
	iCopyOneFile = CCopyOneFile::NewL(iServer.FileSession());
	iCopyFiles = CCopyFiles::NewL(iServer.FileSession());
	iFromName= new (ELeave) TFileName;
	iToName= new (ELeave) TFileName;

	CActiveScheduler::Add(this);
	}


void CMsvCopyEntry::DoCancel()
	{
	// only one will be active but that doesn't matter
	iCopyOneFile->Cancel();
	iCopyFiles->Cancel();
	Completed(KErrCancel);
	}


void CMsvCopyEntry::NextState()
	{
	switch(iState)
		{
		case EEntryCreated:
			iState=ECopyingStore;
			break;
		case ECopyingStore:
			iState=ECopyingFolder;
			break;
		case ECopyingFolder:
			iState=ECompleted;
			break;
		default:
			__ASSERT_DEBUG(EFalse,PanicServer(EMsvCopyEntryFailureBadState));
			iState=ECompleted;
			break;
		}
	}


void CMsvCopyEntry::RunL()
	{
	TInt error=iStatus.Int();
	if(error==KErrNone)
		{
		NextState();

		switch(iState)
			{
			case ECopyingStore:
				CopyStoreFile();
				break;
			case ECopyingFolder:
				CopyFolder();
				break;
			case ECompleted:
				Completed(KErrNone);
				break;
			default:
				__ASSERT_DEBUG(EFalse,PanicServer(EMsvCopyEntryFailureBadState));
				Completed(KErrNone);
				break;
			}
		}
	else
		Completed(error);
	}


void CMsvCopyEntry::CopyStoreFile()
	{
	iServer.GetEntryName(iOrigEntryId, *iFromName, EFalse);
	TUint unused;
	TInt err = iServer.FileSession().Att(*iFromName, unused);
	if (err == KErrNotFound || err == KErrPathNotFound)
		{
		TRequestStatus* status = &iStatus;
		TInt ret = KErrNone;
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
		// We know that there's no entry in the file store. Let's see if this
		// really is an error or if there's an entry in the DB store.
		TRAPD(dbErr, iServer.MessageDBAdapter().CopyHeaderEntryL(iOrigEntry->iMtm, iOrigEntryId, iNewEntry->Id()));
		if(KErrNotFound == dbErr) // The entry didn't have a header table entry.
			dbErr = KErrNone;
		ret = dbErr;
#endif
		User::RequestComplete(status, ret);
		}
	else
		{
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
		// There is an entry for iOrigEntry in the file store. Copy header
		// table entries if any.
		TRAPD(dbErr, iServer.MessageDBAdapter().CopyHeaderEntryL(iOrigEntry->iMtm, iOrigEntryId, iNewEntry->Id()));
		if(KErrNotFound == dbErr) // The entry didn't have a header table entry.
			dbErr = KErrNone;
		
		if(dbErr) // There was infact an error thrown from the DB - self-complete with that.
			{
			TRequestStatus *status=&iStatus;
			User::RequestComplete(status,dbErr);
			SetActive();
			return;
			}
		// There was no error thrown from the DB. Go on to copy file store entries.
#endif
		iServer.GetEntryName(iNewEntry->Id(), *iToName, EFalse);
		TInt error=iServer.FileSession().MkDirAll(*iToName);
		if(error!=KErrNone && error != KErrAlreadyExists)
			{
			TRequestStatus *status=&iStatus;
			User::RequestComplete(status,error);
			}
		else
			{
			iCopyOneFile->Copy(*iFromName,*iToName,iStatus);
			}
		}
	SetActive();
	}


void CMsvCopyEntry::CopyFolder()
	{
	TMsvId service;
	iServer.IndexAdapter().OwningService(iOrigEntryId, service); // error ignored because we know the original exists
	TInt result = MsvUtils::HasDirectory(iServer.FileSession(), iServer.Context().MessageFolder(), service, iOrigEntryId);
	if (result < 0)
		Completed(result);
	else if(result)
		{
		iServer.GetEntryName(iOrigEntryId, *iFromName, ETrue);
		iServer.GetEntryName(iNewEntry->Id(), *iToName, ETrue);
		iCopyFiles->CopyDir(*iFromName,*iToName,iStatus);
		SetActive();
		}
	else
		{
		TRequestStatus *status=&iStatus;
		User::RequestComplete(status,KErrNone);
		SetActive();
		}
	}


void CMsvCopyEntry::StartL(TMsvId aOrigEntryId, TMsvId aTargetId, TRequestStatus& aObserverStatus)
//
//
//
	{
	// Fail now if the index says it's not available
	User::LeaveIfError(iServer.IndexAdapter().ErrorState());
	__ASSERT_DEBUG(!IsActive() && (iState==ENotYetStarted || iState==ECompleted), PanicServer(EMsvActiveCopyEntryReset));
	iState = ENotYetStarted;
	iOrigEntryId = aOrigEntryId;

#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	// Copying of entries across drives not supported.
	if(GetDriveId(aOrigEntryId) != GetDriveId(aTargetId))
		{
		User::Leave(KErrNotSupported);
		}
#endif		// #if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)

	// get the complete index entry to copy
	TMsvEntry* entryPtr;
	TSecureId ownerId;

#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	User::LeaveIfError(iServer.IndexAdapter().GetEntry(iOrigEntryId, iOrigEntry, ownerId));
	entryPtr = iOrigEntry;
#else	
	User::LeaveIfError(iServer.IndexAdapter().GetEntry(iOrigEntryId, entryPtr, ownerId));
#endif

	// create a new entry with the new parent, and add the new entry to the index
	TMsvEntry newEntry = *entryPtr;
	newEntry.SetParent(aTargetId);
	User::LeaveIfError(iServer.IndexAdapter().AddEntry(newEntry, ownerId, ETrue));

	// get the new entry from  the index
	iServer.IndexAdapter().GetEntry(newEntry.Id(), iNewEntry); // will not fail, the entry exists
	iState = EEntryCreated;

	iObserverStatus = &aObserverStatus;
	*iObserverStatus = KRequestPending;

	TRequestStatus *status=&iStatus;
	User::RequestComplete(status,KErrNone);
	SetActive();
	}


void CMsvCopyEntry::Completed(TInt aError)
	{
	// remove the entry if we failed
	if(aError!=KErrNone && iState!=ENotYetStarted)
		iServer.RemoveEntry(iNewEntry->Id());

	// inform the observer
	User::RequestComplete(iObserverStatus, aError);
	}


//**********************************
// CMsvCopy
//**********************************

// static
CMsvCopy* CMsvCopy::NewL(CMsvServer& aServer)
//
//
//
	{
	CMsvCopy* self = new(ELeave) CMsvCopy(aServer);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}


CMsvCopy::CMsvCopy(CMsvServer& aServer)
: CActive(EPriorityStandard), iServer(aServer)
//
//
//
	{}


CMsvCopy::~CMsvCopy()
//
//
//
	{
	Cancel();
	delete iRecursionList;
	delete iFailedEntryParents;
	delete iCopyEntry;
	}


void CMsvCopy::ConstructL()
//
//
//
	{
	iRecursionList = new(ELeave) CMsvEntrySelection;
	iFailedEntryParents = new(ELeave) CMsvEntrySelection;
	iCopyEntry = CMsvCopyEntry::NewL(iServer);
	CActiveScheduler::Add(this);
	}


void CMsvCopy::DoCancel()
//
//
//
	{
	iCopyEntry->Cancel();
	Completed();
	}


void CMsvCopy::StartL(TMsvId aOrigEntryId, TMsvId aTargetId, TRequestStatus& aObserverStatus, TBool aDescendentCopying)
//
//
//
	{
	// Fail now if the index says it's not available
	User::LeaveIfError(iServer.IndexAdapter().ErrorState());
	iFailedEntryParents->Reset();
	iDescendentCopying = aDescendentCopying;
	iDescendentId = KMsvNullIndexEntryId;

#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	// Copying of entries across drives not supported.
	if(GetDriveId(aOrigEntryId) != GetDriveId(aTargetId))
		{
		User::Leave(KErrNotSupported);
		}
#endif		// #if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)

	// check the target exists
	TBool entryExists = EFalse;
	entryExists = iServer.IndexAdapter().EntryExists(aTargetId);
	if (!entryExists)
		User::Leave(KErrNotFound);
	iOrigTargetId = aTargetId;

	// get the index entry
	iOrigSourceId = aOrigEntryId;
	TMsvEntry* entryPtr;
	User::LeaveIfError(iServer.IndexAdapter().GetEntry(iOrigSourceId, entryPtr));
	iFailedEntryParents->AppendL(iOrigSourceId);

	// check the entries can be copied
	iServer.IndexAdapter().ExpandSelectionRecursively(*iFailedEntryParents);
	User::LeaveIfError(iServer.CheckEntries(*iFailedEntryParents));
	iFailedEntryParents->ResizeL(0); // the memory is not freed, so all the entries can be added again

	// set the recursion variables
	iError = KErrNone;
	iTopLevelId = KMsvNullIndexEntryId;
	iTempSourceId = iOrigSourceId;
	iTempTargetId = iOrigTargetId;

	// start the recursive copy
	iCopyEntry->StartL(iOrigSourceId, iOrigTargetId, iStatus);
	SetActive();

	iObserverStatus = &aObserverStatus;
	*iObserverStatus = KRequestPending;
	}


void CMsvCopy::RunL()
//
//
//
	{
	// was this the top level
	TBool topLevel = (iTopLevelId == KMsvNullIndexEntryId);
	if (topLevel)
		{
		if (iStatus!=KErrNone)
			{
			User::RequestComplete(iObserverStatus, iStatus.Int());
			return;
			}
		iTopLevelId = iCopyEntry->NewEntryId();
		if (iDescendentCopying && iDescendentId==KMsvNullIndexEntryId)
			iDescendentId = iTopLevelId;
		}

	// copy next entry
	TRAPD(leave, DoRunL(topLevel));
	if (leave)
		{
		// system error failure rather than couldn't copy a locked entry, so completed
		if (iFailedEntryParents->Find(iTempTargetId)==KErrNotFound)
			iFailedEntryParents->AppendL(iTempTargetId); // will not fail as space has been reserved
		if (iError==KErrNone)
			iError=leave;
		Completed();
		}
	}


void CMsvCopy::DoRunL(TBool aTopLevel)
//
//
//
	{
	// record the success/failure of the copy
	TMsvId newEntryId = KMsvNullIndexEntryId;
	TBool  failed = (iStatus!=KErrNone);
	if (failed)
		{
		if (iError==KErrNone)
			iError = iStatus.Int();
		iFailedEntryParents->AppendL(iTempTargetId); // a child of this entry was not copied
		}
	else
		newEntryId = iCopyEntry->NewEntryId();

	// get the  entry
	
	CMsvCacheEntry* sEntry;
	User::LeaveIfError(iServer.IndexAdapter().GetInternalEntry(iTempSourceId, sEntry));

	TMsvId firstSiblingId;
    if(!aTopLevel && iServer.IndexAdapter().GetNextSiblingL(iTempSourceId,sEntry->Entry().Parent(),firstSiblingId)) 
    	{
    	iRecursionList->AppendL(firstSiblingId);
        iRecursionList->AppendL(iTempTargetId);
    	}
  
  	TMsvId firstChildId;
  	if (!failed && iServer.IndexAdapter().GetFirstChildIdL(iTempSourceId,firstChildId)
  		&& (!iDescendentCopying || firstChildId<iDescendentId))
  		{
        iRecursionList->AppendL(firstChildId);
        iRecursionList->AppendL(newEntryId);
        }
     
	if (iRecursionList->Count()==0)
		{
		// no more to copy
		Completed();
		return;
		}
	// get the next one to copy
	iTempSourceId = iRecursionList->At(0);
	iTempTargetId = iRecursionList->At(1);
	iRecursionList->Delete(0,2);

	// start the copy
	iCopyEntry->StartL(iTempSourceId, iTempTargetId, iStatus);
	SetActive();
	}


void CMsvCopy::Completed()
//
//
//
	{
	if (iError)
		{
		if (iDeleteAllOnFailure)
			iServer.RemoveEntry(iTopLevelId);
		else
			{
			__ASSERT_DEBUG(iFailedEntryParents->Count(), PanicServer(EMsvCopyErrorButNoIds));
			// cleanup the code
			TInt index=iFailedEntryParents->Count();
			while (index--)
				{
				TMsvId id = iFailedEntryParents->At(index);
				TBool entryExists = EFalse;
				entryExists = iServer.IndexAdapter().EntryExists(id);
				if (!entryExists)
					break;

				// recurse back up tree to find the top entry or folder/service
				TMsvEntry* entry;
				TMsvEntry* parent;
				iServer.IndexAdapter().GetEntry(id, entry); // errror ignored as entry exists
				iServer.IndexAdapter().GetEntry(entry->Parent(), parent); // errror ignored as entry exists
				while (entry->Id()!=iTopLevelId && (parent->iType!=KUidMsvFolderEntry || parent->iType!=KUidMsvServiceEntry))
					{
					entry = parent;
					iServer.IndexAdapter().GetEntry(entry->Parent(), parent); // error ignored as entry exists
					}
				// remove the entries
				iServer.RemoveEntry(entry->Id());
				}
			}
		}

	// inform the observer
	User::RequestComplete(iObserverStatus, iError);
	}

//**********************************
// CMsvCopyEntries
//**********************************

// static
CMsvCopyEntries* CMsvCopyEntries::NewL(CMsvServer& aServer)
//
//
//
	{
	CMsvCopyEntries* self = new(ELeave) CMsvCopyEntries();
	CleanupStack::PushL(self);
	self->ConstructL(aServer);
	CleanupStack::Pop();
	return self;
	}


CMsvCopyEntries::CMsvCopyEntries() :
	CMsvCopyMoveEntriesBase()
//
//
//
	{}


CMsvCopyEntries::~CMsvCopyEntries()
//
//
//
	{
	delete iCopy;
	delete iNewEntries;
	}


void CMsvCopyEntries::ConstructL(CMsvServer& aServer)
//
//
//
	{
	iCopy = CMsvCopy::NewL(aServer);
	iCopy->DeleteAllOnFailure();
	iNewEntries = new(ELeave) CMsvEntrySelection;

	CMsvCopyMoveEntriesBase::ConstructL();
	}


void CMsvCopyEntries::DoCancel()
//
//
//
	{
	iCopy->Cancel();
	User::RequestComplete(iObserverStatus, KErrCancel);
	}


void CMsvCopyEntries::DoStartL(TMsvId aSourceId, TMsvId aTargetId, TRequestStatus& aObserverStatus)
//
//
//
	{
	// Reserve space for new entry id
	iNewEntries->SetReserveL(CompletedIds().Count() + 1);

	// Perform the copy
	iCopy->StartL(aSourceId, aTargetId, aObserverStatus, ETrue);
	}


void CMsvCopyEntries::RunL()
	{
	// If no error occured remember the new entry's id
	if (iStatus == KErrNone)
		iNewEntries->AppendL(iCopy->NewEntryId());

	CMsvCopyMoveEntriesBase::RunL();
	}
