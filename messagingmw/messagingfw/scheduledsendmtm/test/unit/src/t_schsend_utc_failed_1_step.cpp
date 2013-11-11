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
#include "t_schsend_utc_failed_1_step.h"

CFailed1Step::CFailed1Step()
	{
	// Call base class method to set up the human readable name for logging
	SetTestStepName(KFailed1Step);
	}


TVerdict CFailed1Step::doTestStepL()
	{		
	CTestScheduleSendBase* testCase = NULL;
	
	TTimeIntervalSeconds utcOffset = ReadUTCOffsetL();
	//pass legacy test code testexecute logger so that logging can go to the testexecute html file
	testCase = CTestFailed_1::NewL(*iTestUtils,utcOffset,*this);
	
	testCase->Start();
	CActiveScheduler::Start();
	
	//get the result
	SetTestStepResult(testCase->GetVerdict());
	delete testCase;	
	
	return TestStepResult();
	}	
	


