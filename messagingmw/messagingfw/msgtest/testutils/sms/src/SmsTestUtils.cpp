// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "smstestutils.h"
#include <e32test.h>
#include <f32fsys.h>
#include <s32file.h>
#include <e32uid.h>
#include <txtrich.h>
#include <txtfmlyr.h>
#include <msvids.h>
#include <msvuids.h>
#include <e32std.h>
#include <smuthdr.h>
#include <msvsysagentaction.h>
#include <smscmds.h>
#include <smsclnt.h>
#include <smutset.h>
#include <msgtestscripts.h>
#include <biouids.h>
#include <msvschedulesettings.h>
#include <commsdattypesv1_1.h>
#include <commsdat.h>
#include <etelmm.h>
#include <logsmspdudata.h>

using namespace CommsDat;

#include "EmsMsgTestUtils.h"
#include "emstestutils.h"
#include <csmsaccount.h>
#include <msvoffpeaktime.h>

#include "CAsyncWaiter.h"

#include <mmsvattachmentmanager.h>
#include <cmsvattachment.h>

#include <e32property.h>

// Constants
//
const TInt KUidNetworkBarsCmtValue=0x100052DB;
const TInt KUidCmtPowerValue=0x100052D9;

/**
	@enum	TSANetworkBarsCmt
	Enum defining the system agent network commands
	@since	6.1
*/
enum TSANetworkBarsCmt
	{
	ENBars_0,		//<
	ENBars_1,		//<
	ENBars_2,		//<
	ENBars_3,		//<
	ENBars_4,		//<
	ECmtPowerOn,	//< Specifies that the power is on
	ECmtPowerOff	//< Specifies that the power is off
	};

/**
	@enum	TSACmtPower
	Enum defining the system agent commands for power
	@since	6.1
*/
enum TSACmtPower
	{	
	ECmtOn,		//< Specifies that the power is on
	ECmtOff		//< Specifies that the power is off
	};



EXPORT_C CSmsTestUtils::~CSmsTestUtils()
	{
	Reset();
	delete iServiceSettings;
	delete iRichText;
	delete iCharFormat;
	delete iParaFormat;

	iRTest.Printf(_L("Waiting 3 seconds for the msg server to close...\n"));
	User::After(3000000);
	}

EXPORT_C void CSmsTestUtils::AddSystemAgentConditionsL()
	{
	CMsvSysAgentActions* actions = new (ELeave) CMsvSysAgentActions();
	CleanupStack::PushL(actions);

	CMsvScheduleSettings* schSettings = CMsvScheduleSettings::NewL();
	CleanupStack::PushL(schSettings);

	CMsvOffPeakTimes* offPeakTimes = new (ELeave) CMsvOffPeakTimes();
	CleanupStack::PushL(offPeakTimes);

	CMsvSendErrorActions* errorActions = CMsvSendErrorActions::NewL();
	CleanupStack::PushL(errorActions);

	CSmsAccount* account = CSmsAccount::NewLC();
	account->LoadSettingsL(*schSettings, *offPeakTimes, *errorActions, *actions);

	actions->iDefault.iAction = ESendActionRetryImmediately;
	actions->iDefault.iRetries = ESendRetriesFixed;
	actions->iDefault.iRetrySpacing = ESendRetrySpacingStatic;
	actions->iDefault.SetMaxRetries(1);

	TMsvSysAgentConditionAction condAction;

	condAction.iUseDefaultSysAgentAction = ETrue;

	condAction.iCondition.iVariable = KMsvTestUidPhonePwr;
	condAction.iCondition.iState = EMsvTestPhoneOn;
	condAction.iCondition.iType = TMsvCondition::EMsvSchSendEquals;

	actions->AppendL(condAction);

	condAction.iCondition.iVariable = KMsvTestUidSIMStatus;
	condAction.iCondition.iState = EMsvTestSimOk;
	condAction.iCondition.iType = TMsvCondition::EMsvSchSendEquals;
	
	actions->AppendL(condAction);

	condAction.iCondition.iVariable = KMsvTestUidNetworkStatus;
	condAction.iCondition.iState = EMsvTestNetworkAvailable;
	condAction.iCondition.iType = TMsvCondition::EMsvSchSendEquals;
	
	actions->AppendL(condAction);

//	TMsvSendErrorAction action;
//	TBool condMet = actions->ConditionsMetL(action);

	account->SaveSettingsL(*schSettings, *offPeakTimes, *errorActions, *actions);
	CleanupStack::PopAndDestroy(4, account);    	
	}

/**
	@fn				NotifySaPhoneOnL()
	Intended Usage	:	To notify the system agent that the phone is on.
	@since			6.1
	@pre 			None
	@post			System agent has been notified.
 */
EXPORT_C void CSmsTestUtils::NotifySaPhoneOnL()
	{
	User::LeaveIfError(RProperty::Set(KUidSystemCategory, KMsvTestUidPhonePwrValue, EMsvTestPhoneOn));
	User::LeaveIfError(RProperty::Set(KUidSystemCategory, KUidCmtPowerValue, ECmtOn));
	User::LeaveIfError(RProperty::Set(KUidSystemCategory, KUidNetworkBarsCmtValue, ECmtPowerOn));
	User::LeaveIfError(RProperty::Set(KUidSystemCategory, 0x100052E7, 8));
	}

/**
	@fn				NotifySaPhoneOffL()
	Intended Usage	:	To notify the system agent that the phone is off.
	@since			6.2
	@pre 			None
	@post			System agent has been notified.
 */
EXPORT_C void CSmsTestUtils::NotifySaPhoneOffL()
	{
	User::LeaveIfError(RProperty::Set(KUidSystemCategory, KMsvTestUidPhonePwrValue, EMsvTestPhoneOff));
	User::LeaveIfError(RProperty::Set(KUidSystemCategory, KUidCmtPowerValue, ECmtOff));
	User::LeaveIfError(RProperty::Set(KUidSystemCategory, KUidNetworkBarsCmtValue, ECmtPowerOff));
	User::LeaveIfError(RProperty::Set(KUidSystemCategory, 0x100052E7, 8));
	}

