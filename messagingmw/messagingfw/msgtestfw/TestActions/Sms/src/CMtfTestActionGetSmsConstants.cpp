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
// GetSmsConstants
// [Action Parameters]
// TUid SmsMtmUid     <output-initiation>: Value of the SMS MTM uid.
// [Action Description]
// Returns the value of SMS MTM Uid 
// [APIs Used]
// none.
// __ACTION_INFO_END__
// 
//

/**
 @file
*/

#include "CMtfTestActionGetSmsConstants.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

#include <smut.h>


CMtfTestAction* CMtfTestActionGetSmsConstants::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionGetSmsConstants* self = new (ELeave) CMtfTestActionGetSmsConstants(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	
//  does nothing
CMtfTestActionGetSmsConstants::CMtfTestActionGetSmsConstants(CMtfTestCase& aTestCase)
	:CMtfSynchronousTestAction(aTestCase)
	{
	}

// does nothing
CMtfTestActionGetSmsConstants::~CMtfTestActionGetSmsConstants()
	{
	}

//  initiates asynchronous request
void CMtfTestActionGetSmsConstants::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionGetSmsConstants);
	TUid smsMtmUid = KUidMsgTypeSMS;
	StoreParameterL<TUid>(TestCase(),smsMtmUid,ActionParameters().Parameter(0));
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionGetSmsConstants);
	TestCase().ActionCompletedL(*this);
	}



