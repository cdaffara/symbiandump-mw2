// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// T_MSGS_CACHE_VISIBLEFOLDER_STEP.H
// 
//

#ifndef T_MSV_CACHE_VISIBLEFOLDER_STEP_H
#define T_MSV_CACHE_VISIBLEFOLDER_STEP_H

#include <test\tefunit.h>
#include <msvuids.h>
#include <msvids.h>
#include <msvstd.h>
#include <e32std.h>

#include "msvcachevisiblefolder.h"
#include "msvcacheindextableentry.h"
#include "msvcacheentry.h"
#include "msventryfreepool.h"
#include "msvdbadapter.h"
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
#include "msvpreferreddrivelist.h"
#endif

/**
@internalTechnology
@prototype
*/
class CTestVisibleFolder : public  CActiveTestFixture
// Note that all test suites must begin with "CTest"
// WARNING: Despite being a C class, CActiveTestFixture is NOT derrived from CBase
// ... so don't expect your data members to be zero'ed
	{
public:
	CTestVisibleFolder();
	virtual ~CTestVisibleFolder();
	void SetupL();
	void TearDownL();

	void TestAddEntry();
	void TestAddEntryList();
	void TestGetEntry();
	void TestGetChildren();
	void TestDeleteEntry();
	void TestDeleteEntryList();
	void TestReleaseAllBlocks();
	void TestReleaseBlocks();
	void TestIsComplete();
	void TestIsAllBlocksDirty();
	void TestSetComplete();
	void TestEntryExists();
	void TestSplitBlock();
	void TestCreation();
	void TestUpdateChildMsvIds();
	
	void AddTestEntries();
	void InitDb();
	void DeleteDb();
	void CreateEntry(TMsvId aId, TMsvId aParentId, TUid aUid, TMsvId& aVisibleFolder, TBool aIsVisible = EFalse);

	static CTestSuite* CreateSuiteL(const TDesC& aName);
	CMsvCacheEntry* Entry();
private:
	RPointerArray<CMsvCacheVisibleFolder> iFolderPointer;
	RPointerArray<CMsvCacheEntry> iEntryPointer;
	CMsvCacheEntry *aEntryA, *aEntryB, *aEntryC;
	TMsvEntry *aTmsvEntryA, *aTmsvEntryB, *aTmsvEntryC;
	CMsvEntryFreePool *iMsvEntryFreePool;
	CMsvDBAdapter* iMsvDbAdaptor;
	RFs iFileHandle;
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	CMsvPreferredDriveList* iDriveList;
#endif
	};
	 
#endif // T_MSV_CACHE_VISIBLEFOLDER_STEP_H
