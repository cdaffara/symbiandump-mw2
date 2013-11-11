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
  
#include "CVerifyIndicationTestStep.h"

#include <smuthdr.h> //SmsHdr
#include <msvapi.h>
#include <txtetext.h>
#include <txtfmlyr.h>
#include <txtrich.h>
#include <e32cmn.h>

#include "MSharedData.h"
#include "MsgingUtils.h"
#include "CAsyncWaiter.h"
#include "CTimerUtils.h"

using namespace SharedData;

class CSessionObserver1 : public MMsvSessionObserver, public CBase
	{
public: 
	void HandleSessionEventL(TMsvSessionEvent,TAny*,TAny*,TAny*) {};
	};

void CVerifyIndicationTestStep::SetObserver(MMsvEntryObserver* aObserver)
	{
	iObserver = aObserver;
	}

/**
 Constructor to set test step name
*/
CVerifyIndicationTestStep ::CVerifyIndicationTestStep (MSharedData& aObj)
:CBasicTestStep(aObj)
	{
	SetTestStepName(KVerifyIndicationTestStep);
	}


CVerifyIndicationTestStep ::~CVerifyIndicationTestStep ()
	{
	}

/** Instructions To Use
A>	If either Input of Video,VoiceMail,Other or Reply is equal To 0 then 
	that function will not be called
B>	The input count for Video + EnhancedVoiceMail + Email + Fax + Voice + other
	is greater than one we will return only the count of number of msgs waiting
C>	For any single type check we will be using individual Functions
*/
TVerdict CVerifyIndicationTestStep ::doTestStepL()
	{
	CActiveScheduler* scheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install(scheduler);
	CleanupStack::PushL(scheduler);
	
	CSessionObserver1* sessionObserver1 = new(ELeave) CSessionObserver1;
	CMsvSession* session = CMsvSession::OpenSyncL(*sessionObserver1);

	TBool result = ETrue;

	CAsyncWaiter* waiter = CAsyncWaiter::NewL();
	CleanupStack::PushL(waiter);

	CTimerUtils* timer = CTimerUtils::NewL();
	CleanupStack::PushL(timer);

	CMsvEntry* observeEntry = session->GetEntryL(MsgingUtils::GetLocalFolderId(KInbox));
	CleanupStack::PushL(observeEntry);
	observeEntry->AddObserverL(*( (CEntryCmdHandler*)iSharedData.GetObserverHandle() ) );
		
	//Needed to Wait for No Of Msgs to be rcvd
	_LIT(KRcvCount,"RcvCount");
	TInt rcvCountTag;
	if ( !GetIntFromConfig(ConfigSection(),KRcvCount,rcvCountTag))
		{
		ERR_PRINTF1(_L("No Input for Rcv Msg Count"));
		User::Leave(KErrNotReady);
		}
	TInt handlerCount = ((CEntryCmdHandler *)iSharedData.GetObserverHandle())->iCount;
	
	while(handlerCount != rcvCountTag)
		{
		timer->After(5,waiter->iStatus);
		waiter->StartAndWait();
		User::LeaveIfError(waiter->Result());
		handlerCount = ((CEntryCmdHandler *)iSharedData.GetObserverHandle())->iCount;
		}
	CleanupStack::PopAndDestroy(observeEntry);
	CleanupStack::PopAndDestroy(2,waiter);

	TPtrC parentTag;
	_LIT(KParent,"Parent");
	if ( !GetStringFromConfig(ConfigSection(),KParent,parentTag))
		{
		ERR_PRINTF1(_L("No parent in Verify Indications"));
		User::Leave(KErrNotReady);
		}
	TMsvId folderId = MsgingUtils::GetLocalFolderId(parentTag); //From Config FILE

	TMsvSelectionOrdering ordering;	
	ordering.ShowInvisibleEntries();
	CMsvEntry* entry = CMsvEntry::NewL(*session, folderId ,ordering); // FolderId should be Inbox
	CleanupStack::PushL(entry);
	
	CMsvEntrySelection* msvEntrySelection;
	msvEntrySelection=entry->ChildrenL();
	CleanupStack::PushL(msvEntrySelection);
	
	// Check how many mesasges there are
	TInt count = msvEntrySelection->Count();
	INFO_PRINTF2(_L("Number of new messages: %d "), count);
	
	//Loop for Multiple Msgs
	for(TInt i=0; (i< count) && (result); i++)
		{
		TInt videoCnt = -1;
		_LIT(KVideo,"Video%d");
		TBuf<10> videoBuf;
		videoBuf.Format(KVideo,i);
		if ( !GetIntFromConfig(ConfigSection(),videoBuf,videoCnt))
			{
			ERR_PRINTF1(_L("No Video Count"));
			User::Leave(KErrNotReady);
			}
			
		TInt enhancedVoiceMailCnt = -1;
		_LIT(KEnhancedVoiceMail,"EnhancedVoiceMail%d");
		TBuf<25>enhancedVoiceMail;
		enhancedVoiceMail.Format(KEnhancedVoiceMail,i);
		if ( !GetIntFromConfig(ConfigSection(),enhancedVoiceMail,enhancedVoiceMailCnt))
			{
			ERR_PRINTF1(_L("No enhancedVoiceMail Count"));
			User::Leave(KErrNotReady);
			}
		
		TInt faxCnt = -1;
		_LIT(KFax,"Fax%d");
		TBuf<10>fax;
		fax.Format(KFax,i);
		if ( !GetIntFromConfig(ConfigSection(),fax,faxCnt))
			{
			ERR_PRINTF1(_L("No Fax Count"));
			User::Leave(KErrNotReady);
			}
			
		TInt emailCnt = -1;
		_LIT(KEmail,"Email%d");
		TBuf<10>email;
		email.Format(KEmail,i);
		if ( !GetIntFromConfig(ConfigSection(),email,emailCnt))
			{
			ERR_PRINTF1(_L("No Email Count"));
			User::Leave(KErrNotReady);
			}
			
		TInt voiceMailCnt = -1;
		_LIT(KVoice,"Voice%d");
		TBuf<10>voice;
		voice.Format(KVoice,i);
		if ( !GetIntFromConfig(ConfigSection(),voice,voiceMailCnt))
			{
			ERR_PRINTF1(_L("No Voice Count"));
			User::Leave(KErrNotReady);
			}
			
		
		TInt otherCnt = -1;
		_LIT(KOther,"Other%d");
		TBuf<10>other;
		other.Format(KOther,i);
		if ( !GetIntFromConfig(ConfigSection(),other,otherCnt))
			{
			ERR_PRINTF1(_L("No Other Count"));
			User::Leave(KErrNotReady);
			}
		
		TInt replyCnt = -1;
		_LIT(KReply,"Reply%d");
		TBuf<10>reply;
		reply.Format(KReply,i);
		if ( !GetIntFromConfig(ConfigSection(),reply,replyCnt))
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


		// Restore the message
		CSmsHeader* smsHeader = CSmsHeader::NewL(CSmsPDU::ESmsDeliver, *bodyRichText);
		CleanupStack::PushL(smsHeader);
		entry->SetEntryL((*msvEntrySelection)[i]);

		CMsvStore* store = entry->ReadStoreL();
		CleanupStack::PushL(store);
		smsHeader->RestoreL(*store);

		TInt countIndications = videoCnt + enhancedVoiceMailCnt + faxCnt + emailCnt + voiceMailCnt + otherCnt;

		if((result)&&(videoCnt > 0)&&(enhancedVoiceMailCnt == 0)&&(faxCnt == 0)&&(emailCnt == 0)&&(voiceMailCnt == 0)&&(otherCnt == 0))
			{
			result = VerifyVideoIndication(*smsHeader, videoCnt);
			}
		if((result)&&(videoCnt == 0)&&(enhancedVoiceMailCnt > 0)&&(faxCnt == 0)&&(emailCnt == 0)&&(voiceMailCnt == 0)&&(otherCnt == 0))
			{
			result = VerifyEnhancedVoiceMailIndication(*smsHeader,enhancedVoiceMailCnt);
			}
		if((result)&&(videoCnt == 0)&&(enhancedVoiceMailCnt == 0)&&(faxCnt > 0)&&(emailCnt == 0)&&(voiceMailCnt == 0)&&(otherCnt == 0))
			{
			result = VerifyFaxIndication(*smsHeader, faxCnt);
			}
		if((result)&&(videoCnt == 0)&&(enhancedVoiceMailCnt == 0)&&(faxCnt == 0)&&(emailCnt > 0)&&(voiceMailCnt == 0)&&(otherCnt == 0))
			{
			result = VerifyEmailIndication(*smsHeader, emailCnt);
			}
		if((result)&&(videoCnt == 0)&&(enhancedVoiceMailCnt == 0)&&(faxCnt == 0)&&(emailCnt == 0)&&(voiceMailCnt > 0)&&(otherCnt == 0))
			{
			result = VerifyVoiceIndication(*smsHeader, voiceMailCnt);
			}
		if((result)&&(videoCnt > 0)&&(enhancedVoiceMailCnt > 0)&&(faxCnt > 0)&&(emailCnt > 0)&&(voiceMailCnt > 0)&&(otherCnt > 0))
			{
			result = VerifyOtherIndication(*smsHeader, countIndications);
			}

		//Reply Address Verification is not done in Above If/else because 
		//there is different checking to be done if multiple indications
		//(Multiple Indication doesnot consider ReplyAddress) are present
		if((result)&&(replyCnt > 0))
			{
			TPtrC replyAddress;
			_LIT(KReplyAddress,"ReplyAddress%d");
			TBuf<15> buf;
			buf.Format(KReplyAddress,i);
			if ( !GetStringFromConfig(ConfigSection(),buf,replyAddress))
				{
				ERR_PRINTF1(_L("No replyAddress "));
				User::Leave(KErrNotReady);
				}
			TRAPD(err,VerifyReplyToAddressL(*smsHeader, replyAddress));
			}

		if(result == EFalse)
				{
				SetTestStepResult(EFail);
				}
			else
				{
				SetTestStepResult(EPass);
				}
		CleanupStack::PopAndDestroy(5,paraFormatLayer); // paraformatlayer, charformatlayer, bodytext, smsheader,store
		}// For Loop
		CleanupStack::PopAndDestroy(msvEntrySelection);
		CleanupStack::PopAndDestroy(entry);
		
		delete session;
		delete sessionObserver1;
		
		CleanupStack::PopAndDestroy(scheduler);
	return TestStepResult();
	}

