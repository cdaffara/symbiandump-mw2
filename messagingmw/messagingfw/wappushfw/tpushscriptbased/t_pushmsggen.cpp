// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// T_AUTOCP.CPP
// 
//

#include <s32file.h>
#include <e32test.h>


#include "wappushmsggen.h"



class RWapPushMsgGenTest : public RTest
	{
public:
	RWapPushMsgGenTest();
	void TestL();
	void Close();
private:
	void Test(TInt aResult);
private:
	};

RWapPushMsgGenTest::RWapPushMsgGenTest():
	RTest(_L("WapPushMsgGen Test Harness"))
	{}


void RWapPushMsgGenTest::Test(TInt aResult)
	{
	operator()(aResult,__LINE__); 
	}

void RWapPushMsgGenTest::TestL()
	{
	Start(_L("@SYMTestCaseID IWS-WAPBROWSER-WAPPUSH-TPUSHSCRIPTBASED-T_PUSHMSGGEN-0001 Testing Wap Push Message Generation"));

	CWapPushMsgGen* msgGen = CWapPushMsgGen::NewL();
	CleanupStack::PushL(msgGen);

	msgGen->StartNewMsgL(CWapPushMsgGen::EServiceIndication);
	msgGen->AppendFieldL(CWapPushMsgGen::EHRef, _L8("http://www.xyz.com/email/123/abc.wml"));
	msgGen->AppendFieldL(CWapPushMsgGen::EServiceIndicationCreated, _L8("1999-06-25T15.23.15Z"));
	msgGen->AppendFieldL(CWapPushMsgGen::EServiceIndicationExpires, _L8("1999-06-30t00.00Z"));
	msgGen->AppendFieldL(CWapPushMsgGen::EServiceIndicationText, _L8("You have 4 new e-mails"));

	HBufC8* msg = msgGen->CreateMsgLC();
	TPtrC8 msgptr(*msg);
	CleanupStack::PopAndDestroy(); //msg


	msgGen->StartNewMsgL(CWapPushMsgGen::EServiceLoad);
	msgGen->AppendFieldL(CWapPushMsgGen::EHRef, _L8("http://www.xyz.com/ppaidl/123/abc.wml"));
	msgGen->AppendFieldL(CWapPushMsgGen::EAction, _L8("execute-high"));

	HBufC8* loadmsg = msgGen->CreateMsgLC();

	CleanupStack::PopAndDestroy(2); // loadMsg, msgGen;
	End();
	}

void RWapPushMsgGenTest::Close()
	{					
	//Do any deleting here
	RTest::Close();
	}



GLDEF_C TInt E32Main()
	{
	CTrapCleanup* tc=CTrapCleanup::New();
	RWapPushMsgGenTest tester;
	__UHEAP_MARK;
	TRAPD(err,tester.TestL());
	if (err!=KErrNone)
		User::Panic(_L("Failed to complete test"),err);
	tester.Close();
	delete tc;
	User::Heap().Check();
	__UHEAP_MARKEND;
	return KErrNone;
	}

