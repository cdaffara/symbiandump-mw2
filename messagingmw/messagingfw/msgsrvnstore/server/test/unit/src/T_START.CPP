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

#include "msvindexadapter.h"
#include <msventry.h>
#include "MSERVER.H"
#include "msvutils.h"
#include "Testsetup.h"
#include <e32base.h>

#pragma warning (disable : 4238) //warning C4238: nonstandard extension used : class rvalue used as lvalue

RTest test(_L("Server starting Testrig"));
TFileName theFileName;

_LIT(KTestIndexFile, "\\Server start testing\\test index file");
_LIT(KTestIndexFolder, "\\Private\\1000484b\\Mail2\\");
_LIT(KMsvStoreInitEntryDescription, "MessagingMailInitTestEntry");
_LIT(KMsvStoreInitEntryDetails, "Entry created by test MailInit");
_LIT(KMsvDDriveNotConfigured, "D Drive not present. Ensure that '_EPOC_DRIVE_D \\epoc32\\wins\\d' appears in \\epoc32\\data\\epoc.ini.");
const TUid KMsvTestUid1 = {0x10200000};

//


const TInt KTestWaitForDeleteFlagMaxSeconds = 20;


TBool MailinitRunning()
	{
	_LIT(KMsvPostInitialisationSearchPattern, "*MAILINIT*");
	_LIT(KMsvPostInitialisationProcessPattern, "*");
	TFullName fullName;
	TBool found = EFalse;
	TFindProcess finder;
	finder.Find(KMsvPostInitialisationProcessPattern);
	while (finder.Next(fullName) == KErrNone && !found)
		{
		fullName.UpperCase();
		if (fullName.Match(KMsvPostInitialisationSearchPattern) != KErrNotFound)
			{
			found = ETrue;
			break;
			}
		}
	return found;
	}

TInt WaitForMailInitComplete()
	{
	TBool flagFileDeleted = EFalse;
	TInt timeLeft = KTestWaitForDeleteFlagMaxSeconds;
	while (!flagFileDeleted && timeLeft--)
		{
		User::After(1000000);
		flagFileDeleted = !MailinitRunning();
		}
	if (!timeLeft)
		return KErrTimedOut;
	return KErrNone;
	}

LOCAL_C void RemoveTestDirectoryL(const TDesC& aDrive)
	{
	CFileMan* fileMan = CFileMan::NewL(theUtils->FileSession());
	CleanupStack::PushL(fileMan);
	TParse parse;
	TInt error;
	theFileName.Copy(KTestIndexFile);
	parse.Set(aDrive, &theFileName, NULL);
	error = fileMan->RmDir(parse.DriveAndPath());
	if (error==KErrNotReady)
		{
		test.Printf(TRefByValue<const TDesC>(_L("Drive %S does not exist - the test code needs this drive\n Exiting - press any key...\n")), &aDrive);
//		test.Getch();
		User::Leave(KErrNotReady);
		}
	CleanupStack::PopAndDestroy();
	error = theUtils->FileSession().RmDir(parse.DriveAndPath());
	if (!(error==KErrNotFound||error==KErrNone))
		{
		TPtrC dir(parse.DriveAndPath());
		test.Printf(TRefByValue<const TDesC>(_L("Directory %S cannot be removed\n Exiting - press any key...\n")), &dir);
//		test.Getch();
		User::Leave(KErrNotReady);
		}
	}

LOCAL_C void RemoveTestFileL(const TDesC& aDrive)
	{
	CFileMan* fileMan = CFileMan::NewL(theUtils->FileSession());
	CleanupStack::PushL(fileMan);
	TParse parse;
	TInt error;
	TFileName fileName(KTestIndexFile);
	parse.Set(fileName, &aDrive, NULL);
	error = fileMan->RmDir(parse.DriveAndPath());
	CleanupStack::PopAndDestroy();
	error = theUtils->FileSession().RmDir(parse.DriveAndPath());
	if (!(error==KErrNotFound||error==KErrNone))
		{
		TPtrC dir(parse.DriveAndPath());
		test.Printf(TRefByValue<const TDesC>(_L("Directory %S cannot be removed\n Exiting - press any key...\n")), &dir);
//		test.Getch();
		User::Leave(KErrNotReady);
		}
	}

void CheckForEDrivePresentL()
	{
   	User::LeaveIfError(theUtils->FileSession().Connect());
  	TDriveList list;
  	User::LeaveIfError(theUtils->FileSession().DriveList(list));
	if (list[EDriveE]==KDriveAbsent)
		theUtils->WriteComment(KMsvDDriveNotConfigured);
	test(list[EDriveE]!=KDriveAbsent);
	}

void CleanMessageFolderEDriveL()
	{
	CFileMan* fileMan = CFileMan::NewL(theUtils->FileSession());
	CleanupStack::PushL(fileMan);

	TParse parse;
	TFileName fileName(KMsvDefaultFolder2);
	TBuf<4> drive=_L("e:");
	parse.Set(fileName, &drive, NULL);

	fileMan->RmDir(parse.DriveAndPath());
	CleanupStack::PopAndDestroy(); // fileMan

	theUtils->FileSession().RmDir(parse.DriveAndPath());
	TInt err = RSqlDatabase::Delete(_L("E:\\messaging.db"));
	}

