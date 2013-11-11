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
#include "MSVMOVE.H"
#include "MSVDELET.H"
#include "MSVCOPY.H"

#include "MSVPANIC.H"
#include "MSVUTILS.H"
#include "msvindexadapter.h"
const TInt KMsvServerEntryStandardBufLength=64;

//**********************************
// CMsvServerEntry
//**********************************

// static
/**
@internalComponent
*/
EXPORT_C CMsvServerEntry* CMsvServerEntry::NewL(CMsvServer& aServer, TMsvId aId)
//
//
//
	{
	CMsvServerEntry* self = new(ELeave) CMsvServerEntry(aServer);
	CleanupStack::PushL(self);
	self->ConstructL(aId);
	CleanupStack::Pop();
	return self;
	}

CMsvServerEntry::CMsvServerEntry(CMsvServer& aServer)
: CActive(EPriorityStandard), iLockedStore(EFalse), iServer(aServer)
//
//
//
	{
	__DECLARE_NAME(_S("CMsvServerEntry"));
	}

void CMsvServerEntry::ConstructL(TMsvId aId)
//
//
//
	{
	iDescription = HBufC::NewL(KMsvServerEntryStandardBufLength);
	iDetails	 = HBufC::NewL(KMsvServerEntryStandardBufLength);
	User::LeaveIfError(SetEntry(aId));

	CActiveScheduler::Add(this);
	}


CMsvServerEntry::~CMsvServerEntry()
//
//
//
/** Frees resources for the object. It:

1. releases the lock on the entry

2. releases the lock on the associated message store if has been opened with
EditStoreL()

3. cancels any outstanding asynchronous MoveEntryL() move operation */
	{
	__ASSERT_DEBUG(iStore==NULL, PanicServer(EMsvStoreLeftOpenOnDestruction));

	if (iEntry.Id() != KMsvNullIndexEntryId)
		{
		iServer.IndexAdapter().ReleaseEntry(iEntry.Id()); // ignore any error
		}

	if (iLockedStore)
		{
		iServer.IndexAdapter().ReleaseStore(iEntry.Id()); // ignore any error
		}

	delete iDescription;
	delete iDetails;

	Cancel();
	}



EXPORT_C TInt CMsvServerEntry::SetEntry(TMsvId aId)
//
// Changes the entry used as the context
//
/** Changes the context of the specified entry.

The call locks the entry, preventing it from being accessed by other clients.
The lock is released when the object is deleted or the context is changed.

Note that you can change the context to KMsvNullIndexEntryId to unlock an
entry without locking another.

@param aId ID of the entry to access
@return KErrNone if successful, KErrNotFound if the entry does not exist, or
KErrLocked if the entry is locked. */
	{
	__ASSERT_ALWAYS(iStore==NULL, PanicServer(EMsvEntryStoreLeftOpen));
	__ASSERT_ALWAYS(iEntryState==EMsvIdle, PanicServer(EMsvServerEntryNotIdle));

	// changing to itself
	if (iEntry.Id()==aId)
		return KErrNone;

	if (aId == KMsvNullIndexEntryId)
		{
		// setting the context to NULL
		iServer.IndexAdapter().ReleaseEntry(iEntry.Id()); // error ignored
		*iDescription = TPtrC();
		*iDetails = TPtrC();
		iEntry = TMsvEntry();
		return KErrNone;
		}

	// lock the new entry
	TInt error = KErrNone;
    error = iServer.IndexAdapter().LockEntry(aId);
	if (error)
		return error;

	// get the new entry
	TMsvEntry* entryPtr;
	error = iServer.IndexAdapter().GetEntry(aId, entryPtr, iContextOwnerId);
	if (error==KErrNone)
		{
		// check we can copy the strings
		error = IncreaseBufferSizes(entryPtr->iDescription.Length(), entryPtr->iDetails.Length());

		if (error==KErrNone)
			{
			// release old entry
			if (iEntry.Id()!=KMsvNullIndexEntryId)
				{
				iServer.IndexAdapter().ReleaseEntry(iEntry.Id()); // error ignored
				}


			// switch the context of this object
			*iDescription = entryPtr->iDescription;
			*iDetails = entryPtr->iDetails;
			iEntry = *entryPtr;
			iEntry.iDescription.Set(*iDescription);
			iEntry.iDetails.Set(*iDetails);
			}
		}

	// release the lock on the new entry
	if (error)
		{
		iServer.IndexAdapter().ReleaseEntry(aId); // error ignored
		}


	return error;
	}



TInt CMsvServerEntry::IncreaseBufferSizes(TInt aNewDescriptionSize, TInt aNewDetailsSize)
//
// Increase the size of the buffers if needed. The current contents of the buffers is maintained
//
	{
	if (aNewDescriptionSize > iDescription->Des().MaxLength())
		{
		HBufC* newBuf = iDescription->ReAlloc(aNewDescriptionSize);
		if (newBuf==NULL)
			return KErrNoMemory;
		iDescription=newBuf;
		}

	if (aNewDetailsSize > iDetails->Des().MaxLength())
		{
		HBufC* newBuf = iDetails->ReAlloc(aNewDetailsSize);
		if (newBuf==NULL)
			return KErrNoMemory;
		iDetails=newBuf;
		}
	return KErrNone;
	}

/**
Sets the context's index entry to the specified values.

@param	aEntry
The new details for the entry.

@return
KErrNone - success; KErrAccessDenied - the entry is read only (deleted
entry, standard folder, or locked); KErrNotSupported - aEntry is invalid or the
ID specified in aEntry is not the same as the context ID or no context has been
set for the object; KErrNoMemory - a memory allocation failed.
*/
EXPORT_C TInt CMsvServerEntry::ChangeEntry(const TMsvEntry& aEntry)
	{
	return DoChangeEntry(aEntry, KMsvServerId, EFalse, EFalse);
	}

/**
Sets the context's index entry to the specified values and updates the owner of
the entry to that specified by the supplied ID.

@param	aEntry
The new details for the entry.

@param	aOwnerId
The ID of the process that should own the entry.

@return
KErrNone - success; KErrAccessDenied - the entry is read only (deleted
entry, standard folder, or locked); KErrNotSupported - aEntry is invalid or the
ID specified in aEntry is not the same as the context ID or no context has been
set for the object; KErrNoMemory - a memory allocation failed.
*/
EXPORT_C TInt CMsvServerEntry::ChangeEntry(const TMsvEntry& aEntry, TSecureId aOwnerId)
	{
	return DoChangeEntry(aEntry, aOwnerId, ETrue, EFalse);
	}
