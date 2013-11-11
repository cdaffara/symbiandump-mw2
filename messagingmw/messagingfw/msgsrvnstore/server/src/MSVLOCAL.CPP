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


#include <e32std.h>

#include "MSVSTD.H"
#include "MSVSTORE.H"
#include "MSVIDS.H"

#include "MSVENTRY.H"
#include "MSVSERV.H"
#include "MSVLOPS.H"
#include "MSVMOVE.H"
#include "MSVCOPY.H"
#include "MSVDELET.H"
#include "MSVPANIC.H"
#include <tmsvsystemprogress.h>
#include "msvindexadapter.h"

//**********************************
// CMsvLocalOperation
//**********************************

CMsvLocalOperation::CMsvLocalOperation(const RMessage2& aMessage, TMsvOp aId, CMsvServer& aMsvServer)
: CMsvServerOperation(aMessage, aId, KUidMsvLocalServiceMtm, KMsvLocalServiceIndexEntryId, -1), iMsvServer(aMsvServer)
	{
	__DECLARE_NAME(_S("CMsvLocalOperation"));
	}

const TDesC8& CMsvLocalOperation::Progress()
	{
	return iProgress;
	}

/**
Obtain the local progress information
@param aOutSysProg The TMsvSystemProgress structure to be populated
@return system wide error code
*/
TInt CMsvLocalOperation::SystemProgress(TMsvSystemProgress& aSysProgress)
	{
	aSysProgress.iErrorCode = iProgress().iError;
	aSysProgress.iId = iProgress().iId;
	return KErrNone;	// This method will not fail
	}

//**********************************
// CMsvLocalCopyOperation
//**********************************



CMsvLocalCopyOperation::CMsvLocalCopyOperation(const RMessage2& aMessage, TMsvOp aId, CMsvEntrySelection* aSelection, TMsvId aTarget, CMsvServer& aMsvServer)
: CMsvLocalOperation(aMessage, aId, aMsvServer), iOrigEntries(aSelection), iTarget(aTarget)
//
//
//
	{
	__DECLARE_NAME(_S("CMsvLocalCopyOperation"));
	// set up the progress
	iProgress().iType = TMsvLocalOperationProgress::ELocalCopy;
	iProgress().iTotalNumberOfEntries = iOrigEntries->Count();
	iProgress().iNumberRemaining = iProgress().iTotalNumberOfEntries;
	CActiveScheduler::Add(this);
	}


CMsvLocalCopyOperation::~CMsvLocalCopyOperation()
//
//
//
	{
	Cancel();
	delete iOrigEntries;
	delete iNewEntries;
	delete iCopy;
	}


void CMsvLocalCopyOperation::DoCancel()
//
//
//
	{
	iCopy->Cancel();
	iProgress().iError = KErrCancel;
	Completed();
	}


void CMsvLocalCopyOperation::RunL()
//
//
//
	{
	// sort out descendent copying
	if (iDescendentCopying && iDescendentId == KMsvNullIndexEntryId)
		iDescendentId = iCopy->NewEntryId();

	// record details of the previous move
	iProgress().iNumberRemaining--;
	if (iStatus!=KErrNone)
		{
		iProgress().iNumberFailed++;
		iProgress().iError=iStatus.Int();
		}
	else
		{
		iNewEntries->AppendL(iCopy->NewEntryId()); // will not fail, as space has been reserved
		iProgress().iNumberCompleted++;
		}

	if (iIndex < iOrigEntries->Count())
		{
		// start the next move
		TInt error =  CopyNextEntry();
		if (error==KErrNone)
			return; // doing another move
		__ASSERT_DEBUG(iProgress().iNumberRemaining==0, PanicServer(EMsvCopyingOperationInvariant));
		}

	// complete the operation
	Completed();
	}


void CMsvLocalCopyOperation::StartL()
//
//
//
	{
	iNewEntries = new(ELeave) CMsvEntrySelection;
	iCopy = CMsvCopy::NewL(iMsvServer);

	iIndex = 0;
	iNewEntries->SetReserveL(iOrigEntries->Count());
	iDescendentId = KMsvNullIndexEntryId;

	// check for copying into a descedent
	TInt count = iOrigEntries->Count();
	while (count--)
		{
		TMsvId id = iOrigEntries->At(count);
		iDescendentCopying = (id==iTarget);
		if (!iDescendentCopying)
			{
			iMsvServer.IndexAdapter().IsADescendent(id, iTarget, iDescendentCopying); // error ignored
			}

		if (iDescendentCopying)
			{
			// has to be done first
			iOrigEntries->Delete(count);
			iOrigEntries->InsertL(0, id);
			break;
			}
		}


	TInt error = CopyNextEntry();
	if (error==KErrNone)
		SetState(EMsvOperationRunning);
	else
		{
		iProgress().iNumberFailed = iProgress().iTotalNumberOfEntries;
		iProgress().iNumberRemaining = 0;
		iProgress().iError=error;
		Completed();
		}
	}


