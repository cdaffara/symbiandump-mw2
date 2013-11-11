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
// MACRO DEFINITION
// 
//
  
#ifndef __MSVENTRYFREEPOOL_H__
#define __MSVENTRYFREEPOOL_H__


/**
 * HEADER FILES
 */ 
#include <e32base.h>
#include "msvcacheentry.h"
#include "msvcachevisiblefolder.h"

#if !defined(MSVSEARCHSORTCACHEMANAGER_H_)
#include "msvsearchsortcachemanager.h"
#endif


class CMsvIndexAdapter;


/**
 * The class provides implementation for pool of unused CMsvCacheEntry.
 * It is a singleton class. A CMsvIndexAdapter instance should create and
 * delete the instance of this class. It reads a msgcache.ini file which
 * stores amount of CMsvCacheEntry to be created at startup. The file 
 * can be configured by licencees.
 * The destructor is made private so that only friend function can delete 
 * the instance of this class. Moreover static function Destroy() should 
 * be called to delete the instance of the class.
 *
 @internalComponent
 @prototype
 */

NONSHARABLE_CLASS (CMsvEntryFreePool): public CBase
{
public:
	static CMsvEntryFreePool* Instance();	
	CMsvCacheEntry* EntryL();
	void ReleaseEntryL(CMsvCacheEntry* aMsvCacheEntry, TBool aBypassTransaction = EFalse);
	void ReleaseEntrySet(RPointerArray<CMsvCacheEntry>& aMsvCacheEntryArray);
	void DoReleaseEntrySetL(RPointerArray<CMsvCacheEntry>& aMsvCacheEntryArray);
	void SetEssentialParam(TDblQue<CMsvCacheVisibleFolder>* aEntryCache, CMsvIndexAdapter* aAdapterObj);
	inline void BeginTransaction();
	void RollbackTransaction();
	void CommitTransaction();
	inline TInt ExcessMemoryAllocated();
	inline void RecordExcessMemoryL(CMsvCacheEntry* aEntry);
	void FlushExcessMemory();
	void RoutineFreePoolCleanUpL();
	void ReleaseEntryWithoutTransaction(CMsvCacheEntry* aMsvCacheEntry);
	
#ifdef SYMBIAN_MESSAGESTORE_UNIT_TESTCODE
	inline TInt GetMaximumCacheSize();
	inline TInt GetGranularity();
	inline TInt GetCacheThreshold();
	inline TInt GetCacheNumberOfEntriesCreated();
	inline TInt GetNumberOfEntriesPresentInFreePool();
	inline TInt GetMaxEntries();
	inline TInt GetUsedEntriesCount();
#endif
	
	
private:
	static CMsvEntryFreePool* CreateL(TDblQue<CMsvCacheVisibleFolder>* aEntryCache = NULL);
	CMsvEntryFreePool(TDblQue<CMsvCacheVisibleFolder>* aEntryCache);
	CMsvEntryFreePool(const CMsvEntryFreePool&);
	~CMsvEntryFreePool();
	void ConstructL();
	void SwapEntriesL(TInt aEntriesToSwap);
	TInt ConvertMemToEntry(TInt aMem);
	TBool IsAllocationRequiredL();
	void AllocateMemoryL();
	TInt CacheSizeL();
	
#ifdef _DEBUG
	static void Destroy(CMsvEntryFreePool* aFreePoolInstance);
#endif

private:
	static CMsvEntryFreePool* iMsvEntryFreePool;
	RPointerArray<CMsvCacheEntry>* iMsvEntries;
	RPointerArray<CMsvCacheEntry> iUsedCacheEntries;
	
	// Reference to catch object.
	// Needed for entry swapping.
	TDblQue<CMsvCacheVisibleFolder>* iEntryCache;
	CMsvIndexAdapter* iAdapter;
	
	// Needed for transaction.
	RPointerArray<CMsvCacheEntry> iMsvTmpEntries;
	TBool isTransactionOpen;
	
	TInt iMsvMaximumCacheSize;
	//TInt iMsvGranularity;
	TInt iMsvCacheThreshold;
	TInt iMsvCacheNumberOfEntriesCreated;
	
	TInt iMsvInitialCacheSize;
	TInt iMsvCacheIncrement;
	TInt iMsvPercentageAllocated;
	TInt iMsvCacheSize;
	TInt iMsvSearchSortCache;
	TInt iMsvMaxEntries;
		
	/**
	 * FRIEND CLASSES:
	 * These classes can access destructor and Destroy().
	 */
	friend class CMsvIndexAdapter;
	friend class CMsvServer;
	friend class CMSvSearchSortCacheManager;

#ifdef SYMBIAN_MESSAGESTORE_UNIT_TESTCODE
	TBool iReleaseError;
	TInt iReleaseErrorOffset;
	friend class CTestEntryFreePool;	
	friend class CTestDbAdapter;
	friend class CTestIndexTableEntry;
	friend class CTestVisibleFolder;
#endif
};





/**
 ***********  INLINE FUNCTIONS   ************
 */

void CMsvEntryFreePool::BeginTransaction()
	{
	isTransactionOpen = ETrue;
	}


TInt CMsvEntryFreePool::ExcessMemoryAllocated()
	{
	if (iMsvMaxEntries < iMsvCacheNumberOfEntriesCreated)
		{
		return (iMsvCacheNumberOfEntriesCreated - iMsvMaxEntries);	
		}
	return NULL;	
	}
	
	
/**
 * RecordExcessMemoryL()
 */
void CMsvEntryFreePool::RecordExcessMemoryL(CMsvCacheEntry* aEntry)
	{	
	if(aEntry)
		{
		iUsedCacheEntries.AppendL(aEntry);
		}
	}
	
	
	
#ifdef SYMBIAN_MESSAGESTORE_UNIT_TESTCODE 
TInt CMsvEntryFreePool::GetMaximumCacheSize()
	{
	return iMsvMaximumCacheSize;
	}


TInt CMsvEntryFreePool::GetCacheThreshold()
	{
	return iMsvCacheThreshold;
	}


TInt CMsvEntryFreePool::GetCacheNumberOfEntriesCreated()
	{
	return iMsvCacheNumberOfEntriesCreated;
	}


TInt CMsvEntryFreePool::GetNumberOfEntriesPresentInFreePool()
	{
	return iMsvEntries->Count();
	}
	
TInt CMsvEntryFreePool::GetMaxEntries()
	{
	return 	iMsvMaxEntries;
	}
	
TInt CMsvEntryFreePool::GetUsedEntriesCount()
	{
	return iUsedCacheEntries.Count();
	}

#endif



#endif
