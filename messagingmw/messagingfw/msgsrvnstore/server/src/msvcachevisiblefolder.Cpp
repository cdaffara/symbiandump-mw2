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
// CMSVCACHEVISIBLEFOLDER.CPP
// 
//

#include "msvcachevisiblefolder.h"
#include "msventryfreepool.h"
#include "msvcacheindextableentry.h"
#include "msvdbadapter.h"
#include "msvcacheentry.h"

#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)	
	#include "msvindexadapter.h"
#endif

#define BLOCK_SIZE 64
#define BLOCK_THRESHOLD 16

/**
Literal Definition
*/

_LIT8(KId, "id ");
_LIT8(KBetween, "between ");
_LIT8(KAnd, " and ");
_LIT8(KOr, " or ");
_LIT8(KIn, " in ");
_LIT8(KRtBrace, ") ");
_LIT8(KLtBrace, "( ");
_LIT8(KComma, ", ");
_LIT8(KSemiColon, ";");
_LIT8(KOrder, " order by id ASC");



/**
CMsvCacheVisibleFolder
*/
const TInt CMsvCacheVisibleFolder::iOffset = _FOFF(CMsvCacheVisibleFolder, iDlink);


/**
 NewL()
@param TMsvId: Visible Folder TMsvId.
@return The newly created Visible Folder.
 
It returns an instance of CMsvCacheVisibleFolder class. 
*/
CMsvCacheVisibleFolder* CMsvCacheVisibleFolder::NewL(TMsvId aId)
	{
	CMsvCacheVisibleFolder *self = new(ELeave) CMsvCacheVisibleFolder(aId);
	return self;
	}


/**
 NewL()
@param 	TMsvId: Visible Folder TMsvId.
@param 	RPointerArray<CMsvCacheEntry>: Rpointer Reference to children Entries
@return The newly created Visible Folder.
 		It returns an instance of CMsvCacheVisibleFolder class. 
*/
CMsvCacheVisibleFolder* CMsvCacheVisibleFolder::NewL(TMsvId aId, RPointerArray<CMsvCacheEntry>& aEntries)
	{
	CMsvCacheVisibleFolder *self = new(ELeave) CMsvCacheVisibleFolder(aId);
	CleanupStack::PushL(self);
	self->ConstructL(aEntries);
	CleanupStack::Pop();
	return self;
	}


/**
 ~CMsvCacheVisibleFolder()
@param 	None.
@return None.
 		Destructor for the CMsvCacheVisibleFolder class.
*/
CMsvCacheVisibleFolder::~CMsvCacheVisibleFolder()
	{
	iIndexTable.ResetAndDestroy();
	iIndexTable.Close();
	}


/**
 CMsvCacheVisibleFolder()
@param 	None.
@return None.
 		Constructor for the CMsvCacheVisibleFolder class. 
*/
CMsvCacheVisibleFolder::CMsvCacheVisibleFolder(TMsvId aId)
:iFlags(EMsvCacheVisibleFolderClearFlag)
	{
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	// Keep standard id unmasked.
	if(IsStandardId(aId))
		{
		iVisibleFolderId = UnmaskTMsvId(aId);
		}
	else
		{
		iVisibleFolderId = aId;
		}
	// Store drive id information.
	iDriveId = GetDriveId(aId);
#else
	iVisibleFolderId = aId;
#endif
	}


/**
 ConstructL()
@param 	RPointerArray<CMsvCacheEntry>& aEntries.
@return None.
 		Two Phase constructor for adding the entries passed during the creation
 		of the CMsvCacheVisibleFolder object. 
*/
void CMsvCacheVisibleFolder::ConstructL(RPointerArray<CMsvCacheEntry>& aEntries)
	{
	AddEntryListL(aEntries, ETrue);
	}


