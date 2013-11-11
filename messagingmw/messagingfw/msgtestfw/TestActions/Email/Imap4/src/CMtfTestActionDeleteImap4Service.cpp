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
// DeleteImap4Service
// [Action Parameters]
// Session   <input>: Reference to session.
// ServiceId <input>: Value of IMAP4 service id.
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


#include "CMtfTestActionDeleteImap4Service.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

#include <msvapi.h>


CMtfTestAction* CMtfTestActionDeleteImap4Service::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionDeleteImap4Service* self = new (ELeave) CMtfTestActionDeleteImap4Service(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionDeleteImap4Service::CMtfTestActionDeleteImap4Service(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionDeleteImap4Service::~CMtfTestActionDeleteImap4Service()
	{
	}


void CMtfTestActionDeleteImap4Service::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionDeleteImap4Service);
	CMsvSession* paramSession = ObtainParameterReferenceL<CMsvSession>(TestCase(),ActionParameters().Parameter(0));
	TMsvId paramServiceId = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(1));

	CMsvEntry* entry = CMsvEntry::NewL(*paramSession,paramServiceId,TMsvSelectionOrdering());
	CleanupStack::PushL(entry);
	entry->SetEntryL(paramServiceId);
	entry->SetEntryL(entry->Entry().Parent());
	entry->DeleteL(paramServiceId);
	CleanupStack::PopAndDestroy(entry);
	DeleteParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(1));

	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionDeleteImap4Service);
	TestCase().ActionCompletedL(*this);
	}

