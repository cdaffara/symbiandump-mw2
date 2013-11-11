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

#include "CMtfTestAction.h"
#include <e32std.h>
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

_LIT(KMtfTestActionPanic,"Messaging Test Action");

void CMtfTestAction::Panic(TMtfTestActionPanic aPanic)
{
	User::Panic(KMtfTestActionPanic,aPanic);
}
 		  		
CMtfTestAction::CMtfTestAction(CMtfTestCase& aTestCase) 
	: CActive(EPriorityStandard), iTestCase(aTestCase), iCurrentlyBeingWaitedFor(EFalse) 
{
	// don't take ownership of parameters
}

/** Ownership is taken at the END of the function.
@param aActionParameters Action parameters, must not be NULL */
void CMtfTestAction::ConstructL(CMtfTestActionParameters* aActionParameters)
{
	__ASSERT_ALWAYS(aActionParameters != NULL, CMtfTestAction::Panic(CMtfTestAction::EMtfNonReferenceParameter));
	
	iTestCase.QueueTestActionL(this);
	// now you can take ownership, we must not leave after this point
	iActionParams = aActionParameters;
}
		
CMtfTestAction::~CMtfTestAction() 
{
	delete iActionParams;
	delete iActionId;
};
	
const CMtfTestActionParameters& CMtfTestAction::ActionParameters() const 
{
	// guaranteed not to be NULL
	return *iActionParams;
}

const TDesC& CMtfTestAction::ActionIdL() const
{
	if (!iActionId)
	{
		User::Leave(KErrNotFound);
	}
	
	return *iActionId;
}

void CMtfTestAction::SetActionIdL(const TDesC& aActionId)
{
	iActionId = aActionId.AllocL();
}

CMtfTestCase& CMtfTestAction::TestCase()
{
	return iTestCase;
}

TBool CMtfTestAction::WaitAction() const
{
	return EFalse;
}

TBool CMtfTestAction::SectionCompleteAction() const
{
	return EFalse;
}

TBool CMtfTestAction::CurrentlyBeingWaitedFor() const
{
	return iCurrentlyBeingWaitedFor;
}

void CMtfTestAction::SetCurrentlyBeingWaitedFor(const TBool& aWaitedFor)
{
	iCurrentlyBeingWaitedFor = aWaitedFor;
}

TInt CMtfTestAction::RunError(TInt aErr) 
{
	TestCase().INFO_PRINTF2(_L("RunError aErr = %d"), aErr);
    return aErr;
}

