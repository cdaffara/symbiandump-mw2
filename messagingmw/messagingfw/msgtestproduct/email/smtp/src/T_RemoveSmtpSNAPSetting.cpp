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
// RemoveSmtpSNAPSetting
// [Paramaters]
// SmtpAccountName			:Name of the Smtp account.
// Removes the provisioned SNAP information for the SMTP account.
// 
//

//User includes
#include "T_RemoveSmtpSNAPSetting.h"
#include <t_utils.h>
#include <t_utilsenumconverter.h>
#include <t_utilscentralrepository.h>

//Epoc include
#include <iapprefs.h>

//Literal used
_LIT(KSmtpAccountName,"SmtpAccountName");

/**
  Function : CT_MsgRemoveSmtpSNAPSetting
  Description : Constructor 
  @param : aSharedDataSMTP Reference to CT_MsgSharedDataSmtp
  @return : N/A
*/
CT_MsgRemoveSmtpSNAPSetting::CT_MsgRemoveSmtpSNAPSetting(CT_MsgSharedDataSmtp& aSharedDataSMTP)
:CT_MsgSyncStepSMTP(aSharedDataSMTP)
	{
	SetTestStepName(KRemoveSmtpSNAPSetting);
	}
	
/**
  Function : ReadIni
  Description :Reads the corresponding .ini file and returns EPass or EFail if any of the inputs are missing
  @return : TBool
*/
TBool CT_MsgRemoveSmtpSNAPSetting::ReadIni()
	{
	TBool result = ETrue;
	
	if(!GetStringFromConfig(ConfigSection(), KSmtpAccountName, iSmtpAccountName))
		{
		ERR_PRINTF1(_L("Smtp account name is not specified"));
		SetTestStepResult(EFail);
		result = EFalse;
		}
		
	return result;
	}

/**
  Function : doTestStepL
  Description : Removes the provisioned SNAP information from the SMTP account settings of a test case.
  @return : TVerdict - Test step result
*/		
TVerdict CT_MsgRemoveSmtpSNAPSetting::doTestStepL()
	{
	INFO_PRINTF1(_L("Test Step : RemoveSmtpSNAPSetting"));
	if(ReadIni())
		{
		CEmailAccounts* accounts = CEmailAccounts::NewL();
		CleanupStack::PushL(accounts);
		
		CImIAPPreferences* smtpIapPrefs = CImIAPPreferences::NewLC();
					
		TSmtpAccount smtpAccountId;
		CT_MsgUtilsCentralRepository::GetSmtpAccountL((TDes&)iSmtpAccountName, smtpAccountId);
				
		accounts->LoadSmtpIapSettingsL(smtpAccountId, *smtpIapPrefs);	
		
		smtpIapPrefs->RemoveSNAP();
		
		accounts->SaveSmtpIapSettingsL(smtpAccountId, *smtpIapPrefs);
	
		CleanupStack::PopAndDestroy(2,accounts); // smtpIapPrefs, accounts
		}
	return TestStepResult();
	}
