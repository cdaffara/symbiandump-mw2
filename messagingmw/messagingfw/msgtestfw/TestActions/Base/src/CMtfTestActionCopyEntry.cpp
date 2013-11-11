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
// CopyEntry
// [Action Parameters]
// Session  <input>: Reference to the session.
// EntryId  <input>: Value of the entry id to be copied.
// TargetId <input>: Value of the target id.
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


#include "CMtfTestActionCopyEntry.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "MtfTestActionUtilsUser.h"


CMtfTestAction* CMtfTestActionCopyEntry::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionCopyEntry* self = new (ELeave) CMtfTestActionCopyEntry(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionCopyEntry::CMtfTestActionCopyEntry(CMtfTestCase& aTestCase)
	: CMtfTestAction(aTestCase)
	{
	}


CMtfTestActionCopyEntry::~CMtfTestActionCopyEntry()
	{
	}


void CMtfTestActionCopyEntry::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionCopyEntry);
	CMsvSession* paramSession = ObtainParameterReferenceL<CMsvSession>(TestCase(),ActionParameters().Parameter(0));
	TMsvId paramEntryId     = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(1));
	TMsvId paramTargetId      = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(2));

	CMsvEntry* entry = CMsvEntry::NewL(*paramSession,paramEntryId,TMsvSelectionOrdering());
	CleanupStack::PushL(entry);
	entry->SetEntryL(paramEntryId);
	entry->SetEntryL(entry->Entry().Parent());
	iOperation = entry->CopyL(paramEntryId,paramTargetId,iStatus);
	CleanupStack::PopAndDestroy(entry);
	CActiveScheduler::Add(this);
	SetActive();
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionCopyEntry);
	}


void CMtfTestActionCopyEntry::DoCancel()
	{
	iOperation->Cancel();
	}


void CMtfTestActionCopyEntry::RunL()
	{
	TInt err = MtfTestActionUtilsUser::FinalProgressStatus(*iOperation,iStatus);

	delete iOperation;

	User::LeaveIfError(err);
	
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionCopyEntry );
	TestCase().ActionCompletedL(*this);
	}

