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
// CreatePopAccount
// [Paramaters]
// PopAccountName			: Name of the pop account which needs to be created
// SettingsFileName		: Config file from which the pop account settings are read 
// Creates a POP account in the Central Repository with the specified name and configuration settings file.
// The settings file includes both POP settings,SMTP settings and IAP preference settings for both POP and SMTP.
// [APIs Used]
// CEmailAccounts::CreatePopAccountL
// CEmailAccounts::CreateSmtpAccountL
// 
//

/**
 @file
*/


// User include
#include "T_CreatePopAccount.h"
#include <t_utilsreademailsettingsfromconfigfile.h>


// epoc includes
#include <iapprefs.h>
#include <smtpset.h>
#include <pop3set.h>
#include <cemailaccounts.h>


// Literals used
_LIT(KPopAccountName,"PopAccountName");
_LIT(KPopConfigFileName,"PopConfigFileName");
_LIT(KEmailAdd, "EmailAdd");

/**
CT_MsgCreatePopAccount()
Sets the teststep name to KCreatePopAccount

@param aSharedDataPOP
Data shared across the POP test steps. Ex. Session
*/
CT_MsgCreatePopAccount::CT_MsgCreatePopAccount(CT_MsgSharedDataPop& aSharedDataPOP)
:	CT_MsgSyncStepPOP(aSharedDataPOP) 
	{
	SetTestStepName(KCreatePopAccount);
	}


/**
~CT_MsgCreatePopAccount()
Destructor
*/
CT_MsgCreatePopAccount::~CT_MsgCreatePopAccount()
	{
	}


/**
doTestStepL()
Reads the Pop account name and the settings file name from the ini file.
Creates an Pop account by reading the settings from the given settings file
  
@return
Returns the test step result
*/
TVerdict CT_MsgCreatePopAccount::doTestStepL()
	{
	INFO_PRINTF1(_L("Test Step: Create Pop Account"));
	// Read Pop Account Name from ini file
	TPtrC popAccountName;
	TPtrC configFileName;
	if(!GetStringFromConfig(ConfigSection(), KPopAccountName, popAccountName))
		{
		ERR_PRINTF1(_L("Pop Account name not specified"));
		SetTestStepResult(EFail);
		}
	else
		{
		// Read the configuration file name
		if(!GetStringFromConfig(ConfigSection(), KPopConfigFileName, configFileName))
			{
			ERR_PRINTF1(_L("Configuration file path is not specified"));
			SetTestStepResult(EFail);
			}
		else
			{
			CEmailAccounts *emailAccounts = CEmailAccounts::NewLC();

			// Create objects requried for POP account
			CImIAPPreferences *popIapPreferences = CImIAPPreferences::NewLC();
			
			CImPop3Settings *popSettings = new (ELeave) CImPop3Settings;
			CleanupStack::PushL(popSettings);

			// Create objects requried for SMTP account
			CImIAPPreferences *smtpIapPreferences = CImIAPPreferences::NewLC();
			
			CImSmtpSettings *smtpSettings = new (ELeave) CImSmtpSettings;
			CleanupStack::PushL(smtpSettings);
		
			// Set the default POP and SMTP settings
			emailAccounts->PopulateDefaultPopSettingsL(*popSettings, *popIapPreferences);
			emailAccounts->PopulateDefaultSmtpSettingsL(*smtpSettings, *smtpIapPreferences);

			// If the setting file name is provided as input, then read the Pop and Smtp settings from the config file
			if(configFileName.CompareC(_L("none")) != 0)
				{
			
				TPtrC ptrEmailAdd = KNone();
				if(!GetStringFromConfig(ConfigSection(), KEmailAdd, ptrEmailAdd))
					{
					INFO_PRINTF1(_L("Email Address not specified"));
					}
				
				TRAPD(err, CT_MsgUtilsReadEmailSettingsFromConfigFile::ReadPopSettingsFromConfigurationFileL(configFileName, *popSettings, *popIapPreferences, ptrEmailAdd));
				if(err)
					{	
					ERR_PRINTF2(_L("Failure while setting the POP/SMTP setting parameters, failed with error %d"), err);
					SetTestStepResult(EFail);															  
					}
				else
					{
					TRAPD(error, CT_MsgUtilsReadEmailSettingsFromConfigFile::ReadSmtpSettingsFromConfigurationFileL(configFileName, *smtpSettings, *smtpIapPreferences, ptrEmailAdd));
					if(error)
						{
						ERR_PRINTF2(_L("Failure while setting the SMTP setting parameters, failed with error %d"), error);
						SetTestStepResult(EFail);															  
						}
					}
				}
			if(TestStepResult() == EPass)
				{
				TPopAccount popAccount = emailAccounts->CreatePopAccountL(popAccountName, *popSettings,*popIapPreferences,EFalse);
				TSmtpAccount smtpAccount = emailAccounts->CreateSmtpAccountL(popAccount, *smtpSettings, *smtpIapPreferences, EFalse);
				emailAccounts->SetDefaultSmtpAccountL(smtpAccount);	
				}
			CleanupStack::PopAndDestroy(5,emailAccounts); // smtpSettings, smtpIapPreferences, popSettings, popIapPreferences, emailAccounts  
			}
		}
	return TestStepResult();
	}

