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

#include "MSVDELET.H"
#include "MSVSERV.H"
#include "MSVPANIC.H"
#include "MSVUTILS.H"
#include "msvindexadapter.h"
#include "msventryfreepool.h"

// static
CMsvDelete* CMsvDelete::NewL(CMsvServer& aServer)
//
//
//
	{
	CMsvDelete* self = new(ELeave) CMsvDelete(aServer);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}


CMsvDelete::CMsvDelete(CMsvServer& aServer)
: CActive(EPriorityStandard), iServer(aServer), iLockedIndex(-2)
//
//
//
	{
	}


void CMsvDelete::ConstructL()
//
//
//
	{
	iFileMan = CFileMan::NewL(iServer.FileSession());
	iDescendents = new(ELeave)CMsvEntrySelection;
	CActiveScheduler::Add(this);
	}


CMsvDelete::~CMsvDelete()
//
//
//
	{
	Cancel();
	delete iDescendents;
	delete iFileMan;
	iDeleteArray.Close();
	}

void CMsvDelete::StartL(TMsvId aId, CMsvEntrySelection& aDeletedEntries, CMsvEntrySelection& aMovedEntries, TRequestStatus& aObserverStatus, TBool aPCSyncOverride)
//
//
//
	{
	DoStartL(aId, aDeletedEntries, aMovedEntries, aPCSyncOverride);

	iStatus = KRequestPending;
	TRequestStatus* st=&iStatus;
	User::RequestComplete(st, KErrNone);
	SetActive();
	//
	iObserverStatus = &aObserverStatus;
	*iObserverStatus = KRequestPending;
	}

void CMsvDelete::StartL(TMsvId aId, CMsvEntrySelection& aDeletedEntries, CMsvEntrySelection& aMovedEntries, TBool aPCSyncOverride)
//
//
//
	{	
	DoStartL(aId, aDeletedEntries, aMovedEntries, aPCSyncOverride);

	// the state machine from RunL
	while (iState!=ECompleted)
		{
		switch (iState)
			{
			case ECheck:
				TRAP(iError, CheckEntriesL());
				if (iError)
					iState=ECompleted;
				break;
			case EFiles:
				TRAPD(leave, DeleteFilesL());
				if (leave)
					{
					iError=leave;
					iState=ECompleted;
					}
				break;
			case EIndex:
				DeleteAllIndexEntries();
				iState=ECompleted;
				break;
			case EIndexIndividually:
				DeleteIndividualIndexEntries();
				iState=ECompleted;
				break;
			default:
				__ASSERT_DEBUG(EFalse, PanicServer(EMsvDeleteBadState2));
			}
		}

	// completion code
	iDeletionIndex=0;

	User::LeaveIfError(iError);
	}

void CMsvDelete::DoStartL(TMsvId aId, CMsvEntrySelection& aDeletedEntries, CMsvEntrySelection& aMovedEntries, TBool aPCSyncOverride)
//
//
//
	{
	// Fail now if the index says it's not available
	User::LeaveIfError(iServer.IndexAdapter().ErrorState());
	iId = aId;
	iPCSyncOverride = aPCSyncOverride;

	// Let's not support deletion of entries from non-current drive.
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	if(GetDriveId(iId) != KCurrentDriveId)
		{
		User::Leave(KErrNotSupported);
		}
#endif

	// get the expanded selection
	iDescendents->Reset();
	iDescendents->AppendL(iId);
	User::LeaveIfError(iServer.IndexAdapter().ExpandSelectionRecursively(*iDescendents));
	// make sure all deleted entries can be stored
	iDeletedEntries = &aDeletedEntries;
	iDeletedEntries->SetReserveL(iDescendents->Count());
	iMovedEntries = &aMovedEntries;
	iMovedEntries->SetReserveL(iDescendents->Count());

	// check if the entry is a service then set up PC override
	TMsvEntry* entry;
	User::LeaveIfError(iServer.IndexAdapter().GetEntry(aId, entry));
	if (entry->iType==KUidMsvServiceEntry)
		iPCSyncOverride=ETrue;

	iState = ECheck;
	}

