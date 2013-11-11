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

#include "MSVMOVE.H"
#include "MSVSERV.H"
#include "MSVPANIC.H"
#include "MSVUTILS.H"
#include "msvindexadapter.h"

const TInt KFileWildCard='*';

//**********************************
// CMsvMove
//**********************************

// static
CMsvMove* CMsvMove::NewL(CMsvServer& aServer)
//
//
//
	{
	CMsvMove* self = new(ELeave) CMsvMove(aServer);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}


CMsvMove::CMsvMove(CMsvServer& aServer)
: CActive(EPriorityStandard), iServer(aServer), iLockedIndex(-2)
//
//
//
	{}


CMsvMove::~CMsvMove()
//
//
//
	{
	Cancel();
	Reset();
	delete iFileMan;
	delete iDescendents;
	}


void CMsvMove::Reset()
//
// NOTE called by d'tor
//
	{
	if (iDescendents && iLockedIndex!=-2)
		{
		TInt count = iDescendents->Count();
		while (count-- > iLockedIndex)
			{
			iServer.IndexAdapter().ReleaseEntryAndStore(iDescendents->At(count));
			}

		iDescendents->Reset();
		}
	iLockedIndex=-2;

	delete iTargetPath;
	iTargetPath=NULL;
	}


void CMsvMove::ConstructL()
//
//
//
	{
	iFileMan = CFileMan::NewL(iServer.FileSession());
	iDescendents = new(ELeave) CMsvEntrySelection;
	CActiveScheduler::Add(this);
	}


void CMsvMove::RunL()
//
//
//
	{
	TInt error = iStatus.Int();
	if (error==KErrNone)
		TRAP(error, DoRunL());
	if (error)
		{
		CleanupFiles(iTargetService);
		iDescendents->Reset();
		User::RequestComplete(iObserverStatus, error);
		}
	}


void CMsvMove::DoRunL()
//
//
//
	{
	if (iState==EIndex)
		{
        // If the entry is not a visible folder and it's visible
        // folder id is changed, the caller should update the 
		// visible folder id of all descendents.
		if(!iCurrentEntry->VisibleFolderFlag())
			{
	        User::LeaveIfError(iServer.IndexAdapter().MoveEntry(iSourceId, iTargetId, iDescendents));   
            }
	    else
	        {
	        User::LeaveIfError(iServer.IndexAdapter().MoveEntry(iSourceId, iTargetId)); 
	        }
		
		CleanupFiles(iSourceService);
		iServer.IndexAdapter().GetEntry(iSourceId, iCurrentEntry); // error ignored as entry exists
		iDescendents->Reset();
		User::RequestComplete(iObserverStatus, KErrNone);
		return;
		}

	FindNextStep();
	StartNextStep();
	}



void CMsvMove::CleanupFiles(TMsvId aService)
	{
	// delete the unwanted copies
	if (iSourceService!=iTargetService)
		{
		TInt count=iDescendents->Count();
		while (count--)
			{
			TFileName filename(iServer.Context().MessageFolder());
			TMsvId id = iDescendents->At(count);
			// BFs
			MsvUtils::ConstructEntryName(aService, id, filename, MsvUtils::EFolder);
			iFileMan->Delete(filename, CFileMan::ERecurse); // error ignored
			// BF
			iServer.FileSession().RmDir(filename); // error ignored
			// Store
			filename = iServer.Context().MessageFolder();
			MsvUtils::ConstructEntryName(aService, id, filename, MsvUtils::EStore);
			iFileMan->Delete(filename); // error ignored
			}
		}

	// release the locks
	TInt count = iDescendents->Count();
	while (count--)
		{
		iServer.IndexAdapter().ReleaseEntryAndStore(iDescendents->At(count)); // error ignored
		}

	iLockedIndex=-2;
	}


void CMsvMove::DoCancel()
//
// CANNOT cancel the fileman operation - so we just have to wait for it to complete normally
//
	{
	User::RequestComplete(iObserverStatus, KErrCancel);
	}


