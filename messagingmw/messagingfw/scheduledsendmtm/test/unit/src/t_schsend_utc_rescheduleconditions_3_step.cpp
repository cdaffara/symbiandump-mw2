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
 @SYMTestCaseID MSG-SCHEDULESENTMTM-RESCEDULECONDITIONS3-0243
 @SYMTestType UT
 @SYMTestPriority High
 @SYMPREQ 234
 @SYMTestCaseDesc Tests the CMsvScheduleSend::ReScheduleL API.
 @SYMTestActions  Creates 2 pigeon messages that are both re-schedule for pending conditions. A short timeout value is specified. Only one conditions is met - therefore timeout should occur.
 @SYMTestExpectedResults Post scheduling sending state values - KMsvSendStateResend. 
*/

#include <test/testexecutelog.h>
#include "t_schsend_utc_rescheduleconditions_3_step.h"
	
CRescheduleConditions3Step::CRescheduleConditions3Step()
	{
	// Call base class method to set up the human readable name for logging
	SetTestStepName(KRescheduleConditions3Step);
	}


TVerdict CRescheduleConditions3Step::doTestStepL()
	{		
	CTestScheduleSendBase* testCase = NULL;
	
	TTimeIntervalSeconds utcOffset = ReadUTCOffsetL();
	//pass legacy test code testexecute logger so that logging can go to the testexecute html file
	testCase = CTestRescheduleConditions_3::NewL(*iTestUtils,utcOffset,*this);
	
	testCase->Start();
	CActiveScheduler::Start();
	
	//get the result
	SetTestStepResult(testCase->GetVerdict());
	delete testCase;	
	
	return TestStepResult();
	}	
	
	


