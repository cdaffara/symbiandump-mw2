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

#include <test\ctefunitserver.h>

#include "t_msv_session_step.h"
#include "t_msv_clnt_test_header_step.h"
#include "t_msv_store_step.h"
#include "t_cmsventry_step.h"

_LIT(KServerName, "t_msgs_client_suite");


GLDEF_C const TTestName ServerName()
	{
	TTestName serverName(KServerName);
	return serverName;
	}

GLDEF_C CTestSuite* CreateTestSuiteL()
	{	
	START_SUITE;
 	ADD_TEST_SUITE(CTestMsvSession);
 	ADD_TEST_SUITE(CTestCMsvEntry);
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
 	ADD_TEST_SUITE(CTestHeaderEntry);
 	ADD_TEST_SUITE(CTestStore);
#endif
 	END_SUITE;
	}

GLDEF_C CTestStep* CreateTEFTestStep(const TDesC& /*aStepName*/, CTEFUnitServer& /*aServer*/)
	{
	// Initialise test step object to NULL if no TEF steps are assigned
	CTestStep* testStep = NULL;
	return testStep;
	}
