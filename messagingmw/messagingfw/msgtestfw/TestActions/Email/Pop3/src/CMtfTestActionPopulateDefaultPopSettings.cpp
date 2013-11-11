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
// PopulateDefaultPopSettings
// [Action Parameters]
// PopSettings			output		CImPop4Settings
// PopIAP				output		CImIAPPreferences
// SMTPSettings		output		CImSMTPSettings
// SmtpIAP				output		CImIAPPreferences
// [Action Description]
// PopulateDefaultPopSettings Test Action is intended to fill the setting objects 
// with default values from account 0.
// [APIs Used]	
// CEmailAccounts::PopulateDefaultPopSettingsL
// __ACTION_INFO_END__
// 
//


#include <pop3set.h>
#include <smtpset.h>

#include <cemailaccounts.h>
#include <iapprefs.h>



#include "CMtfTestActionPopulateDefaultPopSettings.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "CMtfTestActionUtilsPopScripts.h"



/**
  Function : NewL
  Description : 
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @param : aActionParams - CMtfTestActionParameters 
  @return : CMtfTestAction* - a base class pointer to the newly created CMtfTestActionPopulateDefaultPopSettings object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionPopulateDefaultPopSettings::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionPopulateDefaultPopSettings* self = new (ELeave) CMtfTestActionPopulateDefaultPopSettings(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function : ~CMtfTestActionPopulateDefaultPopSettings
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionPopulateDefaultPopSettings::~CMtfTestActionPopulateDefaultPopSettings()
	{
	}

/**
  Function : CMtfTestActionPopulateDefaultPopSettings
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return : N/A
  @pre none
  @post none
*/
CMtfTestActionPopulateDefaultPopSettings::CMtfTestActionPopulateDefaultPopSettings(CMtfTestCase& aTestCase) : CMtfSynchronousTestAction(aTestCase)
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
void CMtfTestActionPopulateDefaultPopSettings::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionPopulateDefaultPopSettings);
	CEmailAccounts* emailAccounts = CEmailAccounts::NewL();
	CleanupStack::PushL(emailAccounts);
	
	CImIAPPreferences* smtpPrefs = CImIAPPreferences::NewLC();
	
	CImSmtpSettings* smtpSettings = new(ELeave) CImSmtpSettings();
	CleanupStack::PushL(smtpSettings);
	
	CImIAPPreferences* popPrefs = CImIAPPreferences::NewLC();
	
	CImPop3Settings* popSettings = new(ELeave) CImPop3Settings();
	CleanupStack::PushL(popSettings);
	
	emailAccounts->PopulateDefaultPopSettingsL(*popSettings, *popPrefs);
	emailAccounts->PopulateDefaultSmtpSettingsL(*smtpSettings, *smtpPrefs);
	
	StoreParameterL<CImPop3Settings>(TestCase(), *popSettings, ActionParameters().Parameter(0));
	CleanupStack::Pop(popSettings);
	
	StoreParameterL<CImIAPPreferences>(TestCase(), *popPrefs, ActionParameters().Parameter(1));
	CleanupStack::Pop(popPrefs);
	
	StoreParameterL<CImSmtpSettings>(TestCase(), *smtpSettings, ActionParameters().Parameter(2));
	CleanupStack::Pop(smtpSettings);
	
	StoreParameterL<CImIAPPreferences>(TestCase(), *smtpPrefs, ActionParameters().Parameter(3));
	CleanupStack::Pop(smtpPrefs);
	
	CleanupStack::PopAndDestroy(emailAccounts);

	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionPopulateDefaultPopSettings);
	TestCase().ActionCompletedL(*this);
	}


