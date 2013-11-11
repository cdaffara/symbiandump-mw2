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


#include "CMtfTestActionSampleActionC.h"
#include "CMtfTestActionManager.h"
#include "CMtfTestServer.h"
#include "CMtfTestCase.h"

CMtfTestAction* CMtfTestActionSampleActionC::NewL(CMtfTestCase& aTestCase,
		CMtfTestActionParameters* aActionParameters)
{
	CMtfTestActionSampleActionC* self = new (ELeave) CMtfTestActionSampleActionC(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
}
	
CMtfTestActionSampleActionC::CMtfTestActionSampleActionC(CMtfTestCase& aTestCase)
: CMtfSynchronousTestAction(aTestCase)
{
}

void CMtfTestActionSampleActionC::ExecuteActionL()
{
	TestCase().ActionCompletedL(*this);
}

CMtfTestActionSampleActionC::~CMtfTestActionSampleActionC()
{
}
