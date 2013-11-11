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
// GetEntryPriorityEnums
// [Action Parameters]
// HighPriority   <output>: Value of the high priority.
// MediumPriority <output>: Value of the medium priority.
// LowPriority    <output>: Value of the low priority.
// [Action Description]
// Returns entry priorities.
// [APIs Used]
// none.
// __ACTION_INFO_END__
// 
//

/**
 @file
*/

#include "CMtfTestActionGetEntryPriorityEnums.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

#include <msvstd.h>


CMtfTestAction* CMtfTestActionGetEntryPriorityEnums::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionGetEntryPriorityEnums* self = new (ELeave) CMtfTestActionGetEntryPriorityEnums(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionGetEntryPriorityEnums::CMtfTestActionGetEntryPriorityEnums(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionGetEntryPriorityEnums::~CMtfTestActionGetEntryPriorityEnums()
	{
	}


void CMtfTestActionGetEntryPriorityEnums::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionGetEntryPriorityEnums);
	TMsvPriority paramHighPriority = EMsvHighPriority;
	StoreParameterL<TMsvPriority>(TestCase(),paramHighPriority,ActionParameters().Parameter(0));

	TMsvPriority paramMediumPriority = EMsvMediumPriority;
	StoreParameterL<TMsvPriority>(TestCase(),paramMediumPriority,ActionParameters().Parameter(1));

	TMsvPriority paramLowPriority = EMsvLowPriority;
	StoreParameterL<TMsvPriority>(TestCase(),paramLowPriority,ActionParameters().Parameter(2));

	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionGetEntryPriorityEnums);
	TestCase().ActionCompletedL(*this);
	}


