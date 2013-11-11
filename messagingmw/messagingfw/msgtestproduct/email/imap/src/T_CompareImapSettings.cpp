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
// CompareImapSettings
// [Paramaters]
// ImapAccountName		:		Name of the Imap account
// ConfigFileName		:		Config file from which the Imap account settings are read 
// [Test Step Description]
// CompareImapSettings compares the IMAP account settings with the settings
// read from the config file.
// 
//



/* User includes */
#include "T_CompareImapSettings.h"
#include <t_utilsconfigfileparserutility.h>
#include <t_utilsreademailsettingsfromconfigfile.h>
#include <t_utilscentralrepository.h>



/* Epoc includes */
#include <cemailaccounts.h>
#include <iapprefs.h>
#include <imapset.h>
#include <smtpset.h>



/* Literals Used */
_LIT(KImapAccountName,		"ImapAccountName");
_LIT(KImapConfigFileName,	"ImapConfigFileName");
_LIT(KMatchExpected,		"MatchExpected");



/**
  Function : CT_MsgCompareImapSettings
  Description : Constructor
  @param :	aSharedDataIMAP		Reference to CT_MsgSharedDataImap
  @return : N/A
*/
CT_MsgCompareImapSettings::CT_MsgCompareImapSettings(CT_MsgSharedDataImap& aSharedDataIMAP)
:	CT_MsgSyncStepIMAP(aSharedDataIMAP)
	{
	SetTestStepName(KCompareImapSettings);
	}


