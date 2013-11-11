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
//

#ifdef _DEBUG
#undef _NO_SERVER_LOGGING_
#endif

/**
 * User Includes
 */
#include "MSVSERV.H"
#include "MSVARRAY.H"
#include "msvdbadapter.h"
#include "msvcacheentry.h"
#include "msvindexadapter.h"
#include "msventryfreepool.h"
#include "msvcachevisiblefolder.h"
#include "msvsearchsortdeltacache.h"
#include "msvsearchsortcachemanager.h"

/**
 *System Includes
 */
#include <msvuids.h>
#include <msvstd.h>

#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS 
#include "msvconsts.h"
#endif

/**
 * MACRO DEFINITIONS
 */
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	#define INITIAL_NUM_ENTRIES 10
#else
	#define INITIAL_NUM_ENTRIES 13
#endif

/**
 * CONSTANT DEFINITIONS
 */
const TInt KMsvMtmListGranularity=8;
const TInt KMsvChangeAttributesListGranularity=32;



/**
 * FUNCTION DEFINITIONS
 */



/**
 * CMsvIndexAdapter()
 */
CMsvIndexAdapter::CMsvIndexAdapter(CMsvServer& aServer)
    :CActive(EPriorityStandard), iServer(aServer), iFolderListHeader(CMsvCacheVisibleFolder::iOffset)
	{	
	CActiveScheduler::Add(this);		
	}




/**
 * ~CMsvIndexAdapter()
 */
CMsvIndexAdapter::~CMsvIndexAdapter()
	{

#ifndef _NO_SERVER_LOGGING_
	iServer.Log(_L("Destroying CMsvIndexAdapter object."));
#endif
	
	Cancel();
	
	// 1. Close the database.
#ifndef _NO_SERVER_LOGGING_
	iServer.Log(_L("Closing the database."));
#endif
	if(iDbAdapter != NULL)
		delete iDbAdapter;

	// 2. Delete Cache.
#ifndef _NO_SERVER_LOGGING_
	iServer.Log(_L("Destroying Cache."));
#endif
	
	CMsvCacheVisibleFolder* folderNode = NULL;
	TDblQueIter<CMsvCacheVisibleFolder> dqIter(iFolderListHeader);

    dqIter.SetToFirst();
    while ((folderNode = dqIter++) != NULL)
        {
        folderNode->iDlink.Deque();
        delete folderNode;
        }
    
    iFreePoolInstance->ReleaseEntryL(iRootEntry);
    iRootEntry = NULL;

    // 3. Delete internal data structure.
    iNonCommittedAddedEntryList.ResetAndDestroy();
    iNonCommittedChangedEntryList.Reset();
    iNonCommittedAddedEntryList.Close();
	iNonCommittedChangedEntryList.Close();
    
    // 4. Flush extra memory in entry freepool.
    if(iFreePoolInstance != NULL)
    	{
    	iFreePoolInstance->FlushExcessMemory();	
    	}
	
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	// Added for 557.
	iMaxMsvIdList.Close();
#endif

	delete iIdle;
	}




#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
/**
 * NewL()
 * @return The newly created index adapter object.
 * 
 * Function added as part of PREQ 557.
 * It returns an instance of CMsvIndexAdapter class. 
 */
CMsvIndexAdapter* CMsvIndexAdapter::NewL(CMsvServer& aServer)
	{
	CMsvIndexAdapter *self = new(ELeave) CMsvIndexAdapter(aServer);
	CleanupStack::PushL(self);
	
	self->ConstructL();
	
	CleanupStack::Pop();
	return self;
	}




/**
 * ConstructL()
 * 
 * Function added as part of PREQ 557.
 * The function is called from NewL().
 * 1. Create the DB adapter object.
 * 2. Initialize freepool object.
 * 3. Create the root entry in cache.
 * 4. Browse through the preferred drive list.
 *    i. For each drive in the preferred drive list the function 
 *       assigns a unique drive-id (0< Id <8).
 *	 ii. Attach the DB to the primary database.
 *  iii. Update the maxId of the database to MaxId table.
 *   iv. If the database is not compatible or corrupt, update the 
 *       appropriate drive status in CMsvServer data structure.
 *    v. Fetch children of root entry and Local-Service entry from the DB to the cache.      
 */
void CMsvIndexAdapter::ConstructL()
	{
#ifndef _NO_SERVER_LOGGING_
	iServer.Log(_L("Inside ConstructL()..."));
#endif
	
	// Create database adapter object.
	iDbAdapter = CMsvDBAdapter::NewL();
		
#ifndef _NO_SERVER_LOGGING_
	iServer.Log(_L("The database object is created succesfully."));
#endif
	
	iProgress.iCompleted = 0;
	iProgress.iTotal = iProgress.iRemaining = INITIAL_NUM_ENTRIES;
	
	// Initialize freepool object.
	iFreePoolInstance = CMsvEntryFreePool::Instance();
	iFreePoolInstance->SetEssentialParam(&iFolderListHeader, this);
	
	// Drive id zero will be synonym of KCurrentDriveId. Drive
	// id of a drive (including current) drive will never be zero.
	// Drive of the current drive will always by KCurrentDriveId.
	TUint driveId = KCurrentDriveId;

	// Since driveId starts from 1, the first 
	// entry in iMaxMsvIdList is not used.
	// Enter a dummy entry in location zero.
	iMaxMsvIdList.InsertL(0, 0);
	
	iProgress.iCompleted++;
	iProgress.iRemaining--;
	iProgress.iId = KMsvRootIndexEntryId;
	
	// For each drive in the preferred drive list, perform followings...
	// 1. Check if a valid message store is available.
	// 2. If not, skip the drive.
	// 3. Assign a new drive Id to the drive and
	//    update the entry in preferred drive list.
	// 4. Attach the drive database to the primary DB.
	// 5. Update the max Id list.
	// 6. For current drive, fetch the children of root entry into cache.
	// 7. Fetch children of localService into cache.
	CMsvPreferredDriveList *driveList = CMsvPreferredDriveList::GetDriveList();
	TUint curDriveIndex = driveList->CurrentDriveIndex();
	for(TInt index=curDriveIndex; index<driveList->Count(); index++)
		{
		TMsvPreferredDrive driveEntry;
		driveList->DriveInfoL(index, driveEntry);
		
		if(EMsvMessageStoreAvailableStatus == driveEntry.status)
			{
			// Assign a unique drive-id.
			driveList->UpdateDriveIdL(index, driveId);
			
			// Attach the database.
			TMsvId maxId;
			iDbAdapter->AttachDBL(driveEntry.driveNum, driveId, maxId);
			
			// Insert the max Id in the list.
			iMaxMsvIdList.InsertL( ((maxId >= MaskTMsvId(driveId, KFirstFreeEntryId))? (maxId+1): MaskTMsvId(driveId, KFirstFreeEntryId)), driveId);
			driveId++;
			}
		iProgress.iCompleted++;
		iProgress.iRemaining--;
		}

	iFirstFreeDriveId = (driveId%8)? (driveId%8): 2;

	// Initializing remaining entries in MaxMsvId list.
	for(TInt index=driveId; index<8; index++)
		{
		iMaxMsvIdList.InsertL(NULL, index);
		}
	
	// For current drive fetch children of root entries also.
	RPointerArray<CMsvCacheEntry> childEntryList;
	CleanupClosePushL(childEntryList);	
	// Create root entry node in the cache. A root folder node will never 
	// be swapped out of cache. The children of root entry will be the 
	// children of root entry in the current drive.
	// Root id is masked to set the driveId in the folder node.
	iRootNode = CMsvCacheVisibleFolder::NewL(MaskTMsvId(KCurrentDriveId, KMsvRootIndexEntryId));
	iFolderListHeader.AddFirst(*iRootNode);
	iRootNode->SetComplete(ETrue);
	iDbAdapter->GetChildrenL(KMsvRootIndexEntryId, childEntryList);
	iRootNode->AddEntryListL(childEntryList, ETrue);
	childEntryList.Reset();
	
	// Fetch child of LocalServices.
	CMsvCacheVisibleFolder* localServiceFolder = CMsvCacheVisibleFolder::NewL(MaskTMsvId(KCurrentDriveId, KMsvLocalServiceIndexEntryId));
	iFolderListHeader.AddFirst(*localServiceFolder);
	iDbAdapter->GetChildrenL(KMsvLocalServiceIndexEntryId, childEntryList);
	localServiceFolder->AddEntryListL(childEntryList, ETrue);
	CleanupStack::PopAndDestroy(1);   //childEntryList
		
	// Create root entry.
	iRootEntry = iFreePoolInstance->EntryL(); //taking it from the freepool.
	iRootEntry->Entry().iType = KUidMsvRootEntry;
	iRootEntry->Entry().iDate.UniversalTime();
	iRootEntry->Entry().SetId(KMsvRootIndexEntryId);
	iRootEntry->Entry().SetParent(KErrNotFound);
	iRootEntry->Entry().SetOwner(ETrue);
	iRootEntry->Entry().iSize = 0;
	
	// Update progress
	iProgress.iCompleted = iProgress.iTotal;
	iProgress.iRemaining = 0;
	iProgress.iId = KMsvRootIndexEntryId;

#ifndef _NO_SERVER_LOGGING_
	iServer.Log(_L("Cache initialized succesfully."));
#endif
	
	iIdle = CIdle::NewL(CActive::EPriorityIdle);
	CompleteSelf();
	}



/**
GetInPreparationIds
*/
void CMsvIndexAdapter::GetInPreparationIds(CMsvEntrySelection& aSelection, TUint aDriveId /*DEFAULT = 0 */)
	{
	if(iDbAdapter)
		{
		TRAP_IGNORE(iDbAdapter->GetInPreparationIdL(aSelection, aDriveId));
		}
	}


#else		// #if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)


/**
 * NewL()
 * @param TFileName: DB File Name.
 * @return The newly created index adapter.
 * 
 * It returns an instance of CMsvIndexAdapter class. 
 */
CMsvIndexAdapter* CMsvIndexAdapter::NewL(CMsvServer& aServer, const TFileName& aDbFileName)
	{
	CMsvIndexAdapter *self = new(ELeave) CMsvIndexAdapter(aServer);
	CleanupStack::PushL(self);
	
	self->ConstructNewL(aDbFileName);
	
	CleanupStack::Pop();
	return self;
	}




/**
 * OpenL()
 * @param TFileName: DB File Name.
 * @return The newly created index adapter.
 * 
 * It returns an instance of CMsvIndexAdapter class. 
 */
CMsvIndexAdapter* CMsvIndexAdapter::OpenL(CMsvServer& aServer, const TFileName& aDbFileName)
	{
	CMsvIndexAdapter *self = new(ELeave) CMsvIndexAdapter(aServer);
	CleanupStack::PushL(self);
	
	self->ConstructOpenL(aDbFileName);
	
	CleanupStack::Pop();
	return self;
	}




/**
 * ConstructNewL()
 * 
 * The function is called from NewL().
 * 1. Check if a database with the passed name already exists.
 * 1.1. If yes, open the database.
 * 1.2. If no, create a new database with the same name.
 * 2. Creates entry cache.
 * 3. If a new database is created, create root entry also.
 */
void CMsvIndexAdapter::ConstructNewL(const TFileName& aDbFileName)
	{
#ifndef _NO_SERVER_LOGGING_
	iServer.Log(_L("Creating CMsvIndexAdapter instance..."));
#endif
	
	// Delete the database first, if it already exists.
	CMsvDBAdapter::DeleteDB(aDbFileName);
	
	// Get the instance of free pool.
	iFreePoolInstance = CMsvEntryFreePool::Instance();

	// Create the new database with the same name.
	// If this leaves, nothing can be done and let NewL() leave.
	iDbAdapter = CMsvDBAdapter::NewL(aDbFileName);
#ifndef _NO_SERVER_LOGGING_
	iServer.Log(_L("A new database %s is created..."), &aDbFileName);
#endif
	
	iProgress.iTotal = iProgress.iRemaining = 1; //root
	// Create root entry. 
	CreateInitialSetOfEntriesL();
	

#ifndef _NO_SERVER_LOGGING_
//	iServer.Log(_L("Cache initialized succesfully."));
#endif
	
	iNextCreateId = KFirstFreeEntryId;
	iIdle = CIdle::NewL(CActive::EPriorityIdle);
	CompleteSelf();
	}
		



	
/**
 * ConstructOpenL()
 * 
 * The function is called from NewL().
 * 1. Check if a database with the passed name already exists.
 * 1.1. If yes, open the database.
 * 1.2. If no, create a new database with the same name.
 * 2. Creates entry cache.
 * 3. If a new database is created, it then read message resource file 
 *    and creates entry as mentioned in cache and database.
 */
void CMsvIndexAdapter::ConstructOpenL(const TFileName& aDbFileName)
	{
#ifndef _NO_SERVER_LOGGING_
	iServer.Log(_L("Creating CMsvIndexAdapter instance..."));
#endif
	
	// Get the instance of free pool
	iFreePoolInstance = CMsvEntryFreePool::Instance();

	// Open the database.
	iDbAdapter = CMsvDBAdapter::OpenL(aDbFileName);
	
#ifndef _NO_SERVER_LOGGING_
	iServer.Log(_L("Opening database %s is succesful."), &aDbFileName);
#endif
		
	iProgress.iTotal = iProgress.iRemaining = INITIAL_NUM_ENTRIES;	
	CreateInitialCacheL();

#ifndef _NO_SERVER_LOGGING_
	iServer.Log(_L("Cache initialized succesfully."));
#endif

	TMsvId tmpNextId = 0;
	iDbAdapter->GetMaxTMsvIdL(tmpNextId);
	iNextCreateId = (tmpNextId >= KFirstFreeEntryId)? (tmpNextId+1) : KFirstFreeEntryId;
	
	iIdle = CIdle::NewL(CActive::EPriorityIdle);
	CompleteSelf();
	}
	



/**
 * CreateInitialSetOfEntries()
 * 
 * The function is called from ConstructL(), whenever a new DB is created.
 * The function creates root entry into database and cache.
 */
void CMsvIndexAdapter::CreateInitialSetOfEntriesL()
	{
	
#ifndef _NO_SERVER_LOGGING_
	iServer.Log(_L("Inside CreateInitialSetOfEntriesL(): Creating standard entries for new DB."));
#endif
	
	// 1. Initialize the instance of free pool.
	iFreePoolInstance->SetEssentialParam(&iFolderListHeader, this);
		
	// 2. Creating root entry into database.
	iRootEntry = iFreePoolInstance->EntryL(); //taking it from the freepool.
	iRootEntry->Entry().iType = KUidMsvRootEntry;
	iRootEntry->Entry().iDate.UniversalTime();
	iRootEntry->Entry().SetId(KMsvRootIndexEntryId);
	iRootEntry->Entry().SetParent(KErrNotFound);
	iRootEntry->Entry().iSize = 0;
	
	TMsvId visibleFolderId = KMsvRootIndexEntryId;
	iDbAdapter->CreateEntryL(iRootEntry->Entry(), visibleFolderId);
	
	// 3. Add root entry to cache as well
	CMsvCacheVisibleFolder* rootFolder = CMsvCacheVisibleFolder::NewL(KMsvRootIndexEntryId);
	iFolderListHeader.AddFirst(*rootFolder);
	rootFolder->SetComplete(ETrue);
	
	// Update progress
	iProgress.iCompleted++;
	iProgress.iRemaining--;
	iProgress.iId = KMsvRootIndexEntryId;
	}





/**
 * CreateInitialCache()
 * 
 * The function is called from NewL(), whenever an existing DB is opened.
 * It creates the intial cache. It creates root entry in cache and fetches
 * child of root from DB and push them into cache. It then fetches child
 * entries of LocalServices and push them too into cache. 
 * 
 * >>>> The function can be enhanced to load children of additional folders
 * to cache along with the above standard folders. These additional folders 
 * can be mentioned by licensees in msgs.ini file.
 */