EXPORT_C void CSmsTestUtils::WaitForInitializeL()
/**
	Initialize the phone for the tsy. This will prevent message sends from completing with KErrNotReady
*/
	{

	TName tsy;
	
	CMDBSession* dbSession = CMDBSession::NewL(CMDBSession::LatestVersion());
	CleanupStack::PushL(dbSession);
	
	CMDBRecordSet<CCDGlobalSettingsRecord> globalSettingsRecord(KCDTIdGlobalSettingsRecord);
	TRAPD(err, globalSettingsRecord.LoadL(*dbSession));
	if(err != KErrNone)
		{
		User::Leave(KErrNotFound);				
		}
		
	CCDModemBearerRecord *modemRecord = static_cast<CCDModemBearerRecord*>(CCDRecordBase::RecordFactoryL(KCDTIdModemBearerRecord));		
	CleanupStack::PushL(modemRecord);	
	modemRecord->SetRecordId(((CCDGlobalSettingsRecord*)globalSettingsRecord.iRecords[0])->iModemForPhoneServicesAndSMS); 
	modemRecord->LoadL(*dbSession);
	tsy = modemRecord->iTsyName;	
	CleanupStack::PopAndDestroy(2); //dbSession, modemRecord

	RTelServer server;
	User::LeaveIfError(server.Connect());
	CleanupClosePushL(server);
	User::LeaveIfError(server.LoadPhoneModule(tsy));

	// Find the phone corresponding to this TSY and open a number of handles on it
	TInt numPhones;
	User::LeaveIfError(server.EnumeratePhones(numPhones));
	RPhone phone;

	while (numPhones--)
		{
		TName phoneTsy;
		User::LeaveIfError(server.GetTsyName(numPhones,phoneTsy));
		if (phoneTsy.CompareF(tsy)==KErrNone)
			{
			RTelServer::TPhoneInfo info;
			User::LeaveIfError(server.GetPhoneInfo(numPhones,info));
			User::LeaveIfError(phone.Open(server,info.iName));
			CleanupClosePushL(phone);
			const TInt err = phone.Initialise();
			User::LeaveIfError(err);
			CleanupStack::PopAndDestroy(&phone);
			break;
			}
		}

	CleanupStack::PopAndDestroy(&server);
	}

EXPORT_C CSmsTestUtils* CSmsTestUtils::NewL(RTest& aRTest, TUint aCreationFlags)
	{
	CSmsTestUtils* self = NewLC(aRTest, aCreationFlags);
	CleanupStack::Pop();
	return self;
	}

EXPORT_C CSmsTestUtils* CSmsTestUtils::NewLC(RTest& aRTest, TUint aCreationFlags)
	{
	CSmsTestUtils* self = new(ELeave) CSmsTestUtils(aRTest);
	CleanupStack::PushL(self);
	self->ConstructL(aCreationFlags);
	return self;
	}

void CSmsTestUtils::Reset()
	{
	if (iSmsClientMtm)
		{
		delete iSmsClientMtm;
		iSmsClientMtm=NULL;
//		iMsvEntry=NULL;
		}

	CMsvTestUtils::Reset();
	}

//
// Register a Sms Mtm server DLL
//
EXPORT_C void CSmsTestUtils::CreateSmsServerMtmRegL()
	{
	CreateServerMtmRegL(KUidMsgTypeSMS, _L("SMS"), KSmsServerMtmFileName(), KSmsClientMtmFileName(), TMsvTestDllInfo(KSmsUiMtmFileFileName,2), TMsvTestDllInfo(KSmsUiMtmFileFileName,1), KUidMsgTypeSMS, KSmsDataComponentFileName);
	}

EXPORT_C TMsvId CSmsTestUtils::CreateSmsServiceL()
	{
	// create service in root
	TInt err = KErrNone;
	iSmsServiceId = 0;
	TRAP(err, ServiceIdL(KUidMsgTypeSMS, iSmsServiceId));

	if (err)
		{
		if (!iServiceSettings)
			{
			iServiceSettings = CSmsSettings::NewL();
			}
	
		CSmsAccount* account = CSmsAccount::NewLC();
		account->InitialiseDefaultSettingsL(*iServiceSettings);
		ServiceIdL(KUidMsgTypeSMS, iSmsServiceId);
			
		iServiceSettings->SetValidityPeriod(ESmsVPWeek);	// week
		iServiceSettings->SetValidityPeriodFormat(TSmsFirstOctet::ESmsVPFInteger); //relative
		iServiceSettings->AddServiceCenterL(_L("Vodafone"), _L("+4477850016005"));
		iServiceSettings->AddServiceCenterL(_L("Sonera"), _L("+358405202000"));
		iServiceSettings->SetDefaultServiceCenter(1);
		iServiceSettings->SetReplyQuoted(EFalse);
		iServiceSettings->SetRejectDuplicate(ETrue);
		iServiceSettings->SetDelivery(ESmsDeliveryImmediately);
		iServiceSettings->SetDeliveryReport(ETrue);
		iServiceSettings->SetReplyPath(EFalse);
		iServiceSettings->SetMessageConversion(ESmsConvPIDNone);
		iServiceSettings->SetCanConcatenate(ETrue);

		account->SaveSettingsL(*iServiceSettings);
		
		CMsvScheduleSettings* schSettings = CMsvScheduleSettings::NewL();
		CleanupStack::PushL(schSettings);

		CMsvOffPeakTimes* offPeakTimes = new (ELeave) CMsvOffPeakTimes();
		CleanupStack::PushL(offPeakTimes);

		CMsvSendErrorActions* errorActions = CMsvSendErrorActions::NewL();
		CleanupStack::PushL(errorActions);

		CMsvSysAgentActions* sysAgentActions = new (ELeave) CMsvSysAgentActions();
		CleanupStack::PushL(sysAgentActions);
		
		account->LoadSettingsL(*schSettings, *offPeakTimes, *errorActions, *sysAgentActions);

		schSettings->SetLongInterval(30);
		schSettings->SetShortInterval(10);

		account->SaveSettingsL(*schSettings, *offPeakTimes, *errorActions, *sysAgentActions);

		CleanupStack::PopAndDestroy(5, account);    	
		}
	else
		{
		User::Leave(KErrAlreadyExists);
		}

	return iSmsServiceId;
	}


