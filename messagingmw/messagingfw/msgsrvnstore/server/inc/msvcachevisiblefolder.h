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
// CMSVCACHEVISIBLEFOLDER.H
// 
//

#ifndef __CMSVCACHEVISIBLEFOLDER_H__
#define __CMSVCACHEVISIBLEFOLDER_H__

#include <e32base.h>
#include <e32std.h>
#include <msvstd.h>
#include <flogger.h>

/**
Forward declarations.
*/
class CMsvCacheIndexTableEntry;
class CMsvDBAdapter;
class CMsvCacheEntry;



/**
Provides the implementation of the Cache Visible Folder list.

This Class represents the list of visible folders, which holds the information
about the blocks and childrens which it corresponds to.

@internalComponent
@prototype
*/
NONSHARABLE_CLASS (CMsvCacheVisibleFolder) : public CBase
	{
public:
	static CMsvCacheVisibleFolder* NewL(TMsvId aId);
	static CMsvCacheVisibleFolder* NewL(TMsvId aId, RPointerArray<CMsvCacheEntry>& aEntries);
	~CMsvCacheVisibleFolder();
	
	void AddEntryL(CMsvCacheEntry*& aEntry,  TBool aReplace = EFalse);
	void AddEntryListL(RPointerArray<CMsvCacheEntry>& aEntries, TBool aIsCompleteChildOfFolder = EFalse);
	TBool GetEntry(TMsvId aId, CMsvCacheEntry*& aEntry);
	TBool GetChildrenL(TMsvId aId, CMsvDBAdapter* aDbAdapter, RPointerArray<CMsvCacheEntry>& aEntries);
	TBool GetChildrenIdL(CMsvEntrySelection& aSelection);
	void DeleteEntryL(TMsvId aId, TBool aForceDelete = EFalse);
	void DeleteEntryListL(CMsvEntrySelection& aEntrySelection);
	TBool EntryExists(TMsvId aId) const;
	TBool IsAllBlocksDirty() const;
	TBool ReleaseAllBlocks(TInt& aCount);
	TInt ReleaseBlocks(TInt aNumberOfEntriesToRelease, TBool& aIsFolderEmpty);
	void UpdateChildMsvIdsL(TMsvId aParentId, TMsvId aChildId, TBool aAppend = ETrue);
	void UpdateChildMsvIdsL(RPointerArray<CMsvCacheEntry>& aEntries);
	void SplitBlockL();
	RPointerArray<CMsvCacheIndexTableEntry>* GetIndexTableEntry();
	inline TBool IsComplete() const;
	inline void SetComplete(TBool aStatus);
	inline TMsvId GetFolderId() const;	
	inline void SetGetChildrenFromVisibleFolder(TBool aFlag);
	inline TBool GetChildrenFromVisibleFolder();

#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	inline TUint GetDrive() const;
	inline void SetDrive(TUint aDriveId);
#endif

#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
#ifdef _DEBUG
	void Print(RFileLogger& aLogger);
#endif
#endif

private:
	CMsvCacheVisibleFolder(TMsvId aId);
	void ConstructL(RPointerArray<CMsvCacheEntry>& aEntries);
	void SplitAndAppendL(RPointerArray<CMsvCacheEntry>& aEntryList, TInt aInitIndex=0);
public:
	TDblQueLink  iDlink;
	static const TInt iOffset;

private:
	TMsvId iVisibleFolderId;
	TInt8 iFlags;
	TTime iAccessTime;
	RPointerArray<CMsvCacheIndexTableEntry> iIndexTable;

#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	TUint iDriveId;
#endif

	enum 
		{	
		EMsvCacheVisibleFolderClearFlag				= 0X00,
		EMsvCacheVisibleFolderComplete				= 0X02,
		EMsvCacheVisibleFolderClearComplete			= 0XF9,
		EMsvGetChildrenFromVisibleFolder			= 0X04,
		};

private:
#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
	friend class CTestVisibleFolder; //only for testing
#endif
	};


#include "msvcachevisiblefolder.Inl"

#endif // __CMSVCACHEVISIBLEFOLDER_H__

	
