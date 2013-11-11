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
// DeleteSession
// [Action Parameters]
// MtmRegistry <input>: Reference to the client MTM registry.
// [Action Description]
// Deletes the specified client MTM registry.
// [APIs Used]
// none
// __ACTION_INFO_END__
// 
//

/**
 @file
*/


#include "CMtfTestActionDeleteRegistry.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

#include <mtclreg.h>


CMtfTestAction* CMtfTestActionDeleteRegistry::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionDeleteRegistry* self = new (ELeave) CMtfTestActionDeleteRegistry(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionDeleteRegistry::CMtfTestActionDeleteRegistry(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionDeleteRegistry::~CMtfTestActionDeleteRegistry()
	{
	}


void CMtfTestActionDeleteRegistry::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionDeleteRegistry);
	DeleteParameterL<CClientMtmRegistry>(TestCase(),ActionParameters().Parameter(0));

	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionDeleteRegistry);
	TestCase().ActionCompletedL(*this);
	}