EXPORT_C void CSmsTestUtils::InstallSmsMtmGroupL()
	{
	InstallMtmGroupL(KSmsDataComponentFileName);
	}

EXPORT_C void CSmsTestUtils::InstantiateSmsClientMtmL()
	{
	__ASSERT_ALWAYS(iSmsServiceId, Panic(KSmsTestUtilsPanicSmsServiceIdNotSet));
	delete iSmsClientMtm;
	iSmsClientMtm = NULL;
	iSmsClientMtm = (CSmsClientMtm*) InstantiateClientMtmL(KUidMsgTypeSMS, iSmsServiceId);
	}

EXPORT_C CSmsTestUtils::CSmsTestUtils(RTest& aRTest)
: CMsvTestUtils(aRTest), iSmsServiceId(KMsvNullIndexEntryId), iSmokeTest(EFalse)
	{
	}

EXPORT_C void CSmsTestUtils::ConstructL(TUint aFlags)
	{
	CMsvTestUtils::ConstructL(aFlags);


	iParaFormat = CParaFormatLayer::NewL();
	iCharFormat = CCharFormatLayer::NewL();
	iRichText = CRichText::NewL(iParaFormat, iCharFormat);
	}

EXPORT_C void CSmsTestUtils::SaveBodyToFileL(TMsvId aId)
	{
	SetEntryL(aId);

	CMsvStore* store = ReadStoreL();
	CleanupStack::PushL(store);
		
	iRichText->Reset();
	store->RestoreBodyTextL(*iRichText);

	CleanupStack::PopAndDestroy(store);

	const TInt length = iRichText->DocumentLength();
		
	HBufC* buf = HBufC::NewLC(length);
	HBufC8* buf8 = HBufC8::NewLC(length);

	TPtr bufPtr(buf->Des());

	iRichText->Extract(bufPtr);
	buf8->Des().Copy(*buf);
	TFileName path;


	TInt bioType = Entry().iBioType;
	path.AppendNum((TUint)Entry().Id());
	path.Append(_L("-"));
	path.AppendNum(bioType);
	path.Append(_L(".body"));
	
	// Remove any attachments with the same name
	if (iClientServer == EClientSide)
		{
		store = iMsvEntry->EditStoreL();
		}
	else if (iClientServer == EServerSide)
		{
		store = iServerEntry->EditStoreL();
		}
	else
		{
		User::Leave(KErrNotSupported);
		}
	CleanupStack::PushL(store);
	
	CAsyncWaiter* waiter = CAsyncWaiter::NewL();
	CleanupStack::PushL(waiter);
	MMsvAttachmentManager& manager = store->AttachmentManagerL();
	for(TInt ii=0; ii<manager.AttachmentCount(); ++ii )
		{
		

		CMsvAttachment* attachment = manager.GetAttachmentInfoL(ii);
		CleanupStack::PushL(attachment);
		if( attachment->AttachmentName().CompareF(path) == 0 )
			{
			CTestActive* testActive = new (ELeave) CTestActive();
			CleanupStack::PushL(testActive);
			
			// found a matching attachment, remove it
			manager.RemoveAttachmentL(ii, waiter->iStatus);
			waiter->StartAndWait();
			User::LeaveIfError(waiter->Result());			
			}
		CleanupStack::PopAndDestroy(attachment);
		
		}
	
	// create a new attachment file
	CMsvAttachment* attachment = CMsvAttachment::NewL(CMsvAttachment::EMsvFile);
	CleanupStack::PushL(attachment);
	attachment->SetSize(buf->Length());
	attachment->SetAttachmentNameL(path);
	RFile file;
	manager.CreateAttachmentL(path, file, attachment, waiter->iStatus);
	CleanupStack::Pop(attachment); // ownership passed
	waiter->StartAndWait();
	User::LeaveIfError(waiter->Result());		

	CleanupClosePushL(file);

	file.Write(*buf8);
	file.Flush();

	CleanupStack::PopAndDestroy(5, buf); // file, waiter, store, buf8, buf
	}