void CMsvIndexAdapter::CreateInitialCacheL()
	{
#ifndef _NO_SERVER_LOGGING_
	iServer.Log(_L("Inside CreateInitialCache(): Initializing Cache."));
#endif

	//1. Initialize the instance of free pool.
	iFreePoolInstance->SetEssentialParam(&iFolderListHeader, this);
	
	// 2. Add root entry to cache.
	CMsvCacheVisibleFolder* rootFolder = CMsvCacheVisibleFolder::NewL(KMsvRootIndexEntryId);
	iFolderListHeader.AddFirst(*rootFolder);
	// 2.1 Update progress
	iProgress.iCompleted++;
	iProgress.iRemaining--;
	iProgress.iId = KMsvRootIndexEntryId;
	
	// 3. Fetch child of root entries from DB.
	RPointerArray<CMsvCacheEntry> childrenOfRoot;
	iDbAdapter->GetChildrenL(KMsvRootIndexEntryId, childrenOfRoot);

	// 3.1 Get the root entry from DB.
	CMsvCacheEntry *rootEntry = NULL;
	TMsvId visibleFolder = NULL;
	iDbAdapter->GetEntryL(KMsvRootIndexEntryId, rootEntry, visibleFolder);
	// 3.2 Copy the TMsvEntry of root from CMsvCacheEntry.
	//Mem::Copy(&iRootEntry->Entry(), &rootEntry->Entry(), sizeof(TMsvEntry));
	iRootEntry = rootEntry;
	// 3.3 Release the cache entry back to the freepool.
	//iFreePoolInstance->ReleaseEntry(rootEntry);	
	
	// 4. Add children to cache.
	rootFolder->AddEntryListL(childrenOfRoot, ETrue);
	// 4.1 Update progress
	iProgress.iCompleted += 4;
	iProgress.iRemaining -= 4;
	iProgress.iId = KMsvLocalServiceIndexEntryId;
	
	// 5. Set the visible folder as complete
	rootFolder->SetComplete(ETrue);
	
	
	// 6. Fetch child of LocalServices
	childrenOfRoot.Reset();
	iDbAdapter->GetChildrenL(KMsvLocalServiceIndexEntryId, childrenOfRoot);
	
	CMsvCacheVisibleFolder* localFolder = CMsvCacheVisibleFolder::NewL(KMsvLocalServiceIndexEntryId);
	iFolderListHeader.AddFirst(*localFolder);
		
	// 7. Add children to cache.
	localFolder->AddEntryListL(childrenOfRoot);
	// 7.1 Update progress
	iProgress.iCompleted = iProgress.iTotal;
	iProgress.iRemaining = 0;
	iProgress.iId = KMsvGlobalInBoxIndexEntryId;
	
	// 8. Set the visible folder as complete
	localFolder->SetComplete(ETrue);
	childrenOfRoot.Close();
	}
	


/**
GetInPreparationIds
*/
void CMsvIndexAdapter::GetInPreparationIds(CMsvEntrySelection& aSelection)
	{
	if(iDbAdapter)
		{
		TRAP_IGNORE(iDbAdapter->GetInPreparationIdL(aSelection));
		}	
	}


#endif 		// #if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)


/**
 *CompleteSelf
 *This will make sure that Runl is called.
 */
void CMsvIndexAdapter::CompleteSelf()
	{
	iStatus=KRequestPending;
	SetActive();
	TRequestStatus* st=&iStatus;
	User::RequestComplete(st, KErrNone);
	}



/**
 * RunL()
 */
void CMsvIndexAdapter::RunL()
	{
	// Set the state for the background state machine.
	iBackgroundOperationState = ERemoveDeletedEntries;
	iBackgroundOperationPerformed = 0;
	}
	



/**
 * DoCancel()
 */
void CMsvIndexAdapter::DoCancel()
	{
	iIdle->Cancel();	
	}




/**
 * AddEntry()
 */
TInt CMsvIndexAdapter::AddEntry(TMsvEntry& aEntry, TSecureId aOwnerId, TBool aAutoAssignId)
	{
	TRAPD(err, DoAddEntryL(aEntry, aOwnerId, aAutoAssignId));
	return err;
	}


/**
 * AddEntryNoCommit()
 */
TInt CMsvIndexAdapter::AddEntryNoCommit(TMsvEntry& aEntry, TSecureId aOwnerId, TBool aAutoAssignId)
	{
	if(iDbAdapter)
		{
		TRAP_IGNORE(iDbAdapter->BeginTransactionL());
		}
	else
		{
		return ErrorState();
		}
	TRAPD(err, DoAddEntryL(aEntry, aOwnerId, aAutoAssignId, EFalse));
	if(err)
		{
		RollbackAdditions();
		}
	return err;
	}



/**
 * DoAddEntryL()
 * 
 * Add the entry to the index and then the file. 
 * If either fails the function leaves. Creates 
 * its own copy of TMsvEntry (SetEntryL() code). 
 * This can be optimized further.
 */
void CMsvIndexAdapter::DoAddEntryL(TMsvEntry& aEntry, TSecureId aOwnerId, TBool aAutoAssignId, TBool aCommitToDb)
	{
	// Leave if the message store is not currently available.
    User::LeaveIfError(iErrorState);
    
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	// get and set the unique id
	if (aAutoAssignId)
		{
		aEntry.SetId(NextId(GetDriveId(aEntry.Parent())));
		}

	if (aEntry.iType == KUidMsvServiceEntry)
		{
		if(KMsvLocalServiceIndexEntryId == UnmaskTMsvId(aEntry.iId))
			{
			aEntry.iServiceId = KMsvLocalServiceIndexEntryId;
			}
		else
			{
			aEntry.iServiceId = aEntry.Id();
			}
		}
#else		// #if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	if (aAutoAssignId)
		{
		aEntry.SetId(NextId());
		}

	if (aEntry.iType == KUidMsvServiceEntry)
		{
		aEntry.iServiceId = aEntry.Id();
		}
#endif		// #if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)

	CMsvCacheEntry* parentEntry = NULL;
	FindEntryL(aEntry.Parent(), parentEntry);

#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	if(UnmaskTMsvId(aEntry.iId) == KMsvDeletedEntryFolderEntryId)
		{
		aEntry.SetVisibleFolderFlag(ETrue);
		}
	else	
#endif
		{		
		// If entry is set to visible while creation, then set the visible folder
		// flag to ETrue, this will not change for the lifetime of an TMsvEntry.
		if(parentEntry->Entry().VisibleFolderFlag())
			{
			if( (aEntry.Visible()) && 
			    (aEntry.iType == KUidMsvFolderEntry || aEntry.iType == KUidMsvServiceEntry)
			  )
				{
				aEntry.SetVisibleFolderFlag(ETrue);
				}
			else
				{
				aEntry.SetVisibleFolderFlag(EFalse);
				}				
			}
		else
			{
			aEntry.SetVisibleFolderFlag(EFalse);
			}
		}
		
	aEntry.SetOwner(EFalse);
	TMsvId visibleFolderId;
	if(!GetVisibleFolderId(aEntry.Parent(), visibleFolderId))
		{
		User::Leave(KErrNotFound);
		}
    
	// Add entry to cache.      
    CMsvCacheEntry* newCacheEntry = NULL;   
    newCacheEntry = iFreePoolInstance->EntryL();
    // Set the owner ID of the entry - the SID of the owning process.
    newCacheEntry->SetEntryOwnerId(aOwnerId);
    
    CMsvCacheVisibleFolder* visibleFolder = NULL;
    TRAPD(err, newCacheEntry->SetEntryL(aEntry);
               visibleFolder = AddEntryToCacheL(visibleFolderId, newCacheEntry);
         );
    if(err)
        {
        iFreePoolInstance->ReleaseEntryWithoutTransaction(newCacheEntry);
        User::Leave(err);
        }

#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
    TRAP(err,
            if(aCommitToDb)
                iDbAdapter->BeginTransactionL();
            iDbAdapter->CreateEntryL(aEntry, visibleFolderId);
            // Set the owner flag of parent entry if not already done.
            // For standard entry created from indexContext, parent Id will not be masked.
            iDbAdapter->UpdateOwnerStatusL(MaskTMsvId(GetDriveId(aEntry.iId), aEntry.Parent()), parentEntry->Entry(), ETrue);
            if(aCommitToDb)
                iDbAdapter->CommitTransactionL();
        );
#else
    TRAP(err,
            if(aCommitToDb)
                iDbAdapter->BeginTransactionL();
            iDbAdapter->CreateEntryL(aEntry, visibleFolderId);
            // Set the owner flag of parent entry if not already done.
            // For standard entry created from indexContext, parent Id will not be masked.
            iDbAdapter->UpdateOwnerStatusL(aEntry.Parent(), parentEntry->Entry(), ETrue);
            if(aCommitToDb)
                iDbAdapter->CommitTransactionL();
        );
#endif  
    if(err & KSqlErrConstraint != err)
        {
        if(aCommitToDb)
            iDbAdapter->RollbackTransactionL();
        // This will not leave...
        TRAP_IGNORE(visibleFolder->DeleteEntryL(newCacheEntry->GetId()));
        User::Leave(err);
        }	
	parentEntry->Entry().SetOwner(ETrue);

	if(!aCommitToDb)
		{
		// Add entry to the list of non-committed entries.
		CNonCommittedAddedEntries* nonCommittedAddedEntries = new(ELeave)CNonCommittedAddedEntries(visibleFolderId, newCacheEntry);
		CleanupStack::PushL(nonCommittedAddedEntries);
		iNonCommittedAddedEntryList.AppendL(nonCommittedAddedEntries);
		CleanupStack::Pop(nonCommittedAddedEntries);
		}

	if(aEntry.iType.iUid == KUidMsvMessageEntryValue && CMSvSearchSortCacheManager::Instance()->iManagerEntry != NULL)
		{	
		TMsgType aType(ENewMsg);
		if(CMSvSearchSortCacheManager::Instance()->iManagerEntry->Count()>0)
			{
			CMsvSearchSortDeltaCache::Instance()->EntryInDeltaCache(aEntry.Id(),aType);
			}
		}	
	}
	
	


/**
 * GetVisibleFolderId()
 * 
 * Returns ETrue, if the parent entry exists. Otherwise returns EFalse.
 * IN: Parent Id of the entry, whose immediate visible folder has to be found.
 * OUT: Visible folder Id.
 */
TBool CMsvIndexAdapter::GetVisibleFolderId(TMsvId aParentId, TMsvId& aVisibleFolderId)
	{
	// If the entry is either a root entry (parent=KErrNotFound) or 
	// child of root entry its visibleFolderId will be root entry itself.
	if((KErrNotFound == aParentId) || (KMsvRootIndexEntryId == aParentId))
		{
		aVisibleFolderId = KMsvRootIndexEntryId;
		return ETrue;
		}
	
	// Get the visible flag of parent entry.
	
	// If parent entry is visible, then immediateVisibleFolder of child
	// should be parent Id. And if it is invisible then child's immediateVisibleFolder 
	// should be same as parent's immediateVisibleFolder.
	// First check in cache.
	if(!iFolderListHeader.IsEmpty())
		{
		CMsvCacheVisibleFolder* folderNode = NULL;
		TDblQueIter<CMsvCacheVisibleFolder> dqIter(iFolderListHeader);
		dqIter.SetToFirst(); 
		while((folderNode = dqIter++)!=NULL)
			{
			// Check if the current folder node is parent.
			if( aParentId == folderNode->GetFolderId() )
				{
				aVisibleFolderId = aParentId;
				return ETrue;
				}
			}
		dqIter.SetToFirst(); 
		CMsvCacheEntry* entry;
		while((folderNode = dqIter++)!=NULL)
			{
			// Check if the current folder node is parent.
			if(folderNode->GetEntry(aParentId, entry))
				{
				if(entry->Entry().VisibleFolderFlag())
					{
					aVisibleFolderId = aParentId;
					}
				else
					{
					//If the visible folder is not the parent, then
					//the current folder must be the visible folder,
					//as otherwise the parent would have been under another
					//folder node.
					aVisibleFolderId = folderNode->GetFolderId();
					}
				return ETrue;
				}
			}
		}
	
	// Entry cannot be found in cache,
	// Now search in DB.
	TInt err = KErrNone;
	if(iDbAdapter)
		{
		TRAP(err, iDbAdapter->GetVisibleFolderIdL(aParentId, aVisibleFolderId));
		}
	else
		{
		return EFalse;
		}
	if(KErrNone == err)
		{
		return ETrue; 		
		}
	return EFalse;		
	}





/**
 * DeleteEntry()
 */
TInt CMsvIndexAdapter::DeleteEntry(TMsvId aId)
	{
	CMsvEntrySelection* selection = NULL;
	
	TRAPD(error, selection = new(ELeave)CMsvEntrySelection; selection->AppendL(aId););
	if(KErrNone == error)
		{
		error = DeleteSelection(*selection);
		}

	TMsgType aType(EDeletedMsg);
	if(CMSvSearchSortCacheManager::Instance()->iManagerEntry->Count()>0 && error == KErrNone)
		{
		CMsvSearchSortDeltaCache::Instance()->EntryInDeltaCache(aId,aType);
		}

	delete selection;
	return error;
	}





/**
 * DeleteSelection
 * Deletes a selection of entries.
 *
 * @param aSelection Selection of TMsvIds of the entries to be deleted
 * @return TInt System-wide error code.
 */
TInt CMsvIndexAdapter::DeleteSelection(const CMsvEntrySelection& aSelection)
	{
	TInt count = aSelection.Count();

	if(!iDbAdapter)
		{
		return ErrorState();
		}
	// Only start a transaction if there are more than 1 entries to be deleted.
	if(count > 1)
		{
		TRAP_IGNORE(iDbAdapter->BeginTransactionL());		
		}
	// If the transaction is opened, deleted
	// entry will be released temporarily.
	iFreePoolInstance->BeginTransaction();
	TInt error = KErrNone;
	TRAP(error, DoDeleteSelectionL(aSelection));
	if (error)
		{
		if(count > 1)
			{
			TRAP_IGNORE(iDbAdapter->RollbackTransactionL());
			}
		iFreePoolInstance->RollbackTransaction();
		}
	else
		{
		if(count > 1)
			{
			TRAP_IGNORE(iDbAdapter->CommitTransactionL());
			}	
		iFreePoolInstance->CommitTransaction();
		}
	return error;
	}





/**
 * DoDeleteSelectionL
 */
void CMsvIndexAdapter::DoDeleteSelectionL(const CMsvEntrySelection& aSelection)
	{
	User::LeaveIfError(iErrorState);
	CMsvCacheVisibleFolder* folderNode = NULL;
	TDblQueIter<CMsvCacheVisibleFolder> dqIter(iFolderListHeader);
	const TInt count = aSelection.Count();
	if(count <= 0)
		{
		return;
		}
	
	CMsvCacheEntry* entry = NULL;
	TBool releaseEntry = FindEntryL(aSelection.At(0), entry, EFalse);
	TMsvId parentId = entry->Entry().Parent();
	if(releaseEntry)
		{
		iFreePoolInstance->ReleaseEntryL(entry, ETrue);
		}
	CMsvEntrySelection* children = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(children);
	
	for (TInt index=0; index<count; ++index)
		{
		dqIter.SetToFirst(); 
		TBool found = EFalse;
		while((folderNode = dqIter++)!=NULL)
			{
			TRAPD(err, folderNode->DeleteEntryL(aSelection.At(index)));	
			if(err != KErrNotFound && err != KErrNone)
				{
				User::Leave(err);
				}
			 else if(err == KErrNone)	
			 	{
			 	found = ETrue;
			 	break;
			 	}
			}
		if(!found)
			{
			if(iDbAdapter)
				{
				if(iDbAdapter->EntryExistsL(aSelection.At(index)))
					{
					User::Leave(KErrAccessDenied);
					}
				}
			else
				{
				User::Leave(ErrorState());	
				}
			User::Leave(KErrNotFound);
			}
		
		if(iDbAdapter)
			{
			iDbAdapter->DeleteEntryL(aSelection.At(index));
			}
		else
			{
			User::Leave(ErrorState());	
			}
		}
	//Find the parent entry in cache and check for any children.
	//If there are no more children under the parent, then
	//reset the owner flag in cache and database.
	TRAPD(err, releaseEntry = FindEntryL(parentId, entry, EFalse));
	//If parent was also deleted then nothing left to do.
	if(err == KErrNotFound)
		{
		CleanupStack::PopAndDestroy(); //children
		return;
		}
	TRAP(err, 
		GetChildrenIdL(parentId, *children);
		if(0 == children->Count() && entry->Entry().Owner())
			{
			if(iDbAdapter)
				{
				iDbAdapter->UpdateOwnerStatusL(parentId, entry->Entry(), EFalse);		
				}
			else
				{
				User::Leave(ErrorState());
				}
			entry->Entry().SetOwner(EFalse);
			}
		);
	// Release the entry, since it is not added to cache.
	if(releaseEntry)
		{
		iFreePoolInstance->ReleaseEntryL(entry, ETrue);
		}
	User::LeaveIfError(err);
	CleanupStack::PopAndDestroy(); //children
	}





/**
 * ExpandSelectionRecursively()
 * Expands a selection to include all the descendents of the entries. They are order
 * in such a way that they can be deleted in order, children first
 *
 * @param aSelection CMsvEntrySelection, which contains the TMsvIds of the entry passed.
 * @return TInt System wide error code.
 */
TInt CMsvIndexAdapter::ExpandSelectionRecursively(CMsvEntrySelection& aSelection)
	{
	TInt error = KErrNone;
	iOrigEntryPos=0;
	iRecursionSelection=&aSelection;
	TInt count = aSelection.Count();
	while (iOrigEntryPos < count)
		{
		TMsvId entryId = aSelection.At(iOrigEntryPos);
		if( !EntryExists(entryId) )
			{
			error = KErrNotFound;
			aSelection.Delete(iOrigEntryPos);
			}
		else
			{
			// if the entry has children
			CMsvEntrySelection* children = NULL;
			TRAPD(err, children = new (ELeave)CMsvEntrySelection);
			if(KErrNone != err)
				{
				return err;
				}
			TInt leave = KErrNone;
			TRAP(leave, GetChildrenIdL(entryId, *children));
			if(KErrNone != leave)
				{
				delete children;
				return leave;				
				}
			TInt count = children->Count();
			for(TInt index=0; index<count; ++index)
				{
				iRecursionIndex = iOrigEntryPos;
				TRAP(leave, DoExpandSelectionRecursivelyL(children->At(index)));
				if(KErrNone != leave)
					{
					delete children;
					return leave;
					}
				}
			delete children;	
			++iOrigEntryPos;
			}
		}
	return error;
	}





