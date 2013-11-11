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

RTest test(_L("SendAs2 - MTM capability testing"));

//

LOCAL_C void TestMtmAvailable(RSendAs& aSendAs, CSendAsMessageTypes* aMtmList, const TDesC& aName, TBool aAvailable)
	{
	TRAPD(error, aSendAs.FilteredMessageTypesL(*aMtmList));
	test(error == KErrNone);
	TRAP(error, aMtmList->UidFromNameL(aName));
	if (error == KErrNone)
		{
		test(aAvailable);
		return;
		}
	test(!aAvailable);
	}

LOCAL_C void TestMtmSelectionL()
	{
	// connect to sendas2
	RSendAs sendas;
	CleanupClosePushL(sendas);
	User::LeaveIfError(sendas.Connect());

	// filtered list of mtms
	CSendAsMessageTypes* mtmList = CSendAsMessageTypes::NewL();
	CleanupStack::PushL(mtmList);
	
	// sendmtm2 cannot send
	sendas.FilteredMessageTypesL(*mtmList);
	test(mtmList->Count() >= KNumberOfTestMtmsCanSend);
	TestMtmAvailable(sendas, mtmList, KMtmName1, ETrue);
	TestMtmAvailable(sendas, mtmList, KMtmName2, EFalse);
	TestMtmAvailable(sendas, mtmList, KMtmName3, ETrue);
	TestMtmAvailable(sendas, mtmList, KMtmName4, ETrue);
	TestMtmAvailable(sendas, mtmList, KMtmName5, ETrue);

	// support attachments
	User::LeaveIfError(sendas.FilterAgainstCapability(KUidMtmQuerySupportAttachments));
	sendas.FilteredMessageTypesL(*mtmList);
	test(mtmList->Count() >= 2);
	TestMtmAvailable(sendas, mtmList, KMtmName1, ETrue);
	TestMtmAvailable(sendas, mtmList, KMtmName2, EFalse);
	TestMtmAvailable(sendas, mtmList, KMtmName3, EFalse);
	TestMtmAvailable(sendas, mtmList, KMtmName4, ETrue);
	TestMtmAvailable(sendas, mtmList, KMtmName5, EFalse);

	// reset
	User::LeaveIfError(sendas.ResetMessageFilter());
	sendas.FilteredMessageTypesL(*mtmList);
	test(mtmList->Count() >= KNumberOfTestMtmsCanSend);
	TestMtmAvailable(sendas, mtmList, KMtmName1, ETrue);
	TestMtmAvailable(sendas, mtmList, KMtmName2, EFalse);
	TestMtmAvailable(sendas, mtmList, KMtmName3, ETrue);
	TestMtmAvailable(sendas, mtmList, KMtmName4, ETrue);
	TestMtmAvailable(sendas, mtmList, KMtmName5, ETrue);

	// off line mode
	User::LeaveIfError(sendas.FilterAgainstCapability(KUidMtmQueryOffLineAllowed));
	sendas.FilteredMessageTypesL(*mtmList);
	test(mtmList->Count() >= 3);
	TestMtmAvailable(sendas, mtmList, KMtmName1, EFalse);
	TestMtmAvailable(sendas, mtmList, KMtmName2, EFalse);
	TestMtmAvailable(sendas, mtmList, KMtmName3, ETrue);
	TestMtmAvailable(sendas, mtmList, KMtmName4, ETrue);
	TestMtmAvailable(sendas, mtmList, KMtmName5, ETrue);
	
	// reset
	User::LeaveIfError(sendas.ResetMessageFilter());
	sendas.FilteredMessageTypesL(*mtmList);
	test(mtmList->Count() >= KNumberOfTestMtmsCanSend);
	TestMtmAvailable(sendas, mtmList, KMtmName1, ETrue);
	TestMtmAvailable(sendas, mtmList, KMtmName2, EFalse);
	TestMtmAvailable(sendas, mtmList, KMtmName3, ETrue);
	TestMtmAvailable(sendas, mtmList, KMtmName4, ETrue);
	TestMtmAvailable(sendas, mtmList, KMtmName5, ETrue);

	// obviously both support sending
	User::LeaveIfError(sendas.FilterAgainstCapability(KUidMtmQueryCanSendMsg));
	sendas.FilteredMessageTypesL(*mtmList);
	test(mtmList->Count() >= KNumberOfTestMtmsCanSend);
	TestMtmAvailable(sendas, mtmList, KMtmName1, ETrue);
	TestMtmAvailable(sendas, mtmList, KMtmName2, EFalse);
	TestMtmAvailable(sendas, mtmList, KMtmName3, ETrue);
	TestMtmAvailable(sendas, mtmList, KMtmName4, ETrue);
	TestMtmAvailable(sendas, mtmList, KMtmName5, ETrue);

	// 0x1000 body size 
	User::LeaveIfError(sendas.FilterAgainstCapability(KUidMtmQueryMaxBodySize, 0x1000, RSendAs::ESendAsGreaterThan));
	sendas.FilteredMessageTypesL(*mtmList);
	test(mtmList->Count() >= 3);
	TestMtmAvailable(sendas, mtmList, KMtmName1, EFalse);
	TestMtmAvailable(sendas, mtmList, KMtmName2, EFalse);
	TestMtmAvailable(sendas, mtmList, KMtmName3, ETrue);
	TestMtmAvailable(sendas, mtmList, KMtmName4, ETrue);
	TestMtmAvailable(sendas, mtmList, KMtmName5, ETrue);

	// reset
	User::LeaveIfError(sendas.ResetMessageFilter());
	sendas.FilteredMessageTypesL(*mtmList);
	test(mtmList->Count() >= KNumberOfTestMtmsCanSend);
	TestMtmAvailable(sendas, mtmList, KMtmName1, ETrue);
	TestMtmAvailable(sendas, mtmList, KMtmName2, EFalse);
	TestMtmAvailable(sendas, mtmList, KMtmName3, ETrue);
	TestMtmAvailable(sendas, mtmList, KMtmName4, ETrue);
	TestMtmAvailable(sendas, mtmList, KMtmName5, ETrue);

	// 16bit body 
	User::LeaveIfError(sendas.FilterAgainstCapability(KUidMtmQuerySupportedBody, KMtm16BitBody, RSendAs::ESendAsBitwiseAnd));
	sendas.FilteredMessageTypesL(*mtmList);
	test(mtmList->Count() >= 2);
	TestMtmAvailable(sendas, mtmList, KMtmName1, ETrue);
	TestMtmAvailable(sendas, mtmList, KMtmName2, EFalse);
	TestMtmAvailable(sendas, mtmList, KMtmName3, EFalse);
	TestMtmAvailable(sendas, mtmList, KMtmName4, EFalse);
	TestMtmAvailable(sendas, mtmList, KMtmName5, ETrue);

	// reset
	User::LeaveIfError(sendas.ResetMessageFilter());
	sendas.FilteredMessageTypesL(*mtmList);
	test(mtmList->Count() >= KNumberOfTestMtmsCanSend);
	TestMtmAvailable(sendas, mtmList, KMtmName1, ETrue);
	TestMtmAvailable(sendas, mtmList, KMtmName2, EFalse);
	TestMtmAvailable(sendas, mtmList, KMtmName3, ETrue);
	TestMtmAvailable(sendas, mtmList, KMtmName4, ETrue);
	TestMtmAvailable(sendas, mtmList, KMtmName5, ETrue);

	// 0x1500 message size 
	User::LeaveIfError(sendas.FilterAgainstCapability(KUidMtmQueryMaxTotalMsgSize, 0x1500, RSendAs::ESendAsGreaterThan));
	sendas.FilteredMessageTypesL(*mtmList);
	test(mtmList->Count() >= 3);
	TestMtmAvailable(sendas, mtmList, KMtmName1, ETrue);
	TestMtmAvailable(sendas, mtmList, KMtmName2, EFalse);
	TestMtmAvailable(sendas, mtmList, KMtmName3, ETrue);
	TestMtmAvailable(sendas, mtmList, KMtmName4, EFalse);
	TestMtmAvailable(sendas, mtmList, KMtmName5, ETrue);

	//
	// recursive adding of capailities

	// reset
	User::LeaveIfError(sendas.ResetMessageFilter());
	sendas.FilteredMessageTypesL(*mtmList);
	test(mtmList->Count() >= KNumberOfTestMtmsCanSend);
	TestMtmAvailable(sendas, mtmList, KMtmName1, ETrue);
	TestMtmAvailable(sendas, mtmList, KMtmName2, EFalse);
	TestMtmAvailable(sendas, mtmList, KMtmName3, ETrue);
	TestMtmAvailable(sendas, mtmList, KMtmName4, ETrue);
	TestMtmAvailable(sendas, mtmList, KMtmName5, ETrue);

	// 0x1500 message size 
	User::LeaveIfError(sendas.FilterAgainstCapability(KUidMtmQueryMaxTotalMsgSize, 0x1500, RSendAs::ESendAsGreaterThan));
	sendas.FilteredMessageTypesL(*mtmList);
	test(mtmList->Count() >= 3);
	TestMtmAvailable(sendas, mtmList, KMtmName1, ETrue);
	TestMtmAvailable(sendas, mtmList, KMtmName2, EFalse);
	TestMtmAvailable(sendas, mtmList, KMtmName3, ETrue);
	TestMtmAvailable(sendas, mtmList, KMtmName4, EFalse);
	TestMtmAvailable(sendas, mtmList, KMtmName5, ETrue);

	// 16bit body
	User::LeaveIfError(sendas.FilterAgainstCapability(KUidMtmQuerySupportedBody, KMtm16BitBody, RSendAs::ESendAsBitwiseAnd));
	sendas.FilteredMessageTypesL(*mtmList);
	test(mtmList->Count() >= 2);
	TestMtmAvailable(sendas, mtmList, KMtmName1, ETrue);
	TestMtmAvailable(sendas, mtmList, KMtmName2, EFalse);
	TestMtmAvailable(sendas, mtmList, KMtmName3, EFalse);
	TestMtmAvailable(sendas, mtmList, KMtmName4, EFalse);
	TestMtmAvailable(sendas, mtmList, KMtmName5, ETrue);

	// 0x1000 body size 
	User::LeaveIfError(sendas.FilterAgainstCapability(KUidMtmQueryMaxBodySize, 0x1000, RSendAs::ESendAsGreaterThan));
	sendas.FilteredMessageTypesL(*mtmList);
	test(mtmList->Count() >= 1);
	TestMtmAvailable(sendas, mtmList, KMtmName1, EFalse);
	TestMtmAvailable(sendas, mtmList, KMtmName2, EFalse);
	TestMtmAvailable(sendas, mtmList, KMtmName3, EFalse);
	TestMtmAvailable(sendas, mtmList, KMtmName4, EFalse);
	TestMtmAvailable(sendas, mtmList, KMtmName5, ETrue);
	
	// support attachments
	User::LeaveIfError(sendas.FilterAgainstCapability(KUidMtmQuerySupportAttachments));
	sendas.FilteredMessageTypesL(*mtmList);
	test(mtmList->Count() >= 0);
	
	// reset
	User::LeaveIfError(sendas.ResetMessageFilter());
	sendas.FilteredMessageTypesL(*mtmList);
	test(mtmList->Count() >= KNumberOfTestMtmsCanSend);

	// support rendering
	User::LeaveIfError(sendas.FilterAgainstCapability(KUidMtmQuerySendAsRequiresRenderedImage, ETrue, RSendAs::ESendAsEquals));
	sendas.FilteredMessageTypesL(*mtmList);
	test(mtmList->Count() >= 1);
	TestMtmAvailable(sendas, mtmList, KMtmName1, EFalse);
	TestMtmAvailable(sendas, mtmList, KMtmName2, EFalse);
	TestMtmAvailable(sendas, mtmList, KMtmName3, EFalse);
	TestMtmAvailable(sendas, mtmList, KMtmName4, ETrue);
	TestMtmAvailable(sendas, mtmList, KMtmName5, EFalse);

	// reset
	User::LeaveIfError(sendas.ResetMessageFilter());
	sendas.FilteredMessageTypesL(*mtmList);
	test(mtmList->Count() >= KNumberOfTestMtmsCanSend);

	// does not support rendering
	User::LeaveIfError(sendas.FilterAgainstCapability(KUidMtmQuerySendAsRequiresRenderedImage, EFalse, RSendAs::ESendAsEquals));
	sendas.FilteredMessageTypesL(*mtmList);
	test(mtmList->Count() >= 1);
	TestMtmAvailable(sendas, mtmList, KMtmName1, EFalse);
	TestMtmAvailable(sendas, mtmList, KMtmName2, EFalse);
	TestMtmAvailable(sendas, mtmList, KMtmName3, EFalse);
	TestMtmAvailable(sendas, mtmList, KMtmName4, EFalse);
	TestMtmAvailable(sendas, mtmList, KMtmName5, ETrue);

	CleanupStack::PopAndDestroy(2, &sendas); // mtmList, sendas
	}

void doTestsL()
	{
	theUtils->Start(_L("Mtm capability filtering"));
	TestMtmSelectionL();		// Qualified: no leavescan error	
	theUtils->Complete();
	}