LOCAL_C void TestMailInitStartCleanL(TBool aClean, TInt aDrive)
	{
	// Wait for mailinit to complete running.
	test(WaitForMailInitComplete()==KErrNone);
	// Ensure that the message server is not up.
	CMsgsTestUtils::WaitForServerClose();

	// Clean message folder.
	CMsgsTestUtils::SetDriveL(aDrive);
	CMsgsTestUtils::SetDebugL(EFalse);
	if (aClean)
		{
		if (aDrive==EDriveE)
			CleanMessageFolderEDriveL();
		else if	(aDrive==EDriveC)
			CMsgsTestUtils::CleanMessageFolderL();
		else 
			test(EFalse);
		}

	CDummyObserver* ob = new(ELeave)CDummyObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	// Wait for mailinit to complete running.
	test(WaitForMailInitComplete()==KErrNone);
	
	CTestActive* active = new (ELeave) CTestActive; 
	CleanupStack::PushL(active);

	CMsvEntry* msvEntry = CMsvEntry::NewL(*session, KMsvLocalServiceIndexEntryId, TMsvSelectionOrdering(KMsvNoGrouping, EMsvSortByNone, ETrue));
	CleanupStack::PushL(msvEntry);

	// Check for presence of 1 test entry in the inbox
	msvEntry->SetEntryL(KMsvGlobalInBoxIndexEntryId);
	CMsvEntrySelection* selection=NULL;
	selection = msvEntry->ChildrenL();
	test(selection->Count()==1);

	// Check for a match
	TInt found=0;
	for (TInt i=0;i<selection->Count();i++)
		{
		msvEntry->SetEntryL(selection->At(i));
		TMsvEntry testEntry=msvEntry->Entry();
		if ((testEntry.iDescription.Compare(KMsvStoreInitEntryDescription)==0) &&
			(testEntry.iDetails.Compare(KMsvStoreInitEntryDetails)==0))
			found++;
		}
	test(found==1);
	delete selection;

	// Cleanup
	CleanupStack::PopAndDestroy(msvEntry);
	CleanupStack::PopAndDestroy(active);
	CleanupStack::PopAndDestroy(session);
	CleanupStack::PopAndDestroy(ob);
	}

LOCAL_C void TestMailInitFakeStartL(TInt aDrive)
	{
	// Wait for mailinit to complete running.
	test(WaitForMailInitComplete()==KErrNone);
	// Ensure that the message server is not up.
	CMsgsTestUtils::WaitForServerClose();

	// Set drive.
	CMsgsTestUtils::SetDriveL(aDrive);
	CMsgsTestUtils::SetDebugL(EFalse);

	TFileName flagFile = _L("c:\\private\\1000484b\\StoreInit.tmp");
	flagFile[0] = (TUint8)('A'+aDrive);

	// Create "fake" mail init flag file
	RFile storeInitFile;
	test(storeInitFile.Replace(theUtils->FileSession(), flagFile, EFileShareAny|EFileWrite)==KErrNone);
	storeInitFile.Write(_L8("StoreInit"));
	storeInitFile.Close();

	CDummyObserver* ob = new(ELeave)CDummyObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	// Wait for mailinit to complete running.
	test(WaitForMailInitComplete()==KErrNone);

	CTestActive* active = new (ELeave) CTestActive; 
	CleanupStack::PushL(active);

	CMsvEntry* msvEntry = CMsvEntry::NewL(*session, KMsvLocalServiceIndexEntryId, TMsvSelectionOrdering(KMsvNoGrouping, EMsvSortByNone, ETrue));
	CleanupStack::PushL(msvEntry);

	// Check for presence of 2 test entries in the inbox
	msvEntry->SetEntryL(KMsvGlobalInBoxIndexEntryId);
	CMsvEntrySelection* selection=NULL;
	selection = msvEntry->ChildrenL();
	test(selection->Count()==2);
	
	// Check for a match
	TInt found=0;
	for (TInt i=0;i<selection->Count();i++)
		{
		msvEntry->SetEntryL(selection->At(i));
		TMsvEntry testEntry=msvEntry->Entry();
		if ((testEntry.iDescription.Compare(KMsvStoreInitEntryDescription)==0) &&
			(testEntry.iDetails.Compare(KMsvStoreInitEntryDetails)==0))
			found++;
		}

	test(found==2);
	delete selection;

	// Cleanup
	CleanupStack::PopAndDestroy(msvEntry);
	CleanupStack::PopAndDestroy(active);
	CleanupStack::PopAndDestroy(session);
	CleanupStack::PopAndDestroy(ob);
	}