//Verifies Video Indication to the Message
TBool CVerifyIndicationTestStep ::VerifyVideoIndication(CSmsHeader& aHeader, TInt aCount)
	{
	INFO_PRINTF1(_L("Verifying Video Msg"));
	
	TBuf<KSmsDescriptionLength> desc;
	TSmsUtilities::GetDescription(aHeader.Message(), desc);
	
	TBuf<KSmsDescriptionLength> buf;
	if(aCount>1)
		{
		_LIT(KCompareVideoMsg1, "%d Video Messages Waiting");
		buf.Format(KCompareVideoMsg1,aCount);
		}
	else
		{
		_LIT(KCompareVideoMsg, "Video Message Waiting");
		buf.Copy(KCompareVideoMsg);
		}

	if(desc.CompareF(buf) != 0)
		{
		ERR_PRINTF1(_L("Verification for Video Indication failed"));
		return EFalse;
		}

	INFO_PRINTF1(_L("**Verified Video Msg "));
	return ETrue;
	}

//Verifies EnhancedVoiceMail Indication to the Message
TBool CVerifyIndicationTestStep ::VerifyEnhancedVoiceMailIndication(CSmsHeader& aHeader, TInt aCount)
	{
	INFO_PRINTF1(_L("Verifying Voice Mail Indication"));

	TBuf<KSmsDescriptionLength> desc;
	TSmsUtilities::GetDescription(aHeader.Message(), desc);

	TBuf<KSmsDescriptionLength> buf;
	if(aCount>1)
		{
		_LIT(KCompareEnhancedVoiceMailMsg1, "%d Enhanced Voicemails Waiting");
		buf.Format(KCompareEnhancedVoiceMailMsg1,aCount);
		}
	else
		{
		_LIT(KCompareEnhancedVoiceMailMsg, "Enhanced Voicemail Waiting");
		buf.Copy(KCompareEnhancedVoiceMailMsg);
		}

	if(desc.CompareF(buf) != 0)
		{
		ERR_PRINTF1(_L("Verification for Enhanced Voice Mail Indication failed"));
		return EFalse;
		}

	INFO_PRINTF1(_L("**Verified VoiceMail Indication "));
	return ETrue;
	}

