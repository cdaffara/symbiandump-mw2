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
#include <smutset.h>
#include <msvapi.h>

#include "CConfigFileParserUtils.h"
#include "CSmsScriptUtils.h"

#include <gsmuelem.h> //For ESmsAlphabet

// Constants to read the values out of the settings file

// Service Centre
_LIT(KScriptItemSC, "SC");

// Default Service Centre
_LIT(KScriptItemDefaultSC, "DefaultSC");

// Bearer
_LIT(KScriptItemBearer, "SmsBearer");

// Encoding
_LIT(KScriptItemEncoding, "Encoding");

// Delivery Report
_LIT(KScriptItemDeliveryReport, "DeliveryReport");

// Reply Quoted
_LIT(KScriptItemReplyQuoted, "ReplyQuoted");

// Description Length
_LIT(KScriptItemDescriptionLength, "DescriptionLength");

// Can concatenate
_LIT(KScriptItemCanConcatenate, "CanConcatenate");

// Reply Path
_LIT(KScriptItemReplyPath, "ReplyPath");

// Reject duplicate
_LIT(KScriptItemRejectDuplicate, "RejectDuplicate");

// Validity period
_LIT(KScriptItemValidityPeriod, "ValidityPeriod");

// Delivery
_LIT(KScriptItemDelivery, "Delivery");

// Status Report Handling
_LIT(KScriptItemStatusReportHandling, "StatusReportHandling");

// Special Message Handling
_LIT(KScriptItemSpecialMessageHandling, "SpecialMessageHandling");

// Commdb Action
_LIT(KScriptItemSmsCommdbAction, "CommdbAction");

// Sms Bearer Action 
_LIT(KScriptItemSmsBearerAction, "SmsBearerAction");

// Message Conversion
_LIT(KScriptItemSmsMessageConversion, "MessageConversion");

// Validity Period Format
_LIT(KScriptItemSmsValidityPeriodFormat, "ValidityPeriodFormat");

// Class2 Folder
_LIT(KScriptItemClass2Folder, "Class2Folder");

// Recipient
_LIT(KRecipient, "Recipients");

// Message Body
_LIT(KMessageBody,	"Message");

//Bio Uid
_LIT(KBioUId, "BioUIdType");


class MDummySessionObserver : public MMsvSessionObserver
	{
public: 
	void HandleSessionEventL(TMsvSessionEvent,TAny*,TAny*,TAny*) {};
	};

CSmsSettings::TMobileSmsBearer CSmsScriptUtils::GetMobileSmsBearer(TDesC& value)
	{

	CSmsSettings::TMobileSmsBearer options;
	
	_LIT(KSmsBearerPacketOnly, "CSmsSettings::TMobileSmsBearer::ESmsBearerPacketOnly");
	_LIT(KSmsBearerCircuitOnly, "CSmsSettings::TMobileSmsBearer::ESmsBearerCircuitOnly");
	_LIT(KSmsBearerPacketPreferred, "CSmsSettings::TMobileSmsBearer::ESmsBearerPacketPreferred");
	_LIT(KSmsBearerCircuitPreferred, "CSmsSettings::TMobileSmsBearer::ESmsBearerCircuitPreferred");
	
	if(value.Compare(KSmsBearerPacketOnly) == 0)
		options = CSmsSettings::ESmsBearerPacketOnly;
	if(value.Compare(KSmsBearerCircuitOnly) == 0)
		options = CSmsSettings::ESmsBearerCircuitOnly;
	if(value.Compare(KSmsBearerPacketPreferred) == 0)
		options = CSmsSettings::ESmsBearerPacketPreferred;
	if(value.Compare(KSmsBearerCircuitPreferred) == 0)
		options = CSmsSettings::ESmsBearerCircuitPreferred;

	return options;
	}