void CMsvMove::StartL(TMsvId aId, TMsvId aTarget, TRequestStatus& aObserverStatus)
//
//
//
	{
	// Fail now if the index says it's not available
	User::LeaveIfError(iServer.IndexAdapter().ErrorState());
	Reset();

	// check the source and target exists and which service they are under
	User::LeaveIfError(iServer.IndexAdapter().OwningService(aId, iSourceService));
	User::LeaveIfError(iServer.IndexAdapter().OwningService(aTarget, iTargetService));

#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	// Movement of entries across drives not supported.
	if(GetDriveId(aId) != GetDriveId(aTarget))
		{
		User::Leave(KErrNotSupported);
		}
#endif		// #if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)

	// get the index entry
	iSourceId = aId;
	iServer.IndexAdapter().GetEntry(iSourceId, iCurrentEntry); // error ignored as entry exists
	iParentId = iCurrentEntry->Parent();
	iTargetId = aTarget;
	iDescendents->AppendL(iSourceId);
	iNextDescendent=1;

	__ASSERT_DEBUG(iCurrentEntry->Parent()!=iTargetId, PanicServer(EMsvMtmMovingToSameParent));

	// check if the entry has children
	if (iCurrentEntry->Owner())
		{
		// check for moving into a descendent
		TBool descendent;
		iServer.IndexAdapter().IsADescendent(aId, aTarget, descendent); // error ignored as both entry exists
		if (descendent)
			User::Leave(KErrArgument);

		// get id of all the descendents
		User::LeaveIfError(iServer.IndexAdapter().ExpandSelectionRecursively(*iDescendents));
		}

	// check all the entries can be moved
	CheckEntriesL();

	// determine whether the store/files actually need copying
	iState = (iSourceService==iTargetService ? EIndex : EFolder);

	// set up the file copying
	if (iState!=EIndex)
		{
		TFileName targetPath(iServer.Context().MessageFolder());
		MsvUtils::ConstructEntryName(iTargetService, iTargetService, targetPath, MsvUtils::EPath);

		iTargetPath = targetPath.AllocL();
		iServer.FileSession().MkDirAll(targetPath);
		User::LeaveIfError(iServer.IndexAdapter().GetEntry(iDescendents->At(0), iCurrentEntry));

		// make sure the first step is valid
		TInt result = MsvUtils::HasDirectory(iServer.FileSession(), iServer.Context().MessageFolder(), iSourceService, iCurrentEntry->Id());
		User::LeaveIfError(result);

		if (!result)
			FindNextStep();
		}

	StartNextStep();

	iObserverStatus = &aObserverStatus;
	*iObserverStatus = KRequestPending;
	}


void CMsvMove::FindNextStep()
//
//
//
	{
	__ASSERT_DEBUG(iState!=EIndex, PanicServer(EMsvIncorrectStateInFindNextStep));

	FOREVER
		{
		// we have copied the files, check if then entry has a store
		if (iState==EFolder && MsvUtils::HasStore(iServer.FileSession(), iServer.Context().MessageFolder(), iSourceService, iCurrentEntry->Id()))
			{
			iState=EStore;
			break;
			}
		TInt count = iDescendents->Count();

		// are there any entries left to check
		if (iNextDescendent==count)
			{
			iState=EIndex;
			break;
			}

		// find the next descendent which still exists
		FOREVER
			{
			TBool error = EFalse;
			error = iServer.IndexAdapter().GetEntry(iDescendents->At(iNextDescendent++), iCurrentEntry);
			if (error==KErrNone)
				{
				break;
				}
			}

			iState = EFolder;
			if (MsvUtils::HasDirectory(iServer.FileSession(), iServer.Context().MessageFolder(), iSourceService, iCurrentEntry->Id()))
				break;
			
		}
	}


