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
// CloseServer
// [Action Parameters]
// Session <input>: Reference to the session.
// [Action Description]
// Closes the messaging server and deletes the session.
// [APIs Used]
// CMsvSession::CloseMessageServer
// __ACTION_INFO_END__
// 
//

/**
 @file
*/


#include "CMtfTestActionCloseServer.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

_LIT(KMsvServerPattern, "!MsvServer*");

CMtfTestAction* CMtfTestActionCloseServer::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionCloseServer* self = new (ELeave) CMtfTestActionCloseServer(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionCloseServer::CMtfTestActionCloseServer(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionCloseServer::~CMtfTestActionCloseServer()
	{
	}

void CMtfTestActionCloseServer::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S started."), &KTestActionCloseServer );

	CMsvSession* paramSession = ObtainParameterReferenceL<CMsvSession>(TestCase(),ActionParameters().Parameter(0));
	
	// Close the messaging server
	paramSession->CloseMessageServer();
	
	// Delete the session
	DeleteParameterL<CMsvSession>(TestCase(),ActionParameters().Parameter(0));
	
	// Wait for the messaging server to close
	WaitForServerClose();

	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionCloseServer );
	TestCase().ActionCompletedL(*this);
	}

void CMtfTestActionCloseServer::WaitForServerClose()
	{

	FOREVER
		{
#if defined(__WINS__) && !defined(EKA2)
		TFileName name;
		TFindThread find(KMsvServerName);
		if (find.Next(name) != KErrNone)
			break;
#endif
#if defined (__EPOC32__) && !defined(EKA2)
		TFullName name;
		TFindProcess find(_L("MSEXE*"));
		if (find.Next(name) != KErrNone)
			break;
#endif
#if defined(EKA2)
		TFullName name;
		TFindServer find(KMsvServerPattern);
		if (find.Next(name) != KErrNone)
			break;
#endif
		User::After(100000);
		}
	}
