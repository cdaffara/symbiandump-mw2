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

#include "MSVARRAY.H"
#include "MSVUIDS.H"
#include "MSVPANIC.H"
#include "cmsvdescriptionarray.h"

const TInt KEntryArrayGranularity=32;
const TInt KSortingMtmListGranularity=8;


//**********************************
// TKeyArrayFixPtr
//**********************************

EXPORT_C TAny* TKeyArrayFixPtr::At(TInt anIndex) const
//
// Key class for sorting on dereferenced pointers
//
	{
	if (anIndex==KIndexPtr)
		return((TUint8 *)((*((const TUint8 **)iPtr))+iKeyOffset)); 			
	return((TAny *)((*((const TUint8 **)(iBase->Ptr(anIndex*iRecordLength).Ptr())))+iKeyOffset));
	}


//**********************************
// CMsvEntryArray
//**********************************

EXPORT_C CMsvEntryArray* CMsvEntryArray::NewL(const CArrayFix<TUid>& aMtmList)
	{
	CMsvEntryArray* self = new(ELeave) CMsvEntryArray(aMtmList);
	return self;
	}

EXPORT_C CMsvEntryArray* CMsvEntryArray::NewLC(const CArrayFix<TUid>& aMtmList)
	{
	CMsvEntryArray* self = new(ELeave) CMsvEntryArray(aMtmList);
	CleanupStack::PushL(self);
	return self;
	}

CMsvEntryArray::CMsvEntryArray(const CArrayFix<TUid>& aMtmList)
: CArrayPtrFlat<const TMsvEntry>(KEntryArrayGranularity), iOrigMtmList(aMtmList)
	{}


EXPORT_C CMsvEntryArray::~CMsvEntryArray()
	{
	delete iActualMtmList;
	}


// Subject based sorting is a special case because text such as Re: , Fwd: etc is removed
// before the sort. This means email threads are now appear in sequence.
// The original TMsvEntry SUBJECT string is preserved as the sort takes place on a temporary
// array of CDescription.
// Second part of the method is a bubble sort on DATE which deals with duplicates.
// This means that email threads will be in DATE order, oldest date first. 
void CMsvEntryArray::SubjectBasedSortL(TBool aReverse, const TDesC& aSubjectSkipString)
	{
	if(Count() < 2) // One or less no sort required
		return;

	// Create a temporary array of CDescription's and set the offset for the sort to iDescription
	CMsvDescriptionArray* descriptionArray = new (ELeave) CMsvDescriptionArray();
	CleanupStack::PushL(descriptionArray);
	TKeyArrayFixPtr key(_FOFF(TMsvDescription,iDescription),ECmpCollated);
	TInt count = Count(); // Use this count for both loops
	// Loop through the class instance array and construct a CDescription array ready
	// for the sort.
	for(TInt i =0;i<count;++i)
		{
		TMsvDescription* modified = new (ELeave) TMsvDescription();
		CleanupStack::PushL(modified);
		modified->iEntry = const_cast<TMsvEntry*>(At(i)); // Pointer to the real TMsvEntry
		modified->iDescription.Set(At(i)->iDescription);// Copy of its TPtrC (may be modified)
		TPtrC ptr(modified->iDescription);
		TInt offset(0);
		// Perform the search and skip Re: Fwd: etc if found
		// offset points to the start of the real subject string.
		if((offset = FindSubjectStart(ptr,aSubjectSkipString)) != KErrNotFound) 
			{
			ptr.Set(ptr.Ptr()+offset,ptr.Length()-offset);
			modified->iDescription.Set(ptr);
			}
		descriptionArray->InsertIsqAllowDuplicatesL(modified,key); // Insert into temporary array
		CleanupStack::Pop(modified);
		}
	// Reverse before we do the duplicate sort
	if(aReverse)
		{
		descriptionArray->ReverseOrder();
		}
	// Bubble sort individual email threads if any, on DATE
	// Achieved in a single loop by resetting current to the beginning of the email thread if
	// we are still bubble sorting the thread.
	// Less complicated than creating new (N email threads) x CArrayFix derived classes just
	// for its sort methods
	TMsvDescription** current = &descriptionArray->At(0);
	TMsvDescription** startSort(NULL); // Beginning of an email thread
	TBool threadSorting(EFalse); // Continue bubble sorting a thread whilst this flag is set
	// Loop exits when sorting is false and end of list
	for(;;)
		{
		++current;
		// IF not the end of list AND still in email thread 
		if(current <= &descriptionArray->At(count-1) && (*current)->iDescription == (*(current-1))->iDescription) // Compare current and previous
			{
			if(startSort == NULL) // Duplicate
				{
				startSort = current-1; // Keep reference to the start of the email thread.
				}
			if((*(current-1))->iEntry->iDate < (*current)->iEntry->iDate) // Check the DATE
				{
				threadSorting = ETrue; // Set this flag so we loop through this thread again
				// Perform the swap
				TMsvDescription* temp = *(current-1);
				*(current-1) = *current;
				*current = temp;
				}
			}
		else if(!threadSorting)
			{
			// (end of thread  OR end of list). Either continue to next email thread or finished the list 
			if(current >= &descriptionArray->At(count-1))
				{
				break; // End of list
				}
			else
				{
				startSort = NULL; // Continue to next email thread, if any.
				}
			}
		else
			{
			// End of thread but not yet sorted. 
			// Reset the list pointer to the start of the email thread.
			threadSorting = EFalse;
			current=startSort;
			}
		}
	// Reset the CMsvEntryArray class instance list then repopulate it with the bubble sorted one.
	// The TMsvEntry's are pointers to the ones in the original list
	Reset();
	for(TInt i=0;i<descriptionArray->Count();++i)
		{
		AppendL(descriptionArray->At(i)->iEntry);
		}
	CleanupStack::PopAndDestroy(descriptionArray);
	}