void CSmsTestUtils::DisplayChildDetailsL(const TMsvEntry& aEntry)
	{
	iRTest.Printf(KMsvTestUtilsNewLine);
	DisplayChildL(aEntry);
	iRTest.Printf(KMsvTestUtilsNewLine);

	if ((aEntry.iMtm != KUidMsgTypeSMS && aEntry.iMtm != KUidBIOMessageTypeMtm) || aEntry.iType != KUidMsvMessageEntry)
		return;

	CMsvEntry* cEntry = iMsvSession->GetEntryL(aEntry.Id());
	CleanupStack::PushL(cEntry);

	CSmsHeader* header = DoGetHeaderLC(*cEntry);

	TBuf<256> temp;
	temp.Append(_L("\tType: SMS "));

	switch (header->Type())
		{
		case CSmsPDU::ESmsDeliver:
			temp.Append(_L("Deliver\n"));
			break;
		case CSmsPDU::ESmsSubmit:
			temp.Append(_L("Submit\n"));
			break;
		case CSmsPDU::ESmsDeliverReport:
			temp.Append(_L("Deliver Report\n"));
			break;
		case CSmsPDU::ESmsSubmitReport:
			temp.Append(_L("Submit Report\n"));
			break;
		default:
			temp.Append(_L("Other\n"));
			break;
		}

	iRTest.Printf(temp);
	temp.Zero();

	iRTest.Printf(_L("\tBio Type UID: %d\n"), aEntry.iBioType);
	iRTest.Printf(_L("\tSending State: %d\n"), aEntry.SendingState());

	if (header->Type() == CSmsPDU::ESmsDeliver)
		{
 		CSmsDeliver& pdu = header->Deliver();

		TBool found = EFalse;
		TInt destination, originator, is16Bit;
		destination = originator = is16Bit = 0;
		if (pdu.UserDataPresent())
			{
			const CSmsUserData& userData = pdu.UserData();
			TInt count = userData.NumInformationElements();
			iRTest.Printf(_L("\tPdu contains %d information elements\n"), count);
			while (!found && count--)
				{
				CSmsInformationElement& nextIE=userData.InformationElement(count);
				switch(nextIE.Identifier())
					{
					case CSmsInformationElement::ESmsIEIApplicationPortAddressing8Bit:
					case CSmsInformationElement::ESmsIEIApplicationPortAddressing16Bit:
						{
						pdu.ApplicationPortAddressing(destination, originator, &is16Bit);
						found = ETrue;
						break;
						}
					default:
						break;
					}
				}
			}
		if (found)
			iRTest.Printf(_L("\tApplication Port Addressing: Dest %d, Origin %d, is16Bit %d\n"), destination, originator, is16Bit);
		else
			iRTest.Printf(_L("\tApplication Port Addressing: Not Found\n"));
		}

	CleanupStack::PopAndDestroy(header);
	CleanupStack::PopAndDestroy(cEntry);

	iRTest.Printf(_L("\n'S'ave or any other key...\n"));
	TChar key = iRTest.Getch();

	if (key == 's' || key == 'S')
		{
		TRAPD(err, SaveBodyToFileL(aEntry.Id()));

		if (!err)
			iRTest.Printf(_L("\nMessage body saved OK\n"));
		else
			iRTest.Printf(_L("\nSave ERROR %d\n"), err);

		iRTest.Getch();
		}
	}

EXPORT_C void CSmsTestUtils::DisplayMessagesL(const CMsvEntrySelection& aSelection)
	{
	TInt count = aSelection.Count();
	
	for (TInt i = 0; i < count; i++)
		{
		DisplayMessageL(aSelection[i]);
		}
	}

EXPORT_C void CSmsTestUtils::DisplayMessageL(TMsvId aId)
	{
	SetEntryL(aId);
	CSmsHeader* header = GetHeaderLC(aId);

	HBufC* buf = HBufC::NewLC(100 + Entry().iDetails.Length() + Entry().iDescription.Length());
	TPtr temp(buf->Des());

	temp.AppendNum((TInt) aId);
	temp.Append(_L(" Type: "));

	switch (header->Type())
		{
		case CSmsPDU::ESmsDeliver:
			temp.Append(_L("Deliver"));
			break;
		case CSmsPDU::ESmsSubmit:
			temp.Append(_L("Submit "));
			break;
		case CSmsPDU::ESmsSubmitReport:
			temp.Append(_L("SubRprt"));
			break;
		case CSmsPDU::ESmsDeliverReport:
			temp.Append(_L("DelRprt"));
			break;
		default:
			temp.Append(_L("Other  "));
			break;
		}

	temp.Append(_L(" Rcpts: "));
	temp.AppendNum(header->Recipients().Count());
	temp.Append(_L(" Det: \""));
	temp.Append(Entry().iDetails);
	temp.Append(_L("\" Desc: \""));
	temp.Append(Entry().iDescription);
	temp.Append(_L("\"\n"));
	Printf(*buf);

	CleanupStack::PopAndDestroy(buf);
	CleanupStack::PopAndDestroy(header);	
	}

EXPORT_C TBool CSmsTestUtils::ReadScriptL(const TDesC& aFile, TMsvId aDestFolder, CMsvEntrySelection& aSelection, const TTime& aDate)
	{
	TParse fileOut;

	User::LeaveIfError(ResolveFile(KSmsComponent, aFile, fileOut));

	return DoReadScriptL(fileOut.FullName(), aDestFolder, aSelection, aDate);
	}

TBool CSmsTestUtils::DoReadScriptL(const TDesC& aFile, TMsvId aDestFolder, CMsvEntrySelection& aSelection, const TTime& aDate)
	{
	CScriptFile* scriptFile = CScriptFile::NewLC(*this, KSmsComponent, aFile);
	TBool retVal = CreateMessagesL(aDestFolder, *scriptFile, aSelection, aDate);
	CleanupStack::PopAndDestroy(scriptFile);
	return retVal;
	}

