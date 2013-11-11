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
//

#include <mtmuids.h>
#include "sendas2test.h"

RTest test(_L("SendAs2 - Server shutdown testing"));

const TInt KSendAsServerCloseTime = 2000000;  // 2 seconds
const TInt KSendAsServerWaitTime = 500000;  // half a second
_LIT(KSendAsServerName, "!SendAsServer*");

//
TBool ServerRunning()
	{
	TFindServer findServer(KSendAsServerName);
	TFullName fullName;
	return (findServer.Next(fullName) == KErrNone);
	}

void TestServerShutdownL()
	{
	RSendAs sendAs;

	theUtils->WriteComment(_L("\nEstablishing a connection with the server..."));
	test(sendAs.Connect() == KErrNone); // Establish a connection
	theUtils->WriteComment(_L("\tsuccess."));

	theUtils->WriteComment(_L("\nChecking SendAs Server is running..."));
	test(ServerRunning()); // Check the server exe exists.
	theUtils->WriteComment(_L("\tsuccess."));

	theUtils->WriteComment(_L("\nClosing session."));
	sendAs.Close(); // Close session.

	theUtils->WriteComment(_L("\nWaiting less that the shutdown period for the server."));
	User::After(KSendAsServerWaitTime); // Wait for less than the shut down period.

	theUtils->WriteComment(_L("\nChecking SendAs Server is running..."));
	test(ServerRunning()); // Check the server exe exists.
	theUtils->WriteComment(_L("\tsuccess."));

	theUtils->WriteComment(_L("\nWaiting the shutdown period for the server."));
	User::After(KSendAsServerCloseTime); // Wait for server shutdown delay.

	theUtils->WriteComment(_L("\nChecking SendAs Server is not running..."));
	test(!ServerRunning()); // Check the server exe no longer exists.
	theUtils->WriteComment(_L("\tsuccess."));
	}

void doTestsL()
	{
	theUtils->Start(_L("Server delayed shutdown test"));
	TestServerShutdownL();		// Qualified: No leavescan error
	theUtils->Complete();
	}





