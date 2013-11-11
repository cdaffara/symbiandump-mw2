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
// SaveImapSettings
// [Action Parameters]
// accountId			input		TIMAP4Account
// ImapSettings			output		CImImap4Settings
// ImapIAP				output		CImIAPPreferences
// SMTPSettings			output		CImSMTPSettings
// SmtpIAP				output		CImIAPPreferences
// [Action Description]
// SaveImapSettings Test Action is intended to modify and save the existing settings to CenRep 
// for the specified IMAP account Id
// [APIs Used]	
// CEmailAccounts::SaveImapSettingsL
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
#include "CMtfTestActionSaveImapSettings.h"


/**
  Function : NewL
  Description : 
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @param : aActionParams - CMtfTestActionParameters 
  @return : CMtfTestAction* - a base class pointer to the newly created CMtfTestActionSaveImapSettings object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionSaveImapSettings::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSaveImapSettings* self = new (ELeave) CMtfTestActionSaveImapSettings(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function : CMtfTestActionSaveImapSettings
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return : N/A
  @pre none
  @post none
*/
CMtfTestActionSaveImapSettings::CMtfTestActionSaveImapSettings(CMtfTestCase& aTestCase) : CMtfSynchronousTestAction(aTestCase)
	{
	}
	
/**
  Function : ~CMtfTestActionSaveImapSettings
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionSaveImapSettings::~CMtfTestActionSaveImapSettings()
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
void CMtfTestActionSaveImapSettings::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSaveImapSettings);
	TImapAccount accountId = ObtainValueParameterL<TImapAccount>( TestCase(),ActionParameters().Parameter(0) );
	TInt paramDefaultIndex = ObtainValueParameterL<TInt>( TestCase(),ActionParameters().Parameter(1) );
	const TPtrC settingsFile = TestCase().GetConfigurationFileL(CMtfConfigurationType::EMtfImap4Settings, paramDefaultIndex);
	
	CImImap4Settings* imapSet = new(ELeave) CImImap4Settings();
	CleanupStack::PushL(imapSet);
	
	CImIAPPreferences* imapIapPref = CImIAPPreferences::NewLC();
	
	CImSmtpSettings* smtpSet = new(ELeave) CImSmtpSettings();
	CleanupStack::PushL(smtpSet);

	CImIAPPreferences* smtpIapPref = CImIAPPreferences::NewLC();
	
	CEmailAccounts* emailAccounts = CEmailAccounts::NewLC();			
	
	CMtfTestActionUtilsImapScripts::ReadImapSettingsFromConfigurationFileL( TestCase(), settingsFile, 
											*imapSet, *imapIapPref, *smtpSet, *smtpIapPref);
	
	emailAccounts->SaveImapSettingsL(accountId, *imapSet);
	emailAccounts->SaveImapIapSettingsL(accountId, *imapIapPref);
	TSmtpAccount smtpAccount;
	emailAccounts->GetSmtpAccountL(accountId.iSmtpService, smtpAccount);
	emailAccounts->SaveSmtpSettingsL(smtpAccount, *smtpSet);
	emailAccounts->SaveSmtpIapSettingsL(smtpAccount, *smtpIapPref);
	
	CleanupStack::PopAndDestroy(emailAccounts);
	
	StoreParameterL<CImIAPPreferences>(TestCase(), *smtpIapPref , ActionParameters().Parameter(5) );
	CleanupStack::Pop(smtpIapPref);
	
	StoreParameterL<CImSmtpSettings>(TestCase(), *smtpSet , ActionParameters().Parameter(4) );
	CleanupStack::Pop(smtpSet);
	
	StoreParameterL<CImIAPPreferences>(TestCase(), *imapIapPref , ActionParameters().Parameter(3) );
	CleanupStack::Pop(imapIapPref);

	StoreParameterL<CImImap4Settings>(TestCase(), *imapSet , ActionParameters().Parameter(2) );
	CleanupStack::Pop(imapSet);
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSaveImapSettings);
	TestCase().ActionCompletedL(*this);
	}


