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
// DeletePop3Mtm
// [Action Parameters]
// Mtm <input>: Reference to POP3 client MTM.
// [Action Description]
// Deletes POP3 client MTM.
// [APIs Used]
// none
// __ACTION_INFO_END__
// 
//

/**
 @file
*/


#include "CMtfTestActionDeletePop3Mtm.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

#include <popcmtm.h>


CMtfTestAction* CMtfTestActionDeletePop3Mtm::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionDeletePop3Mtm* self = new (ELeave) CMtfTestActionDeletePop3Mtm(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionDeletePop3Mtm::CMtfTestActionDeletePop3Mtm(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionDeletePop3Mtm::~CMtfTestActionDeletePop3Mtm()
	{
	}


void CMtfTestActionDeletePop3Mtm::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionDeletePop3Mtm);
	DeleteParameterL<CPop3ClientMtm>(TestCase(),ActionParameters().Parameter(0));
	
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionDeletePop3Mtm);
	TestCase().ActionCompletedL(*this);
	}