LOCAL_C void TestMailInitNotReRunL(TInt aDrive)
	{
	// Wait for mailinit to complete running.
	test(WaitForMailInitComplete()==KErrNone);
	// Ensure that the message server is not up.
	CMsgsTestUtils::WaitForServerClose();

	// Clean message folder.
	CMsgsTestUtils::SetDriveL(aDrive);
	CMsgsTestUtils::SetDebugL(EFalse);
	if (aDrive==EDriveE)
		CleanMessageFolderEDriveL();
	else if	(aDrive==EDriveC)
		CMsgsTestUtils::CleanMessageFolderL();
	else 
		test(EFalse);

	CDummyObserver* ob = new(ELeave)CDummyObserver;
	CleanupStack::PushL(ob);
	
	CTestActive* active = new (ELeave) CTestActive; 
	CleanupStack::PushL(active);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	CMsvEntry* msvEntry = CMsvEntry::NewL(*session, KMsvLocalServiceIndexEntryId, TMsvSelectionOrdering(KMsvNoGrouping, EMsvSortByNone, ETrue));
	CleanupStack::PushL(msvEntry);

	// Don't wait for mailinit to complete running... we have 10 seconds
	User::After(1000000); 

	// Check for presence of 1 test entry in the inbox
	msvEntry->SetEntryL(KMsvGlobalInBoxIndexEntryId);
	CMsvEntrySelection* selection=NULL;
	selection = msvEntry->ChildrenL();
	test(selection->Count()==1);

	// Check for a match
	TInt found=0;
	TInt i;
	for (i=0;i<selection->Count();i++)
		{
		msvEntry->SetEntryL(selection->At(i));
		TMsvEntry testEntry=msvEntry->Entry();
		if ((testEntry.iDescription.Compare(KMsvStoreInitEntryDescription)==0) &&
			(testEntry.iDetails.Compare(KMsvStoreInitEntryDetails)==0))
			found++;
		}
	test(found==1);
	delete selection;

	// close the session.
	CleanupStack::PopAndDestroy(msvEntry);
	CleanupStack::PopAndDestroy(session);
	CleanupStack::PopAndDestroy(active);
	CleanupStack::PopAndDestroy(ob);

	// wait for the server to close.
	CMsgsTestUtils::WaitForServerClose();

	// OK. Now mailinit is currently running, so it has not deleted the flag yet. If we
	// open a new session to the message server, it is gonna find the flag and think that
	// mailinit has not yet been run. If it does run mailinit again, then a second 
	// entry will be added to the inbox... 

	ob = new(ELeave)CDummyObserver;
	CleanupStack::PushL(ob);
	
	active = new (ELeave) CTestActive; 
	CleanupStack::PushL(active);

	session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	msvEntry = CMsvEntry::NewL(*session, KMsvLocalServiceIndexEntryId, TMsvSelectionOrdering(KMsvNoGrouping, EMsvSortByNone, ETrue));
	CleanupStack::PushL(msvEntry);

	// Wait for mailinit to complete running...
	test(WaitForMailInitComplete()==KErrNone);

	// Check for presence of 1 test entry in the inbox - this proves mailinit not run again.
	msvEntry->SetEntryL(KMsvGlobalInBoxIndexEntryId);
	selection = msvEntry->ChildrenL();
	test(selection->Count()==1);

	// Check for a match
	found=0;
	for (i=0;i<selection->Count();i++)
		{
		msvEntry->SetEntryL(selection->At(i));
		TMsvEntry testEntry=msvEntry->Entry();
		if ((testEntry.iDescription.Compare(KMsvStoreInitEntryDescription)==0) &&
			(testEntry.iDetails.Compare(KMsvStoreInitEntryDetails)==0))
			found++;
		}
	test(found==1);
	delete selection;
	
	// Cleanup
	CleanupStack::PopAndDestroy(msvEntry);
	CleanupStack::PopAndDestroy(session);
	CleanupStack::PopAndDestroy(active);
	CleanupStack::PopAndDestroy(ob);
	}



LOCAL_C void TestStartDL()
	{
	TParse parse;
	TEntry entry;

	// remove the test files and directories
	RemoveTestDirectoryL(_L("C:"));
	RemoveTestDirectoryL(RemovableDrive.Name());
	
	//
	//
	// put drive RemovableDrive.Name() into the msgs.ini
	TChar driveChar=theUtils->FileSession().GetSystemDriveChar();
	TBuf<2> systemDrive;
	systemDrive.Append(driveChar);
	systemDrive.Append(KDriveDelimiter);
	TPath pathName(systemDrive);
	pathName.Append(KServerINIFile);
	CDictionaryFileStore* store = CDictionaryFileStore::OpenLC(theUtils->FileSession(),pathName,KNullUid);
	RDictionaryWriteStream stream;
	stream.AssignL(*store, KUidMsvMessageDriveStream);
	stream.WriteUint8L(1); // version number
	stream << RemovableDrive.Name();
	stream.CommitL();
	stream.Close();
	store->CommitL();
	CleanupStack::PopAndDestroy(store);

	// remove the test files and directories
	RemoveTestDirectoryL(_L("C:"));
	RemoveTestDirectoryL(RemovableDrive.Name());

	// start the server - should start on RemovableDrive.Name()
	CMsvServer* server = CMsvServer::NewL();
	delete server;

	// remove the test files
	RemoveTestFileL(RemovableDrive.Name());

	// start the server - should start on c:
	server = CMsvServer::NewL();
	delete server;

	CTestTimer* timer = CTestTimer::NewL();

	// start the server - should start on c:
	server = CMsvServer::NewL();

	timer->After(100000);
	CActiveScheduler::Start();
	delete server;
	delete timer;
	}


