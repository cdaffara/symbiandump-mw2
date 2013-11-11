// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// ForwardSystemTime
// [Action Parameters]
// [Action Description]
// Forward the system time 
// __ACTION_INFO_END__
// 
//

// 
#include "CMtfTestActionForwardSystemTime.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"


CMtfTestAction* CMtfTestActionForwardSystemTime::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionForwardSystemTime* self = new (ELeave) CMtfTestActionForwardSystemTime(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}


CMtfTestActionForwardSystemTime::CMtfTestActionForwardSystemTime(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionForwardSystemTime::~CMtfTestActionForwardSystemTime()
	{
	}


void CMtfTestActionForwardSystemTime::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionForwardSystemTime);
	
	//Get the current system time
	TTime utcTime;
	utcTime.UniversalTime();
	
	//Get value from Ini file
    TInt  forwardTimeInMinutes = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(0));

	//Forward the system time 
	TTimeIntervalMinutes forwardMinutes(forwardTimeInMinutes);
	User::SetUTCTime(utcTime+forwardMinutes);
	
	TestCase().INFO_PRINTF2(_L("Test Action %S Passed..."), &KTestActionForwardSystemTime);	
	TestCase().ActionCompletedL(*this);
	}
	



