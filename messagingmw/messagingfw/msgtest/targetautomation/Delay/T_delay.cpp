// Copyright (c) 1998-2009 Nokia Corporation and/or its subsidiary(-ies).
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
#include <bacline.h>
#include <e32test.h>
#include <kernel\u32std.h>
#include <e32svr.h>

RTest test(_L("T_DELAY"));

const TInt KMsgDefaultBatchFileDelay=20000000;	// 20 seconds
//const TInt KMsgDefaultBatchFileDelay=5000000;	// 5 seconds

LOCAL_C void doDelayL()
	{
//	HAL::SetAutoSwitchOffBehavior(ESwitchOffDisabled);	// Can't do this anymore :<
	User::ResetInactivityTime();

	// Get command line.
	CCommandLineArguments* parsed=CCommandLineArguments::NewLC();
	// Check for numerical arguments.
	TInt theDelay=KMsgDefaultBatchFileDelay;
	if (parsed->Count()==2)
		{
		TLex convArg(parsed->Arg(1));
		TInt parsedDelay;
		if (convArg.Val(parsedDelay)==KErrNone)
			theDelay=parsedDelay*1000000;
		}
	// Delay.
	User::After(theDelay);

	User::ResetInactivityTime();

	CleanupStack::PopAndDestroy(parsed);
	}

GLDEF_C TInt E32Main()
	{	
	__UHEAP_MARK;
	test.Title();
	CTrapCleanup* theCleanup=CTrapCleanup::New();

	TInt totalRamSize;
	TInt freeRamSize;
	TMemoryInfoV1 info;
	TPckg<TMemoryInfoV1> infoPckg(info);
	TInt r=UserSvr::HalGet(EMemoryInfo, (TAny*)&infoPckg);
	if (r==KErrNone)
		{
		totalRamSize=info.iTotalRamInBytes;
		freeRamSize=info.iFreeRamInBytes;
		test.Printf(_L("TotalRam: 0x%8x\n"),totalRamSize);
		test.Printf(_L("FreeRam:  0x%8x\n"),freeRamSize);
		}

	
	TRAPD(ret,doDelayL());          
	delete theCleanup;      
	test.Close();
	__UHEAP_MARKEND;
	return(KErrNone);
	}