void CMsvDelete::DoCancel()
//
//
//
	{
	// need to remove the index entries for any already deleted
	if (iState==EFiles || iState==EIndex || iState==EIndexIndividually)
		iPCSynced ? DeleteIndividualIndexEntries() : DeleteAllIndexEntries();
	Complete(KErrCancel);
	}



void CMsvDelete::RunL()
//
//
//
	{
	TInt error = iStatus.Int();
	if (error==KErrNone)
		TRAP(error, DoRunL());
	if (error)
		Complete(error);
	}


void CMsvDelete::DoRunL()
//
//
//
	{
	switch (iState)
		{
		case EFiles:
			DeleteFilesL();
			break;
		case ECheck:
			CheckEntriesL();
			break;
		case EIndex:
			DeleteAllIndexEntries();
			Complete(iError);
			return;
		case EIndexIndividually:
			DeleteIndividualIndexEntries();
			Complete(iError);
			return;
		default:
			__ASSERT_DEBUG(EFalse, PanicServer(EMsvDeleteBadState));
		}
	iStatus = KRequestPending;
	TRequestStatus* st=&iStatus;
	User::RequestComplete(st, KErrNone);
	SetActive();
	}


void CMsvDelete::CheckEntriesL()
//
//
//
	{
	iLockedIndex = iDescendents->Count();
	while (iLockedIndex)
		{
		TMsvId id = iDescendents->At(iLockedIndex-1);

		// lock the entry
		User::LeaveIfError(iServer.IndexAdapter().IsEntryOrStoreLocked(id));
		iLockedIndex--;

		// check noone is reading the store (reading the store now doesn't
		// keep the file open, therefore we can't rely on checking the file to stop
		// deleting while reading
		TBool reading=EFalse;
		User::LeaveIfError(iServer.IndexAdapter().IsStoreReadingLocked(id,reading));
		if(reading) User::Leave(KErrInUse);


		// get the entry
		TMsvEntry entry;
		User::LeaveIfError(iServer.IndexAdapter().GetEntryNoCache(id, &entry));
		TMsvDelete deleteEntry(entry.Id(), entry.Owner(), entry.PcSyncCount(), entry.iType, entry.Complete());
		iDeleteArray.AppendL(deleteEntry);

		// check if any entries have been synced and not been overridden
		if (!iPCSyncOverride && entry.PcSyncCount())
			iPCSynced=ETrue;

		// cannot delete standard folders
		if (entry.StandardFolder())
			User::Leave(KErrAccessDenied);

		// check the store
		TFileName filename;
		iServer.GetEntryName(id, filename, EFalse);
		TBool open;
		TInt error = iServer.FileSession().IsFileOpen(filename, open);
		if (error!=KErrNone && error!=KErrNotFound && error!=KErrPathNotFound)
			User::Leave(error);
		if (error==KErrNone && open)
			User::Leave(KErrInUse);

		// check any files
		CDir* dir;
		error = iServer.GetFileDirectoryListing(id, filename, dir);
		if (error==KErrNone)
			{
			CleanupStack::PushL(dir);
			User::LeaveIfError(iServer.FileSession().SetSessionPath(filename));
			TInt fCount=dir->Count();
			if (fCount--)
				{
				TBool open;
				TInt error = iServer.FileSession().IsFileOpen((*dir)[fCount].iName, open);
				if (error!=KErrNone && error!=KErrNotFound && error!=KErrPathNotFound)
					User::Leave(error);
				if (error==KErrNone && open)
					User::Leave(KErrInUse);
				if ((*dir)[fCount].IsReadOnly())
					User::LeaveIfError(iServer.FileSession().SetAtt((*dir)[fCount].iName, 0, KEntryAttReadOnly));
				}
			CleanupStack::PopAndDestroy(); // dir
			}
		else if (error!=KErrPathNotFound)
			User::Leave(error);
		}
	iState = EFiles;
	}


