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
// CreateImap4Service
// [Action Parameters]
// Session        <input>: Reference to the session.
// (Priority)     <input>: Value of the entry priority. Default is medium priority.
// (ReadOnlyFlag) <input>: Value of the read only flag. Default is false.
// (VisibleFlag)  <input>: Value of the visible flag. Default is true.
// (Description)  <input>: Name of the description.	Default is blank.
// (Details)      <input>: Name of the details. Default is blank.
// ServiceId     <output>: Value of the created service id.
// [Action Description]
// Creates an IMAP4 service.
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

#include "CMtfTestActionUtilsImapScripts.h"


#include "CMtfTestActionCreateImap4Service.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"



CMtfTestAction* CMtfTestActionCreateImap4Service::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionCreateImap4Service* self = new (ELeave) CMtfTestActionCreateImap4Service(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionCreateImap4Service::CMtfTestActionCreateImap4Service(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionCreateImap4Service::~CMtfTestActionCreateImap4Service()
	{
	delete iBlank;
	}




void CMtfTestActionCreateImap4Service::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionCreateImap4Service);
	// need to create an account in the central repository and make it the default account
	HBufC* paramAccountName = ObtainParameterReferenceL<HBufC>( TestCase(),ActionParameters().Parameter(6) );
	// settings file contains idle params
	HBufC* settingsFile = ObtainParameterReferenceL<HBufC>( TestCase(),ActionParameters().Parameter(7) );

	CEmailAccounts* emailAccounts = CEmailAccounts::NewLC();	
	CImIAPPreferences* smtpPrefs = CImIAPPreferences::NewLC();
	CImSmtpSettings* smtpSettings = new(ELeave) CImSmtpSettings();
	CleanupStack::PushL(smtpSettings);
	
	CImIAPPreferences* imapPrefs = CImIAPPreferences::NewLC();
	CImImap4Settings* imapSettings = new(ELeave) CImImap4Settings();
	CleanupStack::PushL(imapSettings);

	emailAccounts->PopulateDefaultImapSettingsL(*imapSettings, *imapPrefs);
	emailAccounts->PopulateDefaultSmtpSettingsL(*smtpSettings, *smtpPrefs);
		
	if( settingsFile->Compare( _L("none") ) != 0 )
		{
		CMtfTestActionUtilsImapScripts::ReadImapSettingsFromConfigurationFileL(
			TestCase(), settingsFile->Des(), *imapSettings, *imapPrefs, *smtpSettings, *smtpPrefs);
		}
	
	// create the IMAP account and make it the default
	TImapAccount imapAccount = emailAccounts->CreateImapAccountL(paramAccountName->Des(), *imapSettings, *imapPrefs, EFalse);
	TSmtpAccount smtpAccount = emailAccounts->CreateSmtpAccountL(imapAccount, *smtpSettings, *smtpPrefs, EFalse);
	emailAccounts->SetDefaultSmtpAccountL(smtpAccount);
	
	// need to obtain the service id and save it as a param to maintain compatability
	TMsvId serviceId = imapAccount.iImapService;
	CleanupStack::PopAndDestroy( 5, emailAccounts );
	

	StoreParameterL<TMsvId>(TestCase(),serviceId,ActionParameters().Parameter(8));
	
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionCreateImap4Service );
	TestCase().ActionCompletedL(*this);
	
	}
