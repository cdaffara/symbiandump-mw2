// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// @file 
// 
//
 
 #include <e32base.h> 

_LIT(KSqlSrvName, "sqlsrv.exe");

TInt KillProcess(const TDesC& aProcessName)
	{
	TFullName name;
	TBuf<64> pattern(aProcessName);
	TInt length = pattern.Length();
	pattern += _L("*");
	TFindProcess procFinder(pattern);

	while (procFinder.Next(name) == KErrNone)
		{
		if (name.Length() > length)
			{//If found name is a string containing aProcessName string.
			TChar c(name[length]);
			if (c.IsAlphaDigit() ||
				c == TChar('_') ||
				c == TChar('-'))
				{
				// If the found name is other valid application name
				// starting with aProcessName string.
				continue;
				}
			}
		RProcess proc;
		if (proc.Open(name) == KErrNone)
			{
			proc.Kill(0);
			}
		proc.Close();
		}
	return KErrNone;
	}

LOCAL_C void MainL(void)
	{
	(void)KillProcess(KSqlSrvName);
	}

GLDEF_C TInt E32Main()
	{
	CTrapCleanup *cleanup = CTrapCleanup::New();
	if(cleanup == NULL)
		{
		return KErrNoMemory;
		}
		
	__UHEAP_MARK;
	
	TRAPD(retval,MainL());
	
	__UHEAP_MARKEND;
	
	delete cleanup;

	if(retval == KErrArgument)
		{
		return KErrNone;
		}
	return retval;
	}
