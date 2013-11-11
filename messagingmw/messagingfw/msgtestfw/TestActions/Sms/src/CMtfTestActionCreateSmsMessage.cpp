// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// __ACTION_INFO_BEGIN__ 
// [Action Name]
// CreateSmsMessage
// [Action Parameters]
// Session        <input>: Reference to the session.
// ParentId       <input>: Value of the parent id.
// ServiceId      <input>: Value of the the service id.
// (ConfigIndexHeader)	  <input>: Value of config index for SMS header settings. Default is 0.
// MessageId     <output>: Value of the created message id.
// [Action Description]
// Creates a message on the specified parent.
// [APIs Used]	
// TMsvEntry::iMtm	
// TMsvEntry::iServiceId
// TMsvEntry::iDate
// TMsvEntry::Id
// CMsvEntry::SetEntryL
// CMsvEntry::CreateL
// CSmsHeader::NewL
// CSmsHeader::StoreL
// __ACTION_INFO_END__
// 
//

// 
#include "CMtfTestActionCreateSmsMessage.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "CMtfTestActionUtilsConfigFileParser.h"
#include <msvapi.h>
#include <msvuids.h>
#include <txtrich.h>
#include <smuthdr.h>


_LIT(KSentToAddress, "SentToAddress");
_LIT(KBodyText, "BodyText");

CMtfTestAction* CMtfTestActionCreateSmsMessage::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionCreateSmsMessage* self = new (ELeave) CMtfTestActionCreateSmsMessage(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}
	

CMtfTestActionCreateSmsMessage::CMtfTestActionCreateSmsMessage(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}

void CMtfTestActionCreateSmsMessage::ExecuteActionL()
	{
	TestCase().Logger().Write(_L("CMtfTestActionCreateSmsMessage::ExecuteActionL IN"));
	TInt paramCount = ActionParameters().Count();
	if( paramCount != 5)
		{
		User::Leave(KErrArgument);
		}
		
	CMsvSession* paramSession = ObtainParameterReferenceL<CMsvSession>(TestCase(),ActionParameters().Parameter(0));
	TMsvId paramParentId = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(1));
	TMsvId paramServiceId = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(2));
	
	
	TMsvEntry indexEntry; 
	indexEntry.iType = KUidMsvMessageEntry;
	indexEntry.iMtm = KUidMsgTypeSMS;	
	indexEntry.iServiceId = paramServiceId;
	indexEntry.SetVisible(ETrue);	
//	indexEntry.iDate.HomeTime();
	indexEntry.iDate.UniversalTime();
	indexEntry.SetSendingState(KMsvSendStateWaiting);
	CMsvEntry* entry = paramSession->GetEntryL(paramParentId);
	CleanupStack::PushL(entry);
	entry->CreateL(indexEntry);
	TMsvId paramMessageId = indexEntry.Id();
	paramSession->CleanupEntryPushL(paramMessageId);
	entry->SetEntryL(paramMessageId);
	
	InitializeHeaderFromConfigL(entry);

	paramSession->CleanupEntryPop();
	CleanupStack::PopAndDestroy(entry);

	StoreParameterL<TMsvId>(TestCase(),paramMessageId,ActionParameters().Parameter(4));
	
	TestCase().Logger().Write(_L("CMtfTestActionCreateSmsMessage::ExecuteActionL OUT"));
	TestCase().ActionCompletedL(*this);
	}


void CMtfTestActionCreateSmsMessage::InitializeHeaderFromConfigL(CMsvEntry* aEntry)	
	{
	TInt paramConfigIndexHeader = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(3),0);
	TPtrC smsMessageFileName = TestCase().GetConfigurationFileL(CMtfConfigurationType::EMtfSmsMessage,paramConfigIndexHeader);
	CMtfTestActionUtilsConfigFileParser* configParser = CMtfTestActionUtilsConfigFileParser::NewL(smsMessageFileName);
	CleanupStack::PushL(configParser);
	
	CParaFormatLayer* paraFormatLayer = CParaFormatLayer::NewL();
	CleanupStack::PushL(paraFormatLayer);
	
	CCharFormatLayer* charFormatLayer = CCharFormatLayer::NewL();
	CleanupStack::PushL(charFormatLayer);
	
	CRichText* richText = CRichText::NewL(paraFormatLayer,charFormatLayer);
	CleanupStack::PushL(richText);
	
	CSmsHeader* header = CSmsHeader::NewL(CSmsPDU::ESmsSubmit, *richText);
	CleanupStack::PushL(header);

	CMsvStore* store = aEntry->EditStoreL();
	CleanupStack::PushL(store);
	
	TPtrC toAddress;
	if(configParser->GetFieldAsString(KSentToAddress,toAddress) != KErrNotFound)
		{
		CArrayPtrFlat<CSmsNumber>& recipientList = header->Recipients();
		CSmsNumber* recipient = CSmsNumber::NewL();
		CleanupStack::PushL(recipient);
		recipient->SetAddressL(toAddress);
		CleanupStack::Pop(recipient);
		recipientList.AppendL(recipient);	
		}	
	TestCase().Logger().WriteFormat(_L("CMtfTestActionCreateSmsMessage:: recipient: %S"), &toAddress);

	TPtrC bodyText;
	if(configParser->GetFieldAsString(KBodyText, bodyText) != KErrNotFound)
		{
		richText->InsertL(0, bodyText);
		}
	TestCase().Logger().WriteFormat(_L("CMtfTestActionCreateSmsMessage:: body text: %S"), &bodyText);
	
	header->StoreL(*store);
	store->StoreBodyTextL(*richText);
	store->CommitL();
		
	CleanupStack::PopAndDestroy(6, configParser);
	}

