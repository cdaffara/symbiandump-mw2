// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// [TestStep Name]
// CreateImapAccountWithDLRules
// [Paramaters]
// ImapAccountName			:Name of the Imap account.
// ImapConfigFileName		:Config file from which the Imap account settings are read.
// ImapTotalSizeLimit		:The maximum size limit for the total message (bytes). [DEFAULT: KMaxTInt]
// ImapBodyTextSizeLimit	:The maximum size limit for body of the message (bytes). [DEFAULT: KMaxTInt]
// ImapAttachmentSizeLimit	:The maximum size limit for the attachments (bytes). [DEFAULT: KMaxTInt]
// ImapPartialMailOptions	:The required message parts using TImImap4PartialMailOptions.
// ImapGetMailBodyParts		:The required message parts using TImap4GetMailOptions.
// ImapBearerTypes			:The bearer type(s) that will be use the download rules for synchronisation.
// Options available -
// "Unknown": Bearer support unknown.
// "CSD": CSD Bearer support. 
// "Wcdma": WCDMA Bearer support.
// "LAN": LAN Bearer support. 
// "Cdma2000": CDMA2000 Bearer support.
// "Virtual":
// "PAN":
// "WLAN":
// "PSD": Support for both Wcdma and Cdma2000.
// "CSDLAN": Support for both CSD and LAN.
// Creates a IMAP account in the Central Repository with the specified name and sync download
// rules using the IMAP settings read from the config file. (Note: The position the the download rules
// in this test step will be added to the list is 0)
// 
//

//User includes
#include "T_CreateImapAccountWithDLRules.h"
#include <t_utilsreademailsettingsfromconfigfile.h>
#include <t_utilsenumconverter.h>

//Epoc inlcudes
#include <cemailaccounts.h>
#include <iapprefs.h>
#include <imapset.h>
#include <cimapsyncdownloadrules.h>
#include <cdbcols.h>

//Literals used
_LIT(KImapAccountName,"ImapAccountName");
_LIT(KImapConfigFileName,"ImapConfigFileName");
_LIT(KImapTotalSizeLimit,"ImapTotalSizeLimit");
_LIT(KImapBodyTextSizeLimit,"ImapBodyTextSizeLimit");
_LIT(KImapAttachmentSizeLimit,"ImapAttachmentSizeLimit");
_LIT(KImapPartialMailOptions,"ImapPartialMailOptions");
_LIT(KImapGetMailBodyParts,"ImapGetMailBodyParts");
_LIT(KImapBearerTypes,"ImapBearerTypes");

/**
  Function : CT_MsgCreateImapAccountWithDLRules
  Description : Constructor
  @param : aSharedDataIMAP Reference to CT_MsgSharedDataImap
  @return : N/A
*/
CT_MsgCreateImapAccountWithDLRules::CT_MsgCreateImapAccountWithDLRules(CT_MsgSharedDataImap& aSharedDataIMAP)
:CT_MsgSyncStepIMAP(aSharedDataIMAP)
	{
	SetTestStepName(KCreateImapAccountWithDLRules);
	}

/**
  Function : ReadIni
  Description :Reads the corresponding .ini file and returns EPass or EFail if any of the inputs are missing
  @return : TBool
*/
TBool CT_MsgCreateImapAccountWithDLRules::ReadIni()
	{
	TBool result = ETrue;
	
	if(!GetStringFromConfig(ConfigSection(), KImapAccountName, iImapAccountName))
		{
		ERR_PRINTF1(_L("Imap account name is not specified"));
		SetTestStepResult(EFail);
		result = EFalse;
		}
		
	if(!GetStringFromConfig(ConfigSection(), KImapConfigFileName, iConfigFileName))
		{
		ERR_PRINTF1(_L("Configuration file path is not specified"));
		SetTestStepResult(EFail);
		result = EFalse;
		}
	
	if(!GetIntFromConfig(ConfigSection(), KImapTotalSizeLimit, iTotalSizeLimit))
		{
		INFO_PRINTF1(_L("The maximum size limit for the total message [bytes] is not specified, "));
		INFO_PRINTF1(_L("so default value is set: KMaxTInt."));
		iTotalSizeLimit = KMaxTInt;
		}
		
	if(!GetIntFromConfig(ConfigSection(), KImapBodyTextSizeLimit, iBodyTextSizeLimit))
		{
		INFO_PRINTF1(_L("The maximum size limit for body of the message [bytes] is not specified, "));
		INFO_PRINTF1(_L("so default value is set: KMaxTInt."));
		iBodyTextSizeLimit = KMaxTInt;
		}
	
	if(!GetIntFromConfig(ConfigSection(), KImapAttachmentSizeLimit, iAttachmentSizeLimit))
		{
		INFO_PRINTF1(_L("The maximum size limit for the attachments [bytes] is not specified, "));
		INFO_PRINTF1(_L("so default value is set: KMaxTInt."));
		iAttachmentSizeLimit = KMaxTInt;
		}
	
	if(!GetStringFromConfig(ConfigSection(), KImapPartialMailOptions, iPartialMailOptions))
		{
		ERR_PRINTF1(_L("The required message parts using TImImap4PartialMailOptions is not specified."));
		SetTestStepResult(EFail);
		result = EFalse;
		}
		
	if(!GetStringFromConfig(ConfigSection(), KImapGetMailBodyParts, iGetMailBodyParts))
		{
		INFO_PRINTF1(_L("The required message parts using TImap4GetMailOptions is not specified, "));
		INFO_PRINTF1(_L("and no default value is set."));
		}
		
	if(!GetStringFromConfig(ConfigSection(), KImapBearerTypes, iBearerTypes))
		{
		ERR_PRINTF1(_L("Bearer type is not specified - Options available: ")); 
		ERR_PRINTF1(_L("Unknown, CSD, Wcdma, LAN, Virtual, PAN, WLAN, PSD and CSDLAN"));
		SetTestStepResult(EFail);
		result = EFalse;
		}
	
	return result;
	}
	
