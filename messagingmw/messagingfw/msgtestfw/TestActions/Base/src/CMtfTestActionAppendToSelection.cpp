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
// AppendToSelection
// [Action Parameters]
// Selection <input>: Reference to the selection.
// EntryId   <input>: Value of the entry id.
// [Action Description]
// Adds the entry to the selection.
// [APIs Used]
// CMsvEntrySelection::AppendL
// __ACTION_INFO_END__
// 
//

/**
 @file
*/

#include "CMtfTestActionAppendToSelection.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"


CMtfTestAction* CMtfTestActionAppendToSelection::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionAppendToSelection* self = new (ELeave) CMtfTestActionAppendToSelection(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionAppendToSelection::CMtfTestActionAppendToSelection(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionAppendToSelection::~CMtfTestActionAppendToSelection()
	{
	}


void CMtfTestActionAppendToSelection::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionAppendToSelection);
	CMsvEntrySelection* paramSelection = ObtainParameterReferenceL<CMsvEntrySelection>(TestCase(),ActionParameters().Parameter(0));
	TMsvId paramEntryId = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(1));

	paramSelection->AppendL(paramEntryId);

	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionAppendToSelection);
	TestCase().ActionCompletedL(*this);
	}