/**
Sets the context's index entry to the specified values and updates the owner of
the entry to that specified by the supplied ID. It does this as part of a bulk
operation, so the changes are not immediately committed to file.

@param	aEntry
The new details for the entry.

@param	aOwnerId
The ID of the process that should own the entry. Only commits changes to
file at specified bulk commit interval.

@return
KErrNone - success; KErrAccessDenied - the entry is read only (deleted
entry, standard folder, or locked); KErrNotSupported - aEntry is invalid or the
ID specified in aEntry is not the same as the context ID or no context has been
set for the object; KErrNoMemory - a memory allocation failed.
*/
EXPORT_C TInt CMsvServerEntry::ChangeEntryBulk(const TMsvEntry& aEntry, TSecureId aOwnerId)
	{
	return DoChangeEntry(aEntry, aOwnerId, ETrue, ETrue);
	}

/**
Sets the context's index entry to the specified values and updates the owner of
the entry to that specified by the supplied ID. It does this as part of a bulk
operation, so the changes are not immediately committed to file.

@param	aEntry
The new details for the entry.

@return
KErrNone - success; KErrAccessDenied - the entry is read only (deleted
entry, standard folder, or locked); KErrNotSupported - aEntry is invalid or the
ID specified in aEntry is not the same as the context ID or no context has been
set for the object; KErrNoMemory - a memory allocation failed.
*/
EXPORT_C TInt CMsvServerEntry::ChangeEntryBulk(const TMsvEntry& aEntry)
	{
	return DoChangeEntry(aEntry, KMsvServerId, EFalse, ETrue);
	}


TInt CMsvServerEntry::DoChangeEntry(const TMsvEntry& aEntry, TSecureId aOwnerId, TBool aForcedUpdate, TBool aBulk)
	{
	__ASSERT_DEBUG(iEntry.Id()!=KMsvNullIndexEntryId, PanicServer(EMsvEntryWithNoContext1));
	__ASSERT_DEBUG(aEntry.Id()==iEntry.Id(), PanicServer(EMsvNotChangingCurrentContext));
	__ASSERT_DEBUG(MsvUtils::ValidEntry(aEntry), PanicServer(EMsvBadEntryContents));
	__ASSERT_DEBUG(!iEntry.Deleted(), PanicServer(EMsvChangingDeletedEntry));

	// can only change the context, and must be valid
	if (iEntry.Id()==KMsvNullIndexEntryId || aEntry.Id()!=iEntry.Id() || !MsvUtils::ValidEntry(aEntry))
		return KErrNotSupported;

	if (iEntry.Deleted() || iEntry.StandardFolder())
		return KErrAccessDenied;

	TInt error;

	TBool permanentDataUnchanged = iEntry.PermanentDataUnchanged(aEntry);
	if( permanentDataUnchanged && aForcedUpdate )
		{
		// Check to see if the owner ID is being updated - if so then need to
		// update the permanent data.
		permanentDataUnchanged = (iContextOwnerId == aOwnerId);
		}

	if( permanentDataUnchanged )
		{
		error = iServer.IndexAdapter().ChangeTemporaryData(aEntry);
		if (error==KErrNone)
			iEntry.iData = aEntry.iData;
		}
	else
		{
		error = IncreaseBufferSizes(aEntry.iDescription.Length(), aEntry.iDetails.Length());

		if (error==KErrNone)
			{
			TMsvEntry entry = aEntry;

			// check the hidden flags are correct
			entry.SetOwner(iEntry.Owner());
			entry.SetDeleted(iEntry.Deleted());
			error = iServer.ChangeEntry(entry, aOwnerId, aForcedUpdate, aBulk);
			if (error==KErrNone)
				{
				// switch the context of this object
				*iDescription = entry.iDescription;
				*iDetails = entry.iDetails;
				iEntry = entry;
				iEntry.iDescription.Set(*iDescription);
				iEntry.iDetails.Set(*iDetails);
				}
			}
		}

	// notify everyone of the change unless this is a bulk change.
	// for bulk changes (e.g. email synchronize, all notifications get handled elsewhere
	if ((error == KErrNone) && (!aBulk))
		{
		iServer.NotifyChanged(EMsvEntriesChanged, iEntry.Id(), iEntry.Parent());
		}

	return error;
	}

/**
Creates a new entry as a child of the current context.

The parent ID and entry ID are set by the Message Server.

@param	aEntry
Index entry value for the new entry

@return
KErrNone - success; KErrNoMemory - a memory allocation failed;
KErrNotSupported - aEntry is invalid
*/
EXPORT_C TInt CMsvServerEntry::CreateEntry(TMsvEntry& aEntry)
	{
	return CreateEntry(aEntry, KMsvServerId);
	}

/**
Creates a new entry as a child of the current context.

Ownership of the created entry is given to the process with the specified SID.

The parent ID and entry ID are set by the Message Server.

@param	aEntry
Index entry value for the new entry

@param	aOwnerId
The SID of the process that will own the create entry.

@return
KErrNone - success; KErrNoMemory - a memory allocation failed;
KErrNotSupported - aEntry is invalid
*/
EXPORT_C TInt CMsvServerEntry::CreateEntry(TMsvEntry& aEntry, TSecureId aOwnerId)
	{
	return CreateEntry(aEntry, aOwnerId, EFalse);
	}

/**
Creates a new entry as a child of the current context as part of a
bulk creation operation. The entry will not be committed to file immediately.

The parent ID and entry ID are set by the Message Server.

@param	aEntry
Index entry value for the new entry

@param	aOwnerId
The SID of the process that will own the create entry.

@return
KErrNone - success; KErrNoMemory - a memory allocation failed;
KErrNotSupported - aEntry is invalid
*/
EXPORT_C TInt CMsvServerEntry::CreateEntryBulk(TMsvEntry& aEntry, TSecureId aOwnerId)
	{
	return CreateEntry(aEntry, aOwnerId, ETrue);
	}
