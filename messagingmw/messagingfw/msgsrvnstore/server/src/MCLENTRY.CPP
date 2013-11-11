// Copyright (c) 1998-2010 Nokia Corporation and/or its subsidiary(-ies).
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

#ifdef _DEBUG
#undef _NO_SESSION_LOGGING_
#endif

#include <s32std.h>
#include <txtrich.h>
#include "MSVIDS.H"
#include "MSVUIDS.H"

#include "MSVAPI.H"
#include "MSVCOP.H"
#include "MCLENTRY.H"
#include "MSVPANIC.H"
#include "MSVUTILS.H"

#include <mmsvstoremanager.h>

const TInt KMsvClientEntryArrayGranuality=8;
const TInt KMsvEntryObserverArrayGranuality=4;
const TInt KMsvMtmListGranularity=8;

//**********************************
// CMsvClientEntry
//**********************************

// static 
CMsvClientEntry* CMsvClientEntry::NewLC(const TMsvEntry& aEntry, TMsvClientEntryType aType)
	{
	CMsvClientEntry* self = new(ELeave) CMsvClientEntry(aEntry, aType);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}


CMsvClientEntry::CMsvClientEntry(const TMsvEntry& aEntry, TMsvClientEntryType aType)
:iEntry(aEntry), iType(aType)
	{
	__DECLARE_NAME(_S("CMsvClientEntry"));
	}


CMsvClientEntry::~CMsvClientEntry()
	{
	delete iDescription;
	delete iDetails;
	}

void CMsvClientEntry::ConstructL()
	{
	iDescription = HBufC::NewL(iEntry.iDescription.Length());
	iDescription->Des().Copy(iEntry.iDescription);
	iEntry.iDescription.Set(iDescription->Des());
	iDetails = HBufC::NewL(iEntry.iDetails.Length());
	iDetails->Des().Copy(iEntry.iDetails);
	iEntry.iDetails.Set(iDetails->Des());
	}




//**********************************
// CMsvEntry
//**********************************
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)

EXPORT_C CMsvEntry* CMsvEntry::NewL(CMsvSession& aMsvSession, TMsvId aMsvId, const TMsvSelectionOrdering& aOrdering, TBool aChildrenOfAvailableDrives)
/** Creates a new CMsvEntry for the specified entry ID. 

Note that this function does not create a new entry, but simply a new object 
to access an existing entry. To create a new entry, use CreateL().

@param aMsvSession The client’s Message Server session 
@param aMsvId ID of the entry to access 
@param aSortType  The initial sort order of children of the entry, for example, 
when returned by ChildrenL(). The order can be changed later by SetSortTypeL().
@param aChildrenOfAvailableDrives Indicates whether children from all available drives are 
to be fetched during construction of this entry. However, a value of true is valid only
if aMsvId is one of the standard folders, i.e. Inbox, Outbox, Drafts, Sent or Deleted.

@return If the function succeeds, this is a pointer to a newly allocated and initialised object. 
@leave KErrNotFound The requested entry does not exist.
@leave KErrNoMemory A memory allocation failed.
@leave KErrArgument aChildrenOfAvailableDrives is set to true and the requested entry is not 
one of the standard folders, i.e. Inbox, Outbox, Drafts, Sent or Deleted.
*/
    {
    CMsvEntry* self = new(ELeave) CMsvEntry(aMsvSession, aOrdering, aChildrenOfAvailableDrives);
    CleanupStack::PushL(self);
    self->ConstructL(aMsvId);
    CleanupStack::Pop();
    return self;
    }


CMsvEntry::CMsvEntry(CMsvSession& aMsvSession, const TMsvSelectionOrdering& aOrdering, TBool aChildrenOfAvailableDrives)
:iState(EValid), iMsvSession(aMsvSession), iOrdering(aOrdering), iChildrenOfAvailableDrives(aChildrenOfAvailableDrives)
    {
    __DECLARE_NAME(_S("CMsvEntry"));
    }


EXPORT_C void CMsvEntry::SetStandardFolderEntryL(TMsvId aId)
//
// Changes the context to another entry
// If the function leaves, the context is unchanged
//
/** Sets the context to the specified entry while also fetching
children from all available drives. This function can be used to
set the context to only the standard folders i.e. Inbox, Outbox,
Drafts, Sent or Deleted.

If the function leaves, the context is unchanged.

@param aId ID of the message entry which is to become the new context 
@leave KErrNotFound aId could not be found in the index.
@leave KErrArgument aId is not one of the standard folders, i.e. Inbox,
Outbox, Drafts, Sent or Deleted folders.
*/
    {
    __ASSERT_ALWAYS(iStore==NULL, PanicServer(EMsvStoreLeftOpen));

    if (aId==iEntryPtr->Id() && iState==EValid)
        return;
    
    SetEntryNoCheckL(aId, ETrue);
    }



#endif           // #if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)


EXPORT_C CMsvEntry* CMsvEntry::NewL(CMsvSession& aMsvSession, TMsvId aMsvId, const TMsvSelectionOrdering& aOrdering)
//
//
//
/** Creates a new CMsvEntry for the specified entry ID. 

Note that this function does not create a new entry, but simply a new object 
to access an existing entry. To create a new entry, use CreateL().

@param aMsvSession The client’s Message Server session 
@param aMsvId ID of the entry to access 
@param aSortType  The initial sort order of children of the entry, for example, 
when returned by ChildrenL(). The order can be changed later by SetSortTypeL(). 
@return If the function succeeds, this is a pointer to a newly allocated and initialised object. 
@leave KErrNotFound The requested entry does not exist 
@leave KErrNoMemory A memory allocation failed 
*/
	{
	CMsvEntry* self = new(ELeave) CMsvEntry(aMsvSession, aOrdering);
	CleanupStack::PushL(self);
	self->ConstructL(aMsvId);
	CleanupStack::Pop();
	return self;
	}


CMsvEntry::CMsvEntry(CMsvSession& aMsvSession, const TMsvSelectionOrdering& aOrdering)
:iState(EValid), iMsvSession(aMsvSession), iOrdering(aOrdering)
//
//
//
	{
	__DECLARE_NAME(_S("CMsvEntry"));
	}


EXPORT_C CMsvEntry::~CMsvEntry()
//
//
//
/** Destructor. 

This cleans up the object. CMsvEntry objects must be deleted by client applications 
when they are no longer required. Note that deleting a CMsvEntry object does 
not delete the context, simply the immediate means of accessing it. */
	{
	__ASSERT_ALWAYS(iStore==NULL, PanicServer(EMsvStoreLeftOpenOnDestruction2));

	if (iOberserverAdded)
		iMsvSession.RemoveObserver(*this);

	if (iEntries)
		iEntries->ResetAndDestroy();

	delete iEntries;
	delete iSortedChildren;
	delete iObservers;
	delete iMtmList;
	}

void CMsvEntry::ConstructL(TMsvId aId)
//
//
//
	{
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
    if(iChildrenOfAvailableDrives && !MsvUtils::IsStandardId(aId))
        {
        User::Leave(KErrArgument);
        }
#endif
	iEntries = new (ELeave) CArrayPtrFlat<CMsvClientEntry>(KMsvClientEntryArrayGranuality);
	iMtmList = new(ELeave) CArrayFixFlat<TUid>(KMsvMtmListGranularity);
	iSortedChildren = CMsvEntryArray::NewL(*iMtmList);

	// get the required entry from the server
	CMsvClientEntry* cEntry = DoGetEntryLC(aId, iOwningService);
	cEntry->SetType(EMsvClientContext);
	iEntries->AppendL(cEntry);
	CleanupStack::Pop(); // cEntry
	iEntryPtr = &iEntries->At(0)->Entry();

	// get the children
	DoGetChildrenL();

	// Get the notification sequence number
	iNotifySequence = iMsvSession.Session().NotifySequenceL();

	// get the event notifications from the server
	iMsvSession.AddObserverL(*this);
	iOberserverAdded=ETrue;

#ifndef _NO_SESSION_LOGGING_
	Log(_L("Constructed CMsvEntry on entry %x"), aId); 
#endif
	}

#ifndef _NO_SESSION_LOGGING_
void CMsvEntry::Log(TRefByValue<const TDesC> aFmt, ...)
	{
	VA_LIST list;
	VA_START(list, aFmt);

	// Generate the text
	TBuf<256> buf;
	buf.FormatList(aFmt, list);

	// Write to file
	_LIT(KFormatFile, "CMsvEntry %x: %S");
	iMsvSession.iLog.WriteFormat(KFormatFile, this, &buf);

	// Write to serial
#ifndef _NO_SESSION_LOGGING_SERIAL_
	_LIT(KFormatSerial, "MSGS: CMsvEntry %x: %S");
	RDebug::Print(KFormatSerial, this, &buf);
#endif
	}
#endif

CMsvClientEntry* CMsvEntry::DoGetEntryLC(TMsvId aId, TMsvId& aOwningService)
//
// Gets the entry data from the server and create a CMsvClientEntry around it
//
	{
	TMsvEntry tEntry;
	User::LeaveIfError(iMsvSession.Session().GetEntry(aId, aOwningService, tEntry));
	if (tEntry.iType==KUidMsvRootEntry)
		aOwningService=aId;
	return CMsvClientEntry::NewLC(tEntry, EMsvClientNull);
	}



void CMsvEntry::DoGetChildrenL()
//
// Gets the children of the context and places them in the entries array
// The sorted children pointer array is implicited sorted, as the entries are retrieved
// from the server with the current sort order
//
	{
	__ASSERT_DEBUG(iEntries->Count()==1, PanicServer(EMsvChildEntriesExist1));
	__ASSERT_DEBUG(iSortedChildren->Count()==0, PanicServer(EMsvChildEntriesExist2));

	// get the children from the server - using only the visible flag
	TMsvSelectionOrdering order(KMsvNoGrouping, EMsvSortByNone, iOrdering.ShowInvisibleEntries());
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
    // If child entries from all drives in the device are to be fetched.
    if(iChildrenOfAvailableDrives)
        {
		User::LeaveIfError(iMsvSession.Session().GetChildrenAll(iEntryPtr->Id(), *iEntries, order));
		}
	else
#endif
        {
        User::LeaveIfError(iMsvSession.Session().GetChildren(iEntryPtr->Id(), *iEntries, order));
		}
	
	// add the children to the sorted pointer list - current context is the first entry
	TInt totalCount=iEntries->Count();
	for (TInt count=1; count<totalCount; count++)
		iSortedChildren->AppendL(&iEntries->At(count)->Entry());

	// sort the children
	iSortedChildren->SortL(iOrdering);
	}


EXPORT_C void CMsvEntry::SetEntryL(TMsvId aId)
//
// Changes the context to another entry
// If the function leaves, the context is unchanged
//
/** Sets the context to the specified entry. 

If the function leaves, the context is unchanged.

@param aId ID of the message entry which is to become the new context 
@leave KErrNotFound aId could not be found in the index */
	{
	__ASSERT_ALWAYS(iStore==NULL, PanicServer(EMsvStoreLeftOpen));

	if (aId==iEntryPtr->Id() && iState==EValid)
		return;
	
 	SetEntryNoCheckL(aId);
	}

