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
// CreateImapAccount
// [Action Parameters]
// configFile			input			TInt
// accountId 			output			TIMAP4Account
// ImapSettings			output			CImImap4Settings
// ImapIAP				output			CImIAPPreferences
// SMTPSettings			output			CImSMTPSettings
// SmtpIAP				output			CImIAPPreferences
// [Action Description]
// CreateImapAccount Test Action is intended to create an IMAP account in the 
// Central Repository using the configuration settings from ini file. This ini 
// file includes both IMAP, SMTP settings and IAP preference settings for both 
// IMAP and SMTP.
// [APIs Used]	
// CEmailAccounts::CreateImapAccountL
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

#include <cemailaccounts.h>

#include <iapprefs.h>

#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "CMtfTestActionUtilsImapScripts.h"
#include "CMtfTestActionCreateImapAccount.h"

/**
  Function : NewL
  Description : 
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @param : aActionParams - CMtfTestActionParameters 
  @return : CMtfTestAction* - a base class pointer to the newly created CMtfTestActionCreateImapAccount object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionCreateImapAccount::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionCreateImapAccount* self = new (ELeave) CMtfTestActionCreateImapAccount(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function : CMtfTestActionCreateImapAccount
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return : N/A
  @pre none
  @post none
*/
CMtfTestActionCreateImapAccount::CMtfTestActionCreateImapAccount(CMtfTestCase& aTestCase) : CMtfSynchronousTestAction(aTestCase)
	{
	}
	
/**
  Function : ~CMtfTestActionCreateImapAccount
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionCreateImapAccount::~CMtfTestActionCreateImapAccount()
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
void CMtfTestActionCreateImapAccount::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionCreateImapAccount);
	HBufC* paramAccountName =  ObtainParameterReferenceL<HBufC>(TestCase(),ActionParameters().Parameter(0) );		
	HBufC* settingsFile = ObtainParameterReferenceL<HBufC>( TestCase(),ActionParameters().Parameter(1) );
	
	// create the objects neccesary for creating IMAP settings in central repository
	CEmailAccounts *emailAccounts = CEmailAccounts::NewLC();
	
	CImImap4Settings* imapSet = new(ELeave) CImImap4Settings();
	CleanupStack::PushL(imapSet);
	CImIAPPreferences* imapIapPref = CImIAPPreferences::NewLC();	
	CImSmtpSettings* smtpSet = new(ELeave) CImSmtpSettings();
	CleanupStack::PushL(smtpSet);
	CImIAPPreferences* smtpIapPref = CImIAPPreferences::NewLC();
		
	emailAccounts->PopulateDefaultImapSettingsL(*imapSet, *imapIapPref);
	emailAccounts->PopulateDefaultSmtpSettingsL(*smtpSet, *smtpIapPref);
		
	if( settingsFile->CompareF( _L("none") ) != 0 )
		{
		CMtfTestActionUtilsImapScripts::ReadImapSettingsFromConfigurationFileL(
			TestCase(), settingsFile->Des(), *imapSet, *imapIapPref, *smtpSet, *smtpIapPref);
		}

	//Store settings for later comparison
	StoreParameterL<CImImap4Settings>(TestCase(), *imapSet , ActionParameters().Parameter(3) );	
	StoreParameterL<CImIAPPreferences>(TestCase(), *imapIapPref , ActionParameters().Parameter(4) );
	StoreParameterL<CImSmtpSettings>(TestCase(), *smtpSet , ActionParameters().Parameter(5) );
	StoreParameterL<CImIAPPreferences>(TestCase(), *smtpIapPref , ActionParameters().Parameter(6) );
			
	TImapAccount imapAccount = emailAccounts->CreateImapAccountL(*paramAccountName, *imapSet, *imapIapPref, EFalse);
	TSmtpAccount smtpAccount = emailAccounts->CreateSmtpAccountL(imapAccount, *smtpSet, *smtpIapPref, EFalse);
	emailAccounts->SetDefaultSmtpAccountL(smtpAccount);	
		
	StoreParameterL<TImapAccount>(TestCase(), imapAccount, ActionParameters().Parameter(2) );
	
  	CleanupStack::Pop(4, imapSet); // smtpIapPref, smtpSet, imapIapPref, imapSet
  	CleanupStack::PopAndDestroy(emailAccounts);
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionCreateImapAccount);		
	TestCase().ActionCompletedL(*this);	
	}


