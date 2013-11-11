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
// CreateSmtpMobilityAccount
// [Parameters]
// EmailAddress: Email address for mobility account / linked SMTP account
// ConfigFileName: Configuration file name to read for account settings
// Linked: Create a linked mobility account
// AP ID: Access point identifier (eg. IAP number)
// [TestStep Description]
// Creates a mobility account under the default SMTP account
// If linked account option is chosen, also creates a new SMTP account
// and links the mobility account to it.
// 
//

#include "T_CreateSmtpMobilityAccount.h"
#include <t_utilsreademailsettingsfromconfigfile.h>
#include <cemailaccounts.h>
#include <iapprefs.h>
#include <smtpset.h>
#include <imapset.h>
#include <cimsmtpmobilitysettings.h>

_LIT(KMobilityAccountName, "Mobility Account");
_LIT(KLinkedAccountName, "Linked Account");

_LIT(KEmailAddress, "EmailAddress");
_LIT(KLinked, "Linked");
_LIT(KConfigFileName, "ConfigFileName");
_LIT(KAccessPoint, "AccessPoint");

/**
	Function : NewL()
	Description : Static factory constructor.

	@param aSharedDataSMTP

	@return A pointer to the newly created CT_MsgCreateSmtpMobilityAccount object.
*/
CT_MsgCreateSmtpMobilityAccount* CT_MsgCreateSmtpMobilityAccount::NewL(CT_MsgSharedDataSmtp& aSharedDataSMTP)
	{
	CT_MsgCreateSmtpMobilityAccount* self = new(ELeave) CT_MsgCreateSmtpMobilityAccount(aSharedDataSMTP);
	return self;
	}

/**
  Function : CT_MsgCreateSmtpMobilityAccount
  Description : Constructor
  @return : N/A
*/
CT_MsgCreateSmtpMobilityAccount::CT_MsgCreateSmtpMobilityAccount(CT_MsgSharedDataSmtp& aSharedDataSMTP)
:	CT_MsgSyncStepSMTP(aSharedDataSMTP)
	{
	SetTestStepName(KCreateSmtpMobilityAccount);
	}

/**
  Function : doTestStepL
  Description : Creates SMTP mobility account
  @return : TVerdict - Test step result
*/
TVerdict CT_MsgCreateSmtpMobilityAccount::doTestStepL()
	{
	INFO_PRINTF1(_L("Test Step: Create Smtp Mobility Account"));

	TPtrC emailAddress;
	if(!GetStringFromConfig(ConfigSection(), KEmailAddress, emailAddress))
		{
		ERR_PRINTF1(_L("Email address is not specified"));
		SetTestStepResult(EFail);
		return TestStepResult();
		}

	TPtrC	configFileName;
	if(!GetStringFromConfig(ConfigSection(), KConfigFileName, configFileName))
		{
		ERR_PRINTF1(_L("Configuration file path is not specified"));
		SetTestStepResult(EFail);
		return TestStepResult();
		}

	TBool linked = EFalse;
	GetBoolFromConfig(ConfigSection(), KLinked, linked);

	TInt accessPoint = 0;
	GetIntFromConfig(ConfigSection(), KAccessPoint, accessPoint);

	CEmailAccounts* emailAccounts = CEmailAccounts::NewLC();

	CImIAPPreferences* smtpIapPrefs = CImIAPPreferences::NewLC();
	CImSmtpSettings* smtpSettings = new (ELeave) CImSmtpSettings;
	CleanupStack::PushL(smtpSettings);

	if (linked || accessPoint == 0)
		{
		emailAccounts->PopulateDefaultSmtpSettingsL(*smtpSettings, *smtpIapPrefs);

		if( configFileName.CompareF( _L("none") ) != 0 )
			{
			TRAPD(err, CT_MsgUtilsReadEmailSettingsFromConfigFile::ReadSmtpSettingsFromConfigurationFileL(configFileName, *smtpSettings, *smtpIapPrefs));

			if (err == KErrNone)
				{
				if (accessPoint == 0)
					{
					if (smtpIapPrefs->NumberOfIAPs() > 0)
						{
						TImIAPChoice iapChoice = smtpIapPrefs->IAPPreference(0);
						accessPoint = iapChoice.iIAP;
						}
					}
				}
			else
				{
				ERR_PRINTF2(_L("Failure while setting the SMTP setting parameters, failed with error %d"), err);
				SetTestStepResult(EFail);					  
				}
			}
		}

	RArrayIAP iapList;
	CleanupClosePushL(iapList);
	if (TestStepResult() == EPass)
		{
		if (accessPoint != 0)
			{
			iapList.AppendL(accessPoint);
			}
		else
			{
			ERR_PRINTF1(_L("Access point not specified"));
			SetTestStepResult(EFail);					  
			}
		}

	TSmtpAccount parentSmtpAccount;

	if (TestStepResult() == EPass)
		{
		if (emailAccounts->DefaultSmtpAccountL(parentSmtpAccount) != KErrNone)
			{
			ERR_PRINTF1(_L("Default SMTP account not found"));
			SetTestStepResult(EFail);															  
			}
		}

	if (TestStepResult() == EPass)
		{
		if (linked)
			{
			/* Creates objects requried for IMAP account */
			CImIAPPreferences* imapIapPrefs = CImIAPPreferences::NewLC();
			CImImap4Settings* imapSettings = new (ELeave) CImImap4Settings;
			CleanupStack::PushL(imapSettings);
			emailAccounts->PopulateDefaultImapSettingsL(*imapSettings, *imapIapPrefs);

			if( configFileName.CompareF( _L("none") ) != 0 )
				{
				TRAPD(err, CT_MsgUtilsReadEmailSettingsFromConfigFile::ReadImapSettingsFromConfigurationFileL(configFileName, *imapSettings, *imapIapPrefs));

				if (err != KErrNone)
					{
					ERR_PRINTF2(_L("Failure while setting the IMAP setting parameters, failed with error %d"), err);
					SetTestStepResult(EFail);															  
					}
				}

			if( TestStepResult() == EPass )
				{
				smtpSettings->SetEmailAddressL(emailAddress);

				/* Creates the IMAP account for the name specified and for the settings objects */
				TImapAccount linkedImapAccount = emailAccounts->CreateImapAccountL(KLinkedAccountName, *imapSettings, *imapIapPrefs, EFalse);

				/* Creates the SMTP account for the name specified and for the settings objects */
				TSmtpAccount linkedSmtpAccount = emailAccounts->CreateSmtpAccountL(linkedImapAccount, *smtpSettings, *smtpIapPrefs, EFalse);

				emailAccounts->CreateSmtpMobilityAccountL(KMobilityAccountName, iapList, parentSmtpAccount, linkedSmtpAccount, EFalse);
				}
			CleanupStack::PopAndDestroy(2, imapIapPrefs);
			}
		else
			{
			CImSmtpMobilitySettings* mobilitySettings = CImSmtpMobilitySettings::NewLC();
			emailAccounts->PopulateDefaultSmtpMobilitySettingsL(*mobilitySettings);
			mobilitySettings->SetEmailAddressL(emailAddress);
			emailAccounts->CreateSmtpMobilityAccountL(KMobilityAccountName, iapList, parentSmtpAccount, *mobilitySettings, EFalse);
			CleanupStack::PopAndDestroy(mobilitySettings);
			}
		}

	CleanupStack::PopAndDestroy(4, emailAccounts);

	return TestStepResult();
	}
