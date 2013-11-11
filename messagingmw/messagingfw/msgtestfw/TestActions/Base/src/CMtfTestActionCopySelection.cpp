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
// CopySelection
// [Action Parameters]
// Session   <input>: Reference to the session.
// Selection <input>: Reference to the selection.
// TargetId  <input>: Value of the target id.
// [Action Description]
// Copies the entry to the target.
// [APIs Used]
// CMsvEntry::SetEntryL
// CMsvEntry::CopyL
// __ACTION_INFO_END__
// 
//

/**
 @file
*/


#include "CMtfTestActionCopySelection.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "MtfTestActionUtilsUser.h"


CMtfTestAction* CMtfTestActionCopySelection::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionCopySelection* self = new (ELeave) CMtfTestActionCopySelection(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionCopySelection::CMtfTestActionCopySelection(CMtfTestCase& aTestCase)
	: CMtfTestAction(aTestCase)
	{
	}


CMtfTestActionCopySelection::~CMtfTestActionCopySelection()
	{
	}


void CMtfTestActionCopySelection::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionCopySelection);
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
	iOperation = entry->CopyL(*paramSelection,paramTargetId,iStatus);
	CleanupStack::PopAndDestroy(entry);
	CActiveScheduler::Add(this);
	SetActive();
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionCopySelection);
	}


void CMtfTestActionCopySelection::DoCancel()
	{
	iOperation->Cancel();
	}


void CMtfTestActionCopySelection::RunL()
	{
	TInt err = MtfTestActionUtilsUser::FinalProgressStatus(*iOperation,iStatus);

	delete iOperation;

	User::LeaveIfError(err);
	
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionCopySelection);
	TestCase().ActionCompletedL(*this);
	}

