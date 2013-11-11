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


#include "CMtfTestActionSampleActionF.h"
#include "CMtfTestActionManager.h"
#include "CMtfTestServer.h"
#include "CMtfTestCase.h"

CMtfTestAction* CMtfTestActionSampleActionF::NewL(CMtfTestCase& aTestCase,
		CMtfTestActionParameters* aActionParameters)
{
	CMtfTestActionSampleActionF* self = new (ELeave) CMtfTestActionSampleActionF(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
}
	
CMtfTestActionSampleActionF::CMtfTestActionSampleActionF(CMtfTestCase& aTestCase)
: CMtfTestAction(aTestCase)
{
}

void CMtfTestActionSampleActionF::ExecuteActionL()
{
	CActiveScheduler::Add(this);
	SetActive();
	TRequestStatus* status = &iStatus;
	User::RequestComplete(status,KErrNone);
}

CMtfTestActionSampleActionF::~CMtfTestActionSampleActionF()
{
}

void CMtfTestActionSampleActionF::RunL()
{
	TestCase().ActionCompletedL(*this);
}

void CMtfTestActionSampleActionF::DoCancel()
{
	// do nothing
}
