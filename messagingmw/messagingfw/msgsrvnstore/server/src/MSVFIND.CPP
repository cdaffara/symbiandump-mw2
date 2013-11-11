// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "MSVIDS.H"
#include "MSVPANIC.H"
#include "MSVUIDS.H"
#include "MTCLREG.H"
#include "MSVFIND.H"
#include "msvchild.h"

const TInt KMessageFindResultGranularity = 8;
const TInt KMtmArrayGranularity = 4;

//**********************************
// CMsvChildMessages
//**********************************

CMsvChildMessages* CMsvChildMessages::NewL(CMsvSession& aSession, TInt aPriority)
	{
	CMsvChildMessages* self = new(ELeave)CMsvChildMessages(aSession, aPriority);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(); // self
	return self;
	}

CMsvChildMessages::CMsvChildMessages(CMsvSession& aSession, TInt aPriority)
: CMsgActive(aPriority), iSession(aSession)
	{
	}

CMsvChildMessages::~CMsvChildMessages()
	{
	Cancel();
	delete iFolders;
	delete iSelection;
	delete iFilter;
	}

void CMsvChildMessages::ConstructL()
	{
	iFolders = new(ELeave)CMsvEntrySelection;
	iSelection = new(ELeave)CMsvEntrySelection;
	iFilter = CMsvEntryFilter::NewL();

	CActiveScheduler::Add(this);
	}

void CMsvChildMessages::StartL(TMsvId aParentId, CMsvEntrySelection& aChildren, TRequestStatus& aStatus)
	{
	__ASSERT_DEBUG(!aChildren.Count(), PanicServer(EMsvSelectionNotEmpty));

	// Check we were given the root, a folder, or a service
	TMsvId service;
	TMsvEntry entry;
	User::LeaveIfError(iSession.GetEntry(aParentId, service, entry));
	if (entry.iType != KUidMsvRootEntry && entry.iType != KUidMsvServiceEntry && entry.iType != KUidMsvFolderEntry)
		User::Leave(KErrArgument);

	iChildren = &aChildren;
	iFolders->Reset();
	iFolders->AppendL(aParentId);

	Queue(aStatus);
	TRequestStatus* status=&iStatus;
	User::RequestComplete(status, KErrNone);
	SetActive();
	}

void CMsvChildMessages::DoRunL()
	{
	// Get folder to process
	TInt id = iFolders->At(0);
	iFolders->Delete(0);

	// Get services
	iFilter->SetType(KUidMsvServiceEntry);
	iSession.GetChildIdsL(id, *iFilter, *iSelection);
	if (iSelection->Count())
		{
		iFolders->InsertL(0, iSelection->Back(0), iSelection->Count());
		iSelection->Reset();
		}

	// Get folders
	iFilter->SetType(KUidMsvFolderEntry);
	iSession.GetChildIdsL(id, *iFilter, *iSelection);
	if (iSelection->Count())
		{
		iFolders->InsertL(0, iSelection->Back(0), iSelection->Count());
		iSelection->Reset();
		}

	// Get messages
	iFilter->SetType(KUidMsvMessageEntry);
	iSession.GetChildIdsL(id, *iFilter, *iSelection);
	if (iSelection->Count())
		{
		iChildren->InsertL(0, iSelection->Back(0), iSelection->Count());
		iSelection->Reset();
		}

	// Is there anything more to process?
	if (iFolders->Count())
		{
		TRequestStatus* status=&iStatus;
		User::RequestComplete(status,KErrNone);
		SetActive();
		}
	}

//**********************************
// TMsvFindResult
//**********************************

/** Default constructor. */
EXPORT_C TMsvFindResult::TMsvFindResult() :
	iPartList(0)
	{
	}

/** Constructor specifying the results of a search.

@param aPartList The parts of the message that contain the search text
@param aId The entry Id of the message that contains the search text
*/
EXPORT_C TMsvFindResult::TMsvFindResult(TMsvPartList aPartList, TMsvId aId)
	: iPartList(aPartList), iId(aId)
	{
	}

//**********************************
// CMsvFindResultSelection
//**********************************

EXPORT_C CMsvFindResultSelection::CMsvFindResultSelection()
	: CArrayFixFlat<TMsvFindResult>(KMessageFindResultGranularity)
/** Constructs an empty results collection. */
	{
	}

