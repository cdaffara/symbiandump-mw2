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
// CreateImapAccountWithoutSmtp
// [Paramaters]
// ImapAccountName			:Name of the Imap account.
// SettingsFileName			:Config file from which the Imap account settings are read.
// Creates a IMAP account in the Central Repository with the specified name using the 
// settings read from the config file.
// 
//

//User includes
#include "T_CreateImapAccountWithoutSmtp.h"
#include <t_utilsreademailsettingsfromconfigfile.h>

//Epoc inlcudes
#include <cemailaccounts.h>
#include <iapprefs.h>
#include <imapset.h>

//Literals used
_LIT(KImapAccountName,"ImapAccountName");
_LIT(KImapConfigFileName,"ImapConfigFileName");

/**
  Function : CT_MsgCreateImapAccountWithoutSmtp
  Description : Constructor
  @param : aSharedDataIMAP Reference to CT_MsgSharedDataImap
  @return : N/A
*/
CT_MsgCreateImapAccountWithoutSmtp::CT_MsgCreateImapAccountWithoutSmtp(CT_MsgSharedDataImap& aSharedDataIMAP)
:CT_MsgSyncStepIMAP(aSharedDataIMAP)
	{
	SetTestStepName(KCreateImapAccountWithoutSmtp);
	}

/**
  Function : ReadIni
  Description :Reads the corresponding .ini file and returns EPass or EFail if any of the inputs are missing
  @return : TBool
*/
TBool CT_MsgCreateImapAccountWithoutSmtp::ReadIni()
	{
	TBool result = ETrue;
	
	if(!GetStringFromConfig(ConfigSection(), KImapAccountName, iImapAccountName))
		{
		ERR_PRINTF1(_L("Imap account name is not specified"));
		SetTestStepResult(EFail);
		result = EFalse;
		}
		
	if(!GetStringFromConfig(ConfigSection(), KImapConfigFileName, iConfigFileName))
		{
		ERR_PRINTF1(_L("Configuration file path is not specified"));
		SetTestStepResult(EFail);
		}
		
	return result;
	}

/**
  Function : doTestStepL
  Description : Reads the Imap account name and the settings file name from the .ini file
				and then creates an Imap account.
  @return : TVerdict - Test step result
*/
TVerdict CT_MsgCreateImapAccountWithoutSmtp::doTestStepL()
	{
	
	INFO_PRINTF1(_L("Test Step : CreateImapAccountWithoutSmtp"));
	if(ReadIni())
		{
		CEmailAccounts*	emailAccounts = CEmailAccounts::NewLC();
		
		// Creates objects requried for IMAP account
		CImIAPPreferences* imapPrefs = CImIAPPreferences::NewLC();
		CImImap4Settings* imapSettings = new (ELeave) CImImap4Settings;
		CleanupStack::PushL(imapSettings);

		// Populates the default IMAP settings
		emailAccounts->PopulateDefaultImapSettingsL(*imapSettings, *imapPrefs);

		//If the config file name is provided as input, then reads the IMAP settings from it
		if(iConfigFileName.CompareF( _L("none") ) != 0)
			{
			TRAPD(err, CT_MsgUtilsReadEmailSettingsFromConfigFile::ReadImapSettingsFromConfigurationFileL(iConfigFileName, *imapSettings, *imapPrefs));
			if(err)
				{
				ERR_PRINTF2(_L("Failure while setting the IMAP settings, failed with error %d"), err);
				SetTestStepError(err);
				CleanupStack::PopAndDestroy(3,emailAccounts); // imapSettings, imapPrefs, emailAccounts
				return TestStepResult();														  
				}
			if(TestStepResult() == EPass)
				{
				// Creates the IMAP account for the name specified and for the settings objects
				TImapAccount imapAccount = emailAccounts->CreateImapAccountL(iImapAccountName, *imapSettings,*imapPrefs,EFalse);
				}
			}
		CleanupStack::PopAndDestroy(3,emailAccounts); // imapSettings, imapPrefs, emailAccounts
		}
	return TestStepResult();
	}
