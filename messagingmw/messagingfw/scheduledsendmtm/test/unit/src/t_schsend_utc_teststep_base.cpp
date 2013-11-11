// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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

/**
 @SYMTestCaseID MSG-SCHEDULESENTMTM-FAILED1-0243
 @SYMTestType UT
 @SYMTestPriority High
 @SYMPREQ 234
 @SYMTestCaseDesc Tests the CMsvScheduleSend::ReScheduleL API.
 @SYMTestActions  Creates 2 pigeon messages that are both re-schedule to fail.
 @SYMTestExpectedResults Post scheduling sending state values - KMsvSendStateWaiting. The schedule send component does not alter messages that are not re-scheduled.
*/

#include <test/testexecutelog.h>
#include "t_schsend_utc_teststep_base.h"

RTest test(_L("T_SchedSend_UTC Test Harness"));

TTimeIntervalSeconds CSchSendStepBase::ReadUTCOffsetL()
	{
	TInt offset;
	//read the utc offset
	if(!GetIntFromConfig(ConfigSection(),_L("UTCOffset"),offset))
		{
		ERR_PRINTF1(_L("Error reading UTC offset from ini file"));
		User::Leave(KErrArgument);	
		}
	TTimeIntervalSeconds interval(offset);
		
	return interval;	
	}
	

CSchSendStepBase::CSchSendStepBase()
	{
	}
		
CSchSendStepBase::~CSchSendStepBase()
	{
	delete iTestUtils;
	delete iScheduler;
	}


TVerdict CSchSendStepBase::doTestStepPreambleL()
	{
	SetTestStepResult(EPass);	
	// Create and install the active scheduler
 	iScheduler = new (ELeave) CActiveScheduler();
	CActiveScheduler::Install(iScheduler);
	
	iTestUtils = CScheduleSendTestUtils::NewL(test);
	// Install the pigeon MTM.
	iTestUtils->InstallMtmGroupsL();
	
	return TestStepResult();
	}

TVerdict CSchSendStepBase::doTestStepPostambleL()
/**
 * @return - TVerdict code
 * Override of base class virtual
 */
	{
	return TestStepResult();
	}
	

	