/**
Creates a new entry as a child of the current context as part of a
bulk creation operation

The parent ID and entry ID are set by the Message Server.

@param	aEntry
Index entry value for the new entry

@return
KErrNone - success; KErrNoMemory - a memory allocation failed;
KErrNotSupported - aEntry is invalid
*/
EXPORT_C TInt CMsvServerEntry::CreateEntryBulk(TMsvEntry& aEntry)
	{
	return CreateEntry(aEntry, KMsvServerId, ETrue);
	}



/**
Creates a new entry as a child of the current context.

Ownership of the created entry is given to the process with the specified SID.

The parent ID and entry ID are set by the Message Server.

@param	aEntry
Index entry value for the new entry

@param	aOwnerId
The SID of the process that will own the create entry.

@param aBulk
A boolean value to indicate whether this is part of a bulk operation. (ETrue = bulk)

@return
KErrNone - success; KErrNoMemory - a memory allocation failed;
KErrNotSupported - aEntry is invalid
*/
EXPORT_C TInt CMsvServerEntry::CreateEntry(TMsvEntry& aEntry, TSecureId aOwnerId, TBool aBulk)
	{
	__ASSERT_DEBUG(iEntry.Id()!=KMsvNullIndexEntryId, PanicServer(EMsvEntryWithNoContext2));
	aEntry.SetParent(iEntry.Id());
	__ASSERT_DEBUG(MsvUtils::ValidEntry(aEntry, ETrue), PanicServer(EMsvBadEntryContents));

	// if valid - try to create the new child
	TInt error;
	if (iEntry.Id()==KMsvNullIndexEntryId || !MsvUtils::ValidEntry(aEntry, ETrue))
		error = KErrNotSupported;
	else
		{
		error = iServer.AddEntry(aEntry, aOwnerId, ETrue, aBulk);

		// if the child was created,then notify everyone, otherwise reset the parent
		if (error)
			aEntry.SetParent(KMsvNullIndexEntryId);
		else
			{
			if (!aBulk)
				{
				iServer.NotifyChanged(EMsvEntriesCreated, aEntry.Id(), aEntry.Parent());
				}
			iEntry.SetOwner(ETrue);
			}
		}

	return error;
	}

EXPORT_C void CMsvServerEntry::CompleteBulk()
//
/** Completes the current bulk transaction (if any)
 Requests that the message server commit to the index file on disk
 any entries which have not been committed and to generate notifications
 for any entries which require them.
 @return void */
	{
	iServer.CompleteBulkTransaction();
	}



TBool CMsvServerEntry::AreChildren(const CMsvEntrySelection& aSelection) const
//
// Returns true if all the entries are children
//
	{
	TInt count = aSelection.Count();
	while (count--)
		{
		if (!IsAChild(aSelection.At(count)))
			{
			return EFalse;
			}
		}
	return ETrue;
	}

TBool CMsvServerEntry::IsAChild(TMsvId aId) const
//
//
//
	{
	TMsvEntry* entry=NULL;
	TInt err = KErrNone;
	err = iServer.IndexAdapter().GetEntry(aId, entry);
	if (err ==KErrNone && entry->Parent()==iEntry.Id())
		return ETrue;
	return EFalse;
	}


EXPORT_C TInt CMsvServerEntry::DeleteEntry(TMsvId aId)
//
// Deletes the child of the current context recursively.
//
/** Deletes a child entry of the context. The delete works recursively through
all the descendants.

If a child or any descendant is locked by another client, then no entries
are deleted.

@param aId The ID of the entry to delete
@return KErrNone if successful, KErrAccessDenied if the entry or a descendant
was locked by another client, KErrInUse if the store or a file associated
with the entry is open, or KErrNotFound if the entry is not a child of the
context. */
	{
	__ASSERT_DEBUG(iEntry.Id()!=KMsvNullIndexEntryId, PanicServer(EMsvEntryWithNoContext4));
	__ASSERT_DEBUG(aId!=iEntry.Id(), PanicServer(EMsvDeletingCurrentContext));

	// only delete children
	if (!IsAChild(aId))
		return KErrNotFound;

	// get the total selection of entries to be deleted
	CMsvEntrySelection* movedEntries=NULL;
	CMsvEntrySelection* deletedEntries=NULL;
	TRAPD(error, DoDeleteEntryL(aId, deletedEntries, movedEntries));

	// notify server of the deletions & moves
	if (deletedEntries && movedEntries && (deletedEntries->Count() || movedEntries->Count()))
		{
		if (deletedEntries->Count())
			iServer.NotifyChanged(EMsvEntriesDeleted, *deletedEntries, iEntry.Id());
		if (movedEntries->Count())
			iServer.NotifyChanged(EMsvEntriesMoved, *movedEntries, KMsvDeletedEntryFolderEntryId, iEntry.Id());

		// need to remove owner flag if has no children
		TMsvEntry* pEntry;
		TInt err = KErrNone;
		err = iServer.IndexAdapter().GetEntry(iEntry.Id(), pEntry);
		if (err ==KErrNone)
			iEntry.SetOwner(pEntry->Owner());
		}

	delete movedEntries;
	delete deletedEntries;

	return error;
	}

void CMsvServerEntry::DoDeleteEntryL(TMsvId aId, CMsvEntrySelection*& aDeleted, CMsvEntrySelection*& aMoved)
	{
	aDeleted = new(ELeave)CMsvEntrySelection;
	aMoved = new(ELeave)CMsvEntrySelection;

	CMsvDelete* del = CMsvDelete::NewL(iServer);
	CleanupStack::PushL(del);

	del->StartL(aId, *aDeleted, *aMoved);

	CleanupStack::PopAndDestroy(); // del
	}

