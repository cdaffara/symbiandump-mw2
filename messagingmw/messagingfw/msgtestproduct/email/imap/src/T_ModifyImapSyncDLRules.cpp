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
// ModifyImapSyncDLRules
// [Paramaters]
// ImapAccountName				:Name of the Imap account.
// NewTotalSizeLimit			:The new maximum size limit for the total message (bytes). [DEFAULT: KMaxTInt]
// NewBodyTextSizeLimit			:The new maximum size limit for body of the message (bytes). [DEFAULT: KMaxTInt]
// NewAttachmentSizeLimit		:The new maximum size limit for the attachments (bytes). [DEFAULT: KMaxTInt]
// NewPartialMailOptions		:The new required message parts using TImImap4PartialMailOptions.
// NewGetMailBodyParts			:The new required message parts using TImap4GetMailOptions.
// ImapBearerTypes				:The bearer type(s) that will be use the download rules for synchronisation.
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
// ImapFolderDLRulesType		 :Type of folder the download rules is for.
// Options available - 
// "Inbox"
// "Other"
// ImapDLRulesPos				 :Position of the download rules in the list. [DEFAULT: 0]
// Modifies the existing sync download rules using the given IMAP account and new rules.
// 
//

//User includes
#include "T_ModifyImapSyncDLRules.h"
#include <t_utilsreademailsettingsfromconfigfile.h>
#include <t_utilsenumconverter.h>
#include <t_utilscentralrepository.h>

//Epoc inlcudes
#include <cemailaccounts.h>
#include <cimapsyncdownloadrules.h>
#include <cdbcols.h>

//Literals used
_LIT(KImapAccountName,"ImapAccountName");
_LIT(KNewTotalSizeLimit,"NewTotalSizeLimit");
_LIT(KNewBodyTextSizeLimit,"NewBodyTextSizeLimit");
_LIT(KNewAttachmentSizeLimit,"NewAttachmentSizeLimit");
_LIT(KNewPartialMailOptions,"NewPartialMailOptions");
_LIT(KNewGetMailBodyParts,"NewGetMailBodyParts");
_LIT(KImapBearerTypes,"ImapBearerTypes");
_LIT(KImapFolderDLRulesType,"ImapFolderDLRulesType");
_LIT(KImapDLRulesPos,"ImapDLRulesPos");

/**
  Function : CT_MsgModifyImapSyncDLRules
  Description : Constructor
  @param : aSharedDataIMAP Reference to CT_MsgSharedDataImap
  @return : N/A
*/
CT_MsgModifyImapSyncDLRules::CT_MsgModifyImapSyncDLRules(CT_MsgSharedDataImap& aSharedDataIMAP)
:CT_MsgSyncStepIMAP(aSharedDataIMAP)
	{
	SetTestStepName(KModifyImapSyncDLRules);
	}

