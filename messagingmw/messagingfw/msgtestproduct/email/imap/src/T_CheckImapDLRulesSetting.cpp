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
// CheckImapBMSetting
// [Paramaters]
// ImapAccountName			:Name of the Imap account.
// ExpectedDLRulesUsage		:ETrue => Download rules is expected to be used by the IMAP service.
// EFalse => Download rules is not expected to be used by the IMAP service.
// Checks if download rules are to be used by the IMAP service during account synchronisations.
// 
//

//User includes
#include "T_CheckImapDLRulesSetting.h"
#include <t_utils.h>
#include <t_utilsenumconverter.h>
#include <t_utilscentralrepository.h>

//Epoc include
#include <iapprefs.h>

//Literals used
_LIT(KImapAccountName,"ImapAccountName");
_LIT(KExpectedDLRulesUsage,"ExpectedDLRulesUsage");

/**
  Function : CT_MsgCheckImapDLRulesSetting
  Description : Constructor 
  @param : aSharedDataIMAP Reference to CT_MsgSharedDataImap
  @return : N/A
*/
CT_MsgCheckImapDLRulesSetting::CT_MsgCheckImapDLRulesSetting(CT_MsgSharedDataImap& aSharedDataIMAP)
:CT_MsgSyncStepIMAP(aSharedDataIMAP)
	{
	SetTestStepName(KCheckImapDLRulesSetting);
	}
	
/**
  Function : ReadIni
  Description :Reads the corresponding .ini file and returns EPass or EFail if any of the inputs are missing
  @return : TBool
*/
TBool CT_MsgCheckImapDLRulesSetting::ReadIni()
	{
	TBool result = ETrue;
	
	if(!GetStringFromConfig(ConfigSection(), KImapAccountName, iImapAccountName))
		{
		ERR_PRINTF1(_L("Imap account name is not specified"));
		SetTestStepResult(EFail);
		result = EFalse;
		}
		
	if(!GetBoolFromConfig(ConfigSection(), KExpectedDLRulesUsage, iExpectedDLRulesUsage))
		{
		ERR_PRINTF1(_L("An expected \"ETrue\" or \"EFalse\" boolean value is not specified for DL rules usage"));
		SetTestStepResult(EFail);
		result = EFalse;
		}
	
	return result;
	}

/**
  Function : doTestStepL
  Description : Checks whether to use download rules during account synchronisations
  			    from the IMAP account settings of a test case.
  @return : TVerdict - Test step result
*/		
TVerdict CT_MsgCheckImapDLRulesSetting::doTestStepL()
	{
	INFO_PRINTF1(_L("Test Step : CheckImapDLRulesSetting"));
	if(ReadIni())
		{
		CEmailAccounts* accounts = CEmailAccounts::NewL();
		CleanupStack::PushL(accounts);
		
		CImImap4Settings* imapSettings = new(ELeave) CImImap4Settings();
		CleanupStack::PushL(imapSettings);
					
		TImapAccount imapAccountId;
		CT_MsgUtilsCentralRepository::GetImapAccountL((TDes&)iImapAccountName,imapAccountId);
				
		accounts->LoadImapSettingsL(imapAccountId, *imapSettings);
		
		TBool actualDLRulesUsage = imapSettings->UseSyncDownloadRules();
		
		if (actualDLRulesUsage == iExpectedDLRulesUsage)
			{
			INFO_PRINTF3(_L("Actual Download Rules Usage [%d] does equals Expected Download Rules Usage [%d]"),actualDLRulesUsage,iExpectedDLRulesUsage);
			}
		else
			{
			ERR_PRINTF3(_L("Actual Download Rules Usage [%d] does not equal Expected Download Rules Usage [%d]"),actualDLRulesUsage,iExpectedDLRulesUsage);
			SetTestStepResult(EFail);
			}
	
		CleanupStack::PopAndDestroy(2,accounts); // imapSettings, accounts
		}
	return TestStepResult();
	}