/**
 AddEntryL()
@param CMsvCacheEntry&: CMsvCacheEntry Reference.
@return none.

The function Adds an Entry into the cache block
01. check if the parent id of the entry exists; Incase if it is a child of a non-visible folder
	we cannot add it unless the parent is present
02. Check if The Parent Id of the Entry is same as the visible folder id.
02.1. If yes, Set the Flag to Update the ChildMsvId array of the parent.
03. If there is no Index Table created, Create a new index table entry
	and add the entry into the table
04. Iterate the Indextable entries and add the entry in the appropriate 
	IndexTable entry Block and update the child msvid array of the parent
	If the entry is not an immediate child.
05. In case index table is not yet created, then there is a possibility that entry Id does not fall in
 the first block range but it can be added before the first block. In such case, add the entry in the first
 block itself and update the first block range.
06. If the Entry doesnt fall in the range of any of the blocks add the Entry
	in the last block and update the child MsvId array if the entry is not an immediate child.
*/
void CMsvCacheVisibleFolder::AddEntryL(CMsvCacheEntry*& aEntry, TBool aReplace /* DEFAULT = EFalse */)
	{
	TBool updateChild=EFalse;
	TInt noOfIndexTableEntries  = iIndexTable.Count();
	TMsvId entryId = aEntry->Entry().Id();
	TMsvId parentId = aEntry->Entry().Parent();
	
	
	// 02. Check if The Parent Id of the Entry is same as the visible folder id.
	if(parentId != iVisibleFolderId)
		{
		// 02.1. If yes, Set the Flag to Update the ChildMsvId array of the parent.
		updateChild = ETrue;
		}
	
	// 03. If there is no Index Table created, Create a new index table entry
	//		and add the entry into the table
	if(NULL == noOfIndexTableEntries )
		{
		CMsvCacheIndexTableEntry* tableEntry = CMsvCacheIndexTableEntry::NewLC(aEntry);
		tableEntry->SetMinMsvIdRange(aEntry->GetId());
		tableEntry->SetMaxMsvIdRange(aEntry->GetId());
		if(aEntry->Entry().Parent() != iVisibleFolderId)
			{
			tableEntry->SetGrandChildPresent();
			}
		iIndexTable.AppendL(tableEntry);
		CleanupStack::Pop();		
		return;
		}
	
	// 04. Iterate the Indextable entries and add the entry in the appropriate 
	// 		IndexTable entry Block and update the child msvid array of the parent
	//		If the entry is not an immediate child.
	for(TInt index=0; index < noOfIndexTableEntries; ++index)
		{
	 	if(iIndexTable[index]->IsInRange(entryId))
	 		{
	 		iIndexTable[index]->AddEntryL(aEntry, aReplace);
	 		if(aEntry->Entry().Parent() != iVisibleFolderId)
				{
				iIndexTable[index]->SetGrandChildPresent();
				}	
	 		if(updateChild)
	 			{
	 			UpdateChildMsvIdsL(parentId, entryId);
	 			}
	 		return;
	 		}
		//05. In case index table is not yet created, then there is a possibility that entry Id does not fall in
		// the first block range but it can be added before the first block. In such case, add the entry in the first
		// block itself and update the first block range.
		if((NULL == index) && (entryId < iIndexTable[index]->GetMinMsvIdRange()))
			{
	 		iIndexTable[index]->AddEntryL(aEntry, aReplace);
	 		if(aEntry->Entry().Parent() != iVisibleFolderId)
				{
				iIndexTable[index]->SetGrandChildPresent();
				}	
	 		if(updateChild)
	 			{
	 			UpdateChildMsvIdsL(parentId, entryId);
	 			}
	 		return;
			}
		}
		
	// 06. If the Entry doesnt fall in the range of any of the blocks add the Entry
	// 		in the last block and update the child MsvId array if the entry is not an immediate child.
	iIndexTable[noOfIndexTableEntries-1]->AddEntryL(aEntry, aReplace);
	if(aEntry->Entry().Parent() != iVisibleFolderId)
		{
		iIndexTable[noOfIndexTableEntries-1]->SetGrandChildPresent();
		}
	if(entryId > iIndexTable[noOfIndexTableEntries-1]->GetMaxMsvIdRange())
		{
		iIndexTable[noOfIndexTableEntries-1]->SetMaxMsvIdRange(entryId);
		}
	if(updateChild)
		{
		UpdateChildMsvIdsL(parentId, entryId);
		}
	}


