// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Test harness 
// 
//

//  Includes...
#include <e32test.h>
#include <ipaddr.h>

//  Global functions..
//  functions()
GLDEF_C void doMainL();
GLDEF_C TInt E32Main();

//forward reference

//  Resources..
GLDEF_C RTest test(_L("BIO Test CIpAddress"));
LOCAL_D CTrapCleanup*       myCleanup;

//*****************************************************************************
//
//  Implementation; global stuff
//
//*****************************************************************************
GLDEF_C TInt E32Main()
    {
	__UHEAP_MARK;
	myCleanup = CTrapCleanup::New();
	TRAPD(err,doMainL());		
	test(err==KErrNone);
	delete myCleanup;	
	test.End();
	test.Close();
	__UHEAP_MARKEND;
	return(KErrNone);
    }

_LIT(KTestInOut1, "123.123.123.123");
_LIT(KTestInOut2, "123.5.123.12");
_LIT(KTestInOut3, "smtp.dial.pipex.com");
_LIT(KTestInOut4, "Kenny is hacking the message server again.com");
_LIT(KTestInOut5, "");
_LIT(KTestInOut6, " ");
_LIT(KTestInOut7, "  ");

_LIT(KTestIn1, "005.012.123.32");
_LIT(KTestOut1, "5.12.123.32");

_LIT(KTestIn2, "5.12.123.032");
_LIT(KTestOut2, "5.12.123.32");

_LIT(KTestIn3, "005.002.003.002");
_LIT(KTestOut3, "5.2.3.2");

_LIT(KTestIn4, "  5.2.3.2");
_LIT(KTestOut4, "5.2.3.2");

void TestL(TInt aCurTest, const TDesC& aIn, const TDesC& aOut);

void TestInOutL(TInt aCurTest, const TDesC& aInOut)
	{
	TestL(aCurTest, aInOut, aInOut);
	}

void RunTestsL()
	{
	TInt curTest=0;

	TestInOutL(++curTest, KTestInOut1);
	TestInOutL(++curTest, KTestInOut2);
	TestInOutL(++curTest, KTestInOut3);
	TestInOutL(++curTest, KTestInOut4);
	TestInOutL(++curTest, KTestInOut5);
	TestInOutL(++curTest, KTestInOut6);
	TestInOutL(++curTest, KTestInOut7);

	TestL(++curTest, KTestIn1, KTestOut1);
	TestL(++curTest, KTestIn2, KTestOut2);
	TestL(++curTest, KTestIn3, KTestOut3);
	TestL(++curTest, KTestIn4, KTestOut4);
	}

void TestL(TInt aCurTest, const TDesC& aIn, const TDesC& aOut)
	{
	test.Printf(_L("Test %d\n\tIn=\"%S\"\n"), aCurTest, &aIn);
	CIpAddress* ip = CIpAddress::NewLC(aIn);
	test.Printf(_L("\tExpOut=\"%S\"\n"), &aOut);
	test.Printf(_L("\tActOut=\"%S\"\n"), &ip->Addr());
	test(ip->Addr().Compare(aOut)==0);
	CleanupStack::PopAndDestroy(ip);
	}


GLDEF_C void doMainL()
    {
    test.Title();
    test.Start(_L("Test IP Addr"));
	RunTestsL();	// Qualified: comment to ignore false leavescan error
    }
