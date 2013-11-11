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
// SetPop3ServiceParameters
// [Action Parameters]
// Session    <input>: Reference to the session.
// ServiceId  <input>: Value of the POP3 service id.
// (ConfigIndexSettingsFile) <input>: Value of config index for settings file. Default is 0. 
// [Action Description]
// Sets parameters of Pop3 service.
// [APIs Used]
// CMsvEntry::SetEntryL
// CMsvEntry::EditStoreL
// CImPop3Settings::Reset
// CImPop3Settings::SetServerAddressL
// CImPop3Settings::SetLoginNameL
// CImPop3Settings::SetPasswordL
// CImPop3Settings::SetAutoSendOnConnect
// CImPop3Settings::StoreL
// CMsvStore::CommitL
// __ACTION_INFO_END__
// 
//

/**
 @file
*/



#include <msvapi.h>
#include <pop3set.h>
#include  <cemailaccounts.h>


#include "CMtfTestActionSetPop3ServiceParameters.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "CMtfTestActionUtilsPopScripts.h"
#include "CMtfConfigurationType.h"


CMtfTestAction* CMtfTestActionSetPop3ServiceParameters::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSetPop3ServiceParameters* self = new (ELeave) CMtfTestActionSetPop3ServiceParameters(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionSetPop3ServiceParameters::CMtfTestActionSetPop3ServiceParameters(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionSetPop3ServiceParameters::~CMtfTestActionSetPop3ServiceParameters()
	{
	delete iParser;
	}



void CMtfTestActionSetPop3ServiceParameters::ExecuteActionL()
	{


// ifdef  __MESSAGING_API_V2__ should do nothing.


	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSetPop3ServiceParameters);
	TestCase().ActionCompletedL(*this);
	}
