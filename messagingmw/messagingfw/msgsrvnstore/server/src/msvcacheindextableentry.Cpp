// Copyright (c) 2007-2010 Nokia Corporation and/or its subsidiary(-ies).
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
// CMSVCACHEINDEXTABLEENTRY.CPP
// HEADER FILES
// 
//

#include "msvcacheindextableentry.h"
#include "msventryfreepool.h"
#include "msvcacheentry.h"

#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)	
	#include "msvindexadapter.h"
#endif

/**
 * FUNCTION DEFINITION
 */
 

//****************************
//CMsvCacheIndexTableEntry
//****************************

/**
 * NewL()
 * @param None.
 * @return The newly created index table entry
 * 
 * It returns an instance of the CMsvCacheIndexTableEntry class, i.e. a newly 
 * constructed index table with no entries in it.
 */
CMsvCacheIndexTableEntry* CMsvCacheIndexTableEntry::NewL()
	{
	CMsvCacheIndexTableEntry *self = new(ELeave) CMsvCacheIndexTableEntry();
	return self;
	}


/**
 * NewL()
 * @param CMsvCacheEntry& : a new cache entry
 * @return The newly created index table entry
 * 
 * It returns an instance of the CMsvCacheIndexTableEntry class with the passed entry
 * added.
 */
CMsvCacheIndexTableEntry* CMsvCacheIndexTableEntry::NewL(CMsvCacheEntry*& aEntry)
	{
	CMsvCacheIndexTableEntry *self = CMsvCacheIndexTableEntry::NewLC(aEntry);
	CleanupStack::Pop();
	return self;
	}


/**
 * NewLC()
 * @param CMsvCacheEntry& : a new cache entry
 * @return The newly created index table entry
 * 
 * It returns an instance of the CMsvCacheIndexTableEntry class 
 * with the passed entry added. It leaves the object into cleanup stack.
 */
CMsvCacheIndexTableEntry* CMsvCacheIndexTableEntry::NewLC(CMsvCacheEntry*& aEntry)
	{
	CMsvCacheIndexTableEntry *self = new(ELeave) CMsvCacheIndexTableEntry();
	CleanupStack::PushL(self);
	self->AddEntryL(aEntry);
	return self;
	}




/**
 * NewL()
 * @param RPointerArrayCMsvCacheEntry>& : RPointerArray reference to child entries
 * @return The newly created index table entry
 * 
 * It returns an instance of the CMsvCacheIndexTableEntry class filled with child
 * entries.
 */
CMsvCacheIndexTableEntry* CMsvCacheIndexTableEntry::NewL(RPointerArray<CMsvCacheEntry>& aEntries, TInt aInitIndex /*(DEFALUT=0)*/, TInt aCount /* DEFAULT = -1*/)
	{
	CMsvCacheIndexTableEntry *self = CMsvCacheIndexTableEntry::NewLC(aEntries, aInitIndex, aCount);
	CleanupStack::Pop();
	return self;
	}




/**
 * NewLC()
 * @param RPointerArrayCMsvCacheEntry>& : RPointerArray reference to child entries
 * @return The newly created index table entry
 * 
 * It returns an instance of the CMsvCacheIndexTableEntry class filled with child
 * entries. It leaves the object into cleanup stack.
 */
CMsvCacheIndexTableEntry* CMsvCacheIndexTableEntry::NewLC(RPointerArray<CMsvCacheEntry>& aEntries, TInt aInitIndex /*(DEFALUT=0)*/, TInt aCount /* DEFAULT = -1*/)
	{
	CMsvCacheIndexTableEntry *self = new(ELeave) CMsvCacheIndexTableEntry();
	CleanupStack::PushL(self);
	self->AddEntrySetL(aEntries, aInitIndex, aCount);
	return self;
	}




/**
 * CMsvCacheIndexTableEntry()
 * @param None.
 * @return None.
 * 
 * Constructor
 */
CMsvCacheIndexTableEntry::CMsvCacheIndexTableEntry()
:iMinMsvId(0), iMaxMsvId(0), iFlags(EMsvCacheIndexTableClearFlag)
	{
	iTimeStamp.HomeTime(); //set the timestamp to number of seconds since Epoc.
	iBlockPtr = NULL;
	}


/**
 * ~CMsvCacheIndexTableEntry()
 * @param None.
 * @return None.
 * 
 * Destructor - releases the block to the cache free pool and sets it to NULL.
 */