TSmsDataCodingScheme::TSmsAlphabet CSmsScriptUtils::GetSmsAlphabet(TDesC& value)
	{

	TSmsDataCodingScheme::TSmsAlphabet options;
	
	_LIT(KSmsAlphabet7Bit, "TSmsDataCodingScheme::TSmsAlphabet::ESmsAlphabet7Bit");
	_LIT(KSmsAlphabet8Bit, "TSmsDataCodingScheme::TSmsAlphabet::ESmsAlphabet8Bit");
	_LIT(KSmsAlphabetUCS2, "TSmsDataCodingScheme::TSmsAlphabet::ESmsAlphabetUCS2");
	_LIT(KSmsAlphabetReserved, "TSmsDataCodingScheme::TSmsAlphabet::ESmsAlphabetReserved");
	_LIT(KSmsAlphabetMask, "TSmsDataCodingScheme::TSmsAlphabet::ESmsAlphabetMask");
	
	if(value.Compare(KSmsAlphabet7Bit) == 0)
		options = TSmsDataCodingScheme::ESmsAlphabet7Bit;
	if(value.Compare(KSmsAlphabet8Bit) == 0)
		options = TSmsDataCodingScheme::ESmsAlphabet8Bit;
	if(value.Compare(KSmsAlphabetUCS2) == 0)
		options = TSmsDataCodingScheme::ESmsAlphabetUCS2;
	if(value.Compare(KSmsAlphabetReserved) == 0)
		options = TSmsDataCodingScheme::ESmsAlphabetReserved;
	if(value.Compare(KSmsAlphabetMask) == 0)
		options = TSmsDataCodingScheme::ESmsAlphabetMask;

	return options;
	}
	
TSmsDelivery CSmsScriptUtils::GetSmsDelivery(TDesC& value)
	{

	TSmsDelivery options;

	_LIT(KSmsDeliveryImmediately, "TSmsDelivery::ESmsDeliveryImmediately");
	_LIT(KSmsDeliveryUponRequest, "TSmsDelivery::ESmsDeliveryUponRequest");
	_LIT(KSmsDeliveryScheduled, "TSmsDelivery::ESmsDeliveryScheduled");
	
	if(value.Compare(KSmsDeliveryImmediately) == 0)
		options = ESmsDeliveryImmediately;
	if(value.Compare(KSmsDeliveryUponRequest) == 0)
		options = ESmsDeliveryUponRequest;
	if(value.Compare(KSmsDeliveryScheduled) == 0)
		options = ESmsDeliveryScheduled;

	return options;
	}


CSmsSettings::TSmsReportHandling CSmsScriptUtils::GetSmsReportHandling(TDesC& value)
	{

	CSmsSettings::TSmsReportHandling options;
	
	_LIT(KMoveReportToInboxInvisible, "CSmsSettings::TSmsReportHandling::EMoveReportToInboxInvisible");
	_LIT(KMoveReportToInboxVisible, "CSmsSettings::TSmsReportHandling::EMoveReportToInboxVisible");
	_LIT(KDiscardReport, "CSmsSettings::TSmsReportHandling::EDiscardReport");
	_LIT(KDoNotWatchForReport, "CSmsSettings::TSmsReportHandling::EDoNotWatchForReport");
	_LIT(KMoveReportToInboxInvisibleAndMatch, "CSmsSettings::TSmsReportHandling::EMoveReportToInboxInvisibleAndMatch");
	_LIT(KMoveReportToInboxVisibleAndMatch, "CSmsSettings::TSmsReportHandling::EMoveReportToInboxVisibleAndMatch");
	_LIT(KDiscardReportAndMatch, "CSmsSettings::TSmsReportHandling::EDiscardReportAndMatch");
	
	if(value.Compare(KMoveReportToInboxInvisible) == 0)
		options = CSmsSettings::EMoveReportToInboxInvisible;
	if(value.Compare(KMoveReportToInboxVisible) == 0)
		options = CSmsSettings::EMoveReportToInboxVisible;
	if(value.Compare(KDiscardReport) == 0)
		options = CSmsSettings::EDiscardReport;
	if(value.Compare(KDoNotWatchForReport) == 0)
		options = CSmsSettings::EDoNotWatchForReport;
	if(value.Compare(KMoveReportToInboxInvisibleAndMatch) == 0)
		options = CSmsSettings::EMoveReportToInboxInvisibleAndMatch;
	if(value.Compare(KMoveReportToInboxVisibleAndMatch) == 0)
		options = CSmsSettings::EMoveReportToInboxVisibleAndMatch;
	if(value.Compare(KDiscardReportAndMatch) == 0)
		options = CSmsSettings::EDiscardReportAndMatch;

	return options;
	}



