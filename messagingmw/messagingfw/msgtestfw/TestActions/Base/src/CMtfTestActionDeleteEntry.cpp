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
// DeleteEntry
// [Action Parameters]
// Session <input>: Reference to the session.
// EntryId <input>: Value of the entry id.
// [Action Description]
// Deletes the entry.
// [APIs Used]
// CMsvEntry::SetEntryL
// CMsvEntry::DeleteL
// __ACTION_INFO_END__
// 
//

/**
 @file
*/


#include "CMtfTestActionDeleteEntry.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "MtfTestActionUtilsUser.h"

#include <miutset.h>


CMtfTestAction* CMtfTestActionDeleteEntry::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionDeleteEntry* self = new (ELeave) CMtfTestActionDeleteEntry(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionDeleteEntry::CMtfTestActionDeleteEntry(CMtfTestCase& aTestCase)
	: CMtfTestAction(aTestCase)
	{
	}


CMtfTestActionDeleteEntry::~CMtfTestActionDeleteEntry()
	{
	}


void CMtfTestActionDeleteEntry::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionDeleteEntry);
	CMsvSession* paramSession = ObtainParameterReferenceL<CMsvSession>(TestCase(),ActionParameters().Parameter(0));
	TMsvId paramEntryId = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(1));

	CMsvEntry* entry = CMsvEntry::NewL(*paramSession,paramEntryId,TMsvSelectionOrdering());
	CleanupStack::PushL(entry);
	entry->SetEntryL(paramEntryId);
	entry->SetEntryL(entry->Entry().Parent());

	if (entry->OwningService() == KMsvLocalServiceIndexEntryId)
		{
		entry->DeleteL(paramEntryId);
		DeleteParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(1));
		TestCase().ActionCompletedL(*this);
		}
	else
		{
		iOperation = entry->DeleteL(paramEntryId,iStatus);
		SetActive();
		}
	CleanupStack::PopAndDestroy(entry);
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionDeleteEntry);
	}


void CMtfTestActionDeleteEntry::DoCancel()
	{
	iOperation->Cancel();
	}


void CMtfTestActionDeleteEntry::RunL()
	{
	TInt err = MtfTestActionUtilsUser::FinalProgressStatus(*iOperation,iStatus);
	delete iOperation;
	User::LeaveIfError(err);
	DeleteParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(1));

	TestCase().ActionCompletedL(*this);
	}

