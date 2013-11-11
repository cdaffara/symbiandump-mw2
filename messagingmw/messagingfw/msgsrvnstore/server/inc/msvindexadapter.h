// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __MSVINDEXADAPTER_H__
#define __MSVINDEXADAPTER_H__


/**
 * HEADER FILES
 */
#include <e32def.h>
#include <e32base.h>
#include <msvuids.h>
#include <msvipc.h>
#include "msvdbadapter.h"
#include "msvcachevisiblefolder.h"

#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	#include "msvpreferreddrivelist.h"	
#endif    // #if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)


/**
 * FORWARD DECLARATION
 */
class CMsvEntryArray;
class CMsvServer;
class CMsvCacheEntry;
class CMsvCacheVisibleFolder;
class CMsvEntryFreePool;
class CMsvSearchSortDeltaCache;



/**
 * Class: CMsvIndexAdapter
 * 
 * The class encapsulates DB adapter class (which queries the database
 * for index entries) and cache (which stores recently accessed entry in
 * memory). The class also manages the data structure for cache and also 
 * works as background active object to perform low-priority job required
 * for cache management. 
 */

/**
@internalComponent
@released
*/
NONSHARABLE_CLASS (CMsvIndexAdapter) : public CActive
	{
// Interfaces
public:
	class TMsvServerEntryInfo
		{
	public:
		TMsvServerEntryInfo();
		void Reset();
	public:
		TMsvId			iId;				// Masked TMsvId in PREQ 557.
		TMsvId			iTopFolder;			// Masked TMsvId in PREQ 557.
		TMsvId			iService;			// Unmasked TMsvId in PREQ 557.
		TUid			iMtm;
		TUid			iType;
		TUid			iServiceMtm;
		TSecureId		iEntryOwnerId;
		TSecureId		iParentOwnerId;
		TBool			iPartOfMessage;
		};
	class CNonCommittedAddedEntries
		{
	public:	
		CNonCommittedAddedEntries(TMsvId aId, CMsvCacheEntry *aEntry):iVisibleFolder(aId),entry(aEntry)  {}
		TMsvId iVisibleFolder;
		CMsvCacheEntry *entry;
		};
	class TNonCommittedChangedEntries
		{
		public:	
            TNonCommittedChangedEntries(CMsvCacheVisibleFolder *aOldFolderNode, CMsvCacheVisibleFolder* aNewFolderNode, 
                                        CMsvCacheEntry *aEntry, CMsvCacheEntry* aBkpEntry, 
                                        CMsvCacheEntry *aOldParentEntry, CMsvCacheEntry* aNewParentEntry, 
                                        CMsvEntrySelection* aDescendentList, TBool aResetOldParentOwnerFlag)
				:iOldVisibleFolderNode(aOldFolderNode),iNewVisibleFolderNode(aNewFolderNode), iEntry(aEntry), 
				 iBkpEntry(aBkpEntry), iOldParentEntry(aOldParentEntry), iNewParentEntry(aNewParentEntry),
				 iDescendentList(aDescendentList), iResetOldParentOwnerFlag(aResetOldParentOwnerFlag)  {}
			CMsvCacheVisibleFolder *iOldVisibleFolderNode;
			CMsvCacheVisibleFolder *iNewVisibleFolderNode;
			CMsvCacheEntry *iEntry;
			CMsvCacheEntry *iBkpEntry;
			CMsvCacheEntry *iOldParentEntry;
			CMsvCacheEntry *iNewParentEntry;
			CMsvEntrySelection* iDescendentList;
			TBool iResetOldParentOwnerFlag;
		};
public:		

	// New API added in PREQ 557.
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	static CMsvIndexAdapter* NewL(CMsvServer& aServer);
	void GetChildrenAllL(TMsvId aId, CArrayPtr<const TMsvEntry>& aSelection, const TMsvSelectionOrdering& aOrdering, TUid aMtm, TBool aFilterByOwnerId, TSecureId aOwnerId);
	TInt GetChildrenIdAll(TMsvId aId, const CMsvEntryFilter& aFilter, CMsvEntrySelection& aSelection);	
	TInt GetChildrenIdAll(TMsvId aId, const CMsvEntryFilter& aFilter, CMsvEntrySelection& aSelection, TBool aFilterByOwnerId, TSecureId aOwnerId);
	void RemoveDriveL(TUint aDriveId, TUint aDriveIndex, TBool aIsStdFolderVisible = ETrue);
	void GetInPreparationIds(CMsvEntrySelection& aSelection, TUint aDriveId = 0 /* All Drives */);
	void AddDriveL(TUint aDrivePriority);
	void ChangeDriveL(TUint aNewDriveIndex, TBool aIsStdFolderVisible = ETrue);	
	inline TMsvId NextId(TUint aDriveId);
	void ReloadCacheL();
#else
	// Functions to create class instance
	static CMsvIndexAdapter* NewL(CMsvServer& aServer, const TFileName& aFileName);
	static CMsvIndexAdapter* OpenL(CMsvServer& aServer, const TFileName& aFileName);	
	inline TMsvId NextId();
	void GetInPreparationIds(CMsvEntrySelection& aSelection);
	inline void OpenclosedL(const TFileName& aDbFileName);
#endif		// #if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)

	~CMsvIndexAdapter();
	void SetLocalServiceComplete();
	TInt AddEntry(TMsvEntry& aEntry, TSecureId aOwnerId, TBool aAutoAssignId);
	TInt AddEntryToCommitListL(TMsvEntry& aEntry, TSecureId aOwnerId, TBool aAutoAssignId);
	TInt AddEntryNoCommit(TMsvEntry& aEntry, TSecureId aOwnerId, TBool aAutoAssignId);
	
	TInt DeleteEntry(TMsvId aId);
	TInt DeleteSelection(const CMsvEntrySelection& aSelection);
	TInt ExpandSelectionRecursively(CMsvEntrySelection& aSelection);
	
	TInt ChangeEntry(const TMsvEntry& aEntry, TSecureId aOwnerId, TBool aForcedUpdate);
	TInt ChangeEntryNoCommit(const TMsvEntry& aEntry, TSecureId aOwnerId, TBool aForcedUpdate);
	TInt ChangeEntryInternal(const TMsvEntry& aEntry, TSecureId aOwnerId);
	TInt GetEntry(TMsvId aId, TMsvEntry*& aEntry);
	TInt GetEntry(TMsvId aId, TMsvEntry*& aEntry, TSecureId& aOwnerId);
	TInt GetEntryNoCache(TMsvId aId, TMsvEntry* aEntry);
	
	void GetChildrenL(TMsvId aId, CArrayPtr<const TMsvEntry>& aSelection, const TMsvSelectionOrdering& aOrdering, TUid aMtm=KUidMsvNullEntry);
	TInt GetChildrenId(TMsvId aId, const CMsvEntryFilter& aFilter, CMsvEntrySelection& aSelection);
	void GetChildrenL(TMsvId aId, CArrayPtr<const TMsvEntry>& aSelection, const TMsvSelectionOrdering& aOrdering, TUid aMtm, TBool aFilterByOwnerId, TSecureId aOwnerId);
	TInt GetChildrenId(TMsvId aId, const CMsvEntryFilter& aFilter, CMsvEntrySelection& aSelection, TBool aFilterByOwnerId, TSecureId aOwnerId);

	TInt LockEntry(TMsvId aId);
	TInt ReleaseEntry(TMsvId aId);
	TInt IsEntryLocked(TMsvId aId, TBool& aLocked);
	TInt LockStore(TMsvId aId);
	TInt ReleaseStore(TMsvId aId);
	TInt IsStoreLocked(TMsvId aId, TBool& aLocked);
	TInt LockEntryAndStore(TMsvId aId);
	TInt ReleaseEntryAndStore(TMsvId aId);
	TInt IsEntryOrStoreLocked(TMsvId aId, TBool& aLocked);
	TInt IsStoreReadingLocked(TMsvId aId, TBool& aLocked);
	TInt IncStoreReaderCount(TMsvId aId);
	TInt DecStoreReaderCount(TMsvId aId);
	TInt OwningService(TMsvId aId, TMsvId& aService);
	TInt IsLocal(TMsvId aId, TBool& aLocal);
	TInt MoveEntry(TMsvId aId, TMsvId aTarget, CMsvEntrySelection* aDescendents = NULL);
	TInt IsADescendent(TMsvId aAscendentId, TMsvId aDescendentId, TBool& aDescendent);
	TBool EntryExists(TMsvId aId);
	TInt ChangeTemporaryData(const TMsvEntry& aNewEntryContents);
	TInt ChangeAttributes(CMsvEntrySelection& aSelection, TUint aSetAttributes, TUint aClearAttributes);
	inline void SetNextId(TMsvId aSeedId);
	inline void SetErrorState(TInt aError);
	inline TInt ErrorState();
	inline void DeleteDbAdapter();
	TMsvIndexProgress& Progress();
	TInt EntryTreeInfo(TMsvId aId, TMsvServerEntryInfo& aEntryInfo);
	void CommitNonCommitedEntries();
	TInt GetInternalEntry(TMsvId aId, CMsvCacheEntry*& aEntry) ;
	TBool GetNextSiblingL(TMsvId aId,TMsvId aParentId,TMsvId& aNextSiblingId);
	TBool GetFirstChildIdL(TMsvId aParentId,TMsvId& aFirstChild);	
	void UpdateDBWithCommitListL();	
	TBool IsEntryOrStoreLocked(TMsvId aId);
	void AddEntryNoVisibleL(CMsvCacheEntry* aEntry);
	TInt DeleteSelectionUsingTransaction(const CMsvEntrySelection& aSelection);
	TInt ForceDeleteEntry(TMsvId aId);
	void BeginTransaction();
	void CommitTransaction();
	CMsvDBAdapter* GetDbAdapter();
	void BackupDbAdpter();
	void RestoreDbAdpter();
	
#ifdef SYMBIAN_MESSAGESTORE_UNIT_TESTCODE
	inline TDblQueIter<CMsvCacheVisibleFolder> GetVisibleFolderQueue();
	void GetNonComittedAddedEntry(RPointerArray<CNonCommittedAddedEntries>*& a)	{a=&iNonCommittedAddedEntryList;}
	RArray<TNonCommittedChangedEntries>& GetNonComittedChangedEntry()	{return iNonCommittedChangedEntryList;}
#ifdef _DEBUG
	void PrintL();
#endif		// #ifdef _DEBUG
#endif
	
private:
	
	// Constructors
	CMsvIndexAdapter(CMsvServer& aServer);
	void RunL();
	void DoCancel();

#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	void ConstructL();
	TUint GetNextAvailableDriveId();
#else
	void ConstructNewL(const TFileName& aDbFileName);
	void ConstructOpenL(const TFileName& aDbFileName);
	void CreateInitialSetOfEntriesL();
	void CreateInitialCacheL();
#endif
	
	void GetChildrenIdL(TMsvId aId, CMsvEntrySelection& aSelection);
	TBool NewAttributes(const TMsvEntry& aEntry, TInt32& aNewData, TInt32& aNewPcSyncCount, TUint aSetAttributes, TUint aClearAttributes, TBool& aPersistedFlagsChanged);
	TBool FindEntryInCache(TMsvId aId, CMsvCacheEntry*& aEntry);
	TBool FindEntryL(TMsvId aId, CMsvCacheEntry*& aEntry, TBool aAddEntryToCache = ETrue);
	CMsvCacheVisibleFolder* AddEntryToCacheL(TMsvId aVisibleEntryId, CMsvCacheEntry* aEntry);
	void DoAddEntryL(TMsvEntry& aEntry, TSecureId aOwnerId, TBool aAutoAssignId, TBool aCommitToDb = ETrue);
	TBool GetVisibleFolderId(TMsvId aParentId, TMsvId& aVisibleFolderId);
	CMsvEntryArray* DoGetChildrenL(TMsvId aId, const CMsvEntryFilter& aFilter, TBool aFilterByOwnerId, TSecureId aOwnerId);
	void FilterChildrenListL(TMsvId aId, RPointerArray<CMsvCacheEntry> aChacheEntry, CMsvEntryArray& aEntryArray, const CMsvEntryFilter& aFilter, TBool aFilterByOwnerId, TSecureId aOwnerId);
	void DoChangeAttributesL(CMsvEntrySelection& aSelection, TUint aSetAttributes, TUint aClearAttributes);
	void DoChangeEntryL(const TMsvEntry& aNewEntryContents, TSecureId aOwnerId, TBool aChangeStandardFolder, TBool aForcedUpdate, TBool aCommitToFile);
	void DoExpandSelectionRecursivelyL(TMsvId aId);
	void DoDeleteSelectionL(const CMsvEntrySelection& aSelection);
	void UpdateDates(CMsvCacheEntry& aEntry, TBool aSetCreatedDate);
	void DoMoveEntryL(TMsvId aId, TMsvId aTarget, CMsvEntrySelection* aDescendents = NULL);
	void RollbackAdditions();
	void RollbackChanges();
	void ShiftEntryFolderL(TMsvId aNewVisibleFolderId, CMsvCacheVisibleFolder* aOldFolderNode, CMsvCacheEntry* aNewEntry);
	void UpdateEntryInCacheL(CMsvCacheVisibleFolder*, TMsvId, TMsvId, CMsvCacheEntry*);
	CMsvCacheVisibleFolder* UpdateCacheForMoveEntryL(TMsvId aNewVisibleFolderId, CMsvCacheEntry*& aOrigEntry, CMsvCacheVisibleFolder* aOldVisibleFolderNode, CMsvEntrySelection* aDescendents, TBool& aIsChildEntriesNeedsUpdation);
	CMsvCacheVisibleFolder* DoChangeEntryPreambleL(CMsvCacheEntry*& aOldEntry, TMsvId aNewParentId, CMsvCacheEntry*& aOldParentEntry, CMsvCacheEntry*& aNewParentEntry, TMsvId aOldVisibleFolderId, TBool& aResetOldParentOwnerFlag, CMsvEntrySelection*& aDescendentList);
	void DoChangeEntryPostamble(CMsvCacheVisibleFolder* aOldFolderNode, TMsvId aNewVisibleFolderId, TMsvId aEntryId, CMsvEntrySelection* aDescendentList, CMsvCacheEntry* aNewParentEntry, CMsvCacheEntry* aOldParentEntry, TBool aResetOldParentOwnerFlag);
	void GetVisibleFolderDetailsL(TMsvId aEntryId, CMsvCacheEntry*& aEntry, CMsvCacheVisibleFolder*& aVisibleFolder);

	void DoDeleteSelectionUsingTransactionL(const CMsvEntrySelection& aSelection);
	void DoForceDeleteEntryL(TMsvId aId);
	
	static TBool BackGroundOperations(TAny* aPtr);
	TBool DoBackGroundOperations();
	void DoBackGroundOperationsL(TBool& aStopBackgroundOperation);
	TBool DoRemoveDeletedEntriesL();
	TBool CheckAndAllocateMemoryL();
	void SplitBlockL();
	void CompleteSelf();
	
// Data
private:
	// Required for server logging and file handling.
	CMsvServer& 					iServer;
	
	// Global Error for the class.
	TInt 							iErrorState;

	// Entry Cache
	TDblQue<CMsvCacheVisibleFolder> iFolderListHeader;
	
	// Reference to freepool object.
	CMsvEntryFreePool* 				iFreePoolInstance;
	
	// The database pointer for DB operation.
	CMsvDBAdapter* 					iDbAdapter;
	// It's just a temporary iDbAdapter pointer to handle backup/restore scenario's
	CMsvDBAdapter* 					iTempDbAdapter;
	
	TInt 							iOrigEntryPos;
	CMsvEntrySelection*				iRecursionSelection;
	TInt 							iRecursionIndex;
		
	// List of newly added non committed entries.
	RPointerArray<CNonCommittedAddedEntries> iNonCommittedAddedEntryList;
	// List of newly changed non committed entries.
	RArray<TNonCommittedChangedEntries> iNonCommittedChangedEntryList;
	CIdle* 							iIdle;	
	CMsvCacheEntry*					iRootEntry;
	TMsvIndexProgress 				iProgress;
	TInt 							iBackgroundOperationPerformed;
	enum  {
		   ENoOperation,
		   ERemoveDeletedEntries,
		   EAllocateMemoryOperation,
		   ECheckBlockSize
		   } iBackgroundOperationState;

		   
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	TUint 							iFirstFreeDriveId;
	RArray<TMsvId>					iMaxMsvIdList;
	CMsvCacheVisibleFolder*			iRootNode;
#else
	// Next unused TMsvId. To be used while creating new entry.
	TMsvId 							iNextCreateId;
#endif

	friend class CMsvSearchSortCacheEntry;
#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
	friend class CTestIndexAdapter;
	friend class CTestIndexContext;
#endif
	};