/**
 AddEntryListL()
@param RPointerArray<CMsvCacheEntry>&: Entries to be added under this visible folder which are in sorted order.
@param TBool: Set to true if the passed array of CMsvCacheEntry contains the complete childrens of VisibleFolder
@return none.

The function Adds a set of Entries into the cache
1. If entry list is NULL, return to the caller.
2.check if the IndexTable is not created then,
2.1 Yes, create the required number of index tables and append the entries in the appropriate blocks
2.2 No, Check if getchildren has already been performed on this VisibleFolder
2.2.1 Yes Assuming the input list is already sorted, Add the entries in the appropriate
 blocks depending on the range of each blocks
2.2.1.1 The remaining entries should be put in the last block
2.3 No, check if getchildren has'nt been performed on this VisibleFolder
2.3.1 Check If index table exists
2.3.1.1 Yes,Create a Copy of the existing index table entries
2.3.1.2 create the required number of index tables and append the entries in the appropriate blocks
2.3.1.3 check the copy of the previous list if any entry exists
2.3.1.3 copy back only the locked entries or entires
 which are not immediate childrens of the visible folder or
 entries which are locked and release the remaining entries
2.3.2 If indextable is not present add the entries in the appropriate blocks 
*/
void CMsvCacheVisibleFolder::AddEntryListL(RPointerArray<CMsvCacheEntry>& aEntries, TBool aIsCompleteChildOfFolder /*DEFAULT = EFalse */)
	{	
	//1. If entry list is NULL, return to the caller.
	TInt size = aEntries.Count();
	if(size <= 0)
		{
		if(!size && aIsCompleteChildOfFolder)
			{
			SetComplete(ETrue);
			}
		return;
		}

	TBool isGrandChildrenAdded = EFalse;
	if(aEntries[0]->Entry().Parent() != iVisibleFolderId)
		{
		isGrandChildrenAdded = ETrue;
		}
		
	
	//2.check if the IndexTable is not created then,
	if(0 == iIndexTable.Count())
		{
		//2.1 Yes, create the required number of index tables and append the entries in the appropriate blocks
		SplitAndAppendL(aEntries);
		}
	else
		{
		//2.2 No, Check if getchildren has already been performed on this VisibleFolder
		if(IsComplete())		
			{
			TInt tmpIndex = 0, index = 0;
			//2.2.1 Yes Assuming the input list is already sorted, Add the entries in the appropriate
			// blocks depending on the range of each blocks
			for(TInt tableIndex = 0; tableIndex < iIndexTable.Count(); ++tableIndex)
				{
				while((tmpIndex < size) && (iIndexTable[tableIndex]->GetMaxMsvIdRange() >= aEntries[tmpIndex]->GetId()))
					{
					++tmpIndex;
					}
				if(index != tmpIndex)
					{
					// No need to set Max/Min range, as IsComplete() is true.
					iIndexTable[tableIndex]->AddEntrySetL(aEntries, index, tmpIndex-index);
					if(isGrandChildrenAdded)
						{
						iIndexTable[tableIndex]->SetGrandChildPresent();
						}
					}	
				
				if(aIsCompleteChildOfFolder)
					{
					iIndexTable[tableIndex]->ClearDirty();
					}
				
				if( tmpIndex >= size )
					{
					break;
					}
				index = tmpIndex;
				
				}
			
			//2.2.1.1 The remaining entries should be put in the last block
			tmpIndex = size - tmpIndex;		// Amount of entries remaining.
			if(tmpIndex > BLOCK_THRESHOLD)
				{				
				SplitAndAppendL(aEntries, index);
				}
			else if(0 != tmpIndex)	
				{
				TMsvId maxId = aEntries[size-1]->GetId();
				iIndexTable[iIndexTable.Count()-1]->AddEntrySetL(aEntries, index, tmpIndex);
				if(isGrandChildrenAdded)
					{
					iIndexTable[iIndexTable.Count()-1]->SetGrandChildPresent();
					}
				iIndexTable[iIndexTable.Count()-1]->SetMaxMsvIdRange(maxId);
				}
			}
		//2.3 No, check if getchildren has'nt been performed on this VisibleFolder
		else
			{
			//2.3.1 Check If index table exists
			if(aIsCompleteChildOfFolder)
				{
				// 2.3.1.1 Yes,Create a Copy of the existing index table entries
				RPointerArray<CMsvCacheIndexTableEntry> tmpTable;
				CleanupClosePushL(tmpTable);
				for(TInt index=0; index<iIndexTable.Count(); index++)
					{
					tmpTable.AppendL(iIndexTable[index]);
					}
				iIndexTable.Reset();
				// 2.3.1.2 create the required number of index tables and append the entries in the appropriate blocks
				SplitAndAppendL(aEntries);
				// 2.3.1.3 check the copy of the previous list if any entry exists
				while(tmpTable.Count())					
					{
					if(tmpTable[0]->BlockPtr() != NULL)
						{
						RPointerArray<CMsvCacheEntry>* blockPtr = tmpTable[0]->BlockPtr();
						TInt count = blockPtr->Count();
						while(count--)
							{
							// 2.3.1.3 copy back only the locked entries or entires
							//which are not immediate childrens of the visible folder or
							//entries which are locked and release the remaining entries
							if( ((*blockPtr)[count]->Entry().Parent() != iVisibleFolderId) || !((*blockPtr)[count]->IsEntrySwappable()) )
								{
								AddEntryL((*blockPtr)[count], ETrue);
								}
							else
								{
								CMsvEntryFreePool::Instance()->ReleaseEntryL((*blockPtr)[count]);
								}
							}
						blockPtr->Reset();
						blockPtr->Close();
						}
					delete tmpTable[0];
					tmpTable.Remove(0);
					}
				tmpTable.Reset();	
				CleanupStack::PopAndDestroy();
				}
			//2.3.2 If indextable is not present add the entries in the appropriate blocks 
			else	
				{
				TInt size = aEntries.Count();
				while(size--)
					{
					AddEntryL(aEntries[size]);
					}
				}	
			}
		}
	
	if(aIsCompleteChildOfFolder)
		{
		SetComplete(ETrue);
		}
	}


/**
 SplitAndAppendL()
@param RPointerArray<CMsvCacheEntry>&: Entries to be added under this visible folder which are in sorted order.
@param TInt: Maximum range of the last block under this visible folder
@return none.

The function Adds a set Entries into the cache
1. Calculate the number of blocks to be created.
2. If remaining entries are less then BLOCK_THRESHOLD they will be appended to last block.
3. Create new indextable entries and then add the child entries into the appropriate blocks
4. If few entries are still left, append them to the last block.
*/
void CMsvCacheVisibleFolder::SplitAndAppendL(RPointerArray<CMsvCacheEntry>& aEntryList, TInt aInitIndex /*(DEFAULT=0)*/)
	{
	//1. Calculate the number of blocks to be created.
	TInt entriesToBeAdded = aEntryList.Count() - aInitIndex;
	TInt blocksToCreate = entriesToBeAdded /(TInt) BLOCK_SIZE;
	TInt aLastMaxRange = -1;
	TBool isGrandChildrenAdded = EFalse;

	TInt blocksCreated = iIndexTable.Count();
	if(blocksCreated != 0)
		{
		aLastMaxRange = iIndexTable[blocksCreated-1]->GetMaxMsvIdRange();
		}
	if( aEntryList[aInitIndex]->Entry().Parent() != iVisibleFolderId)
		{
		isGrandChildrenAdded = ETrue;
		}
	
	//2. If remaining entries are less then BLOCK_THRESHOLD they will be appended to last block.
	if( (BLOCK_THRESHOLD < (entriesToBeAdded % BLOCK_SIZE)) || 
		( (NULL == (blocksCreated+blocksToCreate)) && (entriesToBeAdded <= BLOCK_THRESHOLD) )
	  )
		{
		++ blocksToCreate;
		}

	TInt blockSize = entriesToBeAdded/blocksToCreate;

	//3. Create new indextable entries and then add the child entries into the appropriate blocks
	for(TInt index = 0; index < blocksToCreate; ++index)
		{
		CMsvCacheIndexTableEntry* newTableEntry = CMsvCacheIndexTableEntry::NewLC(aEntryList, aInitIndex, blockSize);
		if(isGrandChildrenAdded)
			{
			newTableEntry->SetGrandChildPresent();
			}
		TMsvId maxRange = aEntryList[aInitIndex+blockSize-1]->GetId();
		newTableEntry->SetMinMsvIdRange(++aLastMaxRange);
		newTableEntry->SetMaxMsvIdRange(maxRange);
		iIndexTable.AppendL(newTableEntry);
		aLastMaxRange = maxRange;
		aInitIndex += blockSize;
		
		CleanupStack::Pop();
		}

	//4. If few entries are still left, append them to the last block.
	entriesToBeAdded = aEntryList.Count() - aInitIndex;
	if(entriesToBeAdded > 0)	
		{
		CMsvCacheIndexTableEntry* newTableEntry = iIndexTable[iIndexTable.Count()-1];
		newTableEntry->AddEntrySetL(aEntryList, aInitIndex, entriesToBeAdded);
		if(isGrandChildrenAdded)
			{
			newTableEntry->SetGrandChildPresent();
			}
		if(newTableEntry->GetMaxMsvIdRange() < aEntryList[aEntryList.Count()-1]->GetId())
			{
			newTableEntry->SetMaxMsvIdRange(aEntryList[aEntryList.Count()-1]->GetId());
			}
		}
	}



