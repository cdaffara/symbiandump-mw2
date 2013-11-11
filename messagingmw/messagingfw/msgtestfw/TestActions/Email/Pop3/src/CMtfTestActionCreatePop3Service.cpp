// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// CreatePop3Service
// [Action Parameters]
// Session       <input>: Reference to the session.
// (Priority)     <input>: Value of the entry priority. Default is medium priority.
// (ReadOnlyFlag) <input>: Value of the read only flag. Default is false.
// (VisibleFlag)  <input>: Value of the visible flag. Default is true.
// (Description)  <input>: Name of the description.	Default is blank.
// (Details)      <input>: Name of the details. Default is blank.
// ServiceId     <output>: Value of the created service id.
// [Action Description]
// Creates a POP3 service.
// [APIs Used]
// TMsvEntry::iType
// TMsvEntry::SetPriority	
// TMsvEntry::iMtm	
// TMsvEntry::SetReadOnly	
// TMsvEntry::SetVisible	
// TMsvEntry::iDescription
// TMsvEntry::iDetails
// TMsvEntry::iDate
// TMsvEntry::Id
// CMsvEntry::SetEntryL
// CMsvEntry::CreateL
// __ACTION_INFO_END__
// 
//

/**
 @file
*/


#include <msvapi.h>
#include <miutset.h>

#include <cemailaccounts.h>
#include <iapprefs.h>
#include <pop3set.h>

#include "CMtfTestActionUtilsPopScripts.h"





#include "CMtfTestActionCreatePop3Service.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

CMtfTestAction* CMtfTestActionCreatePop3Service::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionCreatePop3Service* self = new (ELeave) CMtfTestActionCreatePop3Service(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionCreatePop3Service::CMtfTestActionCreatePop3Service(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionCreatePop3Service::~CMtfTestActionCreatePop3Service()
	{
	delete iBlank;
	}

void CMtfTestActionCreatePop3Service::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionCreatePop3Service);
	// need to create an account in the central repository and make it the default account
	HBufC* paramAccountName = ObtainParameterReferenceL<HBufC>( TestCase(),ActionParameters().Parameter(6) );
	HBufC* settingsFile = ObtainParameterReferenceL<HBufC>( TestCase(),ActionParameters().Parameter(7) );

	CEmailAccounts *emailAccounts = CEmailAccounts::NewLC();	
	CImIAPPreferences *smtpPrefs = CImIAPPreferences::NewLC();
	CImSmtpSettings *smtpSettings = new (ELeave) CImSmtpSettings;
	CleanupStack::PushL(smtpSettings);
	
	CImIAPPreferences *popPrefs = CImIAPPreferences::NewLC();
	CImPop3Settings *popSettings = new (ELeave) CImPop3Settings;
	CleanupStack::PushL(popSettings);

	emailAccounts->PopulateDefaultPopSettingsL(*popSettings, *popPrefs);
	emailAccounts->PopulateDefaultSmtpSettingsL(*smtpSettings, *smtpPrefs );
		
	if( settingsFile->Compare( _L("none") ) != 0 )
		{
		CMtfTestActionUtilsPopScripts::ReadPopSettingsFromConfigurationFileL(
			TestCase(), settingsFile->Des(), *popSettings, *popPrefs, *smtpSettings, *smtpPrefs);
		}
	
	TPopAccount popAccount = emailAccounts->CreatePopAccountL(paramAccountName->Des(), *popSettings, *popPrefs, EFalse);
	TSmtpAccount smtpAccount = emailAccounts->CreateSmtpAccountL(popAccount, *smtpSettings, *smtpPrefs, EFalse);
	emailAccounts->SetDefaultSmtpAccountL(smtpAccount);
	
	// need to obtain the service id from the account details and save this as the service id
	TMsvId paramServiceId = popAccount.iPopService;
	CleanupStack::PopAndDestroy(5, emailAccounts); // popSettings, popPrefs, smtpSettings, smtpPrefs, emailAccounts
	
	StoreParameterL<TMsvId>( TestCase(),paramServiceId, ActionParameters().Parameter(8) );
	
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionCreatePop3Service);
	TestCase().ActionCompletedL(*this);
	}