CSmsSettings::TSmsSettingsCommDbAction CSmsScriptUtils::GetSmsSettingsCommDbAction (TDesC& value)
	{

	CSmsSettings::TSmsSettingsCommDbAction options;
	
	_LIT(KNone, "CSmsSettings::TSmsSettingsCommDbAction::ENone");
	_LIT(KStoreToCommDb, "CSmsSettings::TSmsSettingsCommDbAction::EStoreToCommDb");
	
	if(value.Compare(KNone) == 0)
		options = CSmsSettings::ENone;
	if(value.Compare(KStoreToCommDb) == 0)
		options = CSmsSettings::EStoreToCommDb;

	return options;
	}


TSmsPIDConversion CSmsScriptUtils::GetSmsPIDConversion(TDesC& value)
	{

	TSmsPIDConversion options;
	
	_LIT(KSmsConvPIDNone, "TSmsPIDConversion::ESmsConvPIDNone");
	_LIT(KSmsConvFax, "TSmsPIDConversion::ESmsConvFax");
	_LIT(KSmsConvX400, "TSmsPIDConversion::ESmsConvX400");
	_LIT(KSmsConvPaging, "TSmsPIDConversion::ESmsConvPaging");
	_LIT(KSmsConvMail, "TSmsPIDConversion::ESmsConvMail");
	_LIT(KSmsConvErmes, "TSmsPIDConversion::ESmsConvErmes");
	_LIT(KSmsConvSpeech, "TSmsPIDConversion::ESmsConvSpeech");
	
	if(value.Compare(KSmsConvPIDNone) == 0)
		options = ESmsConvPIDNone;
	if(value.Compare(KSmsConvFax) == 0)
		options = ESmsConvFax;
	if(value.Compare(KSmsConvX400) == 0)
		options = ESmsConvX400;
	if(value.Compare(KSmsConvPaging) == 0)
		options = ESmsConvPaging;
	if(value.Compare(KSmsConvMail) == 0)
		options = ESmsConvMail;
	if(value.Compare(KSmsConvErmes) == 0)
		options = ESmsConvErmes;
	if(value.Compare(KSmsConvSpeech) == 0)
		options = ESmsConvSpeech;
	
	return options;
	}
	
	
TSmsFirstOctet::TSmsValidityPeriodFormat CSmsScriptUtils::GetSmsValidityPeriodFormat(TDesC& value)
	{

	TSmsFirstOctet::TSmsValidityPeriodFormat options;
	
	_LIT(KSmsVPFNone, "TSmsFirstOctet::TSmsValidityPeriodFormat::ESmsVPFNone");
	_LIT(KSmsVPFEnhanced, "TSmsFirstOctet::TSmsValidityPeriodFormat::ESmsVPFEnhanced");
	_LIT(KSmsVPFInteger, "TSmsFirstOctet::TSmsValidityPeriodFormat::ESmsVPFInteger");
	_LIT(KSmsVPFSemiOctet, "TSmsFirstOctet::TSmsValidityPeriodFormat::ESmsVPFSemiOctet");
	_LIT(KSmsVPFMask, "TSmsFirstOctet::TSmsValidityPeriodFormat::ESmsVPFMask");
	
	if(value.Compare(KSmsVPFNone) == 0)
		options = TSmsFirstOctet::ESmsVPFNone;
	if(value.Compare(KSmsVPFEnhanced) == 0)
		options = TSmsFirstOctet::ESmsVPFEnhanced;
	if(value.Compare(KSmsVPFInteger) == 0)
		options = TSmsFirstOctet::ESmsVPFInteger;
	if(value.Compare(KSmsVPFSemiOctet) == 0)
		options = TSmsFirstOctet::ESmsVPFSemiOctet;
	if(value.Compare(KSmsVPFMask) == 0)
		options = TSmsFirstOctet::ESmsVPFMask;

	return options;
	}