/**
 ***********  INLINE FUNCTIONS   ************
 */
 
 
inline void CMsvIndexAdapter::SetErrorState(TInt aError)
	{
	iErrorState = aError;
	}


inline TInt CMsvIndexAdapter::ErrorState()
	{
	return iErrorState;
	}


inline CMsvDBAdapter* CMsvIndexAdapter::GetDbAdapter()
	{
	return iDbAdapter;
	}

inline void CMsvIndexAdapter::DeleteDbAdapter()
	{
	delete iDbAdapter;
	iDbAdapter = NULL;
	}

#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
// Code added for PREQ 557.
inline TMsvId CMsvIndexAdapter::NextId(TUint aDriveId)
	{
	aDriveId = (aDriveId == 0)? KCurrentDriveId : aDriveId;
	return iMaxMsvIdList[aDriveId]++;
	}
	

inline TUint GetDriveId(TMsvId aMaskedId)
	{
	TUint driveId = ( (aMaskedId & ~KDriveMask) >> 28);
	if(0 == driveId)
		{
		driveId = KCurrentDriveId;	
		}
	return driveId;
	}
	
inline TMsvId UnmaskTMsvId(TMsvId aMaskedId)
	{
	return (aMaskedId & KDriveMask);
	}

inline TBool IsStandardId(TMsvId aId)
	{
	if(UnmaskTMsvId(aId) < KMsvUnkownServiceIndexEntryIdValue)
		{
		return ETrue;
		}
	return EFalse;
	}

