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
// CheckImapSyncDLRules
// [Paramaters]
// ImapAccountName				:Name of the Imap account.
// ExpectedTotalSizeLimit		:The expected maximum size limit for the total message (bytes). [DEFAULT: KMaxTInt]
// ExpectedBodyTextSizeLimit	:The expected maximum size limit for body of the message (bytes). [DEFAULT: KMaxTInt]
// ExpectedAttachmentSizeLimit	:The expected maximum size limit for the attachments (bytes). [DEFAULT: KMaxTInt]
// ExpectedPartialMailOptions	:The expected required message parts using TImImap4PartialMailOptions.
// ExpectedGetMailBodyParts		:The expected required message parts using TImap4GetMailOptions.
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
// ImapFolderDLRulesType		:Type of folder the download rules is for.
// Options available - 
// "Inbox"
// "Other"
// Check the loaded sync download rules using the given IMAP account against the expected rules.
// 
//

//User includes
#include "T_CheckImapSyncDLRules.h"
#include <t_utilsreademailsettingsfromconfigfile.h>
#include <t_utilsenumconverter.h>
#include <t_utilscentralrepository.h>

//Epoc inlcudes
#include <cemailaccounts.h>
#include <cimapsyncdownloadrules.h>
#include <cdbcols.h>

//Literals used
_LIT(KImapAccountName,"ImapAccountName");
_LIT(KExpectedTotalSizeLimit,"ExpectedTotalSizeLimit");
_LIT(KExpectedBodyTextSizeLimit,"ExpectedBodyTextSizeLimit");
_LIT(KExpectedAttachmentSizeLimit,"ExpectedAttachmentSizeLimit");
_LIT(KExpectedPartialMailOptions,"ExpectedPartialMailOptions");
_LIT(KExpectedGetMailBodyParts,"ExpectedGetMailBodyParts");
_LIT(KImapBearerTypes,"ImapBearerTypes");
_LIT(KImapFolderDLRulesType,"ImapFolderDLRulesType");

/**
  Function : CT_MsgCheckImapSyncDLRules
  Description : Constructor
  @param : aSharedDataIMAP Reference to CT_MsgSharedDataImap
  @return : N/A
*/
CT_MsgCheckImapSyncDLRules::CT_MsgCheckImapSyncDLRules(CT_MsgSharedDataImap& aSharedDataIMAP)
:CT_MsgSyncStepIMAP(aSharedDataIMAP)
	{
	SetTestStepName(KCheckImapSyncDLRules);
	}

