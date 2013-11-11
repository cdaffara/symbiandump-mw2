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

#include <e32test.h>
#include <e32std.h>

RTest test(_L("KillSysAgent Testrig"));
CTrapCleanup* theCleanup;

_LIT(KSysAgent, "SYSAG*");

LOCAL_C void doMainL()
	{
	test.Printf(_L("Killing process \"%S\"\n"), &KSysAgent);

	TFullName name;
	TFindProcess find(KSysAgent);
	
	TInt err = find.Next(name);

	test.Printf(_L("find.Next() returned %d\n"), err);

	if (!err)
		{
		RProcess process;
		err = process.Open(find);

		test.Printf(_L("process.Open() returned %d\n"), err);

		if (!err)
			{
			process.Terminate(KErrGeneral);
			process.Close();	
			}
		}

	test.Printf(_L("Press any key to continue..."));
	test.Getch();
	}

GLDEF_C TInt E32Main()
	{	
	__UHEAP_MARK;
	test.Start(_L("Setup"));
	theCleanup = CTrapCleanup::New();
	TRAPD(ret,doMainL());		
	test(ret==KErrNone);
	delete theCleanup;	
	test.Console()->SetPos(0, 13);
	test.End();
	test.Close();
	__UHEAP_MARKEND;
	return(KErrNone);
	}