void CMsvDelete::DeleteFilesL()
//
//
//
	{
	TFileName filename;
	TMsvId id = iDescendents->At(iDeletionIndex++);

	// delete the binary files
	CDir* dir=NULL;
	TBool partiallyDeleted=EFalse;
	TInt error = iServer.GetFileDirectoryListing(id, filename, dir);
	if(dir)
        {
	    CleanupStack::PushL(dir);
        }
	
	if (error==KErrNone)
		{
		User::LeaveIfError(iServer.FileSession().SetSessionPath(filename));
		// remove any files
		TInt fCount=dir->Count();
		while (fCount--)
			{
			error = iServer.FileSession().Delete((*dir)[fCount].iName);
			if (error==KErrNone)
				partiallyDeleted=ETrue;
			else if (error!=KErrNotFound && error!=KErrPathNotFound)
				goto failed;
			}

		// remove the directory
		error = iServer.FileSession().RmDir(filename);
		if (error==KErrNone)
			partiallyDeleted=ETrue;
		else if (error!=KErrNotFound && error!=KErrPathNotFound)
			goto failed;
		}
	else if (error!=KErrPathNotFound && error!=KErrNotFound)
		goto failed;

	// delete the store
	iServer.GetEntryName(id, filename, EFalse); // error ignore as entry exists
	error = iServer.FileSession().Delete(filename);
	// Try to delete the (single digit) parent folder if it is empty. E.g.: "...\00001001_S\9\"
	iServer.FileSession().RmDir(filename); // ignore error
	if (error==KErrNone)
		partiallyDeleted=ETrue;
	else if (error!=KErrNotFound && error!=KErrPathNotFound)
		goto failed;


	filename.Append(KMsvUtilsNewExtension);  // try and make sure that a temporary store file
	iServer.FileSession().Delete(filename);  // hasn't been left behind by CMsvCachedStore
	                                         // but ignore the error as we can't do anything about it

	// for the last entry, check if it a service and move to next state
	if (iDeletionIndex==iDescendents->Count())
		{
		// get the entry and check whether it is a service
		TBool serviceType =EFalse;
		TInt deleteArrayCount = iDeleteArray.Count();
		for(TInt i =0; i < deleteArrayCount; ++i)
			{
			if(iDeleteArray[i].iEntryId == id)
				{
				if(iDeleteArray[i].iType == KUidMsvServiceEntry)
					{
					serviceType = ETrue;
					}
				break;	
				}
			}	
		if(serviceType)
			{
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			iServer.Context().MessageFolder(GetDriveId(id), filename);
			MsvUtils::ConstructEntryName(UnmaskTMsvId(id), id, filename, MsvUtils::EPath);
#else
			filename = iServer.Context().MessageFolder();
			MsvUtils::ConstructEntryName(id, id, filename, MsvUtils::EPath);
#endif
			error = iFileMan->RmDir(filename);
			if (error && error!=KErrPathNotFound && error!=KErrNotFound)
				goto failed;
			}
		// we have completed all the deletion, move onto next state
		iState = iPCSynced ? EIndexIndividually : EIndex;
		}

	// deletion was successful
	iDeletedEntries->AppendL(id); // will not leave, space has been reserved
	iMovedEntries->AppendL(id);   // will not leave, space has been reserved
	if(dir)
        {
	    CleanupStack::PopAndDestroy(dir);
        }
	return;

failed:
	if (partiallyDeleted)
		{
		TMsvEntry* entry=NULL;
		TBool complete =EFalse;
		for(TInt i =0; i< iDeleteArray.Count(); ++i)
			{
			if(iDeleteArray[i].iEntryId == id)
				{
				if(iDeleteArray[i].iIsComplete)
					{
					iServer.IndexAdapter().GetEntry(id, entry);
					if(entry)
						{
						complete = ETrue;
						}
					}
				break;	
				}
			}
			
		if(complete)
			{
			// mark as imcomplete
			TMsvEntry icmpEntry=*entry;
			icmpEntry.SetComplete(EFalse);
			iServer.IndexAdapter().LockEntry(id);
			iServer.IndexAdapter().ChangeEntry(icmpEntry, KMsvServerId, EFalse); // ignore error
			iServer.IndexAdapter().ReleaseEntry(id);
			}
		}

	// we have failed, move onto next state
	iState = iPCSynced ? EIndexIndividually : EIndex;
	iError=error;
	if(dir)
        {
	    CleanupStack::PopAndDestroy(dir);
        }
	    
	}



