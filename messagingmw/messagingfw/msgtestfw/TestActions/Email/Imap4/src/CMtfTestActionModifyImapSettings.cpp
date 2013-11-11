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
// ModifyImapSettings
// [Action Parameters]
// accountId			input		TIMAP4Account
// ImapSettings			output		CImImap4Settings
// ImapIAP				output		CImIAPPreferences
// SMTPSettings			output		CImSMTPSettings
// SmtpIAP				output		CImIAPPreferences
// [Action Description]
// ModifyImapSettings Test Action is intended to modify and save the existing settings to CenRep 
// for the specified IMAP account Id
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
// __ACTION_INFO_END__
// 
//




#include <imapset.h>
#include <smtpset.h>
#include <iapprefs.h>

#include <cemailaccounts.h>


#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "CMtfTestActionUtilsImapScripts.h"
#include "CMtfTestActionModifyImapSettings.h"


/**
  Function : NewL
  Description : 
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @param : aActionParams - CMtfTestActionParameters 
  @return : CMtfTestAction* - a base class pointer to the newly created CMtfTestActionModifyImapSettings object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionModifyImapSettings::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionModifyImapSettings* self = new (ELeave) CMtfTestActionModifyImapSettings(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function : CMtfTestActionModifyImapSettings
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return : N/A
  @pre none
  @post none
*/
CMtfTestActionModifyImapSettings::CMtfTestActionModifyImapSettings(CMtfTestCase& aTestCase) : CMtfSynchronousTestAction(aTestCase)
	{
	}
	
/**
  Function : ~CMtfTestActionModifyImapSettings
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionModifyImapSettings::~CMtfTestActionModifyImapSettings()
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
void CMtfTestActionModifyImapSettings::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionModifyImapSettings);
	TImapAccount accountId = ObtainValueParameterL<TImapAccount>( TestCase(),ActionParameters().Parameter(0) );

	HBufC* settingsFile = ObtainParameterReferenceL<HBufC>( TestCase(),ActionParameters().Parameter(1) );

	CEmailAccounts* emailAccounts=CEmailAccounts::NewLC();
	
	CImImap4Settings* imapSet = new(ELeave) CImImap4Settings();
	CleanupStack::PushL(imapSet);
	
	CImIAPPreferences* imapIapPref = CImIAPPreferences::NewLC();
	
	CImSmtpSettings* smtpSet = new(ELeave) CImSmtpSettings();
	CleanupStack::PushL(smtpSet);

	CImIAPPreferences* smtpIapPref = CImIAPPreferences::NewLC();

	// load the settings for the account with the current settings
	emailAccounts->LoadImapSettingsL(accountId, *imapSet);
	emailAccounts->LoadImapIapSettingsL(accountId, *imapIapPref);
	TSmtpAccount smtpAccount;
	emailAccounts->GetSmtpAccountL(accountId.iSmtpService, smtpAccount);
	emailAccounts->LoadSmtpSettingsL(smtpAccount, *smtpSet);
	emailAccounts->LoadSmtpIapSettingsL(smtpAccount, *smtpIapPref);
	
	// Read the settings from the file
	CMtfTestActionUtilsImapScripts::ReadImapSettingsFromConfigurationFileL( TestCase(), settingsFile->Des(), 
											*imapSet, *imapIapPref, *smtpSet, *smtpIapPref);
	
	// Save the new settings
	emailAccounts->SaveImapSettingsL(accountId, *imapSet);
	emailAccounts->SaveImapIapSettingsL(accountId, *imapIapPref);
	emailAccounts->SaveSmtpSettingsL(smtpAccount, *smtpSet);
	emailAccounts->SaveSmtpIapSettingsL(smtpAccount, *smtpIapPref);

	CleanupStack::PopAndDestroy(5, emailAccounts); // smtpIapPref, smtpSet, imapIapPref, imapSet
		
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionModifyImapSettings);

	TestCase().ActionCompletedL(*this);
	}


