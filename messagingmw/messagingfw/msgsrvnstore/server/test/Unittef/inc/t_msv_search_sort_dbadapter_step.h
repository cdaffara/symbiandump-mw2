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

#ifndef T_MSV_SEARCH_SORT_DBADAPTER_STEP_H
#define T_MSV_SEARCH_SORT_DBADAPTER_STEP_H

#include <test/tefunit.h>
#include "msvsearchsortcachemanager.h"

#include "msvdbadapter.h"
#include "msvsearchsortcacheentry.h"
#include "msvsearchsortdbwrapper.h"
#include <msvsearchsortquery.h>
#include <msvuids.h>
#include <msvids.h>
#include <msvstd.h>
#include <e32std.h>
#include "msvpreferreddrivelist.h"
#include "MSVSERV.H"

/**
@internalTechnology
@prototype
*/
class CTestSearchSortDBadapter : public  CActiveTestFixture
	{
public:
	CTestSearchSortDBadapter(); 
	virtual ~CTestSearchSortDBadapter();
	void SetupL();
	void TearDownL();
	
	//Test Cases
	void TestDoAddRelationalOperatorL();
	void TestIsSortAsNumRequired();
	void TestAddIdtoDBL();
	void TestGetSortedTMsvIdsfromTableL();
	void TestReturnResultCountInTableL();
	void TestDeleteQueryFromTableL();
	void TestDeleteTMsvIdFromAllQueriesL();
	void TestDeleteTMsvIdFromQueryL();
	void TestDoAppendIdConstraintL();
	void TestDoCreateInsertSortQueryL();
	void TestReplaceWildCardCharactersL();
	void TestDoCreateSortQueryL();
	void TestDoCreateSearchQueryL();
	void TestFindL();
	void TestUpdateNewIdsL();
	void TestDoSortL();
	void TestFindLSubFoldersL();
	void TestCreateQueryDateL();
	void TestIteratorFindL();
	void TestGetLastNTMsvIdsfromTableL();

	//Member Functions
	void CreateEntryL(TMsvId aId, TMsvId aParentId, TUid aUid, TMsvId& aVisibleFolder, TBool aIsVisible=EFalse, TBool isPreparationFlag=EFalse, TBool iTestValue=EFalse);
	void CreateSortEntriesL(TInt Iter);
	
	static CTestSuite* CreateSuiteL(const TDesC& aName);
	
private:
	
	CMsvDBAdapter* iDBadapter;
	TSearchSortDbWrapper* iSearchDbAdapter;
	CMsvServer* iServer;
	CMSvSearchSortCacheManager* iSearchSortCacheManager;
	friend class CMSvSearchSortCacheManager;
	};
#endif // T_MSV_SEARCH_SORT_DBADAPTER_STEP_H
