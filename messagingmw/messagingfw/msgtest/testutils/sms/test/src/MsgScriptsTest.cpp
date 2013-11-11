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
//

#include <e32std.h>
#include <e32test.h>
#include <msgtestscripts.h>

_LIT(KSmsConfigFile, "c:\\test\\sms\\msgscriptstest.config");
RTest test(_L("MsgTestScripts"));
RFs fs;

_LIT(KSmsItemA, "Name");
_LIT(KSmsItemB, "Recipients");
_LIT(KSmsItemC, "Self");
_LIT(KSmsItemD, "Message");
_LIT(KSmsItemE, "Encoding");

_LIT(KSmsSection1, "Group");
_LIT(KSmsValue1A, "Anthony");
_LIT(KSmsValue1B, "+447747065431");
_LIT(KSmsValue1C, "+447747065499");
_LIT(KSmsValue1D, "");
const TInt KSmsValue1E = 0;

_LIT(KSmsSection2, "NextGroup");
_LIT(KSmsValue2A, "Bill");
_LIT(KSmsValue2B, "+447747065445");
_LIT(KSmsValue2C, "+447747065498");
_LIT(KSmsValue2D, "This\nis\na\nmultiline\nmessage");
const TInt KSmsValue2E = 7;

_LIT(KSmsSection3, "LastGroup");
_LIT(KSmsValue3A, "");
_LIT(KSmsValue3B, "");
_LIT(KSmsValue3C, "");
_LIT(KSmsValue3D, "");
const TInt KSmsValue3E = 0;

void TestScriptFileL()
	{
	CScriptFile* scriptFile = CScriptFile::NewLC(fs, KSmsConfigFile);

	TPtrC itemValue(KNullDesC);
	TInt intValue(0);

	//Test Section1
	scriptFile->ItemValue(KSmsSection1, KSmsItemA, itemValue, KNullDesC);
	test(itemValue == KSmsValue1A);

	scriptFile->ItemValue(KSmsSection1, KSmsItemB, itemValue, KNullDesC);
	test(itemValue == KSmsValue1B);

	scriptFile->ItemValue(KSmsSection1, KSmsItemC, itemValue, KNullDesC);
	test(itemValue == KSmsValue1C);

	scriptFile->ItemValue(KSmsSection1, KSmsItemD, itemValue, KNullDesC);
	test(itemValue == KSmsValue1D);

	scriptFile->ItemValue(KSmsSection1, KSmsItemE, intValue, 0);
	test(intValue == KSmsValue1E);

	//Test Section2
	scriptFile->ItemValue(KSmsSection2, KSmsItemA, itemValue, KNullDesC);
	test(itemValue == KSmsValue2A);

	scriptFile->ItemValue(KSmsSection2, KSmsItemB, itemValue, KNullDesC);
	test(itemValue == KSmsValue2B);

	scriptFile->ItemValue(KSmsSection2, KSmsItemC, itemValue, KNullDesC);
	test(itemValue == KSmsValue2C);

	scriptFile->ItemValue(KSmsSection2, KSmsItemD, itemValue, KNullDesC);
	test(itemValue == KSmsValue2D);

	scriptFile->ItemValue(KSmsSection2, KSmsItemE, intValue, 0);
	test(intValue == KSmsValue2E);

	//Test Section3
	scriptFile->ItemValue(KSmsSection3, KSmsItemA, itemValue, KNullDesC);
	test(itemValue == KSmsValue3A);

	scriptFile->ItemValue(KSmsSection3, KSmsItemB, itemValue, KNullDesC);
	test(itemValue == KSmsValue3B);

	scriptFile->ItemValue(KSmsSection3, KSmsItemC, itemValue, KNullDesC);
	test(itemValue == KSmsValue3C);

	scriptFile->ItemValue(KSmsSection3, KSmsItemD, itemValue, KNullDesC);
	test(itemValue == KSmsValue3D);

	scriptFile->ItemValue(KSmsSection3, KSmsItemE, intValue, 0);
	test(intValue == KSmsValue3E);

	CleanupStack::PopAndDestroy(scriptFile);
	}

void TestHeapFailL()
	{
	TInt error;
	TInt failCount = 0;

	//Failure Test CSmsSettings::NewL
	do
		{
		__UHEAP_FAILNEXT(failCount++);

		TRAP(error, TestScriptFileL());

		__UHEAP_RESET;

		if (error)
			{
			test(error == KErrNoMemory);
			}

		}
	while (error);

	test.Printf(_L("TestScriptFileL() Fail Count %d\n"), failCount);

	failCount = 0;
	}

void doMainL()
	{
	TestScriptFileL();
	TestHeapFailL();
	}

GLDEF_C TInt E32Main()
	{
	__UHEAP_MARK;
	test.Start(_L("Setup"));
	CTrapCleanup* theCleanup = CTrapCleanup::New();
	TInt ret = fs.Connect();
	if (!ret)
		TRAP(ret,doMainL());		
	test(ret==KErrNone);
	fs.Close();
	delete theCleanup;	
	test.Console()->SetPos(0, 13);
	test.End();
	test.Close();
	__UHEAP_MARKEND;
	return(KErrNone);
	}

