// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// SetImap4ServiceLongIdleParameters
// [Action Parameters]
// Session                   <input>: Reference to the session.
// ServiceId                 <input>: Value of the Imap4 service id.
// (ConfigIndexSettingsFile) <input>: Value of config index for settings file. Default is 0. 
// [Action Description]
// Sets parameters of Imap4 service.
// [APIs Used]
// CMsvEntry::SetEntryL
// CMsvEntry::EditStoreL
// CImImap4Settings::Reset
// CImImap4Settings::SetServerAddressL
// CImImap4Settings::SetLoginNameL
// CImImap4Settings::SetPasswordL
// CImImap4Settings::StoreL
// CMsvStore::CommitL
// __ACTION_INFO_END__
// 
//

/**
 @file
*/

#include <msvapi.h>
#include <imapset.h>

#include <cemailaccounts.h>
#include <iapprefs.h>

#include "Cmtftestactionsetimap4serviceLongidleparameters.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "CMtfConfigurationType.h"

CMtfTestAction* CMtfTestActionSetImap4ServiceLongIdleParameters::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSetImap4ServiceLongIdleParameters* self = new (ELeave) CMtfTestActionSetImap4ServiceLongIdleParameters(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionSetImap4ServiceLongIdleParameters::CMtfTestActionSetImap4ServiceLongIdleParameters(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionSetImap4ServiceLongIdleParameters::~CMtfTestActionSetImap4ServiceLongIdleParameters()
	{
	delete iParser;
	}


void CMtfTestActionSetImap4ServiceLongIdleParameters::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSetImap4ServiceLongIdleParameters);
	CMsvSession* paramSession = 
		ObtainParameterReferenceL<CMsvSession>( TestCase(), ActionParameters().Parameter(0));
	TMsvId paramServiceId = 
		ObtainValueParameterL<TMsvId>( TestCase(), ActionParameters().Parameter(1));
	TBool paramIsImapIdleSelected = 
		ObtainValueParameterL<TBool>( TestCase(), ActionParameters().Parameter(2), ETrue);
	TInt paramImapIdleTimeout = 
		ObtainValueParameterL<TInt>( TestCase(), ActionParameters().Parameter(3), 1680);
	TInt paramImapSyncRate = ObtainValueParameterL<TInt>( TestCase(), ActionParameters().Parameter(4), 300);

	CImImap4Settings* settings = new(ELeave) CImImap4Settings();
	CleanupStack::PushL(settings);

 	CEmailAccounts *accounts = CEmailAccounts::NewLC();
 	TImapAccount imapAccount;
 	accounts->GetImapAccountL(paramServiceId, imapAccount);
	accounts->LoadImapSettingsL(imapAccount, *settings);

	settings->SetImapIdle(paramIsImapIdleSelected);
	settings->SetImapIdleTimeoutL(paramImapIdleTimeout);
	settings->SetSyncRateL(paramImapSyncRate);

	accounts->SaveImapSettingsL(imapAccount, *settings);
	CleanupStack::PopAndDestroy(2, settings);    

	TestCase().INFO_PRINTF1( _L("SetImap4ServiceLongIdleparameters completed") );
	TestCase().ActionCompletedL(*this);
	}

