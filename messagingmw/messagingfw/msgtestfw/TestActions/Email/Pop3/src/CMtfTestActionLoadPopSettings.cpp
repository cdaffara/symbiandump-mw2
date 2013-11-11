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
// accountId			input 			TPopAccount	
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


#include "CMtfTestActionLoadPopSettings.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"


/**
  Function : NewL
  Description : 
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @param : aActionParams - CMtfTestActionParameters 
  @return : CMtfTestAction* - a base class pointer to the newly created CMtfTestActionLoadPopSettings object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionLoadPopSettings::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionLoadPopSettings* self = new (ELeave) CMtfTestActionLoadPopSettings(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function : CMtfTestActionLoadPopSettings
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return : N/A
  @pre none
  @post none
*/
CMtfTestActionLoadPopSettings::CMtfTestActionLoadPopSettings(CMtfTestCase& aTestCase) : CMtfSynchronousTestAction(aTestCase)
	{
	}

/**
  Function : ~CMtfTestActionLoadPopSettings
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionLoadPopSettings::~CMtfTestActionLoadPopSettings()
	{
	}

void CMtfTestActionLoadPopSettings::LoadSettingsL(CEmailAccounts& aAccounts, const TPopAccount& aAccount, CImPop3Settings& aPopSettings, CImIAPPreferences& aPopIapSettings, CImSmtpSettings& aSmtpSettings, CImIAPPreferences& aSmtpIapSettings)
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
void CMtfTestActionLoadPopSettings::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionLoadPopSettings);
	TPopAccount accountId = ObtainValueParameterL<TPopAccount>( TestCase(),ActionParameters().Parameter(0) );
	TInt shouldFail = ObtainValueParameterL<TInt>( TestCase(),ActionParameters().Parameter(1) );

	CEmailAccounts* accounts = CEmailAccounts::NewLC();
	CImIAPPreferences* smtpIapPref = CImIAPPreferences::NewLC();	
	CImSmtpSettings* smtpSet =  new(ELeave) CImSmtpSettings();
	CleanupStack::PushL(smtpSet);
	CImIAPPreferences* popIapPref = CImIAPPreferences::NewLC();
	CImPop3Settings* popSet =  new(ELeave) CImPop3Settings();
	CleanupStack::PushL(popSet);
	
	// this will leave if an attempt is made to access non existent settings
	TRAPD( error, LoadSettingsL(*accounts, accountId, *popSet, *popIapPref, *smtpSet, *smtpIapPref) );
	
	if( error != KErrNone )
		{
		
		if( shouldFail == 1 && error == KErrNotFound )
			{
			TestCase().INFO_PRINTF1(_L("LoadPOPSettingsL Failed as expected ") );			
			}
		else
			{			
			TestCase().ERR_PRINTF2(_L("LoadPOPSettingsL Failed leave with code %d"), error );
			TestCase().SetTestStepResult(EFail);		
			}
			
		CleanupStack::PopAndDestroy( 5, accounts );
		}
	else
		{
		
		// params loaded ok so we can store them
		StoreParameterL<CImPop3Settings>(TestCase(), *popSet , ActionParameters().Parameter(2) );
		CleanupStack::Pop(popSet);
		StoreParameterL<CImIAPPreferences>(TestCase(), *popIapPref , ActionParameters().Parameter(3) );
		CleanupStack::Pop(popIapPref);
		StoreParameterL<CImSmtpSettings>(TestCase(), *smtpSet , ActionParameters().Parameter(4) );
		CleanupStack::Pop(smtpSet);
		StoreParameterL<CImIAPPreferences>(TestCase(), *smtpIapPref , ActionParameters().Parameter(5) );
		CleanupStack::Pop(smtpIapPref);

		CleanupStack::PopAndDestroy(accounts);		
		
		if( shouldFail == 1 )
			{
			// have managed to load settings ok when we should not have
			TestCase().ERR_PRINTF1(_L("CMtfTestActionLoadPopSettings loaded settings ok when we should not have") );
			TestCase().SetTestStepResult(EFail);					
			}
		
		}	
	
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionLoadPopSettings);

	TestCase().ActionCompletedL(*this);
	}