EXPORT_C CMsvFindResultSelection* CMsvFindResultSelection::CopyL() const
//
// Create a new instance of the result selection that is an exact copy
//
/** Creates a copy of the results of the text search operation.

The function leaves if memory cannot be allocated for the new object.

@return Pointer to the new copy of the results of the text search operation. */
	{
	CMsvFindResultSelection* newSelection = CopyLC();
	CleanupStack::Pop();
	return newSelection;
	}

EXPORT_C CMsvFindResultSelection* CMsvFindResultSelection::CopyLC() const
//
// Create a new instance of the result selection that is an exact copy
// and push this onto the cleanup stack
//
/** Creates a copy of the results of the text search operation and puts a pointer 
to the new object onto the cleanup stack.

The function leaves if memory cannot be allocated for the new object.

@return Pointer to the new copy of the results of the text search operation. */
	{
	CMsvFindResultSelection* newSelection = new(ELeave)CMsvFindResultSelection;
	CleanupStack::PushL(newSelection);
	if (Count())
		{
		newSelection->ResizeL(Count());
		Mem::Copy(newSelection->Back(0), Back(0), Count() * sizeof(TMsvFindResult));	
		}
	return newSelection;
	}

EXPORT_C TInt CMsvFindResultSelection::Find(TMsvId aId) const
//
// Find the position of a result belonging to an particular entry
// Returns KErrNotFound if there is no such result
//
/** Returns the index of the entry corresponding to the specified entry Id.

@param aId The entry Id. 
@return The index of the entry within the search results which has the specified 
entry Id. KErrNotFound, if there is no matching entry. */
	{
	TInt count = Count();
	if (!count)
		return KErrNotFound;

	const TMsvFindResult* ptr = End(0);
	while (count--)
		if ((--ptr)->iId == aId)
			break;

	return count;
	}

//**********************************
// TMsvFindOperationProgress
//**********************************

/** Default constructor.

The data members are set to 0 or NULL values. */
EXPORT_C TMsvFindOperationProgress::TMsvFindOperationProgress()
	: iError(KErrNone), iCompleted(0), iRemaining(0), iCurrentId(KMsvNullIndexEntryId)
	{
	}

//**********************************
// CMsvFindOperation
//**********************************

EXPORT_C CMsvFindOperation* CMsvFindOperation::FindInChildrenL(CMsvSession& aSession, const TDesC& aTextToFind, TMsvId aParentId, TMsvPartList aPartList, TRequestStatus& aObserverRequestStatus)
/** Creates a new search operation to search for text within a specified root, 
folder or service.

Searching for messages is done recursively through all child services and 
folders. All messages found are searched for text.

@param aSession An open session with the Message Server. 
@param aTextToFind The text to be found. The length of the text to be found 
must not be greater than KMsvMaxFindTextLength, otherwise the function raises 
a MSGS 306 panic. 
@param aParentId The entry Id of the root, a folder or a service. If this is 
the Id of the root entry, then all messages stored in the message store on 
the Symbian OS phone are searched. The function leaves with the KErrArgument 
code if this is neither the entry Id of the root, nor a folder nor a service. 
@param aPartList The parts of the messages which are to be searched. 
@param aObserverRequestStatus The request status object. This is set when the 
search operation is complete. The is set to: KErrNone, if the search operation 
completes successfully. 
@return Pointer to the new search operation object. */
	{
	CMsvFindOperation* self = new(ELeave) CMsvFindOperation(aSession, aTextToFind, aPartList, aObserverRequestStatus);
	CleanupStack::PushL(self);
	self->ConstructFindInChildrenL(aParentId);
	CleanupStack::Pop(); // self
	return self;
	}

EXPORT_C CMsvFindOperation* CMsvFindOperation::FindInSelectionL(CMsvSession& aSession, const TDesC& aTextToFind, const CMsvEntrySelection& aSelection, TMsvPartList aPartList, TRequestStatus& aObserverRequestStatus)
/** Creates a new search operation to search for text within a specified selection 
of messages.

The function leaves with the KErrArgument code if the first entry in the selection 
is not recognised as a message.

@param aSession An open session with the Message Server. 
@param aTextToFind The text to be found. The length of the text to be found 
must not be greater than KMsvMaxFindTextLength, otherwise the function raises 
a MSGS 306 panic. 
@param aSelection A selection of messages to search. The function raises a 
MSGS 258 panic if the selection is empty (i.e. there are zero messages in 
the selection). 
@param aPartList The parts of the messages which are to be searched. 
@param aObserverRequestStatus The request status object. This is set when the 
search operation is complete. The is set to: KErrNone, if the search operation 
completes successfully. KErrArgument, if any of the entries in the selection, 
apart from the first, cannot be recognised as a message. 
@return Pointer to the new search operation object. */
	{
	CMsvFindOperation* self = new(ELeave) CMsvFindOperation(aSession, aTextToFind, aPartList, aObserverRequestStatus);
	CleanupStack::PushL(self);
	self->ConstructFindInSelectionL(aSelection);
	CleanupStack::Pop(); // self
	return self;
	}

