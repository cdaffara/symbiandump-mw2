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
// FindEntryByName
// [Action Parameters]
// Session   <input>: Reference to the session.
// ParentId  <input>: Value of the parent id.
// EntryName <input>: Name of the entry description or details.
// EntryId  <output>: Value of the found entry id.
// [Action Description]
// Finds and returns a specified entry.
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


#include "CMtfTestActionFindEntryByName.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

#include <msvapi.h>


CMtfTestAction* CMtfTestActionFindEntryByName::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionFindEntryByName* self = new (ELeave) CMtfTestActionFindEntryByName(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionFindEntryByName::CMtfTestActionFindEntryByName(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionFindEntryByName::~CMtfTestActionFindEntryByName()
	{
	}


void CMtfTestActionFindEntryByName::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionFindEntryByName);
	TMsvId paramEntryId;

	CMsvSession* paramSession = ObtainParameterReferenceL<CMsvSession>(TestCase(),ActionParameters().Parameter(0));
	TMsvId paramParentId      = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(1));
	HBufC* paramEntryName   = ObtainParameterReferenceL<HBufC>(TestCase(),ActionParameters().Parameter(2));
	TPtrC entryName = paramEntryName->Des();
	
	TestCase().INFO_PRINTF2( _L("Searching for Entry %S"), &entryName );	
	

	CMsvEntry* entry = CMsvEntry::NewL(*paramSession,paramParentId,TMsvSelectionOrdering(KMsvNoGrouping,EMsvSortByNone,ETrue));
	CleanupStack::PushL(entry);
	entry->SetEntryL(paramParentId);

	TMsvSelectionOrdering order;
	order.SetShowInvisibleEntries(ETrue);
	entry->SetSortTypeL(order);

	CMsvEntrySelection* selection = entry->ChildrenL();
	CleanupStack::PushL(selection);
	
	TBool found = EFalse;
	TInt count = selection->Count();
	for (TInt i=0; i<count; i++) 
		{	
		entry->SetEntryL((*selection)[i]);
		if ( ( entryName.CompareF( entry->Entry().iDescription ) == 0 ) ||
		    ( entryName.CompareF( entry->Entry().iDetails ) == 0 ) )
			{
			found = ETrue;
			break;
			}
		}

	if (found)
		paramEntryId = entry->Entry().Id();
	else
		{
		TestCase().ERR_PRINTF2( _L("Entry %S not found - leaving"), &entryName );	
		User::Leave(KErrNotFound);
		}

	CleanupStack::PopAndDestroy(selection);
	CleanupStack::PopAndDestroy(entry);
	StoreParameterL<TMsvId>(TestCase(),paramEntryId,ActionParameters().Parameter(3));
	
	TestCase().INFO_PRINTF2(_L("CMtfTestActionFindEntryByName search for %S completed ok"), &entryName );

	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionFindEntryByName);
	TestCase().ActionCompletedL(*this);
	}

