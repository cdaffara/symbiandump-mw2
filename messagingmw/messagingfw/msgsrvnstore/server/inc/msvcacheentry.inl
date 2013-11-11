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
// INLINE FUNCTIONS
// 
//

/**
 
 Entry()
 @param None.
 @return TMsvEntry& : a handle to the TMsvEntry of the cache entry.
 
 The function returns a handle to the TMsvEntry of the cache entry.
 
*/
inline TMsvEntry& CMsvCacheEntry::Entry()
	{
	return iEntry;
	}



/**
 * GetId()
 * @param None.
 * @return TMsvId: TMsvId of the entry
 * 
 * The function returns the TMsvId of the entry.
 */
inline TMsvId CMsvCacheEntry::GetId() const
	{
	return iEntry.Id();
	}



/**
 * IsEntryLocked()
 * @param None.
 * @return TBool : ETrue if the entry has been locked, EFalse otherwise.
 * 
 * The function returns the value of the EMsvServerEntryLockFlag flag,
 * which represents whether the cache entry has been locked or not.
 */
inline TBool CMsvCacheEntry::IsEntryLocked() const
	{
	return iFlags & EMsvServerEntryLockFlag;
	}



/**
 * ReleaseEntry()
 * @param None.
 * @return None.
 * 
 * The function resets the value of the EMsvServerEntryLockFlag flag, i.e releases
 * the lock on the cache entry.
 */
inline void CMsvCacheEntry::ReleaseEntry()
	{
	__ASSERT_DEBUG(IsEntryLocked(), PanicServer(EMsvReleasingEntryNotLocked));
	iFlags = iFlags & ~EMsvServerEntryLockFlag;
	}



/**
 * IsStoreLocked()
 * @param None.
 * @return TBool : ETrue if the server entry has been locked, EFalse otherwise.
 * 
 * The function returns the value of the EMsvServerEntryStoreLockFlag flag,
 * which represents whether the server entry has been locked or not.
 */
inline TBool CMsvCacheEntry::IsStoreLocked() const
	{
	return iFlags & EMsvServerEntryStoreLockFlag;
	}



/**
 * ReleaseStore()
 * @param None.
 * @return None.
 * 
 * The function resets the value of the EMsvServerEntryStoreLockFlag flag, i.e releases
 * the lock on the server entry.
 */
inline void CMsvCacheEntry::ReleaseStore()
	{
	__ASSERT_DEBUG(IsStoreLocked(), PanicServer(EMsvReleasingStoreNotLocked));
	iFlags = iFlags & ~EMsvServerEntryStoreLockFlag;
	}



/**
 * IsStoreReadingLocked()
 * @param None.
 * @return TBool : ETrue if there are no readers to the store, EFalse otherwise.
 * 
 * The function returns ETrue if the number of readers to the store (iStoreReaderCount)
 * is 0, EFalse if otherwise.
 */
inline TBool CMsvCacheEntry::IsStoreReadingLocked() const
	{
	return iStoreReaderCount == 0 ? EFalse : ETrue;
	}



/**
 * IncStoreReaderCount()
 * @param None.
 * @return None.
 * 
 * The function increments the number of readers to the store (iStoreReaderCount).
 */
inline void CMsvCacheEntry::IncStoreReaderCount()
	{
	iStoreReaderCount++;
	}



/**
 * DecStoreReaderCount()
 * @param None.
 * @return None.
 * 
 * The function decrements the number of readers to the store (iStoreReaderCount).
 */
inline void CMsvCacheEntry::DecStoreReaderCount()
	{
	__ASSERT_DEBUG(iStoreReaderCount>0, User::Invariant());

	iStoreReaderCount--;
	}



/**
 * IsEntryOrStoreLocked()
 * @param None.
 * @return TBool : ETrue if the server OR cache entries have been locked, EFalse otherwise.
 * 
 * The function returns ETrue if either of store and cache entries have been locked,
 * EFalse if neither are locked.
 */
inline TBool CMsvCacheEntry::IsEntryOrStoreLocked() const
	{
	return iFlags & (EMsvServerEntryStoreLockFlag | EMsvServerEntryLockFlag);
	}



