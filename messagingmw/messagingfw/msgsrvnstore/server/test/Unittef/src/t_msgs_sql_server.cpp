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

#include "t_msv_cacheentry_step.h"
#include "t_msv_indexadapter_step.h"
#include "t_msv_cache_indextableentry_step.h"
#include "t_msv_cache_visiblefolder_step.h"
#include "t_msv_dbadapter_step.h"
#include "t_msv_entryfreepool_step.h"
#include "t_msv_inidata.h"
#include "t_msv_indexcontext_step.h"
#include "t_msv_server_performance.h"

#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	#include "t_msv_server_step.h"
	#include "t_msv_preferreddrivelist_step.h"
	#include "t_msv_diskchangenotifier.h"
#endif


#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	#include "t_msv_messagedbadapter_step.h"
#endif



_LIT(KServerName, "t_msgs_sql_suite");


GLDEF_C const TTestName ServerName()
	{
	TTestName serverName(KServerName);
	return serverName;
	}

GLDEF_C CTestSuite* CreateTestSuiteL()
	{	
	START_SUITE;
	ADD_TEST_SUITE(CTestIndexAdapter);
	ADD_TEST_SUITE(CTestIndexTableEntry);
	ADD_TEST_SUITE(CTestVisibleFolder);
	ADD_TEST_SUITE(CTestIniData);
	ADD_TEST_SUITE(CTestCacheEntry);
 	ADD_TEST_SUITE(CTestDbAdapter);
	ADD_TEST_SUITE(CTestEntryFreePool);
	ADD_TEST_SUITE(CTestIndexContext);
	ADD_TEST_SUITE(CTestMsvServerPerformance);


#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	ADD_TEST_SUITE(CTestMsvServer);
 	ADD_TEST_SUITE(CTestMsvPreferredDriveList);
 	ADD_TEST_SUITE(CTestDiskChangeNotifier);
#endif


#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	ADD_TEST_SUITE(CTestMessageDbAdapter);
#endif

 	END_SUITE;
	}

GLDEF_C CTestStep* CreateTEFTestStep(const TDesC& /*aStepName*/, CTEFUnitServer& /*aServer*/)
	{
	// Initialise test step object to NULL if no TEF steps are assigned
	CTestStep* testStep = NULL;
	return testStep;
	}