/** Constructor, specifying search parameters.

@param aSession An open session with the Message Server
@param aTextToFind The text to be found. The length of the text to be found 
must not be greater than KMsvMaxFindTextLength.
@param aPartList The parts of the messages which are to be searched.
@param aObserverRequestStatus Request status of an observer to signal when the search is
complete.
@panic MSGS 306 aTextToFind is longer than KMsvMaxFindTextLength (debug builds only). In release
builds, a USER 11 panic will occur.
*/
EXPORT_C CMsvFindOperation::CMsvFindOperation(CMsvSession& aSession, const TDesC& aTextToFind, TMsvPartList aPartList, TRequestStatus& aObserverRequestStatus)
	: CMsvOperation(aSession, EPriorityStandard, aObserverRequestStatus), iPartList(aPartList)
	{
	__ASSERT_DEBUG(iTextToFind.Length() <= KMsvMaxFindTextLength, PanicServer(EMsvTooMuchFindTextSpecified));
	iTextToFind.Copy(aTextToFind);
	}

void CMsvFindOperation::ConstructL()
	{
	iSelection = new(ELeave)CMsvEntrySelection;
	iClientRegistry = CClientMtmRegistry::NewL(iMsvSession);
	iMtmArray = new(ELeave)CArrayPtrFlat<CBaseMtm>(KMtmArrayGranularity);
	iFindResultSel = new(ELeave)CMsvFindResultSelection;

	CActiveScheduler::Add(this);
	}

/** Second phase constructor, for constructing a search of an entry
and of all its children.

@param aId The entry Id of the root, folder or service to search
*/
EXPORT_C void CMsvFindOperation::ConstructFindInChildrenL(TMsvId aId)
	{
	ConstructL();
	iCurrentId = aId;
	iState = EMsvExpandingFolders;
	StartL();
	}

/** Second phase constructor, for constructing a search of a specified 
selection of messages.

@param aSelection A selection of messages to search.
@panic MSGS 258 The selection is empty 
*/
EXPORT_C void CMsvFindOperation::ConstructFindInSelectionL(const CMsvEntrySelection& aSelection)
	{
	__ASSERT_ALWAYS(aSelection.Count(), PanicServer(EMsvEmptySelection));

	ConstructL();
	iSelection->AppendL(aSelection.Back(0), aSelection.Count());
	StartL();
	}

/** Destructor. */
EXPORT_C CMsvFindOperation::~CMsvFindOperation()
	{
	Cancel();

	delete iSelection;
	delete iFindResultSel;
	delete iClientRegistry;
	delete iChildMessages;

	// We've probably already deleted this array
	if (iMtmArray)
		{
		iMtmArray->ResetAndDestroy();
		delete iMtmArray;
		}
	}

EXPORT_C const TDesC8& CMsvFindOperation::ProgressL()
/** Returns progress information.

Progress information supplies sufficient data to drive a progress gauge for 
the text search operation and also identifies the message currently being 
searched. It does not provide any information about the search progress within 
a message.

@return A reference to a descriptor holding progress information. Progress 
information is encapsulated by a TMsvFindOperationProgress type object and 
this is returned as a package buffer, a TPckgBuf<TMsvFindOperationProgress> 
object. */
	{
	return iProgress;
	}

EXPORT_C const TDesC8& CMsvFindOperation::FinalProgress()
/** Returns progress information after the search operation is complete.

The function returns the same information as ProgressL() but can only be called 
after the search operation is complete. The function raises a MSGS 285 panic 
if it is called while the search operation is still in progress.

@return A reference to a descriptor holding progress information. Progress 
information is encapsulated by a TMsvFindOperationProgress type object and 
this is returned as a package buffer, a TPckgBuf<TMsvFindOperationProgress> 
object */
	{
	__ASSERT_ALWAYS(!IsActive(), PanicServer(EMsvActiveInFinalProgress));
	return iProgress;
	}

EXPORT_C void CMsvFindOperation::DoCancel()
	{
	if(iChildMessages!=NULL)
		{
		iChildMessages->Cancel();
		}
	Complete(KErrCancel);
	}

