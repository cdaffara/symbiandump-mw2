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
// AddImapSyncDLRules
// [Paramaters]
// ImapAccountName			:Name of the Imap account.
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
// ImapFolderDLRulesType	:Type of folder to add the download rules for.
// Options available - 
// "Inbox"
// "Other"
// Adds a new set of sync download rules using the given IMAP account.
// 
//

//User includes
#include "T_AddImapSyncDLRules.h"
#include <t_utilsreademailsettingsfromconfigfile.h>
#include <t_utilsenumconverter.h>
#include <t_utilscentralrepository.h>

//Epoc inlcudes
#include <cemailaccounts.h>
#include <cimapsyncdownloadrules.h>

//Literals used
_LIT(KImapAccountName,"ImapAccountName");
_LIT(KImapTotalSizeLimit,"ImapTotalSizeLimit");
_LIT(KImapBodyTextSizeLimit,"ImapBodyTextSizeLimit");
_LIT(KImapAttachmentSizeLimit,"ImapAttachmentSizeLimit");
_LIT(KImapPartialMailOptions,"ImapPartialMailOptions");
_LIT(KImapGetMailBodyParts,"ImapGetMailBodyParts");
_LIT(KImapFolderDLRulesType,"ImapFolderDLRulesType");
_LIT(KImapBearerTypes,"ImapBearerTypes");

/**
  Function : CT_MsgAddImapSyncDLRules
  Description : Constructor
  @param : aSharedDataIMAP Reference to CT_MsgSharedDataImap
  @return : N/A
*/
CT_MsgAddImapSyncDLRules::CT_MsgAddImapSyncDLRules(CT_MsgSharedDataImap& aSharedDataIMAP)
:CT_MsgSyncStepIMAP(aSharedDataIMAP)
	{
	SetTestStepName(KAddImapSyncDLRules);
	}

/**
  Function : ReadIni
  Description :Reads the corresponding .ini file and returns EPass or EFail if any of the inputs are missing
  @return : TBool
*/
TBool CT_MsgAddImapSyncDLRules::ReadIni()
	{
	TBool result = ETrue;
	
	if(!GetStringFromConfig(ConfigSection(), KImapAccountName, iImapAccountName))
		{
		ERR_PRINTF1(_L("Imap account name is not specified"));
		SetTestStepResult(EFail);
		result = EFalse;
		}
		
	if(!GetStringFromConfig(ConfigSection(), KImapFolderDLRulesType, iFolderDLRulesType))
		{
		ERR_PRINTF1(_L("Type of folder for the download rules is not specified - Options available: ")); 
		ERR_PRINTF1(_L("Inbox and Other"));
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
  Function : AddDLRulesL
  Description :Adds the download rules to the beginning of the list
  @return : void
*/
void CT_MsgAddImapSyncDLRules::AddDLRulesL(CImapSyncDownloadRules& aRules, TUint32 aBearerTypes, TImImap4GetPartialMailInfo& aMailInfo)
	{
	if (iFolderDLRulesType.CompareF(_L("Inbox")) == 0)
		{
		aRules.AddRuleL(CImapSyncDownloadRules::EInboxRulesType, 0, aBearerTypes, aMailInfo);
		}
	else if (iFolderDLRulesType.CompareF(_L("Other")) == 0)
		{
		aRules.AddRuleL(CImapSyncDownloadRules::EFolderRulesType, 0, aBearerTypes, aMailInfo);
		}
	else
		{
		ERR_PRINTF1(_L("Unknown type of folder for the download rules is specified - Options available: ")); 
		ERR_PRINTF1(_L("Inbox and Other"));
		SetTestStepResult(EFail);
		}
	}

/**
  Function : doTestStepL
  Description : Reads the Imap account name and sync download rules from the .ini file
				and then adds these new sync download rules.
  @return : TVerdict - Test step result
*/
TVerdict CT_MsgAddImapSyncDLRules::doTestStepL()
	{
	
	INFO_PRINTF1(_L("Test Step : AddImapSyncDLRules"));
	if(ReadIni())
		{
		// Retrieves the Imap service Id for the given Imap account
		TMsvId imapServiceId = CT_MsgUtilsCentralRepository::GetImapServiceIdL((TDes&)iImapAccountName);
		INFO_PRINTF2(_L("Imap service id is %d"),imapServiceId);
			
		if(imapServiceId == KMsvNullIndexEntryId)
			{
			ERR_PRINTF1(_L("Invalid IMAP account name specified"));
			SetTestStepResult(EFail);
			return TestStepResult();
			}

		TUint32 bearerTypes = CT_MsgUtilsEnumConverter::ConvertDesToBearerTypes(iBearerTypes);

		CEmailAccounts*	emailAccounts = CEmailAccounts::NewLC();		
		CImapSyncDownloadRules* dLRules = CImapSyncDownloadRules::NewLC();
		
		TImapAccount imapAccount;
		emailAccounts->GetImapAccountL(imapServiceId, imapAccount);
		emailAccounts->LoadImapSyncDownloadRulesL(imapAccount, *dLRules);
		
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
		
		AddDLRulesL(*dLRules,bearerTypes,mailInfo);
		
		emailAccounts->SaveImapSyncDownloadRulesL(imapAccount,*dLRules);
		
		CleanupStack::PopAndDestroy(2,emailAccounts); // dLRules, emailAccounts
		}
	return TestStepResult();
	}
