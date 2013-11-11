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
// Sleep
// [Action Parameters]
// Time <input>: Value of sleep time in seconds.
// [Action Description]
// Action will be completed after the specified time.
// [APIs Used]
// none
// __ACTION_INFO_END__
// 
//

/**
 @file
*/


#include "CMtfTestActionSleep.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"


CMtfTestAction* CMtfTestActionSleep::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSleep* self = new (ELeave) CMtfTestActionSleep(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionSleep::CMtfTestActionSleep(CMtfTestCase& aTestCase)
	: CMtfTestAction(aTestCase)
	{
	}


CMtfTestActionSleep::~CMtfTestActionSleep()
	{
	}
	

void CMtfTestActionSleep::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSleep);
	TInt paramTime = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(0));

	iTimer = CMtfTestActionUtilsTimer::NewL();
	iTimer->After(paramTime,iStatus);
	CActiveScheduler::Add(this);
	SetActive();
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSleep);
	}


void CMtfTestActionSleep::DoCancel()
	{
	iTimer->Cancel();
	}


void CMtfTestActionSleep::RunL()
	{
	delete iTimer;

	User::LeaveIfError(iStatus.Int());
	TestCase().ActionCompletedL(*this);
	}

