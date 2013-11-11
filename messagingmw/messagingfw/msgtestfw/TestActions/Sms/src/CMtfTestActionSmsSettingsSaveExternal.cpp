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
// SmsSettingsSaveExternal
// [Action Parameters]
// CMsvSession&  Session    <input>: Reference to the session.
// TMsvId        ServiceId  <input>: Value of the SMS service id.
// [Action Description]
// Stores the SMS settings to the current context, and to the external file store.
// [APIs Used]
// CMsvSession::GetEntryL
// CMsvStore::EditStoreL
// CSmsSettings::RestoreL
// CSmsSettings::SaveExternalL
// __ACTION_INFO_END__
// 
//

/**
 @file
*/


#include <smutset.h>
#include <msvapi.h>

#include "CMtfTestActionSmsSettingsSaveExternal.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
//#include "MtfTestScripts.h"


CMtfTestAction* CMtfTestActionSmsSettingsSaveExternal::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSmsSettingsSaveExternal* self = new (ELeave) CMtfTestActionSmsSettingsSaveExternal(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionSmsSettingsSaveExternal::CMtfTestActionSmsSettingsSaveExternal(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionSmsSettingsSaveExternal::~CMtfTestActionSmsSettingsSaveExternal()
	{
	}

void CMtfTestActionSmsSettingsSaveExternal::ExecuteActionL()
	{
	
	TestCase().INFO_PRINTF1(_L("This API is removed from Messaging API V2. Test action does nothing."));
	

	TestCase().ActionCompletedL(*this);
	}


