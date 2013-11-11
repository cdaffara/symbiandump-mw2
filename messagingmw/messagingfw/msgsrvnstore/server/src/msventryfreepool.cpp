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
// HEADER FILES
// 
//

#include "msvcachevisiblefolder.h"
#include "msvcacheindextableentry.h"
#include "msvindexadapter.h"
#include "msventryfreepool.h"
#include "msvinifile.h"
#include "MSVPANIC.H"


/**
 * MACRO DEFINITION
 */
#define BLOCK_SIZE 64
#define PERCENTAGE_VALUE 100
#define DEFAULT_MAXIMUM_CACHE_SIZE 1024
#define DEFAULT_INITIAL_CACHE_SIZE_IN_PERCENTAGE 40
#define DEFAULT_CACHE_INCREMENT_SIZE_IN_PERCENTAGE 20
#define DEFAULT_CACHE_THRESHOLD_IN_PERCENTAGE 70
#define CACHE_THRESHOLD_AFTER_FULL_CACHE 80
#define PERCENTAGE_TO_SWAP_OUT 20

/**
 * LITERAL DEFINITION
 */
//Ini file which contains the cache setting
#ifdef SYMBIAN_MESSAGESTORE_UNIT_TESTCODE
	_LIT(KMSGINI, "C:\\private\\1000484b\\msgcache.ini");
#else
	_LIT(KMSGINI, "Z:\\private\\1000484b\\msgcache.ini");
#endif
	

//Ini keywords used for cache setting
_LIT(KIniMsvInitialCacheCreation, "MsvInitialCacheCreation");
_LIT(KIniMsvMaximumCacheSize, "MsvMaximumCacheSize");
_LIT(KIniMsvCacheIncrement, "MsvCacheIncrement");
_LIT(KIniMsvCacheThreshold, "MsvCacheThreshold");
_LIT(KIniMsvSearchSortCache, "MsvSearchSortCachePercentage");


/**
 * STATIC MEMBER FUNCTION DEFINITION
 */
 
CMsvEntryFreePool* CMsvEntryFreePool::iMsvEntryFreePool = NULL;



/**
 * FUNCTION DEFINITION
 */
 
 
 
/**
 * CMsvEntryFreePool()
 *
 * Constructor is kept private, since the class
 * is singleton.
 */
CMsvEntryFreePool::CMsvEntryFreePool(TDblQue<CMsvCacheVisibleFolder>* aEntryCache):iEntryCache(aEntryCache)
	{
	}


/**
 * ~CMsvEntryFreePool()
 *
 * Default Destructor. 
 * The function should delete CMsvCacheEntry
 * created so far. The destructor is made 
 * private so that only friend class can call it.
 */
 
CMsvEntryFreePool::~CMsvEntryFreePool()
	{
	if(iMsvTmpEntries.Count())
		{
		CommitTransaction();
		}
	if(iUsedCacheEntries.Count())
		{
		iUsedCacheEntries.ResetAndDestroy();
		}
	iUsedCacheEntries.Close();
	if(iMsvEntries)
		{
		iMsvEntries->ResetAndDestroy();	
		iMsvEntries->Close();	
		}
	delete iMsvEntries;
	iMsvEntries = NULL;
	iMsvTmpEntries.Close();
	iMsvEntryFreePool = NULL;
	}



/**
 * Instance()
 *
 * The function returns already created instance of
 * this class to the caller. To create a new instance
 * the caller should call CreateL(). If an instance of
 * this object does not already exists, the function 
 * throws a panic EMsvFreePoolNotCreated in DEBUG mode.
 */ 
CMsvEntryFreePool* CMsvEntryFreePool::Instance()
	{
	__ASSERT_DEBUG(iMsvEntryFreePool!=NULL, PanicServer(EMsvFreePoolNotCreated));
	return iMsvEntryFreePool;
	}
	
	

/**
 * CreateL()
 *
 * The only way to create an object of this class.
 * The function ensures that only one instance of
 * the object is created. This is a static interface.
 * The function is made private to ensure that only
 * friend function can create instance of this class.
 */ 