/**
 * DoExpandSelectionRecursivelyL
 * Expands a selection to include all the descendents of the entry with TMsvId aId. 
 *
 * @param aId TMsvId of the entire
 * @return void
 */
void CMsvIndexAdapter::DoExpandSelectionRecursivelyL(TMsvId aId)
	{
	// include this entry
	iRecursionSelection->InsertL(iRecursionIndex,aId);
	iOrigEntryPos++;
	
	// if aEntry has children
	CMsvEntrySelection* children = new (ELeave)CMsvEntrySelection;
	CleanupStack::PushL(children);
	
	GetChildrenIdL(aId, *children);
	
	TInt count = children->Count();
	for(TInt index=0; index<count; ++index)
		{
		DoExpandSelectionRecursivelyL(children->At(index));
		}
	
	CleanupStack::PopAndDestroy(1, children);	//children
	}



/**
 * GetChildrenId()
 * @param TMsvId: Id os the parent.
 * @param CMsvEntrySelection: List of child Ids.
 * 
 * This function should be used only by ExpandSelectionRecursively()
 * and DoExpandSelectionRecursivelyL().
 * --- 557 ---
 * If aId is an standard id ensure that it is masked. If the aId is
 * not masked the function will return children from current drive.
 */
void CMsvIndexAdapter::GetChildrenIdL(TMsvId aId, CMsvEntrySelection& aSelection)
	{
	CMsvCacheVisibleFolder* folderNode = NULL;	
	TDblQueIter<CMsvCacheVisibleFolder> dqIter(iFolderListHeader);

#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	if(IsStandardId(aId))
		{
		TUint driveId = GetDriveId(aId);
		TMsvId unmaskedId = UnmaskTMsvId(aId);
		
		dqIter.SetToFirst();	
		// Look for the entry in cache.
		if(!iFolderListHeader.IsEmpty())
			{
			while(NULL != (folderNode = dqIter++))
				{
				if((unmaskedId == folderNode->GetFolderId()) && (driveId == folderNode->GetDrive()))
					{
					if(!folderNode->GetChildrenIdL(aSelection))
						{
						if(iDbAdapter)
							{
							iDbAdapter->GetChildrenIdL(aId, aSelection);
							}
						else
							{
							User::Leave(ErrorState());
							}
						}
					return;
					}
				}
			}
		if(iDbAdapter)
			{
			iDbAdapter->GetChildrenIdL(aId, aSelection);
			}
		else
			{
			User::Leave(ErrorState());
			}
		return;
		}
#endif		// #if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)

	dqIter.SetToFirst();	
	// Look for the entry in cache.
	if(!iFolderListHeader.IsEmpty())
		{
		// Check if aId is one of the visible folder,
		// already present in the cache.
		while(NULL != (folderNode = dqIter++))
			{
			if(aId == folderNode->GetFolderId())
				{
				// Fetch children. Returns EFalse if unable
				// to get them.
				if(!folderNode->GetChildrenIdL(aSelection))
					{
					if(iDbAdapter)
						{
						// Childrens not fetched. Get them from DB.
						iDbAdapter->GetChildrenIdL(aId, aSelection);
						}
					else
						{
						User::Leave(ErrorState());	
						}
					}
				return;
				}
			}

		// Entry is not present in the visibleFolder list.
		CMsvCacheEntry* entry = NULL;
		if(FindEntryInCache(aId, entry))
			{
			if(NULL != entry->ChildIdArray())
				{
				RArray<TMsvId>* childId = entry->ChildIdArray();
				TInt size = childId->Count();
				TInt index = 0;
				while(index < size)
					{
					aSelection.AppendL((*childId)[index++]);
					}
				return;	
				}
			else
				{
				if(iDbAdapter)
					{
					iDbAdapter->GetChildrenIdL(aId, aSelection);
					}
				else
					{
					User::Leave(ErrorState());	
					}
				RArray<TMsvId>* childId = new(ELeave) RArray<TMsvId>;
				TInt size = aSelection.Count();
				TInt index = 0;
				while(index < size)
					{
					TInt err = childId->Append(aSelection[index++]);
					if(KErrNone != err)
						{
						childId->Reset();
						childId->Close();
						delete childId;
						User::Leave(err);
						}
					}
				entry->SetChildIdArray(childId);
				return;
				}
			}
		}
	
	// Could not find parent entry in cache.
	// Check in DB now.	
	if(iDbAdapter)
		{
		iDbAdapter->GetChildrenIdL(aId, aSelection);
		}
	else
		{
		User::Leave(ErrorState());	
		}
	}




/**
 * ChangeEntry
 * Change the contents of a TMsvEntry. 
 * 
 */
TInt CMsvIndexAdapter::ChangeEntry(const TMsvEntry& aEntry, TSecureId aOwnerId, TBool aForcedUpdate)
	{
	TRAPD(err, DoChangeEntryL(aEntry, aOwnerId, EFalse, aForcedUpdate, ETrue));
	return err;
	}


/**
 * ChangeEntryNoCommit
 */
TInt CMsvIndexAdapter::ChangeEntryNoCommit(const TMsvEntry& aEntry, TSecureId aOwnerId, TBool aForcedUpdate)
	{
	if(!iDbAdapter)
		{
		return ErrorState();
		}
	
	TRAP_IGNORE(iDbAdapter->BeginTransactionL());
	TRAPD(err, DoChangeEntryL(aEntry, aOwnerId, EFalse, aForcedUpdate, EFalse));
	if(err)
		{
		RollbackChanges();
		}
	return err;
	}
	

/**
 * ChangeEntryInternal()
 */
TInt CMsvIndexAdapter::ChangeEntryInternal(const TMsvEntry& aEntry, TSecureId aOwnerId)
	{
	TRAPD(err, DoChangeEntryL(aEntry, aOwnerId, ETrue, EFalse, ETrue));
	return err;
	}



/**
 * DoChangeEntryL
 */	
void CMsvIndexAdapter::DoChangeEntryL(const TMsvEntry& aNewEntryContents, TSecureId aOwnerId, TBool aChangeStandardFolder, TBool aForcedUpdate, TBool aCommitToFile)
    {
    // Leave if the message store is not currently available
    User::LeaveIfError(iErrorState);

    // 1. Handle root entry differently.
    if(aNewEntryContents.Id() == KMsvRootIndexEntryId)
        {
        TMsvEntry& entry = const_cast<TMsvEntry&> (aNewEntryContents);
        iDbAdapter->UpdateEntryL(entry, KMsvRootIndexEntryId, EFalse);
        iRootEntry->Entry() = entry;
        return;
        }

    // 2. Find entry in cache.
    CMsvCacheEntry* oldEntry = NULL;    
    CMsvCacheVisibleFolder* oldFolderNode = NULL;
    GetVisibleFolderDetailsL(aNewEntryContents.Id(), oldEntry, oldFolderNode);
  
    // 3. Validate entry.
    // If the entry is not present in cache, it means it is not locked.
    // Can only change a locked entry and not one marked as a standard 
    // folder unless aChangeStandardFolder is true.
    if( (!oldEntry->IsEntryLocked()) || (aChangeStandardFolder==EFalse && oldEntry->Entry().StandardFolder()) )
        {
        User::Leave(KErrAccessDenied);
        }

    // Reserving space in changed entry list array.
    if(!aCommitToFile)
        {
        iNonCommittedChangedEntryList.ReserveL(iNonCommittedChangedEntryList.Count()+1);
        }
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
    // We do not support movement of entry to a folder present in a different drive.    
    if( oldEntry->Entry().Parent() != KErrNotFound &&   //-1 will appear to be masked. disregard this.
        GetDriveId(aNewEntryContents.Id()) != GetDriveId(oldEntry->Entry().Parent()))
        {
        User::Leave(KErrNotSupported);  
        }
#endif

    TMsvId oldParentId = oldEntry->Entry().Parent();
    TMsvId oldVisibleFolderId = oldFolderNode->GetFolderId();

    // Check if the parent id is also changed.
    CMsvCacheEntry* oldParentEntry = NULL;
    CMsvCacheEntry* newParentEntry = NULL;
    TBool resetOldParentOwnerFlag = EFalse;
    CMsvEntrySelection* descendentList = NULL;
    TMsvId newParentId = aNewEntryContents.Parent();
    CMsvCacheVisibleFolder* newVisibleFolderNode = NULL;
    if(oldParentId != newParentId)
        {
        // YES, PARENT ID IS CHANGED.
        // These steps are similar to MoveEntry()
        newVisibleFolderNode = DoChangeEntryPreambleL(oldEntry,
                                                      newParentId,
                                                      oldParentEntry,
                                                      newParentEntry,
                                                      oldVisibleFolderId,
                                                      resetOldParentOwnerFlag,
                                                      descendentList
                                                     );
        }   // if(oldParentId != newParentId)
    TMsvId newVisibleFolderId = newVisibleFolderNode ? newVisibleFolderNode->GetFolderId() : oldVisibleFolderId;
    // Update entry in cache.
    TBool changedPrivateInfo;
    CMsvCacheEntry* bkpEntry = NULL;
    TRAPD(err, bkpEntry = iFreePoolInstance->EntryL();
               bkpEntry->SetEntryL(oldEntry->Entry());
               bkpEntry->SetEntryOwnerId(oldEntry->EntryOwnerId());
               oldEntry->CopyEntryL(aNewEntryContents, changedPrivateInfo);
         );
    if(err)
        {
        iFreePoolInstance->ReleaseEntryWithoutTransaction(bkpEntry);
        if(newVisibleFolderNode)
            newVisibleFolderNode->DeleteEntryL(oldEntry->GetId());
        User::Leave(err);
        }
 
	UpdateDates(*oldEntry, EFalse);
	if(aForcedUpdate || changedPrivateInfo && aOwnerId != KMsvServerId )
        {
        oldEntry->SetEntryOwnerId(aOwnerId);
        }

    TRAP(err, 
            if(aCommitToFile)
                iDbAdapter->BeginTransactionL();
            // Update the actual entry
            if(oldParentId != newParentId)
                {
                iDbAdapter->UpdateEntryL(oldEntry->Entry(), newVisibleFolderId);
                }
            else
                {
                iDbAdapter->UpdateEntryL(oldEntry->Entry(), newVisibleFolderId, EFalse);
                }
            // Reset old parent owner flag.
            if(resetOldParentOwnerFlag)
                iDbAdapter->UpdateOwnerStatusL(oldParentId, oldParentEntry->Entry(), EFalse);
            // Set new parent owner flag.
            if(newParentEntry && (!newParentEntry->Entry().Owner()))
                iDbAdapter->UpdateOwnerStatusL(newParentId, newParentEntry->Entry(), ETrue);
            // Update the child entries visibleFolderId in DB.
            if(descendentList)
                iDbAdapter->UpdateVisibleFolderL(descendentList, newVisibleFolderId);
            if(aCommitToFile)
                iDbAdapter->CommitTransactionL();
         );
    if(err)
        {
        if(aCommitToFile)
            TRAP_IGNORE(iDbAdapter->RollbackTransactionL());
        oldEntry->SetEntryOwnerId(bkpEntry->EntryOwnerId());
        oldEntry->CopyEntryL(bkpEntry->Entry(), changedPrivateInfo);
        iFreePoolInstance->ReleaseEntryWithoutTransaction(bkpEntry);
        if(newVisibleFolderNode)    
            newVisibleFolderNode->DeleteEntryL(oldEntry->GetId());
        User::Leave(err);
        }
    // Nothing should fail after this. Ensure that 
    // all leaving function after this does not leave.
    if(aCommitToFile)
        {
        iFreePoolInstance->ReleaseEntryWithoutTransaction(bkpEntry);
        DoChangeEntryPostamble(oldFolderNode, newVisibleFolderId, oldEntry->GetId(), descendentList, newParentEntry, oldParentEntry, resetOldParentOwnerFlag);
        if(descendentList)
            {
            CleanupStack::PopAndDestroy(descendentList);
            }
        }   // if(aCommitToFile)
    else
        {
        // Store relevant data for later commit.
        TNonCommittedChangedEntries entryDetails(oldFolderNode, 
                                                 newVisibleFolderNode, 
                                                 oldEntry,
                                                 bkpEntry,
                                                 oldParentEntry, 
                                                 newParentEntry,
                                                 descendentList,
                                                 resetOldParentOwnerFlag);
        iNonCommittedChangedEntryList.AppendL(entryDetails);
        if(descendentList)
            CleanupStack::Pop(descendentList);
        }

    // Update entry in SearchSort delta cache.
    if(aNewEntryContents.iType.iUid == KUidMsvMessageEntryValue && CMSvSearchSortCacheManager::Instance()->iManagerEntry != NULL)
        {
        TMsgType aType(EUpdatedMsg);
        if(CMSvSearchSortCacheManager::Instance()->iManagerEntry->Count()>0)
            {
            CMsvSearchSortDeltaCache::Instance()->EntryInDeltaCache(aNewEntryContents.Id(), aType);
            }
        }
    }



// Used only by DoChangeEntryL()
CMsvCacheVisibleFolder* CMsvIndexAdapter::DoChangeEntryPreambleL(CMsvCacheEntry*& aOldEntry, TMsvId aNewParentId, CMsvCacheEntry*& aOldParentEntry, CMsvCacheEntry*& aNewParentEntry, TMsvId aOldVisibleFolderId, TBool& aResetOldParentOwnerFlag, CMsvEntrySelection*& aDescendentList)
    {
    // YES, PARENT ID IS CHANGED.
    // These steps are similar to MoveEntry()
    TBool des;
    IsADescendent(aOldEntry->Entry().Id(), aNewParentId, des);
    if (des)
        {
        User::Leave(KErrArgument);
        }
   
    // Fetch the parent entries.    
    FindEntryL(aOldEntry->Entry().Parent(), aOldParentEntry);
    FindEntryL(aNewParentId, aNewParentEntry);

    // Check if the owner flag of the old parent needs to be reset.    
    CMsvEntrySelection* children = new(ELeave)CMsvEntrySelection;
    CleanupStack::PushL(children);
    GetChildrenIdL(aOldEntry->Entry().Parent(), *children);
    if(1 == children->Count() && aOldParentEntry->Entry().Owner())
        {
        aResetOldParentOwnerFlag = ETrue;
        }
    CleanupStack::PopAndDestroy(); //children
    
    // If required, pre-allocate memory for child array of newParentEntry.
    if(aNewParentEntry->ChildIdArray())
        {
        // This to ensure that future append does not fail.
        aNewParentEntry->ChildIdArray()->ReserveL(aNewParentEntry->ChildIdArray()->Count() + 1);
        }

    // Find the visibleFolder of new parent. 
    TMsvId newVisibleFolderId = NULL;
    if(!GetVisibleFolderId(aNewParentId, newVisibleFolderId))
         {
         User::Leave(KErrNotFound);
         }

    // Check if the visible folders are different.
    CMsvCacheVisibleFolder* newVisibleFolderNode = NULL;
    if(aOldVisibleFolderId != newVisibleFolderId)
        {
        // If the visible folders are different, check if
        // child entries also needs to be moved.
        if(!aOldEntry->Entry().VisibleFolderFlag())
            {
            // If yes, create a descendent list of the entry.
            aDescendentList = new(ELeave) CMsvEntrySelection;
            CleanupStack::PushL(aDescendentList);
            aDescendentList->AppendL(aOldEntry->GetId());
            User::LeaveIfError(ExpandSelectionRecursively(*aDescendentList));
            }
        
        // Add duplicate entry under new visible folder. Removing 
        // entry from old visible folder is done later.
        CMsvCacheEntry* newCacheEntry = iFreePoolInstance->EntryL();
        TRAPD(err, newCacheEntry->DupNDestroyL(aOldEntry);
                   newVisibleFolderNode = AddEntryToCacheL(newVisibleFolderId, newCacheEntry);
             );
        if(err)
            {
            iFreePoolInstance->ReleaseEntryWithoutTransaction(newCacheEntry);
            User::Leave(err);
            }
        aOldEntry = newCacheEntry;
        }   // if(oldVisibleFolderId != newVisibleFolderId)    }
    return newVisibleFolderNode;
    }