/**
 GetEntry()
@param TMsvId: TMsvId for the Entry to be fetched
@param CMsvCacheEntry&: CMsvCacheEntry Reference.
@return TBool: Cache Hit or Cache Miss.
 
The function Gets an Entry from the cache block
1. Find the IndexTable entry where the Range of the TMsvId falls.
2. Get the Entry in the Block to which it belongs
*/
TBool CMsvCacheVisibleFolder::GetEntry(TMsvId aId, CMsvCacheEntry*& aEntry)
	{
	// 1. Find the IndexTable entry where the Range of the TMsvId falls.
	for(TInt index=0; index < iIndexTable.Count(); ++index)
		{
		if(iIndexTable[index]->IsInRange(aId))
	 		{
	 		//2. Get the Entry in the Block to which it belongs
			return(iIndexTable[index]->GetEntry(aId, aEntry));
	 		}
	 	}
	return EFalse;
	}


/**
 GetChildren()
@param TMsvId: TMsvId of the Parent Entry whose children are to be fetched.
@param RPointerArray<const CMsvCacheEntry>&: Array to fill childrens
@return TBool: Cache Hit or Cache Miss.
 
The function Gets the childrens of the parent from the cache
1. Check if the Parent Id is same as the Visible Folder
1.1 yes, check if getchildren has'nt already been performed on this visible folder entry
	or all the blocks present are dirty.
1.1.1 Then Fetch and fill all the childrens then return the same to the caller
1.2 yes, check if getchildren has beeb performed on this visible folder
1.2.1 yes, only few blocks are dirty, so fetch and fill the dirty blocks
2. If the if the Parent Id is not same as the Visible Folder
2.1 check if the parent of the children is present
2.2 If parent is a visible folder
2.2.1 Yes, then return Efalse to the caller stating that
the parent is a visible folder
2.3 Check if Getchildren has'nt been performed on the parent
2.3.1 Yes, Fetch and fill the childrens of the Parent from DB
2.4 Check If GetChildren has been already performed
2.4.1 If childrens are available in the cache get them
2.4.2 else fetch the children from DB add them to cache
*/
TBool CMsvCacheVisibleFolder::GetChildrenL(TMsvId aId, CMsvDBAdapter* aDbAdapter, RPointerArray<CMsvCacheEntry>& aEntries)
	{
	 SetGetChildrenFromVisibleFolder(ETrue);
	//1. Check if the Parent Id is same as the Visible Folder
	TMsvId id = aId;
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	if(IsStandardId(aId))
		{
		id = UnmaskTMsvId(aId);
		}
#endif
	if(id == iVisibleFolderId)
		{
		//1.1 yes, check if getchildren has'nt already been performed on this visible folder entry
		//	or all the blocks present are dirty.
		if( (!IsComplete()) || (IsComplete() && IsAllBlocksDirty()) )
			{
			//1.1.1 Then Fetch and fill all the childrens then return the same to the caller
			aDbAdapter->GetChildrenL(id, aEntries);
			TInt excessEntries = CMsvEntryFreePool::Instance()->ExcessMemoryAllocated();
			if(excessEntries)
				{
				TInt index = 0;
				RPointerArray<CMsvCacheEntry> entryList;
				CleanupClosePushL(entryList);				
				for(; index<excessEntries; index++)
					{
					CMsvEntryFreePool::Instance()->RecordExcessMemoryL(aEntries[index]);					
					}
				for(; index<aEntries.Count(); index++)
					{
					entryList.AppendL(aEntries[index]);
					}
				AddEntryListL(entryList, EFalse);
				CleanupStack::PopAndDestroy();
				}
			else
				{
				AddEntryListL(aEntries, ETrue);
				}
			SetGetChildrenFromVisibleFolder(EFalse);
			return ETrue;
			}
		//1.2 yes, check if getchildren has beeb performed on this visible folder
		if(IsComplete())
			{
			//1.2.1 yes, only few blocks are dirty, so fetch and fill the dirty blocks
			RBuf8 buf;
			buf.CreateL(2000);
			CleanupClosePushL(buf);
			TBool isDBOperationReqd = EFalse;
			for(TInt index=0; index < iIndexTable.Count(); ++index)
				{
				if(iIndexTable[index]->IsDirty())
					{
					if(!isDBOperationReqd)
						{
						buf.Append(KAnd);		
						buf.Append(KLtBrace);
						isDBOperationReqd = ETrue;
						}
					else
						{
						buf.Append(KOr);	
						}
					buf.Append(KId);
					buf.Append(KBetween);
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
					buf.AppendNum(UnmaskTMsvId(iIndexTable[index]->GetMinMsvIdRange()));
					buf.Append(KAnd);
					buf.AppendNum(UnmaskTMsvId(iIndexTable[index]->GetMaxMsvIdRange()));
#else
					buf.AppendNum(iIndexTable[index]->GetMinMsvIdRange());
					buf.Append(KAnd);
					buf.AppendNum(iIndexTable[index]->GetMaxMsvIdRange());
#endif	// #if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
					}
				}
			if(isDBOperationReqd)	
				{
				buf.Append(KRtBrace);
				buf.Append(KOrder);
				buf.Append(KSemiColon);
				RPointerArray<CMsvCacheEntry> childEntries;
				CleanupClosePushL(childEntries);
				aDbAdapter->GetChildrenL(buf, id, childEntries);
				AddEntryListL(childEntries);
				CleanupStack::PopAndDestroy();
				}
			for(TInt index=0; index < iIndexTable.Count(); ++index)
				{
				iIndexTable[index]->GetChildrenL(id, aEntries);
				iIndexTable[index]->ClearDirty();
				}
			CleanupStack::PopAndDestroy(); 
			SetGetChildrenFromVisibleFolder(EFalse);
			return ETrue;
			}
		}
	//2. If the if the Parent Id is not same as the Visible Folder
	else 
		{
		CMsvCacheEntry* parentEntry;
		//2.1 check if the parent of the children is present
		if(!GetEntry(id, parentEntry))
			{
			User::Leave(KErrNotFound);
			}
		
		//2.2 If parent is a visible folder
		if(parentEntry->Entry().VisibleFolderFlag())
			{
			//2.2.1 Yes, then return Efalse to the caller stating that
			//	the parent is a visible folder
			SetGetChildrenFromVisibleFolder(EFalse);
			return EFalse;
			}
		
		//2.3 Check if Getchildren hasn't been performed on the parent
		if(NULL == parentEntry->ChildIdArray())
			{
			//2.3.1 Yes, Fetch and fill the childrens of the Parent from DB
			aDbAdapter->GetChildrenL(id, aEntries);
			TInt excessEntries = CMsvEntryFreePool::Instance()->ExcessMemoryAllocated();
			if(excessEntries)
				{
				TInt index = 0;
				RPointerArray<CMsvCacheEntry> entryList;
				CleanupClosePushL(entryList);				
				for(; index<excessEntries; index++)
					{
					CMsvEntryFreePool::Instance()->RecordExcessMemoryL(aEntries[index]);					
					}
				for(; index<aEntries.Count(); index++)
					{
					entryList.AppendL(aEntries[index]);
					}
				AddEntryListL(entryList);
				CleanupStack::PopAndDestroy();
				}
			else
				{
				AddEntryListL(aEntries);
				}
			RArray<TMsvId>* childIds = new(ELeave) RArray<TMsvId>;
			CleanupStack::PushL(childIds);
			CleanupClosePushL(*childIds);
			for(TInt index=0; index < aEntries.Count(); ++index)
				{
				childIds->AppendL(aEntries[index]->GetId());
				}
			parentEntry->SetChildIdArray(childIds);
			CleanupStack::Pop(2);
			}
		// 2.4 Check If GetChildren has been already performed
		else
			{
			RBuf8 buf;
			buf.CreateL(2000);
			CleanupClosePushL(buf);
			CMsvCacheEntry* childEntry;
			TBool isDBOperationReqd = EFalse;
			for(TInt index=0; index< parentEntry->ChildIdArray()->Count(); ++index)
				{
				//2.4.1 If childrens are available in the cache get them
				if(GetEntry((*parentEntry->ChildIdArray())[index], childEntry))
					{
					aEntries.AppendL(childEntry);
					}
				//2.4.2 else fetch the children from DB add them to cache
				else
					{
					if(isDBOperationReqd)
						{
						buf.Append(KComma);	
						}
					if(!isDBOperationReqd)
						{
						buf.Append(KAnd);		
						buf.Append(KId);
						buf.Append(KIn);
						buf.Append(KLtBrace);						
						isDBOperationReqd = ETrue;
						}
					#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
						buf.AppendNum(UnmaskTMsvId((*parentEntry->ChildIdArray())[index]));
					#else
						buf.AppendNum((*parentEntry->ChildIdArray())[index]);
					#endif 	
					}
				}
			if(isDBOperationReqd)
				{
				buf.Append(KRtBrace);
				buf.Append(KSemiColon);
				
				RPointerArray<CMsvCacheEntry> childEntries;
				CleanupClosePushL(childEntries);
				aDbAdapter->GetChildrenL(buf, parentEntry->GetId(), childEntries);
				for(TInt index=0; index<childEntries.Count(); ++index)
					{
					aEntries.AppendL(childEntries[index]);
					}
				AddEntryListL(childEntries);	
				CleanupStack::PopAndDestroy(); 
				}
			CleanupStack::PopAndDestroy(); 
			}
		}
		SetGetChildrenFromVisibleFolder(EFalse);
		return ETrue;
	}


