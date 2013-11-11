// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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
#include <smuthdr.h>
#include <e32std.h>
#include <txtfmlyr.h>
#include <txtrich.h>

#include "CMtfTestActionUtilsSmsScripts.h"
#include "CMtfTestActionUtilsConfigFileParser.h"
#include "CMtfTestCase.h"
#include "TestFrameworkActionsUtils.h"





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

//Last Segment Delivery Report
_LIT(KLastSegmentDeliveryReport, "LastSegmentDeliveryReport");


class MDummySessionObserver : public MMsvSessionObserver
	{
public: 
	void HandleSessionEventL(TMsvSessionEvent,TAny*,TAny*,TAny*) {};
	};



void CMtfTestActionUtilsSmsScripts::ReadSmsSettingsFromConfigurationFileL(CMtfTestCase& aTestCase, const TDesC& aSettingsFile, CSmsSettings& aSmsSettings)
	{
	CMtfTestActionUtilsConfigFileParser* scriptFileParser = CMtfTestActionUtilsConfigFileParser::NewL(aSettingsFile);
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
		CSmsSettings::TMobileSmsBearer smsBearer = ObtainValueParameterL<CSmsSettings::TMobileSmsBearer>(aTestCase, stringPtr);
		aSmsSettings.SetSmsBearer(smsBearer);
		}

	// Encoding
	if(scriptFileParser->GetFieldAsString(KScriptItemEncoding, stringPtr) == KErrNone)
		{
		TSmsDataCodingScheme::TSmsAlphabet characterSet = ObtainValueParameterL<TSmsDataCodingScheme::TSmsAlphabet>(aTestCase, stringPtr);
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
		
	// Validity period, value is read in minutes
	if(scriptFileParser->GetFieldAsInteger(KScriptItemValidityPeriod, intValue) == KErrNone)
		{
		aSmsSettings.SetValidityPeriod(TTimeIntervalMinutes(intValue));
		}
		
	// Delivery
	if(scriptFileParser->GetFieldAsString(KScriptItemDelivery, stringPtr) == KErrNone)
		{
		TSmsDelivery delivery = ObtainValueParameterL<TSmsDelivery>(aTestCase, stringPtr);
		aSmsSettings.SetDelivery(delivery);
		}
	
	// Status Report Handling
	if(scriptFileParser->GetFieldAsString(KScriptItemStatusReportHandling, stringPtr) == KErrNone)
		{
		CSmsSettings::TSmsReportHandling statusReportHandling = ObtainValueParameterL<CSmsSettings::TSmsReportHandling>(aTestCase, stringPtr);
		aSmsSettings.SetStatusReportHandling(statusReportHandling);
		}

	// Special Message Handling
	if(scriptFileParser->GetFieldAsString(KScriptItemSpecialMessageHandling, stringPtr) == KErrNone)
		{
		CSmsSettings::TSmsReportHandling specialMessageHandling = ObtainValueParameterL<CSmsSettings::TSmsReportHandling>(aTestCase, stringPtr);
		aSmsSettings.SetSpecialMessageHandling(specialMessageHandling);
		}

	// Commdb action
	if(scriptFileParser->GetFieldAsString(KScriptItemSmsCommdbAction, stringPtr) == KErrNone)
		{
		CSmsSettings::TSmsSettingsCommDbAction commdbAction = ObtainValueParameterL<CSmsSettings::TSmsSettingsCommDbAction>(aTestCase, stringPtr);
		aSmsSettings.SetCommDbAction(commdbAction);
		}
		
	// Sms Bearer Action
	if(scriptFileParser->GetFieldAsString(KScriptItemSmsBearerAction, stringPtr) == KErrNone)
		{
		CSmsSettings::TSmsSettingsCommDbAction smsBearerAction = ObtainValueParameterL<CSmsSettings::TSmsSettingsCommDbAction>(aTestCase, stringPtr);
		aSmsSettings.SetSmsBearerAction(smsBearerAction);
		}

	// Message Conversion
	if(scriptFileParser->GetFieldAsString(KScriptItemSmsMessageConversion, stringPtr) == KErrNone)
		{
		TSmsPIDConversion messageConversion = ObtainValueParameterL<TSmsPIDConversion>(aTestCase, stringPtr);
		aSmsSettings.SetMessageConversion(messageConversion);
		}
	
	// Vailidity Period Format
	if(scriptFileParser->GetFieldAsString(KScriptItemSmsValidityPeriodFormat, stringPtr) == KErrNone)
		{
		TSmsFirstOctet::TSmsValidityPeriodFormat valPeriodFormat = ObtainValueParameterL<TSmsFirstOctet::TSmsValidityPeriodFormat>(aTestCase, stringPtr);
		aSmsSettings.SetValidityPeriodFormat(valPeriodFormat);
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

	//Last Segment Delivery Report
	if(scriptFileParser->GetFieldAsString(KLastSegmentDeliveryReport, stringPtr) == KErrNone)
		{
		if (stringPtr.CompareF(_L("NO")) == 0)
			{
			aSmsSettings.SetLastSegmentDeliveryReport(EFalse);
			}
		else if (stringPtr.CompareF(_L("YES")) == 0)
			{
			aSmsSettings.SetLastSegmentDeliveryReport(ETrue);
			}
		}
	
	CleanupStack::PopAndDestroy(scriptFileParser);
	}




TInt CMtfTestActionUtilsSmsScripts::SetSmsHeaderInfoFromConfigurationFileL(CMtfTestCase& aTestCase, const TDesC& aSettingsFile, CSmsHeader& aSmsHeader)
	{
	CMtfTestActionUtilsConfigFileParser* scriptFileParser = CMtfTestActionUtilsConfigFileParser::NewL(aSettingsFile);
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
		TBioMsgIdType bioIdType = ObtainValueParameterL<TBioMsgIdType>(aTestCase, stringPtr);
		aSmsHeader.SetBioMsgIdType(bioIdType);
		}
	else
		{
		aSmsHeader.SetBioMsgIdType(); //Sets the message's BIO message type identifier as EBioMsgIdNbs
		}

	CleanupStack::Pop(2);
	return KErrNone;
	}

TInt CMtfTestActionUtilsSmsScripts::GetSmsMessageBodyL(const TDesC& aSettingsFile, TPtrC& aMessageBody)
	{
	CMtfTestActionUtilsConfigFileParser* scriptFileParser = CMtfTestActionUtilsConfigFileParser::NewL(aSettingsFile);
	CleanupStack::PushL(scriptFileParser);

	// Check if SMS message body is specified in the data file, if not leave
	TInt returnValue;
	returnValue = scriptFileParser->GetFieldAsString(KMessageBody, aMessageBody);
	CleanupStack::Pop(scriptFileParser);

	return returnValue;
	}
