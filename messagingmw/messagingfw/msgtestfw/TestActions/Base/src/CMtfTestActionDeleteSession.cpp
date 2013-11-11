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
// Session <input>: Reference to the session.
// [Action Description]
// Deletes the specified session.
// [APIs Used]
// none
// __ACTION_INFO_END__
// 
//

/**
 @file
*/


#include "CMtfTestActionDeleteSession.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

#include <msvapi.h>


CMtfTestAction* CMtfTestActionDeleteSession::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionDeleteSession* self = new (ELeave) CMtfTestActionDeleteSession(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionDeleteSession::CMtfTestActionDeleteSession(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionDeleteSession::~CMtfTestActionDeleteSession()
	{
	}


void CMtfTestActionDeleteSession::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionDeleteSession);
	DeleteParameterL<CMsvSession>(TestCase(),ActionParameters().Parameter(0));

	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionDeleteSession);
	TestCase().ActionCompletedL(*this);
	}

