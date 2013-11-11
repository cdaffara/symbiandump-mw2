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
// CreateChildrenSelection
// [Action Parameters]
// Session    <input>: Reference to the session.
// ParentId   <input>: Value of the parent id.
// Selection <output>: Reference to the created selection.
// [Action Description]
// Creates a selection of all children.
// [APIs Used]
// none.
// __ACTION_INFO_END__
// 
//

/**
 @file
*/

#include "CMtfTestActionCreateChildrenSelection.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

#include <msvapi.h>


CMtfTestAction* CMtfTestActionCreateChildrenSelection::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionCreateChildrenSelection* self = new (ELeave) CMtfTestActionCreateChildrenSelection(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionCreateChildrenSelection::CMtfTestActionCreateChildrenSelection(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionCreateChildrenSelection::~CMtfTestActionCreateChildrenSelection()
	{
	}


void CMtfTestActionCreateChildrenSelection::ExecuteActionL()
	{
	if((TestCase().TestStepResult()) == EPass)
		{
		TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionCreateChildrenSelection);
		CMsvSession* paramSession = ObtainParameterReferenceL<CMsvSession>(TestCase(),ActionParameters().Parameter(0));
		TMsvId paramParentId = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(1));

		CMsvEntry* entry = CMsvEntry::NewL(*paramSession,paramParentId,TMsvSelectionOrdering(KMsvNoGrouping,EMsvSortByNone,ETrue));
		CleanupStack::PushL(entry);
		entry->SetEntryL(paramParentId);

		TMsvSelectionOrdering order;
		order.SetShowInvisibleEntries(ETrue);
		entry->SetSortTypeL(order);

		CMsvEntrySelection* paramSelection = entry->ChildrenL();
		CleanupStack::PushL(paramSelection);
		StoreParameterL<CMsvEntrySelection>(TestCase(),*paramSelection,ActionParameters().Parameter(2));
		CleanupStack::Pop(paramSelection);
		CleanupStack::PopAndDestroy(entry);
	
		TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionCreateChildrenSelection);
		}
	TestCase().ActionCompletedL(*this);
	}


