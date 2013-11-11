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
// VerifyPopSettings
// [Paramaters]
// PopAccountName		:		Name of the Pop account
// VerifyPopSettings is intended to populate the default POP and SMTP settings,
// Load the settings of the account created with the default settings, and compare 
// the settings objects.The test passes if all the settings objects match else it fails.
// [APIs Used]
// GetPopAccountL
// LoadPopSettingsL
// LoadPopIapSettingsL
// 
//

/**
 @file
*/


// User include
#include "T_VerfiyPopSettings.h"
#include <t_utilsreademailsettingsfromconfigfile.h>
#include <t_utilscentralrepository.h>


// epoc inlcudes
#include <cemailaccounts.h>
#include <pop3set.h>
#include <iapprefs.h>
#include <smtpset.h>


// Literals used
_LIT(KPopAccountName,"PopAccountName");


/**
CT_MsgVerifyPopSettings()
Sets the teststep name to KCheckChildrenCountPop

@param aSharedDataPOP
Data shared across the POP test steps. Ex. Sessoin 
*/
CT_MsgVerifyPopSettings::CT_MsgVerifyPopSettings(CT_MsgSharedDataPop& aSharedDataPOP)
:	CT_MsgSyncStepPOP(aSharedDataPOP) 
	{
	SetTestStepName(KVerifyPopSettings);
	}


/**
~CT_MsgVerifyPopSettings()
Destructor
*/
CT_MsgVerifyPopSettings::~CT_MsgVerifyPopSettings()
	{
	}


/**
doTestStepL()
Populate the default POP and SMTP settings, Load the settings of the account 
created with the default settings, and compares the settings objects.
The test passes if all the settings objects match else it fails.

@return
Returns the test step result
*/
TVerdict CT_MsgVerifyPopSettings::doTestStepL()
	{
	INFO_PRINTF1(_L("Test Step: VerifyPopSettings"));
	// Read the Pop account name from the ini file
	TPtrC popAccountName;
	if(!GetStringFromConfig(ConfigSection(), KPopAccountName, popAccountName))
		{
		ERR_PRINTF1(_L("PopAccount Name is not specified"));
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
	
			CImPop3Settings* popSettings1 = new(ELeave) CImPop3Settings();
			CleanupStack::PushL(popSettings1);
			
			CImIAPPreferences* popIapPrefs1 = CImIAPPreferences::NewLC();
			
			CImSmtpSettings* smtpSettings1 = new(ELeave) CImSmtpSettings();
			CleanupStack::PushL(smtpSettings1);

			CImIAPPreferences* smtpIapPrefs1 = CImIAPPreferences::NewLC();

			emailAccounts->PopulateDefaultPopSettingsL(*popSettings1, *popIapPrefs1);
			emailAccounts->PopulateDefaultSmtpSettingsL(*smtpSettings1, *smtpIapPrefs1);

			
	
			CImPop3Settings* popSettings2 = new(ELeave) CImPop3Settings();
			CleanupStack::PushL(popSettings2);
			
			CImIAPPreferences* popIapPrefs2 = CImIAPPreferences::NewLC();
			
			CImSmtpSettings* smtpSettings2 = new(ELeave) CImSmtpSettings();
			CleanupStack::PushL(smtpSettings2);

			CImIAPPreferences* smtpIapPrefs2 = CImIAPPreferences::NewLC();

			TPopAccount popAccount;
			emailAccounts->GetPopAccountL(popServiceId, popAccount);
			emailAccounts->LoadPopSettingsL(popAccount, *popSettings2);
			emailAccounts->LoadPopIapSettingsL(popAccount, *popIapPrefs2);
			
			TSmtpAccount smtpAccount;
			emailAccounts->GetSmtpAccountL(popAccount.iSmtpService, smtpAccount);
			emailAccounts->LoadSmtpSettingsL(smtpAccount, *smtpSettings2);
			emailAccounts->LoadSmtpIapSettingsL(smtpAccount, *smtpIapPrefs2);

			TVerdict result = EPass;
			if (!(*popSettings1 == *popSettings2))   
				{
				// failed
				ERR_PRINTF1(_L("Pop3 Settings objects do not match !"));
				result = EFail;
				}
			else if (!(*smtpSettings1 == *smtpSettings2))
				{
				// failed
				ERR_PRINTF1(_L("SMTP Settings objects do not match !"));
				result = EFail;
				}
			else if (!CT_MsgUtilsReadEmailSettingsFromConfigFile::CompareIapPrefs(*popIapPrefs1, *popIapPrefs2))
				{
				// failed
				ERR_PRINTF1(_L("Pop3 IAP Preference objects do not match !"));
				result = EFail;
				}
			else if (!CT_MsgUtilsReadEmailSettingsFromConfigFile::CompareIapPrefs(*smtpIapPrefs1, *smtpIapPrefs2))
				{
				// failed
				ERR_PRINTF1(_L("SMTP IAP Preference objects do not match !"));
				result = EFail;
				}
			SetTestStepResult(result);
			CleanupStack::PopAndDestroy(9,emailAccounts);
			}
		}
	return TestStepResult();
	}


