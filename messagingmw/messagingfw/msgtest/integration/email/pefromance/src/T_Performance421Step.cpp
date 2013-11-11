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

#include "T_Performance421Step.h"
#include <hal_data.h>
#include <hal.h>
#include <profiler.h>


CPerformance421Step::~CPerformance421Step()
	{
	delete iImapClient;	
	}
	
CPerformance421Step::CPerformance421Step()
	{
	// Call base class method to set up the human readable name for logging
	SetTestStepName(KPerformance421);
	iProfile=EFalse;
	}

TVerdict CPerformance421Step::doTestStepL()
	{
	//start the imap client
	iImapClient = CPerformance421Client::NewL(*this);
	iImapClient->StartL();
	//start the test
	if(iProfile)
		{
		LoadProfiler();	
		}
	CActiveScheduler::Start();
	return TestStepResult();
	}

TVerdict CPerformance421Step::doTestStepPostambleL()
	{
	if(iProfile)
		{
		Profiler::Stop();
		Profiler::Close();
		Profiler::Unload();	
		}
	
	TInt frequency;
	HAL::Get(HAL::EFastCounterFrequency, frequency);	
	iDuration=(TReal64)(iStopCount-iStartCount)/frequency;
	INFO_PRINTF2(_L("Elaped time = %f seconds."),iDuration);	
	return TestStepResult();
	}


