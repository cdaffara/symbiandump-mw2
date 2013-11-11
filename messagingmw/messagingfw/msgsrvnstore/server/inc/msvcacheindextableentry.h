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
// CMSVCACHEINDEXTABLEENTRY.H
// 
//

#ifndef __CMSVCACHEINDEXTABLEENTRY_H__
#define __CMSVCACHEINDEXTABLEENTRY_H__

#include <e32base.h>
#include <msvstd.h>
#include <flogger.h>

class CMsvCacheEntry;

/**
Provides the implementation of the Cache Index Table.

The class implements a row in a folder's index table.
The class holds information about each child block associated with the folder. 
This class is used by CMsvCacheVisibleFolder.

@internalComponent
@prototype
*/
NONSHARABLE_CLASS (CMsvCacheIndexTableEntry) : public CBase
	{
public:
	static CMsvCacheIndexTableEntry* NewL();
	static CMsvCacheIndexTableEntry* NewL(CMsvCacheEntry*& aEntry);
	static CMsvCacheIndexTableEntry* NewLC(CMsvCacheEntry*& aEntry);
	static CMsvCacheIndexTableEntry* NewL(RPointerArray<CMsvCacheEntry>& aEntries, TInt aInitIndex = 0, TInt aCount = -1);
	static CMsvCacheIndexTableEntry* NewLC(RPointerArray<CMsvCacheEntry>& aEntries, TInt aInitIndex = 0, TInt aCount = -1);
	~CMsvCacheIndexTableEntry();
	
	void AddEntryL(CMsvCacheEntry*& aEntry, TBool aReplace = EFalse);
	void AddEntrySetL(RPointerArray<CMsvCacheEntry>& aEntries, TInt aInitIndex = 0, TInt aCount = -1);
	TBool GetEntry(TMsvId aId, CMsvCacheEntry*& aEntry);
	void GetChildrenL(TMsvId aParentId, RPointerArray<CMsvCacheEntry>& aEntries) /*const*/;
	void SortBlock();
	TBool EntryExists(TMsvId aId) const;
	void UpdateChildMsvIdsL(TMsvId aParentId, TMsvId aChildId, TBool aAdd = ETrue);
	void UpdateChildMsvIdsL(RPointerArray<CMsvCacheEntry>& aEntries);
	void DeleteEntryL(TMsvId aId, TMsvId& aParentIdOfEntry, TBool aForceDelete = EFalse);
	TBool ReleaseBlock(TBool aForceRelease = EFalse);
	void SplitBlockL(RPointerArray<CMsvCacheEntry>& aSplitBlock);
	inline TBool IsInRange(TMsvId aId) const;
	inline TBool IsDirty() const;
	inline void SetDirty();
	inline void ClearDirty();
	inline void ClearFlags();
	inline TMsvId GetMinMsvIdRange() const;
	inline TMsvId GetMaxMsvIdRange() const;
	inline TInt Size() const;
	inline void SetGrandChildPresent();
	inline void ClearGrandChildPresent();
	inline TBool IsGrandChildPresent() const;
	inline void SetMinMsvIdRange(TMsvId aId);
	inline void SetMaxMsvIdRange(TMsvId aId);	
	inline RPointerArray<CMsvCacheEntry>* BlockPtr();
	inline const TTime AccessTime() const;
	inline void SetAccessTime(TTime aNewTime);
	
#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
#ifdef _DEBUG	
	void Print(RFileLogger& aLogger);
#endif
#endif

private:
	CMsvCacheIndexTableEntry();
	void ConstructL(RPointerArray<CMsvCacheEntry>& aEntries);

public:
	enum
		{	
		EMsvCacheIndexTableClearFlag		= 0x0000,
		EMsvCacheIndexTableSetDirtyFlag		= 0x0001,
		EMsvCacheIndexTableClearDirtyFlag	= 0xFFFE,
		EMsvCacheIndexTableEntrySetGrandChildPresent = 0x0002,
		EMsvCacheIndexTableEntryClearGrandChildPresent = 0xFFFD,
		};
		
private:
	TTime iTimeStamp;
	TMsvId iMinMsvId;
	TMsvId iMaxMsvId;
	TInt16 iFlags;
	RPointerArray<CMsvCacheEntry>* iBlockPtr; 

private:	
#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
	friend class CTestIndexTableEntry; //only for testing
#endif
	};



#include "msvcacheindextableentry.Inl"

#endif // __CMSVCACHEINDEXTABLEENTRY_H__