/**
  Function : doTestStepL
  Description : Reads the Imap account name and config file name from the ini file
				Compares the settings of the Imap account.
  @return : TVerdict - Test step result
  @leave  : KMsvNullIndexEntryId	Invalid IMAP account name specified
*/
TVerdict CT_MsgCompareImapSettings::doTestStepL()
	{
	INFO_PRINTF1(_L("Test Step : CompareImapSettings"));

	/* Reads the expected children count from ini file */
	TPtrC	imapAccountName;
	if(!GetStringFromConfig( ConfigSection(), KImapAccountName, imapAccountName))
		{
		ERR_PRINTF1(_L("ImapAccount Name is not specified"));
		SetTestStepResult(EFail);
		}
	else
		{
		/* Retrieves the Imap service Id for the given Imap account */
		TMsvId	imapServiceId = CT_MsgUtilsCentralRepository::GetImapServiceIdL((TDes&)imapAccountName);
		INFO_PRINTF2(_L("Imap service id is %d"),imapServiceId );

		/* If IMAP Service Id is KMsvNullIndexEntryId then the fail the test step */
		if(imapServiceId == KMsvNullIndexEntryId)
			{
			ERR_PRINTF1(_L("Invalid IMAP account name specified"));
			SetTestStepResult(EFail);
			}
		/* 
		 * If a valid IMAP account is specified then read the name of the Configuration file 
		 * to be compared with.
		 */
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
				/* Reads whether a match is expected */
				TBool	matchExpected = ETrue;
				GetBoolFromConfig( ConfigSection(), KMatchExpected, matchExpected);
				
				/* Creates the IMAP and SMTP settings object and populate them with
				 * the default settings */
				CEmailAccounts*		emailAccounts = CEmailAccounts::NewLC();

				CImImap4Settings*	imapSettings1 = new(ELeave) CImImap4Settings();
				CleanupStack::PushL(imapSettings1);
				CImIAPPreferences*	imapIapPrefs1 = CImIAPPreferences::NewLC();
				CImSmtpSettings*	smtpSettings1 = new(ELeave) CImSmtpSettings();
				CleanupStack::PushL(smtpSettings1);
				CImIAPPreferences*	smtpIapPrefs1 = CImIAPPreferences::NewLC();

				CImImap4Settings*	imapSettings2= new(ELeave) CImImap4Settings();
				CleanupStack::PushL(imapSettings2);
				CImIAPPreferences*	imapIapPrefs2 = CImIAPPreferences::NewLC();
				CImSmtpSettings*	smtpSettings2 = new(ELeave) CImSmtpSettings();
				CleanupStack::PushL(smtpSettings2);
				CImIAPPreferences*	smtpIapPrefs2 = CImIAPPreferences::NewLC();

				emailAccounts->PopulateDefaultImapSettingsL(*imapSettings2, *imapIapPrefs2);
				emailAccounts->PopulateDefaultSmtpSettingsL(*smtpSettings2, *smtpIapPrefs2);
				
				/* Gets the IMAP account and loads the settings on to the created settings objects */
				TImapAccount imapAccount;
				emailAccounts->	GetImapAccountL(imapServiceId, imapAccount);

				TRAPD( error1, LoadAccountSettingsL(*emailAccounts, imapAccount, *imapSettings1, *imapIapPrefs1, 
						*smtpSettings1, *smtpIapPrefs1) );

				/* If the loading fails , fails the test step */
				if(error1 != KErrNone)
					{
					ERR_PRINTF2(_L("LoadIMAPSettingsL Failed with error code %d"), error1);
					SetTestStepResult(EFail);
					}
				/* Else reads the settings from the configuration file specified */
				else
					{
					TRAPD( err, CT_MsgUtilsReadEmailSettingsFromConfigFile::ReadImapSettingsFromConfigurationFileL(configFileName, *imapSettings2, *imapIapPrefs2));
					if(err)
						{	
						ERR_PRINTF2(_L("Failure while setting the IMAP setting parameters, failed with error %d"), err);
						SetTestStepResult(EFail);															  
						}
					else
						{
						TRAPD( error, CT_MsgUtilsReadEmailSettingsFromConfigFile::ReadSmtpSettingsFromConfigurationFileL(configFileName, *smtpSettings2, *smtpIapPrefs2));
						if(error)
							{
							ERR_PRINTF2(_L("Failure while setting the SMTP setting parameters, failed with error %d"), error);
							SetTestStepResult(EFail);															  
							}
						else
							{
							TVerdict result = EPass;
							/* Compares the settings of the IMAP account created with those of the configuration file */
							if (!(*imapSettings1 == *imapSettings2))   
								{
								ERR_PRINTF1(_L("IMAP4 Settings objects do not match !"));
								result = EFail;
								}
							else if (!(*smtpSettings1 == *smtpSettings2))
								{
								ERR_PRINTF1(_L("SMTP Settings objects do not match !"));
								result = EFail;
								}
							else if (!CT_MsgUtilsReadEmailSettingsFromConfigFile::CompareIapPrefs(*imapIapPrefs1, *imapIapPrefs2))
								{
								ERR_PRINTF1(_L("IMAP4 IAP Preference objects do not match !"));
								result = EFail;
								}
							else if (!CT_MsgUtilsReadEmailSettingsFromConfigFile::CompareIapPrefs(*smtpIapPrefs1, *smtpIapPrefs2))
								{
								ERR_PRINTF1(_L("SMTP IAP Preference objects do not match !"));
								result = EFail;
								}
							/* If the parameters do not match and we expected them to then fail the test*/			
							if( result == EFail && matchExpected != EFalse )
								{
								ERR_PRINTF1(_L("Expected to match but failed comparison"));
								SetTestStepResult(EFail);	
								}
							/* If the parameters matched and we didn't expected them to then fail the test*/
							if( result == EPass && matchExpected == EFalse)
								{
								ERR_PRINTF1(_L("Expected to fail comparison but matched"));
								SetTestStepResult(EFail);	
								}
							INFO_PRINTF1(_L("CompareImapSettings completed"));
							}
						}
					}
				CleanupStack::PopAndDestroy(9, emailAccounts);
				}
			}
		}
	return TestStepResult();
	}
	


/**
  Function : LoadAccountSettingsL
  Description : This function loads the IMAP and SMTP settings object from the IMAP account given
  @internalTechnology
  @param :	aEmailAccount		An object of type CEmailAccounts
			aAccount			An object of type TImapAccount
			aImapSettings		An object of type CImImap4Settings
			aImapIapSettings	An object of type CImIAPPreferences
			aSmtpSettings		An object of type CImSmtpSettings
			aSmtpIapSettings	An object of type CImIAPPreferences
  @return : TVerdict - Test step result
*/	
void CT_MsgCompareImapSettings::LoadAccountSettingsL(CEmailAccounts& aEmailAccount, const TImapAccount& aAccount, CImImap4Settings& aImapSettings, 
		CImIAPPreferences& aImapIapSettings,CImSmtpSettings& aSmtpSettings, CImIAPPreferences& aSmtpIapSettings)
	{
	aEmailAccount.LoadImapSettingsL(aAccount, aImapSettings);
	aEmailAccount.LoadImapIapSettingsL(aAccount, aImapIapSettings);

	TSmtpAccount smtpAccount;
	aEmailAccount.GetSmtpAccountL(aAccount.iSmtpService, smtpAccount);
	aEmailAccount.LoadSmtpSettingsL(smtpAccount, aSmtpSettings);
	aEmailAccount.LoadSmtpIapSettingsL(smtpAccount, aSmtpIapSettings);
	}
