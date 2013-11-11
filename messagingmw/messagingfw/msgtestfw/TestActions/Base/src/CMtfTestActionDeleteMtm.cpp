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
// DeleteSession
// [Action Parameters]
// Mtm <input>: Reference to the MTM.
// [Action Description]
// Deletes the specified client MTM.
// [APIs Used]
// none
// __ACTION_INFO_END__
// 
//

/**
 @file
*/


#include "CMtfTestActionDeleteMtm.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

#include <mtclbase.h>


CMtfTestAction* CMtfTestActionDeleteMtm::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionDeleteMtm* self = new (ELeave) CMtfTestActionDeleteMtm(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionDeleteMtm::CMtfTestActionDeleteMtm(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionDeleteMtm::~CMtfTestActionDeleteMtm()
	{
	}


void CMtfTestActionDeleteMtm::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionDeleteMtm);
	DeleteParameterL<CBaseMtm>(TestCase(),ActionParameters().Parameter(0));

	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionDeleteMtm);
	TestCase().ActionCompletedL(*this);
	}