// Return an offset of the real subject. Skips Re: Fwd: etc.
// The string to skip is in the class instance iSubjectSkipString.
// Returns a valid offset or KErrNotFound
TInt CMsvEntryArray::FindSubjectStart(const TDesC& aSubject, const TDesC& aSubjectSkipString) const
	{
	// Set up temporary's for more readable code
	// current moves along the descriptor
	// offset is the index into the descriptor of the real subject. (Can be KErrNotFound)
	TPtrC ptr(aSubject);
	TInt offset(0);
	TInt current(0);
	TInt skipLength = aSubjectSkipString.Length();
	do
		{
		current = ptr.FindF(aSubjectSkipString);
		if(current != KErrNotFound)
			{
			// Found a match so increment past the skip string instance. (Could be more)
			offset+= current + skipLength;
			ptr.Set(aSubject.Ptr()+offset,aSubject.Length()-offset);
			}
		else if(offset == 0)
			{
			// None found
			offset = KErrNotFound;
			}
		}while(current != KErrNotFound);
	return offset;
	}

CMsvDescriptionArray::CMsvDescriptionArray() : CArrayPtrFlat<TMsvDescription>(8)
	{
	}

CMsvDescriptionArray::~CMsvDescriptionArray()
	{
	ResetAndDestroy();
	}

// Reverse on CDescription array
void CMsvDescriptionArray::ReverseOrder()
	{
	TInt c=Count()-1;
	if (c>0)
		{
		TMsvDescription** low=&At(0);
		TMsvDescription** high=low+c;
		while (low<high)
			{
			TMsvDescription* t=*low;
			*low++=*high;
			*high--=t;
			}
		}
	}



EXPORT_C void CMsvEntryArray::SortL(TMsvSelectionOrdering aOrdering)
//
// sorts this array
//
	{
	// If we're going to reverse the array first sort by id so sorting twice will give the same result
	switch(aOrdering.Sorting())
		{
		case EMsvSortByDateReverse:
		case EMsvSortByIdReverse:
		case EMsvSortBySizeReverse:
		case EMsvSortByDescriptionReverse:
		case EMsvSortByDetailsReverse:
			{
			TKeyArrayFixPtr key = MessageSortKey(EMsvSortById);
			User::LeaveIfError(Sort(key));
			break;
			}
		default: // Not required - break
			break;
		}
	if (Count() && (aOrdering.Sorting()!=EMsvSortByNone || aOrdering.GroupingOn()))
		GroupL(EGroupByStandardFolders, aOrdering, (aOrdering.Sorting()!=EMsvSortByNone));
	}

