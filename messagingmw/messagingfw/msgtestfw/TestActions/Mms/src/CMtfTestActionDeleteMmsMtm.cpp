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
// DeleteMmsMtm
// [Action Parameters]
// Mtm <input>: Reference to Mms client MTM.
// [Action Description]
// Deletes Mms client MTM.
// [APIs Used]
// none
// __ACTION_INFO_END__
// 
//

/**
 @file
*/


#include "CMtfTestActionDeleteMmsMtm.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

#include <mmsclientmtm.h>


CMtfTestAction* CMtfTestActionDeleteMmsMtm::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionDeleteMmsMtm* self = new (ELeave) CMtfTestActionDeleteMmsMtm(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionDeleteMmsMtm::CMtfTestActionDeleteMmsMtm(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionDeleteMmsMtm::~CMtfTestActionDeleteMmsMtm()
	{
	}


void CMtfTestActionDeleteMmsMtm::ExecuteActionL()
	{
	DeleteParameterL<CMmsClientMtm>(TestCase(),ActionParameters().Parameter(0));

	TestCase().ActionCompletedL(*this);
	}