CMsvEntryFreePool* CMsvEntryFreePool::CreateL(TDblQue<CMsvCacheVisibleFolder>* aEntryCache /*DEFAULT=NULL*/)
	{
	CMsvEntryFreePool* self = new (ELeave) CMsvEntryFreePool(aEntryCache);
	//Push object in to cleanup stack so that it 
	// will be handles properly incase of a leave.
	CleanupStack::PushL(self);

	self->ConstructL();

	//Pop from cleanupstack
	CleanupStack::Pop(self);
	iMsvEntryFreePool = self;
	
	return self;
	}




/**
 * ConstructL()
 *
 * The function is called from InstanceL() and used to
 * initialize the instance of this class. It firsts reads
 * cache configuration information from msgcache.ini file
 * and stores them into member variable. It then allocates
 * creates initial set of entries as described in conf file. 
 */
void CMsvEntryFreePool::ConstructL()
	{
	// 1. Reading ini file
	
	// If drive information is not mentioned in the 
	// filename, by default the class will check in
	// C: drive. If the file is not found in C: it 
	// will check in Z: drive as well.
	CMsvIniData* iniFile = NULL;
	TRAPD(err, iniFile = CMsvIniData::NewL(KMSGINI));
	
	// If we find the config file, read it. Else assign default values to cache configurations.
	if(!err && iniFile)
		{
		CleanupStack::PushL(iniFile);

		// Reading maximum cache size from ini file.
		// Initially, iMsvMaximumCacheSize will have cache size in KB which
		// will later be converted to number of cache entries possible.
		User::LeaveIfError(iniFile->FindVar(KIniMsvMaximumCacheSize, iMsvMaximumCacheSize));

		// Reading initial cache size from ini file.
		// This is represented in terms of percentage 
		// of maximum cache size.
		User::LeaveIfError(iniFile->FindVar(KIniMsvInitialCacheCreation, iMsvInitialCacheSize));

		// Reading cache increment value from ini file.
		// This is represented in terms of percentage 
		// of maximum cache size.
		User::LeaveIfError(iniFile->FindVar(KIniMsvCacheIncrement, iMsvCacheIncrement));

		// Reading threshold value from ini file
		// This is represented in terms of percentage
		// of maximum cache size. 
		User::LeaveIfError(iniFile->FindVar(KIniMsvCacheThreshold, iMsvCacheThreshold));

		// Reading Search Sort Cache value in percentage from ini file
		// This is represented in terms of percentage
		// of maximum cache size.
		User::LeaveIfError(iniFile->FindVar(KIniMsvSearchSortCache, iMsvSearchSortCache));

		CleanupStack::PopAndDestroy(iniFile);
		}

	// Performing consistency check.
	if(NULL == iMsvMaximumCacheSize)
		{
		iMsvMaximumCacheSize = DEFAULT_MAXIMUM_CACHE_SIZE;
		}
	if((iMsvInitialCacheSize > PERCENTAGE_VALUE) || (NULL == iMsvInitialCacheSize))
		{
		iMsvInitialCacheSize = DEFAULT_INITIAL_CACHE_SIZE_IN_PERCENTAGE;
		}
	if((iMsvCacheIncrement > PERCENTAGE_VALUE) || (NULL == iMsvCacheIncrement))
		{
		iMsvCacheIncrement = DEFAULT_CACHE_INCREMENT_SIZE_IN_PERCENTAGE;
		}
	if((iMsvCacheThreshold > PERCENTAGE_VALUE) || (NULL == iMsvCacheThreshold))
		{
		iMsvCacheThreshold = DEFAULT_CACHE_THRESHOLD_IN_PERCENTAGE;
		}
	
	//Determine maximum number of cache entries possible.
	iMsvMaxEntries = ConvertMemToEntry(iMsvMaximumCacheSize * 1024);
	
	// Approximate number of entries to be created when server starts.
	TInt initialNumberOfEntries = (iMsvInitialCacheSize * iMsvMaxEntries) / 100;

	// Since allocation is done in specific increment size, it is best
	// to give the granularity as the increment count (not percentage here).
	iMsvEntries = new(ELeave) RPointerArray<CMsvCacheEntry>((iMsvCacheIncrement * iMsvMaxEntries) / 100);

	// Create the initial number of cache entries.
	// iMsvCacheNumberOfEntriesCreated will store
	// number of CMsvCacheEntry created so far.
	for (iMsvCacheNumberOfEntriesCreated = 0; iMsvCacheNumberOfEntriesCreated < initialNumberOfEntries ; iMsvCacheNumberOfEntriesCreated++)
		{
		iMsvEntries->AppendL(CMsvCacheEntry::NewL());
		}
	
	// Percentage of allocated memory utilized so far.
	iMsvPercentageAllocated = iMsvInitialCacheSize;
	
	}



