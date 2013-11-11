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
//

/**
 @file
*/

#include "CMtfTestActionWait.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

/** Creates a new wait action. If there are no action ids then WaitForAll is created.
Otherwise, the wait function will wait for the specified action ids to complete. 
This function takes ownership of action ids at the END of the function. */
CMtfTestActionWait* CMtfTestActionWait::NewL(CMtfTestCase& aTestCase, CMtfTestActionParameters* aActionIds)  
{
	TBool waitForAll(aActionIds->Count()==0);

	CMtfTestActionWait* self = new (ELeave) CMtfTestActionWait(aTestCase,waitForAll);
	CleanupStack::PushL(self);
	self->ConstructL(aActionIds);
	CleanupStack::Pop(self);
	return self;
}

/** Creates a WaitForAll wait action. */
CMtfTestActionWait* CMtfTestActionWait::NewL(CMtfTestCase& aTestCase)
{
	CMtfTestActionWait* self = new (ELeave) CMtfTestActionWait(aTestCase,ETrue);
	CleanupStack::PushL(self);
	
	// dummy parameters are used
	self->ConstructL(CMtfTestActionParameters::NewLC());
	
	CleanupStack::Pop(2,self);
	return self;
}

CMtfTestActionWait::CMtfTestActionWait(CMtfTestCase& aTestCase, const TBool& aWaitForAll)
: CMtfTestAction(aTestCase), iWaitForAll(aWaitForAll)
{
}
		
CMtfTestActionWait::~CMtfTestActionWait() 
{
}

void CMtfTestActionWait::RunL()
{
}

void CMtfTestActionWait::DoCancel()
{
}

TBool CMtfTestActionWait::WaitAction() const
{
	return ETrue;
}

void CMtfTestActionWait::ExecuteActionL()
{
	if (iWaitForAll)
	{
		TestCase().WaitForAllActionsL();
	}
	else
	{
		TInt numberOfParameters = ActionParameters().Count();
		
		for (TInt count=0; count<numberOfParameters; count++)
		{
			TestCase().WaitForActionL(ActionParameters().Parameter(count));
		}
	}
}
