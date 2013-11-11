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
// ComparePopSettings
// [Paramaters]
// PopAccountName		:		Name of the Pop account
// ConfigFileName		:		Config file from which the Pop account settings are read 
// ComparePopSettings is intended to compare the Pop account settings.
// 
//

/**
 @file cexppop3mailinfo.h
*/


// User includes
#include "T_ComparePopSettings.h"
#include <t_utilsconfigfileparserutility.h>
#include <t_utilsreademailsettingsfromconfigfile.h>
#include <t_utilscentralrepository.h>


// Epoc includes
#include <cemailaccounts.h>
#include <iapprefs.h>
#include <pop3set.h>
#include <smtpset.h>


// Literals Used
_LIT(KPopAccountName,"PopAccountName");
_LIT(KPopConfigFileName,"PopConfigFileName");
_LIT(KMatchExpected,"MatchExpected");


/**
CT_MsgComparePopSettings()
Constructor - Initializes the data members.

@param aSharedDataPOP
Data shared across the POP test steps. Ex. Session
*/
CT_MsgComparePopSettings::CT_MsgComparePopSettings(CT_MsgSharedDataPop& aSharedDataPOP)
:	CT_MsgSyncStepPOP(aSharedDataPOP) 
	{
	SetTestStepName(KComparePopSettings);
	}


/**
~CT_MsgComparePopSettings
Destructor
*/
CT_MsgComparePopSettings::~CT_MsgComparePopSettings()
	{
	}


/**
doTestStepL()
Reads the Pop account name and config file name from the ini file
Compares the settings of the Imap account.
  
@return
Returns the test step result
*/
TVerdict CT_MsgComparePopSettings::doTestStepL()
	{
	INFO_PRINTF1(_L("Test Step : ComparePopSettings"));
	// Read the POP account name from the ini file
	TPtrC popAccountName;
	if(!GetStringFromConfig(ConfigSection(), KPopAccountName, popAccountName))
		{
		ERR_PRINTF1(_L("PopAccount Name is not specified"));
		SetTestStepResult(EFail);
		}
	else
		{
		// Retrieving the Pop service Id for the given Pop account
		TMsvId popServiceId = KMsvNullIndexEntryId;
		TRAPD(err,popServiceId = CT_MsgUtilsCentralRepository::GetPopServiceIdL((TDes&)popAccountName));
		INFO_PRINTF2(_L("Pop service id is %d"),popServiceId);
		
		TBool matchExpected = ETrue;
		GetBoolFromConfig(ConfigSection(), KMatchExpected, matchExpected);
		
		TVerdict result = EPass;
		TPtrC configFileName;
		
		if(popServiceId == KMsvNullIndexEntryId)
			{
			INFO_PRINTF1(_L("Invalid POP account name is specified"));
			result = EFail;	
			}
		else
			{
			if(!GetStringFromConfig(ConfigSection(), KPopConfigFileName, configFileName))
				{
				ERR_PRINTF1(_L("Configuration file path is not specified"));
				SetTestStepResult(EFail);
				}
			else
				{
				CEmailAccounts* emailAccounts = CEmailAccounts::NewLC();

				// Creating the first set of settings objects
				CImPop3Settings* popSettings1 = new(ELeave) CImPop3Settings();
				CleanupStack::PushL(popSettings1);
				
				CImIAPPreferences* popIapPrefs1 = CImIAPPreferences::NewLC();
				
				CImSmtpSettings* smtpSettings1 = new(ELeave) CImSmtpSettings();
				CleanupStack::PushL(smtpSettings1);

				CImIAPPreferences* smtpIapPrefs1 = CImIAPPreferences::NewLC();
				
				// Creating the second set of settings objects	
				CImPop3Settings* popSettings2= new(ELeave) CImPop3Settings();
				CleanupStack::PushL(popSettings2);
				
				CImIAPPreferences* popIapPrefs2 = CImIAPPreferences::NewLC();
				
				CImSmtpSettings* smtpSettings2 = new(ELeave) CImSmtpSettings();
				CleanupStack::PushL(smtpSettings2);

				CImIAPPreferences* smtpIapPrefs2 = CImIAPPreferences::NewLC();
					
				emailAccounts->PopulateDefaultPopSettingsL(*popSettings2, *popIapPrefs2);
				emailAccounts->PopulateDefaultSmtpSettingsL(*smtpSettings2, *smtpIapPrefs2);
					
				TPopAccount popAccount;
				emailAccounts->	GetPopAccountL(popServiceId, popAccount);
					
				TRAPD(error1, LoadAccountSettingsL(*emailAccounts,popAccount,
					*popSettings1,*popIapPrefs1,*smtpSettings1,*smtpIapPrefs1));
				
				if(error1 != KErrNone)
					{
					ERR_PRINTF2(_L("LoadPOPPSettingsL Failed with error code %d"), error1);
					result = EFail;
					SetTestStepResult(EFail);
					}
				else
					{
					if(configFileName.CompareF(_L("none")) != 0)
						{
						TRAPD(err, CT_MsgUtilsReadEmailSettingsFromConfigFile::ReadPopSettingsFromConfigurationFileL(configFileName, *popSettings2, *popIapPrefs2));
						if(err)
							{	
							ERR_PRINTF2(_L("Failure while setting the Pop setting parameters, failed with error %d"), err);
							SetTestStepResult(EFail);															  
							}
						else
							{
							TRAPD(error, CT_MsgUtilsReadEmailSettingsFromConfigFile::ReadSmtpSettingsFromConfigurationFileL(configFileName, *smtpSettings2, *smtpIapPrefs2));
							if(error)
								{
								ERR_PRINTF2(_L("Failure while setting the SMTP setting parameters, failed with error %d"), error);
								SetTestStepResult(EFail);															  
								}							
							}							
						}
					if(TestStepResult() == EPass)
						{
						result = CompareSettings(*popSettings1 , *popSettings2 ,
											*smtpSettings1,*smtpSettings2,
											*popIapPrefs1,*popIapPrefs2,
											*smtpIapPrefs1,*smtpIapPrefs2);
						}
					}
				CleanupStack::PopAndDestroy(9,emailAccounts);
					
				}				
			}

		// if the parameters did not match and we expected them to then fail the test
		if(result == EPass && matchExpected)
			{
			INFO_PRINTF1(_L("*********Passed As Expected*******"));
			SetTestStepResult(EPass);
			}
		else if(result == EFail && !matchExpected)
			{
			INFO_PRINTF1(_L("*******Failed as Expected********"));
			SetTestStepResult(EPass); // This is required as some checks where it will fail, at that pt teststep might have been set to fail
			}
		else
			{
			ERR_PRINTF1(_L("Pop settings Comparison Failed"));
			SetTestStepResult(EFail);					
			}
		INFO_PRINTF1(_L("ComparePopSettings completed"));
		}
	return TestStepResult();
	}
	