void CMsvEntryFreePool::SetEssentialParam(TDblQue<CMsvCacheVisibleFolder>* aEntryCache, CMsvIndexAdapter* aAdapterObj)
	{
	iEntryCache = aEntryCache;
	iAdapter = aAdapterObj;
	}



/**
 * ConvertMemToEntry()
 * @param aMem: Memory size in bytes.
 * @return TInt: the number of entries that the free pool can have
 *				 given a memory size in bytes.
 *
 * The function calculates the number of cache entries possible in
 * the free pool given the memory size.
 */
TInt CMsvEntryFreePool::ConvertMemToEntry(TInt aMem)
	{
	//This calculation is based on the fact that not all cache entries will have
	//children as most are leaf nodes. Our assumption is that 20% of the entries
	//will have an RArray, i.e. have children. The following formula determines
	//maximum number of entries that the cache can have as:
	//		sizeof(CMsvCacheEntry) = 120 bytes;
	//		sizeof(RArray<TMsvId>) = 36 bytes;
	//		sizeof(RArray<TMsvId>*) = 4 bytes;
	//	=>	multiplying factor = (80*120 + 20*156)/(120*100) = 1.06
	return ( (aMem) / (1.06*sizeof(CMsvCacheEntry)) );	 
	}



/**
 * Destroy()
 * @param CMsvEntryFreePool: The object to be destroyed.
 *
 * Only way to destroy an object of this class in debug 
 * mode. The functions raises a panic when amount of
 * cache entry created so far is not same as amount of 
 * entry the object currently holds. The function is 
 * declared private so that only friend class can call this.
 *
 * In release mode destructor should be called directly.
 */
#ifdef _DEBUG

void CMsvEntryFreePool::Destroy(CMsvEntryFreePool* aFreePoolInstance)
	{
	if(NULL==aFreePoolInstance)
		return;
	
	if(aFreePoolInstance->iMsvTmpEntries.Count())
		{
		aFreePoolInstance->CommitTransaction();
		}
	
	// Check if the number of entries created so far is same
	// as amount of entry currently present with freepool (iMsvEntries).	
	__ASSERT_DEBUG(aFreePoolInstance->iMsvCacheNumberOfEntriesCreated==aFreePoolInstance->iMsvEntries->Count(), User::Invariant());
	
	delete aFreePoolInstance;
	}
	
#endif


	
/**
 * EntryL()
 * @param None.
 * @return CMsvCacheEntry*: An unused CMsvCacheEntry.
 *
 * Returns one unused entry from the freepool. If
 * freepool is empty, it checks if a new entry can 
 * be allocated and returned. If amount of new entry
 * created equals the maximum allowed limit (as set
 * by the licencees in .ini file), the function calls
 * SwapEntries to swap few entries from cache.
 * If no entries can be allocated or swapped, it
 * leaves with KErrNoMemory.
 */
