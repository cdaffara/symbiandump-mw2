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
// DeleteImapSyncDLRules
// [Paramaters]
// ImapAccountName				:Name of the Imap account.
// ImapFolderDLRulesType		:Type of folder to delete the download rules for.
// Options available - 
// "Inbox"
// "Other"
// Deletes the existing sync download rules using the given IMAP account.
// 
//

//User includes
#include "T_DeleteImapSyncDLRules.h"
#include <t_utilsreademailsettingsfromconfigfile.h>
#include <t_utilsenumconverter.h>
#include <t_utilscentralrepository.h>

//Epoc inlcudes
#include <cemailaccounts.h>
#include <cimapsyncdownloadrules.h>

//Literals used
_LIT(KImapAccountName,"ImapAccountName");
_LIT(KImapFolderDLRulesType,"ImapFolderDLRulesType");

/**
  Function : CT_MsgDeleteImapSyncDLRules
  Description : Constructor
  @param : aSharedDataIMAP Reference to CT_MsgSharedDataImap
  @return : N/A
*/
CT_MsgDeleteImapSyncDLRules::CT_MsgDeleteImapSyncDLRules(CT_MsgSharedDataImap& aSharedDataIMAP)
:CT_MsgSyncStepIMAP(aSharedDataIMAP)
	{
	SetTestStepName(KDeleteImapSyncDLRules);
	}

/**
  Function : ReadIni
  Description :Reads the corresponding .ini file and returns EPass or EFail if any of the inputs are missing
  @return : TBool
*/
TBool CT_MsgDeleteImapSyncDLRules::ReadIni()
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
	
	return result;
	}

/**
  Function : DeleteDLRulesL
  Description :Deletes all the download rules in the list
  @return : void
*/
void CT_MsgDeleteImapSyncDLRules::DeleteDLRulesL(CImapSyncDownloadRules& aRules)
	{
	TBool deleted = ETrue;
	if (iFolderDLRulesType.CompareF(_L("Inbox")) == 0)
		{
		do
			{
			deleted = aRules.DeleteRuleL(CImapSyncDownloadRules::EInboxRulesType, 0);
			} 
		while (deleted);
		}
	else if (iFolderDLRulesType.CompareF(_L("Other")) == 0)
		{
		do
			{
			deleted = aRules.DeleteRuleL(CImapSyncDownloadRules::EFolderRulesType, 0);
			}
		while (deleted);
		
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
  Description : Reads the Imap account name and type of folder to delete the download rules for from .ini file
				and then deletes the existing sync download rules.
  @return : TVerdict - Test step result
*/
TVerdict CT_MsgDeleteImapSyncDLRules::doTestStepL()
	{
	
	INFO_PRINTF1(_L("Test Step : DeleteImapSyncDLRules"));
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
		
		CEmailAccounts*	emailAccounts = CEmailAccounts::NewLC();		
		CImapSyncDownloadRules* dLRules = CImapSyncDownloadRules::NewLC();
		
		TImapAccount imapAccount;
		emailAccounts->GetImapAccountL(imapServiceId, imapAccount);
		emailAccounts->LoadImapSyncDownloadRulesL(imapAccount, *dLRules);
		
		DeleteDLRulesL(*dLRules);
		
		emailAccounts->SaveImapSyncDownloadRulesL(imapAccount,*dLRules);
		
		CleanupStack::PopAndDestroy(2,emailAccounts); // dLRules, emailAccounts
		}
	return TestStepResult();
	}
