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


#include <hal_data.h>
#include <hal.h>
#include <profiler.h>
#include "T_PerformanceStepBase.h"
#include "T_Performance423Step.h"
#include "T_PerformanceInitialisationClient.h"


CPerformance423Step::~CPerformance423Step()
	{
	delete iImapClient;	
	}
	

CPerformance423Step::CPerformance423Step()
	{
	// Call base class method to set up the human readable name for logging
	SetTestStepName(KPerformance423);
	iProfile=EFalse;
	}

TVerdict CPerformance423Step::doTestStepL()
	{
	//start the imap client
	iImapClient = CPerformance423Client::NewL(*this);
	iImapClient->StartL();
	//start the test
	if(iProfile)
		{
		LoadProfiler();	
		}
	CActiveScheduler::Start();
	return EPass;
	}

TVerdict CPerformance423Step::doTestStepPreambleL()
	{
	CPerformanceStepBase::doTestStepPreambleL();
	
	CPerformanceInitialisationClient* initialisationClient;

	initialisationClient = CPerformanceInitialisationClient::NewL(*this);
	CleanupStack::PushL(initialisationClient); 
	
	initialisationClient->DeleteRemoteMessagesL();
	CActiveScheduler::Start();

	initialisationClient->CreateMessageL(KMsvGlobalOutBoxIndexEntryIdValue,1,iEmailBodyFile1,iPtrEmailaddress,iSubject,iNullFilePath,iNullFilePath,iNullFilePath);
	initialisationClient->UploadMessagesL();
	CActiveScheduler::Start();

	CleanupStack::PopAndDestroy(initialisationClient); 

	return EPass;
	}


TVerdict CPerformance423Step::doTestStepPostambleL()
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
	//get entry count
	TInt entryCount=GetEntryCount();
	INFO_PRINTF2(_L("Fetched %d email(s)."),entryCount);	
	
	if(entryCount==1)
		{
		return EPass;	
		}
	else
		{
		return EFail;		
		}

	}
