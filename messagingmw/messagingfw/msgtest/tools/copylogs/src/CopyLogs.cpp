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

#include <f32file.h>
#include <e32cons.h>

LOCAL_C TBool AskL(const TDesC& aSrcFile, const TDesC& aDstFile)
	{
	const TInt KMaxLineSize = 256;

	// Open connection to notify server
	RNotifier notifier;
	User::LeaveIfError(notifier.Connect());
	CleanupClosePushL(notifier);

	// Generate line1
	TBuf<KMaxLineSize> line1;
	_LIT(KLine1, "Copy file: '%S'");
	line1.Format(KLine1, &aSrcFile);

	// Generate line 2
	TBuf<KMaxLineSize> line2;
	_LIT(KLine2, "To: '%S'?");	
	line2.Format(KLine2, &aDstFile);

	// Button text
	_LIT(KYesButton, "Yes");
	_LIT(KNoButton, "No");

	// Display the dialog
	TInt button;
	TRequestStatus status;
	notifier.Notify(line1, line2, KNoButton, KYesButton, button, status);
	User::WaitForRequest(status);

	CleanupStack::PopAndDestroy(); // notifier

	// Did they click yes?
	return button == 1;
	}

LOCAL_C void CopyFileL(RFs& aFs, const TDesC& aSrcFile, const TDesC& aDstFile)
	{
	// Buffer size
	const TInt KBufferSize = 1;

	// Check if want to copy the file
	if (!AskL(aSrcFile, aDstFile))
		return;

	// Get source file size
	TEntry entry;
	User::LeaveIfError(aFs.Entry(aSrcFile, entry));

	// Create a buffer to hold file data
	HBufC8* contents = HBufC8::NewLC(entry.iSize);

	// Make sure destination directory exists
	TInt error = aFs.MkDirAll(aDstFile);
	if (error != KErrAlreadyExists)
		User::LeaveIfError(error);

	// Create progress window
	_LIT(KProgressTitle, "Copy Progress");
	CConsoleBase* console = Console::NewL(KProgressTitle, TSize(KConsFullScreen, KConsFullScreen));
	CleanupStack::PushL(console);

	// Read in the data
	TInt pos = 0;
	while(pos < entry.iSize)
		{
		TBuf8<KBufferSize> buf;
		User::LeaveIfError(aFs.ReadFileSection(aSrcFile, pos, buf, Min(KBufferSize, entry.iSize - pos)));

		contents->Des().Append(buf);
		pos += KBufferSize;

		// Display Progress
		if (pos % (1<<10) == 0)
			console->Printf(_L("Copied %dk of %dk\n"), pos>>10, entry.iSize>>10);
		}

	CleanupStack::PopAndDestroy(); // console

	// Open the file
	RFile file;
	User::LeaveIfError(file.Replace(aFs, aDstFile, EFileShareExclusive | EFileWrite));
	CleanupClosePushL(file);

	// Dump the data to file
	User::LeaveIfError(file.Write(*contents));
	User::LeaveIfError(file.Flush());

	CleanupStack::PopAndDestroy(2); // file, contents
	}

LOCAL_C void CopyDirL(RFs& aFs, const TDesC& aSrcDir, const TDesC& aDstDir)
	{
	// Generate file specification
	TParse spec;
	User::LeaveIfError(spec.Set(_L("*"), &aSrcDir, NULL));

	TFindFile find(aFs);
	CDir* dir;
	
	// Find the files
	TInt error = find.FindWildByPath(spec.FullName(), NULL, dir);
	if (error == KErrNone)
		{
		CleanupStack::PushL(dir);

		// Iterate through all the files
		for(TInt i = 0; i < dir->Count(); i++)
			{
			if (!(*dir)[i].IsDir())
				{
				// Generate source path
				TParse srcParse;
				User::LeaveIfError(srcParse.Set((*dir)[i].iName, &aSrcDir, NULL));

				// Generate destination path
				TParse dstParse;
				User::LeaveIfError(dstParse.Set((*dir)[i].iName, &aDstDir, NULL));

				// Copy the file
				CopyFileL(aFs, srcParse.FullName(), dstParse.FullName());
				}
			}

		CleanupStack::PopAndDestroy(); // dir
		}

	// Did an error occur?
	if (error != KErrNotFound)
		User::LeaveIfError(error);
	}

LOCAL_C void CopyLogsL(RFs& aFs, const TDesC& aSrcDir, const TDesC& aDstDir)
	{
	// Copy any files from root folder
	CopyDirL(aFs, aSrcDir, aDstDir);

	// Setup directory scan
	CDirScan* scan = CDirScan::NewLC(aFs);
	scan->SetScanDataL(aSrcDir, KEntryAttDir|KEntryAttMatchExclusive, ESortNone, CDirScan::EScanUpTree);

	// Iterate through all the directories
	FOREVER
		{
		// Get next directory list
		CDir* dir = NULL;
		TRAPD(error, scan->NextL(dir));
		delete dir;

		if (!(error == KErrNone && dir))
			break;

		// Copy the directory
		CopyDirL(aFs, scan->FullPath(), aDstDir);
		};

	CleanupStack::PopAndDestroy(); // scan
	}

LOCAL_C void DoMainL()
	{
	// Create scheduler
	CActiveScheduler* scheduler = new(ELeave)CActiveScheduler;
	CleanupStack::PushL(scheduler);
	CActiveScheduler::Install(scheduler);

	// Create file session
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);

	_LIT(KSrcLogFolder, "c:\\logs\\");

	// Copy the logs
	CopyLogsL(fs, KSrcLogFolder(), RProcess().FileName());

	CleanupStack::PopAndDestroy(2); // fs, scheduler
	}

GLDEF_C TInt E32Main()
	{
	__UHEAP_MARK;
	CTrapCleanup* cleanup = CTrapCleanup::New();
	__ASSERT_ALWAYS(cleanup, User::Invariant());
	TRAPD(error, DoMainL());		
	delete cleanup;
	__UHEAP_MARKEND;
	return(KErrNone);
	}