// Used only by DoChangeEntryL()
void CMsvIndexAdapter::DoChangeEntryPostamble(CMsvCacheVisibleFolder* aOldFolderNode, TMsvId aNewVisibleFolderId, TMsvId aEntryId, CMsvEntrySelection* aDescendentList, CMsvCacheEntry* aNewParentEntry, CMsvCacheEntry* aOldParentEntry, TBool aResetOldParentOwnerFlag)
    {
     // Removing entry from old visible folder.
     if(aOldFolderNode->GetFolderId() != aNewVisibleFolderId)
         {
         // Will never leave.
         //aOldFolderNode->DeleteEntryL(oldEntry->GetId());
         TRAP_IGNORE(aOldFolderNode->DeleteEntryL(aEntryId));
         }
     
     // Removing child entries from old visible folder.
     if(aDescendentList)
         {
         for(TInt index=(aDescendentList->Count()-2); index>=0; index--)
             {
             // Can leave with KErrNotFound.
             TRAP_IGNORE(aOldFolderNode->DeleteEntryL(aDescendentList->At(index)));
             }         
         }
    
     // Add child id to new parent child array.
     if(aNewParentEntry && aNewParentEntry->ChildIdArray())
         {
         // This will not leave, as memory is already reserved.
         TRAP_IGNORE(aNewParentEntry->ChildIdArray()->AppendL(aEntryId));
         }
     
     // Remove child from old parent's child array.
     if(aOldParentEntry)
         {
         RArray<TMsvId>* oldParentChildArr = aOldParentEntry->ChildIdArray();
         if(oldParentChildArr)
             {
             TInt pos = oldParentChildArr->Find(aEntryId);
             if(pos != KErrNotFound)
                 {
                 oldParentChildArr->Remove(pos);
                 }
             }
         }
    
     // Update owner flag of parent entries.
     if(aResetOldParentOwnerFlag)
         {
         aOldParentEntry->Entry().SetOwner(EFalse);
         }
     if(aNewParentEntry)
         {
         aNewParentEntry->Entry().SetOwner(ETrue);
         }
 }
 
 
 
 
/**
 * GetEntry()
 */
TInt CMsvIndexAdapter::GetEntry(TMsvId aId, TMsvEntry*& aEntry) 
	{
	TSecureId dummy;
	return GetEntry(aId, aEntry, dummy);
	}



/**
 * GetEntryNoCache()
 * If aAddToCache is EFalse, entry will not be added to cache,
 * if the entry is fetched from DB. The user of this function
 * should ensure that it releses the memory occupied by entry
 * to the freepool.
 */
TInt CMsvIndexAdapter::GetEntryNoCache(TMsvId aId, TMsvEntry* aEntry)
	{
	if(KMsvRootIndexEntryId == aId)
		{
		*aEntry = iRootEntry->Entry();				
		return KErrNone;
		}

	TBool aIsDanglingEntry = EFalse;
	CMsvCacheEntry* serverEntry=NULL;
	TRAPD(err, aIsDanglingEntry = FindEntryL(aId, serverEntry, EFalse));

	if (err == KErrNone)
		{
		*aEntry = serverEntry->Entry();
		// If entry is not present in cache,
		// it should be handled carefully.
		if(aIsDanglingEntry)
			{
			// Release CMsvCacheEntry to freepool.
			iFreePoolInstance->ReleaseEntryL(serverEntry, ETrue);
			}		
		return KErrNone;	
		}
	else
		{
		return KErrNotFound;
		}
	}



	
/**
 * GetEntry()
 */
TInt CMsvIndexAdapter::GetEntry(TMsvId aId, TMsvEntry*& aEntry, TSecureId& aOwnerId) 
	{
	if(KMsvRootIndexEntryId == aId)
		{
		aEntry = &iRootEntry->Entry();
		aOwnerId = 0;
		return KErrNone;
		}

	CMsvCacheEntry* serverEntry=NULL;
	TRAPD(err, FindEntryL(aId, serverEntry));	

	if (err == KErrNone)
		{
		aEntry = &serverEntry->Entry();
		aOwnerId = serverEntry->EntryOwnerId();
		return KErrNone;	
		}
	else
		{
		return KErrNotFound;
		}
	}
	


/**
 * FindEntryInCache()
 * @param aId: Entry Id.
 * @return aEntry: CMsvCacheEntry
 * @return bool: ETrue if entry found, otherwise EFalse.
 * 
 * Find an entry only in cache and return the entry.
 */
TBool CMsvIndexAdapter::FindEntryInCache(TMsvId aId, CMsvCacheEntry*& aEntry) 
	{
	if(KMsvRootIndexEntryId == aId)
		{
		aEntry = iRootEntry;
		return ETrue;
		}

	// If cache is not empty.
	if(!iFolderListHeader.IsEmpty())
		{
		CMsvCacheVisibleFolder* folderNode = NULL;
		TDblQueIter<CMsvCacheVisibleFolder> dqIter(iFolderListHeader);
		dqIter.SetToFirst(); 
		
		while((folderNode = dqIter++)!=NULL)
			{
			if(folderNode->GetEntry(aId, aEntry))
				{
				return ETrue;
				}
			}
		}
	aEntry = NULL;
	return EFalse;
	}



/**
 * FindEntryL()
 * 
 * WARNING:
 * Returns CMsvCacheEntry which is still owned by Cache. so aEntry should
 * never be deleted by the caller.
 *
 * If the entry is fetched from DB and not added to cache, the function 
 * returns ETrue, indicating that aEntry should be release to the freepool
 * by the caller.
 */
TBool CMsvIndexAdapter::FindEntryL(TMsvId aId, CMsvCacheEntry*& aEntry, TBool aAddToCache /*DEFAULT=ETrue */)
	{
	// First search the entry in cache, 
	if(FindEntryInCache(aId, aEntry))
		{
		return EFalse;
		}	

	// Entry cannot be found in cache,
	// Now search in DB.
	TMsvId visibleEntryId;
	if(iDbAdapter)
		{
		iDbAdapter->GetEntryL(aId, aEntry, visibleEntryId);
		}
	else
		{
		User::Leave(ErrorState());	
		}

	if(aAddToCache)	
		{
		// Add entry to cache.
		AddEntryToCacheL(visibleEntryId, aEntry);	
		}
	else
		{
		// If the entry is not added to cache, we need not start 
		// the background operation. We can return ETrue saying
		// entry is dangling and needs to be freed.
		return ETrue;
		}
	if (!iIdle->IsActive())
		{
		iIdle->Start(TCallBack(BackGroundOperations, this));
		}
	return EFalse;
	}




/**
 * AddEntryNoVisibleL()
 */
void CMsvIndexAdapter::AddEntryNoVisibleL(CMsvCacheEntry* aEntry)
	{
	TMsvId visibleFolder;
	if(GetVisibleFolderId(aEntry->Entry().Parent(), visibleFolder))	
		{
		AddEntryToCacheL(visibleFolder, aEntry);
		}
	}



/**
 * AddEntryToCacheL()
 */
CMsvCacheVisibleFolder* CMsvIndexAdapter::AddEntryToCacheL(TMsvId aVisibleEntryId, CMsvCacheEntry* aEntry)
	{
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	TUint driveId = GetDriveId(aEntry->GetId());
	if(IsStandardId(aEntry->GetId()))
		{
		aEntry->Entry().SetId(UnmaskTMsvId(aEntry->GetId()));
		}
#endif
	// First search the visibleEntry in cache, 
	// if cache is not empty...
	if(!iFolderListHeader.IsEmpty())
		{
		CMsvCacheVisibleFolder* folderNode = NULL;
		TDblQueIter<CMsvCacheVisibleFolder> dqIter(iFolderListHeader);
		dqIter.SetToFirst();
		
		while((folderNode =  dqIter++)!=NULL)
			{
			TMsvId folderId = folderNode->GetFolderId();
			if(aVisibleEntryId == folderId)
				{
				// Visible folder exists.
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
				// For standard entries also check for driveId.
				if(IsStandardId(aVisibleEntryId))
					{
					if(driveId == folderNode->GetDrive())
						{
						folderNode->AddEntryL(aEntry);
						return folderNode;
						}
					}
				else
#endif
					{
					// Add entry to this folder.
					folderNode->AddEntryL(aEntry);
					return folderNode;
					}
				}
			}
		}

	// Visible folder not found in the folder list.
	// Create the visible folder and add it to the
	// beginning of the list.
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	CMsvCacheVisibleFolder* newFolder = CMsvCacheVisibleFolder::NewL(MaskTMsvId(GetDriveId(aEntry->GetId()), aVisibleEntryId));
#else
	CMsvCacheVisibleFolder* newFolder = CMsvCacheVisibleFolder::NewL(aVisibleEntryId);
#endif
	iFolderListHeader.AddFirst(*newFolder);
	
	// Now add entry to the visible folder
	newFolder->AddEntryL(aEntry);
	return newFolder;
	}
	


/**
 * GetChildrenL()
 */
void CMsvIndexAdapter::GetChildrenL(TMsvId aId, CArrayPtr<const TMsvEntry>& aSelection, const TMsvSelectionOrdering& aOrdering, TUid aMtm)
	{
	TSecureId dummy = KMsvServerId.iId;
	GetChildrenL(aId, aSelection, aOrdering, aMtm, EFalse, dummy);
	}


	
/**
 * GetChildrenL()
 */
void CMsvIndexAdapter::GetChildrenL(TMsvId aId, CArrayPtr<const TMsvEntry>& aSelection, const TMsvSelectionOrdering& aOrdering, TUid aMtm, TBool aFilterByOwnerId, TSecureId aOwnerId)
	{
	CMsvEntryFilter* filter = CMsvEntryFilter::NewLC();
	filter->SetOrder(aOrdering);
	filter->SetSortMtm(aMtm);
	CMsvEntryArray* entries = DoGetChildrenL(aId, *filter, aFilterByOwnerId, aOwnerId);	
	CleanupStack::PushL(entries);	
	
	TInt count = entries->Count();
	for (TInt ii=0; ii<count; ii++)
		{
		aSelection.AppendL(entries->At(ii));
		}
	CleanupStack::PopAndDestroy(2, filter); // filter and entries
	}



	
/**
 * DoGetChildrenL
 */	
CMsvEntryArray* CMsvIndexAdapter::DoGetChildrenL(TMsvId aId, const CMsvEntryFilter& aFilter, TBool aFilterByOwnerId, TSecureId aOwnerId)
	{
	// Check if entry for aId exists...
	CMsvCacheEntry* entry=NULL;
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	TUint driveId = GetDriveId(aId);
	TMsvId unmaskedId = UnmaskTMsvId(aId);
	if(IsStandardId(aId))
		{		
		FindEntryL(unmaskedId, entry);
		}
	else
#endif
		{
		FindEntryL(aId, entry);
		}

	TBool isParentAVisibleFolder = EFalse;
	if(entry->Entry().VisibleFolderFlag())
		{
		isParentAVisibleFolder = ETrue;
		}

	CArrayFixFlat<TUid>* mtmList = new(ELeave) CArrayFixFlat<TUid>(KMsvMtmListGranularity);
	CleanupStack::PushL(mtmList);
	if (KUidMsvNullEntry != aFilter.SortMtm())
		{
		mtmList->AppendL(aFilter.SortMtm());
		}

	RPointerArray<CMsvCacheEntry> selection;
	CleanupClosePushL(selection);
	
	CMsvCacheVisibleFolder* folderNode = NULL;	
	CMsvEntryArray* entries = CMsvEntryArray::NewLC(*mtmList);
	TDblQueIter<CMsvCacheVisibleFolder> dqIter(iFolderListHeader);
	

#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)

	if(IsStandardId(aId))
		{
		// Look for the entry in cache.
		if(!iFolderListHeader.IsEmpty())
			{
			dqIter.SetToFirst(); 
			// Check if aId is one of the visible folder,
			// already present in the cache.
			while(NULL != (folderNode = dqIter++))
				{
				if((folderNode->GetFolderId() == unmaskedId) && (folderNode->GetDrive() == driveId))
					{
					folderNode->GetChildrenL(aId, iDbAdapter, selection);
					FilterChildrenListL(aId, selection, *entries, aFilter, aFilterByOwnerId, aOwnerId);
					CleanupStack::Pop(entries);	// Fetch entries from stack. It will be destroyed by caller.
					CleanupStack::PopAndDestroy(2);
					return entries;
					}
				}
			}
		}
	else
		{				
#endif			// #if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)

	// Look for the entry in cache.
	if(!iFolderListHeader.IsEmpty())
		{
		dqIter.SetToFirst(); 
		// Check if aId is one of the visible folder,
		// already present in the cache.
		while(NULL != (folderNode = dqIter++))
			{
			if(aId == folderNode->GetFolderId())
				{
				if(iDbAdapter == NULL)
					{
					User::Leave(ErrorState());	
					}
				folderNode->GetChildrenL(aId, iDbAdapter, selection);
				FilterChildrenListL(aId, selection, *entries, aFilter, aFilterByOwnerId, aOwnerId);
				CleanupStack::Pop(entries);	// Fetch entries from stack. It will be destroyed by caller.
				CleanupStack::PopAndDestroy(2);
				return entries;
				}
			}

		// If the parent is not a visible folder
		// check if it is child of one of the visible folder.
		if(EFalse == isParentAVisibleFolder)
			{
			dqIter.SetToFirst(); 
			while(NULL != (folderNode = dqIter++))
				{
				TBool retVal=EFalse;
				TRAPD(err, retVal = folderNode->GetChildrenL(aId, iDbAdapter, selection))

				// aId not found under current visibleFolder.
				if(KErrNotFound == err)
					{
					continue;
					}
				
				// aId found under current visibleFolder.
				if(retVal)
					{
					// Children is fetched succesfully.
					FilterChildrenListL(aId, selection, *entries, aFilter, aFilterByOwnerId, aOwnerId);	
					CleanupStack::Pop(entries);  // Fetch entries from stack. It will be destroyed by caller.
					CleanupStack::PopAndDestroy(2);
					return entries;
					}
				else
					{
					// aId itself is a visibleFolder.
					isParentAVisibleFolder = ETrue;
					break;
					}
				}
			}
		}

#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
		}
#endif			// #if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
		
	// Could not find parent entry in cache.
	// Check in DB now.	
	TInt err = KErrNone;
	if(iDbAdapter)
		{
		TRAP(err, iDbAdapter->GetChildrenL(aId, selection));
		}
	else
		{
		User::Leave(ErrorState());	
		}
	if(err)
		{
		User::Leave(KErrNotFound);
		}
	FilterChildrenListL(aId, selection, *entries, aFilter, aFilterByOwnerId, aOwnerId);
	
	// Check if the entire entry list can be added to cache.
	// This is important in a situation when all childs of a 
	// folder cannot be accomodated in cache. In such scenario
	// only entries with higher TMsvId will be added to cache.
	TInt excessEntries = iFreePoolInstance->ExcessMemoryAllocated();
	TBool isCompleteChildren = ETrue;
	if(excessEntries)
		{
		isCompleteChildren = EFalse;
		}
	while(excessEntries)
		{
		if(selection.Count())
			{
			iFreePoolInstance->RecordExcessMemoryL(selection[0]);
			selection.Remove(0);
			}
		--excessEntries;
		}		

	// Add children to cache.
	TMsvId visibleFolderId;
	if(!isParentAVisibleFolder)
		{
		if(!GetVisibleFolderId(aId, visibleFolderId))
			{
			User::Leave(KErrNotFound);
			}
		}
	else
		{
		visibleFolderId = aId;
		}
	
	TBool isEntryAdded = EFalse;		
	dqIter.SetToFirst();
	while((folderNode = dqIter++)!=NULL)
		{
		if(visibleFolderId == folderNode->GetFolderId())
			{
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			if(IsStandardId(visibleFolderId))
				{
				if(GetDriveId(aId) == folderNode->GetDrive())
					{
					isEntryAdded=ETrue;
					folderNode->AddEntryListL(selection, (isCompleteChildren && isParentAVisibleFolder));
					}
				}
			else
#endif
				{
				isEntryAdded=ETrue;
				folderNode->AddEntryListL(selection, (isCompleteChildren && isParentAVisibleFolder));
				}
			}
		}
	
	if(!isEntryAdded)
		{
		// If aId is the visibleFolder itself then pass
		// aIsCompleteChildOfFolder as ETrue.
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
		// Standard Id can be unmasked.
		CMsvCacheVisibleFolder* newFolder = CMsvCacheVisibleFolder::NewL(MaskTMsvId(GetDriveId(aId), visibleFolderId));
#else
		CMsvCacheVisibleFolder* newFolder = CMsvCacheVisibleFolder::NewL(visibleFolderId);
#endif	
		iFolderListHeader.AddFirst(*newFolder);
		newFolder->AddEntryListL(selection, (isCompleteChildren && isParentAVisibleFolder));
		}

	CleanupStack::Pop(entries);		// Fetch entries from stack. It will be destroyed by caller.
	CleanupStack::PopAndDestroy(2);
	
	//Start the background operations, if not running.
	if (!iIdle->IsActive())
		{
		iIdle->Start(TCallBack(BackGroundOperations, this));
		}
		
	return entries;
	}




/**
 * GetChildrenId()
 */
TInt CMsvIndexAdapter::GetChildrenId(TMsvId aId, const CMsvEntryFilter& aFilter, CMsvEntrySelection& aSelection)
	{
	TSecureId dummy = KMsvServerId.iId;
	return GetChildrenId(aId, aFilter, aSelection, EFalse, dummy);
	}
	
/**
 * GetChildrenId()
 */
TInt CMsvIndexAdapter::GetChildrenId(TMsvId aId, const CMsvEntryFilter& aFilter, CMsvEntrySelection& aSelection, TBool aFilterByOwnerId, TSecureId aOwnerId)
	{
	CMsvEntryArray* entries = NULL;
	TInt err = KErrNone;
	TRAP(err, entries=DoGetChildrenL(aId, aFilter, aFilterByOwnerId, aOwnerId));
	if(err)
		{
		delete entries;
		return err;
		}

	TInt count = entries->Count();
	for(TInt i=0; i<count; ++i)
		{
		TRAP(err, aSelection.AppendL(entries->At(i)->Id()));
		}
	delete entries;
	return err;
	}



