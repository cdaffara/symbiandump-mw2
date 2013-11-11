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
// ModifyImapSettings
// [Paramaters]
// ImapAccountName			:Name of the Imap account
// ImapConfigFileName		:Config file from which the Imap account settings are read 
// ModifyImapSettings Test Step is intended to modify existing settings and save to CenRep 
// for the specified IMAP account Id
// 
//

/* user includes */
#include "T_ModifyImapSettings.h"
#include <t_utilsreademailsettingsfromconfigfile.h>
#include <t_utilscentralrepository.h>

/* epoc inlcudes */
#include <cemailaccounts.h>
#include <imapset.h>
#include <iapprefs.h>

/* Literals used */
_LIT(KImapAccountName,"ImapAccountName");
_LIT(KImapConfigFileName,"ImapConfigFileName");

/**
  Function : CT_MsgModifyImapSettings
  Description : Constructor
  @param : aSharedDataIMAP Reference to CT_MsgSharedDataImap
  @return : N/A
*/
CT_MsgModifyImapSettings::CT_MsgModifyImapSettings(CT_MsgSharedDataImap& aSharedDataIMAP)
:	CT_MsgSyncStepIMAP(aSharedDataIMAP)
	{
	SetTestStepName(KModifyImapSettings);
	}
	
/**
  Function : ~CT_MsgModifyImapSettings
  Description : Destructor
  @return : N/A
*/
CT_MsgModifyImapSettings::~CT_MsgModifyImapSettings()
	{
	}

/**
  Function : doTestStepL
  Description : Reads the Imap account name and config file name from the .ini file
				and it then modifies the account with the settings read from this config file.
  @return : TVerdict - Test step result
  @leave  : KMsvNullIndexEntryId	Invalid IMAP account name specified	
*/
TVerdict CT_MsgModifyImapSettings::doTestStepL()
	{
	INFO_PRINTF1(_L("Test Step: ModifyImapSettings"));
	// Read the IMAP account name from the ini file
	TPtrC imapAccountName;
	if(!GetStringFromConfig(ConfigSection(), KImapAccountName, imapAccountName))
		{
		ERR_PRINTF1(_L("ImapAccount Name is not specified"));
		SetTestStepResult(EFail);
		}	
	else
		{
		// Read the IMAP Config file name from the ini file
		TPtrC configFileName;
		if(!GetStringFromConfig(ConfigSection(), KImapConfigFileName, configFileName))
			{
			ERR_PRINTF1(_L("Configuration file path is not specified"));
			SetTestStepResult(EFail);
			}
		else
			{
			// Retrieves the Imap service Id for the given Imap account
			TMsvId imapServiceId = CT_MsgUtilsCentralRepository::GetImapServiceIdL((TDes&)imapAccountName);
			INFO_PRINTF2(_L("Imap service id is %d"),imapServiceId);
			
			if(imapServiceId == KMsvNullIndexEntryId)
				{
				ERR_PRINTF1(_L("Invalid IMAP account name specified"));
				SetTestStepResult(EFail);
				}
			// Creates the settings object
			else
				{
				CEmailAccounts* emailAccounts = CEmailAccounts::NewLC();

				CImImap4Settings* imapSettings = new(ELeave) CImImap4Settings();
				CleanupStack::PushL(imapSettings);
				
				CImIAPPreferences* imapIapPrefs = CImIAPPreferences::NewLC();

				// Loads the settings for the account with the current settings
				TImapAccount imapAccount;
				emailAccounts->GetImapAccountL(imapServiceId, imapAccount);
				emailAccounts->LoadImapSettingsL(imapAccount, *imapSettings);
				emailAccounts->LoadImapIapSettingsL(imapAccount, *imapIapPrefs);

				// Reads the settings from the config file
				TRAPD(err, CT_MsgUtilsReadEmailSettingsFromConfigFile::ReadImapSettingsFromConfigurationFileL(configFileName, *imapSettings, *imapIapPrefs));
				if(err)
					{	
					ERR_PRINTF2(_L("Failure while setting the IMAP setting parameters, failed with error %d"), err);
					SetTestStepError(err);															  
					}
				else
					{
					// Saves the new settings
					emailAccounts->SaveImapSettingsL(imapAccount, *imapSettings);
					emailAccounts->SaveImapIapSettingsL(imapAccount, *imapIapPrefs);
					}
				CleanupStack::PopAndDestroy(3, emailAccounts);// imapIapPrefs,imapSettings,emailAccounts
				}
			}
		}
	return TestStepResult();
	}