#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
EXPORT_C void CMsvEntry::SetEntryNoCheckL(TMsvId aId, TBool aChildrenOfAvailableDrives /* DEFAULT=EFalse*/)
//
// Changes the context to another entry
// If the function leaves, the context is unchanged
// The function does not check for when the context is already on
// the given TMsvId.
//
/** Sets the context to the specified entry.
Children from all available drives will be fetched depending on the value of
aChildrenOfAvailableDrives.

If the function leaves, the context is unchanged.

@internalTechnology
@param aId ID of the message entry which is to become the new context
@param aChildrenOfAvailableDrives Indicates whether children from all available drives are to
be fetched during construction of this entry. However, a value of true is valid only
if aMsvId is one among TMsvId's of Inbox, Outbox, Drafts, Sent or Deleted folders.
@leave KErrNotFound aMsvId could not be found in the index 
@leave KErrArgument aChildrenOfAvailableDrives is set to true and the requested entry is not
one of the standard folders, i.e. Inbox, Outbox, Drafts, Sent or Deleted. */
    {
    __ASSERT_ALWAYS(iStore==NULL, PanicServer(EMsvStoreLeftOpen));

    if(aChildrenOfAvailableDrives && !MsvUtils::IsStandardId(aId))
        {
        User::Leave(KErrArgument);
        }

    // create new entry array
    CArrayPtrFlat<CMsvClientEntry>* newEntries = new (ELeave) CArrayPtrFlat<CMsvClientEntry>(KMsvClientEntryArrayGranuality);
    CleanupStack::PushL(newEntries);

    // get the new context and place in array
    TMsvId newOwningService;
    CMsvClientEntry* cEntry = DoGetEntryLC(aId, newOwningService);
    cEntry->SetType(EMsvClientContext);
    newEntries->AppendL(cEntry);
    // create new children array
    CMsvEntryArray* newSortedChildren = CMsvEntryArray::NewL(*iMtmList);

    // keep the old arrays
    CArrayPtrFlat<CMsvClientEntry>* oldEntries = iEntries;
    CMsvEntryArray* oldSortedChildren = iSortedChildren;

    TBool oldFlag = iChildrenOfAvailableDrives;
    iChildrenOfAvailableDrives = aChildrenOfAvailableDrives;

    // use new arrays
    CleanupStack::Pop(2); // cEntry, newEntries
    iEntries = newEntries;
    iSortedChildren = newSortedChildren;
    iEntryPtr = &iEntries->At(0)->Entry();

    // get the children and sort accordingly
    TRAPD(leave, DoGetChildrenL(); iNotifySequence = iMsvSession.Session().NotifySequenceL());

    if (leave)
        {
#ifndef _NO_SESSION_LOGGING_
        Log(_L("Failed to SetEntryL to %x with %d"), aId, leave); 
#endif

        // reset the old context
        iChildrenOfAvailableDrives = oldFlag;

        iEntries = oldEntries;
        iSortedChildren = oldSortedChildren;
        iEntryPtr = &iEntries->At(0)->Entry();
        // cleanup
        newEntries->ResetAndDestroy();
        delete newEntries;
        delete newSortedChildren;
        // propogate leave
        User::Leave(leave);
        }
    else
        {
#ifndef _NO_SESSION_LOGGING_
        Log(_L("SetEntryL to %x, sequence %d"), aId, iNotifySequence); 
#endif

        // set the new owning service
        iOwningService = newOwningService;
        // delete the old context
        oldEntries->ResetAndDestroy();
        delete oldEntries;
        delete oldSortedChildren;
        // make sure the state is marked as valid
        iState = EValid;
        }
    }
#else   //#define SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT

EXPORT_C void CMsvEntry::SetEntryNoCheckL(TMsvId aId)
//
// Changes the context to another entry
// If the function leaves, the context is unchanged
// The function does not check for when the context is already on
// the given TMsvId.
//
/** Sets the context to the specified entry. 

If the function leaves, the context is unchanged.

@internalTechnology
@param aId ID of the message entry which is to become the new context 
@leave KErrNotFound aMsvId could not be found in the index */
	{
	__ASSERT_ALWAYS(iStore==NULL, PanicServer(EMsvStoreLeftOpen));

	// create new entry array
	CArrayPtrFlat<CMsvClientEntry>* newEntries = new (ELeave) CArrayPtrFlat<CMsvClientEntry>(KMsvClientEntryArrayGranuality);
	CleanupStack::PushL(newEntries);

	// get the new context and place in array
	TMsvId newOwningService;
	CMsvClientEntry* cEntry = DoGetEntryLC(aId, newOwningService);
	cEntry->SetType(EMsvClientContext);
	newEntries->AppendL(cEntry);
	// create new children array
	CMsvEntryArray* newSortedChildren = CMsvEntryArray::NewL(*iMtmList);

	// keep the old arrays
	CArrayPtrFlat<CMsvClientEntry>* oldEntries = iEntries;
	CMsvEntryArray* oldSortedChildren = iSortedChildren;

	// use new arrays
	CleanupStack::Pop(2); // cEntry, newEntries
	iEntries = newEntries;
	iSortedChildren = newSortedChildren;
	iEntryPtr = &iEntries->At(0)->Entry();

	// get the children and sort accordingly
	TRAPD(leave, DoGetChildrenL(); iNotifySequence = iMsvSession.Session().NotifySequenceL());

	if (leave)
		{
#ifndef _NO_SESSION_LOGGING_
		Log(_L("Failed to SetEntryL to %x with %d"), aId, leave); 
#endif

		// reset the old context
		iEntries = oldEntries;
		iSortedChildren = oldSortedChildren;
		iEntryPtr = &iEntries->At(0)->Entry();
		// cleanup
		newEntries->ResetAndDestroy();
		delete newEntries;
		delete newSortedChildren;
		// propogate leave
		User::Leave(leave);
		}
	else
		{
#ifndef _NO_SESSION_LOGGING_
		Log(_L("SetEntryL to %x, sequence %d"), aId, iNotifySequence); 
#endif

		// set the new owning service
		iOwningService = newOwningService;
		// delete the old context
		oldEntries->ResetAndDestroy();
		delete oldEntries;
		delete oldSortedChildren;
		// make sure the state is marked as valid
		iState = EValid;
		}
	}
#endif
/**
Creates a new child entry owned by the context asynchronously.

Note that all session observers are notified when a new entry is created with 
an EMsvEntriesCreated event (see TMsvSessionEvent). CMsvEntry objects are 
such session observers themselves. When the object receives such a session 
notification, it calls all registered entry observers with a TMsvEntryEvent 
event EMsvNewChildren, passing in the ID of the new child.

If aEntry is not a service entry, then the context must not be set to the root entry and iServiceId field must be defined .
If aEntry is a service entry, then the context must be set to the root entry.

The returned CMsvOperation object completes when creation is complete.

@param	aEntry
Index entry value for the new entry 

@param	aStatus
The request status to be completed when the operation has finished 

@leave	KErrArgument aEntry is invalid 

@return 
The operation object controlling the create command. 
*/
EXPORT_C CMsvOperation* CMsvEntry::CreateL(const TMsvEntry& aEntry, TRequestStatus& aStatus)
	{
	return CreateL(aEntry, RProcess().SecureId(), aStatus);
	}
	
/**
Creates a new child entry owned by the context asynchronously. Sets the owner of 
the created entry to process specified by the supplied ID.

Note that all session observers are notified when a new entry is created with 
an EMsvEntriesCreated event (see TMsvSessionEvent). CMsvEntry objects are 
such session observers themselves. When the object receives such a session 
notification, it calls all registered entry observers with a TMsvEntryEvent 
event EMsvNewChildren, passing in the ID of the new child.

If aEntry is not a service entry, then the context must not be set to the root entry and iServiceId field must be defined .
If aEntry is a service entry, then the context must be set to the root entry.

The returned CMsvOperation object completes when creation is complete.

@param	aEntry
Index entry value for the new entry 

@param	aOwnerId
The ID of process that owns the created entry.

@param	aStatus
The request status to be completed when the operation has finished 

@leave 	KErrArgument aEntry is invalid 

@return 
The operation object controlling the create command. 
*/
EXPORT_C CMsvOperation* CMsvEntry::CreateL(const TMsvEntry& aEntry, TSecureId aOwnerId, TRequestStatus& aStatus)
	{
#ifndef _NO_SESSION_LOGGING_
	Log(_L("Asynchronous CreateL")); 
#endif

	__ASSERT_DEBUG(iState==EValid || iState==EInvalidDeletedContext || iState==EInvalidChangingContext, PanicServer(EMsvEntryStateUndetermined));

	TMsvEntry entry=aEntry;
	entry.SetParent(iEntryPtr->Id());

	__ASSERT_DEBUG(MsvUtils::ValidEntry(entry, ETrue), PanicServer(EMsvCreatingInvalidEntry));
	if (!MsvUtils::ValidEntry(entry, ETrue))
		User::Leave(KErrArgument);

	CMsvEntryOperation* operation = CMsvEntryOperation::NewL(iMsvSession, aStatus);
	CleanupStack::PushL(operation);
	User::LeaveIfError(iMsvSession.Session().OperationMtmL(iEntryPtr->Id(), operation->iMtm, operation->iService));
	iMsvSession.Session().CreateEntryL(entry, operation->Id(), aOwnerId, operation->iStatus);
	operation->Start();
	CleanupStack::Pop(); // operation
	return operation;
	}
	

/** 
Creates a new child entry owned by the context synchronously. 

Note that all session observers are notified when a new entry is created with 
an EMsvEntriesCreated event (see TMsvSessionEvent). CMsvEntry objects are 
such session observers themselves. When the object receives such a session 
notification, it calls all registered entry observers with a TMsvEntryEvent 
event EMsvNewChildren, passing in the ID of the new child.

If aEntry is not a service entry, then the context must not be set to the root entry and iServiceId field must be defined .
If aEntry is a service entry, then the context must be set to the root entry.

This function can only be used on local entries.

@param	aEntry
Index entry value for the new entry 

@leave	KErrArgument aEntry is invalid
*/
EXPORT_C void CMsvEntry::CreateL(TMsvEntry& aEntry)
	{
	CreateL(aEntry, RProcess().SecureId());
	}
	
/** 
Creates a new child entry owned by the context synchronously. Sets the owner of 
the created entry to process specified by the supplied ID. 

Note that all session observers are notified when a new entry is created with 
an EMsvEntriesCreated event (see TMsvSessionEvent). CMsvEntry objects are 
such session observers themselves. When the object receives such a session 
notification, it calls all registered entry observers with a TMsvEntryEvent 
event EMsvNewChildren, passing in the ID of the new child.

If aEntry is not a service entry, then the context must not be set to the root entry and iServiceId field must be defined .
If aEntry is a service entry, then the context must be set to the root entry.

This function can only be used on local entries.

@param	aEntry
Index entry value for the new entry

@param	aOwnerId
The ID of process that owns the created entry.

@leave	KErrArgument aEntry is invalid
*/
EXPORT_C void CMsvEntry::CreateL(TMsvEntry& aEntry, TSecureId aOwnerId)
	{
#ifndef _NO_SESSION_LOGGING_
	Log(_L("Synchronous CreateL")); 
#endif
	__ASSERT_ALWAYS(iOwningService == KMsvLocalServiceIndexEntryId || iOwningService == KMsvRootIndexEntryId, PanicServer(EMsvNotLocalService));
	__ASSERT_DEBUG(iState==EValid || iState==EInvalidDeletedContext || iState==EInvalidChangingContext, PanicServer(EMsvEntryStateUndetermined));

	TMsvEntry entry=aEntry;
	entry.SetParent(iEntryPtr->Id());

	__ASSERT_DEBUG(MsvUtils::ValidEntry(entry, ETrue), PanicServer(EMsvCreatingInvalidEntry));
	if (!MsvUtils::ValidEntry(entry, ETrue))
		User::Leave(KErrArgument);

	CMsvEntryArray* newSortedChildren = NULL;
	CMsvClientEntry* cEntry = NULL;

	TBool addEntry = entry.Visible() || iOrdering.ShowInvisibleEntries();
	if (addEntry)
		{
		// Create what may be the new child entry if nothing goes wrong
		cEntry = CMsvClientEntry::NewLC(entry, EMsvClientChild);

		// Reserve space for the new entry for later
		iEntries->SetReserveL(iEntries->Count() + 1);

		newSortedChildren = CMsvEntryArray::NewLC(*iMtmList);

		// add the children to the sorted pointer list - current context is the first entry
		TInt totalCount=iEntries->Count();
		for (TInt count=1; count<totalCount; count++)
			newSortedChildren->AppendL(&iEntries->At(count)->Entry());

		// We've created a new sorted child list now so we won't leave later
		newSortedChildren->AppendL(&cEntry->Entry());
		newSortedChildren->SortL(iOrdering);
		}

	TInt id = iMsvSession.OperationId();
	iMsvSession.Session().CreateEntryL(entry, id, aOwnerId);
	iMsvSession.CheckDrive();

    TPckgBuf<TMsvLocalOperationProgress> progressPack;
	User::LeaveIfError(iMsvSession.Session().OperationCompletion(id, progressPack));
	User::LeaveIfError(progressPack().iError);

	// Can't leave after here

	if (addEntry)
		{
		CleanupStack::Pop(); // newSortedChildren
		delete iSortedChildren;
		iSortedChildren = newSortedChildren;

		CleanupStack::Pop(); // cEntry
		iEntries->AppendL(cEntry); // Will not leave because we've reserved space earlier
		cEntry->SetId(progressPack().iId);

		TMsvEntry* ptr = CONST_CAST(TMsvEntry*, iEntryPtr);
		ptr->SetOwner(ETrue);
		}

	aEntry.SetParent(iEntryPtr->Id());
	aEntry.SetId(progressPack().iId);

	// If the entry is a service its service Id is the same as its Id
	// Otherwise it must be a local entry
	// We don't allow synchronous creation of remote entries
	TMsvId serviceId = aEntry.Id();

	aEntry.iServiceId = (aEntry.iType == KUidMsvServiceEntry) ? serviceId : KMsvLocalServiceIndexEntryId;
	}

/** 
Sets the context's index entry to the specified values. The returned CMsvOperation 
object completes when the change is complete.

It is important to note that the state of the context is undefined until the 
observer of the entry has been informed that the entry has been changed, or 
the operation is completed with an error. If the function leaves, the context 
is unchanged.

@param	aEntry 
The new index entry values for the context

@param	aStatus 
The request status to be completed when the operation has finished 

@leave KErrAccessDenied The entry is locked by another client 

@leave KErrArgument aEntry is invalid or the ID specified in aEntry is not 
the same as the context ID 

@leave KErrNoMemory The operation could not be created or passed to the server 

@return
An operation object controlling the change command
*/
EXPORT_C CMsvOperation* CMsvEntry::ChangeL(const TMsvEntry& aEntry, TRequestStatus& aStatus)
	{
	return ChangeL(aEntry, RProcess().SecureId(), aStatus);
	}