TBioMsgIdType CSmsScriptUtils::GetBioMsgIdType(TDesC& value)
	{
	
	TBioMsgIdType options;

	_LIT(KBioMsgIdUnknown, "TBioMsgIdType::EBioMsgIdUnknown");
	_LIT(KBioMsgIdIana, "TBioMsgIdType::EBioMsgIdIana");
	_LIT(KBioMsgIdNbs, "TBioMsgIdType::EBioMsgIdNbs");
	_LIT(KBioMsgIdWap, "TBioMsgIdType::EBioMsgIdWap");
	_LIT(KBioMsgIdWapSecure, "TBioMsgIdType::EBioMsgIdWapSecure");
	_LIT(KBioMsgIdWsp, "TBioMsgIdType::EBioMsgIdWsp");
	_LIT(KBioMsgIdWspSecure, "TBioMsgIdType::EBioMsgIdWspSecure");

	if(value.Compare(KBioMsgIdUnknown) == 0)
		options = EBioMsgIdUnknown;
	if(value.Compare(KBioMsgIdIana) == 0)
		options = EBioMsgIdIana;
	if(value.Compare(KBioMsgIdNbs) == 0)
		options = EBioMsgIdNbs;
	if(value.Compare(KBioMsgIdWap) == 0)
		options = EBioMsgIdWap;
	if(value.Compare(KBioMsgIdWapSecure) == 0)
		options = EBioMsgIdWapSecure;
	if(value.Compare(KBioMsgIdWsp) == 0)
		options = EBioMsgIdWsp;
	if(value.Compare(KBioMsgIdWspSecure) == 0)
		options = EBioMsgIdWspSecure;

	return options;
	}

