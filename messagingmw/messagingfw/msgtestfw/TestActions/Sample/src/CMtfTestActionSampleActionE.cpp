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


#include "CMtfTestActionSampleActionE.h"
#include "CMtfTestActionManager.h"
#include "CMtfTestServer.h"
#include "CMtfTestCase.h"

CMtfTestAction* CMtfTestActionSampleActionE::NewL(CMtfTestCase& aTestCase,
		CMtfTestActionParameters* aActionParameters)
{
	CMtfTestActionSampleActionE* self = new (ELeave) CMtfTestActionSampleActionE(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
}
	
CMtfTestActionSampleActionE::CMtfTestActionSampleActionE(CMtfTestCase& aTestCase)
: CMtfTestAction(aTestCase)
{
}

void CMtfTestActionSampleActionE::ExecuteActionL()
{
	CActiveScheduler::Add(this);
	SetActive();
	TRequestStatus* status = &iStatus;
	User::RequestComplete(status,KErrNone);
}

CMtfTestActionSampleActionE::~CMtfTestActionSampleActionE()
{
}

void CMtfTestActionSampleActionE::RunL()
{
	TestCase().ActionCompletedL(*this);
}

void CMtfTestActionSampleActionE::DoCancel()
{
	// do nothing
}