/** 
Sets the context's index entry to the specified values. The returned CMsvOperation 
object completes when the change is complete. Sets the owner of the changed entry 
to process specified by the supplied ID. 

It is important to note that the state of the context is undefined until the 
observer of the entry has been informed that the entry has been changed, or 
the operation is completed with an error. If the function leaves, the context 
is unchanged.

@param	aEntry 
The new index entry values for the context

@param	aOwnerId
The ID of process that owns the changed entry.

@param	aStatus 
The request status to be completed when the operation has finished 

@leave KErrAccessDenied The entry is locked by another client 

@leave KErrArgument aEntry is invalid or the ID specified in aEntry is not 
the same as the context ID 

@leave KErrNoMemory The operation could not be created or passed to the server 

@return
An operation object controlling the change command
*/
EXPORT_C CMsvOperation* CMsvEntry::ChangeL(const TMsvEntry& aEntry, TSecureId aOwnerId, TRequestStatus& aStatus)
	{
#ifndef _NO_SESSION_LOGGING_
	Log(_L("Asynchronous ChangeL to %x"), aEntry.Id()); 
#endif

	__ASSERT_DEBUG(iState==EValid || iState==EInvalidDeletedContext, PanicServer(EMsvEntryAlreadyChangingContext));
	__ASSERT_DEBUG(aEntry.Id()==iEntryPtr->Id(), PanicServer(EMsvChangingEntryNotContext));
	__ASSERT_DEBUG(MsvUtils::ValidEntry(aEntry), PanicServer(EMsvChangingToInvalidEntry));

	// can only change the current context
	if (aEntry.Id()!=iEntryPtr->Id() || aEntry.Parent()!=iEntryPtr->Parent() || !MsvUtils::ValidEntry(aEntry))
		User::Leave(KErrArgument);

	// cannot change standard folders
	if (iEntryPtr->StandardFolder())
		User::Leave(KErrAccessDenied);

	// create the operation
	CMsvEntryOperation* operation = CMsvEntryOperation::NewL(iMsvSession, aStatus);
	CleanupStack::PushL(operation);
	if (iEntryPtr->iType==KUidMsvServiceEntry)
		{
		operation->iMtm = KUidMsvLocalServiceMtm;
		operation->iService = KMsvLocalServiceIndexEntryId;
		}
	else
		User::LeaveIfError(iMsvSession.Session().OperationMtmL(iEntryPtr->Id(), operation->iMtm, operation->iService));
	
	// check that no other entries are type EMsvClientChangedContext
	TInt count=iEntries->Count();
	while (count--)
		{
		if (iEntries->At(count)->Type() == EMsvClientChangedContext)
			{
			delete iEntries->At(count);
			iEntries->Delete(count);
			}
		}

	// create local copy of entry
	TMsvEntry entry=aEntry;

	// check the hidden flags are correct
	entry.SetOwner(iEntryPtr->Owner());
	entry.SetDeleted(iEntryPtr->Deleted());

	// store the new context for after the operation has completed
	CMsvClientEntry* cEntry = CMsvClientEntry::NewLC(entry, EMsvClientChangedContext);
	if (iEntries->Count()==1)
		iEntries->AppendL(cEntry);
	else
		iEntries->InsertL(1, cEntry);

	// start the change operation
	TRAPD(leave, iMsvSession.Session().ChangeEntryL(entry, operation->Id(), aOwnerId, operation->iStatus)); 
	if (leave)
		{
		iEntries->Delete(1);
		CleanupStack::PopAndDestroy(); // operation	& cEntry
		User::Leave(leave);
		}

	operation->Start();
	iState = EInvalidChangingContext;
	CleanupStack::Pop(2); // operation and cEntry
	return operation;
	}

/** 
Sets the context's index entry to the specified values. The function is performed 
synchronously.

This function can only be used on local entries.

@param	aEntry 
The new index entry values for the context 

@leave KErrAccessDenied The entry is locked by another client 

@leave KErrArgument aEntry is invalid or the ID specified in aEntry is not 
the same as the context ID 

@leave KErrNoMemory The operation could not be created or passed to the server
*/
EXPORT_C void CMsvEntry::ChangeL(const TMsvEntry& aEntry)
	{
	ChangeL(aEntry, RProcess().SecureId());
	}

/** 
Sets the context's index entry to the specified values. The function is performed 
synchronously. Sets the owner of the changed entry to process specified by the 
supplied ID. 

This function can only be used on local entries.

@param	aEntry 
The new index entry values for the context 

@param	aOwnerId
The ID of process that owns the changed entry.

@leave KErrAccessDenied The entry is locked by another client 

@leave KErrArgument aEntry is invalid or the ID specified in aEntry is not 
the same as the context ID 

@leave KErrNoMemory The operation could not be created or passed to the server
*/
EXPORT_C void CMsvEntry::ChangeL(const TMsvEntry& aEntry, TSecureId aOwnerId)
	{
#ifndef _NO_SESSION_LOGGING_
	Log(_L("Synchronous ChangeL to %x"), aEntry.Id()); 
#endif

	__ASSERT_DEBUG(iState==EValid || iState==EInvalidDeletedContext, PanicServer(EMsvEntryAlreadyChangingContext));
	__ASSERT_DEBUG(aEntry.Id()==iEntryPtr->Id(), PanicServer(EMsvChangingEntryNotContext));
	__ASSERT_DEBUG(MsvUtils::ValidEntry(aEntry), PanicServer(EMsvChangingToInvalidEntry));

	// can only change the current context
	if (aEntry.Id()!=iEntryPtr->Id() || aEntry.Parent()!=iEntryPtr->Parent() || !MsvUtils::ValidEntry(aEntry))
		User::Leave(KErrArgument);

	// cannot change standard folders
	if (iEntryPtr->StandardFolder())
		User::Leave(KErrAccessDenied);

	// create local copy of entry
	TMsvEntry entry=aEntry;

	// check the hidden flags are correct
	entry.SetOwner(iEntryPtr->Owner());
	entry.SetDeleted(iEntryPtr->Deleted());

	// store the new context for after the operation has completed
	CMsvClientEntry* cEntry = CMsvClientEntry::NewLC(entry, EMsvClientContext);

	TInt id = iMsvSession.OperationId();
	iMsvSession.Session().ChangeEntryL(aEntry, id, aOwnerId); 

    TPckgBuf<TMsvLocalOperationProgress> progressPack;
	User::LeaveIfError(iMsvSession.Session().OperationCompletion(id, progressPack));
	User::LeaveIfError(progressPack().iError);

	// Cannot leave after this
	delete iEntries->At(0);
	CleanupStack::Pop(); // cEntry
	iEntries->At(0) = cEntry;
	iEntryPtr = &iEntries->At(0)->Entry();
	}

/** 
Update a selection of children to read or Unread asynchronously of the context. 
The returned CMsvOperation object .

@param	aSelection 
The selectio of entry values for the context

@param	aMark 
True : Update selection to UnRead .
False: Update selection to Read . 

@param aStatus The request status to be completed when the operation has finished .

@leave KErrAccessDenied The entry is locked by another client 

@return
An operation object controlling the change command
*/
EXPORT_C CMsvOperation* CMsvEntry::ChangeL(const CMsvEntrySelection& aSelection,TBool aMark, TRequestStatus& aStatus)
     {
#ifndef _NO_SESSION_LOGGING_
    Log(_L("Asynchronous ChangeL with selection of %d entries"), aSelection.Count()); 
#endif

    __ASSERT_DEBUG(aSelection.Count(), PanicServer(EMsvEmptySelection));

#if defined(_DEBUG)    
     
      TInt aCount = aSelection.Count();
      while (aCount--)
          {
          TMsvEntry aEntry;
          TMsvId aService;
          if (iMsvSession.Session().GetEntry(aSelection.At(aCount), aService, aEntry)==KErrNone)
              {
              __ASSERT_DEBUG(iState==EValid || iState==EInvalidDeletedContext, PanicServer(EMsvEntryAlreadyChangingContext));
              __ASSERT_DEBUG(MsvUtils::ValidEntry(aEntry), PanicServer(EMsvChangingToInvalidEntry));
             
              // can only change the current context
              if (!MsvUtils::ValidEntry(aEntry))
                  User::Leave(KErrArgument);
              }
           }
#endif
      // cannot change standard folders
      if (iEntryPtr->StandardFolder())
          User::Leave(KErrAccessDenied);
  
      //// create the operation
      CMsvEntryOperation* operation = CMsvEntryOperation::NewL(iMsvSession, aStatus);
      CleanupStack::PushL(operation);
      User::LeaveIfError(iMsvSession.Session().OperationMtmL(aSelection.At(0), operation->iMtm, operation->iService)); 
   
      // start the change operation
	  iMsvSession.Session().ChangeEntriesL(aSelection, aMark, operation->Id(), RProcess().SecureId(), operation->iStatus);
     
      operation->Start();
      iState = EInvalidChangingContext;
      CleanupStack::Pop(1); // operation 
      return operation; 
      }


EXPORT_C CMsvOperation* CMsvEntry::DeleteL(TMsvId aId, TRequestStatus& aStatus)
//
// Deletes a child of the context
//
	/** Deletes a child entry of the context asynchronously.
	
	The delete works recursively through all the descendants. If a child or any 
	descendant is locked by another client or any store or file is open, then 
	that child will not be deleted. Any files and stores associated with the entry 
	are deleted.
	
	The returned CMsvOperation object completes when deletion is complete.
	
	
	@param aId ID of entry to be deleted 
	@param aStatus The request status to be completed when the operation has finished 
	
	@leave KErrNotFound The specified entry was not a child of the context 
	@leave KErrNotSupported If deleting entries from non-current drive
	@return The operation object controlling the deletion command */
	{
#ifndef _NO_SESSION_LOGGING_
	Log(_L("Asynchronous DeleteL, entry %x"), aId); 
#endif

	__ASSERT_DEBUG(iState==EValid || iState==EInvalidDeletedContext || iState==EInvalidChangingContext, PanicServer(EMsvEntryStateUndetermined));
	CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection;
	CleanupStack::PushL(selection);
	selection->AppendL(aId);
	CMsvOperation* operation = DoDeleteL(*selection, aStatus);
	CleanupStack::PopAndDestroy(); // selection
	return operation;
	}



EXPORT_C CMsvOperation* CMsvEntry::DeleteL(const CMsvEntrySelection& aSelection, TRequestStatus& aStatus)
//
// Deletes a selection containing children of the context
//
/** Deletes child entries of the context asynchronously. 

The delete works recursively through all the descendants. If a child or any 
descendant is locked by another client or any store or file is open, then 
that child will not be deleted. Any files and stores associated with the entries 
are deleted.

The returned CMsvOperation object completes when deletion is complete.

@param aSelection List of ID of the entries to be deleted 
@param aStatus The request status to be completed when the operation has finished 

@leave KErrNotFound A specified entry was not a child of the context 
@leave KErrNotSupported If deleting entries from non-current drive
@return The operation object controlling the deletion command */
	{
#ifndef _NO_SESSION_LOGGING_
	Log(_L("Asynchronous DeleteL with selection of %d entries"), aSelection.Count()); 
#endif

	__ASSERT_DEBUG(iState==EValid || iState==EInvalidDeletedContext || iState==EInvalidChangingContext, PanicServer(EMsvEntryStateUndetermined));
	return DoDeleteL(aSelection, aStatus);
	}