EXPORT_C TInt CMsvServerEntry::DeleteEntries(CMsvEntrySelection& aSelection)
//
// Deletes the children of the current context in the selection recursively
// Returns the children that could not be fully deleted in the selection
//
/** Deletes a selection of child entries. The delete works recursively through
all the descendants.

If a child or any descendant is locked by another client, then no entries
are deleted.

@param aSelection The entries to delete. On return, contains the children
that could not be fully deleted
@return KErrNone if successful, KErrAccessDenied if the entry or a descendant
was locked by another client, KErrInUse if the store or a file associated
with the entry is open, or KErrNotFound if the entry is not a child of the
context. */
	{
	__ASSERT_DEBUG(iEntry.Id()!=KMsvNullIndexEntryId, PanicServer(EMsvEntryWithNoContext4));

	CMsvEntrySelection* deleted = NULL;
	CMsvEntrySelection* moved = NULL;

	TRAPD(error, DoDeleteEntriesL(aSelection, deleted, moved));

	if (moved && deleted)
		{
		// Notify server of the deletions & moves
		if (deleted->Count())
			iServer.NotifyChanged(EMsvEntriesDeleted, *deleted, iEntry.Id());

		if (moved->Count())
			iServer.NotifyChanged(EMsvEntriesMoved, *moved, KMsvDeletedEntryFolderEntryId, iEntry.Id());

		// need to remove owner flag if has no children
		TMsvEntry* pEntry;
		TInt err = KErrNone;
		err = iServer.IndexAdapter().GetEntry(iEntry.Id(), pEntry);
		if (err ==KErrNone)
			iEntry.SetOwner(pEntry->Owner());
		}

	delete moved;
	delete deleted;

	return error;
	}

void CMsvServerEntry::DoDeleteEntriesL(CMsvEntrySelection& aSelection, CMsvEntrySelection*& aDeleted, CMsvEntrySelection*& aMoved)
//
//
//
	{
	__ASSERT_DEBUG(!aDeleted && !aMoved, PanicServer(EMsvDeleteAndMoveSelectionsNotNull));
	__ASSERT_DEBUG(aSelection.Count() > 0, PanicServer(EMsvDeletingEmptySelection));

	// Total entries deleted and moved
	aDeleted = new(ELeave)CMsvEntrySelection;
	aMoved = new(ELeave)CMsvEntrySelection;

	// Entries deleted when a single item is deleted
	CMsvEntrySelection* deleted = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(deleted);

	// Entries moved when a single item is deleted
	CMsvEntrySelection* moved = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(moved);

	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

	CMsvDelete* del = CMsvDelete::NewL(iServer);
	CleanupStack::PushL(del);

	TInt firstError = KErrNone;

	TInt count = aSelection.Count();
	while(count--)
		{
		TMsvId id = aSelection.At(count);
		TInt error = KErrNone;

		// Only delete children
		if (!IsAChild(id))
			error = KErrNotFound;
		else
			{
			// Need to know maximum number of entries that might be deleted
			selection->AppendL(id);
			error = iServer.IndexAdapter().ExpandSelectionRecursively(*selection);
			if (error == KErrNone)
				{
				// Reserve space in lists
				aDeleted->SetReserveL(aDeleted->Count() + selection->Count());
				aMoved->SetReserveL(aMoved->Count() + selection->Count());
				del->StartL(id, *deleted, *moved);
				aSelection.Delete(count);

				if (deleted->Count() > 0)
					aDeleted->AppendL(deleted->Back(0), deleted->Count());

				if (moved->Count() > 0)
					aMoved->AppendL(moved->Back(0), moved->Count());
				}

			deleted->Reset();
			moved->Reset();
			selection->Reset();
			}

		// Remember error
		if (error != KErrNone && firstError == KErrNone)
			firstError = error;
		}

	User::LeaveIfError(firstError);
	CleanupStack::PopAndDestroy(4); // del, selection, moved, deleted
	}

/** Gets the index entry for a specified entry ID.

@param aId ID of the entry to get
@param aEntry On return, a pointer to the index entry with ID aId
@return KErrNone on success; KErrNotFound if the no entry exists with the specified ID
*/
EXPORT_C TInt CMsvServerEntry::GetEntryFromId(TMsvId aId,TMsvEntry*& aEntry)
//
// sets aEntry to the TMsvEntry according to the TMsvId, returns error code
//
	{
	return iServer.IndexAdapter().GetEntry(aId, aEntry);
	}


EXPORT_C CMsvStore* CMsvServerEntry::ReadStoreL()
//
// Returns the message store for the current context with read access only
//
/** Obtains the message store for the current context with read-only access.

Multiple clients can read from a store simultaneously. If another client is already
writing to the store, the function leaves with KErrAccessDenied.

The returned CMsvStore must be deleted when it is no longer required.

@leave KErrAccessDenied Store is locked by another process
@leave KErrNoMemory Not enough memory to open store
@leave KErrNotFound There is no store associated with this entry
@return Context's message store open for read-only access */
	{
	// Leave if the message store is not currently available
	User::LeaveIfError(iServer.IndexAdapter().ErrorState());
	__ASSERT_ALWAYS(iStore==NULL, PanicServer(EMsvStoreAlreadyOpen2));
	__ASSERT_DEBUG(iEntry.Id()!=KMsvNullIndexEntryId, PanicServer(EMsvEntryWithNoContext6));

	// must have a context
	if (iEntry.Id()==KMsvNullIndexEntryId)
		User::Leave(KErrNotSupported);

	TBool locked;
	User::LeaveIfError(iServer.IndexAdapter().IsStoreLocked(iEntry.Id(), locked));
	if (locked)
		User::Leave(KErrAccessDenied);

#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)	// open the store
		{
		iStore = CMsvStore::OpenForReadL(*this, iServer.FileSession(), iServer.ServerStoreManager(), iEntry.Id(), iEntry.iMtm);
		}
#else
	iStore = CMsvStore::OpenForReadL(*this, iServer.FileSession(), iServer.ServerStoreManager(), iEntry.Id());
#endif
	// error ignored as the entry exists, we know this because IsStoreLocked would have returned and error otherwise
	iServer.IndexAdapter().IncStoreReaderCount(iEntry.Id());
	return iStore;
	}