LOCAL_C void TestStartMissingDriveL()
	{
	// remove the test files and directories
	RemoveTestDirectoryL(_L("C:"));
	RemoveTestDirectoryL(RemovableDrive.Name());

	//
	//
	// put drive E: into the msgs.ini
	TChar driveChar=theUtils->FileSession().GetSystemDriveChar();
	TBuf<2> systemDriveName;
	systemDriveName.Append(driveChar);
	systemDriveName.Append(KDriveDelimiter);
    TDriveUnit driveUnit(systemDriveName);
	    
	TPath pathName(driveUnit.Name());
	pathName.Append(KServerINIFile);
	CDictionaryFileStore* store = CDictionaryFileStore::OpenLC(theUtils->FileSession(),pathName,KNullUid);
	RDictionaryWriteStream stream;
	stream.AssignL(*store, KUidMsvMessageDriveStream);
	stream.WriteUint8L(1); // version number
	stream << _L("y:");
	stream.CommitL();
	stream.Close();
	store->CommitL();
	CleanupStack::PopAndDestroy(); // store

	// start the server 
	CMsvServer* server=NULL;
	TRAPD(error, server = CMsvServer::NewL());

	test(error==KErrNone);
	delete server;

	//
	//
	// put drive C: into the msgs.ini
	store = CDictionaryFileStore::OpenLC(theUtils->FileSession(),pathName,KNullUid);
	stream.AssignL(*store, KUidMsvMessageDriveStream);
	stream.WriteUint8L(1); // version number
	stream << _L("c:");
	stream.CommitL();
	stream.Close();
	store->CommitL();
	CleanupStack::PopAndDestroy(); // store
	}


LOCAL_C void TestInPreparationL()
	{
	TMsvEntry entry;
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm = KUidMsvLocalServiceMtm;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	entry.SetInPreparation(ETrue);

	CTestTimer* timer = CTestTimer::NewL();

	// create an entry which are in preparation
	CMsvServer* server = CMsvServer::NewL();

	timer->After(2000000);
	CActiveScheduler::Start();
	CMsvServerEntry* sEntry = CMsvServerEntry::NewL(*server, KMsvLocalServiceIndexEntryId);
	CMsvServerEntry* tempSEntry = CMsvServerEntry::NewL(*server, KMsvGlobalInBoxIndexEntryId);
	TMsvEntry* temp;
	sEntry->GetEntryFromId(KFirstFreeEntryId, temp);
	TMsvEntry initEntry(*temp);
	tempSEntry->DeleteEntry(KFirstFreeEntryId);
	delete tempSEntry;
	//we need to do this as mailinit creates an entry under Inbox which clashes
	//with the below entry

	REPORT_IF_ERROR(sEntry->CreateEntry(entry));
	TMsvId id1 = entry.Id();
	delete sEntry;
	delete server;

	// reopen the server check it has ben removed
	server = CMsvServer::NewL();

	timer->After(2000000);
	CActiveScheduler::Start();
	sEntry = CMsvServerEntry::NewL(*server, KMsvLocalServiceIndexEntryId);
	test(sEntry->SetEntry(id1)==KErrNotFound);
	delete sEntry;
	delete server;

	// create several entries which are in preparation
	server = CMsvServer::NewL();

	timer->After(100000);
	CActiveScheduler::Start();
	sEntry = CMsvServerEntry::NewL(*server, KMsvLocalServiceIndexEntryId);
	REPORT_IF_ERROR(sEntry->CreateEntry(entry));
	id1 = entry.Id();
	REPORT_IF_ERROR(sEntry->CreateEntry(entry));
	TMsvId id2 = entry.Id();
	REPORT_IF_ERROR(sEntry->CreateEntry(entry));
	TMsvId id3 = entry.Id();
	REPORT_IF_ERROR(sEntry->CreateEntry(entry));
	TMsvId id4 = entry.Id();
	delete sEntry;
	delete server;

	// reopen the server check it has ben removed
	server = CMsvServer::NewL();

	timer->After(2000000);
	CActiveScheduler::Start();
	sEntry = CMsvServerEntry::NewL(*server, KMsvLocalServiceIndexEntryId);
	test(sEntry->SetEntry(id1)==KErrNotFound);
	test(sEntry->SetEntry(id2)==KErrNotFound);
	test(sEntry->SetEntry(id3)==KErrNotFound);
	test(sEntry->SetEntry(id4)==KErrNotFound);
	delete sEntry;
	delete server;

	// create nested entries which are in preparation
	server = CMsvServer::NewL();

	timer->After(100000);
	CActiveScheduler::Start();
	sEntry = CMsvServerEntry::NewL(*server, KMsvLocalServiceIndexEntryId);
	REPORT_IF_ERROR(sEntry->CreateEntry(entry));
	id1 = entry.Id();
	REPORT_IF_ERROR(sEntry->SetEntry(id1));
	REPORT_IF_ERROR(sEntry->CreateEntry(entry));
	id2 = entry.Id();
	REPORT_IF_ERROR(sEntry->SetEntry(KMsvGlobalOutBoxIndexEntryId));
	REPORT_IF_ERROR(sEntry->CreateEntry(entry));
	id3 = entry.Id();
	REPORT_IF_ERROR(sEntry->SetEntry(id3));
	REPORT_IF_ERROR(sEntry->CreateEntry(entry));
	id4 = entry.Id();
	delete sEntry;
	delete server;

	// reopen the server check it has ben removed
	server = CMsvServer::NewL();

	timer->After(2000000);
	CActiveScheduler::Start();
	sEntry = CMsvServerEntry::NewL(*server, KMsvLocalServiceIndexEntryId);
	test(sEntry->SetEntry(id1)==KErrNotFound);
	test(sEntry->SetEntry(id2)==KErrNotFound);
	test(sEntry->SetEntry(id3)==KErrNotFound);
	test(sEntry->SetEntry(id4)==KErrNotFound);
	//add the entry created by mailinit again
	tempSEntry = CMsvServerEntry::NewL(*server, KMsvGlobalInBoxIndexEntryId);
	tempSEntry->CreateEntry(initEntry);
	delete tempSEntry;
	delete sEntry;
	delete server;
	delete timer;
	}

