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
// CompareNumbers
// [Action Parameters]
// TInt  Number1       <input> 			   : Value to be compared.
// TInt  Number2       <input>			   : Value to be compared with.
// [Action Description]
// Succeeds if the two numbers are equal.
// [APIs Used]
// none.
// __ACTION_INFO_END__
// 
//

/**
 @file
*/

#include "CMtfTestActionCompareNumbers.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"


// TODO - rename action to compare TMsvId

CMtfTestAction* CMtfTestActionCompareNumbers::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionCompareNumbers* self = new (ELeave) CMtfTestActionCompareNumbers(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	
CMtfTestActionCompareNumbers::CMtfTestActionCompareNumbers(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}

CMtfTestActionCompareNumbers::~CMtfTestActionCompareNumbers()
	{
	}

void CMtfTestActionCompareNumbers::ExecuteActionL()
	{
	// todo - change the name of this test action to CMtfTestActionCompareTMsvIds
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionCompareNumbers);
	TMsvId paramNumber1 = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(0));
	TMsvId paramNumber2 = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(1));
	
	
	TInt shouldMatch = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(2));
	
	if( shouldMatch )
		{
		if(paramNumber1 != paramNumber2)
			{
			TestCase().INFO_PRINTF3( _L("Numbers %d != %d do not match when they should !"), 
				static_cast<TInt>(paramNumber1), static_cast<TInt>(paramNumber2) );
			TestCase().SetTestStepResult(EFail);	
			}
		}
	else   // numbers should not match
		{
		if(paramNumber1 == paramNumber2)
			{
			TestCase().INFO_PRINTF3( _L("Numbers %d != %d match when they should not!"), 
				static_cast<TInt>(paramNumber1), static_cast<TInt>(paramNumber2) );
			TestCase().SetTestStepResult(EFail);	
			}
		}
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionCompareNumbers);
	TestCase().ActionCompletedL(*this);
	}
