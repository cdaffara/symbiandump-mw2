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
// @file 
// [TestStep Name]
// CreateImapAccount
// [Paramaters]
// ImapAccountName		: Name of the Imap account to be created
// SettingsFileName	: Config file from which the Imap account settings are read 
// Creates a IMAP account in the Central Repository with the specified name using the 
// configuration settings file read from the ini file.The settings file includes both IMAP settings,SMTP settings 
// and IAP preference settings for both IMAP and SMTP.
// [APIs Used]
// CEmailAccounts::CreateImapAccountL
// 
//



/* User include*/
#include "T_CreateImapAccount.h"
#include <t_utilsreademailsettingsfromconfigfile.h>



/* epoc inlcudes */
#include <cemailaccounts.h>
#include <iapprefs.h>
#include <imapset.h>
#include <smtpset.h>


/* Literals used */
_LIT(KImapAccountName,"ImapAccountName");
_LIT(KImapConfigFileName,"ImapConfigFileName");



/**
  Function : CT_MsgCreateImapAccount
  Description : Constructor
  @return : N/A
*/
CT_MsgCreateImapAccount::CT_MsgCreateImapAccount(CT_MsgSharedDataImap& aSharedDataIMAP)
:	CT_MsgSyncStepIMAP(aSharedDataIMAP)
	{
	SetTestStepName(KCreateImapAccount);
	}


/**
  Function : doTestStepL
  Description : Reads the Imap ccount name and the settings file name from the ini file.
				Creates an Imap account by reading the settings from the given settings file
  @return : TVerdict - Test step result
*/
TVerdict CT_MsgCreateImapAccount::doTestStepL()
	{
	INFO_PRINTF1(_L("Test Step: Create Imap Account"));
	
	/* Reads the Imap Account Name from ini file */
	TPtrC	imapAccountName;
	if(!GetStringFromConfig( ConfigSection(), KImapAccountName, imapAccountName))
		{
		ERR_PRINTF1(_L("Imap Account name not specified"));
		SetTestStepResult(EFail);
		}
	/* Reads the configuration file name*/
	else
		{
		TPtrC	configFileName;
		if(!GetStringFromConfig( ConfigSection(), KImapConfigFileName, configFileName))
			{
			ERR_PRINTF1(_L("Configuration file path is not specified"));
			SetTestStepResult(EFail);
			}
		else
			{
			CEmailAccounts*		emailAccounts = CEmailAccounts::NewLC();
			
			/* Creates objects requried for IMAP account */
			CImIAPPreferences*	imapPrefs = CImIAPPreferences::NewLC();
			CImImap4Settings*	imapSettings = new (ELeave) CImImap4Settings;
			CleanupStack::PushL(imapSettings);

			/* Creates objects requried for corresponding SMTP account*/
			CImIAPPreferences*	smtpIapPref = CImIAPPreferences::NewLC();
			CImSmtpSettings*	smtpSettings = new (ELeave) CImSmtpSettings;
			CleanupStack::PushL(smtpSettings);

			/* Populates the default IMAP and SMTP settings */
			emailAccounts->PopulateDefaultImapSettingsL(*imapSettings, *imapPrefs);
			emailAccounts->PopulateDefaultSmtpSettingsL(*smtpSettings, *smtpIapPref);

			/* If the setting file name is provided as input, then reads the IMAP and SMTP settings from the config file*/
			if( configFileName.CompareF( _L("none") ) != 0 )
				{
				TRAPD(err, CT_MsgUtilsReadEmailSettingsFromConfigFile::ReadImapSettingsFromConfigurationFileL(configFileName, *imapSettings, *imapPrefs));
				if(err)
					{
					ERR_PRINTF2(_L("Failure while setting the IMAP setting parameters, failed with error %d"), err);
					SetTestStepResult(EFail);															  
					}
				else
					{
					TRAP(err, CT_MsgUtilsReadEmailSettingsFromConfigFile::ReadSmtpSettingsFromConfigurationFileL(configFileName, *smtpSettings, *smtpIapPref));
					if(err)
						{
						ERR_PRINTF2(_L("Failure while setting the SMTP setting parameters, failed with error %d"), err);
						SetTestStepResult(EFail);															  
						}
					}
				}
			if( TestStepResult() == EPass )
				{
				/* Creates the IMAP account for the name specified and for the settings objects */
				TImapAccount	imapAccount = emailAccounts->CreateImapAccountL( imapAccountName, *imapSettings,*imapPrefs,EFalse);

				/* Creates the SMTP account for the name specified and for the settings objects */
				TSmtpAccount	smtpAccount = emailAccounts->CreateSmtpAccountL(imapAccount, *smtpSettings, *smtpIapPref, EFalse);
				
				/* Sets the SMTP account created as the default SMTP account */
				emailAccounts->SetDefaultSmtpAccountL(smtpAccount);	
				}
			CleanupStack::PopAndDestroy(5,emailAccounts); /*smtpSettings, smtpIapPref, imapSettings, imapPrefs, emailAccounts */
			}
		}
	return TestStepResult();
	}
