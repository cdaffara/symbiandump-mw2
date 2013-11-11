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

#ifndef __MSVCACHEENTRY_H__
#define __MSVCACHEENTRY_H__

#include "MSVTIME.H"
#include "MSVPANIC.H"

#include <e32base.h>
#include <msvstd.h>

/**
CMsvCacheEntry corresponds to a TMsvEntry object that has been loaded into memory.
@internalComponent
@prototype
*/

NONSHARABLE_CLASS (CMsvCacheEntry): public CBase
{
public:
	inline TMsvEntry& Entry();
	inline TMsvId GetId() const;
	
	TInt LockEntry();
	inline TBool IsEntryLocked() const;
	inline void ReleaseEntry();
	
	TInt LockStore();
	inline TBool IsStoreLocked() const;
	inline void ReleaseStore();
	
	inline TBool IsStoreReadingLocked() const;
	inline void IncStoreReaderCount();
	inline void DecStoreReaderCount();

	TInt LockEntryAndStore();
	inline TBool IsEntryOrStoreLocked() const;
	inline TBool IsEntryAndStoreLocked() const;
	inline void ReleaseEntryAndStore();
	
	inline const TMsvTime& CreatedDate() const;
	inline void SetCreatedDate(const TMsvTime& aTime);
	inline const TMsvTime& LastChangeDate() const;
	inline void SetLastChangeDate(const TMsvTime& aTime);

	void CopyEntryL(const TMsvEntry& aEntry, TBool& aChangedPrivateInfo);
	void RollBackCopyEntry(const TMsvEntry& aEntry);
	
	inline TSecureId EntryOwnerId() const;
	inline void SetEntryOwnerId(TSecureId aOwnerId);
	
	inline RArray<TMsvId>* ChildIdArray();
	inline void SetChildIdArray(RArray<TMsvId>* aChildArray);
	
	inline TBool IsEntrySwappable() const;
	
	void DupNDestroyL(CMsvCacheEntry*& aEntry);
	void SetEntryL(TMsvEntry& aEntry);
	void ResetEntry();
	
private:
	static CMsvCacheEntry* NewLC(); 
	static CMsvCacheEntry* NewL(); 
	void ConstructL();
	CMsvCacheEntry();
	CMsvCacheEntry(const CMsvCacheEntry&);
	virtual ~CMsvCacheEntry();
	
private:
	enum 
		{
		EMsvServerEntryClearFlag		= 0x00,
		EMsvServerEntryLockFlag			= 0x01,
		EMsvServerEntryStoreLockFlag	= 0x02
		};

	// Only CMsvEntryFreePool class should
	// create an instance of CMsvCacheEntry.
	friend class CMsvEntryFreePool;
	// This class is made friend because
	// we need to destroy entry through ResetAndDestroy().
	friend class RPointerArray<CMsvCacheEntry>;
	
#ifdef SYMBIAN_MESSAGESTORE_UNIT_TESTCODE
	friend class CTestCacheEntry;
#endif
	
public:
	HBufC* 		iDescriptionBuffer;
	HBufC* 		iDetailsBuffer;

private:	
	TMsvEntry	iEntry;
	TUint32		iFlags;
	TInt32		iStoreReaderCount;
	TMsvTime	iLastChange;
	TMsvTime	iCreated;
	TSecureId	iOwnerId;
	RArray<TMsvId>* iChildIdArray;
};

#include "msvcacheentry.inl"

#endif    // #ifndef __MSVCACHEENTRY_H__
