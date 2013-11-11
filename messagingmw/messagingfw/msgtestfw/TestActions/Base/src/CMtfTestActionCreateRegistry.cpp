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
// CreateRegistry
// [Action Parameters]
// Session      <input>: Reference to the session.
// MtmRegistry <output>: Reference to the client MTM registry.
// [Action Description]
// Creates the client MTM registry.
// [APIs Used]
// none
// __ACTION_INFO_END__
// 
//

/**
 @file
*/


#include "CMtfTestActionCreateRegistry.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

#include <msvapi.h>
#include <mtclreg.h>


CMtfTestAction* CMtfTestActionCreateRegistry::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionCreateRegistry* self = new (ELeave) CMtfTestActionCreateRegistry(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionCreateRegistry::CMtfTestActionCreateRegistry(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionCreateRegistry::~CMtfTestActionCreateRegistry()
	{
	}


void CMtfTestActionCreateRegistry::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionCreateRegistry);
	CMsvSession* paramSession = ObtainParameterReferenceL<CMsvSession>(TestCase(),ActionParameters().Parameter(0));

	CClientMtmRegistry* paramMtmRegistry = CClientMtmRegistry::NewL(*paramSession);
	CleanupStack::PushL(paramMtmRegistry);
	StoreParameterL<CClientMtmRegistry>(TestCase(),*paramMtmRegistry,ActionParameters().Parameter(1));
	CleanupStack::Pop(paramMtmRegistry);

	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionCreateRegistry);
	TestCase().ActionCompletedL(*this);
	}