/**
 * FilterChildrenListL
 */	
void CMsvIndexAdapter::FilterChildrenListL(TMsvId aId, RPointerArray<CMsvCacheEntry> aChacheEntry, CMsvEntryArray& aEntryArray, const CMsvEntryFilter& aFilter, TBool aFilterByOwnerId, TSecureId aOwnerId)
	{
	TBool getInvisibleEntries = aFilter.Order().ShowInvisibleEntries();
	TInt count = aChacheEntry.Count();
	for(TInt i=0; i<count; ++i)
		{
		TMsvEntry entry = aChacheEntry[i]->Entry();
		TBool vis = aChacheEntry[i]->Entry().Visible();
		if ((aChacheEntry[i]->Entry().Visible() || getInvisibleEntries) &&
			(aFilter.Service() == KMsvNullIndexEntryId || aChacheEntry[i]->Entry().iServiceId == aFilter.Service()) &&
			(aFilter.Mtm() == KNullUid || aChacheEntry[i]->Entry().iMtm == aFilter.Mtm()) &&
			(aFilter.Type() == KNullUid || aChacheEntry[i]->Entry().iType == aFilter.Type()) &&
			(aFilter.LastChangeDate().Int64() == 0 || aChacheEntry[i]->LastChangeDate() >= aFilter.LastChangeDate()))
			{
			// Add the entry if - 
			// 1. Not filtering by owner ID OR
			// 2. Filtering by owner ID, but entry owner ID matches OR
			// 3. Entry is a standard folder OR
			// 4. Entrt is a service entry.
			if( !aFilterByOwnerId || 
					aChacheEntry[i]->EntryOwnerId() == aOwnerId ||
					aChacheEntry[i]->Entry().StandardFolder() || 
					aChacheEntry[i]->Entry().iType == KUidMsvServiceEntry )
				{
				aEntryArray.AppendL(&aChacheEntry[i]->Entry());
				}
			}
		}
	
	if (aEntryArray.Count())
		{
		if (aId==KMsvRootIndexEntryId)
			{
			aEntryArray.SortL(TMsvSelectionOrdering(KMsvNoGrouping, aFilter.Order().Sorting()));	
			}
		else
			{
			aEntryArray.SortL(aFilter.Order());	
			}
		}	
	}




/** 
 * LockEntry()
 */
TInt CMsvIndexAdapter::LockEntry(TMsvId aId)
	{
	//__DEBUG_INVARIANT_ONEXIT;
	
	CMsvCacheEntry* entry = NULL;
	TRAPD(err, FindEntryL(aId, entry))
	if (err)
		{
		return KErrNotFound;
		}
	
	return entry->LockEntry();
	}
		


/**
 * ReleaseEntry()
 */
TInt CMsvIndexAdapter::ReleaseEntry(TMsvId aId)
	{
	//__DEBUG_INVARIANT_ONEXIT;
	
	CMsvCacheEntry* entry = NULL;
	if(FindEntryInCache(aId, entry))
		{
		entry->ReleaseEntry();
		return KErrNone;
		}
	else
		{
		return KErrNotFound;
		}
	}
	
	


/**
 * IsEntryLocked()
 */
TInt CMsvIndexAdapter::IsEntryLocked(TMsvId aId, TBool& aLocked)
	{
	CMsvCacheEntry* entry = NULL;
	TRAPD(err, FindEntryL(aId, entry));
	if(KErrNone != err)
		{
		aLocked = EFalse;
		return err;
		}
	
	aLocked = entry->IsEntryLocked();
	return KErrNone;
	}
	
	



/**
 * LockStore()
 */
TInt CMsvIndexAdapter::LockStore(TMsvId aId)
	{
	//__DEBUG_INVARIANT_ONEXIT;
	CMsvCacheEntry* entry = NULL;
	TRAPD(err, FindEntryL(aId, entry));
	if (err)
		{
		return KErrNotFound;
		}
	
	return entry->LockStore();
	}
	



/**
 * ReleaseStore()
 */
TInt CMsvIndexAdapter::ReleaseStore(TMsvId aId)
	{
	//__DEBUG_INVARIANT_ONEXIT;
	
	CMsvCacheEntry* entry = NULL;
	if(FindEntryInCache(aId, entry))
		{
		entry->ReleaseStore();
		return KErrNone;
		}
	else
		{
		return KErrNotFound;
		}
	}
	



/**
 * IsStoreLocked()
 */
TInt CMsvIndexAdapter::IsStoreLocked(TMsvId aId, TBool& aLocked)
	{
	CMsvCacheEntry* entry = NULL;
	TRAPD(err, FindEntryL(aId, entry));
	if(KErrNone != err)
		{
		aLocked = EFalse;
		return err;
		}
	aLocked = entry->IsStoreLocked();
	return KErrNone;
	}



	
/**
 * IsEntryAndStoreLocked()
 */
TInt CMsvIndexAdapter::IsEntryOrStoreLocked(TMsvId aId)
	{
	CMsvCacheEntry* entry = NULL;
	TBool locked = EFalse;
	if(FindEntryInCache(aId, entry))
		{
		locked = entry->IsEntryOrStoreLocked();
		}
	if(locked)
		{
		return KErrLocked;
		}
	else
		{
		return KErrNone;
		}
	}



/**
 * LockEntryAndStore()
 */
TInt CMsvIndexAdapter::LockEntryAndStore(TMsvId aId)
	{
	//__DEBUG_INVARIANT_ONEXIT;
	CMsvCacheEntry* entry = NULL;
	TRAPD(err, FindEntryL(aId, entry));
	if (err)
		{
		return KErrNotFound;
		}
		
	return entry->LockEntryAndStore();
	}
	



/**
 * ReleaseEntryAndStore()
 */
TInt CMsvIndexAdapter::ReleaseEntryAndStore(TMsvId aId)
	{
	//__DEBUG_INVARIANT_ONEXIT;
	CMsvCacheEntry* entry = NULL;
	if(FindEntryInCache(aId, entry))
		{
		entry->ReleaseEntryAndStore();
		return KErrNone;
		}
	else
		{
		return KErrNotFound;
		}
	}
	



/**
 * IsEntryOrStoreLocked()
 */
TInt CMsvIndexAdapter::IsEntryOrStoreLocked(TMsvId aId, TBool& aLocked)
	{
	CMsvCacheEntry* entry = NULL;
	if(FindEntryInCache(aId, entry))
		{
		aLocked = entry->IsEntryOrStoreLocked();
		}
	else
		{
		aLocked = EFalse;
		}
	return KErrNone;
	}
	



/**
 * IsStoreReadingLocked()
 */
TInt CMsvIndexAdapter::IsStoreReadingLocked(TMsvId aId, TBool& aLocked)
	{
	CMsvCacheEntry* entry = NULL;
	if(FindEntryInCache(aId, entry))
		{
		aLocked = entry->IsStoreReadingLocked();
		}
	else
		{
		aLocked = EFalse;
		}
	return KErrNone;	
	}
	



/**
 * IncStoreReaderCount()
 */
TInt CMsvIndexAdapter::IncStoreReaderCount(TMsvId aId)
	{
	CMsvCacheEntry* entry = NULL;
	TRAPD(err, FindEntryL(aId, entry));
	if (err)
		{
		return KErrNotFound;
		}
		
	entry->IncStoreReaderCount();
	return KErrNone;
	}
	



/**
 * DecStoreReaderCount()
 */
TInt CMsvIndexAdapter::DecStoreReaderCount(TMsvId aId)
	{
	CMsvCacheEntry* entry = NULL;
	if(FindEntryInCache(aId, entry))
		{
		entry->DecStoreReaderCount();
		}
	return KErrNone;
	}
	



/**
 * OwningService()
 */
TInt CMsvIndexAdapter::OwningService(TMsvId aId, TMsvId& aService)
	{    
	aService = aId;
	if (aId!=KMsvRootIndexEntryId)
		{
		FOREVER
			{
			CMsvCacheEntry* entry = NULL;	
			TRAPD(err, FindEntryL(aService, entry));
			if (err)
				{
				return KErrNotFound;
				}

			if (KUidMsvServiceEntry==entry->Entry().iType)
				{
				break;
				}
			aService = entry->Entry().Parent();
			}
		}

	return KErrNone;
	}
	



/**
 * IsLocal()
 */
TInt CMsvIndexAdapter::IsLocal(TMsvId aId, TBool& aLocal)
	{
	TInt errVal = KErrNone;
	aLocal = ETrue;
	
	if (aId!=KMsvRootIndexEntryId)
		{
		TMsvId service;
		errVal = OwningService(aId, service);
		aLocal = (KMsvLocalServiceIndexEntryId==service);
		}
		
	return errVal;
	}




/**
 * MoveEntry
 * Moves a entry with TMsvId as aId to a folder with target id as aTarget
 * 
 * @param aId TMsvId of the entry to be moved.
 * @param aTarget TMsvId of the parent folder.
 * @param aDescendents List of descendents id, if the caller also wants to move descendents
 * in cache and DB. The last id in the descendent list must be the entry id itself.
 * @return TInt System-wide error code.
 */
TInt CMsvIndexAdapter::MoveEntry(TMsvId aId, TMsvId aTarget, CMsvEntrySelection* aDescendents /* DEFAULT=NULL */)
	{
	TRAPD(err, DoMoveEntryL(aId, aTarget, aDescendents));
	return err;
	}
	


/**
 * DoMoveEntryL()
 * Called from MoveEntry()
 * */
void CMsvIndexAdapter::DoMoveEntryL(TMsvId aId, TMsvId aTarget, CMsvEntrySelection* aDescendents /* DEFAULT=NULL */)
    {
    // Leave if the message store is not currently available.
    User::LeaveIfError(iErrorState);
    
    CMsvCacheEntry* entry = NULL;   
    CMsvCacheVisibleFolder* oldFolderNode = NULL;
    TMsvId oldVisibleFolderId = KMsvNullIndexEntryIdValue;
    
    // 1. Find the entry in cache.
    GetVisibleFolderDetailsL(aId, entry, oldFolderNode);
        
    // 2. Validate entry.
    TBool des;
    __ASSERT_DEBUG(entry->IsEntryAndStoreLocked(), PanicServer(EMsvMovingUnlockedEntry));
    __ASSERT_DEBUG(entry->Entry().Parent() != aTarget, PanicServer(EMsvMovingWithinSameEntry));
    IsADescendent(entry->Entry().Id(), aTarget, des);
    if (des)
        {// we need to put changes in another method.
        User::Leave(KErrArgument);
        }
    
     // 3. Fetch the parent entries.    
    CMsvCacheEntry* oldParentEntry = NULL;
    CMsvCacheEntry* newParentEntry = NULL;
    TMsvId oldParentId = entry->Entry().Parent();
    FindEntryL(oldParentId, oldParentEntry);
    FindEntryL(aTarget, newParentEntry);
    
    // 4. Check if the owner flag of the old parent needs to be reset.    
    TBool resetOldParentOwnerFlag = EFalse;
    CMsvEntrySelection* children = new(ELeave)CMsvEntrySelection;
    CleanupStack::PushL(children);
    GetChildrenIdL(oldParentId, *children);
    if(1 == children->Count() && oldParentEntry->Entry().Owner())
        {
        // Just set this flag, this is updated in step 14.
        resetOldParentOwnerFlag = ETrue;
        }
    CleanupStack::PopAndDestroy(); //children
    
    // 5. If required, pre-allocate memory for child array of newParentEntry.
    // This is later updated in step 12.
    RArray<TMsvId>* newParentChildArr = newParentEntry->ChildIdArray();
    if(newParentChildArr)
        {
        // This to ensure that future append does not fail.
        newParentChildArr->ReserveL(newParentChildArr->Count() + 1);
        }

    // Step 6 & 7 is performed in this function.
    TBool isChildEntriesNeedsUpdation = EFalse;
    CMsvCacheVisibleFolder* newVisibleFolderNode = UpdateCacheForMoveEntryL(aTarget,                                                      
                                                                            entry, 
                                                                            oldFolderNode, 
                                                                            aDescendents,
                                                                            isChildEntriesNeedsUpdation);

    TMsvId newVisibleFolderId = newVisibleFolderNode? newVisibleFolderNode->GetFolderId(): oldFolderNode->GetFolderId();

    // 8. Update the entry.    
    UpdateDates(*entry, EFalse);
    entry->Entry().SetParent(aTarget);

    // 9. Update the DB.
    TRAPD(err, 
            iDbAdapter->BeginTransactionL();
            // Update the actual entry
            iDbAdapter->UpdateEntryL(entry->Entry(), newVisibleFolderId);
            // Reset old parent owner flag.
            if(resetOldParentOwnerFlag)
                iDbAdapter->UpdateOwnerStatusL(oldParentId, oldParentEntry->Entry(), EFalse);
            // Set new parent owner flag.
            if(!newParentEntry->Entry().Owner())
                iDbAdapter->UpdateOwnerStatusL(aTarget, newParentEntry->Entry(), ETrue);
            // Update the child entries visibleFolderId in DB.
            if(isChildEntriesNeedsUpdation)
                iDbAdapter->UpdateVisibleFolderL(aDescendents, newVisibleFolderId);
            iDbAdapter->CommitTransactionL();
         );

    if(err)
        {        
        TRAP_IGNORE(iDbAdapter->RollbackTransactionL());
        // Undo step 8.
        entry->Entry().SetParent(oldParentId);
        // Undo step 7.
        if(newVisibleFolderNode)    
            newVisibleFolderNode->DeleteEntryL(aId);
        if(isChildEntriesNeedsUpdation)
            {
            for(TInt index=(aDescendents->Count()-2); index>=0; index--)
                {
                newVisibleFolderNode->DeleteEntryL(aDescendents->At(index));
                }
            }
        User::Leave(err);
        }

    // Nothing should fail after this. Ensure that 
    // all leaving function after this does not leave.
    
    // 10. Continuing step 7.1.
    // Removing entry from old visible folder.
    if(oldVisibleFolderId != newVisibleFolderId)
        {
        oldFolderNode->DeleteEntryL(aId);
        }
    
    // 11. Continuing step 7.2.
    // Removing child entries from old visible folder.
    if(isChildEntriesNeedsUpdation)
        {
        for(TInt index=(aDescendents->Count()-2); index>=0; index--)
            {
            oldFolderNode->DeleteEntryL(aDescendents->At(index));
            }
        }

    // 12. Add child id to new parent child array.
    if(newParentChildArr)
        {
        // This will not leave, as memory is already reserved.
        newParentChildArr->AppendL(aId);
        }
    
    // 13. Remove child from old parent's child array.
    RArray<TMsvId>* oldParentChildArr = oldParentEntry->ChildIdArray();
    if(oldParentChildArr)
        {
        TInt pos = oldParentChildArr->Find(aId);
        if(pos != KErrNotFound)
            {
            oldParentChildArr->Remove(pos);
            }
        }

    // 14. Update owner flag of parent entries.
    if(resetOldParentOwnerFlag)
        {
        oldParentEntry->Entry().SetOwner(EFalse);
        }
    newParentEntry->Entry().SetOwner(ETrue);
    }




void CMsvIndexAdapter::GetVisibleFolderDetailsL(TMsvId aEntryId, CMsvCacheEntry*& aEntry, CMsvCacheVisibleFolder*& aVisibleFolder)
    {
    TDblQueIter<CMsvCacheVisibleFolder> dqIter(iFolderListHeader);
    dqIter.SetToFirst();
    while((aVisibleFolder = dqIter++)!=NULL)
        {
        if(aVisibleFolder->GetEntry(aEntryId, aEntry))
            {
            break;
            }
        }
    if(aEntry==NULL)
        {
        User::Leave(KErrNotFound);
        }
    }