CMsvCacheIndexTableEntry::~CMsvCacheIndexTableEntry()
	{
	//destructor should delete everything no matter if entries are locked or not
	ReleaseBlock(ETrue);
	}


/**
 * AddEntryL()
 * @param CMsvCacheEntry& : reference to the CMsvCacheEntry to be added.
 * @return None.
 * 
 * The function adds an entry into the index table. The entity who passes the entry
 * to the function will give up ownership of the same and hence cannot modify or 
 * delete it.
 * 1. If the block is NULL, it creates a new one and appends the entry to the block
 *    and accordingly sets the TMsvId ranges.
 * 2. If block is not NULL, then append the entry to it.
 * 	2.1 If the entry already exists in the block then release the old entry from the block and
 *		append the new one.
 * 	2.2 If the entry fits only at the end of the last block then we need to change
 *      the iMaxMsvid.
 *      If the indextable has not been created and if the folder has just one block, and this block contains
 * 		random entries, then we may need to set the iMinMsvId.
 */
void CMsvCacheIndexTableEntry::AddEntryL(CMsvCacheEntry*& aEntry, TBool aReplace /* DEFAULT = EFalse*/)
	{
	TMsvId entryId = aEntry->GetId();
	//	1. If the block is NULL, it creates a new one and appends the entry to the block
	//     and accordingly sets the TMsvId ranges.
	if(NULL == iBlockPtr)
		{
		iBlockPtr = new(ELeave) RPointerArray<CMsvCacheEntry>;
		iBlockPtr->AppendL(aEntry);
		SetMinMsvIdRange(entryId);
		SetMaxMsvIdRange(entryId);
		} //if(NULL == iBlockPtr)
	
	//	2. If block is not NULL:
	else
		{
		// 2.1 If the entry already exists in the block then release the old entry from the
		//	   block and append the new one.
		TInt blockSize = Size();
		for(TInt blockIndex = 0; blockIndex < blockSize; ++blockIndex)
			{
			if((*iBlockPtr)[blockIndex]->GetId() == aEntry->GetId())
				{
				if(!aReplace)
					{
					CMsvEntryFreePool::Instance()->ReleaseEntryL(aEntry);
					aEntry = (*iBlockPtr)[blockIndex];
					return;
					}
				CMsvEntryFreePool::Instance()->ReleaseEntryL((*iBlockPtr)[blockIndex]);
				iBlockPtr->Remove(blockIndex);
				iBlockPtr->Insert(aEntry, blockIndex);
				return;
				}
			}
		iBlockPtr->AppendL(aEntry);
		
		// 2.2 If the entry fits only at the end of the last block then we need to
		//     change the iMaxMsvid.
		//     If the indextable has not been created and if the folder has just
		//     one block, and this block contains random entries, then we may need to 
		//     set the iMinMsvId.
		if(entryId > iMaxMsvId)
			{
			SetMaxMsvIdRange(entryId);
			}
			
		if(entryId < iMinMsvId)
			{
			SetMinMsvIdRange(entryId);
			}
		} //else
	iTimeStamp.HomeTime();
	}




/**
 * AddEntrySetL()
 * @param RPointerArray<CMsvCacheEntry>& : reference to the new entry set to be added.
 * 										   These entries can either be immediate children
 * 										   or non-immediate children, but not both.
 * @param TInt aInitIndex : Start index of the entry to be added in the entryList.
 * @param TInt aCount     : Number of entries that should be added.
 * @return None.
 * 
 * The function adds a set of entries to the index table. The caller who passes the 
 * array of entries to the function will give up ownership of the entries in the array.
 * 1. When the block is NULL:
 * 	1.1 If children of a non-immediate folder are present in the array then set the 
 *      grandchildren and dirty flags.
 * 	1.2 If there aren't, then clear the above two flags.
 * 	1.3 Assign the new entry set to the block, but reset the passed set so that
 *	   the caller cannot modify or delete the cache entries.
 * 2. When the block is not NULL:
 *  2.1 If children of a non-immediate folder are present in the array then
 *	    set the grandchildren flag and simply append entries to the block.
 *  2.2 If there aren't, append entries avoiding duplication.
 * 		2.2.1 Search for each entry that is present in the current block in the
 *			  new set passed. If the entry is present then release it from the
 *			  current block. If not then append to the new set.
 * 		2.2.2 All entries are now in the passed array. Reset the current block,
 *			  allocate space for it and copy the entries to it. Also reset the 
 *			  passed array so that callers do not have access to the cache
 *			  entries after the addition.
 * 3. Calculate and set the maximum and minimum TMsvId ranges in the newly added set.
 */