EXPORT_C CMsvStore* CMsvServerEntry::EditStoreL()
//
// Returns the message store for the current context with write access
//
/** Obtains the message store for the current context with read-write access.

Only one client can edit a message store at one time. If another client is
already writing to the store, KErrAccessDenied is returned. However, any number
of clients can read from the store simultaneously.

If the message store does not exist when EditStore() is called, a new message
store is created.

The returned CMsvStore must be deleted when it is no longer required.

@leave KErrAccessDenied Store is locked by another process or is read-only
@leave KErrNoMemory Not enough memory to open store
@return Context's message store open for read-write access */
	{
	__ASSERT_ALWAYS(iStore==NULL, PanicServer(EMsvStoreAlreadyOpen1));
	__ASSERT_DEBUG(iEntry.Id()!=KMsvNullIndexEntryId, PanicServer(EMsvEntryWithNoContext7));

	// Leave if the message store is not currently available
	User::LeaveIfError(iServer.IndexAdapter().ErrorState());
	// must have a context
	if (iEntry.Id()==KMsvNullIndexEntryId)
		User::Leave(KErrNotSupported);

	if (iEntry.ReadOnly())
		User::Leave(KErrAccessDenied);
	User::LeaveIfError(iServer.IndexAdapter().LockStore(iEntry.Id()));
	// open the store
	TInt error = 0;
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
		{
		TRAP(error,  iStore = CMsvStore::OpenForWriteL(*this, iServer.FileSession(), iServer.ServerStoreManager(), iEntry.Id(), iEntry.iMtm));
		}
#else
	TRAP(error,  iStore = CMsvStore::OpenForWriteL(*this, iServer.FileSession(), iServer.ServerStoreManager(), iEntry.Id()));
#endif
	if (error)
		{
		iServer.IndexAdapter().ReleaseStore(iEntry.Id());
		User::Leave(error);
  		}

	return iStore;
	}



void CMsvServerEntry::HandleStoreEvent(TMsvStoreEvent aEvent, TMsvId aId)
//
//
//
	{
	__ASSERT_DEBUG(aId==iEntry.Id(), PanicServer(EMsvUnknownStoreId));

	switch (aEvent)
		{
		case EMsvEditStoreClosed:
			iServer.IndexAdapter().ReleaseStore(aId); // error ignored
			iStore=NULL;
			break;
		case EMsvReadStoreClosed:
			iServer.IndexAdapter().DecStoreReaderCount(aId); // error ignored
			iStore=NULL;
			break;
		default:
			__ASSERT_DEBUG(EFalse, PanicServer(EMsvUnknownStoreEvent2));
		}
	}

EXPORT_C TInt CMsvServerEntry::GetChildren(CMsvEntrySelection& aSelection)
//
// Returns a the selection of the children of the current context
//
/** Gets a selection containing the IDs of all the context children.

If the entry has no children, the selection is empty.

@param aSelection Initially, this must be an empty selection. On return, it
lists the children.
@return KErrNone - success;
KErrMemory - a memory allocation failed */
	{
	return DoGetChildren(KMsvNullIndexEntryId, KNullUid, KNullUid, aSelection);
	}

EXPORT_C TInt CMsvServerEntry::GetChildrenWithService(TMsvId aServiceId, CMsvEntrySelection& aSelection)
//
// Returns a the selection of the children of the current context with the given service
//
/** Gets a selection containing the IDs of all the context children with the specified
service.

If the entry has no children, the selection is empty.

@param aServiceId Service by which to filter children
@param aSelection Initially, this must be an empty selection. On return, it
lists the children.
@return KErrNone - success;
KErrMemory - a memory allocation failed */
	{
	return DoGetChildren(aServiceId, KNullUid, KNullUid, aSelection);
	}

EXPORT_C TInt CMsvServerEntry::GetChildrenWithMtm(TUid aMtm, CMsvEntrySelection& aSelection)
//
// Returns a the selection of the children of the current context with the given Mtm
//
/** Gets a selection containing the IDs of all the context children with the specified
MTM.

If the entry has no children, the selection is empty.

@param aMtm MTM by which to filter children
@param aSelection Initially, this must be an empty selection. On return, it
lists the children.
@return KErrNone - success;
KErrMemory - a memory allocation failed */
	{
	return DoGetChildren(KMsvNullIndexEntryId, aMtm, KNullUid, aSelection);
	}

EXPORT_C TInt CMsvServerEntry::GetChildrenWithType(TUid aType, CMsvEntrySelection& aSelection)
//
// Returns a the selection of the children of the current context with the given type
//
/** Gets a selection containing the IDs of all the context children with the specified
entry type.

If the entry has no children, the selection is empty.

@param aType Entry type by which to filter children
@param aSelection Initially, this must be an empty selection. On return, it
lists the children.
@return KErrNone - success;
KErrMemory - a memory allocation failed */
	{
	return DoGetChildren(KMsvNullIndexEntryId, KNullUid, aType, aSelection);
	}

TInt CMsvServerEntry::DoGetChildren(TMsvId aServiceId, TUid aMtm, TUid aType, CMsvEntrySelection& aSelection)
	{
	TRAPD(err, DoGetChildrenL(aServiceId, aMtm, aType, aSelection));
	return err;
	}

void CMsvServerEntry::DoGetChildrenL(TMsvId aServiceId, TUid aMtm, TUid aType, CMsvEntrySelection& aSelection)
	{
	__ASSERT_DEBUG(iEntry.Id()!=KMsvNullIndexEntryId, PanicServer(EMsvEntryWithNoContext8));

	CMsvEntryFilter* filter = CMsvEntryFilter::NewLC();

	filter->SetService(aServiceId);
	filter->SetMtm(aMtm);
	filter->SetType(aType);
	filter->SetOrder(iOrdering);
	filter->SetSortMtm(iMtm);

	aSelection.Reset();
	User::LeaveIfError(iServer.IndexAdapter().GetChildrenId(iEntry.Id(), *filter, aSelection));
	CleanupStack::PopAndDestroy(); // filter
	}

EXPORT_C CMsvServerEntry* CMsvServerEntry::NewEntryL(TMsvId aId)
//
// Creates a new server entry object set with entry aId as context
//
/** Gets a new CMsvServerEntry object for the specified entry ID.

The call locks the entry, preventing it being accessed by other clients.

The object must be deleted when it is no longer required. The lock is released
when the object is deleted or the context is changed with SetEntry().

@param aId ID of the entry to access
@leave KErrLocked Entry is locked
@leave KErrNoMemory A memory allocation failed
@leave KErrNotFound The entry does not exist
@return If the function succeeds, this is a pointer to a newly allocated and
initialised object. */
	{
	return CMsvServerEntry::NewL(iServer, aId);
	}


