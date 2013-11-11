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

#include <e32std.h>
#include "CMtfTestActionStopTimer.h"
#include "CMtfTestActionManager.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"


CMtfTestAction* CMtfTestActionStopTimer::NewL(CMtfTestCase& aTestCase,
		CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionStopTimer* self = new (ELeave) CMtfTestActionStopTimer(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	
CMtfTestActionStopTimer::CMtfTestActionStopTimer(CMtfTestCase& aTestCase)
: CMtfSynchronousTestAction(aTestCase)
	{
	}

void CMtfTestActionStopTimer::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionStopTimer);
	

	TTime* time=ObtainParameterReferenceL<TTime>(TestCase(),ActionParameters().Parameter(0));
//	TInt64* time=ObtainParameterReferenceL<TInt64>(TestCase(),ActionParameters().Parameter(0));
	TTime start(*time);
	TTime end;
	end.UniversalTime();
	TInt64 interval = end.MicroSecondsFrom(start).Int64();
	interval /= 1000; // ms
		
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionStopTimer);
	TestCase().INFO_PRINTF2(_L("time (ms)=%d"),interval);
	TestCase().ActionCompletedL(*this);
	}

CMtfTestActionStopTimer::~CMtfTestActionStopTimer()
	{
	}