CMsvOperation* CMsvEntry::DoDeleteL(const CMsvEntrySelection& aSelection, TRequestStatus& aStatus)
//
//
//
	{
	__ASSERT_DEBUG(aSelection.Count(), PanicServer(EMsvEmptySelection));
	
	if (!AreChildren(aSelection))
		User::Leave(KErrNotFound);

	CMsvEntryOperation* operation = CMsvEntryOperation::NewL(iMsvSession, aStatus);
	CleanupStack::PushL(operation);
	if (iEntryPtr->iType==KUidMsvRootEntry)
		{
		// we must be deleting services - so it is a local operation
		operation->iMtm = KUidMsvLocalServiceMtm;
		operation->iService = KMsvLocalServiceIndexEntryId;
		}
	else
		User::LeaveIfError(iMsvSession.Session().OperationMtmL(aSelection.At(0), operation->iMtm, operation->iService));

#if defined(_DEBUG)
	// check other entries in selection are consistent and are not read only
	TInt dCount = aSelection.Count();
	while (dCount--)
		{
		if (iEntryPtr->iType!=KUidMsvRootEntry)
			{
				TMsvId service;
				TUid mtm;
				TInt error = iMsvSession.Session().OperationMtmL(aSelection.At(dCount), mtm, service);
				__ASSERT_DEBUG(error==KErrNone || error==KErrNotFound, PanicServer(EMsvNonConsistentDeleteSelection));
				__ASSERT_DEBUG(mtm==operation->iMtm, PanicServer(EMsvNonConsistentDeleteSelection));
				__ASSERT_DEBUG(service==operation->iService, PanicServer(EMsvNonConsistentDeleteSelection));
			}
		TMsvEntry dEntry;
		TMsvId dService;
		if (iMsvSession.Session().GetEntry(aSelection.At(dCount), dService, dEntry)==KErrNone)
			{
			__ASSERT_DEBUG(dService==iOwningService || (iEntryPtr->iType==KUidMsvServiceEntry && dService==iEntryPtr->Id()) || (iEntryPtr->iType==KUidMsvRootEntry && dService==aSelection.At(dCount)), PanicServer(EMsvDeletingEntryDifferentOwningService));
			}
		}
#endif
	
	iMsvSession.Session().DeleteEntriesL(aSelection, operation->Id(), operation->iStatus);
	operation->Start();
	CleanupStack::Pop(); // operation
	return operation;
	}

EXPORT_C void CMsvEntry::DeleteL(TMsvId aId)
/** Deletes a child entry of the context synchronously. 

The delete works recursively through all the descendants. If a child or any descendant is locked by another 
client or any store or file is open, then that child will not be deleted. Any files and stores associated 
with the entry are deleted.

This function can only be used on local entries.

@param aId ID of entry to be deleted
@leave KErrNotFound The specified entry was not a child of the context
@leave KErrNotSupported If deleting entries from non-current drive
*/
	{
#ifndef _NO_SESSION_LOGGING_
	Log(_L("Synchronous DeleteL, entry %x"), aId); 
#endif
	__ASSERT_ALWAYS(iOwningService == KMsvLocalServiceIndexEntryId || iOwningService == KMsvRootIndexEntryId, PanicServer(EMsvNotLocalService)); // Root as well?
	__ASSERT_DEBUG(iState==EValid || iState==EInvalidDeletedContext || iState==EInvalidChangingContext, PanicServer(EMsvEntryStateUndetermined));
	
	User::LeaveIfError(CMsvEntry::DeleteOneL(aId));
	}

EXPORT_C void CMsvEntry::DeleteL(const CMsvEntrySelection& aSelection, TMsvLocalOperationProgress& aProgress)
/** Deletes child entries of the context synchronously. 

The delete works recursively through all the descendants. If a child or any 
descendant is locked by another client or any store or file is open, then 
that child will not be deleted. Any files and stores associated with the entries 
are deleted.

@param aSelection List of ID of the entries to be deleted 
@param aProgress Progress information for the delete operation
@leave KErrNotFound A specified entry was not a child of the context 
@leave KErrNotSupported If deleting entries from non-current drive */
	{
#ifndef _NO_SESSION_LOGGING_
	Log(_L("Synchronous DeleteL with selection of %d entries"), aSelection.Count()); 
#endif

	__ASSERT_ALWAYS(iOwningService == KMsvLocalServiceIndexEntryId || iOwningService == KMsvRootIndexEntryId, PanicServer(EMsvNotLocalService)); // Root as well?
	__ASSERT_DEBUG(iState==EValid || iState==EInvalidDeletedContext || iState==EInvalidChangingContext, PanicServer(EMsvEntryStateUndetermined));
	__ASSERT_DEBUG(aSelection.Count(), PanicServer(EMsvEmptySelection));

	if (!AreChildren(aSelection))
		User::Leave(KErrNotFound);

	aProgress.iTotalNumberOfEntries=aSelection.Count();
	aProgress.iNumberCompleted=0;
	aProgress.iNumberRemaining=aProgress.iTotalNumberOfEntries;
	aProgress.iError=KErrNone;

	TInt count=aProgress.iTotalNumberOfEntries;
	while(count--)
		{
		aProgress.iId=aSelection.At(count);
		TInt err=DeleteOneL(aProgress.iId);
		aProgress.iNumberRemaining--;
		if(err==KErrNone)
			aProgress.iNumberCompleted++;
		else
			{
			aProgress.iError=err;
			aProgress.iNumberFailed++;
			}
		}
	}


TInt CMsvEntry::DeleteOneL(TMsvId aMsvId)
	{
	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);
	selection->AppendL(aMsvId);
	TInt opid = iMsvSession.OperationId();
	iMsvSession.Session().DeleteEntriesL(*selection, opid);

	TMsvLocalOperationProgress progress;
	TPckg<TMsvLocalOperationProgress> progressPack(progress);
    User::LeaveIfError(iMsvSession.Session().OperationCompletion(opid, progressPack));
	
	if(progress.iError==KErrNone)
		{
		TMsvId id = selection->At(0);
		TInt ii = iSortedChildren->Count();
		while (ii--)
			{
			if (iSortedChildren->At(ii)->Id() == id)
				{
				iSortedChildren->Delete(ii);
				break;
				}
			}

		ii = iEntries->Count();
		while (ii--)
			{
			if (iEntries->At(ii)->Entry().Id() == id)
				{
				delete iEntries->At(ii);
				iEntries->Delete(ii);
				break;
				}
			}
		// Reset the owner flag
		if (Count() == 0)
			{
			TMsvEntry* ptr = CONST_CAST(TMsvEntry*, iEntryPtr);
			ptr->SetOwner(EFalse);
			}		
		}
	CleanupStack::PopAndDestroy(selection);
	return(progress.iError);
	}




EXPORT_C CMsvEntrySelection* CMsvEntry::ChildrenL() const
//
// Gets a selection containing the children of the context
//
/** Gets a selection containing the IDs of all the context children. If the entry 
has no children, the selection is empty.

The calling function is responsible for the deletion of the returned CMsvEntrySelection. 

@leave KErrNoMemory Not enough memory to create the selection 
@return A selection containing the ID of all children of the context */
	{
	__ASSERT_DEBUG(iState==EValid || iState==EInvalidDeletedContext || iState==EInvalidChangingContext, PanicServer(EMsvEntryStateUndetermined));
	CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection;
	CleanupStack::PushL(selection);
	TInt totalCount=iSortedChildren->Count();
	for (TInt count=0; count<totalCount; count++)
		{
		selection->AppendL(iSortedChildren->At(count)->Id());
		}
	CleanupStack::Pop(); // selection
	return selection;
	}

EXPORT_C CMsvEntrySelection* CMsvEntry::ChildrenWithServiceL(TMsvId aServiceId) const
//
// Gets a selection containing the children of the context which use the service
//
/** Gets a selection containing the IDs of all the context children filtered by message service.
i.e. the index entry's iServiceId field equals aId.

If the entry has no such children, the selection is empty.

The calling function is responsible for the deletion of the returned CMsvEntrySelection.
@return	List of IDs of all children of the context meeting the criterion		
@param aServiceId Service by which to filter
@leave KErrNoMemory Not enough memory to create the selection 
*/
	{
	__ASSERT_DEBUG(iState==EValid || iState==EInvalidDeletedContext || iState==EInvalidChangingContext, PanicServer(EMsvEntryStateUndetermined));
	CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection;
	CleanupStack::PushL(selection);
	TInt totalCount=iSortedChildren->Count();
	for (TInt count=0; count<totalCount; count++)
		{
		if (iSortedChildren->At(count)->iServiceId==aServiceId)
			{
			selection->AppendL(iSortedChildren->At(count)->Id());
			}
		}
	CleanupStack::Pop(); // selection
	return selection;
	}

EXPORT_C CMsvEntrySelection* CMsvEntry::ChildrenWithMtmL(TUid aMtm) const
//
// Gets a selection containing the children of the context which use the same MTM
//
/** Gets a selection containing the IDs of all the context children filtered by 
MTM type. i.e. the index entry's iMtm field equals aMtm.

If the entry has no such children, the selection is empty.

The calling function is responsible for the deletion of the returned CMsvEntrySelection. 

@param aMtm MTM type by which to filter 
@leave KErrNoMemory Not enough memory to create the selection 
@return A selection containing the ID of all children of the context meeting 
the criterion */
	{
	__ASSERT_DEBUG(iState==EValid || iState==EInvalidDeletedContext || iState==EInvalidChangingContext, PanicServer(EMsvEntryStateUndetermined));
	CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection;
	CleanupStack::PushL(selection);
	TInt totalCount=iSortedChildren->Count();
	for (TInt count=0; count<totalCount; count++)
		{
		if (iSortedChildren->At(count)->iMtm==aMtm)
			{
			selection->AppendL(iSortedChildren->At(count)->Id());
			}
		}
	CleanupStack::Pop(); // selection
	return selection;
	}

EXPORT_C CMsvEntrySelection* CMsvEntry::ChildrenWithTypeL(TUid aType) const
//
// Gets a selection containing the children of the context which are the same type
//
/** Gets a selection containing the IDs of all the context children filtered by 
entry type. i.e. is the entry a folder, a message, etc.

If the entry has no such children, the selection is empty.

The calling function is responsible for the deletion of the returned CMsvEntrySelection. 


@param aType Entry type by which to filter. 
@leave KErrNoMemory Not enough memory to create the selection 
@return A selection containing the ID of all children of the context meeting 
the criterion */
	{
	__ASSERT_DEBUG(iState==EValid || iState==EInvalidDeletedContext || iState==EInvalidChangingContext, PanicServer(EMsvEntryStateUndetermined));

	CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection;
	CleanupStack::PushL(selection);
	TInt totalCount=iSortedChildren->Count();
	for (TInt count=0; count<totalCount; count++)
		{
		if (iSortedChildren->At(count)->iType==aType)
			{
			selection->AppendL(iSortedChildren->At(count)->Id());
			}
		}
	CleanupStack::Pop(); // selection
	return selection;
	}


#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
/**
 * ChildrenOfAvailableDrivesL()
 *
 * @param None.
 * @return CMsvEntrySelection List of child ids from all available drives.
 * @leave KErrArgument If the function is used for a TMsvId other than that 
 * of standard folders, i.e. Inbox, Outbox, Drafts, Sent or Deleted.
 * @leave KErrNoMemory Not enough memory to create the selection.
 * 
 * Gets a selection containing the child Id's from all drives currently present
 * in the server preferred drive list.
 * The function must be used only if the context is set to one of the standard folders,
 * i.e. Inbox, Outbox, Drafts, Sent or Deleted.
 * 
 * The calling function is responsible for the deletion of the returned CMsvEntrySelection.
 *
 @publishedAll
 @released
 */ 
EXPORT_C CMsvEntrySelection* CMsvEntry::ChildrenOfAvailableDrivesL() const
    {
    __ASSERT_DEBUG(iState==EValid || iState==EInvalidDeletedContext || iState==EInvalidChangingContext, PanicServer(EMsvEntryStateUndetermined));

    if(!iChildrenOfAvailableDrives)
        {
		User::Leave(KErrArgument);		
		}

    CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection;
	CleanupStack::PushL(selection);

    TInt totalCount = iSortedChildren->Count();
	for (TInt count=0; count<totalCount; count++)
		{
		selection->AppendL(iSortedChildren->At(count)->Id());
		}
	CleanupStack::Pop(); // selection
	return selection;
	}
#endif			// #if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)

	

EXPORT_C const TMsvEntry& CMsvEntry::operator[](TInt aIndex) const
//
// Returns the data for a child, zero index with the current sort order
//
/** Gets the index entry of the child at the position specified by the array index. 
The child entries of the context can be considered as a zero-based array, 
with entries sorted according to the current sort order. 

Note:

The function panics with E32USER-CBase 21 if aIndex was out of range.

@param aIndex Array index 
@return Index entry for the specified child. Valid for in-range values of aIndex. */
	{
	__ASSERT_DEBUG(iState==EValid || iState==EInvalidDeletedContext || iState==EInvalidChangingContext, PanicServer(EMsvEntryStateUndetermined));
	return *iSortedChildren->At(aIndex);
	}


EXPORT_C CMsvEntry* CMsvEntry::ChildEntryL(TMsvId aId) const
//
// Returns a new entry with the child as the context
//
/** Gets a new CMsvEntry object with its context set to the child entry ID. aMsvId 
must specify a child of the current context.

The CMsvEntry object must be deleted by the client application when it is 
no longer required. 

@param aId ID of a child entry 
@leave KErrNotFound aMsvId does not specify a child of the context 
@return CMsvEntry object with its context set to child entry */
	{
	__ASSERT_DEBUG(iState==EValid || iState==EInvalidDeletedContext || iState==EInvalidChangingContext, PanicServer(EMsvEntryStateUndetermined));
	if (!IsAChild(aId))
		User::Leave(KErrNotFound);
	return CMsvEntry::NewL(iMsvSession, aId, iOrdering);
	}


