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


#ifndef CTESTSEARCHSORTCACHEMANGER_H
#define CTESTSEARCHSORTCACHEMANGER_H

#include <test/tefunit.h>
#include "msvsearchsortcachemanager.h"
#include "msvsearchsortdeltacache.h"
#include "msvindexadapter.h"

class CMsvDBAdapter;
class CMsvDBAdaption;
/**
@internalTechnology
@prototype
*/
class CTestSearchSortCacheManager : public  CActiveTestFixture
// WARNING: Despite being a C class, CActiveTestFixture is NOT derrived from CBase
// ... so don't expect your data members to be zero'ed
	{
public:
	CTestSearchSortCacheManager(); 
	virtual ~CTestSearchSortCacheManager();
	void SetupL();
	void TearDownL();
	void TestCreateManagerL();
	void TestInstanceL();
	void TestAddEntryL();
	void TestQueryExistEntryL();
	static CTestSuite* CreateSuiteL(const TDesC& aName);
	void CreateEntryL(TMsvId aId, TMsvId aParentId, TUid aUid, TMsvId& aVisibleFolder, TBool aIsVisible /* (Default = EFalse)*/, TBool isPreparationFlag /*(Default=EFalse)*/, TBool iTestValue/*(Default=EFalse)*/);
	void TestCreateEntries();
	void TestRemoveEndEntryL();
	void TestTypeOfSortQueryL();
	void TestRetrunQueryL();
	void TestMoveEntryAtEndL();
	void TestUnMarkedQueryL();
	void TestOnDemandUpdateCacheEntryL();
	void TestResultCountL();
	void TestFinalResultAsIdL();
	void TestStoreResultL();
	void TestDoProcessQueryL();
	void TestRemoveEntryL();
	void TesUpdateQueryResultOnDemandInDBL();
private:
	CMSvSearchSortCacheManager* iSearchSortCacheManager;
	CMsvServer* iServer;
	CMsvIndexAdapter* iIndexAdapter;
	CMsvDBAdapter* iDBadapter;
	CMsvDBAdaption* iSearchDbAdapter;
	CMsvSearchSortDeltaCache* iSearchSortDeltaCache;
	
	};
#endif // ctestsearchsortCacheManager_H