LOCAL_C void TestMsgsIniLockedL()
	{
	// remove the test files and directories
	RemoveTestDirectoryL(_L("C:"));
	RemoveTestDirectoryL(RemovableDrive.Name());

	TParse parse;
	TEntry entry;
	TFileName filename;
	TMsvEntry mEntry;
	mEntry.iType = KUidMsvMessageEntry;
	mEntry.iMtm = KUidMsvLocalServiceMtm;
	mEntry.iServiceId = KMsvLocalServiceIndexEntryId;

	CTestTimer* timer = CTestTimer::NewL();
	TChar driveChar=theUtils->FileSession().GetSystemDriveChar();
	TBuf<2> systemDriveName;
	systemDriveName.Append(driveChar);
	systemDriveName.Append(KDriveDelimiter);
    TDriveUnit driveUnit(systemDriveName);
	    
	TPath pathName(driveUnit.Name());
	pathName.Append(KServerINIFile);
	CDictionaryFileStore* store = CDictionaryFileStore::OpenLC(theUtils->FileSession(),pathName,KNullUid);

	// should start on c:
	CMsvServer* server = CMsvServer::NewL();

	timer->After(1000000);
	CActiveScheduler::Start();

	CMsvServerEntry* sEntry = CMsvServerEntry::NewL(*server, KMsvGlobalInBoxIndexEntryId);
	REPORT_IF_ERROR(sEntry->CreateEntry(mEntry));
	REPORT_IF_ERROR(sEntry->SetEntry(mEntry.Id()));

	delete sEntry;
	delete server;


	// should start on c:
	server = CMsvServer::NewL();

	timer->After(1000000);
	CActiveScheduler::Start();
	sEntry = CMsvServerEntry::NewL(*server, mEntry.Id());
	delete sEntry;
	delete server;

	//
	//
	// put drive RemovableDrive.Name() into the system ini
	RDictionaryWriteStream stream;
	stream.AssignL(*store, KUidMsvMessageDriveStream);
	stream.WriteUint8L(1); // version number
	stream << RemovableDrive.Name();
	stream.CommitL();
	stream.Close();
	store->CommitL();
	CleanupStack::PopAndDestroy(); // store

	// remove the test files and directories
	RemoveTestDirectoryL(_L("C:"));
	RemoveTestDirectoryL(RemovableDrive.Name());

	// should start on RemovableDrive.Name()
	server = CMsvServer::NewL();

	timer->After(1000000);
	CActiveScheduler::Start();
	
	sEntry = CMsvServerEntry::NewL(*server, KMsvGlobalInBoxIndexEntryId);
	REPORT_IF_ERROR(sEntry->CreateEntry(mEntry));

	delete sEntry;
	delete server;


	store = CDictionaryFileStore::OpenLC(theUtils->FileSession(),pathName,KNullUid);

	// should start on RemovableDrive.Name()
	server = CMsvServer::NewL();

	timer->After(1000000);
	CActiveScheduler::Start();
	sEntry = CMsvServerEntry::NewL(*server, KMsvLocalServiceIndexEntryId);//newly added as fix

	delete sEntry;
	delete server;

	CleanupStack::PopAndDestroy(); // store
	delete timer;
	}

LOCAL_C void GetServicePath(TMsvId aService, TFileName& aFileName)
	{
	aFileName = KTestIndexFolder;
	MsvUtils::ConstructEntryName(aService, aService, aFileName, MsvUtils::EPath);
	}


LOCAL_C void DirExists(const TDesC& aDes)
	{
	TUint at;
	test(theUtils->FileSession().Att(aDes, at)==KErrNone);
	}

LOCAL_C void DirMissig(const TDesC& aDes)
	{
	TUint at;
	TInt error = theUtils->FileSession().Att(aDes, at);
	test(error==KErrNotFound);
	}


LOCAL_C void TestMissingServiceDirectoriesL()
	{
	// remove the drive from the system ini
	TUint unused;

	TChar driveChar=theUtils->FileSession().GetSystemDriveChar();
	TBuf<2> systemDriveName;
	systemDriveName.Append(driveChar);
	systemDriveName.Append(KDriveDelimiter);
    TDriveUnit driveUnit(systemDriveName);
	    
	TPath pathName(driveUnit.Name());
	pathName.Append(KServerINIFile);
	if(theUtils->FileSession().Att(pathName,unused)==KErrNone)
		test(theUtils->FileSession().Delete(pathName)==KErrNone);
	
	TMsvEntry service1;
	service1.iType = KUidMsvServiceEntry;
	service1.iMtm = KMsvTestUid1;
	service1.SetVisible(EFalse);
	TMsvEntry service2;
	service2.iType = KUidMsvServiceEntry;
	service2.iMtm = KMsvTestUid1;

	CTestTimer* timer = CTestTimer::NewL();

	// create service
	CMsvServer* server = CMsvServer::NewL();

	timer->After(1000000);
	CActiveScheduler::Start();
	CMsvServerEntry* sEntry = CMsvServerEntry::NewL(*server, KMsvLocalServiceIndexEntryId);
	REPORT_IF_ERROR(sEntry->SetEntry(KMsvRootIndexEntryId));
	REPORT_IF_ERROR(sEntry->CreateEntry(service1));
	REPORT_IF_ERROR(sEntry->CreateEntry(service2));
	TFileName dirname1;
	TFileName dirname2;
	GetServicePath(service1.Id(), dirname1);
	GetServicePath(service2.Id(), dirname2);
	DirExists(dirname1);
	DirExists(dirname2);
	delete sEntry;
	delete server;

	//	restart server
	server = CMsvServer::NewL();

	timer->After(1000000);
	CActiveScheduler::Start();
	DirExists(dirname1);
	DirExists(dirname2);
	delete server;
	
	// delete directory
	theUtils->FileSession().RmDir(dirname1);
	DirMissig(dirname1);
	DirExists(dirname2);
	
	//	restart server
	server = CMsvServer::NewL();

	timer->After(1000000);
	CActiveScheduler::Start();
	DirExists(dirname1);
	DirExists(dirname2);
	delete server;
	
	// delete directory
	theUtils->FileSession().RmDir(dirname2);
	DirExists(dirname1);
	DirMissig(dirname2);
	
	//	restart server
	server = CMsvServer::NewL();

	timer->After(1000000);
	CActiveScheduler::Start();
	DirExists(dirname1);
	DirExists(dirname2);
	delete server;

	// delete directory
	theUtils->FileSession().RmDir(dirname1);
	theUtils->FileSession().RmDir(dirname2);
	DirMissig(dirname1);
	DirMissig(dirname2);
	
	//	restart server
	server = CMsvServer::NewL();

	timer->After(1000000);
	CActiveScheduler::Start();
	DirExists(dirname1);
	DirExists(dirname2);
	delete server;

	delete timer;
	}