// Verifys Fax Indication to the Message
TBool CVerifyIndicationTestStep ::VerifyFaxIndication(CSmsHeader& aHeader, TInt aCount)
	{
	INFO_PRINTF1(_L("Verifying Fax Indication"));
	
	TBuf<KSmsDescriptionLength> desc;
	TSmsUtilities::GetDescription(aHeader.Message(), desc);
	
	TBuf<KSmsDescriptionLength> buf;
	if(aCount>1)
		{
		_LIT(KCompareFaxMsg1, "%d Faxes Waiting");
		buf.Format(KCompareFaxMsg1,aCount);
		}
	else
		{
		_LIT(KCompareFaxMsg, "Fax Waiting");
		buf.Copy(KCompareFaxMsg);
		}		

	if(desc.CompareF(buf) != 0)
		{
		ERR_PRINTF1(_L("Verification for Fax Indication failed"));
		return EFalse;
		}

	INFO_PRINTF1(_L("**Verified fax Indication"));
	return ETrue;
	}
	
// Verifys Voice Indication to the Message
TBool CVerifyIndicationTestStep ::VerifyVoiceIndication(CSmsHeader& aHeader, TInt aCount)
	{
	INFO_PRINTF1(_L("Verifying Email Indication"));
	
	TBuf<KSmsDescriptionLength> desc;
	TSmsUtilities::GetDescription(aHeader.Message(), desc);

	TBuf<KSmsDescriptionLength> buf;
	if(aCount>1)
		{
		_LIT(KCompareVoiceMsg1, "%d Voicemails Waiting");
		buf.Format(KCompareVoiceMsg1,aCount);
		}
	else
		{
		_LIT(KCompareVoiceMsg, "Voicemail Waiting");
		buf.Copy(KCompareVoiceMsg);
		}		

	if(desc.CompareF(buf) != 0)
		{
		ERR_PRINTF1(_L("Verification for Voice Indication failed"));
		return EFalse;
		}

	INFO_PRINTF1(_L("**Verified Voice Indication"));
	return ETrue;
	}

