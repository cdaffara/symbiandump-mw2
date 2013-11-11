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
// DeleteFolder
// [Action Parameters]
// Session  <input>: Reference to the session.
// FolderId <input>: Value of the folder id.
// [Action Description]
// Deletes the folder.
// [APIs Used]
// CMsvEntry::SetEntryL
// CMsvEntry::DeleteL
// __ACTION_INFO_END__
// 
//

/**
 @file
*/


#include "CMtfTestActionDeleteFolder.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "MtfTestActionUtilsUser.h"

#include <miutset.h>


CMtfTestAction* CMtfTestActionDeleteFolder::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionDeleteFolder* self = new (ELeave) CMtfTestActionDeleteFolder(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionDeleteFolder::CMtfTestActionDeleteFolder(CMtfTestCase& aTestCase)
	: CMtfTestAction(aTestCase)
	{
	}


CMtfTestActionDeleteFolder::~CMtfTestActionDeleteFolder()
	{
	}


void CMtfTestActionDeleteFolder::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionDeleteFolder);
	CMsvSession* paramSession = ObtainParameterReferenceL<CMsvSession>(TestCase(),ActionParameters().Parameter(0));
	TMsvId paramFolderId = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(1));

	CMsvEntry* entry = CMsvEntry::NewL(*paramSession,paramFolderId,TMsvSelectionOrdering());
	CleanupStack::PushL(entry);
	entry->SetEntryL(paramFolderId);
	entry->SetEntryL(entry->Entry().Parent());

	if (entry->OwningService() == KMsvLocalServiceIndexEntryId)
		{	
		entry->DeleteL(paramFolderId);
		DeleteParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(1));
		TestCase().ActionCompletedL(*this);
		}
	else
		{
		iOperation = entry->DeleteL(paramFolderId,iStatus);
		SetActive();
		}
	CleanupStack::PopAndDestroy(entry);
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionDeleteFolder);
	}

void CMtfTestActionDeleteFolder::DoCancel()
	{
	iOperation->Cancel();
	}


void CMtfTestActionDeleteFolder::RunL()
	{
	TInt err = MtfTestActionUtilsUser::FinalProgressStatus(*iOperation,iStatus);
	delete iOperation;
	User::LeaveIfError(err);
	DeleteParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(1));

	TestCase().ActionCompletedL(*this);
	}