TInt CMsvLocalCopyOperation::CopyNextEntry()
//
//
//
	{
	FOREVER
		{
		TRAPD(error, iCopy->StartL(iOrigEntries->At(iIndex++), iTarget, iStatus, iDescendentCopying));
		if (error)
			{
			iProgress().iNumberRemaining--;
			iProgress().iNumberFailed++;
			iProgress().iError=error;
			if (iIndex==iOrigEntries->Count())
				return error;
			}
		else
			{
			SetActive();
			break;
			}
		}
	return KErrNone;
	}


void CMsvLocalCopyOperation::Completed()
//
//
//
	{
	SetState(EMsvOperationCompleted);
	if (iNewEntries->Count())
		{
		iProgress().iId = iNewEntries->At(0);
		iMsvServer.NotifyChanged(EMsvEntriesCreated, *iNewEntries, iTarget);
		}
	iMessage.Complete(KErrNone);
	}




//**********************************
// CMsvLocalMoveOperation
//**********************************


CMsvLocalMoveOperation::CMsvLocalMoveOperation(const RMessage2& aMessage, TMsvOp aId, CMsvEntrySelection* aSelection, TMsvId aTarget, CMsvServer& aMsvServer)
: CMsvLocalOperation(aMessage, aId, aMsvServer), iTarget(aTarget), iParent(KMsvNullIndexEntryId), iOrigEntries(aSelection)
	{
	__DECLARE_NAME(_S("CMsvLocalMoveOperation"));
	// set up the progress
	iProgress().iType = TMsvLocalOperationProgress::ELocalMove;
	iProgress().iTotalNumberOfEntries = iOrigEntries->Count();
	iProgress().iNumberRemaining = iProgress().iTotalNumberOfEntries;
	CActiveScheduler::Add(this);
	}


CMsvLocalMoveOperation::~CMsvLocalMoveOperation()
	{
	Cancel();
	delete iOrigEntries;
	delete iMove;
	}

void CMsvLocalMoveOperation::DoCancel()
	{
	iMove->Cancel();
	iOrigEntries->Delete(0,iNextEntry);
	iProgress().iError = KErrCancel;
	Completed();
	}

void CMsvLocalMoveOperation::RunL()
//
//
//
	{
	// record details of the previous move
	iProgress().iNumberRemaining--;
	if (iStatus!=KErrNone)
		{
		iProgress().iNumberFailed++;
		iProgress().iError=iStatus.Int();
		iOrigEntries->Delete(iNextEntry);
		}
	else
		iProgress().iNumberCompleted++;

	TInt error=KErrNone;
	if (iNextEntry) // will be zero when completed
		{
		// start the next move
		error =  MoveNextEntry();
		if (error==KErrNone)
			return; // doing another move
		__ASSERT_DEBUG(iProgress().iNumberRemaining==0, PanicServer(EMsvMovingOperationInvariant));
		}

	// complete the operation
	Completed();
	}

void CMsvLocalMoveOperation::Completed()
//
//
//
	{
	SetState(EMsvOperationCompleted);
	if (iOrigEntries->Count())
		{
		iProgress().iId = iOrigEntries->At(0);
		iMsvServer.NotifyChanged(EMsvEntriesMoved, *iOrigEntries, iTarget, iParent);
		}
	iMessage.Complete(KErrNone);
	}

void CMsvLocalMoveOperation::StartFailed(TInt aError)
//
//
//
	{
	iProgress().iNumberFailed = iProgress().iTotalNumberOfEntries;
	iProgress().iNumberRemaining = 0;
	iProgress().iError=aError;
	iOrigEntries->Reset();
	Completed();
	}