void CMsvFindOperation::StartL()
	{
	if (iState == EMsvExpandingFolders)
		{
		// Get list of children
		iChildMessages = CMsvChildMessages::NewL(iMsvSession, EPriorityStandard);
		iChildMessages->StartL(iCurrentId, *iSelection, iStatus);
		}
	else
		{
		// Get the next entry to process
		iCurrentId = iSelection->At(0);
		iProgress().iCurrentId = iCurrentId;
		iSelection->Delete(0);

		TMsvId service;
		User::LeaveIfError(iMsvSession.GetEntry(iCurrentId, service, iEntry));

		// Check the entry is a message
		if (iEntry.iType != KUidMsvMessageEntry)
			User::Leave(KErrArgument);

		TRequestStatus* status=&iStatus;
		User::RequestComplete(status,KErrNone);
		}

	iObserverRequestStatus = KRequestPending;
	SetActive();
	}

EXPORT_C void CMsvFindOperation::RunL()
	{
	TRAPD(error, DoRunL());
	if (error != KErrNone)
		Complete(error);
	}

void CMsvFindOperation::DoRunL()
	{
	if (iState == EMsvExpandingFolders)
		{
		if (iStatus != KErrNone)
			{
			Complete(iStatus.Int());
			return;
			}
		}
	else
		{
		// Give derived class a chance to prevent searching in invalid entries
		if (IsValid(iEntry))
			{
			// Trap any errors so that the find operation can continue
			// should we encounter a corrupt entry or an message where the body cannot be loaded
			TRAPD(error, FindL());
			if (error != KErrCorrupt && error != KErrNotFound)
				User::LeaveIfError(error);
			}

		// Update progress
		iProgress().iCompleted++;
		}

	iState = EMsvFindingText;
	iProgress().iRemaining = iSelection->Count();

	if (iSelection->Count())
		StartL();
	else
		Complete(KErrNone);
	}

void CMsvFindOperation::FindL()
	{
	TMsvPartList parts = 0;

	// Note: Empty search string IS valid
	if (iPartList && iTextToFind.Length())
		{
		// See if the mtm is in our array
		CBaseMtm* baseMtm = NULL;
		TInt count = iMtmArray->Count();
		while(count--)
			{
			if (iMtmArray->At(count)->Type() == iEntry.iMtm)
				{
				baseMtm = iMtmArray->At(count);
				baseMtm->SwitchCurrentEntryL(iEntry.Id());
				break;
				}
			}

		// Create a new mtm
		if (!baseMtm)
			{
			baseMtm = iClientRegistry->NewMtmL(iEntry.iMtm);
			CleanupStack::PushL(baseMtm);
			iMtmArray->AppendL(baseMtm);
			CleanupStack::Pop(); // baseMtm

			// Note: SetCurrentEntry takes ownership on entry
			CMsvEntry* entry = CMsvEntry::NewL(iMsvSession, iEntry.Id(), TMsvSelectionOrdering());
			baseMtm->SetCurrentEntryL(entry);
			}

		TRAPD(error, baseMtm->LoadMessageL());
		if (error == KErrNotSupported) // not all MTMs support LoadMessageL()
			return;
		User::LeaveIfError(error);

		// Perform the search
		parts = baseMtm->Find(iTextToFind, iPartList);
		if (parts == KMsvMessagePartNone)
			return;
		}

	TMsvFindResult result(parts, iEntry.Id());
	iFindResultSel->AppendL(result);
	}

void CMsvFindOperation::Complete(TInt aStatus)
	{
	iProgress().iError = aStatus;
	TRequestStatus* st = &iObserverRequestStatus;
	User::RequestComplete(st, aStatus);

	// Release the mtm's as soon as possible
	if (iMtmArray)
		{
		iMtmArray->ResetAndDestroy();
		delete iMtmArray;
		iMtmArray = NULL;
		}
	}

TBool CMsvFindOperation::IsValid(const TMsvEntry&/* aEntry*/) const
//
// This can be overridden to prevent searching in certain messages
// Allows you to find by date, size,  mtm etc.
//
	/** Determines whether a message is to be included in the text search operation.
	
	The function acts as a filter that decides whether a message should be included 
	in the text search operation. The function is called before the message is 
	searched.
	
	The default implementation always returns true.
	
	Clients can provide their own implementation.
	
	Note: 
	Messages which are not included in the search are still included in the count 
	of entries completed.
	
	@param aEntry The entry Id for a message. 
	@return True, if the message is to be searched. False if the message is not 
	to be searched. */
	{
	return ETrue;
	}