CMsvCacheEntry* CMsvEntryFreePool::EntryL()
	{
	// Number of entries available in freepool.
	TInt count = iMsvEntries->Count();
	CMsvCacheEntry* entry = NULL;
	
	// Is entry available in freepool?
	if (0 < count)
		{		
		// Just return the last entry.
		entry = (*iMsvEntries)[--count];
		iMsvEntries->Remove(count);
		return entry;
		}
	else
		{
		// Can we still allocate fresh memory?
		if(iMsvMaxEntries > iMsvCacheNumberOfEntriesCreated)
			{
			++iMsvCacheNumberOfEntriesCreated;
			return CMsvCacheEntry::NewL();
			}
		//Otherwise swap out BLOCK_SIZE entries to free up space.
		//Form an entry from this memory and return it.
		else
			{
			SwapEntriesL(BLOCK_SIZE);
			count = iMsvEntries->Count();
			if(count)
				{	
				entry = (*iMsvEntries)[--count];
				iMsvEntries->Remove(count);
				return entry;
				}
			else
				{
				++iMsvCacheNumberOfEntriesCreated;
				return CMsvCacheEntry::NewL();
				}
			}
		}
	}



/**
 * ReleaseEntry()
 * @param CMsvCacheEntry* : Entry to be release to freepool.
 * @return None.
 * 
 * The function resets and appends the passed 
 * entry to freepool so that it can be reused.
 */
void CMsvEntryFreePool::ReleaseEntryL(CMsvCacheEntry* aMsvCacheEntry, TBool aBypassTransaction /* DEFAULT=EFalse*/)
	{
	if( NULL == aMsvCacheEntry )
		{
		return;
		}
	
	if(isTransactionOpen && (EFalse == aBypassTransaction))
		{
		iMsvTmpEntries.AppendL(aMsvCacheEntry);
		}
	else
		{
		// 1. Reset the entry data.
		aMsvCacheEntry->ResetEntry();
	
		// 2. Append entry to the freepool.
		iMsvEntries->AppendL(aMsvCacheEntry);
		}
	}



void CMsvEntryFreePool::ReleaseEntryWithoutTransaction(CMsvCacheEntry* aMsvCacheEntry)
    {
    if(NULL == aMsvCacheEntry)
        {
        return;
        }
   
    aMsvCacheEntry->ResetEntry();
    TRAPD(Err, iMsvEntries->AppendL(aMsvCacheEntry));
    if(Err)
        {
        delete aMsvCacheEntry;
        iMsvCacheNumberOfEntriesCreated--;
        }
    }





/**
 * ReleaseEntrySet()
 * @param RPointerArray<CMsvCacheEntry>* : Entry set to be released.
 * @return None.
 *
 * The function resets and appends the passed set
 * of entries to freepool so that it can be reused.
 */
void CMsvEntryFreePool::ReleaseEntrySet(RPointerArray<CMsvCacheEntry>& aMsvCacheEntryArray)
	{
	TRAPD(error, DoReleaseEntrySetL(aMsvCacheEntryArray));
	if(error)
		{
		// 1. delete the remaining cache entries from aMsvCacheEntryArray
		// 2. reduce appropriate variables in CMsvEntryFreePool
		TInt count = aMsvCacheEntryArray.Count();
		iMsvCacheNumberOfEntriesCreated -= count;
		while(count)
			{
			// 1. Reset the entry data.
			aMsvCacheEntryArray[--count]->ResetEntry();
			
			// 2. Delete the memory allocated to the cache entry.
			delete aMsvCacheEntryArray[count];
			aMsvCacheEntryArray.Remove(count);
			}		
		}
	}


/**
 * DoReleaseEntrySetL()
 * @param RPointerArray<CMsvCacheEntry>* : Entry set to be released.
 * @return None.
 *
 * The function resets and appends the passed set
 * of entries to freepool so that it can be reused.
 */