EXPORT_C const TMsvEntry& CMsvEntry::ChildDataL(TMsvId aId) const
//
// Returns the data for a child with the aId
//
/** Gets the index entry of context's child with the specified ID.

@param aId ID of the child 
@leave KErrNotFound No child exists with that ID 
@return Index entry for the specified child. Valid for in-range values of aIndex. */
	{
	__ASSERT_DEBUG(iState==EValid || iState==EInvalidDeletedContext || iState==EInvalidChangingContext, PanicServer(EMsvEntryStateUndetermined));

	TInt count=iSortedChildren->Count();
	while (count--)
		{
		if (iSortedChildren->At(count)->Id()==aId)
			break;
		}
	User::LeaveIfError(count); // will be -1 (KErrNotFound)
	return *iSortedChildren->At(count);
	}


EXPORT_C CMsvStore* CMsvEntry::ReadStoreL()
//
// Return store for the current context which is opened for read only
//
/** Obtains the message store for the current context with read-only access. 

Multiple clients can read from a store simultaneously. If another client is already 
writing to the store, the function leaves with KErrAccessDenied. 

The returned CMsvStore must be deleted when it is no longer required. 

@leave KErrNoMemory Not enough memory to open store 
@leave KErrAccessDenied Another client is currently writing to the store 
@leave KErrNotFound There is no store associated with this entry 
@return Context's message store open for read-only access */
	{
#ifndef _NO_SESSION_LOGGING_
	Log(_L("ReadStoreL for entry %x"), iEntryPtr->Id()); 
#endif

	__ASSERT_ALWAYS(iStore==NULL, PanicServer(EMsvStoreAlreadyOpen));
	__ASSERT_DEBUG(iState==EValid || iState==EInvalidDeletedContext || iState==EInvalidChangingContext, PanicServer(EMsvEntryStateUndetermined));
	User::LeaveIfError(iMsvSession.Session().ReadStore(iEntryPtr->Id()));

	// open the store
	TInt err =0;
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
		TRAP(err, iStore = CMsvStore::OpenForReadL(*this, iMsvSession.FileSession(), iMsvSession.StoreManager(), iEntryPtr->Id(), this->Entry().iMtm));	
#else
		TRAP(err, iStore = CMsvStore::OpenForReadL(*this, iMsvSession.FileSession(), iMsvSession.StoreManager(), iEntryPtr->Id()));			
#endif
	if (err != KErrNone)
		{
		iMsvSession.Session().DecStoreReaderCount(iEntryPtr->Id()); // error ignored
		User::Leave(err);
		}
	return iStore;
	}


EXPORT_C CMsvStore* CMsvEntry::EditStoreL()
//
// Return store for the current context which can be writen to
//
/** Gets the message store for the current context with read-write access. 

Only one client can edit a message store at one time. If another client is 
already writing to the store, KErrAccessDenied is returned. Other clients 
can be reading the store. 

If the message store does not exist when EditStore() is called, a new message 
store is created. 

The returned CMsvStore must be deleted when it is no longer required. 

@leave KErrAccessDenied Store is locked by another process or the entry is 
read only 
@leave KErrNoMemory Not enough memory to open the store 
@return Context's message store open for read-write access */
	{
#ifndef _NO_SESSION_LOGGING_
	Log(_L("EditStoreL for entry %x"), iEntryPtr->Id()); 
#endif

	__ASSERT_ALWAYS(iStore==NULL, PanicServer(EMsvStoreAlreadyOpen));
	__ASSERT_DEBUG(iState==EValid || iState==EInvalidDeletedContext || iState==EInvalidChangingContext, PanicServer(EMsvEntryStateUndetermined));
	
	if (iEntryPtr->ReadOnly())
		User::Leave(KErrAccessDenied);

	User::LeaveIfError(iMsvSession.Session().LockStore(iEntryPtr->Id()));

	// open the store
	TInt error = 0;
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)	
		TRAP(error,  iStore = CMsvStore::OpenForWriteL(*this, iMsvSession.FileSession(), iMsvSession.StoreManager(), iEntryPtr->Id(), this->Entry().iMtm));
#else
		TRAP(error,  iStore = CMsvStore::OpenForWriteL(*this, iMsvSession.FileSession(), iMsvSession.StoreManager(), iEntryPtr->Id()));
#endif
	if (error)
		{
		iMsvSession.Session().ReleaseStore(iEntryPtr->Id()); // error ignored
		User::Leave(error);		
		}

	return iStore;
	}



void CMsvEntry::HandleStoreEvent(MMsvStoreObserver::TMsvStoreEvent aEvent, TMsvId /*aId*/)
//
//
//
	{
	switch (aEvent)
		{
		case EMsvEditStoreClosed:
			iMsvSession.Session().ReleaseStore(iEntryPtr->Id()); // error ignored
			iStore=NULL;
			break;
		case EMsvReadStoreClosed:
			iMsvSession.Session().DecStoreReaderCount(iEntryPtr->Id()); // error ignored
			iStore=NULL;
			break;
		default:
			__ASSERT_DEBUG(EFalse, PanicServer(EMsvUnknownStoreEvent3));
		}

	}


EXPORT_C CMsvOperation* CMsvEntry::MoveL(TMsvId aMsvId, TMsvId aTargetId, TRequestStatus& aStatus)
//
// Move a single child to another parent
//
/** Moves, asynchronously, a child of the context to become an entry owned by the target entry. 

All descendants will be moved as well. Any files and stores associated with 
the entry are also moved.

The returned CMsvOperation object completes when moving is complete.

@param aMsvId The ID of the entry to be moved 
@param aTargetId The ID of the entry to own the moved entries 
@param aStatus The request status to be completed when the operation has finished 

@leave KErrNoMemory The operation could not be created or passed to the server 
@leave KErrNotFound An entry was not a child of the context 
@return The operation object controlling the move command. */
	{
#ifndef _NO_SESSION_LOGGING_
	Log(_L("Asynchronous MoveL %x to %x"), aMsvId, aTargetId); 
#endif

	CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection;
	CleanupStack::PushL(selection);
	selection->AppendL(aMsvId);
	CMsvOperation* operation = MoveL(*selection, aTargetId, aStatus);
	CleanupStack::PopAndDestroy(); // selection
	return operation;
	}


TInt CMsvEntry::MoveOneL(TMsvId aMsvId, TMsvId aTargetId)
	{
	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);
	selection->AppendL(aMsvId);
	TInt opid = iMsvSession.OperationId();
	iMsvSession.Session().MoveEntriesL(*selection, aTargetId, opid);

	TMsvLocalOperationProgress progress;
	TPckg<TMsvLocalOperationProgress> progressPack(progress);
    User::LeaveIfError(iMsvSession.Session().OperationCompletion(opid, progressPack));
	
	if(progress.iError==KErrNone)
		{
		TMsvId id = selection->At(0);
		TInt ii = iSortedChildren->Count();
		while (ii--)
			{
			if (iSortedChildren->At(ii)->Id() == id)
				{
				iSortedChildren->Delete(ii);
				break;
				}
			}

		ii = iEntries->Count();
		while (ii--)
			{
			if (iEntries->At(ii)->Entry().Id() == id)
				{
				delete iEntries->At(ii);
				iEntries->Delete(ii);
				break;
				}
			}
		// Reset the owner flag
		if (Count() == 0)
			{
			TMsvEntry* ptr = CONST_CAST(TMsvEntry*, iEntryPtr);
			ptr->SetOwner(EFalse);
			}		
		}
	CleanupStack::PopAndDestroy(selection);
	return(progress.iError);
	}

/** Moves, synchronously, a child of the context to become an entry owned by the target entry. 

All descendants will be moved as well. Any files and stores associated with 
the entry are also moved.

@param aMsvId The ID of the entry to be moved 
@param aTargetId The ID of the entry to own the moved entries 

@leave KErrNoMemory 
@leave KErrNotFound An entry was not a child of the context 
*/
EXPORT_C void CMsvEntry::MoveL(TMsvId aMsvId, TMsvId aTargetId)
//
// Move a single child to another parent
//
	{
	__ASSERT_ALWAYS(iOwningService == KMsvLocalServiceIndexEntryId || iOwningService == KMsvRootIndexEntryId, PanicServer(EMsvNotLocalService)); // Root as well?
	__ASSERT_DEBUG(iState==EValid || iState==EInvalidDeletedContext || iState==EInvalidChangingContext, PanicServer(EMsvEntryStateUndetermined));
	__ASSERT_DEBUG(aTargetId!=iEntryPtr->Id(), PanicServer(EMsvMovingEntryToSameParent));

#ifndef _NO_SESSION_LOGGING_
	Log(_L("Synchronous MoveL %x to %x"), aMsvId, aTargetId); 
#endif
	User::LeaveIfError(MoveOneL(aMsvId,aTargetId));
	}


/** Moves, synchronously, children of the context to become entries owned by the target entry. 

All descendants will be moved as well. Any files and stores associated with 
the entries are also moved.

@param aSelection List of IDs of the entries to be moved 
@param aTargetId The ID of the entry to own the moved entires 
@param aProgress On return, records the outcome of the move 

@leave KErrNoMemory 
@leave KErrNotFound An entry was not a child of the context 
*/
EXPORT_C void CMsvEntry::MoveL(const CMsvEntrySelection& aSelection, TMsvId aTargetId, TMsvLocalOperationProgress& aProgress)
//
// Move a selection of children to another parent
//
	{
	__ASSERT_ALWAYS(iOwningService == KMsvLocalServiceIndexEntryId || iOwningService == KMsvRootIndexEntryId, PanicServer(EMsvNotLocalService)); // Root as well?
	__ASSERT_DEBUG(iState==EValid || iState==EInvalidDeletedContext || iState==EInvalidChangingContext, PanicServer(EMsvEntryStateUndetermined));
	__ASSERT_DEBUG(aTargetId!=iEntryPtr->Id(), PanicServer(EMsvMovingEntryToSameParent));
	__ASSERT_DEBUG(aSelection.Count(), PanicServer(EMsvEmptySelection));


#ifndef _NO_SESSION_LOGGING_
	Log(_L("Synchronous MoveL with selection of %d entries to %x"), aSelection.Count(),aTargetId); 
#endif

	aProgress.iTotalNumberOfEntries=aSelection.Count();
	aProgress.iNumberCompleted=0;
	aProgress.iNumberRemaining=aProgress.iTotalNumberOfEntries;
	aProgress.iError=KErrNone;

	TInt count=aProgress.iTotalNumberOfEntries;
	while(count--)
		{
		aProgress.iId=aSelection.At(count);
		TInt err=MoveOneL(aProgress.iId,aTargetId);
		aProgress.iNumberRemaining--;
		if(err==KErrNone)
			aProgress.iNumberCompleted++;
		else
			{
			aProgress.iError=err;
			aProgress.iNumberFailed++;
			}
		}
	}






EXPORT_C CMsvOperation* CMsvEntry::MoveL(const CMsvEntrySelection& aSelection, TMsvId aTargetId, TRequestStatus& aStatus)
//
// Move a selection of children to another parent
//
/** Moves, asynchronously, children of the context to become entries owned by the target entry. 

All descendants will be moved as well. Any files and stores associated with 
the entries are also moved.

The returned CMsvOperation object completes when moving is complete.

@param aSelection List of IDs of the entries to be moved 
@param aTargetId The ID of the entry to own the moved entires 
@param aStatus The request status to be completed when the operation has finished 

@leave KErrNoMemory The operation could not be created or passed to the server 
@leave KErrNotFound An entry was not a child of the context 
@return The operation object controlling the move command. */
	{
#ifndef _NO_SESSION_LOGGING_
	Log(_L("Asynchronous MoveL of selection of %d entries to %x"), aSelection.Count(), aTargetId); 
#endif

	__ASSERT_DEBUG(aTargetId!=iEntryPtr->Id(), PanicServer(EMsvMovingEntryToSameParent));
	__ASSERT_DEBUG(iState==EValid || iState==EInvalidDeletedContext || iState==EInvalidChangingContext, PanicServer(EMsvEntryStateUndetermined));
	if (!AreChildren(aSelection))
		User::Leave(KErrNotFound);
	CMsvEntryOperation* operation = CMsvEntryOperation::NewL(iMsvSession, aStatus);
	CleanupStack::PushL(operation);
	User::LeaveIfError(iMsvSession.Session().OperationMtmL(aSelection.At(0), aTargetId, operation->iMtm, operation->iService));

#if defined(_DEBUG)
	// check other entries in selection are consistent and are not read only
	TInt dCount = aSelection.Count();
	while (dCount--)
		{
		TMsvId service;
		TUid mtm;
		TInt error = iMsvSession.Session().OperationMtmL(aSelection.At(dCount), aTargetId, mtm, service);
		__ASSERT_DEBUG(error==KErrNone || error==KErrNotFound, PanicServer(EMsvMulitpleMtmsForMoveCommand));
		__ASSERT_DEBUG(mtm==operation->iMtm, PanicServer(EMsvMulitpleMtmsForMoveCommand));
		__ASSERT_DEBUG(service==operation->iService, PanicServer(EMsvMulitpleMtmsForMoveCommand));
		TMsvEntry dEntry;
		TMsvId dService;
		if (iMsvSession.Session().GetEntry(aSelection.At(dCount), dService, dEntry)==KErrNone)
			__ASSERT_DEBUG(dService==iOwningService || (iEntryPtr->iType==KUidMsvServiceEntry && dService==iEntryPtr->Id()) || (iEntryPtr->iType==KUidMsvRootEntry && service==aSelection.At(dCount)), PanicServer(EMsvMovingEntryDifferentOwningService));
		}
#endif

	iMsvSession.Session().MoveEntriesL(aSelection, aTargetId, operation->Id(), operation->iStatus);
	operation->Start();
	CleanupStack::Pop(); // operation
	return operation;
	}


