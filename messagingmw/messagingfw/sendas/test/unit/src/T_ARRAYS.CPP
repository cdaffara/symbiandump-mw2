// Copyright (c) 2000-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <s32mem.h>
#include "sendas2test.h"

RTest test(_L("SendAs2 Arrays"));

//

LOCAL_C void TestSendAsAccountsL()
	{
	CSendAsAccounts* accounts = CSendAsAccounts::NewL();
	CleanupStack::PushL(accounts);

	// test adding	
	test(accounts->Count()==0);
	accounts->AppendAccountL(_L("SendAsMtm1"), 0xaaaaaaaa);
	test(accounts->Count()==1);
	accounts->AppendAccountL(_L("SendAsMtm2"), 0x55555555);
	test(accounts->Count()==2);
	accounts->AppendAccountL(_L("SendAsMtm3"), 0x11111111);
	test(accounts->Count()==3);
	accounts->AppendAccountL(_L("SendAsMtm4"), 0x22222222);
	test(accounts->Count()==4);

	// test Account() member
	test(accounts->Account(0) == 0xaaaaaaaa);
	test(accounts->Account(1) == 0x55555555);
	test(accounts->Account(2) == 0x11111111);
	test(accounts->Account(3) == 0x22222222);

	// test Reset() member
	accounts->Reset();
	test(accounts->Count()==0);
	
	// repopulate
	accounts->AppendAccountL(_L("SendAsMtm1"), 0xaaaaaaaa);
	accounts->AppendAccountL(_L("SendAsMtm2"), 0x55555555);
	accounts->AppendAccountL(_L("SendAsMtm3"), 0x11111111);
	accounts->AppendAccountL(_L("SendAsMtm4"), 0x22222222);

	// test AccountNames() member
	test(accounts->AccountNames().MdcaPoint(0)==_L("SendAsMtm1"));
	test(accounts->AccountNames().MdcaPoint(1)==_L("SendAsMtm2"));
	test(accounts->AccountNames().MdcaPoint(2)==_L("SendAsMtm3"));
	test(accounts->AccountNames().MdcaPoint(3)==_L("SendAsMtm4"));

	// test SetMessageType() / GetMessageType()
	TUid testUid = TUid::Uid(0x5a5a5a5a);
	accounts->SetMessageType(testUid);
	test(accounts->MessageType() == testUid);

	// test RemoveAccount() member
	accounts->RemoveAccount(1);	
	test(accounts->Count() == 3);
	test(accounts->AccountNames().MdcaPoint(0) == _L("SendAsMtm1"));
	test(accounts->AccountNames().MdcaPoint(1) == _L("SendAsMtm3"));
	accounts->RemoveAccount(0);	
	test(accounts->AccountNames().MdcaPoint(0) == _L("SendAsMtm3"));
	accounts->RemoveAccount(1);	
	test(accounts->AccountNames().MdcaPoint(0) == _L("SendAsMtm3"));
	accounts->RemoveAccount(0);	
	test(accounts->Count() == 0);
	accounts->Reset();
	
	// repopulate
	accounts->AppendAccountL(_L("SendAsMtm1"), 0xaaaaaaaa);
	accounts->AppendAccountL(_L("SendAsMtm2"), 0x55555555);
	accounts->AppendAccountL(_L("SendAsMtm3"), 0x11111111);
	accounts->AppendAccountL(_L("SendAsMtm4"), 0x22222222);
	accounts->SetMessageType(testUid);

	// test name lookup by account id
	test(accounts->NameFromAccountL(0xaaaaaaaa) == _L("SendAsMtm1"));
	test(accounts->NameFromAccountL(0x55555555) == _L("SendAsMtm2"));
	test(accounts->NameFromAccountL(0x11111111) == _L("SendAsMtm3"));
	test(accounts->NameFromAccountL(0x22222222) == _L("SendAsMtm4"));
	// test non-existent entry
	TRAPD(err1, accounts->NameFromAccountL(0x12121212));
	test(err1 == KErrNotFound);

	// test account id lookup by name
	test(accounts->AccountFromNameL(_L("SendAsMtm1")) == 0xaaaaaaaa);
	test(accounts->AccountFromNameL(_L("SendAsMtm2")) == 0x55555555);
	test(accounts->AccountFromNameL(_L("SendAsMtm3")) == 0x11111111);
	test(accounts->AccountFromNameL(_L("SendAsMtm4")) == 0x22222222);
	// test unknowns
	TRAPD(err2, accounts->AccountFromNameL(_L("Not there")));
	test(err2 == KErrNotFound);
	
	// test Size() member
	TInt size = accounts->Size();
	test(size == 122);
	HBufC8* accBuf = HBufC8::NewLC(size);
	TPtr8 ptr(accBuf->Des());

	// test ExternalizeL() member
	RDesWriteStream writeStream(ptr);
	CleanupClosePushL(writeStream);
	accounts->ExternalizeL(writeStream);
	writeStream.CommitL();
	CleanupStack::PopAndDestroy(&writeStream);
	
	CSendAsAccounts* accounts2 = CSendAsAccounts::NewL();
	CleanupStack::PushL(accounts2);
	
	// test InternalizeL() member
	RDesReadStream readStream(*accBuf);
	CleanupClosePushL(readStream);
	accounts2->InternalizeL(readStream);
	CleanupStack::PopAndDestroy(&readStream);

	// compare 
	test(accounts->Size() == accounts2->Size());
	test(accounts->AccountNames().MdcaPoint(0) == accounts2->AccountNames().MdcaPoint(0));
	test(accounts->AccountNames().MdcaPoint(1) == accounts2->AccountNames().MdcaPoint(1));
	test(accounts->AccountNames().MdcaPoint(2) == accounts2->AccountNames().MdcaPoint(2));
	test(accounts->AccountNames().MdcaPoint(3) == accounts2->AccountNames().MdcaPoint(3));
	test(accounts->Account(0) == accounts2->Account(0));
	test(accounts->Account(1) == accounts2->Account(1));
	test(accounts->Account(2) == accounts2->Account(2));
	test(accounts->Account(3) == accounts2->Account(3));
	test(accounts->MessageType() == accounts2->MessageType());
	CleanupStack::PopAndDestroy(accounts2);

	// clean up
	CleanupStack::PopAndDestroy(2, accounts); // accounts, accBuf
	}

