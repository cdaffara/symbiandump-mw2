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


#include "CMtfTestActionSampleActionD.h"
#include "CMtfTestActionManager.h"
#include "CMtfTestServer.h"
#include "CMtfTestCase.h"

CMtfTestAction* CMtfTestActionSampleActionD::NewL(CMtfTestCase& aTestCase,
		CMtfTestActionParameters* aActionParameters)
{
	CMtfTestActionSampleActionD* self = new (ELeave) CMtfTestActionSampleActionD(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
}
	
CMtfTestActionSampleActionD::CMtfTestActionSampleActionD(CMtfTestCase& aTestCase)
: CMtfSynchronousTestAction(aTestCase)
{
}

void CMtfTestActionSampleActionD::ExecuteActionL()
{
	TestCase().ActionCompletedL(*this);
}

CMtfTestActionSampleActionD::~CMtfTestActionSampleActionD()
{
}