/**
  Function : ReadIni
  Description : Reads the corresponding .ini file and returns EPass or EFail if any of the inputs are missing
  @return : TBool
*/
TBool CT_MsgCheckImapSyncDLRules::ReadIni()
	{
	TBool result = ETrue;
	
	if(!GetStringFromConfig(ConfigSection(), KImapAccountName, iImapAccountName))
		{
		ERR_PRINTF1(_L("Imap account name is not specified"));
		SetTestStepResult(EFail);
		result = EFalse;
		}
		
	if(!GetIntFromConfig(ConfigSection(), KExpectedTotalSizeLimit, iExpectedTotalSizeLimit))
		{
		INFO_PRINTF1(_L("The expected maximum size limit for the total message [bytes] is not specified, "));
		INFO_PRINTF1(_L("so default value is set: KMaxTInt."));
		iExpectedTotalSizeLimit = KMaxTInt;
		}
		
	if(!GetIntFromConfig(ConfigSection(), KExpectedBodyTextSizeLimit, iExpectedBodyTextSizeLimit))
		{
		INFO_PRINTF1(_L("The expected maximum size limit for body of the message [bytes] is not specified, "));
		INFO_PRINTF1(_L("so default value is set: KMaxTInt."));
		iExpectedBodyTextSizeLimit = KMaxTInt;
		}
	
	if(!GetIntFromConfig(ConfigSection(), KExpectedAttachmentSizeLimit, iExpectedAttachmentSizeLimit))
		{
		INFO_PRINTF1(_L("The expected maximum size limit for the attachments [bytes] is not specified, "));
		INFO_PRINTF1(_L("so default value is set: KMaxTInt."));
		iExpectedAttachmentSizeLimit = KMaxTInt;
		}
	
	if(!GetStringFromConfig(ConfigSection(), KExpectedPartialMailOptions, iExpectedPartialMailOptions))
		{
		ERR_PRINTF1(_L("The expected required message parts using TImImap4PartialMailOptions is not specified."));
		SetTestStepResult(EFail);
		result = EFalse;
		}
		
	if(!GetStringFromConfig(ConfigSection(), KExpectedGetMailBodyParts, iExpectedGetMailBodyParts))
		{
		INFO_PRINTF1(_L("The expected required message parts using TImap4GetMailOptions is not specified, "));
		INFO_PRINTF1(_L("and no default value is set."));
		}
		
	if(!GetStringFromConfig(ConfigSection(), KImapBearerTypes, iImapBearerTypes))
		{
		ERR_PRINTF1(_L("Bearer type is not specified - Options available: ")); 
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
	
	return result;
	}
	
/**
  Function : ConvertBearerTypes
  Description :Converts the the bearer type(s) read from the corresponding .ini file as TPtrC into the required
  			   TCommDbBearer enumeration.
  @return : TUint32
*/
TUint32 CT_MsgCheckImapSyncDLRules::ConvertBearerTypes()
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
  Function : CompareDLRules
  Description :Compares the expected sync download rules against the loaded sync download rules for a particular
  			   type of folder and bearer type(s) in the list.
  @return : TBool
*/
TBool CT_MsgCheckImapSyncDLRules::CompareDLRules(TImImap4GetPartialMailInfo& aExpectedMailInfo, TImImap4GetPartialMailInfo& aLoadedMailInfo)
	{
	if ((aExpectedMailInfo.iTotalSizeLimit == aLoadedMailInfo.iTotalSizeLimit) &&
		(aExpectedMailInfo.iBodyTextSizeLimit == aLoadedMailInfo.iBodyTextSizeLimit) &&
		(aExpectedMailInfo.iAttachmentSizeLimit == aLoadedMailInfo.iAttachmentSizeLimit) &&
		(aExpectedMailInfo.iPartialMailOptions == aLoadedMailInfo.iPartialMailOptions))
		{
		if (iExpectedGetMailBodyParts.Length() != 0)
			{
			if (aExpectedMailInfo.iGetMailBodyParts == aLoadedMailInfo.iGetMailBodyParts)
				{
				return ETrue;
				}
			else
				{
				return EFalse;
				}
			}
		return ETrue;	
		}
	else
		{
		return EFalse;
		}
	}

/**
  Function : doTestStepL
  Description : Reads the Imap account name and sync download rules from the .ini file.
				Check the loaded sync download rules against the expected rules from the .ini file.
  @return : TVerdict - Test step result
*/
TVerdict CT_MsgCheckImapSyncDLRules::doTestStepL()
	{
	
	INFO_PRINTF1(_L("Test Step : CheckImapSyncDLRules"));
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
		emailAccounts->LoadImapSyncDownloadRulesL(imapAccount, *dLRules);
		
		// Loaded sync download rules
		TImImap4GetPartialMailInfo loadedMailInfo;
		TInt pos;
		if (iFolderDLRulesType.CompareF(_L("Inbox")) == 0)
			{
			pos = dLRules->GetMailInfoL(CImapSyncDownloadRules::EInboxRulesType, bearerTypes, loadedMailInfo);
			}
		else if (iFolderDLRulesType.CompareF(_L("Other")) == 0)
			{
			pos = dLRules->GetMailInfoL(CImapSyncDownloadRules::EFolderRulesType, bearerTypes, loadedMailInfo);
			}
		else
			{
			ERR_PRINTF1(_L("Unknown type of folder for the download rules is specified - Options available: ")); 
			ERR_PRINTF1(_L("Inbox and Other"));
			SetTestStepResult(EFail);
			CleanupStack::PopAndDestroy(2,emailAccounts); // dLRules, emailAccounts
			return TestStepResult();
			}
		
		if (pos == KErrNotFound)
			{
			SetTestStepError(pos);
			CleanupStack::PopAndDestroy(2,emailAccounts); // dLRules, emailAccounts
			return TestStepResult();
			}		
		
		// Expected sync download rules
		TImImap4GetPartialMailInfo expectedMailInfo;
		TImImap4PartialMailOptions partialMailOptions = CT_MsgUtilsEnumConverter::ConvertDesToTImImap4PartialMailOptions(iExpectedPartialMailOptions);
		
		if (iExpectedGetMailBodyParts.Length() != 0)
			{
			TImap4GetMailOptions getMailBodyParts = CT_MsgUtilsEnumConverter::ConvertDesToTImap4GetMailOptions(iExpectedGetMailBodyParts);
			expectedMailInfo.iGetMailBodyParts = getMailBodyParts;
			}
		expectedMailInfo.iTotalSizeLimit = iExpectedTotalSizeLimit;
		expectedMailInfo.iBodyTextSizeLimit = iExpectedBodyTextSizeLimit;
		expectedMailInfo.iAttachmentSizeLimit = iExpectedAttachmentSizeLimit;
		expectedMailInfo.iPartialMailOptions = partialMailOptions;
		
		if(!CompareDLRules(expectedMailInfo,loadedMailInfo))
			{
			SetTestStepResult(EFail);
			}
		
		CleanupStack::PopAndDestroy(2,emailAccounts); // dLRules, emailAccounts
		}
	return TestStepResult();
	}