/**
 * GetChildrenIdL()
 * @param CMsvEntrySelection& (OUT): List of child TMsvIds of the current visible
 * 									 folder.
 * @return TBool: Returns ETrue, if children list can be returned, otherwise EFalse.
 * 
 * The function returns list of child TMsvId of the current visible folder.
 * It will return ETrue if it is possible to return all child IDs, otherwise
 * it returns EFalse (In case when not all children of the folder is present
 * in cache i.e. IsComplete() is false. OR in case when some of the blocks 
 * are dirty.).
 */
TBool CMsvCacheVisibleFolder::GetChildrenIdL(CMsvEntrySelection& aSelection)
	{
	if(IsComplete())
		{
		for(TInt index=0; index < iIndexTable.Count(); ++index)
			{
			if(iIndexTable[index]->IsDirty())
				{
				return EFalse;
				}
			}
		for(TInt index=0; index < iIndexTable.Count(); ++index)
			{
			CMsvCacheIndexTableEntry* entry = iIndexTable[index];
			TInt blockSize = entry->Size();
			RPointerArray<CMsvCacheEntry>* blkPtr = entry->BlockPtr();
			// 1. Check if there are grandchildren present in the block.
			if(entry->IsGrandChildPresent())
				{
				// 1.1 If yes, then fetch only those entries with parent Id as aParentId.
				//       If the array of entries is NULL, then allocate space for it.
				for(TInt index1 = 0 ; index1 < blockSize ; ++index1)
					{
					if((*blkPtr)[index1]->Entry().Parent() == iVisibleFolderId)
						{
						aSelection.AppendL((*blkPtr)[index1]->GetId());
						}
					}
				} //if(IsGrandChildPresent())
			else
				{
				// 1.2 If not, then return a copy of the whole block.
				for(TInt index1 = 0 ; index1 < blockSize ; ++index1)
					{
					aSelection.AppendL((*blkPtr)[index1]->GetId());
					}
				} //else
			}
		return ETrue;
		}
	else
		{
		return EFalse;
		}
	}