/**
 * IsEntryAndStoreLocked()
 * @param None.
 * @return TBool : ETrue if the server AND cache entries have been locked, EFalse otherwise.
 * 
 * The function returns ETrue if both the store and cache entries have been locked,
 * EFalse if only one of them are locked.
 */
inline TBool CMsvCacheEntry::IsEntryAndStoreLocked() const
	{
	return IsEntryLocked() && IsStoreLocked();
	}



/**
 * ReleaseEntryAndStore()
 * @param None.
 * @return None.
 * 
 * The function resets the value of the EMsvServerEntryLockFlag and
 * EMsvServerEntryStoreLockFlag flags, i.e releases the lock on the server 
 * and cache entries.
 */
inline void CMsvCacheEntry::ReleaseEntryAndStore()
	{
	__ASSERT_DEBUG(IsEntryOrStoreLocked(), PanicServer(EMsvReleasingStoreNotLocked));
	iFlags = iFlags & ~(EMsvServerEntryStoreLockFlag|EMsvServerEntryLockFlag);
	}



/**
 * CreatedDate()
 * @param None.
 * @return TMsvTime& : a reference to the creation time and date of the entry.
 * 
 * The function returns a reference to the creation time and date of the entry.
 */
inline const TMsvTime& CMsvCacheEntry::CreatedDate() const
	{
	return iCreated;
	}



/**
 * SetCreatedDate()
 * @param TMsvTime& : a reference to the creation time and date of the entry.
 * @return None.
 * 
 * The function sets the creation time and date of the entry to the passed value.
 */
inline void CMsvCacheEntry::SetCreatedDate(const TMsvTime& aTime)
	{
	iCreated = aTime;
	}



/**
 * LastChangeDate()
 * @param None.
 * @return TMsvTime& : a reference to the last change time and date of the entry.
 * 
 * The function returns a reference to the last change time and date of the entry.
 */
inline const TMsvTime& CMsvCacheEntry::LastChangeDate() const
	{
	return iLastChange;
	}



/**
 * SetLastChangeDate()
 * @param TMsvTime& : a reference to the last change time and date of the entry.
 * @return None.
 * 
 * The function sets the last change time and date of the entry to the passed value.
 */
inline void CMsvCacheEntry::SetLastChangeDate(const TMsvTime& aTime)
	{
	iLastChange = aTime;
	}



/**
 * EntryOwnerId()
 * @param None.
 * @return TSecureId : TSecureId of the owner of the cache entry.
 * 
 * The function returns the TSecureId of the owner of the cache entry.
 */
inline TSecureId CMsvCacheEntry::EntryOwnerId() const
	{
	return iOwnerId;
	}



/**
 * SetEntryOwnerId()
 * @param TSecureId : TSecureId of the owner of the cache entry.
 * @return None.
 * 
 * The function sets the TSecureId of the owner of the cache entry.
 */
inline void CMsvCacheEntry::SetEntryOwnerId(TSecureId aOwnerId)
	{
	iOwnerId = aOwnerId;
	}



/**
 * ChildIdArray()
 * @param None.
 * @return RArray<TMsvId>* : a handle to the iChildIdArray of the cache entry.
 * 
 * The function returns a handle to the iChildIdArray of the cache entry, which
 * is an array of child entries of this cache entry.
 */
inline RArray<TMsvId>* CMsvCacheEntry::ChildIdArray()
	{
	return iChildIdArray;
	}
	
	
/**
 * SetChildIdArray()
 * @param RArray<TMsvId>*: pointer to the child array
 * @return None.
 *
 * The function sets the child array of the cache entry to the child array passed.
 */
inline void CMsvCacheEntry::SetChildIdArray(RArray<TMsvId>* aChildIdArray)
	{
	iChildIdArray = aChildIdArray;
	}
 




/**
 * IsEntrySwappable()
 * @return TBool: Returns ETrue if entry can be swappend out of cache.
 *
 * The function will mainly be used by cache related code
 * which will check if the entry can be swapped out.
 */
inline TBool CMsvCacheEntry::IsEntrySwappable() const
	{
	return !(IsEntryOrStoreLocked() || IsStoreReadingLocked());
	}