void CMsvCacheIndexTableEntry::AddEntrySetL(RPointerArray<CMsvCacheEntry>& aEntries, TInt aInitIndex /*(DEFAULT=0)*/, TInt aCount /* DEFAULT=-1 */)
	{
	// Add all entries if aCount is -1.
	TInt arrSize = aEntries.Count();
	if(-1 == aCount)
		{
		aCount = arrSize;
		}
	if(aInitIndex < 0 || aInitIndex >= arrSize)
		{
		return;
		}
	
	// 1. When the block is NULL:
	if(NULL == iBlockPtr)
		{
		//     Assign the new entry set to the block, but reset the passed set so that
		//	   the caller cannot modify or delete the cache entries.
		//	   Update the ranges of the block.
		
		iBlockPtr = new(ELeave) RPointerArray<CMsvCacheEntry>;
		for(TInt listIndex = 0; (listIndex < aCount) && (arrSize > listIndex+aInitIndex); ++listIndex)
			{
			iBlockPtr->AppendL(aEntries[listIndex+aInitIndex]);
			}
		
		} //if(NULL == iBlockPtr)
	
	// 2. When the block is not NULL append the entries. If there is a duplicate entry,
	//	  discard the new entry.
	else
		{
		TBool entryFound = EFalse;
		TInt blockSize = Size();
		for(TInt listCount = 0; (listCount < aCount) && (arrSize > listCount+aInitIndex); ++listCount)
			{
			entryFound = EFalse;
			for(TInt blockIndex = 0; blockIndex < blockSize; ++blockIndex)
				{
				if(aEntries[listCount+aInitIndex]->GetId() == (*iBlockPtr)[blockIndex]->GetId())
					{
					CMsvEntryFreePool::Instance()->ReleaseEntryL(aEntries[listCount+aInitIndex]);
					aEntries.Remove(listCount+aInitIndex);
					aEntries.InsertL((*iBlockPtr)[blockIndex], listCount+aInitIndex);
					entryFound = ETrue;
					break;
					}
				}   //for()
			if(!entryFound)
				{
				iBlockPtr->AppendL(aEntries[listCount+aInitIndex]);
				}			
			}   //for()
		} //else
	iTimeStamp.HomeTime();
	}
	
	


/**
 * GetEntry()
 * @param TMsvId : the TMsvId of the entry to be fetched.
 * @param CMsvCacheEntry*& : output parameter for the entry.
 * @return TBool.
 * 
 * The function fetches the entry with the specified TMsvId.
 * 1. If the block is NULL, there is no entry that can be fetched. Return EFalse.
 * 2. Iterate through the block searching for the TMsvId and return ETrue for a hit
 * 3. Return EFalse if the entry is not present in the block.
 */
TBool CMsvCacheIndexTableEntry::GetEntry(TMsvId aId, CMsvCacheEntry*& aEntry)
	{
	// 1. If the block is NULL, there is no entry that can be fetched. Return EFalse.
	if(NULL == iBlockPtr)
		{
		aEntry = NULL;
		return EFalse;
		}
	
	//	2. Iterate through the block searching for the TMsvId and return ETrue for a hit.
	TInt blockSize = Size();
	for(TInt index = 0 ; index < blockSize ; ++index)
		{
		if((*iBlockPtr)[index]->GetId() ==  aId)
			{
			aEntry = (*iBlockPtr)[index];
			iTimeStamp.HomeTime();
			return ETrue;
			}
		} //for
	
	//	3. Return EFalse if the entry is not present in the block.
	return EFalse;
	}


/**
 * GetChildrenL()
 * @param TMsvId : TMsvId of the parent entry of the entries to be fetched.
 * @param RPointerArray<CMsvCacheEntry>*& : output parameter for the entries to be
 *        fetched.
 * @return None.
 * 
 * The function fetches a set of entries with the specified parent TMsvId.
 * 1. Check if there are grandchildren present in the block.
 * 	1.1. If yes, then fetch only those entries with parent Id as aParentId.
 * 		 If the array of entries is NULL, then allocate space for it.
 * 	1.2. If not, then return a copy of the whole block.
 */