/**
 DeleteEntry()
@param TMsvId: TMsvId of the Entry to be deleted.
@param aForceDelete TBool, it indicates whether the entry needs to be locked while deleting.
@return TInt: Error Status.
 
The function deletes an Entry from the Cache.
1. Iterate through the IndexTable Entries.
2. Find the IndexTable where the entry is present
3. Delete the Entry and update the ChildMsvId array of the parent if delete successful
4. If the its the last Entry in the IndexTable and the complete flag is False
	delete the IndexTable entry
*/
void CMsvCacheVisibleFolder::DeleteEntryL(TMsvId aId, TBool aForceDelete)
	{
	TMsvId parentIdOfEntry;
	
	// 1. Iterate through the IndexTable Entries.
	for(TInt index=0; index < iIndexTable.Count(); ++index)
		{
		// 2. Find the IndexTable where the entry is present
		if(iIndexTable[index]->IsInRange(aId))
			{
			// 3. Delete the Entry and update the ChildMsvId array of the parent if delete successful
			iIndexTable[index]->DeleteEntryL(aId, parentIdOfEntry, aForceDelete);
			if(parentIdOfEntry != iVisibleFolderId)
				{
				UpdateChildMsvIdsL(parentIdOfEntry, aId, EFalse);
				}
			
			// 4. If the its the last Entry in the IndexTable and the complete flag is False
			//		delete the IndexTable entry
			if(NULL == iIndexTable[index]->Size() )
				{
				if((index+1) < iIndexTable.Count())
					{
					iIndexTable[index+1]->SetMinMsvIdRange(iIndexTable[index]->GetMinMsvIdRange());
					}
				delete iIndexTable[index];
				iIndexTable.Remove(index);
				}
			return;
			}
		}
	User::Leave(KErrNotFound);
	}


/**
 DeleteEntryList()
@param CMsvEntrySelection& aEntrySelection.
@return TInt: Error Status.
 
The function deletes a set of Entries from the Cache.
*/
void CMsvCacheVisibleFolder::DeleteEntryListL(CMsvEntrySelection& aEntrySelection)
	{
	TInt count = aEntrySelection.Count();
	for(TInt index=0; index < count; ++index)
		{
		DeleteEntryL(aEntrySelection.At(index));
		}
	}


/**
 IsAllBlocksDirty()
@param None.
@return TBool: Is All the IndexTableEntries dirty or not.
 
The function checks whether all the IndexTable Entry are dirty.
1. Iterate through all the IndexTable Entries.
2. Check if the IndexTable Entry is Not Dirty.
2.1 Yes, Then Return EFalse; stating that all IndexTable Entries are not dirty.
2.2 No, Then Return ETrue; stating that all IndexTable Entries are dirty.
*/
TBool CMsvCacheVisibleFolder::IsAllBlocksDirty() const
	{
	// 1. If index table size is zero, return EFalse.
	if(0 == iIndexTable.Count())
		{
		return (EFalse);
		}	
	
	// 2. Iterate through all the IndexTable Entries.
	for(TInt index=0; index <  iIndexTable.Count(); ++index)
		{
		// 3. Check if the IndexTable Entry is Not Dirty.
		if(!(iIndexTable[index]->IsDirty()))
			{
			// 3.1 Yes, Then Return EFalse; stating that all IndexTable Entries are not dirty.
			return(EFalse);
			}
		}
	// 3.2 No, Then Return ETrue; stating that all IndexTable Entries are dirty.
	return(ETrue);
	}


