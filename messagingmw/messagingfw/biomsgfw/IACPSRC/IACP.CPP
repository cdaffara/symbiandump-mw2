// Copyright (c) 1998-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// IACP.CPP
//
    
#include <msvids.h>
#include <mentact.h>
#include <msvapi.h>
#include "BSP.H"
#include "IACP.H"
#include "ISPP.H"
#include "ISSP.H"
#include "IMP.H"
#include "SMSP.H"
#include "TVMP.H"
#include "gprsp.h"
#include "wwwp.h"
#include "IACPDEF.H"
#include "IACPERR.H"
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS 
#include "tmsvbioinfo.h"
#endif

const TInt SMS_SCRIPT_DATA_LEN = 5;
const TInt SMS_SCRIPT_ADD_LEN  = 4;

const TInt KParsedFieldsGranularity=16;

#define KUidMsvBIOInternetAccessPointMessageType  0x1000552F 

#define KCharSlash			'/'
_LIT(KIacpLowerY, "y");
_LIT(KIacpLowerT, "t");
_LIT(KIacpLowerN, "n");
_LIT(KIacpLowerF, "f");

//
// Constructor
//
CIacSettingsParser::CIacSettingsParser(CRegisteredParserDll& aRegisteredParserDll, CMsvEntry& aEntry, RFs& aFs)
:CBaseScriptParser2(aRegisteredParserDll, aEntry, aFs)
	{
	}