EXPORT_C CMsvOperation* CMsvEntry::CopyL(const CMsvEntrySelection& aSelection, TMsvId aTargetId, TRequestStatus& aStatus)
//
// Copy a selection ocf children to another parent
//
/** Creates, asynchronously. copies of children of the context as new entries owned by the specified 
target ID.

All descendants will be copied as well. Any files and stores associated with 
the entries are also copied.

The returned CMsvOperation object completes when copying is complete.

@param aSelection List of IDs of the entries to be copied 
@param aTargetId The ID of the entry to own the copies 
@param aStatus The request status to be completed when the operation has finished 

@leave KErrNoMemory The operation could not be created or passed to the server 
@leave KErrNotFound An entry was not a child of the context 
@return The operation object controlling the copy command. */
	{
#ifndef _NO_SESSION_LOGGING_
	Log(_L("Asynchronous CopyL of selection of %d entries to %x"), aSelection.Count(), aTargetId); 
#endif

	__ASSERT_DEBUG(iState==EValid || iState==EInvalidDeletedContext || iState==EInvalidChangingContext, PanicServer(EMsvEntryStateUndetermined));
	if (!AreChildren(aSelection))
		User::Leave(KErrNotFound);
	CMsvEntryOperation* operation = CMsvEntryOperation::NewL(iMsvSession, aStatus);
	CleanupStack::PushL(operation);
	User::LeaveIfError(iMsvSession.Session().OperationMtmL(aSelection.At(0), aTargetId, operation->iMtm, operation->iService)); 

#if defined(_DEBUG)
	// check other entries in selection are consistent 
	TInt dCount = aSelection.Count();
	while (dCount--)
		{
		TMsvId service;
		TUid mtm;
		TInt error = iMsvSession.Session().OperationMtmL(aSelection.At(dCount), aTargetId, mtm, service);
		__ASSERT_DEBUG(error==KErrNone || error==KErrNotFound, PanicServer(EMsvMulitpleMtmsForCopyCommand));
		__ASSERT_DEBUG(mtm==operation->iMtm, PanicServer(EMsvMulitpleMtmsForCopyCommand));
		__ASSERT_DEBUG(service==operation->iService, PanicServer(EMsvMulitpleMtmsForCopyCommand));
		TMsvEntry dEntry;
		TMsvId dService;
		if (iMsvSession.Session().GetEntry(aSelection.At(dCount), dService, dEntry)==KErrNone)
			__ASSERT_DEBUG(dService==iOwningService || (iEntryPtr->iType==KUidMsvServiceEntry && dService==iEntryPtr->Id()) || (iEntryPtr->iType==KUidMsvRootEntry && service==aSelection.At(dCount)), PanicServer(EMsvCopyingEntryDifferentOwningService));
		}
#endif

	iMsvSession.Session().CopyEntriesL(aSelection, aTargetId, operation->Id(), operation->iStatus);
	operation->Start();
	CleanupStack::Pop(); // operation
	return operation;
	}

EXPORT_C CMsvOperation* CMsvEntry::CopyL(TMsvId aMsvId, TMsvId aTargetId, TRequestStatus& aStatus)
//
// Copy a single entry to another parent
//
/** Creates, asynchronously, a copy of a child of the context as a new entry owned by the specified 
target ID.

All descendants will be copied as well. Any files and stores associated with 
the entry are also copied.

The returned CMsvOperation object completes when copying is complete.

@param aMsvId The ID of the entry to be copied 
@param aTargetId The ID of the entry to own the copy 
@param aStatus The request status to be completed when the operation has finished 

@leave KErrNoMemory The operation could not be created or passed to the server 
@leave KErrNotFound An entry was not a child of the context 
@return The operation object controlling the copy command. */
	{
#ifndef _NO_SESSION_LOGGING_
	Log(_L("Asynchronous CopyL of entry %x to %x"), aMsvId, aTargetId); 
#endif

	CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection;
	CleanupStack::PushL(selection);
	selection->AppendL(aMsvId);
	CMsvOperation* operation = CopyL(*selection, aTargetId, aStatus);
	CleanupStack::PopAndDestroy(); // selection
	return operation;
	}


TInt CMsvEntry::CopyOneL(TMsvId aMsvId, TMsvId aTargetId)
	{

	CMsvEntryArray* newSortedChildren = NULL;
	CMsvClientEntry *toadd=NULL;

	
	if(aTargetId==iEntryPtr->Id())
		{
		const TMsvEntry &entry=ChildDataL(aMsvId);
		if (entry.Visible() || iOrdering.ShowInvisibleEntries())
			{
			// Create what may be the new child entry if nothing goes wrong
			toadd = CMsvClientEntry::NewLC(entry, EMsvClientChild);

			// Reserve space for the new entry for later
			iEntries->SetReserveL(iEntries->Count() + 1);

			newSortedChildren = CMsvEntryArray::NewLC(*iMtmList);

			// add the children to the sorted pointer list - current context is the first entry
			TInt totalCount=iEntries->Count();
			for (TInt count=1; count<totalCount; count++)
				newSortedChildren->AppendL(&iEntries->At(count)->Entry());

			// We've created a new sorted child list now so we won't leave later
			newSortedChildren->AppendL(&toadd->Entry());
			newSortedChildren->SortL(iOrdering);
			}
		}

	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);
	selection->AppendL(aMsvId);
	TInt opid = iMsvSession.OperationId();
	iMsvSession.Session().CopyEntriesL(*selection, aTargetId, opid);

	TMsvLocalOperationProgress progress;
	TPckg<TMsvLocalOperationProgress> progressPack(progress);
    User::LeaveIfError(iMsvSession.Session().OperationCompletion(opid, progressPack));

	CleanupStack::PopAndDestroy(selection);

	// can't leave after this point
	if(newSortedChildren!=NULL) CleanupStack::Pop(newSortedChildren);
	if(toadd!=NULL) CleanupStack::Pop(toadd);
	if(newSortedChildren!=NULL && progress.iError==KErrNone)
		{
		delete iSortedChildren;
		iSortedChildren=newSortedChildren;
		newSortedChildren=NULL;
		toadd->SetId(progress.iId);
		// Will not leave because we've reserved space earlier
		iEntries->AppendL(toadd);
		toadd=NULL;
		}
	delete newSortedChildren;
	delete toadd;
	return(progress.iError);
	}

EXPORT_C void CMsvEntry::CopyL(TMsvId aMsvId, TMsvId aTargetId)
//
// Copy a single child to another parent (or duplicate)
//
/** Creates, synchronously, a copy of a child of the context as a new entry owned by the specified target ID.

@param aMsvId The ID of the entry to be copied
@param aTargetId The ID of the entry to own the copy
*/
	{	
	__ASSERT_ALWAYS(iOwningService == KMsvLocalServiceIndexEntryId || iOwningService == KMsvRootIndexEntryId, PanicServer(EMsvNotLocalService)); // Root as well?
	__ASSERT_DEBUG(iState==EValid || iState==EInvalidDeletedContext || iState==EInvalidChangingContext, PanicServer(EMsvEntryStateUndetermined));
#ifndef _NO_SESSION_LOGGING_
	Log(_L("Synchronous CopyL %x to %x"), aMsvId, aTargetId); 
#endif
	User::LeaveIfError(CopyOneL(aMsvId,aTargetId));
	}


/** Creates, synchronously. copies of children of the context as new entries owned by the specified 
target ID.

All descendants will be copied as well. Any files and stores associated with 
the entries are also copied.

@param aSelection List of IDs of the entries to be copied 
@param aTargetId The ID of the entry to own the copies 
@param aProgress On return, records the outcome of the copy 

@leave KErrNoMemory 
@leave KErrNotFound An entry was not a child of the context 
*/
EXPORT_C void CMsvEntry::CopyL(const CMsvEntrySelection& aSelection, TMsvId aTargetId, TMsvLocalOperationProgress& aProgress)
//
// Copy a selection to another parent (or duplicate)
//
	{
	__ASSERT_ALWAYS(iOwningService == KMsvLocalServiceIndexEntryId || iOwningService == KMsvRootIndexEntryId, PanicServer(EMsvNotLocalService)); // Root as well?
	__ASSERT_DEBUG(iState==EValid || iState==EInvalidDeletedContext || iState==EInvalidChangingContext, PanicServer(EMsvEntryStateUndetermined));
	__ASSERT_DEBUG(aSelection.Count(), PanicServer(EMsvEmptySelection));

#ifndef _NO_SESSION_LOGGING_
	Log(_L("Synchronous CopyL with selection of %d entries to %x"), aSelection.Count(),aTargetId); 
#endif


	aProgress.iTotalNumberOfEntries=aSelection.Count();
	aProgress.iNumberCompleted=0;
	aProgress.iNumberRemaining=aProgress.iTotalNumberOfEntries;
	aProgress.iError=KErrNone;

	TInt count=aProgress.iTotalNumberOfEntries;
	while(count--)
		{
		aProgress.iId=aSelection.At(count);
		TInt err=CopyOneL(aProgress.iId,aTargetId);
		aProgress.iNumberRemaining--;
		if(err==KErrNone)
			aProgress.iNumberCompleted++;
		else
			{
			aProgress.iError=err;
			aProgress.iNumberFailed++;
			}
		}
	}





EXPORT_C void CMsvEntry::AddObserverL(MMsvEntryObserver& aObserver)
//
// Adds an observer to this entry
// If the function leaves, the observer was not appended
// 
/** Registers an observer for the object. 

CMsvEntry objects can call back observer objects that implement the MMsvEntryObserver 
interface when certain events occur. Any number of observers can be registered.

Observers are called primarily when the context changes state or contents. 
For details, see MMsvEntryObserver::TMsvEntryEvent.

@param aObserver The observer to be registered for events 
@leave KErrNoMemory Not enough memory to register the observer */
	{
	if (iObservers==NULL)
		iObservers=new(ELeave) CArrayPtrFlat<MMsvEntryObserver> (KMsvEntryObserverArrayGranuality);
	iObservers->AppendL(&aObserver);

#ifndef _NO_SESSION_LOGGING_
	Log(_L("Observer %d added"), iObservers->Count()); 
#endif
	}


EXPORT_C void CMsvEntry::RemoveObserver(MMsvEntryObserver& aObserver)
//
// Removes an observer of the entry
//
/** Unregisters an observer previously registered with AddObserverL(). 

@param aObserver A reference to an observer to be unregistered for events */
	{
	__ASSERT_DEBUG(iObservers, PanicServer(EMsvEntryUnknownObserver));
	if (iObservers)
		{
		TInt count=iObservers->Count();
		while (count--)
			{
			if (iObservers->At(count)==&aObserver)
				{
#ifndef _NO_SESSION_LOGGING_
				Log(_L("Observer %d removed"), count + 1); 
#endif
				iObservers->Delete(count);
				if (iObservers->Count()==0)
					{
					delete iObservers;
					iObservers=NULL;
					}
				return;
				}
			}
		__ASSERT_DEBUG(count>=0, PanicServer(EMsvEntryUnknownObserver));
		}
	}


void CMsvEntry::NotifyAllObserversL(MMsvEntryObserver::TMsvEntryEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3)
//
// Notifies all observers of the event affecting the context
//
	{
	if (iObservers==NULL)
		return;
	TInt count=iObservers->Count();
	while (count--)
		iObservers->At(count)->HandleEntryEventL(aEvent,aArg1,aArg2,aArg3);	
	}