/**
 EntryExists()
@param TMsvId: TMsvId of the Entry.
@return TBool: Entry Exists or not.
 
The function checks whether an Entry with the Specified TMsvId Exists in the cache.
1. Check if the TMsvId is same as the visible folder id.
1.1. If they are same then return ETrue stating that id is present
2. Else Check through all the IndexTableEntries and check if the Entry Exists.
3. Else Return Entry is not Present.
*/
TBool CMsvCacheVisibleFolder::EntryExists(TMsvId aId) const
	{
	// 1. Check if the TMsvId is same as the visible folder id.
 	if(aId == iVisibleFolderId)
 		{
 		// 1.1. If they are same then return ETrue stating that id is present
		return(ETrue);
		}
 	
 	// 2. Else Check through all the IndexTableEntries and check if the Entry
 	//		Exists.
 	for(TInt index=0; index < iIndexTable.Count(); ++index)
 		{
 		if(iIndexTable[index]->EntryExists(aId))
 			{
 			return(ETrue);
 			}
 		}
 	
 	// 3. Else Return Entry is not Present.
 	return(EFalse);
   	}


/**
 UpdateChildMsvIdsL()
@param TMsvId  aParentId :  parent Id whose MsvId Array has to be updated.
@param TMsvId AChildId : Child Id which as to be updated in the MsvId Array.
@param TBool aAppend : Whether the Entry has to be appended or removed from the Parent MsvId Array.
@return none.
 
The function Updates the Owner with its child TMsvIds.
1. Iterate through the index Table entri
2.Find the indexTable where the entry is present
3. Update the Parent with the child TMsvIds
*/
void CMsvCacheVisibleFolder::UpdateChildMsvIdsL(TMsvId aParentId, TMsvId aChildId, TBool aAppend /* DEFAULT = ETrue */)
	{
	// 1. Iterate through the index Table entries
	for(TInt index = 0; index < iIndexTable.Count(); ++index)
		{
		// 2.Find the indexTable where the entry is present
		if(iIndexTable[index]->IsInRange(aParentId))
			{
			// 3. Update the Parent with the child TMsvIds
			iIndexTable[index]->UpdateChildMsvIdsL(aParentId, aChildId, aAppend);
			return;
			}
		}
	}


/**
 UpdateChildMsvIdsL()
@param RPointerArray<CMsvCacheEntry>&: CMsvCacheEntry Pointer Array.
@return none.
 
The function Updates the Owner with its child TMsvIds.
1. Find the IndexTableEntry where the parent is present.
2. Update the Parent with the child TMsvIds
*/
void CMsvCacheVisibleFolder::UpdateChildMsvIdsL(RPointerArray<CMsvCacheEntry>& aEntries)
	{
	// 1. Find the IndexTableEntry where the parent is present.
	for(TInt index=0; index < iIndexTable.Count(); ++index)
		{
		if(iIndexTable[index]->IsInRange(aEntries[0]->Entry().Parent()))
			{
			// 2. Update the Parent with the child TMsvIds
			iIndexTable[index]->UpdateChildMsvIdsL(aEntries);
			return;
			}
		}
	}


/**
 ReleaseAllBlocks()
@param TInt : Number Entries Successfully released.
@return TBool : Returns true if all the entries are released successfully.
 
The function Releases the blocks owned by the all indextable entries to freepool.
1. Iterate through the indextable entries.
2. Release the IndexTable Entry Block to the free pool
*/
TBool CMsvCacheVisibleFolder::ReleaseAllBlocks(TInt& aCount)
	{
	TBool isAllBlocksReleased = ETrue;
	aCount = 0;
	
	// 1. Iterate through the indextable entries.
	for(TInt index=0; index < iIndexTable.Count(); ++index)
		{
		TInt size = iIndexTable[index]->Size();
		//2.Release the IndexTable Entry Block to the free pool
		if(!iIndexTable[index]->ReleaseBlock())
			{
			isAllBlocksReleased = EFalse;
			}
		else
			{
			aCount += size;				
			}
		}
	if(!IsComplete() && isAllBlocksReleased)
		{
		iIndexTable.ResetAndDestroy();
		}
	
	return (isAllBlocksReleased);
	}


/**
 CompareOrder()
@param CMsvCacheIndexTableEntry : First indexTable entry (Comparison arg)
@param CMsvCacheIndexTableEntry : Second indexTable entry (Comparison arg)
@return TInt : Returns TTime difference.

Used to sort indexTableEntry.
*/
static TInt CompareOrder(const CMsvCacheIndexTableEntry& aFirst, const CMsvCacheIndexTableEntry& aSecond)
	{
	return (TInt) (aFirst.AccessTime().Int64()-aSecond.AccessTime().Int64());
	}



