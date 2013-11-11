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
#include "CCreateReplyTestStep.h"
#include "MSharedData.h"
#include "CentralRepoUtils.h"
#include "smsclnt.h"
#include <smut.h>
#include "MsgingUtils.h"

#include <txtetext.h>
#include <txtfmlyr.h>
#include <txtrich.h>
#include <smuthdr.h>

#include "CAsyncWaiter.h"

using namespace SharedData;

class CSessionObserver1 : public MMsvSessionObserver, public CBase
	{
public: 
	void HandleSessionEventL(TMsvSessionEvent,TAny*,TAny*,TAny*) {};
	};

/**
 Constructor to set test step name
*/
CCreateReplyTestStep::CCreateReplyTestStep(MSharedData& aObj)
:CBasicTestStep(aObj)
	{
	SetTestStepName(KCreateReplyTestStep);
	}

CCreateReplyTestStep::~CCreateReplyTestStep()
	{
	}

TVerdict CCreateReplyTestStep::doTestStepL()
	{
	CActiveScheduler* scheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install(scheduler);
	CleanupStack::PushL(scheduler);

	CSessionObserver1* sessionObserver1 = new (ELeave) CSessionObserver1;
	CleanupStack::PushL(sessionObserver1);
	CMsvSession* session = CMsvSession::OpenSyncL(*sessionObserver1);
	CleanupStack::PushL(session);

	_LIT(KParent, "Parent");
	_LIT(KDestination, "Destination");

	TPtrC tag;
	if ( !GetStringFromConfig(ConfigSection(),KParent,tag))
		{
		ERR_PRINTF1(_L("No Input for Inbox "));
		User::Leave(KErrNotReady);
		}
	TMsvId parentId = MsgingUtils::GetLocalFolderId(tag);
		
	if ( !GetStringFromConfig(ConfigSection(),KDestination,tag))
		{
		ERR_PRINTF1(_L("No Input for OutBox "));
		User::Leave(KErrNotReady);
		}
	TMsvId destId = MsgingUtils::GetLocalFolderId(tag);

	_LIT(KDetails,"Details");
	
	TPtrC detailsTag;

	if ( !GetStringFromConfig(ConfigSection(),KDetails,detailsTag))
	{
	ERR_PRINTF1(_L("No Input for Details"));
	User::Leave(KErrNotReady);
	}
	
//Details = +358408415528
//Description = Reply Address message. Message
	TMsvId id = MsgingUtils::GetEntryByNameL(detailsTag, parentId, *session);
	CMsvEntry* entry = session->GetEntryL(id);
	CleanupStack::PushL(entry);
	
	CClientMtmRegistry* iMtmRegistry = CClientMtmRegistry::NewL(*session);
	CSmsClientMtm* smsMtm = (CSmsClientMtm*)CentralRepoUtils::CreateMtmL(KUidMsgTypeSMS, iMtmRegistry);
	CleanupStack::PushL(smsMtm);
	
	entry->SetEntryL(id);
//Creating reply
	smsMtm->SwitchCurrentEntryL(id); // Lets point at the received message
	smsMtm->LoadMessageL();

	CAsyncWaiter* waiter = CAsyncWaiter::NewL();
	CleanupStack::PushL(waiter);
	
	TMsvPartList part=0;
	CMsvOperation* op=smsMtm->ReplyL(destId, part, waiter->iStatus);
	CleanupStack::PushL(op);
	waiter->StartAndWait();
	User::LeaveIfError(waiter->Result());

	TPckgBuf<TMsvId> pkg;
	pkg.Copy(op->ProgressL());
	TMsvId progress = pkg();
	smsMtm->SwitchCurrentEntryL(progress);
	smsMtm->LoadMessageL();
		
	smsMtm->Body().InsertL(0,_L("This is reply"));
	smsMtm->SaveMessageL();

	CleanupStack::PopAndDestroy(3,smsMtm); //op, waiter, smsMtm
	
	delete iMtmRegistry;
	
	CleanupStack::PopAndDestroy(entry);
//End of Reply Creation
	
	CleanupStack::PopAndDestroy(3, scheduler); // session, sessionObserver1, scheduler

	return TestStepResult();
	}
