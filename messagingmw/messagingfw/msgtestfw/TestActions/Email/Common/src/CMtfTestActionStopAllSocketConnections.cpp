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
// StopAllSocketConnections
// [Action Parameters]
// none
// [Action Description]
// Stops all connections at the socket server level
// [APIs Used]
// none
// __ACTION_INFO_END__
// 
//

/**
 @file
*/

#include <es_sock.h>
#include "CMtfTestActionStopAllSocketConnections.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

CMtfTestAction* CMtfTestActionStopAllSocketConnections::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionStopAllSocketConnections* self = new (ELeave) CMtfTestActionStopAllSocketConnections(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionStopAllSocketConnections::CMtfTestActionStopAllSocketConnections(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionStopAllSocketConnections::~CMtfTestActionStopAllSocketConnections()
	{
	}
	

void CMtfTestActionStopAllSocketConnections::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionStopAllSocketConnections);
	RSocketServ socketServer;
	User::LeaveIfError( socketServer.Connect() );
	CleanupClosePushL(socketServer);

	RConnection conn;
	User::LeaveIfError( conn.Open(socketServer) );
	CleanupClosePushL(conn);

/* ??? */
	// Terminate all currently active connections.
	// (notifies all clients with KErrConnectionTerminated)
	User::LeaveIfError( conn.Stop(RConnection::EStopAuthoritative) );

/* OR

	TConnectionInfo connInfo
	TPckg<TConnectionInfo> connInfoPckg(connInfo);

	// Find out how many active connections there are
	TUint count;
	User::LeaveIfError( conn.EnumerateConnections(count) );
	while(count > 0)
		{
		// Get the connection info of the first active connection...
		User::LeaveIfError( conn.GetConnectionInfo(0, connInfoPckg) );
		// ...so we can attach to it...
		User::LeaveIfError( conn.Attach(connInfoPckg, RConnection::EAttachTypeNormal) );
		// ...and then kill it! (all clients notifed with KErrConnectionTerminated)
		User::LeaveIfError( conn.Stop(RConnection::EStopAuthoritative) );

		// See if there are any more.
		User::LeaveIfError( conn.EnumerateConnections(count) );
		}

??? END */
	CleanupStack::PopAndDestroy(&conn);
	CleanupStack::PopAndDestroy(&socketServer);
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionStopAllSocketConnections);
	TestCase().ActionCompletedL(*this);
	}