TBool CSmsTestUtils::CreateMessagesL(TMsvId aDestFolder, CScriptFile& rFile, CMsvEntrySelection& rSelection, const TTime& aDate)
	{
	CArrayPtrFlat<CScriptSection>& sections = rFile.Sections();

	TBool retVal = EFalse;

	if (!sections.Count())
		return EFalse;

	const TInt secCount = sections.Count();
	TInt count = secCount;
	TInt all = KErrNotFound;
	CScriptSection* section = NULL;

	while (count--)
		{
		section = sections[count];
		if (section->SectionName().CompareF(_L("All")) == 0)
			{
			all = count;
			break;
			}
		}

	count = secCount;

	for(TInt item = 0; item < count; item++)
		{
		section = sections[item];

		if (item == all)
			{
			continue;
			}

		TPtrC typePtr(section->ItemValue(KScriptItemMessageType, KScriptItemSubmit));  //  CJCW
		TPtrC classPtr(section->ItemValue(KScriptItemMessageClass, KScriptItemClassDefault));  //  CJCW
		TPtrC scPtr(section->ItemValue(KScriptItemSC, KScriptItemSCDef));
		TPtrC rcptsPtr(section->ItemValue(KScriptItemRecipients, KScriptItemRecipientsDef));
		TInt bioId(section->ItemValue(KScriptItemBioUid, KScriptItemBioUidDef));
		TInt encoding(section->ItemValue(KScriptItemEncoding, KScriptItemEncodingDef));
		TPtrC drPtr(section->ItemValue(KScriptItemDeliveryReport, KScriptItemDeliveryReportDef));
		TPtrC bearerPtr(section->ItemValue(KScriptItemBearer, KScriptItemBearerDef));
		TPtrC msgPtr(section->ItemValue(KScriptItemMessage, KNullDesC));
		TPtrC filePtr(section->ItemValue(KScriptItemFile, KNullDesC));

		TBioMsgIdType bearer = EBioMsgIdNbs;

		if (bearerPtr.CompareF(_L("wap")) == 0)
			{
			bearer = EBioMsgIdWap;
			}
		else if (bearerPtr.CompareF(_L("wapsecure")) == 0)
			{
			bearer = EBioMsgIdWapSecure;
			}

		TSmsDataCodingScheme::TSmsAlphabet alpha = TSmsDataCodingScheme::ESmsAlphabet7Bit;

		switch (encoding)
			{
			case 7:
				alpha = TSmsDataCodingScheme::ESmsAlphabet7Bit;
				break;
			case 8:
				alpha = TSmsDataCodingScheme::ESmsAlphabet8Bit;
				break;
			case 16:
				alpha = TSmsDataCodingScheme::ESmsAlphabetUCS2;
				break;
			default:
				alpha = TSmsDataCodingScheme::ESmsAlphabet7Bit;
				break;
			}

		TBool changeDR = ETrue;
		TBool deliveryReport = EFalse;

		if (drPtr.CompareF(_L("NO")) == 0)
			{
			deliveryReport = EFalse;
			}
		else if (drPtr.CompareF(_L("YES")) == 0)
			{
			deliveryReport = ETrue;
			}
		else
			{
			changeDR = EFalse;
			}

		if (filePtr.Length())
			{
			HBufC8* tempMsg = NULL;
			TRAPD(resErr, tempMsg = ReadFileLC(filePtr));

			if (resErr)
				{
				TParse fileOut;
				resErr = ResolveFile(KSmsComponent, filePtr, fileOut);

				if (resErr)
					{
					Printf(_L("Cannot resolve file %S\n"), &filePtr);
					User::Leave(resErr);
					}

				tempMsg = ReadFileLC(fileOut.FullName());
				}

			HBufC* msgVal = HBufC::NewLC(tempMsg->Length());
			msgVal->Des().Copy(*tempMsg);
			section->ReplaceItemL(KScriptItemMessage, *msgVal);
			CleanupStack::PopAndDestroy(2); //tempMsg, msgVal
			msgPtr.Set(section->ItemValue(KScriptItemMessage, KNullDesC));
			}

		if (msgPtr.Length())
			{
			//Replace LF with CRLF for vCards and vCals only
			if (bioId == KVCardBioUid.iUid || bioId == KVCalBioUid.iUid || msgPtr.Left(5) == _L("//SCK"))
				{
				CScriptSectionItem* sectionItem = section->Item(KScriptItemMessage);

				__ASSERT_ALWAYS(sectionItem, Panic(KErrNotFound));

				CScriptFile::ReplaceL(KScriptCRLF, KScriptLF, sectionItem->iValue);
				CScriptFile::ReplaceL(KScriptLF, KScriptCRLF, sectionItem->iValue);
				TInt valLen = sectionItem->Value().Length();
				sectionItem->iValue = sectionItem->iValue->ReAllocL(valLen + 2);
				sectionItem->iValue->Des().Append(KScriptCRLF);
				}

			TLex input(rcptsPtr);
			TPtrC rcpt;
			TInt curRcpt = 0;
			TMsvEntry entry;
			
			while (CScriptFile::GetNextWord(input, '|', rcpt) == KErrNone)
				{
				if (rcpt.CompareF(KDefault1) == 0 || rcpt.CompareF(KDefault2) == 0)
					{
					CScriptSection* defaults = section->Defaults();
					if (defaults != NULL)
						{
						rcpt.Set(defaults->ItemValue(KScriptItemRecipients, KScriptItemRecipientsDef));
						}
					}

				retVal = ETrue;

				if (!curRcpt)
					{
					entry = TMsvEntry();
					entry.iBioType = bioId;
					entry.SetSendingState(KMsvSendStateUponRequest);

					CSmsPDU::TSmsPDUType type=CSmsPDU::ESmsSubmit;
					if (typePtr.CompareF(KScriptItemSubmit) == 0)  //  CJCW
						type=CSmsPDU::ESmsSubmit;
					else if (typePtr.CompareF(KScriptItemDeliver) == 0)
						type=CSmsPDU::ESmsDeliver;
					else if (typePtr.CompareF(KScriptItemStatusReport) == 0)
						type=CSmsPDU::ESmsStatusReport;
					else if (typePtr.CompareF(KScriptItemSubmit) == 0)
						type=CSmsPDU::ESmsCommand;
					else
						User::Leave(KErrNotSupported);

					CSmsHeader* header = CSmsHeader::NewL(type, *iRichText);  //  CJCW
					CleanupStack::PushL(header);

					iRichText->Reset();
					if (msgPtr.Length())
						{
						if (type==CSmsPDU::ESmsStatusReport)
							{
							CSmsStatusReport& statusreport=header->StatusReport();
							statusreport.SetParameterIndicatorPresent(ETrue);
							statusreport.SetDataCodingSchemePresent(ETrue);
							statusreport.SetUserDataPresent(ETrue);
							}
						iRichText->InsertL(0, msgPtr);
						}

					__ASSERT_ALWAYS(iServiceSettings, Panic(KSmsTestUtilsPanicSettingsNotConstructed));

					header->SetSmsSettingsL(*iServiceSettings);

					if (type==CSmsPDU::ESmsSubmit || type==CSmsPDU::ESmsDeliver)
						{
						CSmsPDU& pdu=header->Message().SmsPDU();
						if (classPtr.CompareF(KScriptItemClassDefault)==0)
							pdu.SetClass(EFalse,TSmsDataCodingScheme::ESmsClass0);
						else if (classPtr.CompareF(KScriptItemClass0)==0)
							pdu.SetClass(ETrue,TSmsDataCodingScheme::ESmsClass0);
						else if (classPtr.CompareF(KScriptItemClass1)==0)
							pdu.SetClass(ETrue,TSmsDataCodingScheme::ESmsClass1);
						else if (classPtr.CompareF(KScriptItemClass2)==0)
							pdu.SetClass(ETrue,TSmsDataCodingScheme::ESmsClass2);
						else if (classPtr.CompareF(KScriptItemClass3)==0)
							pdu.SetClass(ETrue,TSmsDataCodingScheme::ESmsClass3);
						}

					if (changeDR)
						{
						switch (header->Type())
							{
							case CSmsPDU::ESmsSubmit:
								{
								header->Submit().SetStatusReportRequest(deliveryReport);
								break;
								}
							case CSmsPDU::ESmsCommand:
								{
								header->Command().SetStatusReportRequest(deliveryReport);
								break;
								}
							default:
								User::Leave(KErrNotSupported);
							};
						}

					header->SetBioMsgIdType(bearer);

					if (scPtr.CompareF(KScriptItemSCUseDefault) == 0)
						{
						header->Message().SetServiceCenterAddressL(TPtrC());
						}
					else
						{
						header->Message().SetServiceCenterAddressL(scPtr);
						}

					// Extract the EMS objects
					CEmsExtractor* ems = CEmsExtractor::NewLC(*section);
					ems->AddToMessageL(header->Message());
					CleanupStack::PopAndDestroy(ems);

					CSmsPDU& pdu=header->Message().SmsPDU();
					if (pdu.DataCodingSchemePresent())
						pdu.SetAlphabet(alpha);
					
					TMsvId destfolder=aDestFolder;
					if (destfolder==KMsvNullIndexEntryId)
						destfolder=(type==CSmsPDU::ESmsSubmit) || (type==CSmsPDU::ESmsCommand)? KMsvGlobalOutBoxIndexEntryId: KMsvGlobalInBoxIndexEntryId;

					CreateMessageL(destfolder, entry, *header, *iRichText, aDate);
					CleanupStack::PopAndDestroy(); //header

					AddRecipientL(entry.Id(), rcpt);

					rSelection.AppendL(entry.Id());
					}
				else
					{
					AddRecipientL(entry.Id(), rcpt);
					}

				curRcpt++;
				}
			}
		}

	return retVal;
	}