void CMsvLocalMoveOperation::StartL()
//
//
//
	{
	// check if the target exists
	TMsvEntry* entry;
	TInt err = KErrNone;
	err = iMsvServer.IndexAdapter().GetEntry(iTarget, entry);
	if (err !=KErrNone)
		{
		StartFailed(KErrPathNotFound);
		return;
		}

	// find the current parent
	TInt count=iOrigEntries->Count();
	while (count--)
		{
		TInt error = KErrNone;
		error = iMsvServer.IndexAdapter().GetEntry(iOrigEntries->At(count), entry);
		if (error ==KErrNone)
			{
			iParent=entry->Parent();
			break;
			}
		}

	// nothing to copy
	if (count==KErrNotFound)
		{
		StartFailed(count);
		return;
		}

#if defined(_DEBUG)
	// check all the entries are local
	TInt dCount=iOrigEntries->Count();
	while (dCount--)
		{
		TInt dSourceLocal;
		TInt dError = KErrNone;
		dError = iMsvServer.IndexAdapter().IsLocal(iOrigEntries->At(count), dSourceLocal);
		__ASSERT_DEBUG(dError!=KErrNone || dSourceLocal,   PanicServer(EMsvMovingLocalEntryIsRemote));
		}
#endif

	// start the copying
	iMove = CMsvMove::NewL(iMsvServer);
	iNextEntry=iOrigEntries->Count();
	TInt error = MoveNextEntry();
	if (error==KErrNone)
		SetState(EMsvOperationRunning);
	else
		StartFailed(error);
	}


TInt CMsvLocalMoveOperation::MoveNextEntry()
//
//
//
	{
	TInt error=KErrGeneral;
	while (error!=KErrNone && iNextEntry)
		{
		iMove->Reset();
		error=KErrNone;
		TRAP(error, iMove->StartL(iOrigEntries->At(--iNextEntry), iTarget, iStatus));
		if (error)
			{
			iProgress().iNumberRemaining--;
			iProgress().iNumberFailed++;
			iProgress().iError=error;
			iOrigEntries->Delete(iNextEntry);
			}
		}
	if (error==KErrNone)
		SetActive();
	return error;
	}



//**********************************
// CMsvLocalDeleteOperation
//**********************************


CMsvLocalDeleteOperation::CMsvLocalDeleteOperation(const RMessage2& aMessage, TMsvOp aId, CMsvEntrySelection* aSelection, CMsvServer& aMsvServer)
: CMsvLocalOperation(aMessage, aId, aMsvServer), iSelection(aSelection)
	{
	__DECLARE_NAME(_S("CMsvLocalDeleteOperation"));
	// set up the progress
	iProgress().iType = TMsvLocalOperationProgress::ELocalDelete;
	iProgress().iTotalNumberOfEntries = iSelection->Count();
	iProgress().iNumberRemaining = iProgress().iTotalNumberOfEntries;
	CActiveScheduler::Add(this);
	}


CMsvLocalDeleteOperation::~CMsvLocalDeleteOperation()
	{
	Cancel();
	delete iSelection;
	delete iDeletedEntries;
	delete iMovedEntries;
	delete iDelete;
	delete iWorkSelection1;
	delete iWorkSelection2;
	}


void CMsvLocalDeleteOperation::DoCancel()
//
//
//
	{
	iDelete->Cancel();
	iProgress().iError = KErrCancel;
	Completed();
	// if the last entry was removed, update the notification selections
	TRAP_IGNORE(PartialCompletionNotificationL());
	}


void CMsvLocalDeleteOperation::PartialCompletionNotificationL()
//
//
//
	{
	TMsvId id=iSelection->At(iIndex-1);
	TBool entryExists = EFalse;
	entryExists = iMsvServer.IndexAdapter().EntryExists(id);
	if (!entryExists)
		id = iParent;

	if (iWorkSelection1->Count())
		{
		iDeletedEntries->Reset();
		iDeletedEntries->AppendL(iWorkSelection1->Back(0), iWorkSelection1->Count());
		iMsvServer.NotifyChanged(EMsvEntriesDeleted, *iDeletedEntries, id);
		}

	if (iWorkSelection2->Count())
		{
		iDeletedEntries->Reset();
		iMovedEntries->AppendL(iWorkSelection2->Back(0), iWorkSelection2->Count());
		iMsvServer.NotifyChanged(EMsvEntriesMoved, *iMovedEntries, KMsvDeletedEntryFolderEntryId, id);
		}
	}


void CMsvLocalDeleteOperation::RunL()
//
// Attempts the next delete
//
	{
	TRAPD(leave, DoRunL());
	if (leave)
		{
		// probably OOM so complete the operation
		iProgress().iError = leave;
		Completed();
		}
	}