void CMsvDelete::DeleteAllIndexEntries()
//
//
//
	{
	iMovedEntries->Reset();

	if (iDeletedEntries->Count()==0)
		return;

	// delete the entries
	TInt error = KErrNone;
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	TMsvEntry* entry = NULL;
	TInt deleteIndex = 0;
	while(deleteIndex < iDeletedEntries->Count())
		{
		iServer.IndexAdapter().GetEntry(iDeletedEntries->At(deleteIndex), entry);
		if(entry==NULL)			
			{
			iDeletedEntries->Delete(deleteIndex);
			continue;
			}
		
		// If there's a DB store, delete any header entries associated with the entry.
		TRAP(error, iServer.MessageDBAdapter().DeleteHeaderEntryL(entry->iMtm, entry->iId));
		if(KErrNotFound == error) // Ignore entry not found case
			error = KErrNone;
	
		if(error)
			iDeletedEntries->Delete(deleteIndex);
		else
			++deleteIndex;
		}
#endif
	if(!error) // Looks awkward inside the macro above..
		error = iServer.IndexAdapter().DeleteSelectionUsingTransaction(*iDeletedEntries);

	if (error && iError==KErrNone)
		iError=error;
	}


void CMsvDelete::DeleteIndividualIndexEntries()
//
//
//
	{
	if (iDeletedEntries->Count()==0)
		return;

	TInt moveIndex=0;
	TInt deleteIndex=0;
	iServer.IndexAdapter().BeginTransaction();
	
	while(deleteIndex < iDeletedEntries->Count())
		{
		// get the entry
		TMsvEntry* entry=NULL;
 	    iServer.IndexAdapter().GetEntry(iDeletedEntries->At(deleteIndex), entry);
		if(entry==NULL)			
			{
			iDeletedEntries->Delete(deleteIndex);
			iMovedEntries->Delete(moveIndex);
			continue;
			}

		TInt error = KErrNone;
		if(!entry->Owner())
			{
			// either delete the entry, or move it to the deleted folder
			if(entry->PcSyncCount()==0)
				{ // Deleting the entry..
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
				// If there's a DB store, delete any header entries associated with the entry.
				TRAP(error, iServer.MessageDBAdapter().DeleteHeaderEntryL(entry->iMtm, entry->iId));
				if(KErrNotFound == error) // Ignore entry not found case
					error = KErrNone;
#endif
				if(!error) // Looks awkward inside the macro above..
					error = iServer.IndexAdapter().ForceDeleteEntry(entry->Id());
				
				if (error)
					iDeletedEntries->Delete(deleteIndex);
				else
					deleteIndex++;
				iMovedEntries->Delete(moveIndex);
				}
			else
				{ // Moving the entry..
				TMsvEntry deletedEntry(*entry);
				// this should be removed and CMsvDelete made a friend of TMsvEntry
				deletedEntry.iDetails.Set(TPtrC());
				deletedEntry.iDescription.Set(TPtrC());
				deletedEntry.SetDeleted(ETrue);
				deletedEntry.SetVisible(EFalse);
				deletedEntry.SetParent(KMsvDeletedEntryFolderEntryId);

				TMsvId backUpEntryId = entry->Id();
				iServer.IndexAdapter().LockEntry(backUpEntryId);
				error = iServer.IndexAdapter().ChangeEntry(deletedEntry, KMsvServerId, EFalse);
				iServer.IndexAdapter().ReleaseEntry(backUpEntryId);
				if (error)
					iMovedEntries->Delete(moveIndex);
				else
					moveIndex++;
				iDeletedEntries->Delete(deleteIndex);
				}
			}
		else
			error = KErrAccessDenied;

		if (error && iError==KErrNone)
			iError=error;
		}
	
	iServer.IndexAdapter().CommitTransaction();
	}



void CMsvDelete::Complete(TInt aError)
//
//
//
	{
	User::RequestComplete(iObserverStatus, aError);
	iState=ECompleted;
	iDeletionIndex=0;
	}
