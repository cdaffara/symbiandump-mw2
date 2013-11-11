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
 @file
*/
  
#include "CCreateServiceTestStep.h"
#include "CentralRepoUtils.h"

#include <msvapi.h> //MMsvSessionObserver
#include <mtclreg.h> //Client Registry
#include <smsclnt.h>
#include <smut.h> //KUidMsgTypeSMS

_LIT(KDescription, "Description");
_LIT(KDetails,"Details");
_LIT(KFileName, "FileName");

using namespace SharedData;

class CSessionObserver1 : public MMsvSessionObserver, public CBase
	{
public: 
	void HandleSessionEventL(TMsvSessionEvent,TAny*,TAny*,TAny*) {};
	};


/**
 Constructor to set test step name
*/
CCreateServiceTestStep::CCreateServiceTestStep(MSharedData& aObj)
:CBasicTestStep(aObj)
	{
	SetTestStepName(KCreateServiceTestStep);
	}


CCreateServiceTestStep::~CCreateServiceTestStep()
	{
	}


TVerdict CCreateServiceTestStep::doTestStepL()
	{
	
	CActiveScheduler* scheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install(scheduler);
	CleanupStack::PushL(scheduler);

	CSessionObserver1* sessionObserver1 = new (ELeave) CSessionObserver1;
	CleanupStack::PushL(sessionObserver1);
	
	CMsvSession* session = CMsvSession::OpenSyncL(*sessionObserver1);
	CleanupStack::PushL(session);
	
	TPtrC descriptionTag;
	if ( !GetStringFromConfig(ConfigSection(),KDescription,descriptionTag))
		{
		ERR_PRINTF1(_L("No Input for Description"));
		User::Leave(KErrNotReady);
		}
	
	TPtrC detailsTag;
	if ( !GetStringFromConfig(ConfigSection(),KDetails,detailsTag))
		{
		ERR_PRINTF1(_L("No Input for Details"));
		User::Leave(KErrNotReady);
		}

	CentralRepoUtils::CreateServiceL(session, descriptionTag, detailsTag);
	
	TPtrC tag;
	if ( !GetStringFromConfig(ConfigSection(),KFileName,tag))
		{
		ERR_PRINTF1(_L("No Input for Details"));
		User::Leave(KErrNotReady);
		}

	CentralRepoUtils::SetSmsServiceSettingsL(tag);

	CleanupStack::PopAndDestroy(3,scheduler); //session, sessionObserver1, scheduler
	SetTestStepResult(EPass);
	
	return TestStepResult();
	}

