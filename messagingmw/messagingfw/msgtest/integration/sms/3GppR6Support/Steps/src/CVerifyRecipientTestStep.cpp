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
  
#include "CVerifyRecipientTestStep.h"
#include "MSharedData.h"
#include "CentralRepoUtils.h"
#include "smsclnt.h"
#include <smut.h>
#include "MsgingUtils.h"

#include <txtetext.h>
#include <txtfmlyr.h>
#include <txtrich.h>
#include <smuthdr.h>

using namespace SharedData;

//#include "CAsyncWaiter.h"

class CSessionObserver1 : public MMsvSessionObserver, public CBase
	{
public: 
	void HandleSessionEventL(TMsvSessionEvent,TAny*,TAny*,TAny*) {};
	};

/**
 Constructor to set test step name
*/
CVerifyRecipientTestStep::CVerifyRecipientTestStep(MSharedData& aObj)
:CBasicTestStep(aObj)
	{
	SetTestStepName(KVerifyRecipientTestStep);
	}


CVerifyRecipientTestStep::~CVerifyRecipientTestStep()
	{
	}



TVerdict CVerifyRecipientTestStep::doTestStepL()
	{
	CActiveScheduler* scheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install(scheduler);
	CleanupStack::PushL(scheduler);

	CSessionObserver1* sessionObserver1 = new (ELeave) CSessionObserver1;
	CleanupStack::PushL(sessionObserver1);
	CMsvSession* session = CMsvSession::OpenSyncL(*sessionObserver1);
	CleanupStack::PushL(session);
	
	_LIT(KParent, "Parent");
	TPtrC tag;
	if ( !GetStringFromConfig(ConfigSection(),KParent,tag))
		{
		ERR_PRINTF1(_L("No Input for OutBox "));
		User::Leave(KErrNotReady);
		}
	
	TMsvId parentId = MsgingUtils::GetLocalFolderId(tag);
//Looking up for the msg on Outbox
	TMsvSelectionOrdering ordering;	
	ordering.ShowInvisibleEntries();
	CMsvEntry* entry = CMsvEntry::NewL(*session, parentId, ordering); // FolderId should be Inbox
	CleanupStack::PushL(entry);
	
	CMsvEntrySelection* msvEntrySelection;
	msvEntrySelection=entry->ChildrenL();
	CleanupStack::PushL(msvEntrySelection);
	
	// Check how many mesasges there are
	TInt count = msvEntrySelection->Count();
	INFO_PRINTF2(_L("Number of new messages: %d "), count);

			// Create a Rich Text object
	CPlainText::TTextOrganisation ttOrg = {CPlainText::EOrganiseByLine};

	CParaFormatLayer* paraFormatLayer = CParaFormatLayer::NewL();
	CleanupStack::PushL(paraFormatLayer);
	CCharFormatLayer* charFormatLayer = CCharFormatLayer::NewL();
	CleanupStack::PushL(charFormatLayer);
	CRichText* bodyRichText = CRichText::NewL(paraFormatLayer, charFormatLayer);
	CleanupStack::PushL(bodyRichText);


	// Restore the message
	CSmsHeader* smsHeader = CSmsHeader::NewL(CSmsPDU::ESmsSubmit, *bodyRichText);
	CleanupStack::PushL(smsHeader);
	entry->SetEntryL((*msvEntrySelection)[0]);

	CMsvStore* store = entry->ReadStoreL();
	CleanupStack::PushL(store);
	smsHeader->RestoreL(*store);
	
	const CArrayPtrFlat<CSmsNumber>& rcpts = smsHeader->Recipients();
	count = rcpts.Count();

	const CSmsNumber& rcpt = *rcpts.At(0);
	
	_LIT(KRecipientAddr, "RecipientAddr");
	
	TPtrC rcptAddr;// = aHeader.ReplyAddressL();
	if ( !GetStringFromConfig(ConfigSection(),KRecipientAddr,rcptAddr))
		{
		ERR_PRINTF1(_L("No Recipient Address"));
		User::Leave(KErrNotReady);
		}

	if(rcptAddr.CompareF(rcpt.Address()) != 0)
		{
		ERR_PRINTF1(_L("Reply Address Not same"));
		SetTestStepResult(EFail);
	//	return EFail;
		}

	CleanupStack::PopAndDestroy(7,entry);
	
	CleanupStack::PopAndDestroy(3, scheduler);

	return TestStepResult();
	}
