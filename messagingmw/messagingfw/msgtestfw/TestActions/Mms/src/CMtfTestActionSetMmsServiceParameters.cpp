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
// SetMmsServiceParameters
// [Action Parameters]
// Session                   <input>: Reference to the session.
// ServiceId                 <input>: Value of the Mms service id.
// (ConfigIndexSettingsFile) <input>: Value of config index for settings file. Default is 0. 
// [Action Description]
// Sets parameters of Mms service.
// [APIs Used]
// CMsvEntry::SetEntryL
// CMsvEntry::EditStoreL
// CImMmsSettings::Reset
// CImMmsSettings::SetServerAddressL
// CImMmsSettings::SetEmailAddressL
// CImMmsSettings::StoreL
// CMsvStore::CommitL
// __ACTION_INFO_END__
// 
//

/**
 @file
*/


#include "CMtfTestActionSetMmsServiceParameters.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "CMtfConfigurationType.h"

#include <msvapi.h>
#include <MmsSettings.h>

_LIT(KServerName,"ServerName");


CMtfTestAction* CMtfTestActionSetMmsServiceParameters::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSetMmsServiceParameters* self = new (ELeave) CMtfTestActionSetMmsServiceParameters(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionSetMmsServiceParameters::CMtfTestActionSetMmsServiceParameters(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionSetMmsServiceParameters::~CMtfTestActionSetMmsServiceParameters()
	{
	delete iParser;
	}


void CMtfTestActionSetMmsServiceParameters::ExecuteActionL()
	{
	CMsvSession* paramSession = ObtainParameterReferenceL<CMsvSession>(TestCase(),ActionParameters().Parameter(0));
	TMsvId paramServiceId = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(1));
	TInt paramConfigIndex = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(2),0);

	CMsvEntry* entry = CMsvEntry::NewL(*paramSession,paramServiceId,TMsvSelectionOrdering());
	CleanupStack::PushL(entry);
	entry->SetEntryL(paramServiceId);
	CMsvStore* store = entry->EditStoreL();
	CleanupStack::PushL(store);
	CMmsSettings* settings = CMmsSettings::NewL();
	CleanupStack::PushL(settings);
	TPtrC fileName = TestCase().GetConfigurationFileL(CMtfConfigurationType::EMtfMmsSettings,paramConfigIndex);
	iParser = CMtfTestActionUtilsConfigFileParser::NewL(fileName);
	
	// temporary - READ THOSE PARAMS FROM SETTINGS
	settings->SetProtocolL(EMmsWspProtocol);
	settings->CommsSettings().AddWapBearerPrefL(2);

	TPtrC serverName;
	User::LeaveIfError(iParser->GetFieldAsString(KServerName,serverName));
	settings->SetServerAddressL(serverName);

	settings->StoreL(*store);
	store->CommitL();
	CleanupStack::PopAndDestroy(settings);
	CleanupStack::PopAndDestroy(store);
	CleanupStack::PopAndDestroy(entry);

	TestCase().ActionCompletedL(*this);
	}