/**
The method reads the CMsvScheduleSettings settings from the configuration file.

@param aSettingsFile		The configuration file name
@param aSmsSettings			A reference to the CSmsSettings object which is to be set from the configuration file.

@internalTechnology
*/
void CSmsScriptUtils::ReadSmsSettingsFromConfigurationFileL(const TDesC& aSettingsFile, CSmsSettings& aSmsSettings)
	{
	CConfigFileParserUtils* scriptFileParser = CConfigFileParserUtils::NewL(aSettingsFile);
	CleanupStack::PushL(scriptFileParser);

	TPtrC stringPtr;
	TInt intValue;

	// Service Centre
	if(scriptFileParser->GetFieldAsString(KScriptItemSC, stringPtr) == KErrNone)
		{
		// get the addresses from the config file
		TLex detailsToParse(stringPtr);
		TPtrC scName;
		TPtrC scNumber;
		
		detailsToParse.SkipSpaceAndMark();
		while(!detailsToParse.Eos())
			{
			scName.Set(_L("Unknown")); // default SC name
			
			while(detailsToParse.Peek() != ';' && !detailsToParse.Eos()) // SCs' details separated by semi-colon
				{
				if(detailsToParse.Peek() == ',')  // name and number separated by comma
					{ // has a name specified
					scName.Set(detailsToParse.MarkedToken());
					detailsToParse.Inc();
					detailsToParse.SkipSpaceAndMark();
					}
				else
					detailsToParse.Inc();
				}
			
			scNumber.Set(detailsToParse.MarkedToken());
			
			// add the address to the SCs list
			aSmsSettings.AddServiceCenterL(scName, scNumber);
		
			detailsToParse.Inc();
			detailsToParse.SkipSpaceAndMark();
			}
		}
	
	// Default Service Centre
	if(scriptFileParser->GetFieldAsInteger(KScriptItemDefaultSC, intValue) == KErrNone)
		{
	
		aSmsSettings.SetDefaultServiceCenter(intValue);
	
		}
		
	// Bearer
	if(scriptFileParser->GetFieldAsString(KScriptItemBearer, stringPtr) == KErrNone)
		{
		CSmsSettings::TMobileSmsBearer smsBearer = GetMobileSmsBearer(stringPtr);
		aSmsSettings.SetSmsBearer(smsBearer);
		}

	// Encoding
	if(scriptFileParser->GetFieldAsString(KScriptItemEncoding, stringPtr) == KErrNone)
		{
		TSmsDataCodingScheme::TSmsAlphabet characterSet = GetSmsAlphabet(stringPtr);
		aSmsSettings.SetCharacterSet(characterSet);
		}
		
	// Delivery Report
	if(scriptFileParser->GetFieldAsString(KScriptItemDeliveryReport, stringPtr) == KErrNone)
		{
		if (stringPtr.CompareF(_L("NO")) == 0)
			{
			aSmsSettings.SetDeliveryReport(EFalse);
			}
		else if (stringPtr.CompareF(_L("YES")) == 0)
			{
			aSmsSettings.SetDeliveryReport(ETrue);
			}
		}
		
	// Reply quoted
	if(scriptFileParser->GetFieldAsString(KScriptItemReplyQuoted, stringPtr) == KErrNone)
		{
		if (stringPtr.CompareF(_L("NO")) == 0)
			{
			aSmsSettings.SetReplyQuoted(EFalse);
			}
		else if (stringPtr.CompareF(_L("YES")) == 0)
			{
			aSmsSettings.SetReplyQuoted(ETrue);
			}
		}
	
	// Description length
	if(scriptFileParser->GetFieldAsInteger(KScriptItemDescriptionLength, intValue) == KErrNone)
		{
		aSmsSettings.SetDescriptionLength(intValue);
		}
		
		
	// Can Concatenate
	if(scriptFileParser->GetFieldAsString(KScriptItemCanConcatenate, stringPtr) == KErrNone)
		{
		if (stringPtr.CompareF(_L("NO")) == 0)
			{
			aSmsSettings.SetCanConcatenate(EFalse);
			}
		else if (stringPtr.CompareF(_L("YES")) == 0)
			{
			aSmsSettings.SetCanConcatenate(ETrue);
			}
		}
	
	// Reply path
	if(scriptFileParser->GetFieldAsString(KScriptItemReplyPath, stringPtr) == KErrNone)
		{
		if (stringPtr.CompareF(_L("NO")) == 0)
			{
			aSmsSettings.SetReplyPath(EFalse);
			}
		else if (stringPtr.CompareF(_L("YES")) == 0)
			{
			aSmsSettings.SetReplyPath(ETrue);
			}
		}
		
	// Reject duplicate
	if(scriptFileParser->GetFieldAsString(KScriptItemRejectDuplicate, stringPtr) == KErrNone)
		{
		if (stringPtr.CompareF(_L("NO")) == 0)
			{
			aSmsSettings.SetRejectDuplicate(EFalse);
			}
		else if (stringPtr.CompareF(_L("YES")) == 0)
			{
			aSmsSettings.SetRejectDuplicate(ETrue);
			}
		}

	// Delivery
	if(scriptFileParser->GetFieldAsString(KScriptItemDelivery, stringPtr) == KErrNone)
		{
		TSmsDelivery delivery = GetSmsDelivery(stringPtr);
		aSmsSettings.SetDelivery(delivery);
		}
	
	// Status Report Handling
	if(scriptFileParser->GetFieldAsString(KScriptItemStatusReportHandling, stringPtr) == KErrNone)
		{
		CSmsSettings::TSmsReportHandling statusReportHandling = GetSmsReportHandling(stringPtr);
		aSmsSettings.SetStatusReportHandling(statusReportHandling);
		}

	// Special Message Handling
	if(scriptFileParser->GetFieldAsString(KScriptItemSpecialMessageHandling, stringPtr) == KErrNone)
		{
		CSmsSettings::TSmsReportHandling specialMessageHandling = GetSmsReportHandling(stringPtr);
		aSmsSettings.SetSpecialMessageHandling(specialMessageHandling);
		}

	// Commdb action
	if(scriptFileParser->GetFieldAsString(KScriptItemSmsCommdbAction, stringPtr) == KErrNone)
		{
		CSmsSettings::TSmsSettingsCommDbAction commdbAction = GetSmsSettingsCommDbAction(stringPtr);
		aSmsSettings.SetCommDbAction(commdbAction);
		}
		
	// Sms Bearer Action
	if(scriptFileParser->GetFieldAsString(KScriptItemSmsBearerAction, stringPtr) == KErrNone)
		{
		CSmsSettings::TSmsSettingsCommDbAction smsBearerAction = GetSmsSettingsCommDbAction(stringPtr);
		aSmsSettings.SetSmsBearerAction(smsBearerAction);
		}

	// Message Conversion
	if(scriptFileParser->GetFieldAsString(KScriptItemSmsMessageConversion, stringPtr) == KErrNone)
		{
		TSmsPIDConversion messageConversion = GetSmsPIDConversion(stringPtr);
		aSmsSettings.SetMessageConversion(messageConversion);
		}
	
	// Class2 Folder
	if(scriptFileParser->GetFieldAsString(KScriptItemClass2Folder, stringPtr) == KErrNone)
		{
		// Try to find a folder with that name
		MDummySessionObserver obser;
		CMsvSession* session = CMsvSession::OpenSyncL(obser);
		CleanupStack::PushL(session);
		
		CMsvEntry* entry = CMsvEntry::NewL(*session, KMsvLocalServiceIndexEntryId, TMsvSelectionOrdering(KMsvGroupByStandardFolders||KMsvGroupByType,EMsvSortByNone));
		CleanupStack::PushL(entry);
		entry->SetEntryL(KMsvLocalServiceIndexEntryId); // Search only the local folders

		TMsvSelectionOrdering order;
		order.SetShowInvisibleEntries(ETrue);
		entry->SetSortTypeL(order);

		CMsvEntrySelection* selection = entry->ChildrenL();
		CleanupStack::PushL(selection);

		TBool found = EFalse;
		TInt count = selection->Count();
		for (TInt i=0; i<count; i++) 
			{	
			entry->SetEntryL((*selection)[i]);
			if ((stringPtr.CompareF(entry->Entry().iDescription) == 0) ||
			    (stringPtr.CompareF(entry->Entry().iDetails) == 0))
				{
				found = ETrue;
				break;
				}
			}

		TMsvId entryId = KMsvGlobalInBoxIndexEntryId;
		if (found)
			entryId = entry->Entry().Id();
		else
			User::Leave(KErrNotFound);

		aSmsSettings.SetClass2Folder(entryId); 
		
		CleanupStack::PopAndDestroy(3, session); // selection, entry, session
		}
	
	CleanupStack::PopAndDestroy(scriptFileParser);
	}