CMsvCacheVisibleFolder* CMsvIndexAdapter::UpdateCacheForMoveEntryL(TMsvId aNewParentId,
                                                                   CMsvCacheEntry*& aOrigEntry, 
                                                                   CMsvCacheVisibleFolder* aOldVisibleFolderNode, 
                                                                   CMsvEntrySelection* aDescendents,
                                                                   TBool& aIsChildEntriesNeedsUpdation)
    {
    // 6. Find the visible folder of new parent.
    TMsvId newVisibleFolderId = aOldVisibleFolderNode->GetFolderId();
    if(!GetVisibleFolderId(aNewParentId, newVisibleFolderId))
        {
        User::Leave(KErrNotFound);
        }
    
    // 7. Check if the visible folders are different.
    CMsvCacheVisibleFolder* newVisibleFolderNode = NULL;
    if(aOldVisibleFolderNode->GetFolderId() != newVisibleFolderId)
        {
        // 7.1. If the visible folders are different, add duplicate
        // entry under new visible folder. Removing entry from old
        // visible folder is done in step 10 of DoMoveEntryL().
        CMsvCacheEntry* newCacheEntry = iFreePoolInstance->EntryL();    
        TRAPD(err, newCacheEntry->DupNDestroyL(aOrigEntry);
                   newVisibleFolderNode = AddEntryToCacheL(newVisibleFolderId, newCacheEntry);
             );
        if(err)
            {
            iFreePoolInstance->ReleaseEntryWithoutTransaction(newCacheEntry);
            User::Leave(err);
            }
    
        // 7.2. Check if child entries also needs to be moved. 
        aOrigEntry = newCacheEntry;
        if(aDescendents && !aOrigEntry->Entry().VisibleFolderFlag())
             {
             aIsChildEntriesNeedsUpdation = ETrue;
             
             // Add duplicate child entry under new visible folder.
             // Removing child entries from old visible folder is done in step 11.
             CMsvCacheEntry* childEntry = NULL;
             CMsvCacheEntry* dupChildEntry = NULL;
             for(TInt index=(aDescendents->Count()-2); index>=0; index--)
                 {
                 if(aOldVisibleFolderNode->GetEntry(aDescendents->At(index), childEntry))
                     {
                     dupChildEntry = NULL;
                     // Create the duplicate of original entry.
                     TRAP(err, dupChildEntry = iFreePoolInstance->EntryL();
                               dupChildEntry->DupNDestroyL(childEntry);
                               // Add under new visible folder node.
                               newVisibleFolderNode->AddEntryL(dupChildEntry);
                          );
                     if(err)
                         {
                         // Delete should not be leaving...
                         newVisibleFolderNode->DeleteEntryL(aOrigEntry->GetId());    // Undo step 7.1
                         iFreePoolInstance->ReleaseEntryWithoutTransaction(dupChildEntry);   // Undo current entry
                         // Undo remaining entries.
                         for(TInt numEntries=index+1; numEntries<=(aDescendents->Count()-2); numEntries++)
                             {
                             newVisibleFolderNode->DeleteEntryL(aDescendents->At(numEntries));
                             }
                         User::Leave(err);
                         }
                     }   // if(oldFolderNode->GetEntry(aDescendents->At(index), childEntry))
                 }   // for()
             }   // if(aDescendents && !entry->Entry().VisibleFolderFlag())
        }
    return newVisibleFolderNode;
    }




/**
 * IsADescendent()
 */
TInt CMsvIndexAdapter::IsADescendent(TMsvId aAscendentId, TMsvId aDescendentId, TBool& aDescendent)
	{
	__ASSERT_DEBUG((KMsvRootIndexEntryId != aAscendentId) && (KMsvRootIndexEntryId != aDescendentId), PanicServer(EMsvDescendentArgumentsRoot));
	__ASSERT_DEBUG(aAscendentId!=aDescendentId, PanicServer(EMsvDescendentArgumentsEqual));
	
	aDescendent=EFalse;
	while (KMsvRootIndexEntryId != aDescendentId)
		{
		CMsvCacheEntry* entry;	
		TRAPD(err, FindEntryL(aDescendentId, entry));
		if (err)
			{
			return KErrNotFound;
			}
		if (aDescendentId==aAscendentId)
			{
			aDescendent=ETrue;
			break;
			}
		aDescendentId=entry->Entry().Parent();
		}
		
	return KErrNone;
	}
	



/**
 * EntryExists()
 */
TBool CMsvIndexAdapter::EntryExists(TMsvId aId)
	{
	// First search the entry in cache, 
	// if cache is not empty...
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	if(KMsvRootIndexEntryId == aId)
		{
		return true;
		}
#endif
	if(!iFolderListHeader.IsEmpty())
		{
		CMsvCacheVisibleFolder* folderNode = NULL;
		TDblQueIter<CMsvCacheVisibleFolder> dqIter(iFolderListHeader);
		dqIter.SetToFirst(); 
		
		while((folderNode = dqIter++)!=NULL)
			{
			if(aId == folderNode->GetFolderId())
				{
				return ETrue;
				}
			if(folderNode->EntryExists(aId))
				{
				return ETrue;
				}
			}
		}
	
	TBool entryExists = EFalse;
	if(iDbAdapter)
		{
		TRAP_IGNORE(entryExists = iDbAdapter->EntryExistsL(aId));
		}
	else
		{
		return EFalse;
		}
	if(!entryExists)
		{
		return EFalse;	
		}
	return ETrue;
	}
	



/**
 * ChangeTemporaryData()
 */
TInt CMsvIndexAdapter::ChangeTemporaryData(const TMsvEntry& aNewEntryContents)
	{
	//__DEBUG_INVARIANT_ONEXIT;
	
	CMsvCacheEntry* entry;	

	// find the entry in the index
	TRAPD(err, FindEntryL(aNewEntryContents.Id(), entry));
	if (err)
		{
		return KErrNotFound;
		}

	// Can only change a locked entry and not one marked as read only
	if (!entry->IsEntryLocked() || entry->Entry().StandardFolder())
		{
		return KErrAccessDenied;
		}

	// Check if just changing temporary flags (ie those not stored on file)
	entry->Entry().iData=aNewEntryContents.iData;

	return KErrNone;	
	}
	



/**
 * ChangeAttributes()
 */
TInt CMsvIndexAdapter::ChangeAttributes(CMsvEntrySelection& aSelection, TUint aSetAttributes, TUint aClearAttributes)
	{
	TRAPD(leave, DoChangeAttributesL(aSelection, aSetAttributes, aClearAttributes));
	return leave;
	}
	


void CMsvIndexAdapter::DoChangeAttributesL(CMsvEntrySelection& aSelection, TUint aSetAttributes, TUint aClearAttributes)
	{
	// Leave if the message store is not currently available
	User::LeaveIfError(iErrorState);

	CArrayFixSeg<TInt32>* newData = new(ELeave)CArrayFixSeg<TInt32>(KMsvChangeAttributesListGranularity);
	CleanupStack::PushL(newData);
	CArrayFixSeg<TInt32>* newPcSyncCount = new(ELeave)CArrayFixSeg<TInt32>(KMsvChangeAttributesListGranularity);
	CleanupStack::PushL(newPcSyncCount);

	newData->ResizeL(aSelection.Count(), 0);
	newPcSyncCount->ResizeL(aSelection.Count(), 0);

	TInt count=aSelection.Count();
	TDblQueIter<CMsvCacheVisibleFolder> dqIter(iFolderListHeader);
	CMsvCacheEntry* entry = NULL;	
	CMsvCacheVisibleFolder* folderNode = NULL;
	TMsvId newVisibleFolderId = NULL;
	if(iDbAdapter)
		{
		iDbAdapter->BeginTransactionL();
		}
	else
		{
		User::Leave(ErrorState());	
		}
	while(count--)
		{
		// find the entry in the index
		dqIter.SetToFirst();
		while((folderNode = dqIter++)!=NULL)
			{
			if(folderNode->GetEntry(aSelection.At(count), entry))
				{
				newVisibleFolderId = folderNode->GetFolderId();
				break;
				}
			}
		if (entry == NULL)
			{
			User::Leave(KErrNotFound);
			}
		// can only change a locked entry and not one marked as read only
		if (!entry->IsEntryLocked() || entry->Entry().StandardFolder())
			{
			User::Leave(KErrAccessDenied);
			}
			
		// work out the new attributes
		TBool persistedFlagsChanged;
		if (!NewAttributes(entry->Entry(), newData->At(count), newPcSyncCount->At(count), aSetAttributes, aClearAttributes, persistedFlagsChanged))
			{
			// attributes don't need changing
			aSelection.Delete(count);
			newData->Delete(count);
			newPcSyncCount->Delete(count);
			continue;
			}
		if(persistedFlagsChanged)
			{
			TMsvEntry tEntry = entry->Entry();
			tEntry.iData = newData->At(count);
			tEntry.iPcSyncCount = newPcSyncCount->At(count);
			TInt err = KErrNone;
			if(iDbAdapter)
				{
				TRAP(err, iDbAdapter->UpdateEntryL(tEntry, newVisibleFolderId, EFalse));
				}
			else
				{
				User::Leave(ErrorState());	
				}
			if(err)
				{
				iDbAdapter->RollbackTransactionL();
				User::Leave(err);
				}
			}
		}
	if(iDbAdapter)
		{
		iDbAdapter->CommitTransactionL();
		}
	else
		{
		User::Leave(ErrorState());	
		}	
	
	count=aSelection.Count();
	while(count--)
		{
		// find the entry in the index
		CMsvCacheEntry* entryPtr = NULL;
		dqIter.SetToFirst();
		while((folderNode = dqIter++)!=NULL)
			{
			if(folderNode->GetEntry(aSelection.At(count), entryPtr))
				{
				break;
				}
			}
#if defined(_DEBUG)
		TBool found = (NULL != entryPtr);
		__ASSERT_DEBUG(found, PanicServer(EMsvChangeAttEmtryNotFound));
#endif
		entryPtr->Entry().iData=newData->At(count);
		entryPtr->Entry().iPcSyncCount=newPcSyncCount->At(count);
		}
	CleanupStack::PopAndDestroy(2, newData);
	if (!iIdle->IsActive())
		{
		iIdle->Start(TCallBack(BackGroundOperations, this));	
		}
		
	}

/**
 * NewAttributes
*/	
TBool CMsvIndexAdapter::NewAttributes(const TMsvEntry& aEntry, TInt32& aNewData, TInt32& aNewPcSyncCount, TUint aSetAttributes, TUint aClearAttributes, TBool& aPersistedFlagsChanged)
	{
	aNewData = aEntry.iData;
	aNewPcSyncCount = aEntry.iPcSyncCount;

	// KMsvPcSyncCountAttribute is a dummy flag that is used to increment/decrement the PC Sync Count
	// The visibility attribute is actually recorded as an 'invisible' flag in the index
	if (aSetAttributes)
		{
		if (aSetAttributes & KMsvPcSyncCountAttribute)
			{
			aNewPcSyncCount++;
			aSetAttributes &= ~KMsvPcSyncCountAttribute;
			}
		
		const TInt32 mask = KMsvSendStateMax << KMsvSendingStateShift;
		if (aSetAttributes & mask)
			{
			aNewData &= ~mask;
			}
			

		aNewData |= aSetAttributes;

		if (aSetAttributes & KMsvVisibilityAttribute)
			{
			aNewData &= ~TMsvEntry::KMsvEntryInvisibleFlag;
			}
		if (aSetAttributes & KMsvPendingDeleteAttribute)
			{
			aNewData |= TMsvEntry::KMsvEntryPendingDeleteFlag;
			}
		}

	if (aClearAttributes)
		{
		if (aClearAttributes & KMsvPcSyncCountAttribute)
			{
			aNewPcSyncCount--;
			aClearAttributes &= ~KMsvPcSyncCountAttribute;
			}

		aNewData &= ~aClearAttributes;

		if (aClearAttributes&KMsvVisibilityAttribute)
			{
			aNewData |= TMsvEntry::KMsvEntryInvisibleFlag;
			}
			
		if (aClearAttributes&KMsvPendingDeleteAttribute)
			{
			aNewData &= ~TMsvEntry::KMsvEntryPendingDeleteFlag;
			}
			
		}

	aPersistedFlagsChanged = (aNewData&TMsvEntry::KMsvEntryPersistedFlags)!=(aEntry.iData&TMsvEntry::KMsvEntryPersistedFlags) ||
								aNewPcSyncCount != aEntry.iPcSyncCount;
	return aNewData != aEntry.iData || aNewPcSyncCount != aEntry.iPcSyncCount;
	}

/**
 * UpdateDates
*/
void CMsvIndexAdapter::UpdateDates(CMsvCacheEntry& aEntry, TBool aSetCreatedDate)
	{
	TTime now;
	now.UniversalTime();
	TMsvTime time;
	time.SetTime(now);
	aEntry.SetLastChangeDate(time);
	
	if (aSetCreatedDate)
		{
		aEntry.SetCreatedDate(time);
		}
	}




/**
 * GetInternalEntry()
 */
TInt CMsvIndexAdapter::GetInternalEntry(TMsvId aId, CMsvCacheEntry*& aEntry)
	{
	CMsvCacheEntry* entry=NULL;
	TRAPD(err, FindEntryL(aId, entry));
	if(err!=KErrNone)
		{
		return err;
		}
	if(entry==NULL)
		{
		return KErrNotFound;
		}
	aEntry=entry;
	return KErrNone;
	}

	
/**
 * Progress()
*/ 
TMsvIndexProgress& CMsvIndexAdapter::Progress()
	{
	return iProgress;
	}
	

/**
 * EntryTreeInfo()
 */
TInt CMsvIndexAdapter::EntryTreeInfo(TMsvId aId, TMsvServerEntryInfo& aEntryInfo)
	{
	aEntryInfo.Reset();
	aEntryInfo.iId = aId;

	if( aId != KMsvRootIndexEntryId )
		{
		// Search up the tree to find which service the specified entry is under.
		// Also, record the highest owning folder for the entry.
		TMsvId id = aId;
		CMsvCacheEntry* entry =NULL;
		TRAPD(err, FindEntryL(id, entry));
		if(err)
			return KErrNotFound;
		
		id = entry->Entry().iId; //Id may be masked in cache
		aEntryInfo.iId = id;
		
		// Get info for the entry.
		aEntryInfo.iEntryOwnerId	= entry->EntryOwnerId();
		aEntryInfo.iMtm				= entry->Entry().iMtm;
		aEntryInfo.iType			= entry->Entry().iType;
		
		if( aEntryInfo.iType == KUidMsvFolderEntry )
			{
			// This is a folder - record as this may be needed.
			aEntryInfo.iTopFolder = id;
			}
		
		TBool found = (aEntryInfo.iType == KUidMsvServiceEntry);
		while( !found )
			{
			// Move up the tree and get the next entry...
			id = entry->Entry().Parent();
			TRAPD(err, FindEntryL(id, entry));
			if(err)
			 return KErrNotFound;
			
			TUid type = entry->Entry().iType;
			if( type == KUidMsvServiceEntry )
				{
				// Ok, we've found the owning service - stop looking.
				found = ETrue;
				}
			else if( type == KUidMsvFolderEntry )
				{
				// This is a folder - record as this may be needed.
				aEntryInfo.iTopFolder = id;
				}
			else 
				{
				// This is a message or attachment entry - this implies that the
				// original entry was part of an existing message. Set the owner
				// ID to the highest parent.
				aEntryInfo.iPartOfMessage	= ETrue;
				aEntryInfo.iParentOwnerId	= entry->EntryOwnerId();
				}
			}
		// Record the service and MTM...
		aEntryInfo.iService = id;

		aEntryInfo.iServiceMtm = entry->Entry().iMtm;
		}
	else
		{
		// This is the root entry! Mark the service as being the local service.
		aEntryInfo.iType		= KUidMsvRootEntry;
		aEntryInfo.iMtm			= KUidMsvLocalServiceMtm;
		aEntryInfo.iService		= KMsvLocalServiceIndexEntryId;
		aEntryInfo.iServiceMtm	= KUidMsvLocalServiceMtm;
		aEntryInfo.iTopFolder	= KMsvRootIndexEntryId;
		}
	return KErrNone;
	}
	



/**
 * CommitNonCommitedEntries()
 */
void CMsvIndexAdapter::CommitNonCommitedEntries()
	{
	// 1. Commit transaction in DB.
	if(iDbAdapter)
		{
		TRAPD(err, iDbAdapter->CommitTransactionL());
		if(err)
		    {
		    RollbackAdditions();
		    RollbackChanges();
		    return;
		    }
		}

	// 2. Update changed entries completely.
	TInt count = iNonCommittedChangedEntryList.Count();
	while(count)
		{
		TNonCommittedChangedEntries changedEntry = iNonCommittedChangedEntryList[--count];
		CMsvCacheVisibleFolder* oldVisibleFolderNode = changedEntry.iOldVisibleFolderNode;
        // Removing entry from old visible folder.
        if(changedEntry.iNewVisibleFolderNode &&
           oldVisibleFolderNode->GetFolderId() != changedEntry.iNewVisibleFolderNode->GetFolderId())
            {
            // Will never leave.
            TRAP_IGNORE(changedEntry.iOldVisibleFolderNode->DeleteEntryL(changedEntry.iEntry->GetId()));
            }
        
        // Removing child entries from old visible folder.
        if(changedEntry.iDescendentList)
            {
            for(TInt index=(changedEntry.iDescendentList->Count()-2); index>=0; index--)
                {
                // Can leave with KErrNotFound.
                TRAP_IGNORE(oldVisibleFolderNode->DeleteEntryL(changedEntry.iDescendentList->At(index)));
                }
            delete changedEntry.iDescendentList;            
            }
    
        // Add child id to new parent child array.
        if(changedEntry.iNewParentEntry && (changedEntry.iNewParentEntry->ChildIdArray()))
            {
            // This will not leave, as memory is already reserved.
            TRAP_IGNORE(changedEntry.iNewParentEntry->ChildIdArray()->AppendL(changedEntry.iEntry->GetId()));
            }
        
        // Remove child from old parent's child array.
        if(changedEntry.iOldParentEntry)
            {
            RArray<TMsvId>* oldParentChildArr = changedEntry.iOldParentEntry->ChildIdArray();
            if(oldParentChildArr)
                {
                TInt pos = oldParentChildArr->Find(changedEntry.iEntry->GetId());
                if(pos != KErrNotFound)
                    {
                    oldParentChildArr->Remove(pos);
                    }
                }
            }
    
        // Update owner flag of parent entries.
        if(changedEntry.iResetOldParentOwnerFlag)
            {
            changedEntry.iOldParentEntry->Entry().SetOwner(EFalse);
            }
        if(changedEntry.iNewParentEntry)
            {
            changedEntry.iNewParentEntry->Entry().SetOwner(ETrue);
            }
		}

	// 3. Reset non-committed entry list.
	iNonCommittedAddedEntryList.ResetAndDestroy();
	iNonCommittedChangedEntryList.Reset();
	}



