// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// CreateOrderedChildrenSelection
// [Action Parameters]
// Session          <input>:  Reference to the session.
// ParentId         <input>:  Value of the parent id.
// Sorting          <input>:  Sorting criteria. See TMsvSorting.
// Grouping         <input>:  Grouping criteria. See TMsvGrouping.
// IncludeInvisible <input>:  Determines whether invisible entries are included in the selection.
// Selection        <output>: Reference to the created selection.
// [Action Description]
// Creates an ordered selection of message store children entry ids below a given parent entry.
// (E.g. Chronologically: earliest-latest).
// [APIs Used]
// TMsvGrouping
// TMsvSorting
// TMsvSelectionOrdering
// CMsvEntry::NewL
// CMsvEntry::ChildrenL
// __ACTION_INFO_END__
// 
//

#include "CMtfTestActionCreateOrderedChildrenSelection.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

#include <msvapi.h>


CMtfTestAction* CMtfTestActionCreateOrderedChildrenSelection::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionCreateOrderedChildrenSelection* self = new (ELeave) CMtfTestActionCreateOrderedChildrenSelection(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}
	

CMtfTestActionCreateOrderedChildrenSelection::CMtfTestActionCreateOrderedChildrenSelection(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionCreateOrderedChildrenSelection::~CMtfTestActionCreateOrderedChildrenSelection()
	{
	}


void CMtfTestActionCreateOrderedChildrenSelection::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionCreateOrderedChildrenSelection);
	CMsvSession* paramSession = ObtainParameterReferenceL<CMsvSession>(TestCase(),ActionParameters().Parameter(0));
	TMsvId paramParentId = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(1));
	TInt sorting = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(2));
	TInt grouping = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(3));
	TBool includeInvisible = ObtainValueParameterL<TBool>(TestCase(),ActionParameters().Parameter(4));

	TMsvSelectionOrdering selectionOrdering = TMsvSelectionOrdering(TMsvGrouping(grouping),TMsvSorting(sorting),includeInvisible);

	CMsvEntry* entry = CMsvEntry::NewL(*paramSession,paramParentId,selectionOrdering);
	CleanupStack::PushL(entry);

	CMsvEntrySelection* paramSelection = entry->ChildrenL();
	CleanupStack::PushL(paramSelection);

	StoreParameterL<CMsvEntrySelection>(TestCase(),*paramSelection,ActionParameters().Parameter(5));

	CleanupStack::Pop(paramSelection);
	CleanupStack::PopAndDestroy(entry);

	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionCreateOrderedChildrenSelection);
	TestCase().ActionCompletedL(*this);
	}