/**
  Function : ConvertBearerTypes
  Description :Converts the the bearer type(s) read from the corresponding .ini file as TPtrC into the required
  			   TCommDbBearer enumeration.
  @return : TUint32
*/
TUint32 CT_MsgCreateImapAccountWithDLRules::ConvertBearerTypes()
	{
	TUint32 bearerTypes;
	if (iBearerTypes.CompareF(_L("Unknown")) == 0)
		{
		bearerTypes = KCommDbBearerUnknown;
		}
	else if (iBearerTypes.CompareF(_L("CSD")) == 0)
		{
		bearerTypes = KCommDbBearerCSD;
		}
	else if (iBearerTypes.CompareF(_L("Wcdma")) == 0)
		{
		bearerTypes = KCommDbBearerWcdma;
		}
	else if (iBearerTypes.CompareF(_L("LAN")) == 0)
		{
		bearerTypes = KCommDbBearerLAN;
		}
	else if (iBearerTypes.CompareF(_L("Virtual")) == 0)
		{
		bearerTypes = KCommDbBearerVirtual;
		}
	else if (iBearerTypes.CompareF(_L("PAN")) == 0)
		{
		bearerTypes = KCommDbBearerPAN;
		}
	else if (iBearerTypes.CompareF(_L("WLAN")) == 0)
		{
		bearerTypes = KCommDbBearerWLAN;
		}
	else if (iBearerTypes.CompareF(_L("PSD")) == 0)
		{
		bearerTypes = KCommDbBearerPSD;
		}
	else if (iBearerTypes.CompareF(_L("CSDLAN")) == 0)
		{
		bearerTypes = KCommDbBearerCSD | KCommDbBearerLAN;
		}
	else
		{
		ERR_PRINTF1(_L("Unknown Bearer type is specified - Options available:\n")); 
		ERR_PRINTF1(_L("Unknown, CSD, Wcdma, LAN, Virtual, PAN, WLAN, PSD and CSDLAN"));
		SetTestStepResult(EFail);
		return TestStepResult();
		}
	
	return bearerTypes;
	}

/**
  Function : doTestStepL
  Description : Reads the Imap account name and the settings file name from the .ini file
				and creates an Imap account with sync download rules.
  @return : TVerdict - Test step result
*/
TVerdict CT_MsgCreateImapAccountWithDLRules::doTestStepL()
	{
	
	INFO_PRINTF1(_L("Test Step : CreateImapAccountWithDLRules"));
	if(ReadIni())
		{
		TUint32 bearerTypes = ConvertBearerTypes();
		
		CEmailAccounts*	emailAccounts = CEmailAccounts::NewLC();
		
		// Creates objects requried for IMAP account
		CImIAPPreferences* imapPrefs = CImIAPPreferences::NewLC();
		CImImap4Settings* imapSettings = new (ELeave) CImImap4Settings;
		CleanupStack::PushL(imapSettings);

		// Populates the default IMAP settings
		emailAccounts->PopulateDefaultImapSettingsL(*imapSettings, *imapPrefs);
		
		CImapSyncDownloadRules* dLRules = CImapSyncDownloadRules::NewLC();
		
		TImImap4GetPartialMailInfo mailInfo;
		
		TImImap4PartialMailOptions partialMailOptions = CT_MsgUtilsEnumConverter::ConvertDesToTImImap4PartialMailOptions(iPartialMailOptions);
		
		if (iGetMailBodyParts.Length() != 0)
			{
			TImap4GetMailOptions getMailBodyParts = CT_MsgUtilsEnumConverter::ConvertDesToTImap4GetMailOptions(iGetMailBodyParts);
			mailInfo.iGetMailBodyParts = getMailBodyParts;
			}
		
		mailInfo.iTotalSizeLimit = iTotalSizeLimit;
		mailInfo.iBodyTextSizeLimit = iBodyTextSizeLimit;
		mailInfo.iAttachmentSizeLimit = iAttachmentSizeLimit;
		mailInfo.iPartialMailOptions = partialMailOptions;
				
		dLRules->AddRuleL(CImapSyncDownloadRules::EInboxRulesType, 0, bearerTypes, mailInfo);
		dLRules->AddRuleL(CImapSyncDownloadRules::EFolderRulesType, 0, bearerTypes, mailInfo);
		
		//If the config file name is provided as input, then reads the IMAP settings from it
		if(iConfigFileName.CompareF(_L("none")) != 0)
			{
			TRAPD(err, CT_MsgUtilsReadEmailSettingsFromConfigFile::ReadImapSettingsFromConfigurationFileL(iConfigFileName, *imapSettings, *imapPrefs));
			if(err)
				{
				ERR_PRINTF2(_L("Failure while setting the IMAP settings, failed with error %d"), err);
				SetTestStepResult(EFail);
				return TestStepResult();														  
				}
			if(TestStepResult() == EPass)
				{
				// Creates the IMAP account from the given name, download rules and the IMAP settings.
				TImapAccount imapAccount = emailAccounts->CreateImapAccountL(iImapAccountName, *imapSettings,*imapPrefs,*dLRules,EFalse);
				}
			}
		CleanupStack::PopAndDestroy(4,emailAccounts); // dLRules ,imapSettings, imapPrefs, emailAccounts
		}
	return TestStepResult();
	}
