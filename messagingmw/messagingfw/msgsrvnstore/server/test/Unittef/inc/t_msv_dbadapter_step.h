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
//

#ifndef T_MSV_DBADAPTER_STEP_H
#define T_MSV_DBADAPTER_STEP_H


#include <test/tefunit.h>
#include <msvuids.h>
#include <msvids.h>
#include <msvstd.h>
#include <e32std.h>
#include "msvdbadapter.h"
#include "msventryfreepool.h"
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	#include "msvpreferreddrivelist.h"
#endif

/**
@internalTechnology
@prototype
*/

class CTestDbAdapter : public  CActiveTestFixture
// WARNING: Despite being a C class, CActiveTestFixture is NOT derrived from CBase
// ... so don't expect your data members to be zero'ed
	{
public:
	CTestDbAdapter();
	virtual ~CTestDbAdapter();
	void SetupL();
	void TearDownL();

	void TestDatabaseBasics();
	void TestCreateEntry();
	void TestEntryExists();
	void TestDeleteEntry();
	void TestUpdateEntry();
	void TestGetEntry();
	void TestTransaction();
	void TestDeleteBulkEntry();
	void TestGetInPreparationId();
	void TestUpdateVisibleFolder();
	void TestUpdateOwnerStatus();
	void TestGetChildren();
	void TestGetChildrenCondL();
	static CTestSuite* CreateSuiteL(const TDesC& aName);
	
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	void TestValidateDatabase();
	void TestMaxTMsvId();
	void TestGetVisibleFlag();
	void TestGetChildrenId();
	void TestCreateRootEntry();
	void TestDBPointerCreation();
	void TestGetVisibleFolderId();
#else
	void TestDeletePerf();
	void TestGetMaxTMsvId();
	void TestValidateDatabaseL();
#endif
		
private:
	TBool EntryExists(TMsvId aId);
	void CreateEntryL(TMsvId aId, TMsvId aParentId, TUid aUid, TMsvId& aVisibleFolder, TBool aIsVisible = EFalse, TBool isPreparationFlag = EFalse);
	void ResetAndDestroy(RPointerArray<CMsvCacheEntry>& childArray);

private:
	RFs iFileHandle;
	CMsvDBAdapter* iMsvDbAdaptor;
	CMsvEntryFreePool *iMsvEntryFreePool;

#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	CMsvPreferredDriveList* iDriveList;
#endif
	};
#endif // T_MSV_DBADAPTER_STEP_H
