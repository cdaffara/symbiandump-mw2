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
// SetSmtpServiceParameters
// [Action Parameters]
// Session                   <input>: Reference to the session.
// ServiceId                 <input>: Value of the SMTP service id.
// (ConfigIndexSettingsFile) <input>: Value of config index for settings file. Default is 0. 
// [Action Description]
// Sets parameters of Smtp service.
// [APIs Used]
// CMsvEntry::SetEntryL
// CMsvEntry::EditStoreL
// CImSmtpSettings::Reset
// CImSmtpSettings::SetServerAddressL
// CImSmtpSettings::SetEmailAddressL
// CImSmtpSettings::StoreL
// CMsvStore::CommitL
// __ACTION_INFO_END__
// 
//

/**
 @file
*/

#include <msvapi.h>
#include <smtpset.h>
#include  <cemailaccounts.h>


#include "CMtfTestActionSetSmtpServiceParameters.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "CMtfConfigurationType.h"



CMtfTestAction* CMtfTestActionSetSmtpServiceParameters::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSetSmtpServiceParameters* self = new (ELeave) CMtfTestActionSetSmtpServiceParameters(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionSetSmtpServiceParameters::CMtfTestActionSetSmtpServiceParameters(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionSetSmtpServiceParameters::~CMtfTestActionSetSmtpServiceParameters()
	{
	delete iParser;
	}


void CMtfTestActionSetSmtpServiceParameters::ExecuteActionL()
	{

// ifdef  __MESSAGING_API_V2__ should do nothing.


	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSetSmtpServiceParameters);
	TestCase().ActionCompletedL(*this);
	}
