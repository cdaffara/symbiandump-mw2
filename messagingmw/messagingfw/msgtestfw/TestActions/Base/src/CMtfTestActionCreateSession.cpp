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
// CreateSession
// [Action Parameters]
// Session <output>: Reference to the created session.
// [Action Description]
// Creates a session to the messaging server.
// [APIs Used]
// MMsvSessionObserver
// CMsvSession::OpenSyncL
// __ACTION_INFO_END__
// 
//

/**
 @file
*/


#include "CMtfTestActionCreateSession.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"


CMtfTestAction* CMtfTestActionCreateSession::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionCreateSession* self = new (ELeave) CMtfTestActionCreateSession(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionCreateSession::CMtfTestActionCreateSession(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionCreateSession::~CMtfTestActionCreateSession()
	{
	delete iSessionObserver;
	}

void CMtfTestActionCreateSession::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionCreateSession);
	iSessionObserver = new (ELeave) CSessionObserver;
	CMsvSession* paramSession = CMsvSession::OpenSyncL(*iSessionObserver);
	CleanupStack::PushL(paramSession);

	StoreParameterL<CMsvSession>(TestCase(),*paramSession,ActionParameters().Parameter(0));
	CleanupStack::Pop(paramSession);

	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionCreateSession );
	TestCase().ActionCompletedL(*this);
	}

