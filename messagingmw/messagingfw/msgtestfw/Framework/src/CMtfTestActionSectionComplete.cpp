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

#include "CMtfTestActionSectionComplete.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionWait.h"
#include "CMtfTestActionParameters.h"

/** Creates a new instance. A wait action is always inserted before a section complete,
because all test actions must complete before next section can be executed. */
CMtfTestActionSectionComplete* CMtfTestActionSectionComplete::NewL(CMtfTestCase& aTestCase)  
{
	CMtfTestActionSectionComplete* self = new (ELeave) CMtfTestActionSectionComplete(aTestCase);
	CleanupStack::PushL(self);
	
	// insert a wait action, NewL adds the new wait action to the test case
	CMtfTestActionWait::NewL(aTestCase);
	
	// takes ownership of action parameters at the end of the function
	self->ConstructL(CMtfTestActionParameters::NewLC());
	
	CleanupStack::Pop(2,self);
	return self;
}

CMtfTestActionSectionComplete::CMtfTestActionSectionComplete(CMtfTestCase& aTestCase)
: CMtfTestAction(aTestCase)
{
}
		
CMtfTestActionSectionComplete::~CMtfTestActionSectionComplete() 
{
}

void CMtfTestActionSectionComplete::RunL()
{
}

void CMtfTestActionSectionComplete::DoCancel()
{
}

TBool CMtfTestActionSectionComplete::SectionCompleteAction() const
{
	return ETrue;
}

void CMtfTestActionSectionComplete::ExecuteActionL()
{
	TestCase().SectionCompletedL();
}
