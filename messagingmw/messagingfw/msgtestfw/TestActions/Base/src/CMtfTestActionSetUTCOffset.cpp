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
// __ACTION_INFO_BEGIN__ 
// [Action Name]
// SetUTCOffset
// [Action Parameters]
// Offset    <input>: UTC time offset in seconds.
// [Action Description]
// Sets the UTC offset to the given number of seconds (+/-).
// [APIs Used]
// User.
// __ACTION_INFO_END__
// 
//


#include "CMtfTestActionSetUTCOffset.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

#include <msvapi.h>


CMtfTestAction* CMtfTestActionSetUTCOffset::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSetUTCOffset* self = new (ELeave) CMtfTestActionSetUTCOffset(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}
	

CMtfTestActionSetUTCOffset::CMtfTestActionSetUTCOffset(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionSetUTCOffset::~CMtfTestActionSetUTCOffset()
	{
	}


void CMtfTestActionSetUTCOffset::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSetUTCOffset);
	TInt offset = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(0));

	User::SetUTCOffset(offset);

	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSetUTCOffset );
	TestCase().ActionCompletedL(*this);
	}

