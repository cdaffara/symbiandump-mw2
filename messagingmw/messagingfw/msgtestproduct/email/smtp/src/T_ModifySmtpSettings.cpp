// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// ModifySmtpSettings
// [Paramaters]
// SmtpAccountName			:Name of the Smtp account
// SmtpConfigFileName		:Config file from which the Smtp account settings are read 
// ModifySmtpSettings Test Step is intended to modify existing settings and save to CenRep 
// for the specified SMTP account Id
// 
//

/* user includes */
#include "T_ModifySmtpSettings.h"
#include <t_utilsreademailsettingsfromconfigfile.h>
#include <t_utilscentralrepository.h>

/* epoc inlcudes */
#include <cemailaccounts.h>
#include <smtpset.h>
#include <iapprefs.h>

/* Literals used */
_LIT(KSmtpAccountName,"SmtpAccountName");
_LIT(KSmtpConfigFileName,"SmtpConfigFileName");

/**
  Function : CT_MsgModifySmtpSettings
  Description : Constructor
  @param : aSharedDataSMTP Reference to CT_MsgSharedDataSmtp
  @return : N/A
*/
CT_MsgModifySmtpSettings::CT_MsgModifySmtpSettings(CT_MsgSharedDataSmtp& aSharedDataSMTP)
:	CT_MsgSyncStepSMTP(aSharedDataSMTP)
	{
	SetTestStepName(KModifySmtpSettings);
	}
	
/**
  Function : ~CT_MsgModifySmtpSettings
  Description : Destructor
  @return : N/A
*/
CT_MsgModifySmtpSettings::~CT_MsgModifySmtpSettings()
	{
	}

/**
  Function : doTestStepL
  Description : Reads the Smtp account name and config file name from the .ini file
				and it then modifies the account with the settings read from the config file.
  @return : TVerdict - Test step result
  @leave  : KMsvNullIndexEntryId	Invalid SMTP account name specified	
*/
TVerdict CT_MsgModifySmtpSettings::doTestStepL()
	{
	INFO_PRINTF1(_L("Test Step: ModifySmtpSettings"));
	// Read the SMTP account name from the ini file
	TPtrC smtpAccountName;
	if(!GetStringFromConfig(ConfigSection(), KSmtpAccountName, smtpAccountName))
		{
		ERR_PRINTF1(_L("SMTP account name is not specified"));
		SetTestStepResult(EFail);
		}
	else
		{
		// Read the SMTP Config file name from the ini file
		TPtrC configFileName;
		if(!GetStringFromConfig(ConfigSection(), KSmtpConfigFileName, configFileName))
			{
			ERR_PRINTF1(_L("Configuration file path is not specified"));
			SetTestStepResult(EFail);
			}
		else
			{
			// Retrieving the SMTP service Id for the given SMTP account
			TMsvId smtpServiceId = CT_MsgUtilsCentralRepository::GetSmtpServiceIdL((TDes&)smtpAccountName);
			INFO_PRINTF2(_L("SMTP service Id is %d"),smtpServiceId);
			
			if(smtpServiceId == KMsvNullIndexEntryId)
				{
				ERR_PRINTF1(_L("Invalid SMTP account name specified"));
				SetTestStepResult(EFail);
				}
			// Creates the settings object
			else
				{
				CEmailAccounts* emailAccounts = CEmailAccounts::NewLC();

				CImSmtpSettings* smtpSettings = new(ELeave) CImSmtpSettings();
				CleanupStack::PushL(smtpSettings);
				
				CImIAPPreferences* smtpIapPrefs = CImIAPPreferences::NewLC();

				// Loads the settings for the account with the current settings
				TSmtpAccount smtpAccount;
				emailAccounts->GetSmtpAccountL(smtpServiceId, smtpAccount);
				emailAccounts->LoadSmtpSettingsL(smtpAccount, *smtpSettings);
				emailAccounts->LoadSmtpIapSettingsL(smtpAccount, *smtpIapPrefs);

				// Reads the settings from the config file
				TRAPD(err, CT_MsgUtilsReadEmailSettingsFromConfigFile::ReadSmtpSettingsFromConfigurationFileL(configFileName, *smtpSettings, *smtpIapPrefs));
				if(err)
					{	
					ERR_PRINTF2(_L("Failure while setting the SMTP setting parameters, failed with error %d"), err);
					SetTestStepError(err);															  
					}
				else
					{
					// Saves the new settings
					emailAccounts->SaveSmtpSettingsL(smtpAccount, *smtpSettings);
					emailAccounts->SaveSmtpIapSettingsL(smtpAccount, *smtpIapPrefs);
					}
				CleanupStack::PopAndDestroy(3, emailAccounts);// smtpIapPrefs,smtpSettings,emailAccounts
				}
			}
		}
	return TestStepResult();
	}
