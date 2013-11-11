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
// MoveEntry
// [Action Parameters]
// Session   <input>: Reference to the session.
// EntryId <input>: Value of the Entry id to be copied.
// TargetId  <input>: Value of the target folder or service id.
// [Action Description]
// Moves the Entry to the target folder or service.
// [APIs Used]
// CMsvEntry::SetEntryL
// CMsvEntry::MoveL
// __ACTION_INFO_END__
// 
//

/**
 @file
*/


#include "CMtfTestActionMoveEntry.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "MtfTestActionUtilsUser.h"


CMtfTestAction* CMtfTestActionMoveEntry::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionMoveEntry* self = new (ELeave) CMtfTestActionMoveEntry(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionMoveEntry::CMtfTestActionMoveEntry(CMtfTestCase& aTestCase)
	: CMtfTestAction(aTestCase)
	{
	}


CMtfTestActionMoveEntry::~CMtfTestActionMoveEntry()
	{
	}


void CMtfTestActionMoveEntry::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionMoveEntry);
	CMsvSession* paramSession = ObtainParameterReferenceL<CMsvSession>(TestCase(),ActionParameters().Parameter(0));
	TMsvId paramEntryId = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(1));
	TMsvId paramTargetId  = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(2));

	CMsvEntry* entry = CMsvEntry::NewL(*paramSession,paramEntryId,TMsvSelectionOrdering());
	CleanupStack::PushL(entry);
	entry->SetEntryL(paramEntryId);
	entry->SetEntryL(entry->Entry().Parent());
	iOperation = entry->MoveL(paramEntryId,paramTargetId,iStatus);
	CleanupStack::PopAndDestroy(entry);
	CActiveScheduler::Add(this);
	SetActive();
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionMoveEntry);
	}


void CMtfTestActionMoveEntry::DoCancel()
	{
	iOperation->Cancel();
	}


void CMtfTestActionMoveEntry::RunL()
	{
	TInt err = MtfTestActionUtilsUser::FinalProgressStatus(*iOperation,iStatus);

	delete iOperation;

	User::LeaveIfError(err);
	TestCase().ActionCompletedL(*this);
	}

