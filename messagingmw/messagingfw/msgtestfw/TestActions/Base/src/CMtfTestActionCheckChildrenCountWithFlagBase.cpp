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
// Abstrast base class for Test Actions that check the number of Message Entries with a 
// particular flag set.
// Derived classes need to override FlagIsSetL() which tests the specific TMsvEntry flag. 
// [Action Parameters]
// Session  <input>: Reference to the session.
// ParentId <input>: Value of the parent id.
// Count    <input>: Value of expected count with particular flag set.
// [Action Description]
// Checks if children count with particular flag set is as expected.
// [APIs Used]
// CMsvEntry::SetEntryL
// CMsvEntry::ChildrenL
// CMsvEntry::SetSortTypeL
// TMsvSelectionOrdering::SetShowInvisibleEntries
// 
//

/**
 @file
 @internalTechnology
*/


#include "CMtfTestActionCheckChildrenCountWithFlagBase.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

#include <msvapi.h>

_LIT(KFlagNameUnknown, "Unknown Flag");

TInt CMtfTestActionCheckChildrenCountWithFlagBase::CountChildrenL(TMsvId aParent, CMsvSession& aSession)
	{
	TInt children = 0;
	// Get children at this level

	CMsvEntry* entryContext = CMsvEntry::NewL(aSession, aParent, 
	    	TMsvSelectionOrdering(KMsvNoGrouping, EMsvSortByNone, ETrue));
	CleanupStack::PushL(entryContext);
	
	entryContext->SetEntryL(aParent);
	TMsvSelectionOrdering order;
	order.SetShowInvisibleEntries(ETrue);
	entryContext->SetSortTypeL(order);
	
	CMsvEntrySelection* selection = entryContext->ChildrenL();
	CleanupStack::PushL(selection);
	
	// for each child
	TInt count=selection->Count();
	for(TInt child=0;child<count;child++)
		{
		entryContext->SetEntryL((*selection)[child]);
		TMsvEntry entry=entryContext->Entry();
		if (entry.iType==KUidMsvMessageEntry && FlagIsSet(entry) )
			{
			++children;
			}
		// if this is a folder then recurse
		if (entry.iType==KUidMsvFolderEntry)
			children +=CountChildrenL((*selection)[child], aSession);
		}
	CleanupStack::PopAndDestroy(2); // selection,entryContext
	return children;
	}


CMtfTestActionCheckChildrenCountWithFlagBase::CMtfTestActionCheckChildrenCountWithFlagBase(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionCheckChildrenCountWithFlagBase::~CMtfTestActionCheckChildrenCountWithFlagBase()
	{
	}

TBool CMtfTestActionCheckChildrenCountWithFlagBase::FlagIsSet(TMsvEntry& /*entry */) 
    {
	return EFalse;
	}

void CMtfTestActionCheckChildrenCountWithFlagBase::ExecuteActionL()
	{
	TestCase().INFO_PRINTF1(_L("Test Action CMtfTestActionCheckChildrenCountWithFlagBase start..."));
	CMsvSession* paramSession = ObtainParameterReferenceL<CMsvSession>(TestCase(),ActionParameters().Parameter(0));
	TMsvId paramParentId = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(1));
	TInt paramCount = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(2));

	TInt count = CountChildrenL(paramParentId, *paramSession);
	
	if (paramCount != count)
	    {
	    TestCase().INFO_PRINTF4(_L("ChildrenCount with Flag %S was %d, expected %d"), FlagName(), count, paramCount );
		TestCase().SetTestStepResult(EFail);
		User::Leave(KErrArgument);
		}
	else
	    {
		TestCase().INFO_PRINTF2(_L("Test Action CheckChildrenCountWithFlag %S completed."), FlagName() );
		}
	TestCase().ActionCompletedL(*this);
	}

