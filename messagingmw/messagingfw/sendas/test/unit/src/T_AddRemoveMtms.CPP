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

RTest test(_L("SendAs2 - Add/Remove MTM testing"));

//

void TestMTMRemoveL()
	{
	CDummyObserver* ob1 = new(ELeave) CDummyObserver;
	CleanupStack::PushL(ob1);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob1);
	CleanupStack::PushL(session);

	RSendAs sendAs;
	CleanupClosePushL(sendAs);

	theUtils->WriteComment(_L("\nEstablishing a connection with the server..."));
	test(sendAs.Connect() == KErrNone); // Establish a connection
	theUtils->WriteComment(_L("\tsuccess."));

	CSendAsMessageTypes* messageTypes = CSendAsMessageTypes::NewL();
	CleanupStack::PushL(messageTypes);

	theUtils->WriteComment(_L("\nRetrieving message types."));
	sendAs.FilteredMessageTypesL(*messageTypes);

	TInt typeCount = messageTypes->Count();
	test(typeCount > 0);

	// Remove sendas test mtm
	User::LeaveIfError(session->DeInstallMtmGroup(_L("z:\\resource\\messaging\\mtm\\SENDASTESTMTM.RSC")));	
	// Wait one second. Allows the active scheduler to run and update the mtm list.
	User::After(1000000); 

	// Repopulate sendas list.
	messageTypes->Reset();
	sendAs.FilteredMessageTypesL(*messageTypes);

	// Check the sendAs mtm list count is one lower
	theUtils->WriteComment(_L("\nChecking mtm count is one lower."));
	test(messageTypes->Count() == (typeCount - 1));

	CleanupStack::PopAndDestroy(4, ob1); // messageTypes, sendAs, session, ob1
	theUtils->WriteComment(_L("\nClosing session."));
	}

void TestMTMAddL()
	{
	CDummyObserver* ob1 = new(ELeave) CDummyObserver;
	CleanupStack::PushL(ob1);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob1);
	CleanupStack::PushL(session);

	RSendAs sendAs;
	CleanupClosePushL(sendAs);

	theUtils->WriteComment(_L("\nEstablishing a connection with the server..."));
	test(sendAs.Connect() == KErrNone); // Establish a connection
	theUtils->WriteComment(_L("\tsuccess."));

	CSendAsMessageTypes* messageTypes = CSendAsMessageTypes::NewL();
	CleanupStack::PushL(messageTypes);

	theUtils->WriteComment(_L("\nRetrieving message types."));
	sendAs.FilteredMessageTypesL(*messageTypes);

	TInt typeCount = messageTypes->Count();
	test(typeCount > 0);

	// Add sendas test mtm
	User::LeaveIfError(session->InstallMtmGroup(_L("z:\\resource\\messaging\\mtm\\SENDASTESTMTM.RSC")));
	// Wait one second. Allows the active scheduler to run and update the mtm list.
	User::After(1000000); 

	// Repopulate sendas list.
	messageTypes->Reset();
	sendAs.ResetMessageFilter();
	sendAs.FilteredMessageTypesL(*messageTypes);

	// Check the sendAs mtm list count is one higher
	theUtils->WriteComment(_L("\nChecking mtm count is one higher."));
	test(messageTypes->Count() == (typeCount + 1));

	CleanupStack::PopAndDestroy(4, ob1); // messageTypes, sendAs, session, ob1
	theUtils->WriteComment(_L("\nClosing session."));
	}


void doTestsL()
	{
	theUtils->Start(_L("MTM remove test"));
	TestMTMRemoveL();		// Qualified: No leavescan error
	theUtils->Complete();

	theUtils->Start(_L("MTM add test"));
	TestMTMAddL();			// Qualified: No leavescan error
	theUtils->Complete();
	}





