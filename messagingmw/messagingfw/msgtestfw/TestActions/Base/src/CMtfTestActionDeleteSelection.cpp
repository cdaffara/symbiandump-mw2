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
// DeleteSelection
// [Action Parameters]
// Selection <input>: Reference to the Selection.
// [Action Description]
// Deletes the specified selection.
// [APIs Used]
// none
// __ACTION_INFO_END__
// 
//

/**
 @file
*/


#include "CMtfTestActionDeleteSelection.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

#include <msvapi.h>


CMtfTestAction* CMtfTestActionDeleteSelection::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionDeleteSelection* self = new (ELeave) CMtfTestActionDeleteSelection(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionDeleteSelection::CMtfTestActionDeleteSelection(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionDeleteSelection::~CMtfTestActionDeleteSelection()
	{
	}


void CMtfTestActionDeleteSelection::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionDeleteSelection);
	DeleteParameterL<CMsvEntrySelection>(TestCase(),ActionParameters().Parameter(0));

	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionDeleteSelection);
	TestCase().ActionCompletedL(*this);
	}