void CMsvCacheIndexTableEntry::GetChildrenL(TMsvId aParentId, RPointerArray<CMsvCacheEntry>& aEntries) /*const*/
	{ 
	// 1. Check if there are grandchildren present in the block.
	if(IsGrandChildPresent())
		{
		// 1.1 If yes, then fetch only those entries with parent Id as aParentId.
		//       If the array of entries is NULL, then allocate space for it.
		TInt blockSize = Size();
		for(TInt index = 0 ; index < blockSize ; ++index)
			{
			if((*iBlockPtr)[index]->Entry().Parent() == aParentId)
				{
				aEntries.AppendL((*iBlockPtr)[index]);
				}
			}
		} //if(IsGrandChildPresent())
	
	else
		{
		// 1.2 If not, then return a copy of the whole block.
		TInt blockSize = Size();
		for(TInt index = 0 ; index < blockSize ; ++index)
			{
			aEntries.AppendL((*iBlockPtr)[index]);
			}
		} //else
	iTimeStamp.HomeTime();
	}
	

/**
 * DeleteEntryL()
 * @param TMsvId : the TMsvId of the entry to be deleted.
 * @param aParentIdOfEntry
 * @param aForceDelete TBool, it indicates whether the entry needs to be locked while deleting.
 * @return None.
 * 
 * The function deletes an entry from the cache releasing the entry to the cache free
 * pool. The function is called when the entry is being deleted from both the DB and
 * cache.
 * 1. Return if the block is NULL.
 * 2. Iterate through the block searching for the TMsvId.
 *	2.1 If the entry exists, remove it and release to the free pool. Deletion
 *		 is possible only if the entry is locked in the cache, otherwise the function
 * 		 returns KErrAccessDenied.
 * 		 2.1.1 If the entry deleted was the last entry in the block, delete the
 *		        block.
 * 3. Leave with KErrNotFound if the entry does not exist in the block.	
 */
void CMsvCacheIndexTableEntry::DeleteEntryL(TMsvId aId, TMsvId& aParentIdOfEntry /*DEFAULT = NULL*/, TBool aForceDelete /*DEFAULT=EFalse*/)
	{ 
	// 1. Return if the block is NULL.
	if(NULL == iBlockPtr)
		{
		User::Leave(KErrNotFound);
		}
	
	//	2. Iterate through the block searching for the TMsvId.
	TInt blockSize = Size();
	for(TInt index = 0 ; index < blockSize ; ++index)
		{
		//	2.1 If the entry exists, remove it and release to the free pool. Deletion
		//		 is possible only if the entry is locked in the cache.
		if((*iBlockPtr)[index]->GetId() ==  aId)
			{
			if(aForceDelete || (*iBlockPtr)[index]->IsEntryLocked())
				{
				aParentIdOfEntry = (*iBlockPtr)[index]->Entry().Parent();
				CMsvEntryFreePool::Instance()->ReleaseEntryL((*iBlockPtr)[index]);
				iBlockPtr->Remove(index);
				// 2.1.1. If the entry deleted was the last entry in the block,
				//        delete the block.
				if(iBlockPtr->Count() == 0)
					{
					iBlockPtr->Close();
					delete iBlockPtr;
					iBlockPtr = NULL;
					}
				iTimeStamp.HomeTime();
				}
			else
				{
				User::Leave(KErrAccessDenied);
				}
			return;
			}
		} //for
	// 3. Leave with KErrNotFound if the entry does not exist in the block.
	User::Leave(KErrNotFound);
	}


/**
 * ReleaseBlock()
 * @param TBool: internally used flag which is used by destructor to forcibly swap out
 * 				 the block even if entries have been locked.
 * @return TBool: EFalse if block cannot be released as a result of entries having been
 * 				  locked in the cache, ETrue if otherwise.
 * 
 * The function releases a block to the cache free pool.
 * 1. Return EFalse if any of the entries in the block have been locked. Do not
 *    swap out the block to the free pool if so.
 * 2. No entries are locked and/or the block is to be swapped out forcibly.
 *  2.1 Release the entries to the free pool.
 *  2.2 Free the memory allocated to the block itself.
 *  2.3 Set the dirty flag.
 */
TBool CMsvCacheIndexTableEntry::ReleaseBlock(TBool aForceRelease /*DEFAULT = EFalse*/)
	{ 
	if(NULL == iBlockPtr)
		{
		return ETrue;
		}
	// 1. Return EFalse if any of the entries in the block have been locked. Do not
	//	  swap out the block to the free pool if so.
	if(!aForceRelease)
		{
		TInt blockSize = Size();
		for(TInt index = 0; index < blockSize; ++index)
			{
			if(!(*iBlockPtr)[index]->IsEntrySwappable())
				{
				return EFalse;
				}
			}
		}
	
	// 2. No entries are locked and the block can be swapped out safely.
	CMsvEntryFreePool::Instance()->ReleaseEntrySet(*iBlockPtr);
	
	iBlockPtr->Close();
	delete iBlockPtr;
	iBlockPtr = NULL;
	SetDirty();
	return ETrue;
	}