EXPORT_C TMsvId CSmsTestUtils::CreateDummyMessageToSendL()
	{
	TMsvEntry entry;
	entry.SetSendingState(KMsvSendStateUponRequest);
	entry.iDate.HomeTime();

	iRichText->Reset();
	iRichText->InsertL(0,_L("This is a send test.\nдце\n Cheers, Mico\n"));

	CSmsHeader* header = CSmsHeader::NewL(CSmsPDU::ESmsSubmit,*iRichText);
	CleanupStack::PushL(header);

	header->SetSmsSettingsL(*iServiceSettings);
	
	CSmsNumber* recipient=CSmsNumber::NewL();
	CleanupStack::PushL(recipient);
	recipient->SetAddressL(KScriptItemRecipientsDef);
	recipient->SetNameL(_L("Anthony"));
	header->Recipients().AppendL(recipient);
	CleanupStack::Pop(); //recipients

	CSmsNumber* recipient2=CSmsNumber::NewL();
	CleanupStack::PushL(recipient2);
	recipient2->SetAddressL(KScriptItemRecipients2Def);
	recipient2->SetNameL(_L("Julia"));
	header->Recipients().AppendL(recipient2);
	CleanupStack::Pop(); //recipients

	TTime now;
	now.HomeTime();

	CreateMessageL(KMsvGlobalOutBoxIndexEntryId, entry, *header, *iRichText, now);
//	Printf(_L("Test Create Message (%d) To Send\n"), entry.Id());

	CleanupStack::PopAndDestroy(1); //header

	return entry.Id();
	}

EXPORT_C TMsvId CSmsTestUtils::CreateDummyMessageToReceiveL()
	{
	TMsvEntry entry;
	entry.iType=KUidMsvMessageEntry;
	entry.SetSendingState(KMsvSendStateNotApplicable);

	iRichText->Reset();
	iRichText->InsertL(0,_L("Hiphei"));

	CSmsHeader* header = CSmsHeader::NewL(CSmsPDU::ESmsDeliver, *iRichText);
	CleanupStack::PushL(header);

	header->SetFromAddressL(KScriptItemRecipientsDef);
	header->Message().SetServiceCenterAddressL(KScriptItemSCDef);

	TTime now;
	now.HomeTime();

	CreateMessageL(KMsvGlobalInBoxIndexEntryId, entry, *header, *iRichText, now);
	entry.SetReadOnly(ETrue);
	ChangeEntryL(entry);

//	Printf(_L("Create Message (%d) To Receive\n"), entry.Id());

	CleanupStack::PopAndDestroy(1); //header
	return entry.Id();
	}

EXPORT_C void CSmsTestUtils::CreateMessageL(TMsvId aDestFolder, TMsvEntry& aEntry, CSmsHeader& aHeader, CRichText& aBody, const TTime& aDate)
	{
	TSmsUtilities::PopulateMsgEntry(aEntry, aHeader.Message(), iSmsServiceId);
	TBuf<KSmsDescriptionLength> desc;

	TSmsUtilities::GetDescription(aHeader.Message(), desc);
	aEntry.iDescription.Set(desc);

	aEntry.iDate = aDate;

	SetEntryL(aDestFolder);

	CreateEntryL(aEntry);

	if (iClientServer == EClientSide)
		iMsvSession->CleanupEntryPushL(aEntry.Id());

	SetEntryL(aEntry.Id());

	CMsvStore* store = EditStoreL();
	CleanupStack::PushL(store);

	aHeader.StoreL(*store);

	store->StoreBodyTextL(aBody);

	store->CommitL();

	aEntry.iSize = store->SizeL();
	ChangeEntryL(aEntry);

	CleanupStack::PopAndDestroy(store);

	if (iClientServer == EClientSide)
		iMsvSession->CleanupEntryPop();
	}

