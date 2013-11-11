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
// CreatePopAccount
// [Action Parameters]
// accountName		<input>:	Name of the account to create
// configFile		<input>:	Configuration file to use for the settings
// accountId		<output>:	POP3 account ID
// popSettings		<output>:	POP3 settings object
// popIAPPrefs		<output>:	POP3 IAP Preferences
// smtpSettings		<output>:	SMTP settings object
// smtpIAPPrefs		<output>:	SMTP IAP Preferences
// [Action Description]
// CreatePopAccount Test Action is intended to create a POP account in the 
// Central Repository using the configuration settings from ini file.  
// This ini file includes both POP, SMTP settings and IAP preference settings 
// for both POP and SMTP.
// [APIs Used]	
// CEmailAccounts::CreatePopAccountL
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
// CImSmtpSettings::SetVersion
// CImIAPPreferences::Version
// CImIAPPreferences::AddIAPL
// __ACTION_INFO_END__
// 
//




#include <pop3set.h>
#include <smtpset.h>
#include <iapprefs.h>

#include <cemailaccounts.h>

#include "CMtfTestCase.h"
//#include "CMtfTestServer.h"
#include "CMtfTestActionParameters.h"
#include "CMtfTestActionCreatePopAccount.h"
#include "CMtfTestActionUtilsPopScripts.h"

/**
  Function : NewL
  Description : 
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @param : aActionParams - CMtfTestActionParameters 
  @return : CMtfTestAction* - a base class pointer to the newly created CMtfTestActionCreatePopAccount object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionCreatePopAccount::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionCreatePopAccount* self = new (ELeave) CMtfTestActionCreatePopAccount(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}
	
/**
  Function : CMtfTestActionCreatePopAccount
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return : N/A
  @pre none
  @post none
*/
CMtfTestActionCreatePopAccount::CMtfTestActionCreatePopAccount(CMtfTestCase& aTestCase) : CMtfSynchronousTestAction(aTestCase)
	{
	}
	
/**
  Function : ~CMtfTestActionCreatePopAccount
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionCreatePopAccount::~CMtfTestActionCreatePopAccount()
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
void CMtfTestActionCreatePopAccount::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionCreatePopAccount);

	HBufC* paramAccountName = ObtainParameterReferenceL<HBufC>( TestCase(),ActionParameters().Parameter(0) );
	HBufC* settingsFile = ObtainParameterReferenceL<HBufC>( TestCase(),ActionParameters().Parameter(1) );

	CEmailAccounts *emailAccounts = CEmailAccounts::NewL();
	CleanupStack::PushL(emailAccounts);
	
	CImIAPPreferences* smtpPrefs = CImIAPPreferences::NewLC();
	CImSmtpSettings* smtpSettings = new(ELeave) CImSmtpSettings();
	CleanupStack::PushL(smtpSettings);
	
	CImIAPPreferences* popPrefs = CImIAPPreferences::NewLC();
	CImPop3Settings* popSettings = new(ELeave) CImPop3Settings();
	CleanupStack::PushL(popSettings);

	emailAccounts->PopulateDefaultPopSettingsL(*popSettings, *popPrefs);
	emailAccounts->PopulateDefaultSmtpSettingsL(*smtpSettings, *smtpPrefs);
		
	if( settingsFile->CompareF( _L("none") ) != 0 )
		{
		CMtfTestActionUtilsPopScripts::ReadPopSettingsFromConfigurationFileL(
			TestCase(), settingsFile->Des(), *popSettings, *popPrefs, *smtpSettings, *smtpPrefs);
		}
	
	//Store settings for later comparison
	StoreParameterL<CImPop3Settings>(TestCase(), *popSettings , ActionParameters().Parameter(3) );	
	StoreParameterL<CImIAPPreferences>(TestCase(), *popPrefs , ActionParameters().Parameter(4) );
	StoreParameterL<CImSmtpSettings>(TestCase(), *smtpSettings , ActionParameters().Parameter(5) );
	StoreParameterL<CImIAPPreferences>(TestCase(), *smtpPrefs , ActionParameters().Parameter(6) );
	
	TPopAccount popAccount = emailAccounts->CreatePopAccountL(paramAccountName->Des(), *popSettings, *popPrefs, EFalse);
	TSmtpAccount smtpAccount = emailAccounts->CreateSmtpAccountL(popAccount, *smtpSettings, *smtpPrefs, EFalse);
	emailAccounts->SetDefaultSmtpAccountL(smtpAccount);	

	StoreParameterL<TPopAccount>(TestCase(), popAccount, ActionParameters().Parameter(2));	
	
  	CleanupStack::Pop(4, smtpPrefs); // popSettings, popPrefs, smtpSettings, smtpPrefs
  	CleanupStack::PopAndDestroy(emailAccounts);
	
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionCreatePopAccount);
	TestCase().ActionCompletedL(*this);
	}