// Verifys Email Indication to the Message
TBool CVerifyIndicationTestStep ::VerifyEmailIndication(CSmsHeader& aHeader, TInt aCount)
	{
	INFO_PRINTF1(_L("Verifying Email Indication"));
	
	TBuf<KSmsDescriptionLength> desc;
	TSmsUtilities::GetDescription(aHeader.Message(), desc);

	TBuf<KSmsDescriptionLength> buf;
	if(aCount>1)
		{
		_LIT(KCompareEmailMsg1, "%d E-Mails Waiting");
		buf.Format(KCompareEmailMsg1,aCount);
		}
	else
		{
		_LIT(KCompareEmailMsg, "E-Mail Waiting");
		buf.Copy(KCompareEmailMsg);
		}		

	if(desc.CompareF(buf) != 0)
		{
		ERR_PRINTF1(_L("Verification for Email Indication failed"));
		return EFalse;
		}

	INFO_PRINTF1(_L("**Verified Email Indication"));
	return ETrue;
	}

// Verifies Any Other/ Mixed Indication to the Message
TBool CVerifyIndicationTestStep ::VerifyOtherIndication(CSmsHeader& aHeader, TInt aCount)
	{
	INFO_PRINTF1(_L("Verifying Other Many Indication"));
	
	TBuf<KSmsDescriptionLength> desc;
	TSmsUtilities::GetDescription(aHeader.Message(), desc);
	
	TBuf<KSmsDescriptionLength> buf;
	if(aCount>1)
		{
		_LIT(KCompareOtherMsg1, "%d Messages Waiting");
		buf.Format(KCompareOtherMsg1,aCount);
		}
	else
		{
		_LIT(KCompareOtherMsg, "Message Waiting");
		buf.Copy(KCompareOtherMsg);
		}		

	if(desc.CompareF(buf) != 0)
		{
		ERR_PRINTF1(_L("Verification for Many Other Indications failed"));
		return EFalse;
		}

	INFO_PRINTF1(_L("**Verified Other Many Indication"));
	return ETrue;
	}

//Verifies ReplyTo Address to the Message
void CVerifyIndicationTestStep ::VerifyReplyToAddressL(CSmsHeader& aHeader,const TDesC& aReplyAddress)
	{
	INFO_PRINTF1(_L("Verifying ReplyTo Address"));
	TPtrC replyAdd = aHeader.ReplyAddressL();
	if(replyAdd.CompareF(aReplyAddress) != 0)
		{
		ERR_PRINTF1(_L("Reply Address Not same"));
		SetTestStepResult(EFail);
		}

	INFO_PRINTF1(_L("**Verified ReplyTo Address "));
	}