/**
  Function : ReadIni
  Description :Reads the corresponding .ini file and returns EPass or EFail if any of the inputs are missing
  @return : TBool
*/
TBool CT_MsgModifyImapSyncDLRules::ReadIni()
	{
	TBool result = ETrue;
	
	if(!GetStringFromConfig(ConfigSection(), KImapAccountName, iImapAccountName))
		{
		ERR_PRINTF1(_L("Imap account name is not specified"));
		SetTestStepResult(EFail);
		result = EFalse;
		}
		
	if(!GetIntFromConfig(ConfigSection(), KNewTotalSizeLimit, iNewTotalSizeLimit))
		{
		INFO_PRINTF1(_L("The new maximum size limit for the total message [bytes] is not specified, "));
		INFO_PRINTF1(_L("so default value is set: KMaxTInt."));
		iNewTotalSizeLimit = KMaxTInt;
		}
		
	if(!GetIntFromConfig(ConfigSection(), KNewBodyTextSizeLimit, iNewBodyTextSizeLimit))
		{
		INFO_PRINTF1(_L("The new maximum size limit for body of the message [bytes] is not specified, "));
		INFO_PRINTF1(_L("so default value is set: KMaxTInt."));
		iNewBodyTextSizeLimit = KMaxTInt;
		}
	
	if(!GetIntFromConfig(ConfigSection(), KNewAttachmentSizeLimit, iNewAttachmentSizeLimit))
		{
		INFO_PRINTF1(_L("The new maximum size limit for the attachments [bytes] is not specified, "));
		INFO_PRINTF1(_L("so default value is set: KMaxTInt."));
		iNewAttachmentSizeLimit = KMaxTInt;
		}
	
	if(!GetStringFromConfig(ConfigSection(), KNewPartialMailOptions, iNewPartialMailOptions))
		{
		ERR_PRINTF1(_L("The new required message parts using TImImap4PartialMailOptions is not specified."));
		SetTestStepResult(EFail);
		result = EFalse;
		}
		
	if(!GetStringFromConfig(ConfigSection(), KNewGetMailBodyParts, iNewGetMailBodyParts))
		{
		INFO_PRINTF1(_L("The new required message parts using TImap4GetMailOptions is not specified, "));
		INFO_PRINTF1(_L("and no default value is set."));
		}
		
	if(!GetStringFromConfig(ConfigSection(), KImapBearerTypes, iImapBearerTypes))
		{
		ERR_PRINTF1(_L("New Bearer type is not specified - Options available: ")); 
		ERR_PRINTF1(_L("Unknown, CSD, Wcdma, LAN, Virtual, PAN, WLAN, PSD and CSDLAN"));
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
	if(!GetIntFromConfig(ConfigSection(), KImapDLRulesPos, iDLRulesPos))
		{
		INFO_PRINTF1(_L("The position of the download rules in the list is not specified, "));
		INFO_PRINTF1(_L("so default value is set: 0."));
		iDLRulesPos = 0;
		}
	
	return result;
	}
	
/**
  Function : ConvertBearerTypes
  Description :Converts the the bearer type(s) read from the corresponding .ini file as TPtrC into the required
  			   TCommDbBearer enumeration.
  @return : TUint32
*/
TUint32 CT_MsgModifyImapSyncDLRules::ConvertBearerTypes()
	{
	TUint32 bearerTypes;
	if (iImapBearerTypes.CompareF(_L("Unknown")) == 0)
		{
		bearerTypes = KCommDbBearerUnknown;
		}
	else if (iImapBearerTypes.CompareF(_L("CSD")) == 0)
		{
		bearerTypes = KCommDbBearerCSD;
		}
	else if (iImapBearerTypes.CompareF(_L("Wcdma")) == 0)
		{
		bearerTypes = KCommDbBearerWcdma;
		}
	else if (iImapBearerTypes.CompareF(_L("LAN")) == 0)
		{
		bearerTypes = KCommDbBearerLAN;
		}
	else if (iImapBearerTypes.CompareF(_L("Virtual")) == 0)
		{
		bearerTypes = KCommDbBearerVirtual;
		}
	else if (iImapBearerTypes.CompareF(_L("PAN")) == 0)
		{
		bearerTypes = KCommDbBearerPAN;
		}
	else if (iImapBearerTypes.CompareF(_L("WLAN")) == 0)
		{
		bearerTypes = KCommDbBearerWLAN;
		}
	else if (iImapBearerTypes.CompareF(_L("PSD")) == 0)
		{
		bearerTypes = KCommDbBearerPSD;
		}
	else if (iImapBearerTypes.CompareF(_L("CSDLAN")) == 0)
		{
		bearerTypes = KCommDbBearerCSD | KCommDbBearerLAN;
		}
	else
		{
		ERR_PRINTF1(_L("Unknown Bearer type is specified - Options available: ")); 
		ERR_PRINTF1(_L("Unknown, CSD, Wcdma, LAN, Virtual, PAN, WLAN, PSD and CSDLAN"));
		SetTestStepResult(EFail);
		return TestStepResult();
		}
	
	return bearerTypes;
	}

/**
  Function : doTestStepL
  Description : Reads the Imap account name and sync download rules from the .ini file and
				modifies the existing sync download rules with these new rules.
  @return : TVerdict - Test step result
*/
TVerdict CT_MsgModifyImapSyncDLRules::doTestStepL()
	{
	
	INFO_PRINTF1(_L("Test Step : ModifyImapSyncDLRules"));
	if(ReadIni())
		{
		TUint32 bearerTypes = ConvertBearerTypes();
		
		// Retrieves the Imap service Id for the given Imap account
		TMsvId imapServiceId = CT_MsgUtilsCentralRepository::GetImapServiceIdL((TDes&)iImapAccountName);
		INFO_PRINTF2(_L("Imap service id is %d"),imapServiceId);
			
		if(imapServiceId == KMsvNullIndexEntryId)
			{
			ERR_PRINTF1(_L("Invalid IMAP account name specified"));
			SetTestStepResult(EFail);
			return TestStepResult();
			}
		
		CEmailAccounts*	emailAccounts = CEmailAccounts::NewLC();
		CImapSyncDownloadRules* dLRules = CImapSyncDownloadRules::NewLC();
		
		TImapAccount imapAccount;
		emailAccounts->GetImapAccountL(imapServiceId, imapAccount);
		
		// Expected sync download rules
		TImImap4GetPartialMailInfo newMailInfo;
		TImImap4PartialMailOptions partialMailOptions = CT_MsgUtilsEnumConverter::ConvertDesToTImImap4PartialMailOptions(iNewPartialMailOptions);
		
		if (iNewGetMailBodyParts.Length() != 0)
			{
			TImap4GetMailOptions getMailBodyParts = CT_MsgUtilsEnumConverter::ConvertDesToTImap4GetMailOptions(iNewGetMailBodyParts);
			newMailInfo.iGetMailBodyParts = getMailBodyParts;
			}
		newMailInfo.iTotalSizeLimit = iNewTotalSizeLimit;
		newMailInfo.iBodyTextSizeLimit = iNewBodyTextSizeLimit;
		newMailInfo.iAttachmentSizeLimit = iNewAttachmentSizeLimit;
		newMailInfo.iPartialMailOptions = partialMailOptions;
		
		if (iFolderDLRulesType.CompareF(_L("Inbox")) == 0)
			{
			dLRules->AddRuleL(CImapSyncDownloadRules::EInboxRulesType, iDLRulesPos, bearerTypes, newMailInfo);
			}
		else if (iFolderDLRulesType.CompareF(_L("Other")) == 0)
			{
			dLRules->AddRuleL(CImapSyncDownloadRules::EFolderRulesType, iDLRulesPos, bearerTypes, newMailInfo);
			}
		else
			{
			ERR_PRINTF1(_L("Unknown type of folder for the download rules is specified - Options available: ")); 
			ERR_PRINTF1(_L("Inbox and Other"));
			SetTestStepResult(EFail);
			CleanupStack::PopAndDestroy(2,emailAccounts); // dLRules, emailAccounts
			return TestStepResult();
			}
		emailAccounts->SaveImapSyncDownloadRulesL(imapAccount,*dLRules);
		
		CleanupStack::PopAndDestroy(2,emailAccounts); // dLRules, emailAccounts
		}
	return TestStepResult();
	}