LOCAL_C void TestChangingDriveL()
	{
	theFileName.Copy(KTestIndexFile);

	// Start the server
	CMsvServer* server = CMsvServer::NewL();

	CleanupStack::PushL(server);

	// Wait a bit
	CTestTimer* timer = CTestTimer::NewL();
	timer->After(1000000);
	CActiveScheduler::Start();
	delete timer;

	CleanupStack::PopAndDestroy(); // server

	// Start the server
	server = CMsvServer::NewL();

	CleanupStack::PushL(server);

	// Wait a bit
	timer = CTestTimer::NewL();
	timer->After(1000000);
	CActiveScheduler::Start();
	delete timer;

#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	TDriveUnit drive(EDriveD);
#else
	TDriveUnit drive(RemovableDrive);
#endif
	test(server->ChangeDrive(drive, NULL) == KErrNone);

	// Wait a bit
	timer = CTestTimer::NewL();
	timer->After(1000000);
	CActiveScheduler::Start();
	delete timer;

	CTestActive* active = new(ELeave)CTestActive;
	drive = EDriveC;
	
	active->StartL();
	test(server->ChangeDrive(drive, &(active->iStatus)) == KErrNone);
	CActiveScheduler::Start();
	test(active->iStatus == KErrNone);

	delete active;

	// Wait a bit
	timer = CTestTimer::NewL();
	timer->After(1000000);
	CActiveScheduler::Start();
	delete timer;

	CleanupStack::PopAndDestroy(); // server
	}
	
LOCAL_C void TestChangingExistingDriveL()
	{
	theFileName.Copy(KTestIndexFile);

	// Start the server
	CMsvServer* server = CMsvServer::NewL();

	CleanupStack::PushL(server);

	// Wait a bit
	CTestTimer* timer = CTestTimer::NewL();
	timer->After(1000000);
	CActiveScheduler::Start();
	delete timer;

	CleanupStack::PopAndDestroy(); // server

	// Start the server
	server = CMsvServer::NewL();

	CleanupStack::PushL(server);

	// Wait a bit
	timer = CTestTimer::NewL();
	timer->After(1000000);
	CActiveScheduler::Start();
	delete timer;

	TDriveUnit drive(RFs::GetSystemDrive());
	test(server->ChangeDrive(drive, NULL) == KErrNotSupported);
	CleanupStack::PopAndDestroy(); // server
	}
	
const TInt KMinTestHeapSize = 0x10000;
const TInt KMaxTestHeapSize = 0x100000;
	
void DoStartMsvSessionL()
	{
	CActiveScheduler* scheduler = new (ELeave) CActiveScheduler();
	CleanupStack::PushL(scheduler);

	// Install scheduler
	CActiveScheduler::Install(scheduler);

	CDummyObserver* ob = new(ELeave)CDummyObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);
	
	// Wait a bit
	CTestTimer* timer = CTestTimer::NewL();
	timer->After(5000000);
	CActiveScheduler::Start();
	delete timer;
	
	// And then just end...
	CleanupStack::PopAndDestroy(session);
	CleanupStack::PopAndDestroy(ob);
	CMsgsTestUtils::WaitForServerClose();		
	CleanupStack::PopAndDestroy(scheduler);
	}
	
static TInt DoLaunchThread(TAny*)
	{
	CTrapCleanup* cleanup = CTrapCleanup::New();
	__ASSERT_ALWAYS(cleanup, User::Invariant());
	TRAPD(ret, DoStartMsvSessionL());				
	delete cleanup;
	return ret;
	}
	
class CTestMsvStarter;
	
class MTestStarterObserver
	{
public:
	virtual void StarterComplete(CTestMsvStarter* aStarter) =0;
	};
	
