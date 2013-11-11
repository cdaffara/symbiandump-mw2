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
// DeleteMmsService
// [Action Parameters]
// Session   <input>: Reference to session.
// ServiceId <input>: Value of MMS service id.
// [Action Description]
// Deletes the specified service.
// [APIs Used]
// none
// __ACTION_INFO_END__
// 
//

/**
 @file
*/


#include "CMtfTestActionDeleteMmsService.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

#include <msvapi.h>


CMtfTestAction* CMtfTestActionDeleteMmsService::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionDeleteMmsService* self = new (ELeave) CMtfTestActionDeleteMmsService(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionDeleteMmsService::CMtfTestActionDeleteMmsService(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionDeleteMmsService::~CMtfTestActionDeleteMmsService()
	{
	}


void CMtfTestActionDeleteMmsService::ExecuteActionL()
	{
	CMsvSession* paramSession = ObtainParameterReferenceL<CMsvSession>(TestCase(),ActionParameters().Parameter(0));
	TMsvId paramServiceId = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(1));

	CMsvEntry* entry = CMsvEntry::NewL(*paramSession,paramServiceId,TMsvSelectionOrdering());
	CleanupStack::PushL(entry);
	entry->SetEntryL(paramServiceId);
	entry->SetEntryL(entry->Entry().Parent());
	entry->DeleteL(paramServiceId);
	CleanupStack::PopAndDestroy(entry);
	DeleteParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(1));

	TestCase().ActionCompletedL(*this);
	}