//
// Factory fns
//
EXPORT_C CIacSettingsParser* CIacSettingsParser::NewL(CRegisteredParserDll& aRegisteredParserDll, CMsvEntry& aEntry, RFs& aFs)
	{
	CIacSettingsParser* self= new(ELeave) CIacSettingsParser(aRegisteredParserDll,aEntry,aFs);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

//
// 2nd stage of construction
//
void CIacSettingsParser::ConstructL()
	{
	iParsedFieldArray = new(ELeave) CArrayPtrSeg<CParsedField>(KParsedFieldsGranularity);	
	iSmsParsed=EFalse;
	iParsedFieldCollections = new(ELeave)CArrayPtrSeg<CParsedFieldCollection>(4);
	CActiveScheduler::Add(this);
	}

//
// Destruction
//
CIacSettingsParser::~CIacSettingsParser()
	{
	Cancel();

	
	if(iParsedFieldCollections)
		{
		iParsedFieldCollections->ResetAndDestroy();
		delete iParsedFieldCollections;
		}

	if(iParsedFieldArray)
		{
		iParsedFieldArray->ResetAndDestroy();
		delete iParsedFieldArray;
		}
	delete iSmsBuf;
	delete iMailParser;
	delete iIspParser;
	delete iScriptParser;
	delete iSmsParser;
	delete iTelVoiceMailboxParser;
	delete iGprsParser;
	delete iWWWHotlistParser;
	}

//
// 
//
void CIacSettingsParser::RestoreParsedDataL()
	{
	//	We are assuming the server context is set to the Bio Msg entry	
	iEntry.SetEntryL(iEntryId);

	if(iEntry.Entry().MtmData3() != KBIO_MSG_ENTRY_PARSED && iEntry.Entry().MtmData3() != KBIO_MSG_ENTRY_PROCESSED)
		{
		iSmsParsed=EFalse; //sms not parsed yet
		User::Leave(KIacpErrSmsDataNotRestored);
		}

	if(!iEntry.HasStoreL())
		{
		iSmsParsed=EFalse; //sms not parsed yet
		User::Leave(KIacpErrSmsDataNotRestored);
		}
			
	CMsvStore* store=iEntry.EditStoreL();
	CleanupStack::PushL(store);
	RestoreL(*store);
	CleanupStack::PopAndDestroy();//store

	// work out what type it is
	PopulateFieldCollectionsL();
	}

void CIacSettingsParser::ParseL(TRequestStatus& aStatus,const TDesC& aSms)
	{
	__ASSERT_DEBUG (aSms.Length() > 0 ,User::Panic(KIACP, EIacpEmptyBuffer));
	//make a local copy of sms
	if(iSmsBuf)
		{
		delete iSmsBuf;
		iSmsBuf=NULL;
		}

	iSmsBuf = aSms.AllocL();
	iSms = iSmsBuf->Des(); //initialise Tlex

	ChangeStateL(EParseSms); //Set to initial request
	aStatus = KRequestPending;
	iReport = &aStatus;
	}
//
// Create service or database entries
//
void CIacSettingsParser::ProcessL(TRequestStatus& aStatus)
	{
	iState = EProcessSms;
	iEntryId= iEntry.Entry().Id(); //store id of Bio Msg entry 

	__ASSERT_DEBUG(iEntry.Entry().iMtm==KUidSmartMessageMtm, User::Panic(KIACP,EIacpInvalidEntry));
	__ASSERT_DEBUG(iEntry.Entry().iBioType==KUidMsvBIOInternetAccessPointMessageType, User::Panic(KIACP,EIacpInvalidEntry));
	__ASSERT_DEBUG(iEntry.Entry().Failed()== 0 , User::Panic(KIACP,EIacpInvalidEntry));

	ChangeStateL(ESetParsersData); //Set to initial state
	aStatus = KRequestPending;
	iReport = &aStatus;
	}


void CIacSettingsParser::ChangeStateL(TParseSession aState)
	{
	iState = aState;
	switch (iState)
		{
		case EParseSms:
			ParseSmsL();
			break;

		case ECheckData:
			iCurrentSettingsCtr = 0;
			CheckMandatoryDataL();
			break;

		case ESetParsersData:
			iCurrentSettingsCtr = 0;
			SetDataL();
			break;

		case EProcessSms:
			// Commit parsed data
			DoProcessL();
			break;

		default:
			break;
		}

		RequestComplete(iStatus,KErrNone);
		SetActive();
	}

void CIacSettingsParser::CheckMandatoryDataL()
	{
	CParsedFieldCollection& currentFieldCollection = *(*iParsedFieldCollections)[iCurrentSettingsCtr];
	TSmsType currentType = currentFieldCollection.MessageType();
	switch(currentType)
		{
	case EBasicMail:
		delete iMailParser;
		iMailParser = NULL;
		iMailParser = CMailParser::NewL();
		iMailParser->CheckMandatoryFieldsL(currentFieldCollection);
		break;
	case EBasicIAP:
	case EExtendedIAP:
		delete iIspParser;
		iIspParser = NULL;
		iIspParser = CIspParser::NewL(currentType);
		iIspParser->CheckMandatoryFieldsL(currentFieldCollection);
		break;
	case EExtScriptSettings:
	case EExtSmsSettings:
	case EExtTelephoneSettings:
	case EExtendedMail:
	case EExtWWWHostListItem:
	case EExtTelnetSettings:
	case EExtTerminalSettings:
	case EExtWWWSettings:	
	case EExtTTMLSettings:
	case EExtFTPSettings:
	case EExtInternetSettings:
	case EExtWWWAutofetchSettings:
	case ESmsTypeUndefined:
	case EBasicMailIAP:
	case EExtGprsIAP:	
	case EExtGprsMail:
	case EExtGprsIAPMail:
	case ESmsMixedContent:
		break;
	case EExtGprsSettings:
		delete iGprsParser;
		iGprsParser = NULL;
		iGprsParser = CGprsParser::NewL();
		iGprsParser->CheckMandatoryFieldsL(currentFieldCollection);
		break;
		}
	}

void CIacSettingsParser::SetDataL()
	{
	if(!iSmsParsed)
		{
		RestoreParsedDataL();
		}
	SetParsersDataL();
	}

void CIacSettingsParser::DoProcessL()
	{
	switch((*iParsedFieldCollections)[iCurrentSettingsCtr]->MessageType())
		{
		case EBasicMail:
			iMailParser->ProcessL(iEntry);
			break;
		case EBasicIAP:
			iIspParser->ProcessL(iConnectionPreference);
			break;
		case EExtendedIAP:
			iIspParser->ProcessL(iConnectionPreference);
			break;
		case EExtScriptSettings:
			iScriptParser->ProcessL(iEntry);
			break;
		case EExtSmsSettings:
			iSmsParser->ProcessL(iEntry);
			break;
		case EExtTelephoneSettings:
			iTelVoiceMailboxParser->ProcessL(iEntry);
			break;
		case EExtGprsSettings:
			iGprsParser->ProcessL(iConnectionPreference);
			break;
		case EExtWWWHostListItem:
			iWWWHotlistParser->ProcessL(iEntry);
			break;
		default:
			User::Leave(KIacpUnknownSmsType);
		}
	}
//
//
//
void CIacSettingsParser::DoCancel()
	{
	User::RequestComplete(iReport,KErrCancel);
	}

//
//  
//
TInt CIacSettingsParser::RunError(TInt aError)
	{
	User::RequestComplete(iReport, aError);
	return KErrNone;
	}

//
//  
//
void CIacSettingsParser::RunL()
	{
	TInt eCode = iStatus.Int();

	if(eCode==KErrNone)
		{
		switch(iState)
			{
			case EParseSms:
				ChangeStateL(ECheckData);
				return;

			case ECheckData:
				if(++iCurrentSettingsCtr < iParsedFieldCollections->Count())
					{
					CheckMandatoryDataL();
					SetActive();
					RequestComplete(iStatus, KErrNone);
					return;
					}
				else
					{
					// Storing parsed data array as a new stream within the CMsvStore
					// associated with the Msg Entry
					StoreParsedDataL();
					iSmsParsed=ETrue;
					}
				break;

			case ESetParsersData:
				ChangeStateL(EProcessSms);
				return;
				
			case EProcessSms:
				if(eCode == KErrNone)
					{
					if(++iCurrentSettingsCtr < iParsedFieldCollections->Count())
						{
						// set connections preferences flag
						TSmsType currentType = (*iParsedFieldCollections)[iCurrentSettingsCtr-1]->MessageType();
						if(currentType==EBasicIAP || currentType== EExtGprsSettings || currentType== EExtendedIAP)
							{
							if(iConnectionPreference==EIacpAttempCreateAsFirstRank)
								iConnectionPreference =  EIacpAttempCreateAsSecondRank;
							else
								iConnectionPreference = EIacpDoNotCreatePreference;
							}
						// If IAP settings are found, store a ref to them
						if(currentType== EBasicIAP || currentType== EExtendedIAP)
							{
							iDefaultIAP = iIspParser->IapEntryId();
							// If Mail settings have been found previously, associate mail with IAP
							if(iDefaultMail)
								{
								iMailParser->AssociateIAPWithMailL(iEntry,iDefaultIAP);
								}
							}
						// If Mail settings are found, set a flag
						if(currentType== EBasicMail || currentType== EExtendedMail)
							{
							iDefaultMail = 1;
							// If IAP settings have ben found previously, associate mail with IAP
							if(iDefaultIAP)
								{
								iMailParser->AssociateIAPWithMailL(iEntry,iDefaultIAP);
								}
							}
						
						
						SetParsersDataL();
						iState = ESetParsersData;
						SetActive();
						RequestComplete(iStatus,KErrNone);
						return;
						}
					else
						eCode = CompleteMessage();
					}
					TSmsType currentType = (*iParsedFieldCollections)[iCurrentSettingsCtr-1]->MessageType();
					// If IAP settings are found, store a ref to them
					if(currentType== EBasicIAP || currentType== EExtendedIAP)
						{
						iDefaultIAP = iIspParser->IapEntryId();
						// If Mail settings have been found previously, associate mail with IAP
						if(iDefaultMail)
							{
							iMailParser->AssociateIAPWithMailL(iEntry,iDefaultIAP);
							}
						}
					// If Mail settings are found, set a flag
					if(currentType== EBasicMail || currentType== EExtendedMail)
						{
						iDefaultMail = 1;
						// If IAP settings have ben found previously, associate mail with IAP
						if(iDefaultIAP)
							{
							iMailParser->AssociateIAPWithMailL(iEntry,iDefaultIAP);
							}
						}
				break;
			}
		}

	User::RequestComplete(iReport,eCode);
	}

//
//
//
TInt CIacSettingsParser::CompleteMessage()
	{
	TMsvEntry entry = iEntry.Entry();
	entry.SetMtmData3(KBIO_MSG_ENTRY_PROCESSED);
	TRAPD(error, iEntry.ChangeL(entry));
	return error;
	}
//
// Untility function which sends completion signal to client active object
//
void CIacSettingsParser::RequestComplete(TRequestStatus& aStatus,TInt aCompletion)
	{
	TRequestStatus* statusPtr=&aStatus;
	User::RequestComplete(statusPtr,aCompletion);
	}
//
//
//Data structure "LeftToken:RightToken<LF>"
//
void CIacSettingsParser::ParseSmsL()
	{
	iEntryId= iEntry.Entry().Id(); //store id of Bio Msg entry if changed 
	__ASSERT_DEBUG(iEntry.Entry().iMtm==KUidSmartMessageMtm, User::Panic(KIACP,EIacpInvalidEntry));
	__ASSERT_DEBUG(iEntry.Entry().iBioType==KUidMsvBIOInternetAccessPointMessageType, User::Panic(KIACP,EIacpInvalidEntry));
	__ASSERT_DEBUG(iEntry.Entry().Failed()== 0 , User::Panic(KIACP,EIacpInvalidEntry));

	iParsedFieldArray->ResetAndDestroy();
	//if SMS allready parsed no need to parse again, restore the parsed data
	iEntry.Entry().MtmData3() == KBIO_MSG_ENTRY_PARSED ? iSmsParsed = ETrue : iSmsParsed = EFalse;
	if(iSmsParsed)
		{
		RestoreParsedDataL();
		return;
		}

	// default to create 1st ranking Connection preference ntry in CommDb if required
	iConnectionPreference = EIacpAttempCreateAsFirstRank;
	
	DoParseL();
	}
//
// append sms data field/value to array,
// set iSmsType to EBasicMail orEBasicIAP
//
void CIacSettingsParser::DoParseL()
	{
	TBuf<256> tempBuf;
	// Set extraction mark at first character
    iSms.SkipSpaceAndMark();
	// might or might not have header?
	if(iSms.Peek() == KCharSlash)
		{
		// Check first line is <header>
		iSms.SkipCharacters();
		if (iSms.MarkedToken() != KSmsHeader)
			{
			User::Leave(KBspInvalidMessage);
			}
       
		tempBuf=iSms.MarkedToken(); //save a copy of header field
		//appending header field to array
		CParsedField* headerField= new (ELeave) CParsedField;
		CleanupStack::PushL(headerField);
		SetDataFieldsL(*headerField, SMS_HEADER,tempBuf,ETrue);
		iParsedFieldArray->AppendL(headerField);
		iSms.Inc();
		CleanupStack::Pop();//headerField (No need to delete the CParsedField object, since it's now owned by the array)
		}

	// might or might not have password?
	// if line does not contain ':' then we consider it as password
	// if line contains ':' then we check the left token for any valid field names,
	// a line with valid field name is not considered as password.
    TBool PwdWithColumn=EFalse;
	iSms.SkipSpaceAndMark();
	while ( iSms.Peek()!= '\n'  && !iSms.Eos())
		iSms.Inc();
	
	TLex lex(iSms.MarkedToken());
	while (lex.Peek() != ':' && lex.Peek() != '\n' && !lex.Eos())
		lex.Inc();
	
	if(lex.Peek() == ':')
		{
		TPtrC LeftToken = lex.MarkedToken(); // extract left token
		if(!IsValidToken(LeftToken))
			PwdWithColumn=ETrue;//we consider it as password containing ':'
		else
			iSms.Inc(-iSms.MarkedToken().Length());	//reset pos. (password is not included in SMS message)
		}


	if(lex.Peek() != ':' || PwdWithColumn )
		{
		if (iSms.TokenLength() >255)
			User::Leave(KErrOverflow); //leave if there is a ridiculously long password even though Nokia Smart Messaging Spec 3 allows password of length 0-infinity
		tempBuf = iSms.MarkedToken(); //save a copy of password field 
		//appending password field to array
		CParsedField* passwordField = new (ELeave) CParsedField();
		CleanupStack::PushL(passwordField);
		SetDataFieldsL(*passwordField, SMS_PASSWORD,tempBuf,ETrue);
		iParsedFieldArray->AppendL(passwordField);
		CleanupStack::Pop();//passwordField
		}

	//start parsing body of message
	while (!iSms.Eos())
		{
		iSms.SkipSpaceAndMark();
		while (iSms.Peek() != '\n'  && !iSms.Eos())
			iSms.Inc();
		
		TLex lex(iSms.MarkedToken());
		// extract Line i.e "LeftToken:RightToken<LF>", then asign to TLex object
		while (lex.Peek() != ':' && lex.Peek() != '\n' && !lex.Eos())
			lex.Inc();
		
		//we are only interested in lines containing a colon delimeter ':'
		// other text lines are ignored i.e "Welcome !<LF>"
		if(lex.Peek() == ':')
			{
			if (lex.TokenLength() > 0 ) // if valid potential token
				{
				TPtrC LeftToken = lex.MarkedToken() ;        // extract left token
				if(IsValidToken(LeftToken))
					{					 
					tempBuf= LeftToken; //save of copy of left token
					// get the right token
					lex.Inc();  // go past the ':'
					
					//Check if we have a case of script message, then deal with it.
					//--start script data parsing...
					TInt TokenLength=0; 
					TInt pos=0;
					TBool FoundScriptData=EFalse;
					
					if(LeftToken.CompareF(SMS_SCRIPT_DATA) ==0)
						{
						TokenLength= SMS_SCRIPT_DATA_LEN;
						
						if(KErrNotFound ==(pos=iSmsBuf->Find(SMS_SCRIPT_DATA)))
							User::Leave(KIacpMandatoryDataNotSet);
						
						FoundScriptData=ETrue;
						}
					else if(LeftToken.CompareF(SMS_SCRIPT_ADD) ==0)
						{
						TokenLength= SMS_SCRIPT_ADD_LEN;
						
						if(KErrNotFound ==(pos=iSmsBuf->Find(SMS_SCRIPT_ADD)))
							User::Leave(KIacpMandatoryDataNotSet);

						FoundScriptData=ETrue;
						}

					//if found script data extract and append the data to the array then exit the while loop
					if(FoundScriptData)
						{
						TPtrC PtrScript= iSmsBuf->Right(iSmsBuf->Length()-(pos+TokenLength+1));//add 1 for ':'

						CParsedField* parsedField = new (ELeave) CParsedField();						
						CleanupStack::PushL(parsedField);
						SetDataFieldsL(*parsedField, tempBuf, PtrScript,isMandatoryData(tempBuf));
						iParsedFieldArray->AppendL(parsedField);
						CleanupStack::Pop();//parsedField
						break;
						}
					//--...end script data parsing

					lex.SkipSpaceAndMark();
					while (lex.Peek() != '\n' && !lex.Eos())
						lex.Inc();
					
					//appending parsedField to array
					CParsedField* parsedField = new (ELeave) CParsedField();
					CleanupStack::PushL(parsedField);
					SetDataFieldsL(*parsedField, tempBuf, lex.MarkedToken(),isMandatoryData(tempBuf));
					iParsedFieldArray->AppendL(parsedField);
					CleanupStack::Pop();//parsedField
					}
				}
			}

		iSms.Inc();
		}
	PopulateFieldCollectionsL();
	}

//
// Divide parsed field array into individual groups of settings
//
void CIacSettingsParser::PopulateFieldCollectionsL()
	{
	TSmsType currentSmsType;
	iSmsType = ESmsTypeUndefined;
	iParsedFieldCollections->ResetAndDestroy();
	CParsedFieldCollection* currentParsedFieldCollection=NULL;

	TInt count = iParsedFieldArray->Count();
	for(TInt loop = 0; loop < count; loop++)
		{
		CParsedField& field = *(*iParsedFieldArray)[loop];
		if(SetBioMsgTypeL(field.FieldName(),currentSmsType))
			{
			// append previous if it exists
			if(currentParsedFieldCollection)
				{
				iParsedFieldCollections->AppendL(currentParsedFieldCollection);
				CleanupStack::Pop();	// currentParsedFieldCollection
				}

			currentParsedFieldCollection = new(ELeave)CParsedFieldCollection(currentSmsType);
			CleanupStack::PushL(currentParsedFieldCollection);
			}

		if(currentParsedFieldCollection)
			currentParsedFieldCollection->AppendL(&field);
		}
	// add last collection
	if(currentParsedFieldCollection)
		{
		iParsedFieldCollections->AppendL(currentParsedFieldCollection);
		CleanupStack::Pop();	// currentParsedFieldCollection
		}
	else
		User::Leave(KIacpMandatoryDataNotSet);
	}

//
//
//
void CIacSettingsParser::GetPtrToScriptDataL(HBufC*& aPtrScriptData)
	{
	HBufC* PtrScript= iSmsBuf;
	TInt loc = iSmsBuf->Find(SMS_SCRIPT_DATA);
	PtrScript+=loc;
	aPtrScriptData =PtrScript;
	}

//
//
//
TBool CIacSettingsParser::SetBioMsgTypeL(const TDesC& aLeftToken, TSmsType& aType)
	{
	// basic internet
	if(aLeftToken.CompareF(KINTERNET) == 0 )
		{
		aType = IsSmsExtendedIAP() ? EExtendedIAP : EBasicIAP;

		if(iSmsType == ESmsTypeUndefined)
			iSmsType = aType;
		else if(iSmsType == EExtGprsSettings)
			iSmsType = EExtGprsIAP;
		else if(iSmsType == EBasicMail)
			iSmsType = EBasicMailIAP;
		else if(iSmsType == EBasicMailIAP)
			iSmsType = EBasicMailIAP;
		else
			iSmsType = ESmsMixedContent;
		}
	// gprs
	else if(aLeftToken.CompareF(KGPRS) == 0 )
		{
		if(iSmsType == ESmsTypeUndefined)
			iSmsType = EExtGprsSettings;
		else if(iSmsType == EBasicMail)
			iSmsType = EExtGprsMail;
		else if(iSmsType == EBasicIAP)
			iSmsType = EExtGprsIAP;
		else if(iSmsType == EBasicMailIAP)
			iSmsType = EExtGprsIAPMail;
		else
			iSmsType = ESmsMixedContent;

		aType = EExtGprsSettings;
		}
	// basic email
	else if(aLeftToken.CompareF(KMAIL) == 0 )
		{
		if(iSmsType == ESmsTypeUndefined)
			iSmsType = EBasicMail;
		else if(iSmsType == EExtGprsSettings)
			iSmsType = EExtGprsMail;
		else if(iSmsType == EBasicIAP)
			iSmsType = EBasicMailIAP;
		else if(iSmsType == EExtGprsIAP)
			iSmsType = EExtGprsIAPMail;
		else if(iSmsType == EBasicMailIAP)
			iSmsType = EBasicMailIAP;
		else
			iSmsType = ESmsMixedContent;

		aType = EBasicMail;
		}
	// script settings
	else if(aLeftToken.CompareF(KSCRIPT) == 0 ) //Script Settings
		{
		if(iSmsType == ESmsTypeUndefined)
			aType = iSmsType = EExtScriptSettings;
		else
			iSmsType = ESmsMixedContent;
		}
	// SMS settings
	else if(aLeftToken.CompareF(KSMS) == 0 ) //SMS Settings
		{
		if(iSmsType == ESmsTypeUndefined)
			aType = iSmsType = EExtSmsSettings;
		else
			iSmsType = ESmsMixedContent;
		}
	// voice telephone settings
	else if(aLeftToken.CompareF(KTELEPHONE) == 0 ) //Telephone Settings (voice mailbox number)
		{
		if(iSmsType == ESmsTypeUndefined)
			aType = iSmsType = EExtTelephoneSettings;
		else
			iSmsType = ESmsMixedContent;
		}
	// WWW Hotlist items
	else if(aLeftToken.CompareF(KWWWHotlist) == 0 ) 
		{
		if(iSmsType == ESmsTypeUndefined)
			aType = iSmsType = EExtWWWHostListItem;
		else if(iSmsType == EExtWWWHostListItem)
			return EFalse;
		else
			iSmsType = ESmsMixedContent;
		}
	else 
		return EFalse;

	return ETrue;
	}
//
//
//
TBool CIacSettingsParser::IsSmsExtendedIAP()
	{
	TInt count = iParsedFieldArray->Count();
	for(TInt loop = 0; loop < count; loop++)
		{
		TPtrC fieldName = (*iParsedFieldArray)[loop]->FieldName();
		if(fieldName.Length()==4)
			{
			if(fieldName.CompareF(SMS_PROXY_EXCEPTIONS)   == 0 || fieldName.CompareF(SMS_PROXY_PORT_NUMBER)   == 0 ||
				fieldName.CompareF(SMS_PROXY_SERVER_NAME) == 0 || fieldName.CompareF(SMS_LOGIN_CUSTOMISATION) == 0 ||
				fieldName.CompareF(SMS_ENABLE_SW_COMP)       == 0 || fieldName.CompareF(SMS_SECURE_PROXY)        == 0 ||
				fieldName.CompareF(SMS_SECURE_PORT) == 0 )
					{
					return ETrue;
					}
			}
		}
	return EFalse;
	}
//
//
//
TBool CIacSettingsParser::isMandatoryData(const TDesC& aFieldName)
	{
	__ASSERT_DEBUG(aFieldName.Length() > 0 ,User::Panic(KIACP,EIacpEmptyBuffer));

	if(aFieldName.Length() == 5)
		{
		if(aFieldName.CompareF(SMS_ISP_M_NAME)   == 0 || aFieldName.CompareF(SMS_ISP_I_NAME) ==0 ||
		  (aFieldName.CompareF(GPRSS_PDP_TYPE))  )
			return ETrue;
		}
	else if(aFieldName.Length() == 4)
		{	
		if(aFieldName.CompareF(SMS_MAILBOX_NAME)   == 0 || aFieldName.CompareF(SMS_MAILBOX_PASS) == 0 ||
		   aFieldName.CompareF(SMS_RECEIVING_HOST) == 0 || aFieldName.CompareF(SMS_SENDING_HOST) == 0 ||
		   aFieldName.CompareF(SMS_USER_EMAIL_ADDR)== 0 || aFieldName.CompareF(SMS_MAIL_PROTOCOL)== 0 ||
		   aFieldName.CompareF(GPRSS_APN)          == 0 || aFieldName.CompareF(GPRSS_PDP_ADDRESS) == 0)
			return ETrue;
		}

	return EFalse;
	}

TBool CIacSettingsParser::IsValidToken(const TDesC& aToken)
	{
	__ASSERT_DEBUG(aToken.Length() > 0 ,User::Panic(KIACP,EIacpEmptyBuffer));

	// case EBasicMail
	if(aToken.Length() == 5)
		{
		if (aToken.CompareF(SMS_ISP_M_NAME)    ==0 ||aToken.CompareF(SMS_HOTLIST_ITEM_NAME) == 0)
			return ETrue;
		}
	else if(aToken.Length() == 4)
		{
		if (aToken.CompareF(SMS_MAILBOX_NAME)    ==0 || aToken.CompareF(SMS_MAILBOX_PASS)   ==0 ||
			aToken.CompareF(SMS_USER_EMAIL_ADDR) ==0 || aToken.CompareF(SMS_RECEIVING_HOST) ==0 ||
			aToken.CompareF(SMS_SENDING_HOST)    ==0 || aToken.CompareF(SMS_MAIL_PROTOCOL)  ==0 ||
			aToken.CompareF(SMS_FOLDER_PATH)==0 ||
			aToken.CompareF(SMS_HOTLIST_ITEM_URL)  ==0
			|| aToken.CompareF(SMS_HOTLIST_AUTOSELECT_IAP)  ==0 || aToken.CompareF(SMS_HOTLIST_ACCESS_POINT)  ==0
			|| aToken.CompareF(SMS_HOTLIST_FOLDER)  ==0)
			return ETrue;
		}

	//case EBasicIAP	
	if(aToken.Length() == 5)
		{
		if (aToken.CompareF(SMS_ISP_I_NAME)		         ==0 || aToken.CompareF(SMS_IP_NAME_SERVER1) ==0 ||
			aToken.CompareF(SMS_IP_NAME_SERVER2)==0) 
			return ETrue;
		}
	else if(aToken.Length() == 4)
		{
		if (aToken.CompareF(SMS_INIT_STRING)		 ==0 ||
			aToken.CompareF(SMS_DEFAULT_TEL_NUM)  ==0 || aToken.CompareF(SMS_LOGIN_NAME) ==0 ||
			aToken.CompareF(SMS_PROMPT_FOR_LOGIN) ==0 || aToken.CompareF(SMS_LOGIN_PASS) ==0 ||
			aToken.CompareF(SMS_IP_NETMASK) ==0 || aToken.CompareF(SMS_IP_GATEWAY) ==0)
			return ETrue;
		}
	else if(aToken.Length() == 3)
		{
		if(aToken.CompareF(SMS_IP_ADDR) ==0)
			return ETrue;
		}

	//caes EExtendedIAP
	if(aToken.Length() == 4)
		{
		if (aToken.CompareF(SMS_PROXY_EXCEPTIONS) ==0 || aToken.CompareF(SMS_PROXY_PORT_NUMBER)  ==0 ||
			aToken.CompareF(SMS_PROXY_SERVER_NAME)==0 || aToken.CompareF(SMS_LOGIN_CUSTOMISATION)==0 ||
			aToken.CompareF(SMS_ENABLE_SW_COMP)	  ==0 || aToken.CompareF(SMS_SECURE_PROXY)		 ==0 ||
			aToken.CompareF(SMS_SECURE_PORT)==0)
			return ETrue;
		}

	// GPRS settings
	if(aToken.Length() == 4)
		{
		if  (aToken.CompareF(GPRSS_APN)==0              || aToken.CompareF(GPRSS_PDP_ADDRESS)==0 ||
			 aToken.CompareF(GPRSS_IF_NAME)==0          || aToken.CompareF(GPRSS_LOGIN_NAME)==0  ||
			 aToken.CompareF(GPRSS_PROMPT_FOR_LOGIN)==0 || aToken.CompareF(GPRSS_LOGIN_PASS)==0  ||
			 aToken.CompareF(GPRSS_IP_ADDR)==0          || aToken.CompareF(GPRSS_DNS_FROM_SERVER)==0  ||		 
			 aToken.CompareF(GPRSS_IP_NETMASK)==0       
			 )
			 return ETrue;
		}
	else if (aToken.Length() == 5)
		{
		if  (aToken.CompareF(GPRSS_PDP_TYPE)==0                || aToken.CompareF(GPRSS_DNS_FROM_SERVER)==0  ||
			 aToken.CompareF(GPRSS_IP_NAME_SERVER1)==0         || aToken.CompareF(GPRSS_IP_NAME_SERVER2)==0  ||
			 aToken.CompareF(GPRSS_NAME)==0 
			)
			return ETrue;
		}
	else if(aToken.Length() == 3)
		{
		if  (aToken.CompareF(GPRSS_IP_ADDR)==0)
			return ETrue;
		}

	//case EExtScriptSettings
	if(aToken.Length() == 5)
		{
		if(aToken.CompareF(SMS_SCRIPT_NAME) ==0 || aToken.CompareF(SMS_SCRIPT_TYPE) ==0 ||
		   aToken.CompareF(SMS_SCRIPT_DATA) ==0 )
			return ETrue;
		}
	else if(aToken.Length() == 4)
		{
		if(aToken.CompareF(SMS_SCRIPT_ADD) ==0)
			return ETrue;
		}


	//case EExtSmsSettings
	if(aToken.Length() == 5)
		{
		if(aToken.CompareF(SMS_SERVICE_CENTER_NAME) ==0 )
			return ETrue;
		}
	else if(aToken.Length() == 4)
		{
		if(aToken.CompareF(SMS_SERVICE_CENTER_ADDRESS) ==0)
			return ETrue;
		}


	//case EExtTelephoneSettings
	if(aToken.Length() == 4)
		{
		if(aToken.CompareF(SMS_TEL_VOICE_MAILBOX_NUM) ==0)
			return ETrue;
		}


	return EFalse; //Token not valid
	}
//
//
//
void CIacSettingsParser::SetParsersDataL()
	{	
	CParsedFieldCollection& parsedFields = *(*iParsedFieldCollections)[iCurrentSettingsCtr];
	TSmsType dataType = parsedFields.MessageType();
	switch(dataType)
		{
		case EBasicMail:
			if (iMailParser)
				{
				delete iMailParser;
				iMailParser =NULL;
				}
			iMailParser = CMailParser::NewL();
			iMailParser->ParseL(parsedFields);
			break;
		case EBasicIAP:
			if (iIspParser)
				{
				delete iIspParser;
				iIspParser =NULL;
				}
			iIspParser = CIspParser::NewL(EBasicIAP);
			iIspParser->ParseL(parsedFields);
			break;

		case EExtendedIAP:
			if (iIspParser)
				{
				delete iIspParser;
				iIspParser =NULL;
				}
			iIspParser = CIspParser::NewL(EExtendedIAP);
			iIspParser->ParseL(parsedFields);
			break;

		case EExtScriptSettings:
			if (iScriptParser)
				{
				delete iScriptParser;
				iScriptParser =NULL;
				}
			iScriptParser = CScriptParser::NewL();
			iScriptParser->ParseL(parsedFields);
			break;
		case EExtSmsSettings:
			if (iSmsParser)
				{
				delete iSmsParser;
				iSmsParser =NULL;
				}
			iSmsParser = CSmsParser::NewL();
			iSmsParser->ParseL(parsedFields);

			break;
		case EExtTelephoneSettings:
			if (iTelVoiceMailboxParser)
				{
				delete iTelVoiceMailboxParser;
				iTelVoiceMailboxParser =NULL;
				}
			iTelVoiceMailboxParser = new(ELeave)CTelVoiceMailboxParser();
			iTelVoiceMailboxParser->ParseL(parsedFields);
			break;
		case EExtGprsSettings:
			if(iGprsParser)
				{
				delete iGprsParser;
				iGprsParser = NULL;
				}
			iGprsParser = CGprsParser::NewL();
			iGprsParser->ParseL(parsedFields);
			break;
		case EExtWWWHostListItem:
			if (iWWWHotlistParser)
				{
				delete iWWWHotlistParser;
				iWWWHotlistParser =NULL;
				}
			iWWWHotlistParser = CWWWHotlistParser::NewL(iFs);
			iWWWHotlistParser->ParseL(parsedFields);
			break;
		default:
			break;
		}
	}

//
//
//
void CIacSettingsParser::SetDataFieldsL(CParsedField& aParsedField, const TDesC& aName, const TDesC& aValue, TBool aBool)
	{
	aParsedField.SetFieldNameL(aName);
	aParsedField.SetFieldValueL(aValue);
	aParsedField.SetMandatoryField(aBool);
	}
//
//
//
CArrayPtrSeg<CParsedField>& CIacSettingsParser::ParsedFieldArray() const
	{
	return *iParsedFieldArray;
	}

//
//
//
void CIacSettingsParser::SetFieldValueL(const TDesC& aFieldName, const TDesC& aFieldValue)
	{
	TInt count = iParsedFieldArray->Count();
	for ( TInt i=0; i < count; i++ )
		{
		if(iParsedFieldArray->At(i)->FieldName().CompareF(aFieldName)==0)
			{
			iParsedFieldArray->At(i)->SetFieldValueL(aFieldValue);
			return;
			}
		}
	}
//
//
//
TBool CIacSettingsParser::GetMandatoryField(const TDesC& aFieldName) const
	{
	TInt count = iParsedFieldArray->Count();
	for ( TInt i=0; i < count; i++ )
		{
		if(iParsedFieldArray->At(i)->FieldName().CompareF(aFieldName)==0)
			return iParsedFieldArray->At(i)->MandatoryField();
		}

	return EFalse;
	}
//
// store parsed data array as a stream store of SMS entry
//
void CIacSettingsParser::StoreParsedDataL()
	{
	iEntry.SetEntryL(iEntryId);
	CMsvStore* store=iEntry.EditStoreL();
	CleanupStack::PushL(store);
	StoreL(*store);
	CleanupStack::PopAndDestroy();//store

	// indicate that Bio Msg has been parsed OK and Store has been created.
	iEntry.SetEntryL(iEntryId);
	TMsvEntry entry= iEntry.Entry();
	entry.SetMtmData3(KBIO_MSG_ENTRY_PARSED);
	// store iSmsType
	entry.SetMtmData1(iSmsType);
	//set SMS subject and Sender
	iEntry.ChangeL(entry);
	}


//
//
//
CParsedFieldCollection::CParsedFieldCollection(CIacSettingsParser::TSmsType aSmsType)
: CArrayPtrSeg<CParsedField>(8), iSmsType(aSmsType)
	{
	iCanCommit = ETrue;
	}

TPtrC CParsedFieldCollection::GetFieldValue(const TDesC& aFieldName) const
	{
	const TInt count = Count();

	for ( TInt i=0; i < count; i++ )
		{
		if(At(i)->FieldName().CompareF(aFieldName)==0)
			return At(i)->FieldValue();
		}
	return TPtrC();
	}

TInt CParsedFieldCollection::GetFieldValueAndLength(const TDesC& aFieldName, TPtrC& aValue) const
	{
	aValue.Set(GetFieldValue(aFieldName));
	return aValue.Length();
	}

void CParsedFieldCollection::GetTBoolL(const TDesC& aDes, TBool& aBool) const
	{
	__ASSERT_DEBUG(aDes.Length() > 0 ,User::Panic(KIACP,EIacpEmptyBuffer));

 	if ((aDes.Left(1).CompareF(KIacpLowerT)==0) || (aDes.Left(1).CompareF(KIacpLowerY)==0))
		aBool= ETrue;
	else if ((aDes.Left(1).CompareF(KIacpLowerF)==0) || (aDes.Left(1).CompareF(KIacpLowerN)==0))
		aBool= EFalse;
	else
		User::Leave(KIacpErrRightToken);
	}

//
//
//
void CParsedFieldCollection::GetTUint32NumL(const TDesC& aDes, TUint32& aTUint32Value) const
	{
	__ASSERT_DEBUG(aDes.Length() > 0 ,User::Panic(KIACP,EIacpEmptyBuffer));
	
	TLex lex(aDes);
    if (lex.Val(aTUint32Value,EDecimal) != KErrNone)
		User::Leave(KIacpErrRightToken);
	}

TBool CParsedFieldCollection::FieldNameExists(const TDesC& aFieldName) const
	{
	const TInt count = Count();

	for ( TInt i=0; i < count; i++ )
		{
		if(At(i)->FieldName().CompareF(aFieldName)==0)
			{
			return ETrue;
			}
		}
	return EFalse;
	} 
