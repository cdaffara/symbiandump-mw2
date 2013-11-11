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
 @SYMTestCaseID MSG-SCHEDULESENTMTM-RESCEDULETIME2-0243
 @SYMTestType UT
 @SYMTestPriority High
 @SYMPREQ 234
 @SYMTestCaseDesc Tests the CMsvScheduleSend::ReScheduleL API.
 @SYMTestActions  Creates 2 pigeon messages - one is scheduled for immeduate sending and the other is scheduled for later sending.
 @SYMTestExpectedResults Post scheduling sending state values - KMsvSendStateResend. Tests correct scheduling base on UTC time.
*/

#include <test/testexecutelog.h>
#include "t_schsend_utc_rescheduletime_2_step.h"

CRescheduleTime2Step::CRescheduleTime2Step()
	{
	// Call base class method to set up the human readable name for logging
	SetTestStepName(KRescheduleTime2Step);
	}


TVerdict CRescheduleTime2Step::doTestStepL()
	{		
	CTestScheduleSendBase* testCase = NULL;
	
	TTimeIntervalSeconds utcOffset = ReadUTCOffsetL();
	//pass legacy test code testexecute logger so that logging can go to the testexecute html file
	testCase = CTestReScheduleTime_2::NewL(*iTestUtils,utcOffset,*this);
	
	testCase->Start();
	CActiveScheduler::Start();
	
	//get the result
	SetTestStepResult(testCase->GetVerdict());
	delete testCase;

	return TestStepResult();
	}	
	


