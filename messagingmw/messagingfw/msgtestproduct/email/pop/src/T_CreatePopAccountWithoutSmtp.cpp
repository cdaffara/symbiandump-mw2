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
// CreatePopAccountWithoutSmtp
// [Paramaters]
// PopAccountName			:Name of the Pop account.
// PopConfigFileName		:Config file from which the Pop account settings are read.
// Creates a POP account in the Central Repository with the specified name using the 
// settings read from the config file.
// 
//

//User includes
#include "T_CreatePopAccountWithoutSmtp.h"
#include <t_utilsreademailsettingsfromconfigfile.h>

//Epoc inlcudes
#include <cemailaccounts.h>
#include <iapprefs.h>
#include <pop3set.h>

//Literals used
_LIT(KPopAccountName,"PopAccountName");
_LIT(KPopConfigFileName,"PopConfigFileName");

/**
  Function : CT_MsgCreatePopAccountWithoutSmtp
  Description : Constructor
  @param : aSharedDataPOP Reference to CT_MsgSharedDataPop
  @return : N/A
*/
CT_MsgCreatePopAccountWithoutSmtp::CT_MsgCreatePopAccountWithoutSmtp(CT_MsgSharedDataPop& aSharedDataPOP)
:CT_MsgSyncStepPOP(aSharedDataPOP)
	{
	SetTestStepName(KCreatePopAccountWithoutSmtp);
	}

/**
  Function : ReadIni
  Description :Reads the corresponding .ini file and returns EPass or EFail if any of the inputs are missing
  @return : TBool
*/
TBool CT_MsgCreatePopAccountWithoutSmtp::ReadIni()
	{
	TBool result = ETrue;
	
	if(!GetStringFromConfig(ConfigSection(), KPopAccountName, iPopAccountName))
		{
		ERR_PRINTF1(_L("Pop account name is not specified"));
		SetTestStepResult(EFail);
		result = EFalse;
		}
		
	if(!GetStringFromConfig(ConfigSection(), KPopConfigFileName, iConfigFileName))
		{
		ERR_PRINTF1(_L("Configuration file path is not specified"));
		SetTestStepResult(EFail);
		}
		
	return result;
	}

/**
  Function : doTestStepL
  Description : Reads the Pop account name and the settings file name from the .ini file
				and then creates a Pop account.
  @return : TVerdict - Test step result
*/
TVerdict CT_MsgCreatePopAccountWithoutSmtp::doTestStepL()
	{
	
	INFO_PRINTF1(_L("Test Step : CreatePopAccountWithoutSmtp"));
	if(ReadIni())
		{
		CEmailAccounts*	emailAccounts = CEmailAccounts::NewLC();
		
		// Creates objects requried for POP account
		CImIAPPreferences *popIapPreferences = CImIAPPreferences::NewLC();
		CImPop3Settings *popSettings = new (ELeave) CImPop3Settings;
		CleanupStack::PushL(popSettings);

		// Populates the default POP settings
		emailAccounts->PopulateDefaultPopSettingsL(*popSettings, *popIapPreferences);

		//If the config file name is provided as input, then reads the POP settings from it
		if(iConfigFileName.CompareF( _L("none") ) != 0)
			{
			TRAPD(err, CT_MsgUtilsReadEmailSettingsFromConfigFile::ReadPopSettingsFromConfigurationFileL(iConfigFileName, *popSettings, *popIapPreferences));
			if(err)
				{
				ERR_PRINTF2(_L("Failure while setting the POP settings, failed with error %d"), err);
				SetTestStepError(err);
				CleanupStack::PopAndDestroy(3,emailAccounts); // popSettings, popIapPreferences, emailAccounts
				return TestStepResult();														  
				}
			if(TestStepResult() == EPass)
				{
				// Creates the POP account for the name specified and for the settings objects
				TPopAccount popAccount = emailAccounts->CreatePopAccountL(iPopAccountName, *popSettings,*popIapPreferences,EFalse);
				}
			}
		CleanupStack::PopAndDestroy(3,emailAccounts); // popSettings, popIapPreferences, emailAccounts
		}
	return TestStepResult();
	}
