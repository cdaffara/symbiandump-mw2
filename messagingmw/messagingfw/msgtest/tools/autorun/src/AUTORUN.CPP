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

#include <e32test.h>
#include <f32file.h>

#include <msvapi.h>

_LIT(KMsvServerPattern, "!MsvServer*");
RTest test(_L("AutoRun"));

//


LOCAL_C void WaitForServerClose()
	{
	// Wait 10 seconds for server to exit
	TInt count = 10;
	while(count--)
		{
		TFullName name;
		TFindProcess find(KMsvServerPattern);
		if (find.Next(name) != KErrNone)
			return;
		else if (count <= 0)
			{
			RProcess process;
			process.Open(find);
			process.Terminate(KErrGeneral);
			process.Close();
			}
		User::After(1000000);
		}
	}

LOCAL_C void LaunchAndWaitL(const TDesC& file)
	{
	RProcess p;
	test(p.Create(file, KNullDesC) == KErrNone);

	TRequestStatus status = KRequestPending;
	p.Logon(status);
	p.Resume();
	User::WaitForRequest(status);
	p.Close();

	WaitForServerClose();	
	}

LOCAL_C void ScanDirL(RFs& aFs, const TDesC& aDir)
	{
	User::After(1000000);

	TParse parse;
	parse.Set(_L("t_*.exe"), &aDir, NULL);
	TPtrC spec(parse.FullName());

	test.Start(parse.DriveAndPath());
	TFindFile find(aFs);
	CDir* dir;
	
	if (!find.FindWildByPath(parse.FullName(), NULL, dir))
		{
		CleanupStack::PushL(dir);

		for(int i = 0; i < dir->Count(); i++)
			{
			parse.Set((*dir)[i].iName, &spec, NULL);

			if (i == 0)
				test.Start(parse.FullName());
			else
				test.Next(parse.FullName());

			LaunchAndWaitL(parse.FullName());
			}

		CleanupStack::PopAndDestroy(); // dir
		}
	test.End();
	}

LOCAL_C void ScanStartL(RFs aFs, const TDesC& aPath)
	{
	// Scan the top-level directory for test executables
	ScanDirL(aFs, aPath);

	// Setup directory scan
	CDirScan* scan = CDirScan::NewLC(aFs);
	scan->SetScanDataL(aPath, KEntryAttDir|KEntryAttMatchExclusive, ESortNone, CDirScan::EScanUpTree);

	// Iterate through all the directories
	FOREVER
		{
		// Get next directory list
		CDir* dir = NULL;
		TRAPD(error, scan->NextL(dir));
		if (error || !dir)
			break;
		
		delete dir;
		ScanDirL(aFs, scan->FullPath());
		};
	CleanupStack::PopAndDestroy(); // scan
	}

LOCAL_C void doMainL()
	{
	CActiveScheduler::Install(new(ELeave)CActiveScheduler);

	RFs fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);

	ScanStartL(fs, RProcess().FileName());
	CleanupStack::PopAndDestroy(); // fs

	delete CActiveScheduler::Current();
	}


GLDEF_C TInt E32Main()
	{
	__UHEAP_MARK;
	CTrapCleanup* cleanup = CTrapCleanup::New();
	__ASSERT_ALWAYS(cleanup, User::Invariant());
	TRAPD(ret,doMainL());		
	test(ret == KErrNone);
	test.Close();
	delete cleanup;	
	__UHEAP_MARKEND;
	return(KErrNone);
	}



