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


#include "CMtfTestActionSampleActionB.h"
#include "CMtfTestActionManager.h"
#include "CMtfTestServer.h"
#include "CMtfTestCase.h"

CMtfTestAction* CMtfTestActionSampleActionB::NewL(CMtfTestCase& aTestCase,
		CMtfTestActionParameters* aActionParameters)
{
	CMtfTestActionSampleActionB* self = new (ELeave) CMtfTestActionSampleActionB(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
}
	
CMtfTestActionSampleActionB::CMtfTestActionSampleActionB(CMtfTestCase& aTestCase)
: CMtfSynchronousTestAction(aTestCase)
{
}

void CMtfTestActionSampleActionB::ExecuteActionL()
{
	TestCase().ActionCompletedL(*this);
}

CMtfTestActionSampleActionB::~CMtfTestActionSampleActionB()
{
}
