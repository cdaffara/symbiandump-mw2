// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// SetUserResponse
// [Action Parameters]
// UserResponse	  <input>: (optional)User response for confirmed send.Default value is 0
// [Action Description]
// Sets the user response for confirmed send
// [APIs Used]
// TODO
// __ACTION_INFO_END__
// 
//


#include "CMtfTestActionSetUserResponse.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

CMtfTestAction* CMtfTestActionSetUserResponse::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSetUserResponse* self = new(ELeave) CMtfTestActionSetUserResponse(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionSetUserResponse::CMtfTestActionSetUserResponse(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionSetUserResponse::~CMtfTestActionSetUserResponse()
	{
	}

void CMtfTestActionSetUserResponse::ExecuteActionL()
	{	
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSetUserResponse);
	CMsvSession* paramSession = ObtainParameterReferenceL<CMsvSession> (TestCase(),ActionParameters().Parameter(0));	
	TInt paramUserResponse = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(1),EFalse);
	
	CMsvEntry* cEntry = CMsvEntry::NewL(*paramSession, KMsvDraftEntryId, 
						TMsvSelectionOrdering(KMsvNoGrouping,EMsvSortByNone,ETrue));
	CleanupStack::PushL(cEntry);
	
	CMsvEntrySelection* selection = cEntry->ChildrenL();
	CleanupStack::PushL(selection);

	TestCase().INFO_PRINTF2(_L("Count of Draft Folder's selection is %d"),selection->Count());
	
	if (selection->Count() == 0)
	{	
	User::Leave(KErrNotFound);
	}
	
	cEntry->SetEntryL((*selection)[0]);

	if (!paramUserResponse)
	{	
	TMsvEntry entry = cEntry->Entry();
	entry.iError = KErrCancel;
	cEntry->ChangeL(entry);
	}
	
	CleanupStack::PopAndDestroy(2);
	
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSetUserResponse);
	TestCase().ActionCompletedL(*this);
	}

