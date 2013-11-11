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
// Contributors: Ashok Kumar
//
// Description:
//

#include <test/ctefunitserver.h>

#include "t_msv_converterwaiter.h"
#include "t_msv_convertermanager.h"
#include "t_msv_version0version1converter.h"
#include "t_msv_version1version2converter.h"
#include "t_msv_converterdbadapter.h"
#include "t_msv_smsmessage_v1v2_converter_step.h"
#include "t_msv_inidata.h"

_LIT(KServerName, "T_MSGS_MESSAGE_CONVERTER_SUITE");

GLDEF_C const TTestName ServerName()
	{
	TTestName serverName(KServerName);
	return serverName;
	}

GLDEF_C CTestSuite* CreateTestSuiteL()
	{	
	START_SUITE;
 	ADD_TEST_SUITE(CTestMessageConverterWaiter);
 	ADD_TEST_SUITE(CTestMessageConverterManager);
 	ADD_TEST_SUITE(CTestVersion0Version1Converter); 
 	ADD_TEST_SUITE(CTestVersion1Version2Converter); 
    ADD_TEST_SUITE(CTestConverterDBAdapter);  
    ADD_TEST_SUITE(CTestSmsMessageV1toV2Converter);      		
 	END_SUITE;
	}

GLDEF_C CTestStep* CreateTEFTestStep(const TDesC& /*aStepName*/, CTEFUnitServer& /*aServer*/)
	{
	// Initialise test step object to NULL if no TEF steps are assigned
	CTestStep* testStep = NULL;
	return testStep;
	}