/**
The method reads the CMsvScheduleSettings settings from the configuration file.

@param aSettingsFile		The configuration file name
@param aSmsHeader			A reference to the CSmsHeader object which is to be set from the configuration file.

@internalTechnology
*/
TInt CSmsScriptUtils::SetSmsHeaderInfoFromConfigurationFileL(const TDesC& aSettingsFile, CSmsHeader& aSmsHeader)
	{
	CConfigFileParserUtils* scriptFileParser = CConfigFileParserUtils::NewL(aSettingsFile);
	CleanupStack::PushL(scriptFileParser);

	// Recipient Number: If the recipient number is not present in the file, Leave.
	TPtrC stringPtr;
	User::LeaveIfError(scriptFileParser->GetFieldAsString(KRecipient,stringPtr));

	CSmsNumber* recipientNumber = CSmsNumber::NewL();
	CleanupStack::PushL(recipientNumber);
	recipientNumber->SetAddressL(stringPtr);
	aSmsHeader.Recipients().AppendL(recipientNumber);

	//Bio UId Type
	if((scriptFileParser->GetFieldAsString(KBioUId,stringPtr)) == KErrNone)
		{	
		TBioMsgIdType bioIdType = GetBioMsgIdType(stringPtr);
		aSmsHeader.SetBioMsgIdType(bioIdType);
		}
	else
		{
		aSmsHeader.SetBioMsgIdType(); //Sets the message's BIO message type identifier as EBioMsgIdNbs
		}

	CleanupStack::Pop(2);
	return KErrNone;
	}

/**
The method reads the CMsvScheduleSettings settings from the configuration file.

@param aSettingsFile		The configuration file name
@param aMessageBody			A reference to Msg object which is to be set from the configuration file.

@internalTechnology
*/
TInt CSmsScriptUtils::GetSmsMessageBodyL(const TDesC& aSettingsFile, TPtrC& aMessageBody)
	{
	CConfigFileParserUtils* scriptFileParser = CConfigFileParserUtils::NewL(aSettingsFile);
	CleanupStack::PushL(scriptFileParser);

	// Check if SMS message body is specified in the data file, if not leave
	TInt returnValue;
	returnValue = scriptFileParser->GetFieldAsString(KMessageBody, aMessageBody);
	CleanupStack::Pop(scriptFileParser);

	return returnValue;
	}