void CMsvMove::StartNextStep()
//
//
//
	{
	TInt error;
	switch (iState)
		{
		case EFolder:
			error = SetupFileCopy();
			break;
		case EStore:
			error = SetupStoreCopy();
			break;
		default:
			error=KErrNone;
		}

	if (iStatus!=KRequestPending)
		{
		// either the store/file copy has failed or the state is EIndex
		iStatus=KRequestPending;
		TRequestStatus* st = &iStatus;
		User::RequestComplete(st, error);
		}

	SetActive();
	}


void CMsvMove::CheckEntriesL()
//
//
//
	{
	iLockedIndex = iDescendents->Count();
	while (iLockedIndex)
		{
		TMsvId id = iDescendents->At(iLockedIndex-1);

		// lock the entry
		User::LeaveIfError(iServer.IndexAdapter().LockEntryAndStore(id));
		iLockedIndex--;

		// check noone is reading the store (reading the store now doesn't
		// keep the file open, therefore we can't rely on checking the file to stop
		// deleting while reading
		TBool reading=EFalse;
		User::LeaveIfError(iServer.IndexAdapter().IsStoreReadingLocked(id,reading));
		if(reading) User::Leave(KErrInUse);

		// get the entry
		TMsvEntry* entry;
		User::LeaveIfError(iServer.IndexAdapter().GetEntry(id, entry));
		// check the store
		TFileName filename;
		iServer.GetEntryName(id, filename, EFalse);
		TBool open;
		TInt error = iServer.FileSession().IsFileOpen(filename, open);
		if (error != KErrNotFound && error!=KErrPathNotFound)
			{
			if (error != KErrNone)
				User::Leave(error);
			if (open)
				User::Leave(KErrInUse);
			}

		// check any files
		CDir* dir;
		error = iServer.GetFileDirectoryListing(id, filename, dir);
		if (error == KErrNone)
			{
			CleanupStack::PushL(dir);
			User::LeaveIfError(iServer.FileSession().SetSessionPath(filename));
			TInt fCount=dir->Count();
			if (fCount--)
				{
				TBool open;
				User::LeaveIfError(iServer.FileSession().IsFileOpen((*dir)[fCount].iName, open));
				if (open)
					User::Leave(KErrInUse);
				}
			CleanupStack::PopAndDestroy(); // dir
			}
		else if (error != KErrPathNotFound)
			User::Leave(error);
		}
	}


TInt CMsvMove::SetupStoreCopy()
//
//
//
	{
	TFileName storeName;
	iServer.GetEntryName(iCurrentEntry->Id(), storeName, EFalse);

	TFileName targetPath(*iTargetPath);
	TBuf<1> dir;
	dir.NumFixedWidth(iCurrentEntry->Id()&0xf, EHex, 1);
	targetPath.Append(dir);
	_LIT(KDirSep,"\\");
	targetPath.Append(KDirSep);
	iServer.FileSession().MkDirAll(targetPath); // ignore errors, if it failed to create the directory the next line will fail
	return iFileMan->Copy(storeName, targetPath, CFileMan::EOverWrite, iStatus);
	}


TInt CMsvMove::SetupFileCopy()
//
//
//
	{
	// construct source directory
	TFileName sourceFileDirectory;
	iServer.GetEntryName(iCurrentEntry->Id(), sourceFileDirectory, ETrue);
	sourceFileDirectory.Append(KFileWildCard);
	// construct target directory
	TFileName targetFileDirectory(*iTargetPath);
	TBuf<KFileNameFixedWidth> dir;
	dir.NumFixedWidth(iCurrentEntry->Id()&0xf, EHex, 1);
	targetFileDirectory.Append(dir);
	_LIT(KDirSep,"\\");
	targetFileDirectory.Append(KDirSep);
	dir.NumFixedWidth(iCurrentEntry->Id(), EHex, KFileNameFixedWidth);
	targetFileDirectory.Append(dir);
	targetFileDirectory.Append(KMsvBinaryFolderExt);
	// setup the copy
	return iFileMan->Copy(sourceFileDirectory, targetFileDirectory, CFileMan::EOverWrite|CFileMan::ERecurse, iStatus);
	}