void CMsvLocalDeleteOperation::DoRunL()
//
//
//
	{
	// record the result of the last delete
	iProgress().iNumberRemaining--;
	if (iStatus.Int()==KErrNone)
		{
		iProgress().iNumberCompleted++;
		iDeletedEntries->AppendL(iWorkSelection1->Back(0), iWorkSelection1->Count());
		iMovedEntries->AppendL(iWorkSelection2->Back(0), iWorkSelection2->Count());
		}
	else
		{
		iProgress().iNumberFailed++;
		if (iProgress().iError==KErrNone)
			iProgress().iError = iStatus.Int();
		}

	// start thje next delete or complete the operation
	if (iProgress().iNumberRemaining)
		{
		iWorkSelection1->Reset();
		iWorkSelection2->Reset();
		iDelete->StartL(iSelection->At(iIndex++), *iWorkSelection1, *iWorkSelection2, iStatus);
		SetActive();
		}
	else
		{
		__ASSERT_DEBUG(iIndex==iSelection->Count(), PanicServer(EMsvLocalDeletionMisCount));
		Completed();
		}
	}


void CMsvLocalDeleteOperation::Completed()
//
// The operation has completed
//
	{
/**
Added code to commit transaction for bulk deletion.
*/
	TRAP_IGNORE(iMsvServer.IndexAdapter().GetDbAdapter()->CommitTransactionL());

	// notify server of any deletions or moves to the deleted folder
	if (iDeletedEntries->Count())
		iMsvServer.NotifyChanged(EMsvEntriesDeleted, *iDeletedEntries, iParent);
	if (iMovedEntries->Count())
		iMsvServer.NotifyChanged(EMsvEntriesMoved, *iMovedEntries, KMsvDeletedEntryFolderEntryId, iParent);

	// complete the request
	SetState(EMsvOperationCompleted);
	iMessage.Complete(KErrNone);
	}

void CMsvLocalDeleteOperation::StartL()
	{
	// reserve space so we don't manage to delete the entries but run
	// out of memory when trying to report the fact.
	iDeletedEntries = new(ELeave)CMsvEntrySelection;
	iDeletedEntries->SetReserveL(iSelection->Count());
	iMovedEntries = new(ELeave)CMsvEntrySelection;
	iMovedEntries->SetReserveL(iSelection->Count());
	iWorkSelection1 = new(ELeave)CMsvEntrySelection;
	iWorkSelection2 = new(ELeave)CMsvEntrySelection;
	iDelete = CMsvDelete::NewL(iMsvServer);

	// find the parent
	TInt count=iSelection->Count();
	while (count--)
		{
		TMsvEntry* entry;
		TInt error = KErrNone;
		error = iMsvServer.IndexAdapter().GetEntry(iSelection->At(count),entry);
		if (error)
			{
			iSelection->Delete(count);
			iProgress().iNumberRemaining--;
			iProgress().iNumberCompleted++;
			}
		else
			iParent = entry->Parent();
		}

	if (iSelection->Count())
		{
		// start deleting the first entry
		iDelete->StartL(iSelection->At(iIndex++), *iWorkSelection1, *iWorkSelection2, iStatus);
		SetActive();
		SetState(EMsvOperationRunning);
		}
	else
		Completed();
	}





//**********************************
// CMsvLocalChangeOperation
//**********************************


CMsvLocalChangeOperation::CMsvLocalChangeOperation(const RMessage2& aMessage, TMsvOp aId, const TMsvEntry& aEntry, CMsvServer& aMsvServer)
: CMsvLocalOperation(aMessage, aId, aMsvServer), iEntry(aEntry)
	{
	__DECLARE_NAME(_S("CMsvLocalChangeOperation"));
	// set up the progress
	iProgress().iType = TMsvLocalOperationProgress::ELocalChanged;
	iProgress().iTotalNumberOfEntries = 1;
	iProgress().iId = iEntry.Id();
	CActiveScheduler::Add(this);
	}

CMsvLocalChangeOperation::~CMsvLocalChangeOperation()
//
//
//
	{
	Cancel();
	delete iDelete;
	delete iWorkSelection1;
	delete iWorkSelection2;
	}

void CMsvLocalChangeOperation::DoCancel()
//
//
//
	{
	__ASSERT_DEBUG(iDelete!=NULL, PanicServer(EMsvChangeEntryBadState));
	iDelete->Cancel();
	iProgress().iError = KErrCancel;
	Completed();
	}