LOCAL_C void TestSendAsMessageTypesL()
	{
	CSendAsMessageTypes* msgtypes = CSendAsMessageTypes::NewL();
	CleanupStack::PushL(msgtypes);
	
	// test adding	
	test(msgtypes->Count()==0);
	msgtypes->AppendMessageTypeL(_L("SendAsMtm1"), TUid::Uid(0xaaaaaaaa));
	test(msgtypes->Count()==1);
	msgtypes->AppendMessageTypeL(_L("SendAsMtm2"), TUid::Uid(0x55555555));
	test(msgtypes->Count()==2);
	msgtypes->AppendMessageTypeL(_L("SendAsMtm3"), TUid::Uid(0x11111111));
	test(msgtypes->Count()==3);
	msgtypes->AppendMessageTypeL(_L("SendAsMtm4"), TUid::Uid(0x22222222));
	test(msgtypes->Count()==4);

	// test Account() member
	test(msgtypes->MessageTypeUid(0) == TUid::Uid(0xaaaaaaaa));
	test(msgtypes->MessageTypeUid(1) == TUid::Uid(0x55555555));
	test(msgtypes->MessageTypeUid(2) == TUid::Uid(0x11111111));
	test(msgtypes->MessageTypeUid(3) == TUid::Uid(0x22222222));

	// test Reset() member
	msgtypes->Reset();
	test(msgtypes->Count()==0);
	
	// repopulate
	msgtypes->AppendMessageTypeL(_L("SendAsMtm1"), TUid::Uid(0xaaaaaaaa));
	msgtypes->AppendMessageTypeL(_L("SendAsMtm2"), TUid::Uid(0x55555555));
	msgtypes->AppendMessageTypeL(_L("SendAsMtm3"), TUid::Uid(0x11111111));
	msgtypes->AppendMessageTypeL(_L("SendAsMtm4"), TUid::Uid(0x22222222));

	// test AccountNames() member
	test(msgtypes->AvailableMessageTypes().MdcaPoint(0)==_L("SendAsMtm1"));
	test(msgtypes->AvailableMessageTypes().MdcaPoint(1)==_L("SendAsMtm2"));
	test(msgtypes->AvailableMessageTypes().MdcaPoint(2)==_L("SendAsMtm3"));
	test(msgtypes->AvailableMessageTypes().MdcaPoint(3)==_L("SendAsMtm4"));

	// test RemoveAccount() member
	msgtypes->RemoveMessageType(1);	
	test(msgtypes->Count() == 3);
	test(msgtypes->AvailableMessageTypes().MdcaPoint(0) == _L("SendAsMtm1"));
	test(msgtypes->AvailableMessageTypes().MdcaPoint(1) == _L("SendAsMtm3"));
	msgtypes->RemoveMessageType(0);	
	test(msgtypes->AvailableMessageTypes().MdcaPoint(0) == _L("SendAsMtm3"));
	msgtypes->RemoveMessageType(1);	
	test(msgtypes->AvailableMessageTypes().MdcaPoint(0) == _L("SendAsMtm3"));
	msgtypes->RemoveMessageType(0);	
	test(msgtypes->Count() == 0);
	msgtypes->Reset();
	
	// repopulate
	msgtypes->AppendMessageTypeL(_L("SendAsMtm1"), TUid::Uid(0xaaaaaaaa));
	msgtypes->AppendMessageTypeL(_L("SendAsMtm2"), TUid::Uid(0x55555555));
	msgtypes->AppendMessageTypeL(_L("SendAsMtm3"), TUid::Uid(0x11111111));
	msgtypes->AppendMessageTypeL(_L("SendAsMtm4"), TUid::Uid(0x22222222));
	
	// test name lookup by mtm uid
	test(msgtypes->NameFromUidL(TUid::Uid(0xaaaaaaaa)) == _L("SendAsMtm1"));
	test(msgtypes->NameFromUidL(TUid::Uid(0x55555555)) == _L("SendAsMtm2"));
	test(msgtypes->NameFromUidL(TUid::Uid(0x11111111)) == _L("SendAsMtm3"));
	test(msgtypes->NameFromUidL(TUid::Uid(0x22222222)) == _L("SendAsMtm4"));
	// test non-existent entry
	TRAPD(err1, msgtypes->NameFromUidL(TUid::Uid(0x21212121)));
	test(err1 == KErrNotFound);

	// test mtm uid lookup by name
	test(msgtypes->UidFromNameL(_L("SendAsMtm1")) == TUid::Uid(0xaaaaaaaa));
	test(msgtypes->UidFromNameL(_L("SendAsMtm2")) == TUid::Uid(0x55555555));
	test(msgtypes->UidFromNameL(_L("SendAsMtm3")) == TUid::Uid(0x11111111));
	test(msgtypes->UidFromNameL(_L("SendAsMtm4")) == TUid::Uid(0x22222222));
	// test non-existent entry
	TRAPD(err2, msgtypes->UidFromNameL(_L("not there")));
	test(err2 == KErrNotFound);
	
	// test Size() member
	TInt size = msgtypes->Size();
	test(size == 118);
	HBufC8* accBuf = HBufC8::NewLC(size);
	TPtr8 ptr(accBuf->Des());

	// test ExternalizeL() member
	RDesWriteStream writeStream(ptr);
	CleanupClosePushL(writeStream);
	msgtypes->ExternalizeL(writeStream);
	writeStream.CommitL();
	CleanupStack::PopAndDestroy(&writeStream);
	
	CSendAsMessageTypes* msgtypes2 = CSendAsMessageTypes::NewL();
	CleanupStack::PushL(msgtypes2);

	// test InternalizeL() member
	RDesReadStream readStream(*accBuf);
	CleanupClosePushL(readStream);
	msgtypes2->InternalizeL(readStream);
	CleanupStack::PopAndDestroy(&readStream);

	// compare 
	test(msgtypes->Size() == msgtypes2->Size());
	test(msgtypes->AvailableMessageTypes().MdcaPoint(0) == msgtypes2->AvailableMessageTypes().MdcaPoint(0));
	test(msgtypes->AvailableMessageTypes().MdcaPoint(1) == msgtypes2->AvailableMessageTypes().MdcaPoint(1));
	test(msgtypes->AvailableMessageTypes().MdcaPoint(2) == msgtypes2->AvailableMessageTypes().MdcaPoint(2));
	test(msgtypes->AvailableMessageTypes().MdcaPoint(3) == msgtypes2->AvailableMessageTypes().MdcaPoint(3));
	test(msgtypes->MessageTypeUid(0) == msgtypes2->MessageTypeUid(0));
	test(msgtypes->MessageTypeUid(1) == msgtypes2->MessageTypeUid(1));
	test(msgtypes->MessageTypeUid(2) == msgtypes2->MessageTypeUid(2));
	test(msgtypes->MessageTypeUid(3) == msgtypes2->MessageTypeUid(3));
	CleanupStack::PopAndDestroy(msgtypes2);

	// clean up
	CleanupStack::PopAndDestroy(2, msgtypes); // msgtypes, accBuf
	}
	
GLDEF_C void doTestsL()
	{	
	theUtils->Start(_L("Accounts"));
	TestSendAsAccountsL();		// Qualified: No leavescan error
	theUtils->Complete();
	
	theUtils->Start(_L("Message Types"));
	TestSendAsMessageTypesL();	// Qualified: No leavescan error
	theUtils->Complete();
	}
