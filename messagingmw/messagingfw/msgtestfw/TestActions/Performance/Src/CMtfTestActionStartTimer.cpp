// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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


#include "CMtfTestActionStartTimer.h"
#include "CMtfTestActionManager.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"


CMtfTestAction* CMtfTestActionStartTimer::NewL(CMtfTestCase& aTestCase,
		CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionStartTimer* self = new (ELeave) CMtfTestActionStartTimer(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	
CMtfTestActionStartTimer::CMtfTestActionStartTimer(CMtfTestCase& aTestCase)
: CMtfSynchronousTestAction(aTestCase)
	{
	}

void CMtfTestActionStartTimer::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionStartTimer);
	
	TTime now;
	now.UniversalTime();
	TTime paramTime(now);
//	TInt64 paramTime = now.Int64();

	StoreParameterL<TTime>(TestCase(),paramTime,ActionParameters().Parameter(0));
//	StoreParameterL<TInt64>(TestCase(),paramTime,ActionParameters().Parameter(0));	
	
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionStartTimer);			
	TestCase().ActionCompletedL(*this);
	}

CMtfTestActionStartTimer::~CMtfTestActionStartTimer()
	{
	}

