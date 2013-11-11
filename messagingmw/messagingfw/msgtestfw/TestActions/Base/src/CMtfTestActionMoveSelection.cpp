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
// MoveSelection
// [Action Parameters]
// Session   <input>: Reference to the session.
// Selection <input>: Reference to the selection.
// TargetId  <input>: Value of the target id.
// [Action Description]
// Copies the entry to the target.
// [APIs Used]
// CMsvEntry::SetEntryL
// CMsvEntry::MoveL
// __ACTION_INFO_END__
// 
//

/**
 @file
*/


#include "CMtfTestActionMoveSelection.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "MtfTestActionUtilsUser.h"


CMtfTestAction* CMtfTestActionMoveSelection::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionMoveSelection* self = new (ELeave) CMtfTestActionMoveSelection(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionMoveSelection::CMtfTestActionMoveSelection(CMtfTestCase& aTestCase)
	: CMtfTestAction(aTestCase)
	{
	}


CMtfTestActionMoveSelection::~CMtfTestActionMoveSelection()
	{
	}


void CMtfTestActionMoveSelection::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionMoveSelection);
	CMsvSession* paramSession = ObtainParameterReferenceL<CMsvSession>(TestCase(),ActionParameters().Parameter(0));
	CMsvEntrySelection* paramSelection = ObtainParameterReferenceL<CMsvEntrySelection>(TestCase(),ActionParameters().Parameter(1));
	TMsvId paramTargetId      = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(2));

	TInt count = paramSelection->Count();
	if (count == 0)
		User::Leave(KErrArgument);

	TMsvId entryId = (*paramSelection)[0];

	CMsvEntry* entry = CMsvEntry::NewL(*paramSession,entryId,TMsvSelectionOrdering());
	CleanupStack::PushL(entry);
	entry->SetEntryL(entryId);
	entry->SetEntryL(entry->Entry().Parent());
	iOperation = entry->MoveL(*paramSelection,paramTargetId,iStatus);
	CleanupStack::PopAndDestroy(entry);
	CActiveScheduler::Add(this);
	SetActive();
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionMoveSelection);
	}


void CMtfTestActionMoveSelection::DoCancel()
	{
	iOperation->Cancel();
	}


void CMtfTestActionMoveSelection::RunL()
	{
	TInt err = MtfTestActionUtilsUser::FinalProgressStatus(*iOperation,iStatus);

	delete iOperation;

	User::LeaveIfError(err);
	TestCase().ActionCompletedL(*this);
	}