void CMsvEntryFreePool::DoReleaseEntrySetL(RPointerArray<CMsvCacheEntry>& aMsvCacheEntryArray)
	{
	TInt count = aMsvCacheEntryArray.Count();
	while ( 0 < count)
		{
		if(isTransactionOpen)
			{
			iMsvTmpEntries.AppendL(aMsvCacheEntryArray[--count]);
			}
		else
			{
			// 1. Reset the entry data.
			aMsvCacheEntryArray[--count]->ResetEntry();

#ifdef SYMBIAN_MESSAGESTORE_UNIT_TESTCODE
			if(iReleaseError)
				{
				if(iReleaseErrorOffset == count)
					{
					iReleaseError = EFalse;
					User::Leave(KErrNoMemory); // an arbitrary error purely for testing purpose
					}
				}
#endif
			// 2. Add entry to the freepool.
			iMsvEntries->AppendL(aMsvCacheEntryArray[count]);
			}
		
		// 3. Remove entry from the input array.
		aMsvCacheEntryArray.Remove(count);
		}
	}



/**
 * 
 */
void CMsvEntryFreePool::RollbackTransaction()
	{
	TInt count = iMsvTmpEntries.Count();
	while(0 < count)
		{
		if(NULL != iAdapter)
			{
			TRAP_IGNORE(iAdapter->AddEntryNoVisibleL(iMsvTmpEntries[--count]));
			}
		}
	iMsvTmpEntries.Reset();
	isTransactionOpen = EFalse;
	}


/**
 * 
 */
void CMsvEntryFreePool::CommitTransaction()
	{
	TInt count = iMsvTmpEntries.Count();
	while(0 < count)
		{
		iMsvTmpEntries[--count]->ResetEntry();
		iMsvEntries->Append(iMsvTmpEntries[count]);
		}
	iMsvTmpEntries.Reset();
	isTransactionOpen = EFalse;
	}



/**
 * SwapEntriesL()
 * @param TInt: number of entries to be swapped out.
 * @return None.
 *
 * Swap a specified amount of entries from cache.
 * The function is called by GetEntry() when it
 * cannot return an unused entry since freepool is
 * empty. 
 */
void CMsvEntryFreePool::SwapEntriesL(TInt aEntriesToSwap)
	{
	if(!iEntryCache)
		{
		User::Leave(KErrArgument);
		}
	CMsvCacheVisibleFolder* folderNode = NULL;
	TDblQueIter<CMsvCacheVisibleFolder> dqIter(*iEntryCache);

    dqIter.SetToLast();
    TInt entriesReleased = 0;
    TBool isFolderEmpty;
        
    while(	((folderNode = dqIter--) != NULL) &&  (entriesReleased < aEntriesToSwap) 
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)           
   	//We do not swap out Local service and root folder node.
           &&  (KMsvLocalServiceIndexEntryId) != UnmaskTMsvId(folderNode->GetFolderId())
           &&  (KMsvRootIndexEntryId != folderNode->GetFolderId())
#else 
           &&  (KMsvLocalServiceIndexEntryId) != folderNode->GetFolderId()
           &&  (KMsvRootIndexEntryId != folderNode->GetFolderId())
#endif
	  	 )
        {
        isFolderEmpty = EFalse;
        //Attempt to reclaim aEntriesToSwap from a single folder.
        //The number of entries swapped out can be more than aEntriesToSwap.
       	entriesReleased += folderNode->ReleaseBlocks((aEntriesToSwap - entriesReleased), isFolderEmpty);
		#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)  
			TMsvId folderId = UnmaskTMsvId(folderNode->GetFolderId());
		#else
			TMsvId folderId = folderNode->GetFolderId();
		#endif

		if(isFolderEmpty && !folderNode->GetChildrenFromVisibleFolder() && folderId > KMsvDeletedEntryFolderEntryId )
        	{
        	folderNode->iDlink.Deque();
        	delete folderNode;
        	break;
        	}
        }
    }




/**
 * IsAllocationRequiredL()
 * @param None.
 * @return TBool: ETrue if the free pool has reached its threshold,
 *				  EFalse if otherwise.
 *
 * The function return ETrue if the free pool has reached its 
 * threshold and EFalse if otherwise.
 */
