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
// CheckSmtpSNAPSetting
// [Paramaters]
// SmtpAccountName			:Name of the Smtp account.
// ExpectedSNAPDefinition	:ETrue => A SNAP has expected to been provisioned for the SMTP account.
// EFalse => A SNAP has not expected to been provisioned for the SMTP account.
// ExpectedSNAPPreference	:Expected SNAP ID (Assuming the SNAP has been defined for the SMTP a/c)
// This parameter can be ignored if the "ExpectedSNAPDefinition" is EFalse.
// Checks the provisioned SNAP information for the SMTP account.
// 
//

//User includes
#include "T_CheckSmtpSNAPSetting.h"
#include <t_utils.h>
#include <t_utilsenumconverter.h>
#include <t_utilscentralrepository.h>

//Epoc include
#include <iapprefs.h>

//Literals used
_LIT(KSmtpAccountName,"SmtpAccountName");
_LIT(KSNAPDefined,"ExpectedSNAPDefinition");
_LIT(KSNAPPreference,"ExpectedSNAPPreference");

/**
  Function : CT_MsgCheckSmtpSNAPSetting
  Description : Constructor 
  @param : aSharedDataSMTP Reference to CT_MsgSharedDataSmtp
  @return : N/A
*/
CT_MsgCheckSmtpSNAPSetting::CT_MsgCheckSmtpSNAPSetting(CT_MsgSharedDataSmtp& aSharedDataSMTP)
:CT_MsgSyncStepSMTP(aSharedDataSMTP)
	{
	SetTestStepName(KCheckSmtpSNAPSetting);
	}
	
/**
  Function : ReadIni
  Description :Reads the corresponding .ini file and returns EPass or EFail if any of the inputs are missing
  @return : TBool
*/
TBool CT_MsgCheckSmtpSNAPSetting::ReadIni()
	{
	TBool result = ETrue;
	
	if(!GetStringFromConfig(ConfigSection(), KSmtpAccountName, iSmtpAccountName))
		{
		ERR_PRINTF1(_L("Smtp account name is not specified"));
		SetTestStepResult(EFail);
		result = EFalse;
		}
		
	if(!GetBoolFromConfig(ConfigSection(), KSNAPDefined, iExpectedSNAPDefinition))
		{
		ERR_PRINTF1(_L("An expected \"ETrue\" or \"EFalse\" boolean value is not specified for the SNAP definition"));
		SetTestStepResult(EFail);
		result = EFalse;
		}
	
	return result;
	}

/**
  Function : doTestStepL
  Description : Checks the provisioned SNAP information for the SMTP account settings of a test case.
  @return : TVerdict - Test step result
*/		
TVerdict CT_MsgCheckSmtpSNAPSetting::doTestStepL()
	{
	INFO_PRINTF1(_L("Test Step : CheckSmtpSNAPSetting"));
	if(ReadIni())
		{
		CEmailAccounts* accounts = CEmailAccounts::NewL();
		CleanupStack::PushL(accounts);
		
		CImIAPPreferences* smtpIapPrefs = CImIAPPreferences::NewLC();
					
		TSmtpAccount smtpAccountId;
		CT_MsgUtilsCentralRepository::GetSmtpAccountL((TDes&)iSmtpAccountName,smtpAccountId);
				
		accounts->LoadSmtpIapSettingsL(smtpAccountId, *smtpIapPrefs);	
		
		TBool actualSNAPDefintion = smtpIapPrefs->SNAPDefined();
		
		if (actualSNAPDefintion == iExpectedSNAPDefinition)
			{
			if (actualSNAPDefintion)
				{
				TInt expectedSNAPPreference = 0;
				if(!GetIntFromConfig(ConfigSection(), KSNAPPreference, expectedSNAPPreference))
					{
					ERR_PRINTF1(_L("An expected SNAP ID value is not specified"));
					SetTestStepResult(EFail);
					CleanupStack::PopAndDestroy(2,accounts); // smtpIapPrefs, accounts
					return TestStepResult();
					}
				TInt actualSNAPPreference = smtpIapPrefs->SNAPPreference();
				
				if (actualSNAPPreference != expectedSNAPPreference)
					{
					ERR_PRINTF3(_L("Actual SNAP ID [%d] does not equal Expected SNAP ID [%d]"),actualSNAPPreference,expectedSNAPPreference);
					SetTestStepResult(EFail);
					}
				else
					{
					INFO_PRINTF3(_L("Actual SNAP ID [%d] equals Expected SNAP ID [%d]"),actualSNAPPreference,expectedSNAPPreference);
					}
				}
			}
		else
			{
			ERR_PRINTF3(_L("Actual SNAP Defintion [%d] does not equal Expected SNAP Defintion [%d]"),actualSNAPDefintion,iExpectedSNAPDefinition);
			SetTestStepResult(EFail);
			}
	
		CleanupStack::PopAndDestroy(2,accounts); // smtpIapPrefs, accounts
		}
	return TestStepResult();
	}
