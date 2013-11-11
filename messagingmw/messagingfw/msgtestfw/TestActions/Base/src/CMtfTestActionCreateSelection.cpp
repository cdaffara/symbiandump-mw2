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
// CreateSelection
// [Action Parameters]
// Selection <output>: Reference to the created empty selection.
// [Action Description]
// Creates an empty selection.
// [APIs Used]
// none.
// __ACTION_INFO_END__
// 
//

/**
 @file
*/

#include "CMtfTestActionCreateSelection.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

#include <msvapi.h>


CMtfTestAction* CMtfTestActionCreateSelection::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionCreateSelection* self = new (ELeave) CMtfTestActionCreateSelection(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionCreateSelection::CMtfTestActionCreateSelection(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionCreateSelection::~CMtfTestActionCreateSelection()
	{
	}


void CMtfTestActionCreateSelection::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionCreateSelection);
	CMsvEntrySelection* paramSelection = new (ELeave) CMsvEntrySelection;
	CleanupStack::PushL(paramSelection);
	StoreParameterL<CMsvEntrySelection>(TestCase(),*paramSelection,ActionParameters().Parameter(0));
	CleanupStack::Pop(paramSelection);

	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionCreateSelection);
	TestCase().ActionCompletedL(*this);
	}