TBool CMsvEntryFreePool::IsAllocationRequiredL()
	{
	// Size of actual cache data structure.
    iMsvCacheSize = CacheSizeL();
    
    // Total cache size includes the size of freepool as well.
    TInt totalCacheSize = iMsvCacheSize + iMsvEntries->Count()*sizeof(CMsvCacheEntry);
    
    //Check if the size of the folder list exceeds or equals the threshold.
    //If all of the cache memory has been used up, checking is done against
    //the secondary threshold value.
    if( PERCENTAGE_VALUE == iMsvPercentageAllocated )
    	{	
    	if(iMsvCacheSize > (totalCacheSize * CACHE_THRESHOLD_AFTER_FULL_CACHE/100))
    		{
    		return ETrue;
    		}
    	else
	    	{
    		return EFalse;
    		}
    	}
    else
    	{
		if(iMsvCacheSize > (totalCacheSize * iMsvCacheThreshold/100))
    		{
    		return ETrue;
    		}
    	else
	    	{
    		return EFalse;
    		}
    	}
	}



/**
 * CacheSizeL()
 * @param None.
 * @return TInt: the size of the cache data structure in bytes.
 *
 * The function returns the size of the cache data structure in bytes, 
 * excluding the memory occupied by the free entries in the freepool.
 */
TInt CMsvEntryFreePool::CacheSizeL()
	{
	if(!iEntryCache)
		{
		User::Leave(KErrArgument);
		}
	CMsvCacheVisibleFolder* folderNode = NULL;
	TDblQueIter<CMsvCacheVisibleFolder> dqIter(*iEntryCache);
 	TInt cacheSize = 0;
 	TInt folderCount = 0;
	
    //Calculate the size of the whole folder list structure.
    //This includes the size required for the data structures,
    // Eg: index table, block of entries etc.
    dqIter.SetToFirst();
    while ((folderNode = dqIter++) != NULL)
        {
        ++folderCount;
    	RPointerArray<CMsvCacheIndexTableEntry> *indexTable = folderNode->GetIndexTableEntry();
    	//Number of blocks in the indextable, i.e. number of indextable entries.
    	TInt count = indexTable->Count();
    	for(TInt index = 0; index < count; ++index)
    		{
    		TInt entryCount = (*indexTable)[index]->Size();
    		cacheSize += entryCount * (sizeof(CMsvCacheEntry) * 1.06);
    		//Include the size of the details and description buffers of a cache entry.
    		//Size of buffers and members of TMsvEntry is the same.
    		//Description and details each have standard size of 15.
    		cacheSize += (2*15) * entryCount;
    		}
    	//Size of data structure for the blocks.
    	cacheSize += count * sizeof(CMsvCacheIndexTableEntry);
    	}
    //Size of the data structure for index tables.
    cacheSize += folderCount * sizeof(CMsvCacheVisibleFolder); //bytes
    return cacheSize;
	}



/**
 * AllocateMemoryL()
 * @param None.
 * @return None.
 *
 * The function expands the cache free pool when it has reached its threshold.
 * If all of the memory has not been used up yet, it allocates the entries as per
 * the next increment. It allocates memory again if threshold is still being exceeded.
 * Swapping might be performed to maintain the threshold.
 * If all of the memory has been used up, it swaps out (100-iMsvCacheThreshold)% entries
 * from cache.
 */