void CMsvEntry::HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3)
//
// Is informed of all the session events
// This are filtered to find the events relating to the context
//
	{
	// Check the notification sequence and ignore if neccessary
	if (iNotifySequence >= iMsvSession.iNotifySequence)
		{
#ifndef _NO_SESSION_LOGGING_
		Log(_L("Ignoring notification, %d >= %d"), iNotifySequence, iMsvSession.iNotifySequence); 
#endif
		return;
		}

	switch (aEvent)
		{
		case EMsvEntriesChanged:
			{
			CMsvEntrySelection* selection = (CMsvEntrySelection*) aArg1;
			if (iEntryPtr->Id() == selection->At(0))
				ContextChangedL(MMsvEntryObserver::EMsvEntryChanged);
			else if (iEntryPtr->Id()==*(TMsvId*) aArg2)
				ChildrenChangedL(*selection);
			break;
			}
		case EMsvEntriesCreated:
			if (*(TMsvId*) aArg2==iEntryPtr->Id())
				NewChildrenL(*(CMsvEntrySelection*) aArg1);
			else
				CheckNewGrandchildrenL(*(TMsvId*) aArg2);
			break;
		case EMsvEntriesDeleted:
			{
			CMsvEntrySelection* selection = (CMsvEntrySelection*) aArg1;
			if (*(TMsvId*) aArg2==iEntryPtr->Id())
				DeletedChildrenL(*selection);
			else
				{
				// check if we have been deleted
				TInt index=selection->Find(iEntryPtr->Id());
				if (index!=KErrNotFound)
					{
					iState = EInvalidDeletedContext;
					NotifyAllObserversL(MMsvEntryObserver::EMsvEntryDeleted, NULL, NULL, NULL);
					}
				else
					CheckDeletedGrandchildrenL(*(TMsvId*) aArg2);
				}
			break;
			}
		case EMsvEntriesMoved:
			if (*(TMsvId*) aArg3==iEntryPtr->Parent())
				CheckIfContextMovedL(*(CMsvEntrySelection*) aArg1);
			if (*(TMsvId*) aArg2==iEntryPtr->Id())
				NewChildrenL(*(CMsvEntrySelection*) aArg1);
			else if (*(TMsvId*) aArg3==iEntryPtr->Id())
				DeletedChildrenL(*(CMsvEntrySelection*) aArg1);
			else
				{
				CheckNewGrandchildrenL(*(TMsvId*) aArg2);
				CheckDeletedGrandchildrenL(*(TMsvId*) aArg3);
				}
			break;
		case EMsvMediaChanged:
			{
			TRAPD(error, HandleMediaChangeL());			
			if (error)
				{
				// An error occurred or this is a non standard entry
				// in which case the media has changed so this entry is not accessible
				// Just mark the entry as invalid - there is nothing else we can do!
				iState = EInvalidOldContext;
				NotifyAllObserversL(MMsvEntryObserver::EMsvContextInvalid, (TAny*)&error, NULL, NULL);
				}
			break;
			}
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
		case EMsvRefreshMessageView:
			{
			// A drive/disk has been added/removed.
			if(iEntryPtr->StandardFolder())
				{
				SetEntryNoCheckL(iEntryPtr->Id(), iChildrenOfAvailableDrives);
				}
			break;
			}
#endif
		default:
			break;
		}
	}

void CMsvEntry::HandleMediaChangeL()
	{
	// If this is not a standard entry there is nothing we can do
	if (!iEntryPtr->StandardFolder())
		User::Leave(KMsvMediaChanged);

	// This is a standard folder so it will exist on all media
	// Refresh the entry and child list - if this fails mark the entry as invalid
	// Otherwise the context will be told that everything has changed
	CMsvEntrySelection* oldChildren = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(oldChildren);

	// Get list of old children
	TInt count = iSortedChildren->Count();
	while(count--)
		oldChildren->AppendL(iSortedChildren->At(count)->Id());

	// Refresh the context
	iState = EInvalidOldContext;

#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	if(iChildrenOfAvailableDrives)
		{
		SetStandardFolderEntryL(iEntryPtr->Id());
		}
	else
#endif
		{
		SetEntryL(iEntryPtr->Id());
		}
	CMsvEntrySelection* newChildren = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(newChildren);

	// Get list of new children
	count = iSortedChildren->Count();
	while(count--)
		newChildren->AppendL(iSortedChildren->At(count)->Id());

	// Tell the context about the children that have effectively been deleted and created
	if (oldChildren->Count())
		NotifyAllObserversL(MMsvEntryObserver::EMsvDeletedChildren, (TAny*)oldChildren, NULL, NULL);
	if (newChildren->Count())
		NotifyAllObserversL(MMsvEntryObserver::EMsvNewChildren, (TAny*)newChildren, NULL, NULL);

	// Tell the context that it might have changed
	NotifyAllObserversL(MMsvEntryObserver::EMsvEntryChanged, NULL, NULL, NULL);

	CleanupStack::PopAndDestroy(2); // newChildren, oldChildren
	}

void CMsvEntry::CheckIfContextMovedL(const CMsvEntrySelection& aSelection)
//
// Some of the contexts parents children have moved, have to check if the context was one of them
//
	{
	TInt index=aSelection.Find(iEntryPtr->Id());
	if (index!=KErrNotFound)
		ContextChangedL(MMsvEntryObserver::EMsvEntryMoved);
	}


void CMsvEntry::ContextChangedL(MMsvEntryObserver::TMsvEntryEvent aEvent)
//
// The context has ben changed, spo we need to get the enw version
//
	{
#ifndef _NO_SESSION_LOGGING_
	Log(_L("Context changed")); 
#endif

	if (iEntries->Count()>1 && iEntries->At(1)->Type()==EMsvClientChangedContext)
		{
		// we changed the entry, so get the new conext from the entry list
		// delete the old entry context (the new one is at position 1)
		delete iEntries->At(0);
		iEntries->Delete(0);
		}
	else
		{
		// someone else changed the context, so we have to update
		CMsvClientEntry* cEntry=NULL;
		TMsvId owningService=KMsvNullIndexEntryId;
		TRAPD(error, {cEntry = DoGetEntryLC(iEntryPtr->Id(), owningService); CleanupStack::Pop();});
		if (error==KErrNone)
			{
			if(iEntries->Count() != 0)
				{
				delete iEntries->At(0);
				iEntries->At(0) = cEntry;
				}
			else
				{
				iEntries->AppendL(cEntry); 	
				}
			iOwningService = owningService;
			}
		else
			{
			iState = EInvalidOldContext;
			NotifyAllObserversL(MMsvEntryObserver::EMsvContextInvalid, (TAny*)&error, NULL, NULL);
			return;
			}
		}

	iEntries->At(0)->SetType(EMsvClientContext);
	iEntryPtr = &iEntries->At(0)->Entry();
	iState = EValid;

	// notify all observers
	NotifyAllObserversL(aEvent, NULL, NULL, NULL);
	}



void CMsvEntry::NewChildrenL(const CMsvEntrySelection& aSelection)
//
// New children have been created
//
	{
#ifndef _NO_SESSION_LOGGING_
	Log(_L("New children")); 
#endif

	CMsvEntrySelection* newChildren = DoGetNewChildrenL(aSelection);
	CleanupStack::PushL(newChildren);
	if (newChildren->Count())
		{
		TMsvEntry* ptr = CONST_CAST(TMsvEntry*, iEntryPtr);
		ptr->SetOwner(ETrue);
		NotifyAllObserversL(MMsvEntryObserver::EMsvNewChildren, (TAny*)newChildren, NULL, NULL);
		}
	CleanupStack::PopAndDestroy(); // newChildren
	}


CMsvEntrySelection* CMsvEntry::DoGetNewChildrenL(const CMsvEntrySelection& aSelection)
//
// New children have been created
//
	{
	CMsvEntrySelection* newChildren = aSelection.CopyLC();
	TInt count=aSelection.Count();
	while (count--)
		{
		// check if we already have this child
		if (IsAChild(aSelection.At(count)))
			{
			newChildren->Delete(count);
			continue;
			}

		// get the new child data and add to to the list
		TMsvEntry tEntry;
		TMsvId service;
		TInt error=iMsvSession.Session().GetEntry(aSelection.At(count), service, tEntry);
		if(error!=KErrNotFound) User::LeaveIfError(error);
		
		if (error == KErrNone && (tEntry.Visible() || iOrdering.ShowInvisibleEntries()))
			{
			__ASSERT_DEBUG(service==iOwningService || (iEntryPtr->iType==KUidMsvServiceEntry && service==iEntryPtr->Id()) || (iEntryPtr->iType==KUidMsvRootEntry && service==(aSelection.At(count))), PanicServer(EMsvNewChildDifferentOwningService));

			CMsvClientEntry* cEntry = CMsvClientEntry::NewLC(tEntry, EMsvClientNull);
			cEntry->SetType(EMsvClientChild);
			iEntries->AppendL(cEntry);
			CleanupStack::Pop(); // cEntry
			// resort the children
			iSortedChildren->AppendL(&cEntry->Entry());
			}
		else
			newChildren->Delete(count);
		}
	
	if (newChildren->Count())
		iSortedChildren->SortL(iOrdering);
	CleanupStack::Pop(); // newChildren
	return newChildren;
	}


void CMsvEntry::CheckNewGrandchildrenL(TMsvId aId)
//
//
//
	{
	TInt count=iEntries->Count();
	while (count--)
		{
		if (iEntries->At(count)->Entry().Id()==aId && !iEntries->At(count)->Entry().Owner())
			{
			iEntries->At(count)->SetOwnerFlag(ETrue);
			NotifyChildChangedL(aId);
			break;
			}
		}
	}


void CMsvEntry::CheckDeletedGrandchildrenL(TMsvId aId)
//
//
//
	{
	TInt count=iEntries->Count();
	while (count--)
		{
		if (iEntries->At(count)->Entry().Id()==aId)
			{
			TMsvEntry entry;
			TMsvId service;
			TInt error = iMsvSession.Session().GetEntry(aId, service, entry);
			__ASSERT_DEBUG(error || service==iOwningService || (iEntryPtr->iType==KUidMsvServiceEntry && service==iEntryPtr->Id()) || (iEntryPtr->iType==KUidMsvRootEntry && service==aId), PanicServer(EMsvDeletedGrandChildDifferentOwningService));
			if (error)
				NotifyAllObserversL(MMsvEntryObserver::EMsvChildrenInvalid, (TAny*)&error, NULL, NULL);
			else if (!entry.Owner())
				{
				iEntries->At(count)->SetOwnerFlag(EFalse);
				NotifyChildChangedL(aId);
				}
			break;
			}
		}
	}


void CMsvEntry::NotifyChildChangedL(TMsvId aId)
//
//
//
	{
	CMsvEntrySelection* selection = DoMakeSelectionL(aId);
	CleanupStack::PushL(selection);
	NotifyAllObserversL(MMsvEntryObserver::EMsvChildrenChanged, (TAny*)selection, NULL, NULL);
	CleanupStack::PopAndDestroy(); // selection
	}


CMsvEntrySelection* CMsvEntry::DoMakeSelectionL(TMsvId aId)
//
//
//
	{
	CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection;
	CleanupStack::PushL(selection);
	selection->AppendL(aId);
	CleanupStack::Pop(); // selection
	return selection;
	}


void CMsvEntry::ChildrenChangedL(const CMsvEntrySelection& aSelection)
//
// Children have been changed
//
	{
#ifndef _NO_SESSION_LOGGING_
	Log(_L("Children changed")); 
#endif

	CMsvEntrySelection* changedChildren = new(ELeave) CMsvEntrySelection;
	CleanupStack::PushL(changedChildren);
	CMsvEntrySelection* newChildren = new(ELeave) CMsvEntrySelection;
	CleanupStack::PushL(newChildren);
	CMsvEntrySelection* deletedChildren = new(ELeave) CMsvEntrySelection;
	CleanupStack::PushL(deletedChildren);

	TInt count=aSelection.Count();
	while (count--)
		{
		// get the changed child data
		TMsvId id = aSelection.At(count);
		TMsvEntry tEntry;
		TMsvId service;

		// find the child in the sorted list
		TInt pos=iSortedChildren->Count();
		while (pos--)
			if (iSortedChildren->At(pos)->Id()==id)
				break;

		TInt error = iMsvSession.Session().GetEntry(id, service, tEntry);
		if (error == KErrNotFound)
			{
			if (pos >= 0)
				{
				// The child has been deleted by the server
				DeleteChild(pos);
				deletedChildren->AppendL(id);
				}
			}
		else
			{
			User::LeaveIfError(error);
			__ASSERT_DEBUG(service==iOwningService || (iEntryPtr->iType==KUidMsvServiceEntry && service==iEntryPtr->Id()) || (iEntryPtr->iType==KUidMsvRootEntry && service==id), PanicServer(EMsvChangedChildHasDifferentOwningService));

			if ( pos!=KErrNotFound  )
				{
				// replace it, if showing all children or its was and still is visible
				if (iOrdering.ShowInvisibleEntries() || tEntry.Visible())
					{
					ReplaceChildL(pos, tEntry);
					changedChildren->AppendL(id);
					continue;
					}
				}

			if ( pos==KErrNotFound )
				{
				if (tEntry.Visible())
					{
					// the child has just been made visible so add it to our sorted list
					CMsvClientEntry* cEntry = CMsvClientEntry::NewLC(tEntry, EMsvClientChild);
					iEntries->AppendL(cEntry);
					CleanupStack::Pop(); // cEntry
					iSortedChildren->AppendL(&cEntry->Entry());
					newChildren->AppendL(id);		
					}				
				}
			else if (!tEntry.Visible())
				{
				DeleteChild(pos);
				deletedChildren->AppendL(id);
				}
			}
		}
	
	// resort the children
	if (changedChildren->Count() || newChildren->Count() || deletedChildren->Count())
		{
		iSortedChildren->SortL(iOrdering);
		// notify the observers
		if (changedChildren->Count())
			NotifyAllObserversL(MMsvEntryObserver::EMsvChildrenChanged, (TAny*)changedChildren, NULL, NULL);
		if (newChildren->Count())
			NotifyAllObserversL(MMsvEntryObserver::EMsvNewChildren, (TAny*)newChildren, NULL, NULL);
		if (deletedChildren->Count())
			NotifyAllObserversL(MMsvEntryObserver::EMsvDeletedChildren, (TAny*)deletedChildren, NULL, NULL);
		}

	CleanupStack::PopAndDestroy(3); // changedChildren, newChildren, deletedChildren
	}