void CMsvLocalChangeOperation::RunL()
//
//
//
	{
	if (iStatus.Int()==KErrNone)
		{
		iProgress().iNumberCompleted++;
		}
	else
		{
		iProgress().iNumberFailed++;
		iProgress().iError = iStatus.Int();
		}
	Completed();
	}

void CMsvLocalChangeOperation::StartL(TSecureId aOwnerId, TBool aForcedUpdate)
//
//
//
	{
	iId = iEntry.Id();
	iParent = iEntry.Parent();
	// check if this is actually a deletion
	if (iEntry.Deleted() && iEntry.PcSyncCount()==0)
		{
		iDelete = CMsvDelete::NewL(iMsvServer);
		iWorkSelection1 = new(ELeave)CMsvEntrySelection;
		iWorkSelection2 = new(ELeave)CMsvEntrySelection;
		iDelete->StartL(iEntry.Id(), *iWorkSelection1, *iWorkSelection2, iStatus, ETrue);
		SetActive();
		return;
		}

	// otherwise just changing it
	TInt error = KErrNone;
	error = iMsvServer.IndexAdapter().LockEntry(iId);
	if (error==KErrNone)
		{
		error = iMsvServer.ChangeEntry(iEntry, aOwnerId, aForcedUpdate);
		iMsvServer.IndexAdapter().ReleaseEntry(iId); // error ignored
		}

	if (error==KErrNone)
		iProgress().iNumberCompleted++;
	else
		{
		iProgress().iNumberFailed++;
		iProgress().iError = error;
		}
	Completed();
	}


void CMsvLocalChangeOperation::Completed()
//
//
//
	{
	if (iProgress().iError==KErrNone)
		iMsvServer.NotifyChanged(iDelete==NULL ? EMsvEntriesChanged : EMsvEntriesDeleted, iId, iParent);
	SetState(EMsvOperationCompleted);
	iMessage.Complete(KErrNone);
	}
//**********************************
// CMsvLocalChangeEntriesOperation
//**********************************


CMsvLocalChangeEntriesOperation::CMsvLocalChangeEntriesOperation(const RMessage2& aMessage, TMsvOp aId, CMsvEntrySelection* aSelection, CMsvServer& aMsvServer, TInt aMark)
: CMsvLocalOperation(aMessage, aId, aMsvServer), iSelection(aSelection), iMark(aMark)
    {
    __DECLARE_NAME(_S("CMsvLocalChangeEntriesOperation"));
    // set up the progress
    iProgress().iType = TMsvLocalOperationProgress::ELocalChanged;
    iProgress().iTotalNumberOfEntries = iSelection->Count();;
    iProgress().iNumberRemaining = iProgress().iTotalNumberOfEntries;
    CActiveScheduler::Add(this);
    }

CMsvLocalChangeEntriesOperation::~CMsvLocalChangeEntriesOperation()
//
//
//
    {
    Cancel();
    delete iSelection;
    delete iDelete;
    delete iWorkSelection1;
    delete iWorkSelection2;
    }

void CMsvLocalChangeEntriesOperation::DoCancel()
//
//
//
    {
    __ASSERT_DEBUG(iDelete!=NULL, PanicServer(EMsvChangeEntryBadState));
    iDelete->Cancel(); 
    iProgress().iError = KErrCancel;
    Completed();
    }

void CMsvLocalChangeEntriesOperation::RunL()
//
//
//
    {
    if (iStatus.Int()==KErrNone)
        {
        iProgress().iNumberCompleted++;
        }
    else
        {
        iProgress().iNumberFailed++;
        iProgress().iError = iStatus.Int();
        }
    
    Completed();
    }


