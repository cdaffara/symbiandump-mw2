// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// __ACTION_INFO_BEGIN__ 
// [Action Name]
// SavePopSettings
// [Action Parameters]
// accountId			input 			TPOP3Account
// configFileIndex		input			Index of config file	
// PopSettings			output			CImPop3Settings
// PopIAP				output			CImIAPPreferences
// SMTPSettings			output			CImSMTPSettings
// SmtpIAP				output			CImIAPPreferences
// [Action Description]
// SavePopSettings Test Action is intended to modify and save existing POP settings to 
// CenRep for the specified POP account Id from configuration settings file.
// [APIs Used]	
// CEmailAccounts::SavePopSettingsL
// CImPop3Settings::SetServerAddressL
// CImPop3Settings::SetLoginNameL
// CImPop3Settings::SetPasswordL
// CImPop3Settings::SetMaxEmailSize
// CImPop3Settings::SetPort
// CImPop3Settings::SetInboxSynchronisationLimit
// CImPop3Settings::SetUserAddressL
// CImPop3Settings::SetAutoSendOnConnect
// CImPop3Settings::SetApop
// CImPop3Settings::SetDisconnectedUserMode
// CImPop3Settings::SetDeleteEmailWhenDisconnecting
// CImPop3Settings::SetAcknowledgeReceipts
// CImPop3Settings::SetGetMailOptions
// CImPop3Settings::SetPopulationLimit
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
// CImIAPPreferences::AddIAPL
// __ACTION_INFO_END__
// 
//


#include <pop3set.h>
#include <smtpset.h>

#include <cemailaccounts.h>
#include <iapprefs.h>

#include "CMtfTestActionSavePopSettings.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "CMtfTestActionUtilsPopScripts.h"


/**
  Function : NewL
  Description : 
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @param : aActionParams - CMtfTestActionParameters 
  @return : CMtfTestAction* - a base class pointer to the newly created CMtfTestActionSavePopSettings object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionSavePopSettings::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSavePopSettings* self = new (ELeave) CMtfTestActionSavePopSettings(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function : CMtfTestActionSavePopSettings
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return : N/A
  @pre none
  @post none
*/
CMtfTestActionSavePopSettings::CMtfTestActionSavePopSettings(CMtfTestCase& aTestCase) : CMtfSynchronousTestAction(aTestCase)
	{
	}

/**
  Function : ~CMtfTestActionSavePopSettings
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionSavePopSettings::~CMtfTestActionSavePopSettings()
	{
	}

/**
  Function : ExecuteActionL
  Description : Entry point for the this test action in the test framework
  @internalTechnology
  @param : none
  @return : void
  @pre none 
  @post none
*/
void CMtfTestActionSavePopSettings::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSavePopSettings);
	// Get input parameter
	TPopAccount popAccount = ObtainValueParameterL<TPopAccount>(TestCase(), ActionParameters().Parameter(0));
	
	TInt configFileIndex = ObtainValueParameterL<TInt>( TestCase(),ActionParameters().Parameter(1) );
	
	// Get the settings file name from the test case
	const TPtrC settingsFile = TestCase().GetConfigurationFileL(CMtfConfigurationType::EMtfPop3Settings, configFileIndex);
	
	CEmailAccounts* emailAccounts = CEmailAccounts::NewL();
	CleanupStack::PushL(emailAccounts);
	
	CImIAPPreferences* smtpPrefs = CImIAPPreferences::NewLC();
	
	CImSmtpSettings* smtpSettings = new(ELeave) CImSmtpSettings();
	CleanupStack::PushL(smtpSettings);
	
	CImIAPPreferences* popPrefs = CImIAPPreferences::NewLC();
	
	CImPop3Settings* popSettings = new(ELeave) CImPop3Settings();
	CleanupStack::PushL(popSettings);
	
	CMtfTestActionUtilsPopScripts::ReadPopSettingsFromConfigurationFileL(TestCase(), settingsFile, 
							*popSettings, *popPrefs, *smtpSettings, *smtpPrefs);
							
	emailAccounts->SavePopSettingsL(popAccount, *popSettings);
	emailAccounts->SavePopIapSettingsL(popAccount, *popPrefs);
	TSmtpAccount smtpAccount;
	emailAccounts->GetSmtpAccountL(popAccount.iSmtpService, smtpAccount);
	emailAccounts->SaveSmtpSettingsL(smtpAccount, *smtpSettings);
	emailAccounts->SaveSmtpIapSettingsL(smtpAccount, *smtpPrefs);	
	
	StoreParameterL<CImPop3Settings>(TestCase(), *popSettings, ActionParameters().Parameter(2));
	CleanupStack::Pop(popSettings);
	StoreParameterL<CImIAPPreferences>(TestCase(), *popPrefs, ActionParameters().Parameter(3));
	CleanupStack::Pop(popPrefs);
	StoreParameterL<CImSmtpSettings>(TestCase(), *smtpSettings, ActionParameters().Parameter(4));
	CleanupStack::Pop(smtpSettings);
	StoreParameterL<CImIAPPreferences>(TestCase(), *smtpPrefs, ActionParameters().Parameter(5));
	CleanupStack::Pop(smtpPrefs);
	CleanupStack::PopAndDestroy(emailAccounts);

	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSavePopSettings);
	TestCase().ActionCompletedL(*this); 
	}