class CTestMsvStarter : public CActive
	{
public:
	static CTestMsvStarter* NewL(MTestStarterObserver& aObserver, const TDesC& aThreadName);
	virtual ~CTestMsvStarter();
	void Start();

private:
	virtual void RunL();
	virtual void DoCancel();
private:
	CTestMsvStarter(MTestStarterObserver& aObserver);
	void ConstructL(const TDesC& aThreadName);
private:
	MTestStarterObserver& iObserver;
	RThread	iThread;
	HBufC*	iThreadName; 
	TInt	iError;
	};
	
CTestMsvStarter* CTestMsvStarter::NewL(MTestStarterObserver& aObserver, const TDesC& aThreadName)
	{
	CTestMsvStarter* self = new (ELeave) CTestMsvStarter(aObserver);
	CleanupStack::PushL(self);
	self->ConstructL(aThreadName);
	CleanupStack::Pop(self);
	return self;
	}
	
CTestMsvStarter::CTestMsvStarter(MTestStarterObserver& aObserver)
: CActive(CActive::EPriorityStandard), iObserver(aObserver)
	{
	CActiveScheduler::Add(this);
	}
	
CTestMsvStarter::~CTestMsvStarter()
	{
	Cancel();
	iThread.Close();
	delete iThreadName;
	}

void CTestMsvStarter::ConstructL(const TDesC& aThreadName)
	{
	iThreadName = aThreadName.AllocL();
	
	iError = iThread.Create(*iThreadName, DoLaunchThread, KDefaultStackSize, KMinTestHeapSize, KMaxTestHeapSize, NULL, EOwnerThread);
	}
	
void CTestMsvStarter::Start()
	{
	iStatus = KRequestPending;

	if( iError == KErrNone )
		{
		iThread.Logon(iStatus);
		iThread.Resume();		
		}
	else
		{
		TRequestStatus* status = &iStatus;
		User::RequestComplete(status, iError);			
		}
	SetActive();
	}
	
void CTestMsvStarter::RunL()
	{
	test(iStatus.Int()==KErrNone);
	
	// Notify the observer that we're done.
	iObserver.StarterComplete(this);
	}
	
void CTestMsvStarter::DoCancel()
	{
	// Kill off the thread
	iThread.Kill(KErrCancel);
	iThread.Close();
	}
	
class CTestStarterObserver : public CBase, public MTestStarterObserver
	{
public:
	static CTestStarterObserver* NewL();
	virtual ~CTestStarterObserver();
private:
	virtual void StarterComplete(CTestMsvStarter* aStarter);
private:
	CTestStarterObserver();
	void ConstructL();
private:
	RPointerArray<CTestMsvStarter> iStarters;
	};
	
CTestStarterObserver* CTestStarterObserver::NewL()
	{
	CTestStarterObserver* self = new (ELeave) CTestStarterObserver();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}
	
CTestStarterObserver::CTestStarterObserver()
: CBase()
	{
	}
	
CTestStarterObserver::~CTestStarterObserver()
	{
	iStarters.ResetAndDestroy();
	iStarters.Close();
	}

void CTestStarterObserver::ConstructL()
	{
	_LIT(KThreadName1, "MsvStarterThread1");
	_LIT(KThreadName2, "MsvStarterThread2");

	// Create the thread creation object...
	CTestMsvStarter* thread1 = CTestMsvStarter::NewL(*this, KThreadName1);
	CleanupStack::PushL(thread1);
	iStarters.AppendL(thread1);
	CleanupStack::Pop(thread1);

	CTestMsvStarter* thread2 = CTestMsvStarter::NewL(*this, KThreadName2);
	CleanupStack::PushL(thread2);
	iStarters.AppendL(thread2);
	CleanupStack::Pop(thread2);

	thread1->Start();
	thread2->Start();	
	}
	
void CTestStarterObserver::StarterComplete(CTestMsvStarter* aStarter)
	{
	// Find starter...
	TBool found = EFalse;
	TInt count = iStarters.Count();
	while( !found && count-- )
		{
		if( iStarters[count] == aStarter )
			{
			// Remove the starter and stop the search
			delete iStarters[count];
			iStarters.Remove(count);
			found = ETrue;
			}
		}
	__ASSERT_ALWAYS( found, User::Invariant() );
	
	// Check to see any starters remain.
	if( iStarters.Count() == 0 )
		{
		// Nope - stop the scheduler.
		CActiveScheduler::Stop();
		}
	}
		
LOCAL_C void TestStartMultiSessionsL()
	{
	CTestStarterObserver* starters = CTestStarterObserver::NewL();	
	CActiveScheduler::Start();	
	delete starters;
	}

LOCAL_C TVolumeInfo GetVolumeInfoL(TInt aDrive)
	{
	TVolumeInfo volumeInfo;
	TDriveInfo driveInfo;
	
	RFs fs = theUtils->FileSession();	
	
	TInt error = fs.Drive( driveInfo, aDrive );
	if (error)
		{
		test.Printf(_L("ModifyINIFile has return an error %d\n"),error);
		User::Leave(error);		
		}
	else
		{
		error = fs.Volume( volumeInfo, aDrive );
		if (error)
			{
			theUtils->WriteComment(KMsvDDriveNotConfigured);
			test.Printf(_L("ModifyINIFile has return an error %d\n"), error);
			User::Leave(error);		
			}
		}	
	
	return 	volumeInfo;
	}