/**
 * CompareOrder()
 * @param const CMsvCacheEntry& : the first operand
 * @param const CMsvCacheEntry& : the second operand
 * @return TInt : the order (-1/0/+1)
 * 
 * The function returns the order for the sort operation.
 */
static TInt CompareOrder(const CMsvCacheEntry& aFirst, const CMsvCacheEntry& aSecond)
	{
	return aFirst.GetId() - aSecond.GetId();
	}


/**
 * SortBlockL()
 * @param None.
 * @return None.
 * 
 * The function is called by a background active object to sort the index table
 * according to TMsvIds of the entries.
 * 1. Sort based on the order in the TLinearOrder package.
 */
void CMsvCacheIndexTableEntry::SortBlock()
	{
	if(iBlockPtr != NULL)
		{
		//	1. Sort based on the order in the TLinearOrder package.
		TLinearOrder<CMsvCacheEntry> order(CompareOrder);
		iBlockPtr->Sort(order);
		}
	}


/**
 * EntryExists()
 * @param TMsvId : the Id of the entry to be checked for existence.
 * @return TBool : success/failure.
 * 
 * The function return EFalse if either the block is NULL or the entry does not exist.
 * ETrue is returned for a hit.
 */
TBool CMsvCacheIndexTableEntry::EntryExists(TMsvId aId) const
 	{
 	if(NULL == iBlockPtr)
		{
		return EFalse;
		}
 	TInt blockSize = Size();
 	for(TInt index = 0 ; index < blockSize ; ++index)
 		{
 		if(aId == (*iBlockPtr)[index]->GetId())
 			{
 			return ETrue;
 			}
 		} //for
 	return EFalse;
 	}


/**
 * UpdateChildMsvIdsL()
 * @param TMsvId : TMsvId of the parent entry.
 * @param TMsvId : TMsvId of the child entry.
 * @param TBool : flag, set to ETrue if aChildId is to be added, and EFalse if it is
 * 				  to be removed from the parent's child array.
 * @return None.
 * 
 * The function updates the child array of the parent entry if it is not NULL.
 * 1. Return if the block is NULL, i.e. the parent is not in the cache.
 * 2. Iterate through the block searching for the parent entry.
 * 	2.1 If the iChildIdArray is not NULL:
 * 		2.1.1 If the child has to be added as a consequence of it being
 *				  added to the cache, then append it to the child array.
 * 		2.1.2 If the child has to be removed as a consequence to the child
 * 		       entry being deleted, then remove it from the child array.
 */
void CMsvCacheIndexTableEntry::UpdateChildMsvIdsL(TMsvId aParentId, TMsvId aChildId, TBool aAdd /*DEFAULT = ETrue*/)
	{
	// 1. Return if the block is NULL, i.e. the parent is not in the cache.
	if(NULL == iBlockPtr)
		{
		return;
		}
	
	//	2. Iterate through the block searching for the parent entry.
	TInt blockSize = Size();
	for(TInt index = 0 ; index < blockSize ; ++index)
		{
		if((*iBlockPtr)[index]->GetId() ==  aParentId)
			{
			RArray<TMsvId>* childArr = (*iBlockPtr)[index]->ChildIdArray();
			// 2.1 If the iChildIdArray is not NULL: i.e GetChildren has been performed
			if(childArr != NULL)
				{
				// 2.1.1 If the child has to be added as a consequence of it being
				//		  added to the cache, then append it to the child array.
				TInt arrSize = childArr->Count();
				if(aAdd)
					{
					TInt index = childArr->Find(aChildId);
					if(index == KErrNotFound)
						{
						childArr->AppendL(aChildId);
						}
					}
				
				// 2.1.2 If the child has to be removed as a consequence to the child
				//        entry being deleted, then remove it from the child array.
				else
					{
					TInt index = childArr->Find(aChildId);
					if(index != KErrNotFound)
						{
						childArr->Remove(index);
						}
					}
				} //if(((*iBlockPtr)[index]->ChildIdArray()) != NULL)
			return;
			} //if((*iBlockPtr)[index]->GetId() ==  aParentId)
		} //for(TInt index = 0 ; index < Size() ; index++)
}


