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
// CreateSmtpAccount
// [Paramaters]
// SmtpAccountName			:Name of the Smtp account.
// SmtpConfigFileName		:Config file from which the Smtp account settings are read.
// Creates a SMTP account in the Central Repository with the specified name using the 
// settings read from the config file.
// 
//

//User includes
#include "T_CreateSmtpAccount.h"
#include <t_utilsreademailsettingsfromconfigfile.h>

//Epoc inlcudes
#include <cemailaccounts.h>
#include <iapprefs.h>
#include <smtpset.h>

//Literals used
_LIT(KSmtpAccountName,"SmtpAccountName");
_LIT(KSmtpConfigFileName,"SmtpConfigFileName");

/**
  Function : CT_MsgCreateSmtpAccount
  Description : Constructor
  @param : aSharedDataSMTP Reference to CT_MsgSharedDataSmtp
  @return : N/A
*/
CT_MsgCreateSmtpAccount::CT_MsgCreateSmtpAccount(CT_MsgSharedDataSmtp& aSharedDataSMTP)
:CT_MsgSyncStepSMTP(aSharedDataSMTP)
	{
	SetTestStepName(KCreateSmtpAccount);
	}

/**
  Function : ReadIni
  Description :Reads the corresponding .ini file and returns EPass or EFail if any of the inputs are missing
  @return : TBool
*/
TBool CT_MsgCreateSmtpAccount::ReadIni()
	{
	TBool result = ETrue;
	
	if(!GetStringFromConfig(ConfigSection(), KSmtpAccountName, iSmtpAccountName))
		{
		ERR_PRINTF1(_L("Smtp account name is not specified"));
		SetTestStepResult(EFail);
		result = EFalse;
		}
		
	if(!GetStringFromConfig(ConfigSection(), KSmtpConfigFileName, iConfigFileName))
		{
		ERR_PRINTF1(_L("Configuration file path is not specified"));
		SetTestStepResult(EFail);
		}
		
	return result;
	}

/**
  Function : doTestStepL
  Description : Reads the Smtp account name and the settings file name from the .ini file
				and then creates an Smtp account.
  @return : TVerdict - Test step result
*/
TVerdict CT_MsgCreateSmtpAccount::doTestStepL()
	{
	
	INFO_PRINTF1(_L("Test Step : CreateSmtpAccount"));
	if(ReadIni())
		{
		CEmailAccounts*	emailAccounts = CEmailAccounts::NewLC();
		
		// Creates objects requried for SMTP account
		CImIAPPreferences *smtpIapPreferences = CImIAPPreferences::NewLC();
		CImSmtpSettings *smtpSettings = new (ELeave) CImSmtpSettings;
		CleanupStack::PushL(smtpSettings);

		// Populates the default SMTP settings
		emailAccounts->PopulateDefaultSmtpSettingsL(*smtpSettings, *smtpIapPreferences);

		//If the config file name is provided as input, then reads the SMTP settings from it
		if(iConfigFileName.CompareF( _L("none") ) != 0)
			{
			TRAPD(err, CT_MsgUtilsReadEmailSettingsFromConfigFile::ReadSmtpSettingsFromConfigurationFileL(iConfigFileName, *smtpSettings, *smtpIapPreferences));
			if(err)
				{
				ERR_PRINTF2(_L("Failure while setting the SMTP settings, failed with error %d"), err);
				SetTestStepError(err);
				CleanupStack::PopAndDestroy(3,emailAccounts); // smtpSettings, smtpIapPreferences, emailAccounts
				return TestStepResult();														  
				}
			if(TestStepResult() == EPass)
				{
				// Creates the SMTP account for the name specified and for the settings objects
				TSmtpAccount smtpAccount = emailAccounts->CreateSmtpAccountL(iSmtpAccountName, *smtpSettings,*smtpIapPreferences,EFalse);
				
				// Sets the SMTP account created as the default SMTP account
				emailAccounts->SetDefaultSmtpAccountL(smtpAccount);
				}
			}
		CleanupStack::PopAndDestroy(3,emailAccounts); // smtpSettings, smtpIapPreferences, emailAccounts
		}
	return TestStepResult();
	}