void CMsvMove::StartL(TMsvId aId, TMsvId aTarget)
//
//
//
	{
	// Fail now if the index says it's not available
	User::LeaveIfError(iServer.IndexAdapter().ErrorState());

#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	// Movement of entries across drives not supported.
	if(GetDriveId(aId) != GetDriveId(aTarget))
		{
		User::Leave(KErrNotSupported);
		}
#endif		// #if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)

	// check the spource and target exists and which service they are under
	User::LeaveIfError(iServer.IndexAdapter().OwningService(aId, iSourceService));
	User::LeaveIfError(iServer.IndexAdapter().OwningService(aTarget, iTargetService));
	__ASSERT_ALWAYS(iSourceService==iTargetService, PanicServer(EMsvMovingentryAccrossServices));

	// get the index entry
	iSourceId = aId;
	iServer.IndexAdapter().GetEntry(iSourceId, iCurrentEntry); // error ignored as entry exists
	iParentId = iCurrentEntry->Parent();
	iTargetId = aTarget;

	iDescendents->Reset();
	iDescendents->AppendL(iSourceId);
	iNextDescendent=1;

	__ASSERT_DEBUG(iCurrentEntry->Parent()!=iTargetId, PanicServer(EMsvMtmMovingToSameParent));

	// check if the entry has children
	if (iCurrentEntry->Owner())
		{
		// check for moving into a descendent
		TBool descendent;
		iServer.IndexAdapter().IsADescendent(aId, aTarget, descendent); // error ignored as both entry exists
		if (descendent)
			User::Leave(KErrArgument);

		// get id of all the descendents
		User::LeaveIfError(iServer.IndexAdapter().ExpandSelectionRecursively(*iDescendents));
		}

	// check all the entries can be moved
	CheckEntriesL();

	// move the entry
	if(!iCurrentEntry->VisibleFolderFlag())
	    {
	    User::LeaveIfError(iServer.IndexAdapter().MoveEntry(iSourceId, iTargetId, iDescendents));   
	    }
	else 
	    {
	    User::LeaveIfError(iServer.IndexAdapter().MoveEntry(iSourceId, iTargetId)); 
	    }
	
	// release the locks
	TInt count = iDescendents->Count();
	while (count--)
		{
		iServer.IndexAdapter().ReleaseEntryAndStore(iDescendents->At(count)); // error ignored
		}

	iLockedIndex=-2;
	}

//**********************************
// CMsvMoveEntries
//**********************************

// static
CMsvMoveEntries* CMsvMoveEntries::NewL(CMsvServer& aServer)
//
//
//
	{
	CMsvMoveEntries* self = new(ELeave) CMsvMoveEntries();
	CleanupStack::PushL(self);
	self->ConstructL(aServer);
	CleanupStack::Pop();
	return self;
	}


CMsvMoveEntries::CMsvMoveEntries() :
	CMsvCopyMoveEntriesBase()
//
//
//
	{}


CMsvMoveEntries::~CMsvMoveEntries()
//
//
//
	{
	delete iMove;
	}


void CMsvMoveEntries::ConstructL(CMsvServer& aServer)
//
//
//
	{
	iMove = CMsvMove::NewL(aServer);
	CMsvCopyMoveEntriesBase::ConstructL();
	}


void CMsvMoveEntries::DoCancel()
//
//
//
	{
	iMove->Cancel();
	User::RequestComplete(iObserverStatus, KErrCancel);
	}


void CMsvMoveEntries::DoStartL(TMsvId aSourceId, TMsvId aTargetId, TRequestStatus& aObserverStatus)
//
//
//
	{
	iMove->StartL(aSourceId, aTargetId, aObserverStatus);
	}