EXPORT_C TInt CMsvServerEntry::MoveEntryWithinService(TMsvId aId, TMsvId aDestination)
//
// Moves the child of the current context
//
/** Moves a child of the context to under another entry. All descendants will be
moved as well. The destination must belong to the same service as the context.

If an error occurs, no changes are made.

For pre-Unicode releases see the synchronous overload of MoveEntry().

@param aId The ID of the entry to move
@param aDestination The ID of new parent
@return KErrNone if successful, KErrArgument if the destination is a child
of aId, KErrInUse if the store or a file associated with the entry is open,
KErrNotFound if the aId is not a child of the context KErrPathNotFound, if
the destination does not exist. */
	{
	__ASSERT_DEBUG(iEntry.Id()!=KMsvNullIndexEntryId, PanicServer(EMsvEntryWithNoContext10));

	// only move children
	if (!IsAChild(aId))
		return KErrNotFound;

	// check for moving into its current parent
	if (aDestination==iEntry.Id())
		return KErrNone;

	TRAPD(error, DoMoveEntryL(aId, aDestination));

	// notify server of the move
	if (error==KErrNone)
		{
		iServer.NotifyChanged(EMsvEntriesMoved, aId, aDestination, iEntry.Id());
		// need to remove owner flag if has no children
		TMsvEntry* pEntry;
		TInt err = KErrNone;
		err = iServer.IndexAdapter().GetEntry(iEntry.Id(), pEntry);
		if (err==KErrNone)
			iEntry.SetOwner(pEntry->Owner());
		}

	return error;
	}

void CMsvServerEntry::DoMoveEntryL(TMsvId aId, TMsvId aDestination)
//
//
//
	{
	CMsvMove* move = CMsvMove::NewL(iServer);
	CleanupStack::PushL(move);
	move->StartL(aId, aDestination);
	CleanupStack::PopAndDestroy(); // move
	}

EXPORT_C TInt CMsvServerEntry::MoveEntriesWithinService(CMsvEntrySelection& aSelection, TMsvId aDestination)
//
//
//
/** Moves a child of the context to under another entry. All descendants will be
moved as well. The destination must belong to the same service as the context.

@param aSelection The entries to move. On return, contains the children that
could not be fully moved.
@param aDestination The ID of new parent
@return KErrNone if successful, KErrArgument if the destination is a child
of aSelection entry, KErrInUse if the store or a file associated with an entry
is open, KErrNotFound if an aSelection entry is not a child of the context
the, or KErrPathNotFound if the destination does not exist. */
	{
	__ASSERT_DEBUG(iEntry.Id()!=KMsvNullIndexEntryId, PanicServer(EMsvEntryWithNoContext10));

	CMsvEntrySelection* moved = NULL;

	TRAPD(error, DoMoveEntriesL(aSelection, aDestination, moved));

	if (moved && moved->Count())
		{
		iServer.NotifyChanged(EMsvEntriesMoved, *moved, aDestination, iEntry.Id());

		// need to remove owner flag if has no children
		TMsvEntry* pEntry;
		TInt err = KErrNone;
		err = iServer.IndexAdapter().GetEntry(iEntry.Id(), pEntry);
		if (err==KErrNone)
			iEntry.SetOwner(pEntry->Owner());
		}

	delete moved;
	return error;
	}

void CMsvServerEntry::DoMoveEntriesL(CMsvEntrySelection& aSelection, TMsvId aDestination, CMsvEntrySelection*& aMoved)
	{
	__ASSERT_DEBUG(!aMoved, PanicServer(EMsvMoveSelectionNotNull));
	__ASSERT_DEBUG(aSelection.Count() > 0, PanicServer(EMsvMovingEmptySelection));

	aMoved = new(ELeave)CMsvEntrySelection;
	aMoved->SetReserveL(aSelection.Count());

	CMsvMove* move = CMsvMove::NewL(iServer);
	CleanupStack::PushL(move);

	TInt error = KErrNone;

	TInt count = aSelection.Count();
	while(count--)
		{
		TMsvId id = aSelection.At(count);
		if (!IsAChild(id))
			error = KErrNotFound;
		else
			{
			move->StartL(id, aDestination);
			aSelection.Delete(count);
			aMoved->AppendL(id);
			}
		}
	User::LeaveIfError(error);
	CleanupStack::PopAndDestroy(); // move
	}

EXPORT_C void CMsvServerEntry::MoveEntryL(TMsvId aId, TMsvId aDestination, TRequestStatus& aObserverStatus)
//
//
//
/** Moves a child of the context to another entry that belongs to a different service.
All descendants will be moved as well.

The move is carried out asynchronously. The caller should supply in aObserverStatus
the status word of an active object that it owns. The function will signal
this to be completed when the move is complete.

If the function leaves, no changes are made.

In pre-Unicode versions an asynchronous move can be cancelled through CancelMoveEntry();
in other releases, use Cancel().

@param aId The ID of the entry to move
@param aDestination The ID of new parent
@param aObserverStatus The request status to be completed when the operation
has finished
@leave KErrArgument The destination is a child of aId
@leave KErrInUse The store or a file associated with the entry is open
@leave KErrNoMemory A memory allocation failed
@leave KErrNotFound aId is not a child of the context
@leave KErrPathNotFound The destination does not exist */
	{
	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);
	selection->AppendL(aId);

	MoveEntriesL(*selection, aDestination, aObserverStatus);

	CleanupStack::PopAndDestroy(); // selection
	}

EXPORT_C void CMsvServerEntry::MoveEntriesL(const CMsvEntrySelection& aSelection, TMsvId aDestination, TRequestStatus& aObserverStatus)
//
//
//
/** Moves a selection of children of the context to another entry that belongs
to a different service. All descendants will be moved as well.

The move is carried out asynchronously. The caller should supply in aObserverStatus
the status word of an active object that it owns. The function will signal
this to be completed when the move is complete.

@param aSelection The IDs of the entry to move. On return, contains the children
that could not be fully moved.
@param aDestination The ID of new parent
@param aObserverStatus The request status to be completed when the operation
has finished
@leave KErrArgument The destination is a child of an aSelection entry
@leave KErrInUse The store or a file associated with an entry is open
@leave KErrNoMemory A memory allocation failed
@leave KErrNotFound An aSelection entry is not a child of the context
@leave KErrPathNotFound The destination does not exist */
	{
	__ASSERT_DEBUG(iEntry.Id()!=KMsvNullIndexEntryId, PanicServer(EMsvEntryWithNoContext10));
	__ASSERT_ALWAYS(aDestination!=iEntry.Id(), PanicServer(EMsvMovingToSamePArent));
	__ASSERT_ALWAYS(iEntryState == EMsvIdle, PanicServer(EMsvServerEntryNotIdle));
	__ASSERT_ALWAYS(aSelection.Count() > 0, PanicServer(EMsvEmptySelection));
	__ASSERT_DEBUG(iCopyMove==NULL, PanicServer(EMsvMoveNotCompleted));

	// only move children
	if (!AreChildren(aSelection))
		User::Leave(KErrNotFound);

	CMsvMoveEntries* move = CMsvMoveEntries::NewL(iServer);
	CleanupStack::PushL(move);

	move->StartL(aSelection, aDestination, iStatus);

	CleanupStack::Pop(); // move

	iCopyMove=move;
	iEntryState = EMsvMoving;
	iObserverStatus = &aObserverStatus;
	*iObserverStatus = KRequestPending;

	SetActive();
	}

