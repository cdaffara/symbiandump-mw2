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
// DeleteChildren
// [Action Parameters]
// Session  <input>: Reference to the session.
// ParentId <input>: Value of the parent id.
// [Action Description]
// Deletes all children of the entry. 
// [APIs Used]
// none.
// __ACTION_INFO_END__
// 
//

/**
 @file
*/


#include "CMtfTestActionDeleteChildren.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

#include <msvapi.h>


CMtfTestAction* CMtfTestActionDeleteChildren::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionDeleteChildren* self = new (ELeave) CMtfTestActionDeleteChildren(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionDeleteChildren::CMtfTestActionDeleteChildren(CMtfTestCase& aTestCase)
	: CMtfTestAction(aTestCase)
	{
	}


CMtfTestActionDeleteChildren::~CMtfTestActionDeleteChildren()
	{
	}


void CMtfTestActionDeleteChildren::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionDeleteChildren);
	CMsvSession* paramSession = ObtainParameterReferenceL<CMsvSession>(TestCase(),ActionParameters().Parameter(0));
	TMsvId paramParentId = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(1));

	iDeleteAllChildren = new (ELeave) CMtfTestActionUtilsDeleteAllChildren(paramSession,paramParentId);
	iDeleteAllChildren->StartL(iStatus);
	CActiveScheduler::Add(this);
	SetActive();
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionDeleteChildren);
	}


void CMtfTestActionDeleteChildren::DoCancel()
	{
	iDeleteAllChildren->Cancel();
	}


void CMtfTestActionDeleteChildren::RunL()
	{
	TInt undeleted = iDeleteAllChildren->Undeleted();
	delete iDeleteAllChildren;

	User::LeaveIfError(iStatus.Int() || undeleted);

	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionDeleteChildren);
	TestCase().ActionCompletedL(*this);
	}

