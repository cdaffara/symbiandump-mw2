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
  
#include "CCreateSmsMessageTestStep.h"

#include <msvstd.h> //TMsvEntry
#include <smut.h> //TSmsUtitlies
#include <msvuids.h> //KUidMsvMessageEntry
#include <smuthdr.h> //SmsHeader
#include <txtetext.h> //CPlainText
#include <txtfmlyr.h> //CParaFormat
#include <txtrich.h> //CRichText
#include <csmsaccount.h>

#include "CTimerUtils.h"
#include "MsgingUtils.h"
#include "CAsyncWaiter.h"

using namespace SharedData;


class CSessionObserver : public MMsvSessionObserver, public CBase
	{
public: 
	void HandleSessionEventL(TMsvSessionEvent,TAny*,TAny*,TAny*) {};
	};

/**
 Constructor to set test step name
*/
CCreateSmsMessageTestStep::CCreateSmsMessageTestStep(MSharedData& aObj)
:CBasicTestStep(aObj)
	{
	SetTestStepName(KCreateSmsMessageTestStep);
	}


CCreateSmsMessageTestStep::~CCreateSmsMessageTestStep()
	{
	}


TVerdict CCreateSmsMessageTestStep::doTestStepL()
	{

	CActiveScheduler* scheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install(scheduler);
	CleanupStack::PushL(scheduler);

	CSessionObserver* sessionObserver = new (ELeave) CSessionObserver;
	CleanupStack::PushL(sessionObserver);
	CMsvSession* session = CMsvSession::OpenSyncL(*sessionObserver);
	CleanupStack::PushL(session);
	
	_LIT(KFileName,"FileName");
	TPtrC tag;
	if ( !GetStringFromConfig(ConfigSection(),KFileName,tag))
		{
		ERR_PRINTF1(_L("No Input for FileName in CreateSmsMessage"));
		User::Leave(KErrNotReady);
		}
	
	HBufC* fileName = tag.AllocLC();
	

	// Create a Rich Text object
	CPlainText::TTextOrganisation ttOrg = {CPlainText::EOrganiseByLine};

	CParaFormatLayer* paraFormatLayer = CParaFormatLayer::NewL();
	CleanupStack::PushL(paraFormatLayer);
	CCharFormatLayer* charFormatLayer = CCharFormatLayer::NewL();
	CleanupStack::PushL(charFormatLayer);
	CRichText* bodyRichText = CRichText::NewL(paraFormatLayer, charFormatLayer);
	CleanupStack::PushL(bodyRichText);
	
	// Store the file contents into the CRichText object
	bodyRichText->ImportTextFileL(0, fileName->Des(), ttOrg);
	
	
	// Create the SMS header object...
	CSmsHeader* header = CSmsHeader::NewL(CSmsPDU::ESmsSubmit, *bodyRichText);
	CleanupStack::PushL(header);
	
	// Set the body text...
	CSmsSettings* smsSettings = CSmsSettings::NewL();
	CleanupStack::PushL(smsSettings);

	CSmsAccount* account = CSmsAccount::NewLC();
	account->LoadSettingsL(*smsSettings);
	// Copy the message settings...
	header->SetSmsSettingsL(*smsSettings); 
	
	// Set the service centre
	TInt defaultIndex = smsSettings->DefaultServiceCenter();
	header->SetServiceCenterAddressL(smsSettings->GetServiceCenter(defaultIndex).Address());

	// Set recipient - ask derived class
	SetRecipientsL(*header);
	CArrayPtrFlat<CSmsNumber>& recipient = header->Recipients();

	INFO_PRINTF1(_L("Creating message..."));
	
	TMsvEntry entry;
	entry.SetVisible(ETrue); 
	entry.SetInPreparation(ETrue);
	TMsvId srvcId=0;
	TSmsUtilities::ServiceIdL(*session, srvcId);
	entry.iServiceId = srvcId;
	entry.iType = KUidMsvMessageEntry; 
	entry.iMtm = KUidMsgTypeSMS; 
	entry.iDate.UniversalTime();
	entry.iSize = 0; 
	entry.iDescription.Set(KNullDesC); 
	entry.iDetails.Set(KNullDesC); 
	entry.SetSendingState(KMsvSendStateWaiting);

	// Update entry description and details...
	entry.iDetails.Set(recipient[0]->Address());
	entry.iDescription.Set(recipient[0]->Address());
	entry.SetInPreparation(EFalse);

	//TPtrC tag;
	_LIT(KParent,"Parent");
	TPtrC parentTag;
	if ( !GetStringFromConfig(ConfigSection(),KParent,parentTag))
		{
		ERR_PRINTF1(_L("No Input for Outbox"));
		User::Leave(KErrNotReady);
		}
	// Create the entry - set context to the global outbox.
	TMsvId paramParentId =	MsgingUtils::GetLocalFolderId(parentTag);//KMsvGlobalOutBoxIndexEntryId;

	CMsvEntry* newEntry = CMsvEntry::NewL(*session,paramParentId,TMsvSelectionOrdering());
	CleanupStack::PushL(newEntry);
	newEntry->SetEntryL(paramParentId);
	
	newEntry->CreateL(entry);

	// Create new store and save header information
	newEntry->SetEntryL(entry.Id());
	CMsvStore* store = newEntry->EditStoreL();
	CleanupStack::PushL(store);
	header->StoreL(*store);
	store->StoreBodyTextL(*bodyRichText);
	store->CommitL();

//store,newEntry,account, smsSettings, header, bodyRichText,charFormatLayer, paraFormatLayer,fileName
	CleanupStack::PopAndDestroy(9,fileName); 
	
	CleanupStack::PopAndDestroy(3, scheduler);
	
	SetTestStepResult(EPass);
	
	return TestStepResult();
	}
	
void CCreateSmsMessageTestStep::SetRecipientsL(CSmsHeader& aHeader)
	{
	INFO_PRINTF1(_L("--Setting single recipient\n"));
	_LIT(KRecipient, "SentToAddress");
	
	TPtrC rcptTag;
	if ( !GetStringFromConfig(ConfigSection(),KRecipient,rcptTag))
		{
		ERR_PRINTF1(_L("No Input for FileName in CreateSmsMessage"));
		User::Leave(KErrNotReady);
		}
	
	HBufC* recipientAdd = rcptTag.AllocLC();
		
	CSmsNumber* rcpt = CSmsNumber::NewL();
	CleanupStack::PushL(rcpt);
	
	rcpt->SetAddressL(*recipientAdd);
	aHeader.Recipients().AppendL(rcpt);
	CleanupStack::Pop(rcpt);
	
	CleanupStack::PopAndDestroy(recipientAdd);
	}