LOCAL_C void UpdateINIFileL(TUint uid)
	{
	TUint unused;
	TChar driveChar=theUtils->FileSession().GetSystemDriveChar();
	TBuf<2> systemDriveName;
	systemDriveName.Append(driveChar);
	systemDriveName.Append(KDriveDelimiter);
    	TDriveUnit driveUnit(systemDriveName);
	    
	TPath pathName(driveUnit.Name());
	pathName.Append(KServerINIFile);

	if(theUtils->FileSession().Att(pathName,unused)==KErrNone)
		test(theUtils->FileSession().Delete(pathName)==KErrNone);	
	CDictionaryFileStore *store = CDictionaryFileStore::OpenLC(theUtils->FileSession(), pathName, KNullUid);
	RDictionaryWriteStream stream;
	stream.AssignLC(*store, KUidMsvMessageDriveStream);
	
	stream.WriteUint8L(KMsvMessageDriveStreamVersionNumber); // version number
	stream << _L("e:");
	stream.WriteUint32L(uid); 	
	stream.WriteInt8L(0);		
	
	stream.CommitL();
	store->CommitL();
	CleanupStack::PopAndDestroy(2, store); // stream, store		
	}
	
LOCAL_C void TestCorrectMsgsIniFileL()	
	{
	TVolumeInfo volumeInfo;	
	volumeInfo = GetVolumeInfoL(EDriveE);
		
	//Update the msgs.ini file with D drive and its unique id
	UpdateINIFileL(volumeInfo.iUniqueID);
	
	CDummyObserver* ob = new(ELeave)CDummyObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);
	
	//Should return ETrue. 
	TBool retValue = EFalse;	
	TRAPD(err, retValue = session->MessageStoreDrivePresentL());	
	test(err == KErrNone);
	test(retValue);
	
	CleanupStack::PopAndDestroy(2, ob); //ob, session
	}
	
LOCAL_C void TestModifiedMsgsIniFileL()	
	{
	//Read the current configuration
	TMsvConfig config;
	CMsgsTestUtils::GetConfigL(config);
	
	CDummyObserver* ob = new(ELeave)CDummyObserver;
	CleanupStack::PushL(ob);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);
	
	//Update the ini with wrong uid and correct drive
	UpdateINIFileL(1);
	
	TBool retValue=EFalse;
	
	TRAPD(err, retValue = session->MessageStoreDrivePresentL());	
	test(err == KErrNone);
	test(!retValue);
	
	//update with the old configuration
	CMsgsTestUtils::SetConfigL(config);
		
	CleanupStack::PopAndDestroy(2, ob); //ob, session
	}
		
LOCAL_C void doTestsL()
	{
	CMsgsTestUtils::SetDebugL(EFalse);
	
	CMsgsTestUtils::Start(_L("Multi Session start-up"));
	TestStartMultiSessionsL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Start(_L("Check E: drive correctly configured"));
	CheckForEDrivePresentL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Clean starting"));
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Starting on C:"));
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Starting on removable drive"));
	TestStartDL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Starting on Y:"));
	TestStartMissingDriveL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("In Preparation flag"));
	TestInPreparationL();
	CMsgsTestUtils::Complete();

#if (!defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	//msgs.ini does not exist after PREQ557.
	CMsgsTestUtils::Next(_L("Msgs.ini locked"));
	TestMsgsIniLockedL();
	CMsgsTestUtils::Complete();
#endif

	CMsgsTestUtils::Next(_L("Missing service directories"));
	TestMissingServiceDirectoriesL();
	CMsgsTestUtils::Complete();
	
	CMsgsTestUtils::Next(_L("Changing Existing drive"));
	TestChangingExistingDriveL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Changing drive"));
	TestChangingDriveL();
	CMsgsTestUtils::Complete();

/*	test.Printf(_L("Waiting 2 1/2 minutes before running mailinit tests...\n"));
	User::After(150000000);
*/	test.Printf(_L("Waiting 1 1/2 minutes before running mailinit tests...\n"));
	User::After(90000000);
		
	CMsgsTestUtils::Next(_L("Mailinit run on C:"));
	TestMailInitStartCleanL(ETrue,EDriveC);
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Mailinit not run on C:"));
	TestMailInitStartCleanL(EFalse,EDriveC);
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Mailinit run with fake flag on C:"));
	TestMailInitFakeStartL(EDriveC);
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Mailinit not re-run when a mailinit already active"));
	TestMailInitNotReRunL(EDriveC);
	CMsgsTestUtils::Complete();

#if (!defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	CMsgsTestUtils::Next(_L("Test MessageStoreDrivePresentL with correct drive and uniqueid in msgs.ini"));
	TestCorrectMsgsIniFileL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Test MessageStoreDrivePresentL with correct drive and fake uniqueid in msgs.ini"));
	TestModifiedMsgsIniFileL();
	CMsgsTestUtils::Complete();
	
	test.Printf(_L("Waiting 3 1/2 minutes before running mailinit test's on E drive...\n"));
	User::After(210000000);
#endif

	CMsgsTestUtils::Next(_L("Mailinit run on E:"));
	TestMailInitStartCleanL(ETrue,EDriveE);
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Mailinit not run on E:"));
	TestMailInitStartCleanL(EFalse,EDriveE);
	CMsgsTestUtils::Complete();

#if (!defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	//msgs.ini does not exist after PREQ557.
	CMsgsTestUtils::Next(_L("Mailinit run with fake flag on E:"));
	TestMailInitFakeStartL(EDriveE);
	CMsgsTestUtils::Complete();	
#endif
	
	RemoveTestDirectoryL(_L("C:"));
	RemoveTestDirectoryL(RemovableDrive.Name());
	}

#pragma warning (default : 4238)