void CMsvEntryArray::GroupL(TGroupCriterion aGroupCriterion,TMsvSelectionOrdering aOrdering,TBool aDoSort)
//
// This function works recursively, grouping and sorting the entry selection.  The 
// 'sort' happens at the same time as the 1st 'group', then separate grouped arrays 
// are grouped indidvidually and then merged together at the end. The order in which 
// the grouping occurs is determined by the CMsvEntryArray::TGroupCriterion enum
//	
	{
	TMsvSorting sortType=aOrdering.Sorting();
	TKeyArrayFixPtr key=MessageSortKey(sortType);
	
	if (aGroupCriterion==EStopGrouping)
		{  // if you haven't sorted yet
		if (aDoSort)
			{
			// Subject based sorting requires a special algorithm. Only message entries are treated as other entries normally
			// do not have a prefix like e.g. "re: " or "fwd: "
			if(At(0)->iType == KUidMsvMessageEntry && (sortType == 	EMsvSortByDescription || sortType == EMsvSortByDescriptionReverse))
				{
				SubjectBasedSortL(sortType == EMsvSortByDescriptionReverse,aOrdering.SubjectSkipString());							
				}
			else
				{
				CMsvEntryArray* temp=CMsvEntryArray::NewLC(iOrigMtmList);
				TInt count=Count();
				if (count)
					temp->InsertL(0,&(*(this))[0],count);
				Reset();
				const TMsvEntry** entry = &temp->At(0);
				while (count--)
					InsertIsqAllowDuplicatesL(*entry++, key); // Sorted
				ReverseOrder(sortType);
				CleanupStack::PopAndDestroy(); // temp
				}
			if (At(0)->iType == KUidMsvMessageEntry && (sortType == EMsvSortByDetails || sortType == EMsvSortByDetailsReverse))
				{
				DetailBasedSortL(); // Sort blocks of messages with matching details into newest first
				}
			}
		else
			{
			// The aDoSort flag is not set, but we still need to do a subject
			// based sort if this array contains only message entries, and we are
			// sorting by description. Alternatively, we need to do a date based sort
			// if this array contains only message entries and we are sorting by detail.
			// In order to ensure the array contains only message entries, we
			// check that we have previously grouped the entries by type which would
			// have put all the message entries together in their own array.
			if (Count() > 0 && At(0)->iType == KUidMsvMessageEntry && OkToGroup(EGroupByType, aOrdering))
				{
				if (sortType == EMsvSortByDescription || sortType == EMsvSortByDescriptionReverse)
					{
					SubjectBasedSortL(sortType == EMsvSortByDescriptionReverse,aOrdering.SubjectSkipString());
					}
				else if (sortType == EMsvSortByDetails || sortType == EMsvSortByDetailsReverse)
					{
					DetailBasedSortL(); // Sort blocks of messages with matching details into newest first
					}
				}
			}
		return;
		}

	if (OkToGroup(aGroupCriterion, aOrdering))
		{
		//
		// Copy contents into temp and then put new grouped contents into 'this'
		TInt count=Count();
		if (count==0)
			return; // nothing to do here
		const TInt numberOfArrays=NumberOfArraysToSplitIntoL(aGroupCriterion);
		if (numberOfArrays<1)  // cannot group on this so move on to next grouping
			{
			GroupL(TGroupCriterion(aGroupCriterion+1), aOrdering, aDoSort);
			return;
			}
		CMsvEntryArray* temp;
		if (iActualMtmList)
			temp = CMsvEntryArray::NewLC(*iActualMtmList);
		else
			temp = CMsvEntryArray::NewLC(iOrigMtmList);
		temp->InsertL(0,&(*(this))[0],count);
		Reset();

		//
		// create the separate arrays for each group
		CArrayFixFlat<CMsvEntryArray*>* arrays=new(ELeave) CArrayFixFlat<CMsvEntryArray*>(numberOfArrays);
		CleanupStack::PushL(arrays);
		for (TInt ii=0; ii<numberOfArrays; ii++)
			{
			if (iActualMtmList)
				arrays->AppendL(CMsvEntryArray::NewLC(*iActualMtmList));
			else
				arrays->AppendL(CMsvEntryArray::NewLC(iOrigMtmList));
			}

		//
		// split the selection into the correct group, 
		// sorting aswell if needed and not doing standard folders
		const TMsvEntry** entry = &temp->At(0);
		if (!aDoSort || aGroupCriterion==EGroupByStandardFolders)
			{
			while (count--)
				{
				arrays->At(ArrayId(*entry,aGroupCriterion))->AppendL(*entry);
				entry++;
				}
			}
		else if (aGroupCriterion==EGroupByType)
			{
			TKeyArrayFixPtr folderKey = TKeyArrayFixPtr(_FOFF(TMsvEntry,iDetails),ECmpCollated);
			while (count--)
				{
				if ((*entry)->iType==KUidMsvFolderEntry)
					arrays->At(ArrayId(*entry, aGroupCriterion))->InsertIsqAllowDuplicatesL(*entry, folderKey);
				else
					arrays->At(ArrayId(*entry, aGroupCriterion))->InsertIsqAllowDuplicatesL(*entry, key);
				entry++;
				}
			for (TInt jj=0; jj<numberOfArrays; jj++)
				{
				if (arrays->At(jj)->Count() && arrays->At(jj)->At(0)->iType!=KUidMsvFolderEntry)
					arrays->At(jj)->ReverseOrder(sortType);
				}
			aDoSort=EFalse; 
			}
		else
			{
			while (count--)
				{
				arrays->At(ArrayId(*entry, aGroupCriterion))->InsertIsqAllowDuplicatesL(*entry, key); // Sorted
				entry++;
				}
			for (TInt jj=0; jj<numberOfArrays; jj++)
				arrays->At(jj)->ReverseOrder(sortType);
			aDoSort=EFalse; 
			}

		
		
		
		
		//
		// group further - but check that standard entries and grouped folders are not grouped anymore
		if (aGroupCriterion==EGroupByStandardFolders)
			{
			__ASSERT_DEBUG(numberOfArrays==2, PanicServer(EMsvToManyGroups));
			arrays->At(0)->GroupL(TGroupCriterion(aGroupCriterion+1), aOrdering, aDoSort);
			}
		else if (aGroupCriterion==EGroupByType)
			{
			for (TInt jj=0; jj<numberOfArrays; jj++)
				if (arrays->At(jj)->Count() && arrays->At(jj)->At(0)->iType!=KUidMsvFolderEntry)
					arrays->At(jj)->GroupL(TGroupCriterion(aGroupCriterion+1), aOrdering, aDoSort);
			}
		else 
			{
			for (TInt jj=0; jj<numberOfArrays; jj++)
				arrays->At(jj)->GroupL(TGroupCriterion(aGroupCriterion+1), aOrdering, aDoSort);
			}
		
		//
		// merge the separate arrays into 'this'
		for (TInt kk=0; kk<numberOfArrays; kk++)
			{
			count=arrays->At(kk)->Count();
			if (count)
				InsertL(0,&(*(arrays->At(kk)))[0],count);
			}
		CleanupStack::PopAndDestroy(numberOfArrays+2); // arrays contents + temp + arrays
		}
	else // move on to the next grouping
		GroupL(TGroupCriterion(aGroupCriterion+1), aOrdering, aDoSort);
	}