EXPORT_C void CSmsTestUtils::AddRecipientL(TMsvId aId, const TDesC& aNumber)
	{
	CSmsHeader* header = GetHeaderLC(aId);

	SetEntryL(aId);
	CMsvStore* store = EditStoreL();
	CleanupStack::PushL(store);

	CSmsNumber* rcpt = CSmsNumber::NewL();
	CleanupStack::PushL(rcpt);
	rcpt->SetAddressL(aNumber);

	TBuf<KSmsDetailsLength> details;

	if (TSmsUtilities::GetDetails(iFs, aNumber, details) == KErrNone)
		{
		rcpt->SetNameL(details);
		TMsvEntry entry(Entry());
		entry.iDetails.Set(details);
		ChangeEntryL(entry);
		}

	header->Recipients().AppendL(rcpt);
	CleanupStack::Pop(); //rcpt;

	header->StoreL(*store);

	store->CommitL();

	CleanupStack::PopAndDestroy(store);
	CleanupStack::PopAndDestroy(header);
	}

EXPORT_C void CSmsTestUtils::Panic(TSmsTestUtilsPanic aPanic)
	{
	Panic((TInt) aPanic);
	}

EXPORT_C void CSmsTestUtils::Panic(TInt aPanic)
	{
	User::Panic(SMSTEST_PANIC, aPanic);
	}

EXPORT_C void CSmsTestUtils::InstantiateSmsServerMtmL()
	{
	InstantiateServerMtmL(KUidMsgTypeSMS, iSmsServiceId);
	}

EXPORT_C void CSmsTestUtils::DeleteSmsServiceL()
	{
	DeleteServiceL(KUidMsgTypeSMS);
	}

EXPORT_C TMsvId CSmsTestUtils::FindExistingSmsServiceL()
	{
	TMsvId firstId = 0;
	ServiceIdL(KUidMsgTypeSMS, firstId);
	return firstId;
	}

EXPORT_C void CSmsTestUtils::FindExistingServicesL()
	{
	iSmsServiceId = 0;
	TRAPD(err, iSmsServiceId = FindExistingSmsServiceL());

	if (err)
		Printf(_L("No existing SMS services found!\n"));
	}

EXPORT_C void CSmsTestUtils::DeleteSmsMessagesL(TMsvId aFolder)
	{
	SetEntryL(aFolder);
	CMsvEntrySelection* sel = ChildrenWithMtmLC(KUidMsgTypeSMS);

	TInt count = sel->Count();

	while (count--)
		{
		DeleteEntryL(sel->At(count));
		}

	CleanupStack::PopAndDestroy(); //sel
	}

EXPORT_C TBool CSmsTestUtils::AreMessagesSameTextL(TMsvId aSentId, TMsvId aRecvId, TBool aPrintIfMismatch)
	{
	__ASSERT_ALWAYS(aSentId != aRecvId, Panic(KSmsTestUtilsPanicCannotCompareSameIds));

	CSmsHeader* sentHeader = CSmsHeader::NewL(CSmsPDU::ESmsDeliver, *iRichText);
	CleanupStack::PushL(sentHeader);
	CSmsHeader* rcvdHeader = CSmsHeader::NewL(CSmsPDU::ESmsDeliver, *iRichText);
	CleanupStack::PushL(rcvdHeader);
	
	// extract the sent message
	SetEntryL(aSentId);
	CMsvStore* store = ReadStoreL();
	CleanupStack::PushL(store);
	sentHeader->RestoreL(*store);
	CleanupStack::PopAndDestroy(); //store

	// do the same for the rx message
	SetEntryL(aRecvId);
	store = ReadStoreL();
	CleanupStack::PushL(store);
	rcvdHeader->RestoreL(*store);
	CleanupStack::PopAndDestroy(); //store

	// compare the text and the EMS objects
	TBool retVal = EmsTestUtils::CompareEmsMsgL(sentHeader->Message(), rcvdHeader->Message());
	if (!retVal && aPrintIfMismatch)
		{
		Printf(_L("Message mismatch...\n"));
		}

	CleanupStack::PopAndDestroy(rcvdHeader);
	CleanupStack::PopAndDestroy(sentHeader);

	return retVal;
	}

void CSmsTestUtils::DisplayLogEventData(const CLogEvent& aEvent)
	{
	if (aEvent.EventType() == KLogShortMessageEventTypeUid)
		{
		TLogSmsPduData data;
		TPckg<TLogSmsPduData> packeddata(data);
		packeddata.Copy(aEvent.Data());

		TBuf<256> output;
		output.Append(_L("\t"));

		switch (data.iType)
			{
			case CSmsPDU::ESmsSubmit:
				output.Append(_L("Submit"));
				break;
			case CSmsPDU::ESmsDeliver:
				output.Append(_L("Deliver"));
				break;
			case CSmsPDU::ESmsStatusReport:
				output.Append(_L("Status Report"));
				break;
			case CSmsPDU::ESmsSubmitReport:
				output.Append(_L("Submit Report"));
				break;
			case CSmsPDU::ESmsDeliverReport:
				output.Append(_L("Deliver Report"));
				break;
			default:
				output.Append(_L("Command"));
				break;
			}

		output.Append(_L(" Tot %d Sent %d Deliver %d Fail %d Recvd %d\n"));
		Printf(output, data.iTotal, data.iSent, data.iDelivered, data.iFailed, data.iReceived);
		}
	}

