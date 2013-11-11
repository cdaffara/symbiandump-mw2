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

#include <msvids.h>
#include "msvindexadapter.h"
#include <msventry.h>
#include "MSERVER.H"
#include "msvutils.h"
#include "Testsetup.h"

#pragma warning (disable : 4238) //warning C4238: nonstandard extension used : class rvalue used as lvalue

RTest test(_L("Index rebuilding Testrig"));
CTestTimer* theTimer;

_LIT(KTestFolder ,"\\Index rebuild testing\\");
_LIT(KTestIndexFile ,"\\Index rebuild testing\\Index");
_LIT(KTestLocalService ,"\\Index rebuild testing\\00001001_S");

const TInt KNumberCurruptionModes=4;

_LIT(KShortDescription1, "Description");
_LIT(KShortDescription2, "dfmodskfsdfsd");
_LIT(KLongDescription1, "A very long description which goes over thirty two characters but is now needed to go over 64");
_LIT(KLongDescription2, "A VERY LONG DESCRIPTION WHICH GOES OVER THIRTY TWO CHARACTERS BUT IS NOW NEEDED TO GO OVER 64.....................");
_LIT(KLongDetails2, "xfldsknfkdsfondsf  fdshoiffhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh dffffffffffffffffffffffffffffff sdffffffffffffffff sd");

const TUid KMsvEntryFile={0x10003C68};

const TInt KTestRawNotSupportdeDataSize = 32;
const TUint8 KTestRawNotSupportedData[KTestRawNotSupportdeDataSize] = 
	{ 0x50,0x20,0x20,0x10,0x6b,0x3c,0x20,0x10,0x20,0x20,0x20,0x20,0x59,0x37,0xe2,0x24,
	  0x02,0xf6,0x01,0x20,0x20,0x20,0x20,0x20,0x17,0xfc,0x20,0x20,0x7c,0x5d,0x08,0x40 };

#define PAUSE(a) {theTimer->After(a); CActiveScheduler::Start();}

//

LOCAL_C void TestInitialCreationL()
	{
	CMsvServer* server;

	// non-existant directory
	CMsgsTestUtils::CleanMessageFolderL(KTestIndexFile);
	server=CMsvServer::NewL();
	PAUSE(100000);
	delete server;
	server=NULL;

	// only directory exists
	CMsgsTestUtils::CleanMessageFolderL(KTestIndexFile);
	theUtils->FileSession().MkDirAll(KTestIndexFile);
	server=CMsvServer::NewL();
	PAUSE(100000);
	delete server;
	server=NULL;

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
		server=NULL;
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
		server=NULL;
		}

	// test missing index file but stores are still there
#if defined(_NDEBUG)
	CMsgsTestUtils::CleanMessageFolderL(KTestIndexFile);
	server=CMsvServer::NewL();
	PAUSE(100000);
	delete server;
	server=NULL;
	theUtils->FileSession().Delete(KTestIndexFile);
	server=CMsvServer::NewL();
	delete server;
	server=NULL;
#endif
	}


LOCAL_C void TestInitialStructureL(CMsvServer& aServer)
	{
	TInt services=1;
	TInt localFolders=5;

	CMsvServerEntry* sEntry = CMsvServerEntry::NewL(aServer, KMsvRootIndexEntryId);
	CleanupStack::PushL(sEntry);
	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);
	TMsvSelectionOrdering sort(KMsvNoGrouping, EMsvSortByNone, ETrue);
	sEntry->SetSort(sort);

	// root
	REPORT_IF_ERROR(sEntry->SetEntry(KMsvRootIndexEntryId));
	test(sEntry->Entry().iDetails==KNullDesC);
	test(sEntry->Entry().iDescription==KNullDesC);
	REPORT_IF_ERROR(sEntry->GetChildren(*selection));
	test(selection->Count()==services);
	
	// local service
	REPORT_IF_ERROR(sEntry->SetEntry(KMsvLocalServiceIndexEntryId));
	test(sEntry->Entry().iDetails==_L("Local"));
	REPORT_IF_ERROR(sEntry->GetChildren(*selection));
	test(selection->Count()==localFolders);

	// standard folders
	REPORT_IF_ERROR(sEntry->SetEntry(KMsvGlobalInBoxIndexEntryId));
	test(sEntry->Entry().iDetails==_L("Inbox"));
	REPORT_IF_ERROR(sEntry->SetEntry(KMsvGlobalOutBoxIndexEntryId));
	test(sEntry->Entry().iDetails==_L("Outbox"));
	REPORT_IF_ERROR(sEntry->SetEntry(KMsvDraftEntryId));
	test(sEntry->Entry().iDetails==_L("Drafts"));
	REPORT_IF_ERROR(sEntry->SetEntry(KMsvSentEntryId));
	test(sEntry->Entry().iDetails==_L("Sent"));
	REPORT_IF_ERROR(sEntry->SetEntry(KMsvDeletedEntryFolderEntryId));
	test(sEntry->Entry().iDetails==_L("Deleted"));

	// check that only the index file and the local services dir are present
	CDir* dir;
	const TUidType type(KMsvEntryFile, KMsvEntryFile, KNullUid);
	REPORT_IF_ERROR(theUtils->FileSession().GetDir(KTestFolder,type , ESortNone, dir));
	TInt count=dir->Count();
	test(count==0) ;
	delete dir;

	// check that the local services dir is empty
	TFileName filename;
	filename.Append(KTestLocalService);
	filename.Append(_L("\\"));
	REPORT_IF_ERROR(theUtils->FileSession().GetDir(filename,type , ESortNone, dir));
	count=dir->Count();
	test(count==0) ;
	delete dir;

	CleanupStack::PopAndDestroy(2); // sEntry,selection
	}


LOCAL_C void TestStoreDeletedL()
	{
	RFs fs;
	REPORT_IF_ERROR(fs.Connect());
	_LIT(KStoreDeleted,"c:\\private\\1000484b\\StoreInit.tmp");
	TInt delErr = fs.Delete(KStoreDeleted);
	test((delErr == KErrNone) || (delErr ==KErrNotFound));
	fs.Close();
	}
//
		
LOCAL_C void doTestsL()
	{
	theTimer = CTestTimer::NewL();
	CleanupStack::PushL(theTimer);

	test.Printf(_L("Using %d different corruption modes\n"), KNumberCurruptionModes);

	CMsgsTestUtils::Start(_L("Initial creation"));
	TestInitialCreationL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Rebuilding on not supported error"));
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Rebuilding initial structure"));
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Rebuilding index 1"));
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Rebuilding index 2"));
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Rebuilding index 3"));
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Rebuilding index 5"));
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Checking for StoreDeleted file"));
	TestStoreDeletedL();
	CMsgsTestUtils::Complete();

	CleanupStack::PopAndDestroy(); // theTimer
	}

#pragma warning (default : 4238)