TInt CMsvEntryArray::NumberOfArraysToSplitIntoL(TGroupCriterion aGroupCriterion)
//
// Returns the number of arrays the selection will be split into with this grouping method
//
	{
	switch (aGroupCriterion)
		{
		case EGroupByType:
			return 5;
		case EGroupByStandardFolders:
			return 2;
		case EGroupByPriority:
			return 3;
		case EGroupByMtm:
			{
			// copy the orig mtm list
			if (iActualMtmList==NULL)
				iActualMtmList = new(ELeave) CArrayFixFlat<TUid>(KSortingMtmListGranularity);
			else
				iActualMtmList->Reset();

			if (iOrigMtmList.Count())
				iActualMtmList->InsertL(0, &iOrigMtmList.At(0), iOrigMtmList.Count());

			// need to add any unknown MTM to the list 
			TKeyArrayFix key = TKeyArrayFix(0, ECmpTUint);
			for (TInt ii=0; ii<Count(); ii++)
				{
				TUid uid = At(ii)->iMtm;
				TInt temp;
				if (iActualMtmList->Find(uid, key, temp))
					iActualMtmList->AppendL(uid);
				}	
			return iActualMtmList->Count();
			}
		default: // case EStopGrouping:
			return 0;
		}
	}


TBool CMsvEntryArray::OkToGroup(TGroupCriterion aGroupCriterion,TMsvSelectionOrdering aOrdering) const
//
// Returns true if the current grouping method is selected
// 
	{
	switch (aGroupCriterion)
		{
		case EGroupByType:
			return aOrdering.GroupByType();
		case EGroupByStandardFolders:
			return aOrdering.GroupStandardFolders();
		case EGroupByPriority:
			return aOrdering.GroupByPriority();
		case EGroupByMtm:
			return aOrdering.GroupByMtm();
		default: //case EStopGrouping:
			return EFalse;
		}
//	return EFalse;
	}

