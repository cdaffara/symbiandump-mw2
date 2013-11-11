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
// PopulatedefaultImapSettings
// [Action Parameters]
// accounted			input			TIMAP4Account
// ImapSettings			output			CImImap4Settings
// ImapIAP				output			CImIAPPreferences
// SMTPSettings			output			CImSMTPSettings
// SmtpIAP				output			CImIAPPreferences
// [Action Description]
// PopulateDefaultImapSettings Test Action is intended to fill the setting 
// objects with default values from account 0.
// [APIs Used]	
// CEmailAccounts::PopulateDefaultImapSettingsL
// __ACTION_INFO_END__
// 
//





#include <imapset.h>
#include <smtpset.h>
#include <iapprefs.h>



#include "CMtfTestCase.h"

#include <cemailaccounts.h>

#include "CMtfTestActionParameters.h"
#include "CMtfTestActionPopulateDefaultImapSettings.h"
#include "CMtfTestActionUtilsImapScripts.h"




/**
  Function : NewL
  Description : 
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @param : aActionParams - CMtfTestActionParameters 
  @return : CMtfTestAction* - a base class pointer to the newly created CMtfTestActionPopulateDefaultImapSettings object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionPopulateDefaultImapSettings::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionPopulateDefaultImapSettings* self = new (ELeave) CMtfTestActionPopulateDefaultImapSettings(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function : CMtfTestActionPopulateDefaultImapSettings
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return : N/A
  @pre none
  @post none
*/
CMtfTestActionPopulateDefaultImapSettings::CMtfTestActionPopulateDefaultImapSettings(CMtfTestCase& aTestCase) : CMtfSynchronousTestAction(aTestCase)
	{
	}
	
/**
  Function : ~CMtfTestActionPopulateDefaultImapSettings
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionPopulateDefaultImapSettings::~CMtfTestActionPopulateDefaultImapSettings()
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
void CMtfTestActionPopulateDefaultImapSettings::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionPopulateDefaultImapSettings);
	CEmailAccounts* emailAccounts=CEmailAccounts::NewLC();
			
	CImImap4Settings* imapSet =  new (ELeave) CImImap4Settings;
	CleanupStack::PushL(imapSet);
	
	CImIAPPreferences* imapIapPref = CImIAPPreferences::NewLC();
	
	CImSmtpSettings* smtpSet =  new (ELeave) CImSmtpSettings;
	CleanupStack::PushL(smtpSet);

	CImIAPPreferences* smtpIapPref	=  CImIAPPreferences::NewLC();
	
	emailAccounts->PopulateDefaultImapSettingsL(*imapSet, *imapIapPref);
	emailAccounts->PopulateDefaultSmtpSettingsL(*smtpSet, *smtpIapPref);
	
	StoreParameterL<CImIAPPreferences>(TestCase(), *smtpIapPref , ActionParameters().Parameter(3) );
	CleanupStack::Pop(smtpIapPref);
	
	StoreParameterL<CImSmtpSettings>(TestCase(), *smtpSet , ActionParameters().Parameter(2) );
	CleanupStack::Pop(smtpSet);
	
	StoreParameterL<CImIAPPreferences>(TestCase(), *imapIapPref , ActionParameters().Parameter(1) );
	CleanupStack::Pop(imapIapPref);
	
	StoreParameterL<CImImap4Settings>(TestCase(), *imapSet , ActionParameters().Parameter(0) );
	CleanupStack::Pop(imapSet);	

	CleanupStack::PopAndDestroy(emailAccounts);
	
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionPopulateDefaultImapSettings);
	TestCase().ActionCompletedL(*this);
	
	}