EXPORT_C void CMsvServerEntry::CopyEntryL(TMsvId aId, TMsvId aDestination, TRequestStatus& aObserverStatus)
//
// Recursively copies a child of the context to another entry that belongs to a different service.
//
/** Copies a child of the context to another entry. All descendants will be copied
as well.

The copy is carried out asynchronously. The caller should supply in aObserverStatus
the status word of an active object that it owns. The function will signal
this to be completed when the copy is complete.

If the function leaves, no changes are made.

@param aId The ID of the entry to copy
@param aDestination The ID of new parent
@param aObserverStatus The request status to be completed when the operation
has finished
@leave KErrArgument The destination is a child of an aSelection entry
@leave KErrInUse The store or a file associated with an entry is open
@leave KErrNoMemory A memory allocation failed
@leave KErrNotFound An aSelection entry is not a child of the context
@leave KErrPathNotFound The destination does not exist */
	{
	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);
	selection->AppendL(aId);
	iCompletedSelection = NULL;
	iCompletedEntryId = NULL;

	DoCopyEntriesL(*selection, aDestination, aObserverStatus);

	CleanupStack::PopAndDestroy(); // selection
	}

EXPORT_C void CMsvServerEntry::CopyEntryL(TMsvId aId, TMsvId aDestination, TMsvId& aCompletedEntry, TRequestStatus& aObserverStatus)
//
//
//
/** Copies a child of the context to another entry. All descendants will be copied
as well.

This overload returns the ID of the new entry.

The copy is carried out asynchronously. The caller should supply in aObserverStatus
the status word of an active object that it owns. The function will signal
this to be completed when the copy is complete.

If the function leaves, no changes are made.

@param aId The ID of the entry to copy
@param aDestination The ID of new parent
@param aCompletedEntry On return, the ID of the new entry
@param aObserverStatus The request status to be completed when the operation
has finished
@leave KErrArgument The destination is a child of an aSelection entry
@leave KErrInUse The store or a file associated with an entry is open
@leave KErrNoMemory A memory allocation failed
@leave KErrNotFound An aSelection entry is not a child of the context
@leave KErrPathNotFound The destination does not exist */
	{
	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);
	selection->AppendL(aId);
	iCompletedSelection = NULL;
	iCompletedEntryId = &aCompletedEntry;

	DoCopyEntriesL(*selection, aDestination, aObserverStatus);

	CleanupStack::PopAndDestroy(); // selection
	}

EXPORT_C void CMsvServerEntry::CopyEntriesL(const CMsvEntrySelection& aSelection, TMsvId aDestination, TRequestStatus& aObserverStatus)
//
//
//
/** Copies a selection of children of the context to another entry that belongs
to a different service. All descendants will be copied as well.

The copy is carried out asynchronously. The caller should supply in aObserverStatus
the status word of an active object that it owns. The function will signal
this to be completed when the copy is complete.

@param aSelection The IDs of the entry to copy. On return, contains the children
that could not be fully copied.
@param aDestination The ID of new parent
@param aObserverStatus The request status to be completed when the operation
has finished
@leave KErrArgument The destination is a child of an aSelection entry
@leave KErrInUse The store or a file associated with an entry is open
@leave KErrNoMemory A memory allocation failed
@leave KErrNotFound An aSelection entry is not a child of the context
@leave KErrPathNotFound The destination does not exist */
	{
	iCompletedSelection = NULL;
	iCompletedEntryId = NULL;
	DoCopyEntriesL(aSelection, aDestination, aObserverStatus);
	}

EXPORT_C void CMsvServerEntry::CopyEntriesL(const CMsvEntrySelection& aSelection, TMsvId aDestination, CMsvEntrySelection& aCompletedSelection, TRequestStatus& aObserverStatus)
//
// Recursively copies a selection of children of the context to another entry that belongs to a different service.
// Will return the target entries through the aCompletedSelection parameter.
//
/** Copies a selection of children of the context to another entry that belongs
to a different service. All descendants will be copied as well.

This overload returns the IDs of the new entries.

The copy is carried out asynchronously. The caller should supply in aObserverStatus
the status word of an active object that it owns. The function will signal
this to be completed when the copy is complete.

@param aSelection The IDs of the entry to copy. On return, contains the children
that could not be fully copied.
@param aDestination The ID of new parent
@param aCompletedSelection On return, the IDs of the new entries.
@param aObserverStatus The request status to be completed when the operation
has finished
@leave KErrArgument The destination is a child of an aSelection entry
@leave KErrInUse The store or a file associated with an entry is open
@leave KErrNoMemory A memory allocation failed
@leave KErrNotFound An aSelection entry is not a child of the context
@leave KErrPathNotFound The destination does not exist */
	{
	iCompletedSelection = &aCompletedSelection;
	iCompletedEntryId = NULL;
	DoCopyEntriesL(aSelection, aDestination, aObserverStatus);
	}

