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
// DeleteImap4Mtm
// [Action Parameters]
// Mtm <input>: Reference to Imap4 client MTM.
// [Action Description]
// Deletes Imap4 client MTM.
// [APIs Used]
// none
// __ACTION_INFO_END__
// 
//

/**
 @file
*/


#include "CMtfTestActionDeleteImap4Mtm.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

#include <impcmtm.h>


CMtfTestAction* CMtfTestActionDeleteImap4Mtm::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionDeleteImap4Mtm* self = new (ELeave) CMtfTestActionDeleteImap4Mtm(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionDeleteImap4Mtm::CMtfTestActionDeleteImap4Mtm(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionDeleteImap4Mtm::~CMtfTestActionDeleteImap4Mtm()
	{
	}


void CMtfTestActionDeleteImap4Mtm::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionDeleteImap4Mtm);
	DeleteParameterL<CImap4ClientMtm>(TestCase(),ActionParameters().Parameter(0));

	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionDeleteImap4Mtm);
	TestCase().ActionCompletedL(*this);
	}