/**
 ReleaseBlocks()
@param TInt : number of blocks to be released to free pool.
@param TBool : Returns true if the indextable was deleted.
@return TInt : Returns Number of entries successfully released.
 
The function Releases the required number blocks owned by the all indextable entries to freepool.
1. Create a Copy of the IndexTable Entries & sort them w.r.t time
2. Iterate through all the blocks or until the expected number of entries are released
3. Release the entries to free pool
*/
TInt CMsvCacheVisibleFolder::ReleaseBlocks(TInt aNumberOfEntriesToRelease, TBool& aIsFolderEmpty)
	{
	aIsFolderEmpty = EFalse;
	TInt index = 0;
	TInt sizeOfBlock = 0;
	TInt numEntries = aNumberOfEntriesToRelease;
	TInt addrIndex = 0;
	
	//1. Create a Copy of the IndexTable Entries & sort them w.r.t time
	RPointerArray<CMsvCacheIndexTableEntry> indexTableCopytoSort;
	for(index=0; index<iIndexTable.Count(); ++index)
		{
		TInt err = indexTableCopytoSort.Append(iIndexTable[index]);
		if(KErrNone != err)
			{
			indexTableCopytoSort.Close();
			return err;
			}
		}
	
	TLinearOrder<CMsvCacheIndexTableEntry> order(CompareOrder);
	indexTableCopytoSort.Sort(order);
	
	index = 0;
	// 2. Iterate through all the blocks or until the expected number of entries are released
	while( (aNumberOfEntriesToRelease > 0) && (index < iIndexTable.Count()))
		{
		// 3. Release the entries to free pool
		sizeOfBlock = indexTableCopytoSort[index]->Size();
		if(indexTableCopytoSort[index]->ReleaseBlock())
			{
			aNumberOfEntriesToRelease -= sizeOfBlock;
			if(!IsComplete())
				{
				addrIndex = iIndexTable.Find(indexTableCopytoSort[index]);
				if((addrIndex+1) < iIndexTable.Count())
					{
					iIndexTable[addrIndex+1]->SetMinMsvIdRange(iIndexTable[addrIndex]->GetMinMsvIdRange());
					}
				delete iIndexTable[addrIndex];
				iIndexTable.Remove(addrIndex);
				indexTableCopytoSort.Remove(index);
				--index;
				}
			}
		++index;
		}
	
	if(!IsComplete() && 0 == iIndexTable.Count())
		{
		aIsFolderEmpty = ETrue;
		}
	indexTableCopytoSort.Close();
	return (numEntries - aNumberOfEntriesToRelease);
	}



/**
 *SplitBlock
 *Will check if the block size will be more than 120, then call the IndexTable SplitBlock()
 * 	
 *@param None.
 *@return void 
 */
void CMsvCacheVisibleFolder::SplitBlockL()
	{
	TInt tableCount = iIndexTable.Count(); 
	while(tableCount--)
		{
		TInt size = iIndexTable[tableCount]->Size();
		if(size > 120)
			{
			iIndexTable[tableCount]->SortBlock();
			RPointerArray<CMsvCacheEntry> splitEntryBlock;
			CleanupClosePushL(splitEntryBlock);
			// Split the block and get the enteries of the 2nd block				
			iIndexTable[tableCount]->SplitBlockL(splitEntryBlock);
			TInt count = splitEntryBlock.Count();
			
			CMsvCacheIndexTableEntry* newTableEntry = CMsvCacheIndexTableEntry::NewLC(splitEntryBlock, 0, count);
			
			TMsvId minId = splitEntryBlock[0]->GetId();
			// Copy the flags(dirty and grandchildren) of the parent block.
			if(iIndexTable[tableCount]->IsDirty())
				{
				newTableEntry->SetDirty();
				}
			if(iIndexTable[tableCount]->IsGrandChildPresent())
				{
				newTableEntry->SetGrandChildPresent();
				}
			newTableEntry->SetAccessTime(iIndexTable[tableCount]->AccessTime());
			
			iIndexTable.InsertL(newTableEntry, tableCount+1);
			iIndexTable[tableCount+1]->SetMinMsvIdRange(minId);
			iIndexTable[tableCount+1]->SetMaxMsvIdRange(splitEntryBlock[count-1]->GetId());
			
			iIndexTable[tableCount]->SetMaxMsvIdRange(minId - 1);
			CleanupStack::Pop();//newTableEntry
			splitEntryBlock.Reset();
			CleanupStack::PopAndDestroy();// splitEntryBlock
			}
		}
	}



/**
 * GetIndexTableEntry()
 * @param TInt : Index at which the desired indextable entry exists.
 * @return CMsvCacheIndexTableEntry* : Returns pointer at the index specified.
 * 
 * The function returns the pointer to the IndexTable from the list.
 */
RPointerArray<CMsvCacheIndexTableEntry>* CMsvCacheVisibleFolder::GetIndexTableEntry()
	{
	return &iIndexTable;
	}




#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
#ifdef _DEBUG
// To print message index cache.
void CMsvCacheVisibleFolder::Print(RFileLogger& aLogger)
	{
	_LIT8(KBlock, "     BLOCK-ID: ");
	TInt blockCount = iIndexTable.Count(); 
	
	for(TInt index=0; index < blockCount; index++)
		{
		RBuf8 text;
		text.CreateL(100);
		text.Append(KBlock);
		text.AppendNum(index+1);
		aLogger.Write(text);
		text.Close();	
		iIndexTable[index]->Print(aLogger);	
		aLogger.Write(_L(""));
		}
	}
#endif 		// #ifdef _DEBUG
#endif		// #if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
	
