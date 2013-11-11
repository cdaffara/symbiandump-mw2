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
  
#include "CAddIndicationTestStep.h"

#include <msvstd.h> //TMsvId
#include <smuthdr.h> //SmsHdr
#include <gsmuieoperations.h>
#include <msvapi.h>
#include <txtetext.h> //CPlainText
#include <txtfmlyr.h> //CParaFormat
#include <txtrich.h> //CRichText

#include "MsgingUtils.h"

using namespace SharedData;

class CSessionObserver1 : public MMsvSessionObserver, public CBase
	{
public: 
	void HandleSessionEventL(TMsvSessionEvent,TAny*,TAny*,TAny*) {};
	};

/**
 Constructor to set test step name
*/
CAddIndicationTestStep::CAddIndicationTestStep(MSharedData& aObj)
:CBasicTestStep(aObj)
	{
	SetTestStepName(KAddIndicationTestStep);
	}


CAddIndicationTestStep::~CAddIndicationTestStep()
	{
	}

/** Instructions To Use
A>	If either Input of Video, VoiceMail,Other or Reply is equal To 0 then that function will not be called
B>	The input count for Video and Voice would be used to generate that number of indication Messages
C>	For Other Indications, if Multiple indications need to be generated then the teststep should be called 
	that number of times with other inputs set to zero
*/
TVerdict CAddIndicationTestStep::doTestStepL()
	{

	CActiveScheduler* scheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install(scheduler);
	CleanupStack::PushL(scheduler);
	
	CSessionObserver1* sessionObserver1 = new(ELeave) CSessionObserver1;
	CleanupStack::PushL(sessionObserver1);
	CMsvSession* session = CMsvSession::OpenSyncL(*sessionObserver1);
	CleanupStack::PushL(session);
	
	_LIT(KParent,"Parent");
	TPtrC parentTag;
	if ( !GetStringFromConfig(ConfigSection(),KParent,parentTag))
		{
		ERR_PRINTF1(_L("No Input for Outbox"));
		User::Leave(KErrNotReady);
		}

	// LookUp for the FolderId.
	TMsvId paramParentId =	MsgingUtils::GetLocalFolderId(parentTag);//KMsvGlobalOutBoxIndexEntryId;
	
	_LIT(KSubject,"Subject");
	TPtrC subjectTag;
	if ( !GetStringFromConfig(ConfigSection(),KSubject,subjectTag))
		{
		ERR_PRINTF1(_L("No Input for Subject in AddIndication"));
		User::Leave(KErrNotReady);
		}

	TInt videoCnt;
	_LIT(KVideo,"Video");
	if ( !GetIntFromConfig(ConfigSection(),KVideo,videoCnt))
		{
		ERR_PRINTF1(_L("No Video Count"));		
		User::Leave(KErrNotReady);
		}
	
	TInt voiceMailCnt;
	_LIT(KVoiceMail,"VoiceMail");
	if ( !GetIntFromConfig(ConfigSection(),KVoiceMail,voiceMailCnt))
		{
		ERR_PRINTF1(_L("No VoiceMail Count"));		
		User::Leave(KErrNotReady);
		}
	
	TInt otherCnt;
	_LIT(KOther,"Other");
	if ( !GetIntFromConfig(ConfigSection(),KOther,otherCnt))
		{
		ERR_PRINTF1(_L("No Other Count"));		
		User::Leave(KErrNotReady);
		}
	
	TInt replyCnt;
	_LIT(KReply,"Reply");
	if ( !GetIntFromConfig(ConfigSection(),KReply,replyCnt))
		{
		ERR_PRINTF1(_L("No Reply Count"));		
		User::Leave(KErrNotReady);
		}
		
		// Create a Rich Text object
	CPlainText::TTextOrganisation ttOrg = {CPlainText::EOrganiseByLine};

	CParaFormatLayer* paraFormatLayer = CParaFormatLayer::NewL();
	CleanupStack::PushL(paraFormatLayer);
	CCharFormatLayer* charFormatLayer = CCharFormatLayer::NewL();
	CleanupStack::PushL(charFormatLayer);
	CRichText* bodyRichText = CRichText::NewL(paraFormatLayer, charFormatLayer);
	CleanupStack::PushL(bodyRichText);

	CSmsHeader* smsHeader = CSmsHeader::NewL(CSmsPDU::ESmsDeliver, *bodyRichText);
	CleanupStack::PushL(smsHeader);
	
	CMsvEntry* entry = session->GetEntryL(MsgingUtils::GetEntryByNameL(subjectTag, paramParentId, *session));
	CleanupStack::PushL(entry);
	
	CMsvStore* store = entry->ReadStoreL();
	CleanupStack::PushL(store);	
	smsHeader->RestoreL(*store);
	
	if(store->HasBodyTextL())
		{
		store->RestoreBodyTextL(*bodyRichText);	
		}
	TInt err=0;
	SetTestStepResult(EPass);
	
	TRAP(err,ClearAllSpecialMsgIndicationsL(*smsHeader));
	
	if((err == KErrNone) && (videoCnt > 0))
		{
		TRAP(err,AddVideoIndicationL(*smsHeader, videoCnt));
		}
	if((err == KErrNone)&& (voiceMailCnt > 0))
		{
		TRAP(err,AddVoiceMailIndicationL(*smsHeader, voiceMailCnt));
		}
	if((err == KErrNone)&&(replyCnt > 0))
		{
		TPtrC replyAddress;
		_LIT(KReplyAddress,"ReplyAddress");
		if ( !GetStringFromConfig(ConfigSection(),KReplyAddress,replyAddress))
			{
			ERR_PRINTF1(_L("No replyAddress "));		
			User::Leave(KErrNotReady);
			}
		TRAP(err,AddReplyToAddressL(*smsHeader, replyAddress));
		}
	if((err == KErrNone) && (otherCnt))
		{
		TRAP(err,AddFaxIndicationL(*smsHeader));
		}

	if(err != KErrNone)
		{
		SetTestStepResult(EFail);
		}
	
	CleanupStack::PopAndDestroy(6,paraFormatLayer); //store, entry, smsHeader, bodyRichText, charFormatLayer, paraFormatLayer

	CleanupStack::PopAndDestroy(3,scheduler); //session, sessionObserver1, scheduler

	return TestStepResult();
	}