void CMsvLocalChangeEntriesOperation::StartL(TSecureId aOwnerId, TBool aForcedUpdate)
//
//
//
    {
    TMsvEntry* entry1;
 
    TInt count = iSelection->Count();
    while (count--)
        {
        iMsvServer.IndexAdapter().GetEntry(iSelection->At(count),entry1);
        iId = entry1->Id();
        iParent = entry1->Parent();
        // check if this is actually a deletion
        if (entry1->Deleted() && entry1->PcSyncCount()==0)
            {
             iDelete = CMsvDelete::NewL(iMsvServer);
             iWorkSelection1 = new(ELeave)CMsvEntrySelection;
             iWorkSelection2 = new(ELeave)CMsvEntrySelection;

            iDelete->StartL(entry1->Id(), *iWorkSelection1, *iWorkSelection2, iStatus, ETrue);
            SetActive();
            return;
            }
        }
  
    TInt error  = KErrNone;
    TMsvEntry* entry;
    count = iSelection->Count();
    while (count--)
        {
        error = iMsvServer.IndexAdapter().GetEntry(iSelection->At(count),entry);
        if (error==KErrNone)
            {
            error = iMsvServer.IndexAdapter().LockEntry(iSelection->At(count));
            error = iMsvServer.ChangeEntry(*entry, aOwnerId, aForcedUpdate);
            error = iMsvServer.IndexAdapter().ReleaseEntry(iSelection->At(count)); // error ignored
            }
         }

    if (error==KErrNone)
        iProgress().iNumberCompleted++;
    else
        {
        iProgress().iNumberFailed++;
        iProgress().iError = error;
        }
    Completed();
    }

void CMsvLocalChangeEntriesOperation::Completed()
//
//
//
    {
    
    if (iProgress().iError==KErrNone)
        iMsvServer.NotifyChanged(iDelete==NULL ? EMsvEntriesChanged : EMsvEntriesDeleted, iId, iParent);
    SetState(EMsvOperationCompleted);
    iMessage.Complete(KErrNone);
     }


//**********************************
// CMsvLocalCreateOperation
//**********************************

CMsvLocalCreateOperation::CMsvLocalCreateOperation(const RMessage2& aMessage, TMsvOp aId, const TMsvEntry& aEntry, CMsvServer& aMsvServer)
: CMsvLocalOperation(aMessage, aId, aMsvServer), iEntry(aEntry)
	{
	__DECLARE_NAME(_S("CMsvLocalCreateOperation"));
	// set up the progress
	iProgress().iType = TMsvLocalOperationProgress::ELocalNew;
	iProgress().iTotalNumberOfEntries = 1;
	CActiveScheduler::Add(this);
	}


CMsvLocalCreateOperation::~CMsvLocalCreateOperation()
	{}

void CMsvLocalCreateOperation::DoCancel()
	{}

void CMsvLocalCreateOperation::RunL()
	{}

void CMsvLocalCreateOperation::Start(TSecureId aOwnerId)
	{
	TMsvEntry entry = iEntry;
	TInt error = iMsvServer.AddEntry(entry, aOwnerId, ETrue);

	if (error)
		{
		iProgress().iNumberFailed++;
		iProgress().iError = error;
		}
	else
		{
		iProgress().iNumberCompleted++;
		iProgress().iId = entry.Id();
		iMsvServer.NotifyChanged(EMsvEntriesCreated, entry.Id(), entry.Parent());
		}

	SetState(EMsvOperationCompleted);
	iMessage.Complete(KErrNone);
	}

//**********************************
// CMsvChangeDriveOperation
//**********************************
CMsvChangeDriveOperation::CMsvChangeDriveOperation(const RMessage2& aMessage, TMsvOp aId, TInt aDrive, CMsvServer& aServer)
: CMsvServerOperation(aMessage, aId, KUidMsvLocalServiceMtm, KMsvLocalServiceIndexEntryId, -1), iDrive(aDrive), iServer(aServer)
	{
	CActiveScheduler::Add(this);
	}

CMsvChangeDriveOperation::~CMsvChangeDriveOperation()
	{
	Cancel();
	}

const TDesC8& CMsvChangeDriveOperation::Progress()
	{
	if (iServer.NewContext())
		iProgress() = iServer.NewContext()->Progress();

	return iProgress;
	}

void CMsvChangeDriveOperation::DoCancel()
	{
	if (iServer.NewContext())
		iServer.NewContext()->Cancel();

	iProgress().iError = KErrCancel;
	Completed();
	}

void CMsvChangeDriveOperation::RunL()
	{
	Completed();
	}

void CMsvChangeDriveOperation::Completed()
	{
	// Get progress
	if (iServer.NewContext())
		{
		iProgress() = iServer.NewContext()->Progress();

		// If an error occurs we have to delete the new context
		iServer.DeleteNewContext();
		}
	else
		iProgress() = iServer.Context().Progress();

	SetState(EMsvOperationCompleted);
	iMessage.Complete(KErrNone);
	}

TInt CMsvChangeDriveOperation::Start()
	{
	TInt error = iServer.ChangeDrive(iDrive, &iStatus);
	if (!error)
		SetActive();
	return error;
	}