void CMsvEntryFreePool::AllocateMemoryL()
	{
	//Total cache size includes the size of freepool as well.
	TInt totalCacheSize = iMsvCacheSize + iMsvEntries->Count()*sizeof(CMsvCacheEntry);
	
	// Check if entire memory is not yet allocated to cache.
	if(PERCENTAGE_VALUE > iMsvPercentageAllocated)
		{
		//Yes, it has not been used up. Calculate the percentage allocation done till now.
		TInt actualPercentage = (totalCacheSize * 100/1024)/iMsvMaximumCacheSize;
		while(actualPercentage >= iMsvPercentageAllocated && iMsvPercentageAllocated < PERCENTAGE_VALUE)
			{
			iMsvPercentageAllocated += iMsvCacheIncrement;
			}
		if(PERCENTAGE_VALUE < iMsvPercentageAllocated)
			{
			iMsvPercentageAllocated = PERCENTAGE_VALUE;
			}
		
		//Calculate how much memory has to be allocated in terms of cache entries.
		TInt memoryDiff = iMsvMaximumCacheSize*1024*(iMsvPercentageAllocated - actualPercentage)/100;
		if(memoryDiff > 0)
			{
			TInt entriesToCreate = ConvertMemToEntry(memoryDiff); 
			for(TInt index = 0; index < entriesToCreate; ++index)
				{
				iMsvEntries->AppendL(CMsvCacheEntry::NewL());
				++iMsvCacheNumberOfEntriesCreated;
				}	
			}
		//If the cache data structure size still exceeds the threshold go for 
		//the next increment allocation.
		totalCacheSize = iMsvCacheSize + iMsvEntries->Count()*sizeof(CMsvCacheEntry);
		if( (totalCacheSize * iMsvCacheThreshold)/100 < iMsvCacheSize )
			{
			//If all of the memory has been used up now, swap out entries.
			if( PERCENTAGE_VALUE == iMsvPercentageAllocated )
				{
				SwapEntriesL(PERCENTAGE_TO_SWAP_OUT*iMsvMaxEntries/100);					
				}
			//Otherwise calculate the number of entries in the increment and allocate.
			else
				{
				TInt previousPercent = iMsvPercentageAllocated;
				iMsvPercentageAllocated += iMsvCacheIncrement;
				if(PERCENTAGE_VALUE < iMsvPercentageAllocated)
					{
					iMsvPercentageAllocated = PERCENTAGE_VALUE;
					}
				memoryDiff = iMsvMaximumCacheSize*1024*(iMsvPercentageAllocated - previousPercent)/100;
				TInt entriesToCreate = ConvertMemToEntry(memoryDiff); 
				for(TInt index = 0; index < entriesToCreate; ++index)
					{
					iMsvEntries->AppendL(CMsvCacheEntry::NewL());
					++iMsvCacheNumberOfEntriesCreated;
					}		
				}
			}		
		}	
	else
		{
		//All of the cache memory has been used. Go for swapping out of entries from the 
		//cache data structure to free up memory.
		SwapEntriesL(PERCENTAGE_TO_SWAP_OUT*iMsvMaxEntries/100);
		}	
		
	//Calculate the number of entries that can be formed from the remaining memory.
	if( PERCENTAGE_VALUE == iMsvPercentageAllocated )
		{
		iMsvMaxEntries = iMsvCacheNumberOfEntriesCreated;
		}
	else
		{
		iMsvMaxEntries = iMsvCacheNumberOfEntriesCreated + ConvertMemToEntry((iMsvMaximumCacheSize * 1024) * (100-iMsvPercentageAllocated)/100);
		}
	}
	


/**
 * FlushExcessMemory()
 */
void CMsvEntryFreePool::FlushExcessMemory()
	{
	TInt count = iUsedCacheEntries.Count();
	if(count)
		{
		CMsvCacheEntry *entry = NULL;
		while(count)
			{
			entry = iUsedCacheEntries[0];
			iUsedCacheEntries.Remove(0);
			if(iMsvMaxEntries < iMsvCacheNumberOfEntriesCreated)
				{
				delete entry;
				iMsvCacheNumberOfEntriesCreated--;
				}
			else
				{
				entry->ResetEntry();
				iMsvEntries->Append(entry);
				}
			--count;
			}
		iUsedCacheEntries.Reset();
		}
	}
	
	
	
void CMsvEntryFreePool::RoutineFreePoolCleanUpL()
	{
	FlushExcessMemory();
	TInt excessMemory = ExcessMemoryAllocated();
	if(excessMemory)
		{
		SwapEntriesL(excessMemory);
		}	
	}