void CMsvEntry::DeleteChild(TInt aPosition)
	{
	TMsvId id = iSortedChildren->At(aPosition)->Id();
	iSortedChildren->Delete(aPosition);
	TInt ii=iEntries->Count();
	while (ii--)
		if (iEntries->At(ii)->Entry().Id()==id)
			{
			__ASSERT_DEBUG(iEntries->At(ii)->Type()==EMsvClientChild, PanicServer(EMsvNonChildDeleted));
			delete iEntries->At(ii);
			iEntries->Delete(ii);
			break;
			}
	__ASSERT_DEBUG(ii!=KErrNotFound, PanicServer(EMsvChangedChildNotFound1));
	}

void CMsvEntry::ReplaceChildL(TInt aPosition, const TMsvEntry& aEntry)
//
//
//
	{
	CMsvClientEntry* cEntry = CMsvClientEntry::NewLC(aEntry, EMsvClientChild);
			
	TInt ii=iEntries->Count();
	while (ii--)
		{
		if (iEntries->At(ii)->Entry().Id()==aEntry.Id())
			{
			__ASSERT_DEBUG(iEntries->At(ii)->Type()==EMsvClientChild, PanicServer(EMsvNonChildDeleted));
			delete iEntries->At(ii);
			iEntries->At(ii) = cEntry;
			iSortedChildren->At(aPosition) = &cEntry->Entry();
			break;
			}
		}
	__ASSERT_DEBUG(ii!=KErrNotFound, PanicServer(EMsvChangedChildNotFound1));

	CleanupStack::Pop(); // cEntry
	}



void CMsvEntry::DeletedChildrenL(const CMsvEntrySelection& aSelection)
//
// Some of the children have been deleted
//
	{
#ifndef _NO_SESSION_LOGGING_
	Log(_L("Deleted children")); 
#endif

	CMsvEntrySelection* deletedChildren = aSelection.CopyL();
	CleanupStack::PushL(deletedChildren);

	TInt count=aSelection.Count();
	while (count--)
		{
		TMsvId id = aSelection.At(count);
		TInt ii=iSortedChildren->Count();
		while (ii--)
			{
			if (iSortedChildren->At(ii)->Id()==id)
				{
				iSortedChildren->Delete(ii);
				break;
				}
			}
		if (ii==KErrNotFound)
			deletedChildren->Delete(count);
		else
			{
			ii=iEntries->Count();
			while (ii--)
				{
				if (iEntries->At(ii)->Entry().Id()==id)
					{
					__ASSERT_DEBUG(iEntries->At(ii)->Type()==EMsvClientChild, PanicServer(EMsvNonChildDeleted));
					delete iEntries->At(ii);
					iEntries->Delete(ii);
					break;
					}
				__ASSERT_DEBUG(ii!=KErrNotFound, PanicServer(EMsvDeletedChildNotInMainList));
				}
			}
		}

	// notify all observers
	if (deletedChildren->Count())
		{
		// reset the owner flag
		if (Count()==0)
			{
			TMsvEntry* ptr = CONST_CAST(TMsvEntry*, iEntryPtr);
			ptr->SetOwner(EFalse);
			}

		NotifyAllObserversL(MMsvEntryObserver::EMsvDeletedChildren, (TAny*)deletedChildren, NULL, NULL);
		}

	CleanupStack::PopAndDestroy(); // deletedChildren
	}


CMsvEntryArray* CMsvEntry::GetNewSortedListL(const TMsvSelectionOrdering& aOrdering, const CArrayFix<TUid>& aMtmList)
//
// Gets a new sorted list for new order and mtm list
// The entries should have the correct visiblity
//
	{
	CMsvEntryArray* newSortedChildren = CMsvEntryArray::NewLC(aMtmList);
	if (iSortedChildren->Count())
		{
//		newSortedChildren->InsertL(0, &iSortedChildren->At(0), iSortedChildren->Count());
//		newSortedChildren->SortL(aOrdering);
		TInt count=1;
		if (iEntries->At(count)->Type()!=EMsvClientChild)
			count++;
		TInt totalCount=iEntries->Count();
		for (; count<totalCount; count++)
			newSortedChildren->AppendL(&iEntries->At(count)->Entry());
		newSortedChildren->SortL(aOrdering);
		}
	CleanupStack::Pop(); // newSortedChildren
	return newSortedChildren;
	}


EXPORT_C void CMsvEntry::SetSortTypeL(const TMsvSelectionOrdering& aOrdering)
//
// Sets the sort type
// If this leaves the sort type has not been changed
//
/** Sets the sort order that is used when listing children, for example with ChildrenL().

If the function leaves, the sort order is unchanged.

@param aOrdering Sort order to use 
@leave KErrNoMemory Insufficient memory to resort the entries */
	{
	__ASSERT_DEBUG(iState==EValid || iState==EInvalidDeletedContext || iState==EInvalidChangingContext, PanicServer(EMsvEntryStateUndetermined));

#ifndef _NO_SESSION_LOGGING_
		Log(_L("Sort order changed, sorting %x, invisible %d"), aOrdering.Sorting(), aOrdering.ShowInvisibleEntries()); 
#endif

	if (aOrdering.ShowInvisibleEntries()==iOrdering.ShowInvisibleEntries())
		{
		// just resort the current list of children
		CMsvEntryArray* newSortedChildren = GetNewSortedListL(aOrdering, *iMtmList);
		delete iSortedChildren;
		iSortedChildren = newSortedChildren;
		iOrdering = aOrdering;
		}
	else if (!aOrdering.ShowInvisibleEntries())
		{
		// resort the current list
		CMsvEntryArray* newSortedChildren = GetNewSortedListL(aOrdering, *iMtmList);
		delete iSortedChildren;
		iSortedChildren = newSortedChildren;
		iOrdering = aOrdering;
		// remove the invisible entries from sort list
		TInt count = iSortedChildren->Count();
		while (count--)
			if (!iSortedChildren->At(count)->Visible())
				iSortedChildren->Delete(count);
		// remove the invisible children from main list
		count = iEntries->Count();
		while (count-->=1)
			if (!iEntries->At(count)->Entry().Visible())
			{
			delete iEntries->At(count);
			iEntries->Delete(count);
			}
		}
	else
		{
		// keep old variable
		CArrayPtrFlat<CMsvClientEntry>* oldEntries = iEntries;
		iEntries=NULL;
		CMsvEntryArray* oldSortedChildren = iSortedChildren;
		iSortedChildren=NULL;
		TMsvSelectionOrdering oldOrder = iOrdering;	
		
		iOrdering = aOrdering;
		TRAPD(leave, DoSortTypeL(oldEntries->At(0)));

		if (leave)
			{
			// we left, the function may have created a new iEntries,
			// if iEntries has been created we need to delete all the elements
			// except the first one, that is used in the old list
			// then delete iEntries, and put back the old one.

			if(iEntries!=NULL)
				{
				if(iEntries->Count()!=0) 
					iEntries->Delete(0);
				iEntries->ResetAndDestroy();
				delete iEntries;
				}
			iEntries = oldEntries;

			// iSortedChildren doesn't own the children so just delete the new one.
			// and put the old one back.
			delete iSortedChildren;
			iSortedChildren = oldSortedChildren;
			iOrdering = oldOrder;
			User::Leave(leave);
			}
		else
			{
			oldEntries->Delete(0); // the object is used in new list
			oldEntries->ResetAndDestroy();
			delete oldEntries;
			delete oldSortedChildren;
			}
		}
	}



void CMsvEntry::DoSortTypeL(CMsvClientEntry* aContext)
//
// 
//
	{
	iEntries = new(ELeave) CArrayPtrFlat<CMsvClientEntry>(KMsvClientEntryArrayGranuality);
	iEntries->AppendL(aContext);

	iSortedChildren = CMsvEntryArray::NewL(*iMtmList);

	DoGetChildrenL();
	}



EXPORT_C void CMsvEntry::SetMtmListL(const CArrayFix<TUid>& aMtmList)
//
// Sets the mtm list
// If this leaves the mtm list has not been changed
//
/** Sets the MTM order to the specified sort order. When children of an entry are 
sorted, entries belonging to the same MTM type can be grouped together. 

MTM grouping can be switched on or off through setting the appropriate TMsvSelectionOrdering 
value by SetSortTypeL(). 

If the function leaves, the sort order is unchanged.

@param aMtmList The order of MTMs to use for sorting 
@leave KErrNoMemory Insufficient memory to resort the entries */
	{
	__ASSERT_DEBUG(iState==EValid || iState==EInvalidDeletedContext || iState==EInvalidChangingContext, PanicServer(EMsvEntryStateUndetermined));

	// create new mtm list
	CArrayFixFlat<TUid>* mtmList = new(ELeave)CArrayFixFlat<TUid>(KMsvMtmListGranularity);
	CleanupStack::PushL(mtmList);
	mtmList->InsertL(0, &aMtmList.At(0), aMtmList.Count());

	// create new sorted children list
	CMsvEntryArray* newSortedChildren = GetNewSortedListL(iOrdering, *mtmList);
	
	// install the new sorted children array
	delete iSortedChildren;
	delete iMtmList;
	iSortedChildren = newSortedChildren;
	iMtmList = mtmList;
	
	CleanupStack::Pop(); // mtmList
	}


TBool CMsvEntry::IsAChild(TMsvId aId) const
//
// Returns true if the entry is a child
//
	{
	TInt count=iSortedChildren->Count();
	while (count--)
		{
		if (iSortedChildren->At(count)->Id()==aId)
			return ETrue;
		}
	return EFalse;
	}

TBool CMsvEntry::AreChildren(const CMsvEntrySelection& aSelection) const
//
// Returns true if all the entries are children
//
	{
	TInt count = aSelection.Count();
	if(!count)
	    {
        return EFalse;
	    }
	else
	    {
        while (count--)
            {
            if (!IsAChild(aSelection.At(count)))
                {
                return EFalse;
                }
            }
	    }
	return ETrue;
	}


EXPORT_C TBool CMsvEntry::HasStoreL() const
/** Checks if the context has an associated message store.

@return ETrue: entry has a message store EFalse: entry does not have a message 
store */
	{
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	if(iMsvSession.StoreManager().DoesAnyStoreExists(iEntryPtr->Id(), this->Entry().iMtm))
		return ETrue;
	else
		return iMsvSession.StoreManager().FileStoreExistsL(iEntryPtr->Id());
#else
	return iMsvSession.StoreManager().FileStoreExistsL(iEntryPtr->Id());
#endif
	}

/** Sets or clears multiple fields in a selection of children of the context.

Fields to change are specified using a bitmask of TMsvAttribute values. Possible 
fields that can be changed using this function are the PC synchronisation, Visibility, 
Pending Deletion, Read, In-preparation, Connected, and New flags. 

@param aSelection The entries to change
@param aSetAttributes A bitmask of the fields to set
@param aClearAttributes A bitmask of the fields to clear
@leave KErrNotFound An entry was not a child of the context
@see CMsvSession::ChangeAttributesL() 
*/
EXPORT_C void CMsvEntry::ChangeAttributesL(const CMsvEntrySelection& aSelection, TUint aSetAttributes, TUint aClearAttributes)
	{
#ifndef _NO_SESSION_LOGGING_
	Log(_L("ChangeAttributesL")); 
#endif

	__ASSERT_DEBUG(iState==EValid || iState==EInvalidDeletedContext || iState==EInvalidChangingContext, PanicServer(EMsvEntryStateUndetermined));
	__ASSERT_DEBUG(aSelection.Count(), PanicServer(EMsvEmptySelection));

	if (!AreChildren(aSelection))
		User::Leave(KErrNotFound);

	iMsvSession.Session().ChangeAttributesL(aSelection, aSetAttributes, aClearAttributes);
	}

