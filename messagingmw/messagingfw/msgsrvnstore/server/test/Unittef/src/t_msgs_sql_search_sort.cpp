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

#include <test/ctefunitserver.h>

#include "ctestsearchsortdeltacache.h"
#include "ctestsearchsortcachemanager.h"
#include "t_msv_search_sort_cacheentry_step.h"
#include "t_msv_search_sort_dbadapter_step.h"
#include "ctestsearchsortoperation.h"
#include "ctestheadersearchsort.h"
#include "ctestoomsearchsortcachemanager.h"
#include "ctestmsvipc.h"
#include "ctestattachmentsearch.h"

#include "t_msv_inidata.h"

_LIT(KServerName, "tmsgsqlsearchsortunitserver");

GLDEF_C const TTestName ServerName()
	{
	TTestName serverName(KServerName);
	return serverName;
	}

GLDEF_C CTestSuite* CreateTestSuiteL()
	{	
	START_SUITE;
 	ADD_TEST_SUITE(CTestSearchSortDBadapter);
	ADD_TEST_SUITE(CTestSearchSortCacheEntry);
	ADD_TEST_SUITE(CTestSearchSortDeltaCache);
	ADD_TEST_SUITE(CTestMsvIpc);
#ifdef _DEBUG_
	//Running OOM conditions only in debug mode
	ADD_TEST_SUITE(CTestOOMSearchSortCacheManager);
	ADD_TEST_SUITE(CTestAttachmentSearch);
	ADD_TEST_SUITE(CTestHeaderSearchSortOperation);
	ADD_TEST_SUITE(CTestSearchSortCacheManager);
	ADD_TEST_SUITE(CTestSearchSortOperation);
#endif
 	END_SUITE;
	}

GLDEF_C CTestStep* CreateTEFTestStep(const TDesC& /*aStepName*/, CTEFUnitServer& /*aServer*/)
	{
	// Initialise test step object to NULL if no TEF steps are assigned
	CTestStep* testStep = NULL;
	/*
	if(aStepName == KSampleStep1)
		{
		testStep = new CSampleStep1(aServer);
		}
	*/
	return testStep;
	}
