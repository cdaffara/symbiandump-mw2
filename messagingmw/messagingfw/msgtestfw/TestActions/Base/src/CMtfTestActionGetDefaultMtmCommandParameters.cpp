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
// GetDefaultMtmCommandParameters
// [Action Parameters]
// CommandParameters <output>: Reference to the command parameters.
// [Action Description]
// Returns the default MTM default command parameters.
// [APIs Used]
// none.
// __ACTION_INFO_END__
// 
//

/**
 @file
*/

#include "CMtfTestActionGetDefaultMtmCommandParameters.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"


CMtfTestAction* CMtfTestActionGetDefaultMtmCommandParameters::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionGetDefaultMtmCommandParameters* self = new (ELeave) CMtfTestActionGetDefaultMtmCommandParameters(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionGetDefaultMtmCommandParameters::CMtfTestActionGetDefaultMtmCommandParameters(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionGetDefaultMtmCommandParameters::~CMtfTestActionGetDefaultMtmCommandParameters()
	{
	}


void CMtfTestActionGetDefaultMtmCommandParameters::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionGetDefaultMtmCommandParameters);
	HBufC8* paramCommandParameters = HBufC8::NewL(1);
	CleanupStack::PushL(paramCommandParameters);
	StoreParameterL<HBufC8>(TestCase(),*paramCommandParameters,ActionParameters().Parameter(0));
	CleanupStack::Pop(paramCommandParameters);

	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionGetDefaultMtmCommandParameters);
	TestCase().ActionCompletedL(*this);
	}