/**
 * RollbackAdditions()
 */
void CMsvIndexAdapter::RollbackAdditions()
	{
	// 1. Rollback transaction in DB.
	if(iDbAdapter)
		{
		TRAP_IGNORE(iDbAdapter->RollbackTransactionL());
		}
	
	// 2. Delete added entry from cache.
	CMsvCacheVisibleFolder* folderNode = NULL;
	TInt count = iNonCommittedAddedEntryList.Count();
	TDblQueIter<CMsvCacheVisibleFolder> dqIter(iFolderListHeader);
	while(count)
		{
		--count;
		dqIter.SetToFirst();
	    while ((folderNode = dqIter++) != NULL)
	        {
	        if(iNonCommittedAddedEntryList[count]->iVisibleFolder == folderNode->GetFolderId())
	           	{
	           	iNonCommittedAddedEntryList[count]->entry->LockEntry();
	        	// Error can cause if the entry does not exist. Ignore.
	        	TRAP_IGNORE(folderNode->DeleteEntryL(iNonCommittedAddedEntryList[count]->entry->GetId()));
	        	break;
	        	}
	        };
		}
	
	// 3. Reset non-committed entry list.
	iNonCommittedAddedEntryList.ResetAndDestroy();
	}





/**
 * RollbackChanges()
 */
void CMsvIndexAdapter::RollbackChanges()
	{
	// 1. Rollback transaction in DB.
	if(iDbAdapter)
		{
		TRAP_IGNORE(iDbAdapter->RollbackTransactionL());
		}
	
	// 2. Undo changed entry in cache.
	TInt count = iNonCommittedChangedEntryList.Count();
	while(count)
		{
		TNonCommittedChangedEntries changedEntry = iNonCommittedChangedEntryList[--count];
		CMsvCacheEntry* oldEntry = changedEntry.iEntry;

		// If the visible folder Id of the entry is changed, 
		// oldEntry represents the entry under new visible folder node.
		// Get the actual old entry from old visible folder node.
		if(changedEntry.iNewVisibleFolderNode)
		    {
		    // This will be non leaving as entry is already present.
		    if(changedEntry.iOldVisibleFolderNode->GetEntry(oldEntry->GetId(), oldEntry))
				{
				// Remove added entry from new visible node. 
				// This function will never leave in the current 
				// scenario, hence ignoring the error.
				TRAP_IGNORE(changedEntry.iNewVisibleFolderNode->DeleteEntryL(oldEntry->GetId()));
				}
		    }
        oldEntry->SetEntryOwnerId(changedEntry.iBkpEntry->EntryOwnerId());
        oldEntry->RollBackCopyEntry(changedEntry.iBkpEntry->Entry());
        iFreePoolInstance->ReleaseEntryWithoutTransaction(changedEntry.iBkpEntry);
        
        if(changedEntry.iDescendentList)
            {
            delete changedEntry.iDescendentList;
            }
 		}
        
	// 3. Reset non-committed entry list.
	iNonCommittedChangedEntryList.Reset();
	}




CMsvIndexAdapter::TMsvServerEntryInfo::TMsvServerEntryInfo()
	{
	iId				= KMsvNullIndexEntryIdValue;
	iTopFolder		= KMsvNullIndexEntryIdValue;
	iService		= KMsvNullIndexEntryIdValue;
	iMtm			= KUidMsvNullEntry;
	iType			= KUidMsvNullEntry;
	iServiceMtm		= KUidMsvNullEntry;
	iEntryOwnerId	= KMsvServerId.iId; 
	iParentOwnerId	= KMsvServerId.iId;
	iPartOfMessage	= EFalse;	
	}




void CMsvIndexAdapter::TMsvServerEntryInfo::Reset()
	{
	iId				= KMsvNullIndexEntryIdValue;
	iTopFolder		= KMsvNullIndexEntryIdValue;
	iService		= KMsvNullIndexEntryIdValue;
	iMtm			= KUidMsvNullEntry;
	iType			= KUidMsvNullEntry;
	iServiceMtm		= KUidMsvNullEntry;
	iEntryOwnerId	= KMsvServerId.iId; 
	iParentOwnerId	= KMsvServerId.iId;
	iPartOfMessage	= EFalse;	
	}	


/**
Need to see the posibilty of getting this first from cache
*/
TBool CMsvIndexAdapter::GetNextSiblingL(TMsvId aId,TMsvId aParentId,TMsvId& aNextSiblingId)
	{
	TBool flag = EFalse;
	if(iDbAdapter)
		{
		flag = iDbAdapter->GetNextSiblingL(aId, aParentId, aNextSiblingId);
		}
	else
		{
		User::Leave(ErrorState());	
		}
	return flag;
	}


/**
Need to see the posibilty of getting this first from cache
*/
TBool CMsvIndexAdapter::GetFirstChildIdL(TMsvId aParentId,TMsvId& aFirstChild)
	{
	TBool flag = EFalse;
	if(iDbAdapter)
		{
		flag = iDbAdapter->GetFirstChildIdL(aParentId, aFirstChild);
		}
	else
		{
		User::Leave(ErrorState());	
		}
	return flag;
	}


/**
 * SetLocalServiceComplete
 * @param TUint: Drive Id of the local service folder node.
 *
 * Code changes for PREQ 557.
 * Sets the localservice visiblefolder.
 */	
void CMsvIndexAdapter::SetLocalServiceComplete()
	{
	TDblQueIter<CMsvCacheVisibleFolder> dqIter(iFolderListHeader);
	dqIter.SetToFirst();
	CMsvCacheVisibleFolder* folderNode = NULL;

	while((folderNode = dqIter++)!=NULL)
		{
		if(KMsvLocalServiceIndexEntryIdValue == folderNode->GetFolderId())
			{
			folderNode->SetComplete(ETrue);
			break;
			}
		}
	}

	
/**
 *BackGroundOperations()
 *@param  aPtr 	A TAny*
 *@return TBool ETrue if the background operation needs to be rescheduled.
 *				EFalse if the background operation need not be rescheduled.
 */
TBool CMsvIndexAdapter::BackGroundOperations(TAny* aPtr)
	{
	return ((CMsvIndexAdapter*)aPtr)->DoBackGroundOperations();
	}





/**
 *DoBackGroundOperations()
 *@param  None
 *@return TBool ETrue if the background operation needs to be rescheduled.
 *				EFalse if the background operation need not be rescheduled.
 */
TBool CMsvIndexAdapter::DoBackGroundOperations()
	{
	TInt stopBackgroundOperation = EFalse;
	TRAPD(err, DoBackGroundOperationsL(stopBackgroundOperation));
	if(KErrNone == err && !stopBackgroundOperation)
		{
		return ETrue;
		}
	return EFalse;	
	}





/**
 *DoBackGroundOperations()
 *This is the background operation state machine, this will perform a set
 *of operations in each of the iteration.
 *
 *@param  None
 *@return void
 */
void CMsvIndexAdapter::DoBackGroundOperationsL(TBool& aStopBackgroundOperation)
	{
	iFreePoolInstance->RoutineFreePoolCleanUpL();
	switch(iBackgroundOperationState)
		{
		case ERemoveDeletedEntries:
			{
			iBackgroundOperationState = EAllocateMemoryOperation;
			DoRemoveDeletedEntriesL();
			break;
			}
		case EAllocateMemoryOperation:
			{
			iBackgroundOperationState = ECheckBlockSize;
			if(CheckAndAllocateMemoryL())
				{
				iBackgroundOperationPerformed = 0;
				break;	
				}
			iBackgroundOperationPerformed++;	
			}
		case ECheckBlockSize:
			{
			iBackgroundOperationState = ENoOperation;	
			SplitBlockL();
			break;
			}
		case ENoOperation:
		default:
			{
			iBackgroundOperationState = ERemoveDeletedEntries;
			// The background operation has run multiple times, without doing
			// much work, so it can be stopped temporarily.
			if(iBackgroundOperationPerformed >= 5)
				{
				iBackgroundOperationPerformed = 0;
				aStopBackgroundOperation = ETrue;
				}
				
			break;
			}		
		}

	}



/**
 *CheckAndAllocateMemory
 *Will check the state of the free pool and allocate memory if required.
 *Allocation will be done if 70% of the CMsvCacheEntries created are used.
 *
 *@return  TBool    ETrue,  if allocation has been done.
 *		   			EFalse, if no allocation is done.			
 */
TBool CMsvIndexAdapter::CheckAndAllocateMemoryL()
	{
	if(iFreePoolInstance->IsAllocationRequiredL())
		{
		iFreePoolInstance->AllocateMemoryL();
		return ETrue;
		}
	return EFalse;	
	}
	


	
/**
 *SplitBlockL
 *Will sort and split the blocks of CMsvCacheEntries.
 *
 *@return  void     		
 */	
void CMsvIndexAdapter::SplitBlockL()
	{
	TDblQueIter<CMsvCacheVisibleFolder> dqIter(iFolderListHeader);
	dqIter.SetToFirst();
	CMsvCacheVisibleFolder* folderNode = NULL;
	while((folderNode = dqIter++)!=NULL)
		{
		folderNode->SplitBlockL();
		}
	}



/**
 *ForceDeleteEntry
 *Deletes the TMsvEntry even if it is not in the cache.This does not require 
 *the entry to be locked.
 *
 *@param aId  TMsvId, id ofthe entry to be deleted.
 *@return TInt Any of the system wide error codes.
 */
TInt CMsvIndexAdapter::ForceDeleteEntry(TMsvId aId)
	{
	TRAPD(leave, DoForceDeleteEntryL(aId));
	return leave;
	}



/**
 *DoForceDeleteEntryL
 *Called from ForceDeleteEntry
 *
 *@param aId  TMsvId, id ofthe entry to be deleted.
 *@return void 
 */
void CMsvIndexAdapter::DoForceDeleteEntryL(TMsvId aId)
	{
	User::LeaveIfError(iErrorState);
	CMsvCacheVisibleFolder* folderNode = NULL;
	TDblQueIter<CMsvCacheVisibleFolder> dqIter(iFolderListHeader);
	
	if(!iDbAdapter)
		{
		User::Leave(ErrorState());
		}
	CMsvCacheEntry* entry = NULL;
	TBool releaseEntry = FindEntryL(aId, entry, EFalse);
	TMsvId parentId = entry->Entry().Parent();
	TRAPD(err, iDbAdapter->DeleteEntryL(aId));
	// If the entry does not exists in cache,
	// we need to release the entry explicitly.
	if(releaseEntry)
		{
		iFreePoolInstance->ReleaseEntryL(entry, ETrue);
		User::LeaveIfError(err);
		}
	else
		{
		User::LeaveIfError(err);
		// Otherwise remove the entry from cache.
		dqIter.SetToFirst();
		while((folderNode = dqIter++)!=NULL)
			{
			TRAPD(err, folderNode->DeleteEntryL(aId, ETrue));	
			if(KErrNone == err)
				{
			 	break;
			 	}
			}
		}
	//Find the parent entry in cache and check for any children.
	//If there are no more children under the parent, then
	//reset the owner flag in cache and database.
	CMsvEntrySelection* children = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(children);
	releaseEntry = FindEntryL(parentId, entry, EFalse);
	TRAP(err, 
		GetChildrenIdL(parentId, *children);
		if(0 == children->Count() && entry->Entry().Owner())
			{
			iDbAdapter->UpdateOwnerStatusL(parentId, entry->Entry(), EFalse);
			entry->Entry().SetOwner(EFalse);
			}
		);
	if(releaseEntry)
		{
		iFreePoolInstance->ReleaseEntryL(entry, ETrue);
		}
	User::LeaveIfError(err);
	CleanupStack::PopAndDestroy(); //children
	}



/**
 *DeleteSelectionUsingTransaction
 *Deletes a selection of TMsvEntries.This will also delete enteries that are not locked.
 *
 *@param aSelection  CMsvEntrySelection&, selection of the TMsvEntries.
 @param TInt Any of the system wide error codes.
 */

TInt CMsvIndexAdapter::DeleteSelectionUsingTransaction(const CMsvEntrySelection& aSelection)
	{
	TRAPD(leave, DoDeleteSelectionUsingTransactionL(aSelection));
	return leave;
	}


/**
 *DoDeleteSelectionUsingTransactionL
 *Called from DeleteSelectionUsingTransaction.
 *
 *@param aSelection  CMsvEntrySelection&, selection of the TMsvEntries.
 @param void
 */
void CMsvIndexAdapter::DoDeleteSelectionUsingTransactionL(const CMsvEntrySelection& aSelection)
	{
    User::LeaveIfError(iErrorState);
    if(!iDbAdapter)
        {
        User::Leave(iErrorState);
        }
    
    CMsvCacheVisibleFolder* folderNode = NULL;
    TDblQueIter<CMsvCacheVisibleFolder> dqIter(iFolderListHeader);
    const TInt count = aSelection.Count();
    
    if(!count)
        {
        return;
        }   
        
    CMsvEntrySelection* children = new(ELeave)CMsvEntrySelection;
    CleanupStack::PushL(children);
    CMsvCacheEntry* entry = NULL;
    TBool releaseEntry = FindEntryL(aSelection.At(0), entry, EFalse);
    TMsvId parentId = entry->Entry().Parent();
    if(releaseEntry)
        {
        iFreePoolInstance->ReleaseEntryL(entry, ETrue);
        }

    // Find the parent in the cache - we may need to update its owner status flag both in cache and DB.
    TBool updateTheParent = ETrue;
    TRAPD(err, releaseEntry = FindEntryL(parentId, entry, EFalse)); // can't let it leave - need to delete the child in any case.
    if(err == KErrNotFound)
        {
        updateTheParent = EFalse;
        if(releaseEntry)
            {
            iFreePoolInstance->ReleaseEntryL(entry, ETrue);
            }
        }
    else
        {
        // Get parent's children.
        GetChildrenIdL(parentId, *children); // can leave - we'll let it leave as there's nothing to be rolled back now.
        if(children->Count() > 1)
            updateTheParent = EFalse;
        }
        
    // Perform DB operations:
    //      - Delete the entry.
    //      - Update owner status flag of the parent.
    iDbAdapter->DeleteEntryL(aSelection); // can leave - we'll let it leave as there's nothing to be rolled back now.
    if(updateTheParent)
        {
        TRAP(err, iDbAdapter->UpdateOwnerStatusL(parentId, entry->Entry(), EFalse));
        }
    // Leave if DB operations fail. Cache is not touched.
    if(err)
        {
        TRAP_IGNORE(iDbAdapter->RollbackTransactionL()); //
        User::Leave(err);
        }
    else
        {
        if(updateTheParent)
            {
            entry->Entry().SetOwner(EFalse);
            }
        // Do the cache operations. These don't require any memory allocation and cannot fail.
        for (TInt index=0; index<count; ++index)
            {
            // If the entry is a visible folder, then delete the node.
            dqIter.SetToFirst(); 
            while((folderNode = dqIter++)!=NULL)
                {
                if(folderNode->GetFolderId() == aSelection.At(index))
                    {
                    folderNode->iDlink.Deque();
                    delete folderNode;
                    break;
                    }
                }
            
            // Delete the entry from under its parent visible folder.
            dqIter.SetToFirst();
            while((folderNode = dqIter++)!=NULL)
                {
                TRAPD(err, folderNode->DeleteEntryL(aSelection.At(index), ETrue));  
                if(KErrNone == err) 
                    {
                    break;
                    }
                }
            }
        }
    
    // Update SearchSort delta cache.
    TMsgType aType(EDeletedMsg);
    if(CMSvSearchSortCacheManager::Instance()->iManagerEntry != NULL)
        {   
        if(CMSvSearchSortCacheManager::Instance()->iManagerEntry->Count()>0)
            {
            for(TInt ii =0; ii < aSelection.Count() ; ii ++)
                {
                CMsvSearchSortDeltaCache::Instance()->EntryInDeltaCache(aSelection[ii],aType);
                }
            }
        }

    CleanupStack::PopAndDestroy(); //children
	}



/**
 * BeginTransaction()
 * 
 * Starts a new DB transaction.
 */
void CMsvIndexAdapter::BeginTransaction()
	{
	if(iDbAdapter)
		{
		TRAP_IGNORE(iDbAdapter->BeginTransactionL());
		}
	}



/**
 * CommitTransaction()
 * 
 * Commits an already opened transaction.
 */
void CMsvIndexAdapter::CommitTransaction()
	{
	if(iDbAdapter)
		{
		TRAP_IGNORE(iDbAdapter->CommitTransactionL());
		}
	}
	


/**
 * DoRemoveDeletedEntriesL()
 * 
 * Deletes all the entries whose PcSyncCount is 0 and delete flag is set.
 */