void CMsvServerEntry::DoCopyEntriesL(const CMsvEntrySelection& aSelection, TMsvId aDestination, TRequestStatus& aObserverStatus)
//
//
//
	{
	__ASSERT_DEBUG(iEntry.Id()!=KMsvNullIndexEntryId, PanicServer(EMsvEntryWithNoContext10));
	__ASSERT_ALWAYS(aDestination!=iEntry.Id(), PanicServer(EMsvCopyingToSameParent));
	__ASSERT_ALWAYS(iEntryState == EMsvIdle, PanicServer(EMsvServerEntryNotIdle));
	__ASSERT_ALWAYS(aSelection.Count() > 0, PanicServer(EMsvEmptySelection));
	__ASSERT_DEBUG(iCopyMove==NULL, PanicServer(EMsvCopyNotCompleted));

	// only copy children
	if (!AreChildren(aSelection))
		User::Leave(KErrNotFound);

	CMsvCopyEntries* copy = CMsvCopyEntries::NewL(iServer);
	CleanupStack::PushL(copy);
	copy->StartL(aSelection, aDestination, iStatus);
	CleanupStack::Pop(); // copy

	iCopyMove=copy;
	iEntryState = EMsvCopying;
	iObserverStatus = &aObserverStatus;
	*iObserverStatus = KRequestPending;

	SetActive();
	}
void CMsvServerEntry::RunL()
//
//
//
	{
	__ASSERT_DEBUG(iEntryState != EMsvIdle, PanicServer(EMsvServerEntryIdle));
	__ASSERT_DEBUG(iCopyMove, PanicServer(EMsvCopyMoveCompletionMissing));

	if (iCopyMove->CompletedIds().Count() > 0)
		{
		switch (iEntryState)
			{
			case EMsvMoving:
				{
				iServer.NotifyChanged(EMsvEntriesMoved, iCopyMove->CompletedIds(), iCopyMove->TargetId(), iEntry.Id());

				TMsvEntry* pEntry;
				TInt err = KErrNone;
				err = iServer.IndexAdapter().GetEntry(iEntry.Id(), pEntry);
				if (err ==KErrNone)
					iEntry.SetOwner(pEntry->Owner());
				break;
				}

			case EMsvCopying:
				{
				const CMsvEntrySelection& newEntries = static_cast<CMsvCopyEntries*>(iCopyMove)->NewEntryIds();
				iServer.NotifyChanged(EMsvEntriesCreated, newEntries, iCopyMove->TargetId());

				if (iCompletedSelection)
					{
					iCompletedSelection->Reset();
					TInt count = newEntries.Count();

					while (count--)
						iCompletedSelection->AppendL(newEntries[count]);
					}
				else if (iCompletedEntryId)
					*iCompletedEntryId = newEntries[0];

				break;
				}

			default:
				break;
			}
		}

	delete iCopyMove;
	iCopyMove = NULL;
	iEntryState = EMsvIdle;

	User::RequestComplete(iObserverStatus, iStatus.Int());
	}


void CMsvServerEntry::DoCancel()
//
//
//
	{
	__ASSERT_DEBUG(iEntryState != EMsvIdle, PanicServer(EMsvServerEntryIdle));
	__ASSERT_DEBUG(iCopyMove, PanicServer(EMsvCopyMoveCancelMissing));

	iCopyMove->Cancel();

	delete iCopyMove;
	iCopyMove=NULL;
	iEntryState = EMsvIdle;

	User::RequestComplete(iObserverStatus, KErrCancel);
	}


EXPORT_C TMsvId CMsvServerEntry::OwningService() const
//
//
//
/** Gets the ID of the service that owns the context.

Local entries are considered as being members of the local service.

If the entry is the root, then the root ID (KMsvRootIndexEntryId) is returned.

@return ID of the service that owns this entry */
	{
	if (iEntry.Id()==KMsvRootIndexEntryId)
		return KMsvRootIndexEntryId;
	TMsvId owningService;
	iServer.IndexAdapter().OwningService(iEntry.Id(), owningService); // error ignored as entry obviously exists
	return owningService;
	}


EXPORT_C TInt CMsvServerEntry::ChangeAttributes(const CMsvEntrySelection& aSelection, TUint aSetAttributes, TUint aClearAttributes)
//
//
//
/** Provides a quick way to set or clear multiple fields in a selection of entries.

Fields to change are specified using a bitmask of TMsvAttribute values. Possible
fields that can be changed using this function are:

1. PC synchronisation

2. Visibility flag

3. Read flag

4. In-preparation flag

5. Connected flag

6. New flag

@param aSelection The entries to change
@param aSetAttributes A bitmask of the fields to set
@param aClearAttributes A bitmask of the fields to clear
@return KErrNone if successful, otherwise one of the system-wide error codes.
KErrNotFound if a specified entry does not exist.
@see TMsvAttribute */
	{
	__ASSERT_DEBUG(iEntry.Id()!=KMsvNullIndexEntryId, PanicServer(EMsvEntryWithNoContext11));

	// only change children
	if (!AreChildren(aSelection))
		return KErrNotFound;

	// get a copy of the selection
	CMsvEntrySelection* changedEntries=NULL;
	TRAPD(error, changedEntries = aSelection.CopyL());
	if (error)
		return error;

	// lock all the selection
	TInt count1=aSelection.Count();
	while (count1--)
		{
		error = iServer.IndexAdapter().LockEntry(aSelection.At(count1));
		if (error)
			break;
		}

	// change the attributes if all were locked
	if (error==KErrNone)
		{
		error = iServer.IndexAdapter().ChangeAttributes(*changedEntries, aSetAttributes, aClearAttributes);
		}

	// release all that were locked
	TInt count2=aSelection.Count();
	while (--count2>count1)
		{
		iServer.IndexAdapter().ReleaseEntry(aSelection.At(count2));
		}


	// notify server if any have been changed
	if (error==KErrNone && changedEntries->Count())
		iServer.NotifyChanged(EMsvEntriesChanged, *changedEntries, iEntry.Id());

	delete changedEntries;

	return error;
	}


EXPORT_C TBool CMsvServerEntry::HasStoreL() const
/** Tests if the context has an associated message store.

@return ETrue: entry has a message store EFalse: entry does not have a message
store */
	{
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	if(iServer.ServerStoreManager().DoesAnyStoreExists(iEntry.Id(), iEntry.iMtm))
		return ETrue;
	else
		return iServer.ServerStoreManager().FileStoreExistsL(iEntry.Id());
#else
	return iServer.ServerStoreManager().FileStoreExistsL(iEntry.Id());
#endif
	}


EXPORT_C RFs& CMsvServerEntry::FileSession()
/** Allows a Server-side MTM to access the file session handle created by the Message
Server. This is preferable, as more efficient, to creating another handle.

@return File session handle */
	{
	return iServer.FileSession();
	}
	
	
