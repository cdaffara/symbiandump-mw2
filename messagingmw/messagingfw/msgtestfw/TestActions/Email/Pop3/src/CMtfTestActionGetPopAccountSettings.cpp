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
// LoadPopSettings
// [Action Parameters]
// accountId			input 			TPOP3Account	
// configFileIndex		input			Index of config file	
// PopSettings			output			CImPop3Settings
// PopIAP				output			CImIAPPreferences
// SMTPSettings			output			CImSMTPSettings
// SmtpIAP				output			CImIAPPreferences
// [Action Description]
// LoadPopSettings Test Action is intended to retrieve POP settings from 
// the central repository for the specified account Id.
// [APIs Used]	
// CEmailAccounts::LoadPopSettingsL
// __ACTION_INFO_END__
// 
//

#include <pop3set.h>
#include <smtpset.h>

#include <cemailaccounts.h>
#include <iapprefs.h>


#include "CMtfTestActionGetPopAccountSettings.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"


/**
  Function : NewL
  Description : 
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @param : aActionParams - CMtfTestActionParameters 
  @return : CMtfTestAction* - a base class pointer to the newly created CMtfTestActionGetPopAccountSettings object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionGetPopAccountSettings::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionGetPopAccountSettings* self = new (ELeave) CMtfTestActionGetPopAccountSettings(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function : CMtfTestActionGetPopAccountSettings
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return : N/A
  @pre none
  @post none
*/
CMtfTestActionGetPopAccountSettings::CMtfTestActionGetPopAccountSettings(CMtfTestCase& aTestCase) : CMtfSynchronousTestAction(aTestCase)
	{
	}

/**
  Function : ~CMtfTestActionGetPopAccountSettings
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionGetPopAccountSettings::~CMtfTestActionGetPopAccountSettings()
	{
	}

void CMtfTestActionGetPopAccountSettings::LoadSettingsL(CEmailAccounts& aAccounts, const TPopAccount& aAccount, CImPop3Settings& aPopSettings, CImIAPPreferences& aPopIapSettings, CImSmtpSettings& aSmtpSettings, CImIAPPreferences& aSmtpIapSettings)
	{
	aAccounts.LoadPopSettingsL(aAccount, aPopSettings);
	aAccounts.LoadPopIapSettingsL(aAccount, aPopIapSettings);
	TSmtpAccount smtpAccount;
	aAccounts.GetSmtpAccountL(aAccount.iSmtpService, smtpAccount);
	aAccounts.LoadSmtpSettingsL(smtpAccount, aSmtpSettings);
	aAccounts.LoadSmtpIapSettingsL(smtpAccount, aSmtpIapSettings);
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
void CMtfTestActionGetPopAccountSettings::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionGetPopAccountSettings);
	TPopAccount accountId = ObtainValueParameterL<TPopAccount>( TestCase(),ActionParameters().Parameter(0) );
	CEmailAccounts* accounts = CEmailAccounts::NewLC();
	CImIAPPreferences* smtpIapPref =CImIAPPreferences::NewLC();	
	CImSmtpSettings* smtpSet =  new (ELeave) CImSmtpSettings;
	CleanupStack::PushL(smtpSet);
	CImIAPPreferences* popIapPref = CImIAPPreferences::NewLC();
	CImPop3Settings* popSet =  new (ELeave) CImPop3Settings;
	CleanupStack::PushL(popSet);
	
	TRAPD( error, LoadSettingsL(*accounts, accountId, *popSet, *popIapPref, *smtpSet, *smtpIapPref) );
	
	if( error != KErrNone )
		{		
		TestCase().INFO_PRINTF2(_L("CMtfTestActionGetPopAccountSettings LoadPOPSettingsL Failed leave with code %d"), error );
		TestCase().SetTestStepResult(EFail);		
		}
	StoreParameterL<CImPop3Settings>(TestCase(), *popSet , ActionParameters().Parameter(1) );
	CleanupStack::Pop(popSet);
	StoreParameterL<CImIAPPreferences>(TestCase(), *popIapPref , ActionParameters().Parameter(2) );
	CleanupStack::Pop(popIapPref);
	StoreParameterL<CImSmtpSettings>(TestCase(), *smtpSet , ActionParameters().Parameter(3) );
	CleanupStack::Pop(smtpSet);
	StoreParameterL<CImIAPPreferences>(TestCase(), *smtpIapPref , ActionParameters().Parameter(4) );
	CleanupStack::Pop(smtpIapPref);
	CleanupStack::PopAndDestroy(accounts);
	
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionGetPopAccountSettings);
	
	TestCase().ActionCompletedL(*this);
	}


