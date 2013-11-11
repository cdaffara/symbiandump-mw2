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
// LoadImapSettings
// [Action Parameters]
// accountId			input		TIMAP4Account
// ImapSettings			output		CImImap4Settings
// ImapIAP				output		CImIAPPreferences	
// SMTPSettings			output		CImSMTPSettings	
// SmtpIAP				output		CImIAPPreferences	
// [Action Description]
// LoadImapSettings Test Action is intended to retrieve IMAP settings 
// from CenRep for the specified account Id.
// [APIs Used]	
// CEmailAccounts::LoadImapSettingsL
// CImImap4Settings::SetServerAddressL
// CImImap4Settings::SetLoginNameL
// CImImap4Settings::SetPasswordL
// CImSmtpSettings::SetServerAddressL
// CImSmtpSettings::SetEmailAddressL
// CImIAPPreferences::Version
// CImIAPPreferences::AddIAPL
// __ACTION_INFO_END__
// 
//


#include <cemailaccounts.h>

#include <imapset.h>
#include <smtpset.h>
#include <iapprefs.h>

#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "CMtfTestActionLoadImapSettings.h"

/**
  Function : NewL
  Description : 
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @param : aActionParams - CMtfTestActionParameters 
  @return : CMtfTestAction* - a base class pointer to the newly created CMtfTestActionLoadImapSettings object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionLoadImapSettings::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionLoadImapSettings* self = new (ELeave) CMtfTestActionLoadImapSettings(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}
	
/**
  Function : CMtfTestActionLoadImapSettings
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return : N/A
  @pre none
  @post none
*/
CMtfTestActionLoadImapSettings::CMtfTestActionLoadImapSettings(CMtfTestCase& aTestCase) : CMtfSynchronousTestAction(aTestCase)
	{
	}

/**
  Function : ~CMtfTestActionLoadImapSettings
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionLoadImapSettings::~CMtfTestActionLoadImapSettings()
	{
	}

void CMtfTestActionLoadImapSettings::LoadAccountSettingsL(CEmailAccounts& aEmailAccount, const TImapAccount& aAccount, CImImap4Settings& aImapSettings, CImIAPPreferences& aImapIapSettings, CImSmtpSettings& aSmtpSettings, CImIAPPreferences& aSmtpIapSettings)
	{
	aEmailAccount.LoadImapSettingsL(aAccount, aImapSettings);
	aEmailAccount.LoadImapIapSettingsL(aAccount, aImapIapSettings);
	
	TSmtpAccount smtpAccount;
	aEmailAccount.GetSmtpAccountL(aAccount.iSmtpService, smtpAccount);
	aEmailAccount.LoadSmtpSettingsL(smtpAccount, aSmtpSettings);
	aEmailAccount.LoadSmtpIapSettingsL(smtpAccount, aSmtpIapSettings);
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
void CMtfTestActionLoadImapSettings::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionLoadImapSettings);
	// load the previously stored account id
	TImapAccount accountId = ObtainValueParameterL<TImapAccount>( TestCase(),ActionParameters().Parameter(0) );
	TInt shouldFail = ObtainValueParameterL<TInt>( TestCase(),ActionParameters().Parameter(1) );

	// create the objects neccessary for loading the settings
	CEmailAccounts* emailAccounts = CEmailAccounts::NewLC();
	CImImap4Settings* imapSet = new(ELeave) CImImap4Settings();
	CleanupStack::PushL(imapSet);
	CImIAPPreferences* imapIapPref = CImIAPPreferences::NewLC();
	CImSmtpSettings* smtpSet = new(ELeave) CImSmtpSettings();
	CleanupStack::PushL(smtpSet);
	CImIAPPreferences* smtpIapPref = CImIAPPreferences::NewLC();

	
	// load the settings
	// this will leave if an attempt is made to access non existent settings
	TRAPD( error, LoadAccountSettingsL(*emailAccounts, accountId, *imapSet, *imapIapPref, *smtpSet, *smtpIapPref) );
	
	if( error != KErrNone )
		{
		
		if( shouldFail == 1 && error == KErrNotFound )
			{
			TestCase().INFO_PRINTF1(_L("LoadIMAPSettingsL Failed as expected ") );			
			}
		else
			{			
			TestCase().ERR_PRINTF2(_L("LoadIMAPSettingsL Failed leave with code %d"), error );
			TestCase().SetTestStepResult(EFail);		
			}
			
		CleanupStack::PopAndDestroy( 5, emailAccounts );
		}
	else
		{

		// params loaded ok so we can store them	
		StoreParameterL<CImIAPPreferences>(TestCase(), *smtpIapPref , ActionParameters().Parameter(5) );
		CleanupStack::Pop(smtpIapPref);
	
		StoreParameterL<CImSmtpSettings>(TestCase(), *smtpSet , ActionParameters().Parameter(4) );
		CleanupStack::Pop(smtpSet);

		StoreParameterL<CImIAPPreferences>(TestCase(), *imapIapPref , ActionParameters().Parameter(3) );
		CleanupStack::Pop(imapIapPref);
	
		StoreParameterL<CImImap4Settings>(TestCase(), *imapSet , ActionParameters().Parameter(2) );
		CleanupStack::Pop(imapSet);
		
		CleanupStack::PopAndDestroy(emailAccounts);
		
		if( shouldFail == 1 )
			{
			// have managed to load settings ok when we should not have
			TestCase().ERR_PRINTF1(_L("CMtfTestActionLoadImapSettings loaded settings ok when we should not have") );
			TestCase().SetTestStepResult(EFail);					
			}
		
		}	
	
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionLoadImapSettings);
	TestCase().ActionCompletedL(*this);
	}