/**
CompareSettings()
Compares the two sets of POP settings, IAPPrefernces, and SMTP settings.
  
@param aPopSettings1
An object of type CImPop3Settings

@param aPopSettings2
An object of type CImPop3Settings

@param aPopIapPrefs1
An object of type CImIAPPreferences

@param aPopIapPrefs2
An object of type CImIAPPreferences

@param aSmtpIapPrefs1
An object of type CImIAPPreferences

@param aSmtpIapPrefs2
An object of type CImIAPPreferences

@param aSmtpSettings
An object of type CImSmtpSettings

@param aSmtpIapSettings
An object of type CImIAPPreferences

@return
Returns EPass if exactly the same for all values in the two sets of parameters.
Returns EFail if any value is different between the two sets of parameters.
*/
TVerdict CT_MsgComparePopSettings::CompareSettings(CImPop3Settings& aPopSettings1, CImPop3Settings& aPopSettings2,
						 CImSmtpSettings& aSmtpSettings1,CImSmtpSettings& aSmtpSettings2, CImIAPPreferences& aPopIapPrefs1,
							CImIAPPreferences& aPopIapPrefs2, CImIAPPreferences& aSmtpIapPrefs1, CImIAPPreferences& aSmtpIapPrefs2)
	{
	TVerdict result = EPass;
	if (!(aPopSettings1 == aPopSettings2))   
		{
		// failed
		ERR_PRINTF1(_L("Pop3 Settings objects do not match !"));
		result = EFail;
		}
	else if (!(aSmtpSettings1 == aSmtpSettings2))
		{
		// failed
		ERR_PRINTF1(_L("SMTP Settings objects do not match !"));
		result = EFail;
		}
	else if (!CT_MsgUtilsReadEmailSettingsFromConfigFile::CompareIapPrefs(aPopIapPrefs1, aPopIapPrefs2))
		{
		// failed
		ERR_PRINTF1(_L("Pop3 IAP Preference objects do not match !"));
		result = EFail;
		}
	else if (!CT_MsgUtilsReadEmailSettingsFromConfigFile::CompareIapPrefs(aSmtpIapPrefs1, aSmtpIapPrefs2))
		{
		// failed
		ERR_PRINTF1(_L("SMTP IAP Preference objects do not match !"));
		result = EFail;
		}
	
	return result;
	}
	
/**
LoadAccountSettingsL()
This function loads the IMAP and SMTP settings object from the IMAP account given

@param aEmailAccount
An object of type CEmailAccounts

@param aAccount
An object of type TPopAccount

@param aPopSettings
An object of type CImPop3Settings

@param aPopIapSettings
An object of type CImIAPPreferences

@param aSmtpSettings
An object of type CImSmtpSettings

@param aSmtpIapSettings
An object of type CImIAPPreferences

@return : void
*/	
void CT_MsgComparePopSettings::LoadAccountSettingsL(CEmailAccounts& aEmailAccount, const TPopAccount& aAccount, CImPop3Settings& aPopSettings, 
		CImIAPPreferences& aPopIapSettings,CImSmtpSettings& aSmtpSettings, CImIAPPreferences& aSmtpIapSettings)
	{
	aEmailAccount.LoadPopSettingsL(aAccount, aPopSettings);
	aEmailAccount.LoadPopIapSettingsL(aAccount, aPopIapSettings);
	
	TSmtpAccount smtpAccount;
	aEmailAccount.GetSmtpAccountL(aAccount.iSmtpService, smtpAccount);
	aEmailAccount.LoadSmtpSettingsL(smtpAccount, aSmtpSettings);
	aEmailAccount.LoadSmtpIapSettingsL(smtpAccount, aSmtpIapSettings);
	}

