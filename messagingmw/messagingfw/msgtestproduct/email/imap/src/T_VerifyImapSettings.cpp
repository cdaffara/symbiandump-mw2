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
// VerifyImapSettings
// [Paramaters]
// ImapAccountName		:		Name of the Imap account
// VerifyImapSettings is intended to populate the default IMAP and SMTP settings,
// Load the settings of the account created with the default settings, and compare 
// the settings objects.The test passes if all the settings objects match else it fails.
// [APIs Used]	
// CEmailAccounts::ModifyImapSettingsL
// CImImap4Settings::SetServerAddressL
// CImImap4Settings::SetUserAddressL
// CImImap4Settings::SetLoginNameL
// CImImap4Settings::SetPasswordL
// CImImap4Settings::SetPort
// CImImap4Settings::SetSecureSockets
// CImImap4Settings::SetVersion
// CImImap4Settings::SetFolderPathL
// CImImap4Settings::SetPathSeparator
// CImImap4Settings::SetMaxEmailSize
// CImImap4Settings::SetGetMailOptions
// CImImap4Settings::SetInboxSynchronisationLimit
// CImImap4Settings::SetMailboxSynchronisationLimit
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



/*  User includes */
#include "T_VerfiyImapSettings.h"
#include <t_utilsreademailsettingsfromconfigfile.h>
#include <t_utilscentralrepository.h>




/* Epoc inlcudes */
#include <cemailaccounts.h>
#include <imapset.h>
#include <iapprefs.h>
#include <smtpset.h>



/* Literals used */
_LIT(KImapAccountName,"ImapAccountName");


/**
  Function : CT_MsgVerifyImapSettings
  Description : Constructor
  @return : N/A
*/
CT_MsgVerifyImapSettings::CT_MsgVerifyImapSettings(CT_MsgSharedDataImap& aSharedDataIMAP)
:	CT_MsgSyncStepIMAP(aSharedDataIMAP)
	{
	SetTestStepName(KVerifyImapSettings);
	}


	
/**
  Function : ~CT_MsgVerifyImapSettings
  Description : Destructor
  @return : N/A
*/
CT_MsgVerifyImapSettings::~CT_MsgVerifyImapSettings()
	{
	}



/**
  Function : doTestStepL
  Description : Populate the default IMAP and SMTP settings,
				Load the settings of the account created with the default settings, and compares 
				the settings objects.The test passes if all the settings objects match else it fails.
  @return : TVerdict - Test step result
  @leave	: KMsvNullIndexEntryId		Invalid IMAP account name specified
*/
TVerdict CT_MsgVerifyImapSettings::doTestStepL()
	{
	INFO_PRINTF1(_L("Test Step: VerifyImapSettings"));

	/* Reads the IMAP account name from the ini file */
	TPtrC imapAccountName;
	if(!GetStringFromConfig( ConfigSection(), KImapAccountName, imapAccountName))
		{
		ERR_PRINTF1(_L("ImapAccount Name is not specified"));
		SetTestStepResult(EFail);
		}
	else
		{
		/* Retrieves the Imap service Id for the given Imap account */
		TMsvId imapServiceId = CT_MsgUtilsCentralRepository::GetImapServiceIdL((TDes&)imapAccountName);
		INFO_PRINTF2(_L("Imap service id is %d"),imapServiceId );
		
		/* If IMAP Service Id is KMsvNullIndexEntryId then the fail the test step */
		if(imapServiceId == KMsvNullIndexEntryId)
			{
			ERR_PRINTF1(_L("Invalid IMAP account name specified"));
			SetTestStepResult(EFail);
			}
		/* 
		 * If a valid IMAP account is specified creates the IMAP and SMTP settings object and populates them with
		 * the default settings 
		 */
		 
		else
			{
			CEmailAccounts* emailAccounts = CEmailAccounts::NewLC();
	
			CImImap4Settings* imapSettings1 = new(ELeave) CImImap4Settings();
			CleanupStack::PushL(imapSettings1);
			
			CImIAPPreferences* imapIapPrefs1 = CImIAPPreferences::NewLC();
			
			CImSmtpSettings* smtpSettings1 = new(ELeave) CImSmtpSettings();
			CleanupStack::PushL(smtpSettings1);

			CImIAPPreferences* smtpIapPrefs1 = CImIAPPreferences::NewLC();

			emailAccounts->PopulateDefaultImapSettingsL(*imapSettings1, *imapIapPrefs1);
			emailAccounts->PopulateDefaultSmtpSettingsL(*smtpSettings1, *smtpIapPrefs1);

					
			CImImap4Settings* imapSettings2 = new(ELeave) CImImap4Settings();
			CleanupStack::PushL(imapSettings2);
			
			CImIAPPreferences* imapIapPrefs2 = CImIAPPreferences::NewLC();
			
			CImSmtpSettings* smtpSettings2 = new(ELeave) CImSmtpSettings();
			CleanupStack::PushL(smtpSettings2);

			CImIAPPreferences* smtpIapPrefs2 = CImIAPPreferences::NewLC();
			
			/* Gets the IMAP account and loads the settings on to the created settings objects */
			TImapAccount imapAccount;
			emailAccounts->GetImapAccountL(imapServiceId, imapAccount);
			emailAccounts->LoadImapSettingsL(imapAccount, *imapSettings2);
			emailAccounts->LoadImapIapSettingsL(imapAccount, *imapIapPrefs2);
				
			TSmtpAccount smtpAccount;
			emailAccounts->GetSmtpAccountL(imapAccount.iSmtpService, smtpAccount);
			emailAccounts->LoadSmtpSettingsL(smtpAccount, *smtpSettings2);
			emailAccounts->LoadSmtpIapSettingsL(smtpAccount, *smtpIapPrefs2);

			/* Compares the two settings objects and fails the test step if any of them do 
			 * not match
			 */
			if( TestStepResult() == EPass )
				{
				TVerdict	result = EPass;
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
				SetTestStepResult(result);
				}
			CleanupStack::PopAndDestroy(9,emailAccounts);
			}
		}
	return TestStepResult();
	}
	
	

						