TInt CMsvEntryArray::ArrayId(const TMsvEntry* aEntry, TGroupCriterion aGroupCriterion) const
//
// Returns the id of the array to plave the entry into
//
	{
	switch (aGroupCriterion)
		{
		case EGroupByType:
			{
			switch (aEntry->iType.iUid)
				{
				case KUidMsvAttachmentEntryValue:
					return 1;
				case KUidMsvMessageEntryValue:
					return 2;
				case KUidMsvFolderEntryValue:
					return 3;
				case KUidMsvServiceEntryValue:
					return 4;
				default:
					return 0; // unknown types are grouped after attachments
				}
			}
		case EGroupByPriority:
			{
			switch (aEntry->Priority())
				{
				case EMsvLowPriority:
					return 0;
				case EMsvMediumPriority:
					return 1;
				case EMsvHighPriority:
					return 2;
				default:
					PanicServer(EMsvUnknownPriority);
				}
			}
		case EGroupByMtm:
			{
			__ASSERT_DEBUG(iActualMtmList, PanicServer(EMsvMtmListNotDefined));
			TInt index;
			TKeyArrayFix key(0, ECmpTUint);
			TInt error = iActualMtmList->Find(aEntry->iMtm, key, index);
			__ASSERT_ALWAYS(error==KErrNone, PanicServer(EMsvUnknownMtm));
			return iActualMtmList->Count() - index - 1;
			}
		case EGroupByStandardFolders:
			return aEntry->StandardFolder() ? 1 : 0;
		default:
			return -1;
		}
	}


TKeyArrayFixPtr CMsvEntryArray::MessageSortKey(TMsvSorting aSortType) const
//
// Return appropriate key for desired sort
//
	{
	switch(aSortType)
		{
		case EMsvSortByDate:
		case EMsvSortByDateReverse:
			{
			return TKeyArrayFixPtr(_FOFF(TMsvEntry,iDate),ECmpTInt64);
			}
		case EMsvSortBySize:
		case EMsvSortBySizeReverse:
			{
			return TKeyArrayFixPtr(_FOFF(TMsvEntry,iSize),ECmpTUint);
			}
		case EMsvSortByDescription:
		case EMsvSortByDescriptionReverse:
			{
			return TKeyArrayFixPtr(_FOFF(TMsvEntry,iDescription),ECmpCollated);
			}
		case EMsvSortByDetails:
		case EMsvSortByDetailsReverse:
			{
			return TKeyArrayFixPtr(_FOFF(TMsvEntry,iDetails),ECmpCollated);
			}
		case EMsvSortById:
		case EMsvSortByIdReverse:
			{
			return TKeyArrayFixPtr(_FOFF(TMsvEntry,iId),ECmpTInt32);
			}
		// case EMsvSortByNone:
		default:
			return TKeyArrayFixPtr(0,ECmpNormal);			
		}
	}

void CMsvEntryArray::ReverseOrder(TMsvSorting aSortType)
//
// Reverse based in SortBy value
//
	{
	switch(aSortType)
		{
		case EMsvSortByDateReverse:
		case EMsvSortByIdReverse:
		case EMsvSortBySizeReverse:
		case EMsvSortByDescriptionReverse:
		case EMsvSortByDetailsReverse:
			ReverseOrder();
			break;
		default: // Not required - break
			return;
		}
	}

void CMsvEntryArray::ReverseOrder()
//
// Atomic selection reverse function
//
	{
	TInt c=Count()-1;
	if (c>0)
		{
		const TMsvEntry** low=&At(0);
		const TMsvEntry** high=low+c;
		while (low<high)
			{
			const TMsvEntry* t=*low;
			*low++=*high;
			*high--=t;
			}
		}
	}

void CMsvEntryArray::DetailBasedSortL()
	{
	// Find blocks of entries with matching details
	TInt blockStart = 0;

	while (blockStart < Count())
		{
		const TMsvEntry* firstEntry = At(blockStart);
		TInt nextBlock = blockStart + 1;

		// Find the start of the next block		
		while (nextBlock < Count() && At(nextBlock)->iDetails == firstEntry->iDetails)
			{
			nextBlock++;
			}
		const TInt blockSize = nextBlock - blockStart;

		// Sort the block by date, newest first
		if (blockSize > 1)
			{
			TKeyArrayFixPtr key(_FOFF(TMsvEntry,iDate),ECmpTInt64);
			CMsvEntryArray* temp = CMsvEntryArray::NewLC(iOrigMtmList);
			const TMsvEntry** entry = &At(blockStart);

			// Copy block entries in sequence to temporary array
			for (TInt tempIndex = 0; tempIndex < blockSize; tempIndex++, entry++)
				{
				temp->InsertIsqAllowDuplicatesL(*entry, key);
				}
			// Reverse to give newest first
			temp->ReverseOrder();

			// Replace block with sorted entries
			entry = &At(blockStart);
			for (TInt tempIndex = 0; tempIndex < blockSize; tempIndex++, entry++)
				{
				*entry = temp->At(tempIndex);
				}

			CleanupStack::PopAndDestroy(temp);
			}

		// Move on to next block of entries
		blockStart = nextBlock;
		}
	}

