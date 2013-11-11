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

#include <msvids.h>
#include "msvindexadapter.h"
#include <msventry.h>
#include "MSERVER.H"
#include "msvutils.h"
#include "Testsetup.h"

#pragma warning (disable : 4238) //warning C4238: nonstandard extension used : class rvalue used as lvalue

RTest test(_L("Index rebuilding Testrig"));
CTestTimer* theTimer;

_LIT(KTestIndexFile ,"\\Index rebuild testing\\Index");

#define PAUSE(a) {theTimer->After(a); CActiveScheduler::Start();}

//
LOCAL_C void CorruptINIFile()
	{
	TChar driveChar=theUtils->FileSession().GetSystemDriveChar();
	TBuf<2> systemDrive;
	systemDrive.Append(driveChar);
	systemDrive.Append(KDriveDelimiter);
    TPath pathName(systemDrive);
	pathName.Append(KServerINIFile);
	CDictionaryFileStore *store=CDictionaryFileStore::OpenLC(theUtils->FileSession(),pathName,KNullUid);
	RDictionaryWriteStream stream;
	stream.AssignLC(*store, KUidMsvMessageDriveStream);
	
	stream.WriteUint8L(KMsvMessageDriveStreamVersionNumber + 2); // version number
	stream << _L("C:");
	stream.WriteUint32L(0x1111); 	// bogus number for corruption
	stream.WriteInt8L(1);			// bogus number for corruption
	
	stream.CommitL();
	store->CommitL();
	CleanupStack::PopAndDestroy(2,store); // stream, store		
}
	
	
LOCAL_C void TestInitialCreationL()
	{
	CMsvServer* server;

	// non-existant directory
	CMsgsTestUtils::CleanMessageFolderL(KTestIndexFile);
	server=CMsvServer::NewL();
	PAUSE(100000);
	delete server;

	// only directory exists
	CMsgsTestUtils::CleanMessageFolderL(KTestIndexFile);
	theUtils->FileSession().MkDirAll(KTestIndexFile);
	server=CMsvServer::NewL();
	PAUSE(100000);
	delete server;

	// heap failure when constructing index
	CMsgsTestUtils::CleanMessageFolderL(KTestIndexFile);
	TInt failCount=0;
	TBool failed=ETrue;
	TUint at;
	TInt error;
	while (failed)
		{
		__UHEAP_FAILNEXT(failCount++);
		TRAP(error, server=CMsvServer::NewL());
		__UHEAP_RESET;
		PAUSE(100000);
		if (error)
			{
			TUint at;
			TInt error = theUtils->FileSession().Att(KTestIndexFile, at);
			test(error==KErrNotFound||error==KErrPathNotFound);
			}
		else
			failed=EFalse;
		delete server;
		}

	// file failure when constructing index
	CMsgsTestUtils::CleanMessageFolderL(KTestIndexFile);
	failCount=0;
	failed=ETrue;
	while (failed && failCount<3)  
		{
		theUtils->FileSession().SetErrorCondition(KErrGeneral, failCount++);
		TRAP(error, server=CMsvServer::NewL());
		theUtils->FileSession().SetErrorCondition(KErrGeneral, KMaxTInt);
		PAUSE(100000);
		if (error)
			{
			TInt error2 = theUtils->FileSession().Att(KTestIndexFile, at);
			test(error2==KErrNotFound||error2==KErrPathNotFound);
			}
		delete server;
		}

	// test missing index file but stores are still there
#if defined(_NDEBUG)
	CMsgsTestUtils::CleanMessageFolderL(KTestIndexFile);
	server=CMsvServer::NewL();
	PAUSE(100000);
	delete server;
	theUtils->FileSession().Delete(KTestIndexFile);
	server=CMsvServer::NewL();
	delete server;
#endif
	}



//
		
LOCAL_C void doTestsL()
	{
	theTimer = CTestTimer::NewL();
	CleanupStack::PushL(theTimer);
	
	CorruptINIFile();

	//test.Printf(_L("Using %d different corruption modes\n"), KNumberCurruptionModes);

	CMsgsTestUtils::Start(_L("Initial creation"));
	TestInitialCreationL();
	CMsgsTestUtils::Complete();


	CleanupStack::PopAndDestroy(); // theTimer
	}

#pragma warning (default : 4238)
