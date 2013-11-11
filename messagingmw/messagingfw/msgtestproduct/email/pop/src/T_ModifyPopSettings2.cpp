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
// ModifyPopSettings2
// [Paramaters]
// PopAccountName			:Name of the Pop account
// PopConfigFileName		:Config file from which the Pop account settings are read 
// ModifyPopSettings2 Test Step is intended to modify existing settings and save to CenRep 
// for the specified POP account Id
// 
//

/* user includes */
#include "T_ModifyPopSettings2.h"
#include <t_utilsreademailsettingsfromconfigfile.h>
#include <t_utilscentralrepository.h>

/* epoc inlcudes */
#include <cemailaccounts.h>
#include <pop3set.h>
#include <iapprefs.h>

/* Literals used */
_LIT(KPopAccountName,"PopAccountName");
_LIT(KPopConfigFileName,"PopConfigFileName");

/**
  Function : CT_MsgModifyPopSettings2
  Description : Constructor
  @param : aSharedDataPOP Reference to CT_MsgSharedDataPop
  @return : N/A
*/
CT_MsgModifyPopSettings2::CT_MsgModifyPopSettings2(CT_MsgSharedDataPop& aSharedDataPOP)
:	CT_MsgSyncStepPOP(aSharedDataPOP)
	{
	SetTestStepName(KModifyPopSettings2);
	}
	
/**
  Function : ~CT_MsgModifyPopSettings2
  Description : Destructor
  @return : N/A
*/
CT_MsgModifyPopSettings2::~CT_MsgModifyPopSettings2()
	{
	}

/**
  Function : doTestStepL
  Description : Reads the Pop account name and config file name from the .ini file
				 and it then modifies the account with the settings read from the config file.
  @return : TVerdict - Test step result
  @leave  : KMsvNullIndexEntryId	Invalid POP account name specified	
*/
TVerdict CT_MsgModifyPopSettings2::doTestStepL()
	{
	INFO_PRINTF1(_L("Test Step: ModifyPopSettings2"));
	// Read the POP account name from the ini file
	TPtrC popAccountName;
	if(!GetStringFromConfig(ConfigSection(), KPopAccountName, popAccountName))
		{
		ERR_PRINTF1(_L("POP account name is not specified"));
		SetTestStepResult(EFail);
		}
	else
		{
		// Read the POP Config file name from the ini file
		TPtrC configFileName;
		if(!GetStringFromConfig(ConfigSection(), KPopConfigFileName, configFileName))
			{
			ERR_PRINTF1(_L("Configuration file path is not specified"));
			SetTestStepResult(EFail);
			}
		else
			{
			// Retrieving the POP service Id for the given POP account
			TMsvId popServiceId = CT_MsgUtilsCentralRepository::GetPopServiceIdL((TDes&)popAccountName);
			INFO_PRINTF2(_L("POP service Id is %d"),popServiceId);
			
			if(popServiceId == KMsvNullIndexEntryId)
				{
				ERR_PRINTF1(_L("Invalid POP account name specified"));
				SetTestStepResult(EFail);
				}
			// Creates the settings object
			else
				{
				CEmailAccounts* emailAccounts = CEmailAccounts::NewLC();

				CImPop3Settings* popSettings = new(ELeave) CImPop3Settings();
				CleanupStack::PushL(popSettings);
				
				CImIAPPreferences* popIapPrefs = CImIAPPreferences::NewLC();

				// Loads the settings for the account with the current settings
				TPopAccount popAccount;
				emailAccounts->GetPopAccountL(popServiceId, popAccount);
				emailAccounts->LoadPopSettingsL(popAccount, *popSettings);
				emailAccounts->LoadPopIapSettingsL(popAccount, *popIapPrefs);

				// Reads the settings from the config file
				TRAPD(err, CT_MsgUtilsReadEmailSettingsFromConfigFile::ReadPopSettingsFromConfigurationFileL(configFileName, *popSettings, *popIapPrefs));
				if(err)
					{	
					ERR_PRINTF2(_L("Failure while setting the POP setting parameters, failed with error %d"), err);
					SetTestStepError(err);															  
					}
				else
					{
					// Saves the new settings
					emailAccounts->SavePopSettingsL(popAccount, *popSettings);
					emailAccounts->SavePopIapSettingsL(popAccount, *popIapPrefs);
					}
				CleanupStack::PopAndDestroy(3, emailAccounts);// popIapPrefs,popSettings,emailAccounts
				}
			}
		}
	return TestStepResult();
	}
