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
// InitNtRas
// [Action Parameters]
// none
// [Action Description]
// Creates the serial drivers required by NTRAS.
// [APIs Used]
// none
// __ACTION_INFO_END__
// 
//

/**
 @file
*/


#include "CMtfTestActionInitNtRas.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

#if defined (__WINS__)
	#define PDD_NAME		_L("ECDRV")
	#define LDD_NAME		_L("ECOMM")
#else
	#define PDD_NAME		_L("EUART1")
	#define LDD_NAME		_L("ECOMM")
#endif


CMtfTestAction* CMtfTestActionInitNtRas::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionInitNtRas* self = new (ELeave) CMtfTestActionInitNtRas(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionInitNtRas::CMtfTestActionInitNtRas(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionInitNtRas::~CMtfTestActionInitNtRas()
	{
	}
	

void CMtfTestActionInitNtRas::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionInitNtRas);
	TInt driverErr;
	driverErr=User::LoadPhysicalDevice(PDD_NAME);
	if (driverErr!=KErrNone && driverErr!=KErrAlreadyExists)
		User::Leave(driverErr);
	driverErr=User::LoadLogicalDevice(LDD_NAME);
	if (driverErr!=KErrNone && driverErr!=KErrAlreadyExists)
		User::Leave(driverErr);

	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionInitNtRas);
	TestCase().ActionCompletedL(*this);
	}

