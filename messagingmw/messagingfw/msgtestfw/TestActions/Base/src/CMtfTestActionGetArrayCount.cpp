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
// GetArrayCount
// [Action Parameters]
// CMsvEntrySelection  selection      <input>: Reference to the entry selection.
// TInt                result         <output-initiation>: Value of the Number of children.
// [Action Description]
// Returns the number of messages in a given selection.
// [APIs Used]
// none.
// __ACTION_INFO_END__
// 
//

/**
 @file
*/

#include <msvstd.h>

#include "CMtfTestActionGetArrayCount.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"


CMtfTestAction* CMtfTestActionGetArrayCount::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionGetArrayCount* self = new (ELeave) CMtfTestActionGetArrayCount(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionGetArrayCount::CMtfTestActionGetArrayCount(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionGetArrayCount::~CMtfTestActionGetArrayCount()
	{
	}


void CMtfTestActionGetArrayCount::ExecuteActionL()
	{
    TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionGetArrayCount);
	CMsvEntrySelection* paramSelection = ObtainParameterReferenceL<CMsvEntrySelection>(TestCase(),ActionParameters().Parameter(0));
	TInt totalChildren = paramSelection->Count();
	StoreParameterL<TInt>(TestCase(),totalChildren,ActionParameters().Parameter(1));
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionGetArrayCount);
	TestCase().ActionCompletedL(*this);
	}
