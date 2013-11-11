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
// CheckSmtpBMSetting
// [Paramaters]
// SmtpAccountName			:Name of the Smtp account.
// ExpectedBMSupport		:ETrue => Bearer mobility is expected to be supported by the SMTP service.
// EFalse => Bearer mobility is not expected to be supported by the SMTP service.
// Checks if bearer mobility is supported by the SMTP service.
// 
//

//User includes
#include "T_CheckSmtpBMSetting.h"
#include <t_utils.h>
#include <t_utilsenumconverter.h>
#include <t_utilscentralrepository.h>

//Epoc include
#include <iapprefs.h>

//Literals used
_LIT(KSmtpAccountName,"SmtpAccountName");
_LIT(KExpectedBMSupport,"ExpectedBMSupport");

/**
  Function : CT_MsgCheckSmtpBMSetting
  Description : Constructor 
  @param : aSharedDataSMTP Reference to CT_MsgSharedDataSmtp
  @return : N/A
*/
CT_MsgCheckSmtpBMSetting::CT_MsgCheckSmtpBMSetting(CT_MsgSharedDataSmtp& aSharedDataSMTP)
:CT_MsgSyncStepSMTP(aSharedDataSMTP)
	{
	SetTestStepName(KCheckSmtpBMSetting);
	}
	
/**
  Function : ReadIni
  Description :Reads the corresponding .ini file and returns EPass or EFail if any of the inputs are missing
  @return : TBool
*/
TBool CT_MsgCheckSmtpBMSetting::ReadIni()
	{
	TBool result = ETrue;
	
	if(!GetStringFromConfig(ConfigSection(), KSmtpAccountName, iSmtpAccountName))
		{
		ERR_PRINTF1(_L("Smtp account name is not specified"));
		SetTestStepResult(EFail);
		result = EFalse;
		}
		
	if(!GetBoolFromConfig(ConfigSection(), KExpectedBMSupport, iExpectedBMSupport))
		{
		ERR_PRINTF1(_L("An expected \"ETrue\" or \"EFalse\" boolean value is not specified for BM support"));
		SetTestStepResult(EFail);
		result = EFalse;
		}
	
	return result;
	}

/**
  Function : doTestStepL
  Description : Checks the provisioned BM support for the SMTP account settings of a test case.
  @return : TVerdict - Test step result
*/		
TVerdict CT_MsgCheckSmtpBMSetting::doTestStepL()
	{
	INFO_PRINTF1(_L("Test Step : CheckSmtpBMSetting"));
	if(ReadIni())
		{
		CEmailAccounts* accounts = CEmailAccounts::NewL();
		CleanupStack::PushL(accounts);
		
		CImSmtpSettings* smtpSettings = new(ELeave) CImSmtpSettings();
		CleanupStack::PushL(smtpSettings);
					
		TSmtpAccount smtpAccountId;
		CT_MsgUtilsCentralRepository::GetSmtpAccountL((TDes&)iSmtpAccountName, smtpAccountId);
				
		accounts->LoadSmtpSettingsL(smtpAccountId, *smtpSettings);
		
		TBool actualBMSupport = smtpSettings->BearerMobility();
		
		if (actualBMSupport == iExpectedBMSupport)
			{
			INFO_PRINTF3(_L("Actual Bearer Mobility Support [%d] does equals Expected Bearer Mobility Support [%d]"),actualBMSupport,iExpectedBMSupport);
			}
		else
			{
			ERR_PRINTF3(_L("Actual Bearer Mobility Support [%d] does not equal Expected Bearer Mobility Support [%d]"),actualBMSupport,iExpectedBMSupport);
			SetTestStepResult(EFail);
			}
	
		CleanupStack::PopAndDestroy(2,accounts); // smtpSettings, accounts
		}
	return TestStepResult();
	}
