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

#include "CMtfTestActionModifyPopSettings.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "CMtfTestActionUtilsPopScripts.h"


/**
  Function : NewL
  Description : 
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @param : aActionParams - CMtfTestActionParameters 
  @return : CMtfTestAction* - a base class pointer to the newly created CMtfTestActionModifyPopSettings object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionModifyPopSettings::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionModifyPopSettings* self = new (ELeave) CMtfTestActionModifyPopSettings(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function : CMtfTestActionModifyPopSettings
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return : N/A
  @pre none
  @post none
*/
CMtfTestActionModifyPopSettings::CMtfTestActionModifyPopSettings(CMtfTestCase& aTestCase) : CMtfSynchronousTestAction(aTestCase)
	{
	}

/**
  Function : ~CMtfTestActionModifyPopSettings
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionModifyPopSettings::~CMtfTestActionModifyPopSettings()
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
void CMtfTestActionModifyPopSettings::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionModifyPopSettings);
	TPopAccount popAccount = ObtainValueParameterL<TPopAccount>(TestCase(), ActionParameters().Parameter(0));
	
	HBufC* settingsFile = ObtainParameterReferenceL<HBufC>( TestCase(),ActionParameters().Parameter(1) );
	
	CEmailAccounts* emailAccounts = CEmailAccounts::NewL();
	CleanupStack::PushL(emailAccounts);
	
	CImIAPPreferences* smtpPrefs = CImIAPPreferences::NewLC();
	CImSmtpSettings* smtpSettings = new(ELeave) CImSmtpSettings();
	CleanupStack::PushL(smtpSettings);
	
	CImIAPPreferences* popPrefs = CImIAPPreferences::NewLC();
	
	CImPop3Settings* popSettings = new(ELeave) CImPop3Settings();
	CleanupStack::PushL(popSettings);
	
	// populate the default settings to make sure that they contain no empty strings
	emailAccounts->PopulateDefaultPopSettingsL(*popSettings, *popPrefs);
	emailAccounts->PopulateDefaultSmtpSettingsL(*smtpSettings, *smtpPrefs);

	CMtfTestActionUtilsPopScripts::ReadPopSettingsFromConfigurationFileL(TestCase(), settingsFile->Des(), 
							*popSettings, *popPrefs, *smtpSettings, *smtpPrefs);
							
													
	emailAccounts->SavePopSettingsL(popAccount, *popSettings);
	emailAccounts->SavePopIapSettingsL(popAccount, *popPrefs);
	TSmtpAccount smtpAccount;
	emailAccounts->GetSmtpAccountL(popAccount.iSmtpService, smtpAccount);
	emailAccounts->SaveSmtpSettingsL(smtpAccount, *smtpSettings);
	emailAccounts->SaveSmtpIapSettingsL(smtpAccount, *smtpPrefs);
	
	CleanupStack::PopAndDestroy( 5, emailAccounts ); 
		
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionModifyPopSettings);
	TestCase().ActionCompletedL(*this); 
	}


