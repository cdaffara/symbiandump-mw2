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
// ModifyPopSettings
// [Paramaters]
// PopAccountName		:		Name of the Pop account
// ConfigFileName		:		Config file from which the Pop account settings are read 
// [Test Step Description]
// ModifyPopSettings Test Action is intended to modify and save the existing settings to CenRep 
// for the specified Pop account Id
// [APIs Used]	
// CEmailAccounts::ModifyPopSettingsL
// CImPop3Settings::SetServerAddressL
// CImPop3Settings::SetUserAddressL
// CImPop3Settings::SetLoginNameL
// CImPop3Settings::SetPasswordL
// CImPop3Settings::SetPort
// CImPop3Settings::SetSecureSockets
// CImPop3Settings::SetVersion
// CImPop3Settings::SetFolderPathL
// CImPop3Settings::SetPathSeparator
// CImPop3Settings::SetMaxEmailSize
// CImPop3Settings::SetGetMailOptions
// CImPop3Settings::SetInboxSynchronisationLimit
// CImPop3Settings::SetMailboxSynchronisationLimit
// CImSmtpSettings::SetServerAddressL
// CImSmtpSettings::SetEmailAddressL
// CImSmtpSettings::SetBodyEncoding
// CImSmtpSettings::SetReceiptAddressL
// CImSmtpSettings::SetRequestReceipts
// CImSmtpSettings::SetSendMessageOption
// CImSmtpSettings::SetPort
// CImSmtpSettings::SetEmailAliasL
// CImSmtpSettings::SetBodyEncoding
// CImSmtpSettings::SetDefaultMsgCharSet
// CImSmtpSettings::SetUserAddress
// CImSmtpSettings::SetVersion
// CImIAPPreferences::Version
// CImIAPPreferences::AddIAPL 
// 
//

/**
 @file
*/



// User include
#include "T_ModifyPopSettings.h"
#include <t_utilsreademailsettingsfromconfigfile.h>
#include <t_utilscentralrepository.h>



// epoc inlcudes
#include <cemailaccounts.h>
#include <pop3set.h>
#include <iapprefs.h>
#include <smtpset.h>



// Literals used
_LIT(KPopAccountName,"PopAccountName");
_LIT(KPopConfigFileName,"PopConfigFileName");


/**
CT_MsgModifyPopSettings()
Sets the teststep name to KModifyPopSettings

@param aSharedDataPOP
Data shared across the POP test steps. Ex. Session
*/
CT_MsgModifyPopSettings::CT_MsgModifyPopSettings(CT_MsgSharedDataPop& aSharedDataPOP)
:	CT_MsgSyncStepPOP(aSharedDataPOP) 
	{
	SetTestStepName(KModifyPopSettings);
	}


/**
~CT_MsgModifyPopSettings()
Destructor
*/
CT_MsgModifyPopSettings::~CT_MsgModifyPopSettings()
	{
	}


/**
doTestStepL()
Reads the Pop account name, config file name from the ini file.
It then modifies the account settings with the settings read from the config file

@return
Returns the test step result
*/
TVerdict CT_MsgModifyPopSettings::doTestStepL()
	{
	INFO_PRINTF1(_L("Test Step: ModifyPopSettings"));
	// Read the Pop account name from the ini file
	TPtrC popAccountName;
	if(!GetStringFromConfig(ConfigSection(), KPopAccountName, popAccountName))
		{
		ERR_PRINTF1(_L("PopAccount Name is not specified"));
		SetTestStepResult(EFail);
		}
	else
		{
		TPtrC configFileName;
		if(!GetStringFromConfig(ConfigSection(), KPopConfigFileName, configFileName))
			{
			ERR_PRINTF1(_L("Configuration file path is not specified"));
			SetTestStepResult(EFail);
			}
		else
			{
			// Retrieving the Pop service Id for the given Pop account
			TMsvId popServiceId = CT_MsgUtilsCentralRepository::GetPopServiceIdL((TDes&)popAccountName);
			INFO_PRINTF2(_L("Pop service id is %d"),popServiceId);
			
			if(popServiceId == KMsvNullIndexEntryId)
				{
				ERR_PRINTF1(_L("Invalid POP account name specified"));
				SetTestStepResult(EFail);
				}
			else
				{
				CEmailAccounts* emailAccounts = CEmailAccounts::NewLC();

				CImPop3Settings* popSettings = new(ELeave) CImPop3Settings();
				CleanupStack::PushL(popSettings);
				
				CImIAPPreferences* popIapPrefs = CImIAPPreferences::NewLC();
				
				CImSmtpSettings* smtpSettings = new(ELeave) CImSmtpSettings();
				CleanupStack::PushL(smtpSettings);

				CImIAPPreferences* smtpIapPrefs = CImIAPPreferences::NewLC();				
				
				TPopAccount popAccount;
				emailAccounts->GetPopAccountL(popServiceId, popAccount);
				
				emailAccounts->PopulateDefaultPopSettingsL(*popSettings, *popIapPrefs);
				emailAccounts->PopulateDefaultSmtpSettingsL(*smtpSettings, *smtpIapPrefs);

				_LIT(KEmailAdd, "EmailAdd");
				
				TPtrC ptrEmailAdd;
				if(!GetStringFromConfig(ConfigSection(), KEmailAdd, ptrEmailAdd))
					{
					ERR_PRINTF1(_L("Email Address not specified"));
					}
				
				TRAPD(err, CT_MsgUtilsReadEmailSettingsFromConfigFile::ReadPopSettingsFromConfigurationFileL(configFileName, *popSettings, *popIapPrefs, ptrEmailAdd));
				if(err)
					{	
					ERR_PRINTF2(_L("Failure while setting the Pop setting parameters, failed with error %d"), err);
					SetTestStepResult(EFail);															  
					}
				else
					{
					TRAPD(error, CT_MsgUtilsReadEmailSettingsFromConfigFile::ReadSmtpSettingsFromConfigurationFileL(configFileName, *smtpSettings, *smtpIapPrefs, ptrEmailAdd));
					if(error)
						{
						ERR_PRINTF2(_L("Failure while setting the SMTP setting parameters, failed with error %d"), error);
						SetTestStepResult(EFail);															  
						}
					else
						{												
						// Save the new settings
						emailAccounts->SavePopSettingsL(popAccount, *popSettings);
						emailAccounts->SavePopIapSettingsL(popAccount, *popIapPrefs);
					
						TSmtpAccount smtpAccount;
						emailAccounts->GetSmtpAccountL(popAccount.iSmtpService, smtpAccount);											
				
						emailAccounts->SaveSmtpSettingsL(smtpAccount, *smtpSettings);
						emailAccounts->SaveSmtpIapSettingsL(smtpAccount, *smtpIapPrefs);
						}
					}
				CleanupStack::PopAndDestroy(5, emailAccounts); //smtpIapPrefs,smtpSettings,popIapPrefs,popSettings,emailAccounts
				}
			}
		}
	return TestStepResult();
	}
	
	
