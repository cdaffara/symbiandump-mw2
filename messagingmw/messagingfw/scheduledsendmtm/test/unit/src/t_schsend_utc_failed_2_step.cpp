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
 @SYMTestCaseID MSG-SCHEDULESENTMTM-FAILED2-0243
 @SYMTestType UT
 @SYMTestPriority High
 @SYMPREQ 234
 @SYMTestCaseDesc Tests the CMsvScheduleSend::ReScheduleL API.
 @SYMTestActions  Creates 2 pigeon messages that are both re-schedule to fail, but on a second re-schedule.
 @SYMTestExpectedResults Post scheduling sending state values - KMsvSendStateWaiting. Sent sending state value - KMsvSendStateWaiting. The schedule send component does not alter messages that are not re-scheduled.
*/

#include <test/testexecutelog.h>
#include "t_schsend_utc_failed_2_step.h"

CFailed2Step::CFailed2Step()
	{
	// Call base class method to set up the human readable name for logging
	SetTestStepName(KFailed2Step);
	}

TVerdict CFailed2Step::doTestStepL()
	{		
	CTestScheduleSendBase* testCase = NULL;
	
	TTimeIntervalSeconds utcOffset = ReadUTCOffsetL();
	//pass legacy test code testexecute logger so that logging can go to the testexecute html file
	testCase = CTestFailed_2::NewL(*iTestUtils,utcOffset,*this);
	
	testCase->Start();
	CActiveScheduler::Start();
	
	//get the result
	SetTestStepResult(testCase->GetVerdict());
	delete testCase;	
	
	return TestStepResult();
	}	
	


