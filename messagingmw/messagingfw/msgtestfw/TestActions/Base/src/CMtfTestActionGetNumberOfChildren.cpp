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
// GetNumberOfChildren
// [Action Parameters]
// CMsvSession  Session       <input>: Reference to the session.
// TMsvId       ParentId      <input>: Value of the parent id.
// TInt         childrenCount <output-initiation>: Value of the Number of children.
// [Action Description]
// Returns the number of children of a given parent.
// [APIs Used]
// none.
// __ACTION_INFO_END__
// 
//

/**
 @file
*/

#include <msvapi.h>

#include "CmtfTestActionGetNumberOfChildren.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"


CMtfTestAction* CMtfTestActionGetNumberOfChildren::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionGetNumberOfChildren* self = new (ELeave) CMtfTestActionGetNumberOfChildren(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionGetNumberOfChildren::CMtfTestActionGetNumberOfChildren(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionGetNumberOfChildren::~CMtfTestActionGetNumberOfChildren()
	{
	}


void CMtfTestActionGetNumberOfChildren::ExecuteActionL()
	{
    TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionGetNumberOfChildren);
	CMsvSession* paramSession = ObtainParameterReferenceL<CMsvSession>(TestCase(),ActionParameters().Parameter(0));
	TMsvId paramParentId = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(1));
	
	CMsvEntry* entry = CMsvEntry::NewL(*paramSession,paramParentId,TMsvSelectionOrdering(KMsvNoGrouping,EMsvSortByNone,ETrue));
	CleanupStack::PushL(entry);
	entry->SetEntryL(paramParentId);

	TMsvSelectionOrdering order;
	order.SetShowInvisibleEntries(ETrue);
	entry->SetSortTypeL(order);
 		
	CMsvEntrySelection* selection = entry->ChildrenL();
	CleanupStack::PushL(selection);

	TInt childrenCount = selection->Count();
		
	StoreParameterL<TInt>(TestCase(),childrenCount,ActionParameters().Parameter(2));
	CleanupStack::PopAndDestroy(selection);
	CleanupStack::PopAndDestroy(entry);

	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionGetNumberOfChildren);
	TestCase().ActionCompletedL(*this);
	}
