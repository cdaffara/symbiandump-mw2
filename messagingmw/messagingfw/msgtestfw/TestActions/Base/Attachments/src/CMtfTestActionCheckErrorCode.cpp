// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// CheckErrorCode
// [Action Parameters]
// ErrorCode      <input>: The error to check.
// ExpectedError  <input>: The expected error code to check error against.
// [Action Description]
// Checks that an error code matches an expected error code.
// [APIs Used]
// None
// __ACTION_INFO_END__
// 
//


#include "CMtfTestActionCheckErrorCode.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "MtfTestActionUtilsUser.h"

CMtfTestAction* CMtfTestActionCheckErrorCode::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionCheckErrorCode* self = new(ELeave) CMtfTestActionCheckErrorCode(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionCheckErrorCode::CMtfTestActionCheckErrorCode(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionCheckErrorCode::~CMtfTestActionCheckErrorCode()
	{
	}


void CMtfTestActionCheckErrorCode::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionCheckErrorCode);
	TInt errorCode = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(0));
	TInt expectedError = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(1));
	
	TestCase().INFO_PRINTF3(_L("Actual Error Code = %d ,Expected Error Code = %d"),errorCode,expectedError);
	if( errorCode!=expectedError )
		User::Leave(KErrGeneral);
	
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionCheckErrorCode);
	TestCase().ActionCompletedL(*this);
	}