//Clears All Special Message Indications
void CAddIndicationTestStep::ClearAllSpecialMsgIndicationsL(CSmsHeader& aHeader)
	{
	INFO_PRINTF1(_L("Clearing All Special Msg Indications"));

	CSmsMessage& smsMsg = aHeader.Message();
 
	CSmsSpecialSMSMessageOperations& operations = STATIC_CAST(CSmsSpecialSMSMessageOperations&,smsMsg.GetOperationsForIEL(CSmsInformationElement::ESmsIEISpecialSMSMessageIndication));

	TRAPD(err,operations.RemoveAllSpecialMessageIndicationsL());
	if(err != KErrNone)
		{
		SetTestStepResult(EFail);
		}

	TInt count = operations.GetCountOfSpecialMessageIndicationsL();
	INFO_PRINTF2(_L("**Count of Special Msg Indications After all the Indications are cleared = %d"),count);
	}

//Adds VoiceMail Indication to the Message
void CAddIndicationTestStep::AddVideoIndicationL(CSmsHeader& aHeader, TInt aMsgCount)
	{
	INFO_PRINTF1(_L("Adding Video Msg"));
		//Get the operations
	CSmsSpecialSMSMessageOperations& operations = STATIC_CAST(CSmsSpecialSMSMessageOperations&,aHeader.Message().GetOperationsForIEL(CSmsInformationElement::ESmsIEISpecialSMSMessageIndication));
	
	TBool toStore=ETrue;
	TSmsMessageIndicationType messageIndicationType=EGsmSmsExtendedMessageTypeWaiting;
	TExtendedSmsIndicationType extendedType=EGsmSmsVideoMessageWaiting;
	TSmsMessageProfileType messageProfileType=EGsmSmsProfileId1;
	
	TRAPD(err,operations.AddSpecialMessageIndicationL(toStore,messageIndicationType,
								extendedType,messageProfileType,aMsgCount));
	if(err != KErrNone)
		{
		SetTestStepResult(EFail);
		}

	TInt count = operations.GetCountOfSpecialMessageIndicationsL();
	INFO_PRINTF2(_L("**Count of Special Msg Indications After Adding Video Msg are %d"),count);
	}