EXPORT_C void CSmsTestUtils::SetCommDbServiceCenterFromScriptL(const TDesC& aScriptFile)
	{
	//Update the service settings with the Service Centre address read from the script
	Printf(_L("Setting Comm DB Service Center from Script\n"));
	
	TTime now;
	now.HomeTime();

	CMsvEntrySelection* sel = new (ELeave) CMsvEntrySelection();
	CleanupStack::PushL(sel);

	ReadScriptL(aScriptFile, KMsvGlobalOutBoxIndexEntryId, *sel, now);

	Printf(_L("\t%d msgs read from %S\n"), sel->Count(), &aScriptFile);

	if (!sel->Count())
		User::Leave(KErrNotFound);

	CSmsHeader* header = CSmsHeader::NewL(CSmsPDU::ESmsSubmit, *iRichText);
	CleanupStack::PushL(header);

	SetEntryL(sel->At(0));
	CMsvStore* store = ReadStoreL();
	CleanupStack::PushL(store);
	header->RestoreL(*store);
	CleanupStack::PopAndDestroy(store);

	CSmsAccount* account = CSmsAccount::NewLC();
	account->LoadSettingsL(*iServiceSettings);
	
	iServiceSettings->SetCommDbAction(CSmsSettings::EStoreToCommDb);

	TInt count = iServiceSettings->ServiceCenterCount();
	while (count--)
		{
		iServiceSettings->RemoveServiceCenter(count);
		}

	TPtrC sc(header->Message().ServiceCenterAddress());
	Printf(_L("\tUsing service center %S\n"), &sc);
	iServiceSettings->AddServiceCenterL(_L("Unknown"), sc);

	account->SaveSettingsL(*iServiceSettings);
	CleanupStack::PopAndDestroy(account);    		

	count = sel->Count();
	while (count--)
		{
		TMsvId id = sel->At(count);
		SetEntryL(id);
		SetEntryL(Entry().Parent());
		DeleteEntryL(id);
		}

	CleanupStack::PopAndDestroy(header);
	CleanupStack::PopAndDestroy(sel);
	Printf(_L("\tComplete: Setting Comm DB Service Center from Script\n"));
	}

EXPORT_C TBool CSmsTestUtils::SendingCompleteL(const CMsvEntrySelection& aSelection, TInt& rError)
	{
	TBool sent = ETrue;
	const TInt msgCount = aSelection.Count();
	TInt err = KErrNone;

	for (TInt msg = 0; msg < msgCount; msg++)
		{
		TRAP(err, SetEntryL(aSelection[msg]));

		if (err == KErrNone)
			{
			TMsvEntry entry = Entry();
			
			TRAP(err, SetEntryL(KMsvGlobalOutBoxIndexEntryId)); //ignore error

			switch (entry.SendingState())
				{
				case KMsvSendStateSending:
				case KMsvSendStateResend:
				case KMsvSendStateScheduled:
				case KMsvSendStateWaiting:
					sent = EFalse;
					break;
				case KMsvSendStateFailed:
					if (entry.iError)
						rError = entry.iError;
					else
						rError = KErrGeneral;
					break;
				default:
					break;
					//nothing
				}
			}
		}

	return sent;
	}

EXPORT_C void CSmsTestUtils::DisplaySendingStatesL(const CMsvEntrySelection& aSelection)
	{
	TInt err = KErrNone;
	TInt msgCount = aSelection.Count();

	for (TInt msg = 0; msg < msgCount; msg++)
		{
		TRAP(err, SetEntryL(aSelection[msg]));

		if (!err)
			{
			TMsvEntry entry = Entry();
			TRAP(err, SetEntryL(KMsvGlobalOutBoxIndexEntryId)); //ignore error

				TBuf<0x100> output;
				TBuf<0x50>  nowBuf;

				switch (entry.SendingState())
					{
					case KMsvSendStateSending:
						output.AppendFormat(_L("Message %d Sending with error %d\n"), entry.Id(), entry.iError);
						break;
					case KMsvSendStateResend:
						output.AppendFormat(_L("Message %d Resend with error %d at:\n"), entry.Id(), entry.iError);
						err = AppendScheduleTimeL(entry.Id(), output);
						if (err)
							{
							entry.iDate.FormatL(nowBuf, _L("%-B%:0%J%:1%T%:2%S%:3%+B"));
							output.Append(nowBuf);
							output.Append(_L("\n"));
							}
						break;
					case KMsvSendStateScheduled:
						output.AppendFormat(_L("Message %d Scheduled with error %d at:\n"), entry.Id(), entry.iError);
						err = AppendScheduleTimeL(entry.Id(), output);
						break;
					case KMsvSendStateFailed:
						output.AppendFormat(_L("Message %d Failed with error %d\n"), entry.Id(), entry.iError);
						break;
					case KMsvSendStateWaiting:
						output.AppendFormat(_L("Message %d Waiting with error %d\n"), entry.Id(), entry.iError);
						break;
					case KMsvSendStateSent:
						output.AppendFormat(_L("Message %d Sent\n"), entry.Id());
						break;
					case KMsvSendStateSuspended:
						output.AppendFormat(_L("Message %d Suspended\n"), entry.Id());
						break;
					default:
						output.AppendFormat(_L("Message %d Other Sending State with error %d and failed %d\n"), entry.Id(), entry.iError, entry.Failed());
						break;
						//nothing
					} //end switch
				Printf(output);
			} //end if
		} //end for
	}

CSmsHeader* CSmsTestUtils::DoGetHeaderLC(CMsvEntry& aEntry)
	{
	CSmsHeader* header = CSmsHeader::NewL(CSmsPDU::ESmsSubmit, *iRichText);
	CleanupStack::PushL(header);

	CMsvStore* store = aEntry.ReadStoreL();
	CleanupStack::PushL(store);

	header->RestoreL(*store);
	iRichText->Reset();
	store->RestoreBodyTextL(*iRichText);

	CleanupStack::PopAndDestroy(store);

	return header;
	}

EXPORT_C CSmsHeader* CSmsTestUtils::GetHeaderLC(TMsvId aId)
	{
	iMsvEntry->SetEntryL(aId);
	return DoGetHeaderLC(*iMsvEntry);
	}