inline TBool IsStandardFolder(TMsvId aId)
	{
	if(UnmaskTMsvId(aId) < KMsvUnkownServiceIndexEntryIdValue &&
	   UnmaskTMsvId(aId) > KMsvLocalServiceIndexEntryIdValue )
		{
		return ETrue;
		}
	return EFalse;
	}
	
inline TMsvId MaskTMsvId(TUint aDriveId, TMsvId aUnmaskedId)
	{
	__ASSERT_DEBUG(aDriveId <= 7, User::Invariant());
	return (UnmaskTMsvId(aUnmaskedId) | (aDriveId << 28));
	}


// Wrap DB error and return KErrGeneral instead.
inline void WrapDBErrorL(TInt err)
	{	
	// Do not return DB error.
	if(err <= KSqlErrGeneral)
		{
		// If DB Error, return KErrGeneral.
		User::LeaveIfError(KErrGeneral);
		}
	else
		{
		User::LeaveIfError(err);
		}
	}
	
inline void CMsvIndexAdapter::BackupDbAdpter()
	{
	iTempDbAdapter = iDbAdapter;
	iDbAdapter = NULL;
	}

inline void CMsvIndexAdapter::RestoreDbAdpter()
	{
	iDbAdapter = iTempDbAdapter;
	iTempDbAdapter = NULL;
	}
#else		// #if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)

inline TMsvId CMsvIndexAdapter::NextId()
	{
	return iNextCreateId++;
	}
	

inline void CMsvIndexAdapter::OpenclosedL(const TFileName& aDbFileName)
	{
	iDbAdapter = CMsvDBAdapter::OpenL(aDbFileName);
	}


#endif     	// #if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)



#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
inline TDblQueIter<CMsvCacheVisibleFolder> CMsvIndexAdapter::GetVisibleFolderQueue()
	{
	return iFolderListHeader;	
	}	
#endif		// #if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)


#endif // __MSVINDEXADAPTER_H__