//Adds VoiceMail Indication to the Message
void CAddIndicationTestStep::AddVoiceMailIndicationL(CSmsHeader& aHeader, TInt aMsgCount)
	{
	INFO_PRINTF1(_L("Adding Voice Mail Indication"));

	CSmsEnhancedVoiceMailOperations& enhancedOperations = STATIC_CAST(CSmsEnhancedVoiceMailOperations&,aHeader.Message().GetOperationsForIEL(CSmsInformationElement::ESmsEnhanceVoiceMailInformation));

	CEnhancedVoiceMailNotification* enhancedVoiceMailNotification=CEnhancedVoiceMailNotification::NewL();
	CleanupStack::PushL(enhancedVoiceMailNotification);

	enhancedVoiceMailNotification->SetNumberOfVoiceMessages(aMsgCount);
	TRAPD(err,enhancedOperations.AddEnhancedVoiceMailIEL(*enhancedVoiceMailNotification));
	if(err != KErrNone)
		{
		SetTestStepResult(EFail);
		}

	CleanupStack::PopAndDestroy(enhancedVoiceMailNotification);

	CSmsSpecialSMSMessageOperations& operations = STATIC_CAST(CSmsSpecialSMSMessageOperations&,aHeader.Message().GetOperationsForIEL(CSmsInformationElement::ESmsIEISpecialSMSMessageIndication));

	TInt count = operations.GetCountOfSpecialMessageIndicationsL();
	INFO_PRINTF2(_L("**Count of Special Msg Indications after VoiceMail Indication are %d"),count);
	}

//Adds ReplyTo Address to the Message
void CAddIndicationTestStep::AddReplyToAddressL(CSmsHeader& aHeader,const TDesC& aReplyAddress)
	{
	INFO_PRINTF1(_L("Adding ReplyTo Address"));

	CSmsSpecialSMSMessageOperations& operations = STATIC_CAST(CSmsSpecialSMSMessageOperations&,aHeader.Message().GetOperationsForIEL(CSmsInformationElement::ESmsIEISpecialSMSMessageIndication));
	
	CSmsReplyAddressOperations& replyOperations = STATIC_CAST(CSmsReplyAddressOperations&,aHeader.Message().GetOperationsForIEL(CSmsInformationElement::ESmsReplyAddressFormat));		
	//Add address

	TRAPD(err,replyOperations.AddReplyAddressL(aReplyAddress));
	if(err != KErrNone)
		{
		SetTestStepResult(EFail);
		}

	TInt count = operations.GetCountOfSpecialMessageIndicationsL();
	INFO_PRINTF2(_L("**Count of Special Msg Indications after ReplyTo addition are %d"),count);
	}

// Adds Fax Indication to the Message
void CAddIndicationTestStep::AddFaxIndicationL(CSmsHeader& aHeader)
	{
	INFO_PRINTF1(_L("Adding Fax Indication"));
	
	TBuf8<2> msgInd;
	msgInd.Copy(_L("AA"));
	
	CSmsPDU& pdu = aHeader.Message().SmsPDU();
	CSmsUserData& userData = pdu.UserData();
	
	//Add a fax waiting notification (1 fax)
	msgInd[0] = TSmsUtilities::EFaxMessageWaiting;
	msgInd[1] = 0x00000001;
	
	TRAPD(err,userData.AddInformationElementL(CSmsInformationElement::ESmsIEISpecialSMSMessageIndication, msgInd));
	if(err != KErrNone)
		{
		SetTestStepResult(EFail);
		}

	CSmsSpecialSMSMessageOperations& operations = STATIC_CAST(CSmsSpecialSMSMessageOperations&,aHeader.Message().GetOperationsForIEL(CSmsInformationElement::ESmsIEISpecialSMSMessageIndication));

	TInt count = operations.GetCountOfSpecialMessageIndicationsL();
	INFO_PRINTF2(_L("**Count of Special Msg Indications after Fax are %d"),count);
	}