TBool CMsvIndexAdapter::DoRemoveDeletedEntriesL()
	{
	CMsvCacheEntry* parent = NULL;
	CMsvCacheEntry* entry = NULL;
	CMsvCacheEntry* siblingEntry = NULL;
	TMsvId siblingId = KMsvNullIndexEntryId;
	TMsvId visibleFolder = KMsvNullIndexEntryId;
	TMsvId firstchildId = KMsvNullIndexEntryId;
	TInt commitCount = 1;
	
	// Find deleted folder
	FindEntryL(KMsvDeletedEntryFolderEntryId, parent, ETrue);

	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

	// All deleted entries are a child of deleted folder
	if(iDbAdapter->GetFirstChildIdL(parent->Entry().Id(),firstchildId))
		{
		iDbAdapter->GetEntryL(firstchildId, entry, visibleFolder);
		}
					
	while(entry && commitCount > 0)
		{
		if (entry->Entry().PcSyncCount() <= 0)
			{
			// Add entry to list of things to be deleted
			__ASSERT_DEBUG(entry->Entry().Deleted(), PanicServer(EMsvDeletedFlagNotSet));
			
			User::LeaveIfError(LockEntryAndStore(entry->Entry().Id()));
			selection->AppendL(entry->Entry().Id());
			// Will the next entry be deleted?
			if(iDbAdapter->GetNextSiblingL(entry->Entry().Id(), parent->Entry().Id(), siblingId))
				{
				iDbAdapter->GetEntryL(siblingId, siblingEntry, visibleFolder);
				if(siblingEntry->Entry().PcSyncCount() > 0)
					{
					commitCount--;
					}
				}
			else
			    break;
			}
		iFreePoolInstance->ReleaseEntryWithoutTransaction(entry);
		entry = siblingEntry;
		}

	if (selection->Count() > 0)
		{
		User::LeaveIfError(DeleteSelection(*selection));
		}
	CleanupStack::PopAndDestroy(); // selection
	
	// Possibly more to delete if we did not get to the end of the child list
	TBool retVal = entry != NULL;
	iFreePoolInstance->ReleaseEntryWithoutTransaction(entry);
	return retVal;
	}




#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)

/**
 * GetChildrenAllL()
 *
 * Code added for PREQ 557.
 *
 * The function is similar to function GetChildrenL(), but
 * additionally it fetches entries from all the drive in the
 * preferred drive list.
 */
void CMsvIndexAdapter::GetChildrenAllL(TMsvId aId, CArrayPtr<const TMsvEntry>& aSelection, const TMsvSelectionOrdering& aOrdering, TUid aMtm, TBool aFilterByOwnerId, TSecureId aOwnerId)
	{
	TMsvPreferredDrive driveEntry;
	CMsvPreferredDriveList *driveList = CMsvPreferredDriveList::GetDriveList();
	for(TInt index=0; index<driveList->Count(); ++index)
		{
		driveEntry = (*driveList)[index];
		if(EMsvMessageStoreAvailableStatus == driveEntry.status)
			{
			GetChildrenL(MaskTMsvId(driveEntry.driveId, aId), aSelection, aOrdering, aMtm, aFilterByOwnerId, aOwnerId);
			}
		}
	}




/**
 * GetChildrenIdAll()
 *
 * Code added for PREQ 557.
 *
 * The function is similar to function GetChildrenId(), but
 * additionally it fetches entries from all the drive in the
 * preferred drive list.
 */
TInt CMsvIndexAdapter::GetChildrenIdAll(TMsvId aId, const CMsvEntryFilter& aFilter, CMsvEntrySelection& aSelection)
	{
	TSecureId dummy = KMsvServerId.iId;
	return GetChildrenIdAll(aId, aFilter, aSelection, EFalse, dummy);
	}
	
	 

// Overloaded.
TInt CMsvIndexAdapter::GetChildrenIdAll(TMsvId aId, const CMsvEntryFilter& aFilter, CMsvEntrySelection& aSelection, TBool aFilterByOwnerId, TSecureId aOwnerId)
	{
	TInt err = KErrNone;
	TMsvPreferredDrive driveEntry;
	CMsvPreferredDriveList *driveList = CMsvPreferredDriveList::GetDriveList();
	for(TInt index=0; index<driveList->Count(); ++index)
		{
		driveEntry = (*driveList)[index];
		if(EMsvMessageStoreAvailableStatus == driveEntry.status)
			{
			err = GetChildrenId(MaskTMsvId(driveEntry.driveId, aId), aFilter, aSelection, aFilterByOwnerId, aOwnerId);
			if(KErrNone != err)
				{
				return err;
				}
			}
		}
	return KErrNone;
	}
	


/**
 * RemoveDriveL()
 * 
 * This function is added in PREQ 557.
 * @param TUint: The drive priority, which is also the index of drive 
 *               entry in message server data structure.
 * @param TBool: If the drive still remains in the preferred drive list.
 * @return None.
 * 
 */
void CMsvIndexAdapter::RemoveDriveL(TUint aDriveId, TUint aDriveIndex, TBool aIsStdFolderVisible /*=ETrue */)
	{
	CMsvCacheVisibleFolder* folderNode = NULL;
	TDblQueIter<CMsvCacheVisibleFolder> dqIter(iFolderListHeader);

	// Browse through the folder list.
    dqIter.SetToFirst();
    while ((folderNode = dqIter++) != NULL)
        {
        // If the folder is from the same drive.
        // Caution: If the current drive is removed, 
        // this will also include root and localService.
        if(folderNode->GetDrive() == aDriveId)
        	{
       				folderNode->iDlink.Deque();
       				delete folderNode;
        			}
        }	// while ((folderNode = dqIter++) != NULL)
    
    // If current drive is removed, 
    // Remove localService entry as well.

	// Detach the database and remove maxId entry.   	
    if(!aIsStdFolderVisible)
    	{
    	iDbAdapter->DetachDBL(aDriveId);
    	iMaxMsvIdList[aDriveId] = NULL;

		CMsvPreferredDriveList::GetDriveList()->UpdateDriveIdL(aDriveIndex, KMsvInvalidDriveId);
		CMsvPreferredDriveList::GetDriveList()->UpdateDriveStatusL(aDriveIndex, EMsvDriveDiskNotAvailableStatus);
		}
	}





/**
 * AddDriveL()
 * 
 * This function is added in PREQ 557.
 * @param TUint: The drive priority, which is also the index of drive 
 *               entry in message server data structure.
 * 
 */
void CMsvIndexAdapter::AddDriveL(TUint aDrivePriority)
	{
	// Assign a new drive id.
	TUint driveId = GetNextAvailableDriveId();
	CMsvPreferredDriveList::GetDriveList()->UpdateDriveIdL(aDrivePriority, driveId);
	
	// Attach the database.
	TMsvId maxId;
	TMsvPreferredDrive driveEntry;
	CMsvPreferredDriveList::GetDriveList()->DriveInfoL(aDrivePriority, driveEntry);
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)	
	TRAPD(err, iDbAdapter->AttachDBL(driveEntry.driveNum, driveId, maxId, &iServer.MessageDBAdapter()));
#else
	TRAPD(err, iDbAdapter->AttachDBL(driveEntry.driveNum, driveId, maxId));
#endif
	WrapDBErrorL(err);
	
	// Insert the max Id in the list.
	iMaxMsvIdList[driveId] = ((maxId >= MaskTMsvId(driveId, KFirstFreeEntryId))? (maxId+1) : MaskTMsvId(driveId, KFirstFreeEntryId));
	}




/**
 * ChangeDriveL()
 * 
 * This function is added in PREQ 557.
 * @param TUint: The drive priority, which is also the index of drive 
 *               entry in message server data structure.
 * @param TBool: If the previous drive still remains in the preferred drive list.
 * @return None.
 * 
 */
void CMsvIndexAdapter::ChangeDriveL(TUint aNewDriveIndex, TBool aIsStdFolderVisible /*= ETrue*/)
	{
	// Rule1: Remove all non-standard folder node of current drive.
	TInt oldCurrentDriveIndex = CMsvPreferredDriveList::GetDriveList()->CurrentDriveIndex();
	RemoveDriveL(KCurrentDriveId, oldCurrentDriveIndex, EFalse);

	// If old drive should be visible, re-attach the drive.
	// This assigns a new drive id to the old current drive.
	if(aIsStdFolderVisible)
		{
		CMsvPreferredDriveList::GetDriveList()->UpdateDriveStatusL(oldCurrentDriveIndex, EMsvMessageStoreAvailableStatus);
		AddDriveL(oldCurrentDriveIndex);
		}

	
	// Clear search sort cache db table
	TRAPD(err, GetDbAdapter()->ClearDBContentsL());
	WrapDBErrorL(err);
	
	
	// 3. Flush up entries from new current drive, 
	// if it is already attached.
	
	TMsvPreferredDrive driveEntry;
	CMsvPreferredDriveList::GetDriveList()->DriveInfoL(aNewDriveIndex, driveEntry);	
	if(KMsvInvalidDriveId != driveEntry.driveId)
		{
		RemoveDriveL(driveEntry.driveId, aNewDriveIndex, EFalse);
		}
	

	// Assign a new drive Id to the drive.
	CMsvPreferredDriveList::GetDriveList()->UpdateDriveIdL(aNewDriveIndex, KCurrentDriveId);
	CMsvPreferredDriveList::GetDriveList()->UpdateDriveStatusL(aNewDriveIndex, EMsvMessageStoreAvailableStatus);
	

	// Assign a new drive Id to the drive.
	CMsvPreferredDriveList::GetDriveList()->UpdateDriveIdL(aNewDriveIndex, KCurrentDriveId);
	CMsvPreferredDriveList::GetDriveList()->UpdateDriveStatusL(aNewDriveIndex, EMsvMessageStoreAvailableStatus);
	// --------- Attach the database and update MaxId.		
	TMsvId maxId;
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	TRAP(err, iDbAdapter->AttachDBL(driveEntry.driveNum, KCurrentDriveId, maxId, &iServer.MessageDBAdapter()));
#else
	TRAP(err, iDbAdapter->AttachDBL(driveEntry.driveNum, KCurrentDriveId, maxId));
#endif
	WrapDBErrorL(err);
	
	// Insert the max Id in the list.
	iMaxMsvIdList[KCurrentDriveId] = ((maxId >= MaskTMsvId(KCurrentDriveId, KFirstFreeEntryId))? (maxId+1) : MaskTMsvId(KCurrentDriveId, KFirstFreeEntryId));		


	// ------- Create a root node and its children.
	TMsvId folderId = MaskTMsvId(KCurrentDriveId, KMsvRootIndexEntryId);
	
	// Add local service folder node in the folder list.
	iRootNode = CMsvCacheVisibleFolder::NewL(folderId);
	iFolderListHeader.AddFirst(*iRootNode);
	
	// Fetch children of root folder to cache.
	RPointerArray<CMsvCacheEntry> childEntryList;
	CleanupClosePushL(childEntryList);
	
	TRAP(err, iDbAdapter->GetChildrenL(folderId, childEntryList));
	WrapDBErrorL(err);
	iRootNode->AddEntryListL(childEntryList, ETrue);
	childEntryList.Reset();	

	
	// ------- Create a local service node and its children.
	folderId = MaskTMsvId(KCurrentDriveId, KMsvLocalServiceIndexEntryId);
	
	// Add local service folder node in the folder list.
	CMsvCacheVisibleFolder* localServiceFolder = CMsvCacheVisibleFolder::NewL(folderId);
	iFolderListHeader.AddFirst(*localServiceFolder);
	
	TRAP(err, iDbAdapter->GetChildrenL(folderId, childEntryList));
	WrapDBErrorL(err);
	localServiceFolder->AddEntryListL(childEntryList, ETrue);

	CleanupStack::PopAndDestroy();   // childEntryList.	
	}



/**
 * GetNextAvailableDriveId()
 * 
 * This function is added in PREQ 557.
 * @param None: 
 * @return TUint: The next available drive Id.
 * 
 */
TUint CMsvIndexAdapter::GetNextAvailableDriveId()
	{
	TMsvPreferredDrive driveEntry;
	TUint driveId = iFirstFreeDriveId;
	for(TUint index=0; index<8; index++)
		{
		TBool driveIdFound = EFalse;
		CMsvPreferredDriveList *driveList = CMsvPreferredDriveList::GetDriveList();
		for(TInt index=0; index<driveList->Count(); ++index)
			{
			driveEntry = (*driveList)[index];
			if(driveId == driveEntry.driveId)
				{
				driveIdFound = ETrue;
				break;
				}
			}
		if(EFalse == driveIdFound)
			{
			iFirstFreeDriveId = ((driveId)%7 ? (driveId+1) : 2);
			return driveId;
			}
		driveId = ((driveId)%7 ? (driveId+1) : 2);
		}
	return KErrNone;
	}
	
	

/**
 * ReloadCacheL()
 * 
 * This function is used by backup and restore functionality
 * when it finds that DB being restored is changed. The function
 * destroys the cache of the current drive and recreates it.
 */
void CMsvIndexAdapter::ReloadCacheL()
	{
	TUint currDriveIndex = CMsvPreferredDriveList::GetDriveList()->CurrentDriveIndex();
	TDriveNumber currDriveNum = CMsvPreferredDriveList::GetDriveList()->CurrentDriveNumber();

	// Attach the database.
	TMsvId maxId;
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	TRAPD(err, iDbAdapter->AttachDBL(currDriveNum, KCurrentDriveId, maxId, &iServer.MessageDBAdapter()));
#else
	TRAPD(err, iDbAdapter->AttachDBL(currDriveNum, KCurrentDriveId, maxId));
#endif
	WrapDBErrorL(err);
	
	// Update the entry in drive list.
	CMsvPreferredDriveList::GetDriveList()->UpdateDriveStatusL(currDriveIndex, EMsvMessageStoreAvailableStatus);

	// Insert the max Id in the list.
	iMaxMsvIdList[KCurrentDriveId] = ((maxId >= MaskTMsvId(KCurrentDriveId, KFirstFreeEntryId))? (maxId+1) : MaskTMsvId(KCurrentDriveId, KFirstFreeEntryId));		

	TMsvId folderId = MaskTMsvId(KCurrentDriveId, KMsvRootIndexEntryId);
	
	// Add local service folder node in the folder list.
	iRootNode = CMsvCacheVisibleFolder::NewL(folderId);
	iFolderListHeader.AddFirst(*iRootNode);
	
	// Fetch children of root folder to cache.
	RPointerArray<CMsvCacheEntry> childEntryList;
	CleanupClosePushL(childEntryList);
	
	// Updating children under root node.	
	TRAP(err, iDbAdapter->GetChildrenL(folderId, childEntryList));
	WrapDBErrorL(err);
	iRootNode->AddEntryListL(childEntryList, ETrue);
	childEntryList.Reset();	

	// Create a local service node and its children.
	folderId = MaskTMsvId(KCurrentDriveId, KMsvLocalServiceIndexEntryId);
	
	// Add local service folder node in the folder list.
	CMsvCacheVisibleFolder* localServiceFolder = CMsvCacheVisibleFolder::NewL(folderId);
	iFolderListHeader.AddFirst(*localServiceFolder);
	
	// Fetch children of localService folder to cache.
	TRAP(err, iDbAdapter->GetChildrenL(folderId, childEntryList));
	WrapDBErrorL(err);
	localServiceFolder->AddEntryListL(childEntryList, ETrue);

	CleanupStack::PopAndDestroy();   // childEntryList.
	}
#endif 		// #if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)




#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
#ifdef _DEBUG
void CMsvIndexAdapter::PrintL()
	{
	_LIT8(KFolderId, "FOLDER ID: ");
	_LIT8(KDriveId, "      DRIVE ID: ");
	_LIT8(KComplete, "      COMPLETE FLAG: ");
	_LIT8(KFalse, "FALSE.");
	_LIT8(KTrue, "TRUE.");
	
	RFileLogger logger;
	CleanupClosePushL(logger);
	if (logger.Connect() == KErrNone)
		{
		logger.CreateLog(_L("msgs"), _L("Cache.txt"), EFileLoggingModeAppend);
		logger.SetDateAndTime(EFalse, EFalse);
		logger.Write(_L(" Message Index Cache Structure."));
		logger.Write(_L("--------------------------------"));
		logger.Write(_L(""));
		}
	
	CMsvCacheVisibleFolder* folderNode = NULL;
	TDblQueIter<CMsvCacheVisibleFolder> dqIter(iFolderListHeader);

	dqIter.SetToFirst();
	while((folderNode = dqIter++)!=NULL)
		{
		RBuf8 text;
		CleanupClosePushL(text);
		text.CreateL(100);
		text.Append(KFolderId);
		text.AppendNum(folderNode->GetFolderId());
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
		text.Append(KDriveId);
		text.AppendNum(folderNode->GetDrive());
#endif
		text.Append(KComplete);
		if(folderNode->IsComplete())
			{
			text.Append(KTrue);
			}
		else
			{
			text.Append(KFalse);
			}
		logger.Write(text);
		CleanupStack::PopAndDestroy();  // text
		folderNode->Print(logger);
		logger.Write(_L(""));		
		}
	logger.CloseLog();
	CleanupStack::PopAndDestroy();  // logger
	}
#endif			// #ifdef _DEBUG	
#endif			// #if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
	