/**
 * UpdateChildMsvIdsL()
 * @param RPointerArray<CMsvCacheEntry>& : array of complete set of child 
 * 										   CMsvCacheEntries sorted by TMsvId.
 * @return None.
 * 
 * The function updates, or creates if necessary, a parent entry's iChildIdArray with
 * the children entries passed to it in the form of a RPointerArray.
 * 1. Return if the block is NULL, i.e. the parent is not in the cache.
 * 2. Otherwise, find the parent sequentially.
 * 	2.1 If the parent is found:
 * 		2.1.1 If the parent already has a iChildIdArray, i.e. if GetChildren() has
 * 			   been performed on it, delete and create a new child array with the 
 * 			   entries passed. If iChildIdArray is NULL then GetChildren() has not been
 *			   performed on the entry yet, in which case append the passed children.
 */
void CMsvCacheIndexTableEntry::UpdateChildMsvIdsL(RPointerArray<CMsvCacheEntry>& aEntries)
	{
	
	// 1. Return if the block is NULL, i.e. the parent is not in the cache.
	if(NULL == iBlockPtr)
		{
		return;
		}
	
	// 2. Otherwise, find the parent sequentially.
	TMsvId parentId = aEntries[0]->Entry().Parent();
	TInt blockSize = Size();
	for(TInt index = 0 ; index < blockSize ; ++index)
		{
		// 2.1 If the parent is found:
		if((*iBlockPtr)[index]->Entry().Id() == parentId)
			{
			// 2.1.1 If the parent already has a iChildIdArray, i.e. if GetChildren() has
			// been performed on it, delete and create a new child array with the 
			// entries passed. If iChildIdArray is NULL then GetChildren() has not been
			// performed on the entry yet, in which case append the passed children.
			RArray<TMsvId>* childArr = (*iBlockPtr)[index]->ChildIdArray();
			TInt entryCount = aEntries.Count();
			if(NULL == childArr)
				{
				childArr = new(ELeave) RArray<TMsvId>;
				(*iBlockPtr)[index]->SetChildIdArray(childArr);
				}
			else
				{
				childArr->Reset();
				}
			while(0 < entryCount)
				{
				TInt leave = childArr->Append(aEntries[--entryCount]->GetId());
				if(KErrNone != leave)
					{
					childArr->Close();
					delete childArr;
					(*iBlockPtr)[index]->SetChildIdArray(NULL);
					User::Leave(leave);
					}
				}
			return;
			} //if((*iBlockPtr)[index]->Entry().Parent() == parentId)
		} //for(TInt index = 0 ; index < blockSize ; ++index)
	}
	
	
/**
 *SplitBlock
 *This will be called when the block size will be more than 120, this will split the block
 * into 2 blocks.	
 *@param aSplitBlock 	RPointerArray<CMsvCacheEntry>& that will be filled with CMsvCacheEntries,
 *@param aMaxId			The maximum TMsvId of the first block.		 
 *@return void
 *	
 */
void CMsvCacheIndexTableEntry::SplitBlockL(RPointerArray<CMsvCacheEntry>& aSplitBlock)
	{
	TInt size = Size();
	TInt i = size/2;
	while(i < size)
		{
		aSplitBlock.AppendL((*iBlockPtr)[size/2]);
		iBlockPtr->Remove(size/2);
		i++;
		}
	}





#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
#ifdef _DEBUG
// To print message index cache.
void CMsvCacheIndexTableEntry::Print(RFileLogger& aLogger)
	{
	_LIT(KId, "       ID: ");
	_LIT(KParentId, "    ParentId: ");
	TInt blockSize = Size();
	for(TInt index=0; index<blockSize; ++index)
		{
		RBuf text;
		text.CreateL(150);
		text.Append(KId);
		text.AppendNum((*iBlockPtr)[index]->Entry().Id());
		text.Append(KParentId);
		text.AppendNum((*iBlockPtr)[index]->Entry().Parent());
		text.Append(_L("    VisibleEntry: "));
		text.AppendNum((*iBlockPtr)[index]->Entry().VisibleFolderFlag());
		text.Append(_L("	Complete flag:"));
		text.AppendNum((*iBlockPtr)[index]->Entry().Complete());
		text.Append(_L("    	Description:"));
		text.Append((*iBlockPtr)[index]->Entry().iDetails);
		aLogger.Write(text);
		text.Close();
		}
	}
#endif 		// #ifdef _DEBUG
#endif		// #if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
