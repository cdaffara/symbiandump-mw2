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
// CheckChildrenCount
// [Action Parameters]
// Session  <input>: Reference to the session.
// ParentId <input>: Value of the parent id.
// Count    <input>: Value of expected count.
// [Action Description]
// Checks if children count is as expected.
// [APIs Used]
// CMsvEntry::SetEntryL
// CMsvEntry::ChildrenL
// CMsvEntry::SetSortTypeL
// TMsvSelectionOrdering::SetShowInvisibleEntries
// __ACTION_INFO_END__
// 
//

/**
 @file
*/


#include "CMtfTestActionCheckChildrenCount.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

#include <msvapi.h>


CMtfTestAction* CMtfTestActionCheckChildrenCount::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionCheckChildrenCount* self = new (ELeave) CMtfTestActionCheckChildrenCount(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionCheckChildrenCount::CMtfTestActionCheckChildrenCount(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionCheckChildrenCount::~CMtfTestActionCheckChildrenCount()
	{
	}


void CMtfTestActionCheckChildrenCount::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionCheckChildrenCount);
	if((TestCase().TestStepResult()) == EPass)
		{
		CMsvSession* paramSession = ObtainParameterReferenceL<CMsvSession>(TestCase(),ActionParameters().Parameter(0));
		TMsvId paramParentId = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(1));
		TInt paramCount = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(2));

		CMsvEntry* entry = CMsvEntry::NewL(*paramSession,paramParentId,TMsvSelectionOrdering(KMsvNoGrouping,EMsvSortByNone,ETrue));
		CleanupStack::PushL(entry);
		entry->SetEntryL(paramParentId);

		TMsvSelectionOrdering order;
		order.SetShowInvisibleEntries(ETrue);
		entry->SetSortTypeL(order);

		CMsvEntrySelection* selection = entry->ChildrenL();
		CleanupStack::PushL(selection);
		
		TInt count = selection->Count();
		if (paramCount != count)
			User::Leave(KErrArgument);

		CleanupStack::PopAndDestroy(selection);
		CleanupStack::PopAndDestroy(entry);

		TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionCheckChildrenCount );
		}
	TestCase().ActionCompletedL(*this);
	}

