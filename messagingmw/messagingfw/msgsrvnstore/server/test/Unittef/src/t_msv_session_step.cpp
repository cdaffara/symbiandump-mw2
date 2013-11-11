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
// t_msv_session_step.cpp
//
#include "t_msv_session_step.h"
#include <sqldb.h>
#include <msvruids.h>
#include <msvuids.h>
#include <msvids.h>
#include <msvapi.h>
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS  
#include "msvconsts.h"
#endif

#define MY_ASSERT_TRUE(a)  \
		INFO_PRINTF1(KFunction);   \
		ASSERT_TRUE(a);
#define MY_ASSERT_FALSE(a)  \
		INFO_PRINTF1(KFunction);   \
		ASSERT_FALSE(a);
#define MY_ASSERT_EQUALS(a,b)  \
		INFO_PRINTF1(KFunction);   \
		ASSERT_EQUALS(a,b);



_LIT(KDbFileName, "\\messaging.db");
_LIT(KBaseDBFileName, "C:\\messaging_base.db");
_LIT(KMsgTestIndexFile, "\\private\\1000484b\\Index");
_LIT(KMsgIndexFile, "\\private\\1000484b\\Mail2\\Index");
_LIT(KMsvServerPattern, "!MsvServer*");

_LIT(KPreferredDriveListFilePath, "C:\\private\\1000484b\\msgprioritydrivelist.ini");
_LIT(KMsgTestPriorityDriveListFile0, "C:\\private\\1000484b\\msgprioritydrivelist1.ini");
_LIT(KMsgTestPriorityDriveListFile1, "C:\\private\\1000484b\\msgprioritydrivelist8.ini"); 
_LIT(KMsgTestPriorityDriveListFile2, "C:\\private\\1000484b\\msgprioritydrivelist9.ini");




CTestMsvSession::CTestMsvSession():iSession(NULL)
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	,iUnsupportedDrivesPresent(EFalse)
#endif
	{
	}

CTestMsvSession::~CTestMsvSession()
	{
	delete iSession;
	}



// ---------- Utility methods. ---------------

void CTestMsvSession::SetupL()
	{
	_LIT(KFunction, "SetupL");
	INFO_PRINTF1(KFunction);
	
	INFO_PRINTF1(_L("Resetting CentRep that holds the DriveList to default values."));
	ResetRepositoryL();
	
	RFs fs;
	TInt err = fs.Connect();
	MY_ASSERT_EQUALS(err, KErrNone);
	CFileMan* file = CFileMan::NewL(fs);
	CleanupStack::PushL(file);
	
	TParse parse;
	TPtrC drive = TDriveUnit(EDriveC).Name();
	parse.Set(KDbFileName, &drive, NULL);
	TFileName dbFile = parse.FullName();
	RSqlDatabase::Delete(dbFile);		// Ignore error.
	parse.Set(KMsgIndexFile, &drive, NULL);
	file->RmDir(parse.DriveAndPath());
	
	drive.Set(TDriveUnit(EDriveD).Name());
	parse.Set(KDbFileName, &drive, NULL);
	dbFile = parse.FullName();
	RSqlDatabase::Delete(dbFile);		// Ignore error.
	parse.Set(KMsgIndexFile, &drive, NULL);
	file->RmDir(parse.DriveAndPath());
	
	drive.Set(TDriveUnit(EDriveE).Name());
	parse.Set(KDbFileName, &drive, NULL);
	dbFile = parse.FullName();
	RSqlDatabase::Delete(dbFile);		// Ignore error.
	parse.Set(KMsgIndexFile, &drive, NULL);
	file->RmDir(parse.DriveAndPath());
	
	drive.Set(TDriveUnit(EDriveF).Name());
	parse.Set(KDbFileName, &drive, NULL);
	dbFile = parse.FullName();
	RSqlDatabase::Delete(dbFile);		// Ignore error.
	parse.Set(KMsgIndexFile, &drive, NULL);
	file->RmDir(parse.DriveAndPath());
	
	drive.Set(TDriveUnit(EDriveJ).Name());
	parse.Set(KDbFileName, &drive, NULL);
	dbFile = parse.FullName();
	RSqlDatabase::Delete(dbFile);		// Ignore error.
	parse.Set(KMsgIndexFile, &drive, NULL);
	file->RmDir(parse.DriveAndPath());
	CleanupStack::PopAndDestroy();
	fs.Close();
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	iUnsupportedDrivesPresent = EFalse;
#endif
	}


class CDummyObserver : public CBase, public MMsvSessionObserver
	{
public:
	void HandleSessionEventL(TMsvSessionEvent, TAny*, TAny*, TAny*) {};
	};
	
	

void CTestMsvSession::TearDownL()
	{
	_LIT(KFunction, "TearDownL");
	INFO_PRINTF1(KFunction);

	// TearDownL() could already be called by 
	// test code directly.
	if(iSession)
		{
		TRAPD(err, iSession->ResetRepositoryL());
		MY_ASSERT_EQUALS(err, KErrNone);
		
		iSession->CloseMessageServer();

		delete iEntry;
		iEntry = NULL;
		delete iExtraEntry;
		iExtraEntry = NULL;
		
		delete iMsvEntryObserver;
		iMsvEntryObserver = NULL;
		delete iMsvExtraEntryObserver;
		iMsvExtraEntryObserver = NULL;
				
		delete iSession;
		iSession = NULL;	
		delete iMsvSesionObserver;
		iMsvSesionObserver = NULL;
		
		delete iExtraSession;
		iExtraSession = NULL;
		delete iMsvExtraSesionObserver;
		iMsvExtraSesionObserver = NULL;
	
		TFindProcess find(KMsvServerPattern);
		TFullName name;
		if (find.Next(name) == KErrNone)
			{
			TRequestStatus status;
			RProcess process;
    		User::LeaveIfError(process.Open(name));
    		process.Logon(status);
    		User::WaitForRequest(status);
    		User::After(200000);	
			}
		}
	
	RFs fs;
	TInt err = fs.Connect();
	MY_ASSERT_EQUALS(err, KErrNone);
	CFileMan* file = CFileMan::NewL(fs);
	CleanupStack::PushL(file);
	
	// C:
	TParse parse;
	TPtrC drive = TDriveUnit(EDriveC).Name();
	parse.Set(KDbFileName, &drive, NULL);
	TFileName dbFile = parse.FullName();
	RSqlDatabase::Delete(dbFile);		// Ignore error.
	parse.Set(KMsgIndexFile, &drive, NULL);
	TFileName filename = parse.DriveAndPath();
	err = file->RmDir(parse.DriveAndPath());
	
	drive.Set(TDriveUnit(EDriveD).Name());
	parse.Set(KDbFileName, &drive, NULL);
	dbFile = parse.FullName();
	RSqlDatabase::Delete(dbFile);		// Ignore error.
	parse.Set(KMsgIndexFile, &drive, NULL);
	file->RmDir(parse.DriveAndPath());
	
	drive.Set(TDriveUnit(EDriveE).Name());
	parse.Set(KDbFileName, &drive, NULL);
	dbFile = parse.FullName();
	RSqlDatabase::Delete(dbFile);		// Ignore error.
	parse.Set(KMsgIndexFile, &drive, NULL);
	file->RmDir(parse.DriveAndPath());
		
	drive.Set(TDriveUnit(EDriveF).Name());
	parse.Set(KDbFileName, &drive, NULL);
	dbFile = parse.FullName();
	RSqlDatabase::Delete(dbFile);		// Ignore error.
	parse.Set(KMsgIndexFile, &drive, NULL);
	file->RmDir(parse.DriveAndPath());
		
	drive.Set(TDriveUnit(EDriveJ).Name());
	parse.Set(KDbFileName, &drive, NULL);
	dbFile = parse.FullName();
	RSqlDatabase::Delete(dbFile);		// Ignore error.
	parse.Set(KMsgIndexFile, &drive, NULL);
	file->RmDir(parse.DriveAndPath());
		
	CleanupStack::PopAndDestroy();
	fs.Close();
	}



void CTestMsvSession::InitialiseSessionObserver()
	{
	_LIT(KFunction, "InitialiseSessionObserver");
	INFO_PRINTF1(KFunction);

	// Resetting repository through the session first..
	TRAPD(err, iMsvSesionObserver = new(ELeave) CTestSessionObserver());	
	MY_ASSERT_EQUALS(err, KErrNone);
	
	TRAP(err, iSession = CMsvSession::OpenSyncL(*iMsvSesionObserver));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	// Global entry on Inbox.
	TRAP(err, iEntry = CMsvEntry::NewL(*iSession, KMsvGlobalInBoxIndexEntryIdValue, TMsvSelectionOrdering(), ETrue));
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, iMsvEntryObserver = new(ELeave) CTestEntryObserver);
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, iEntry->AddObserverL(*iMsvEntryObserver));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	// Extra session to check notifications.
	TRAP(err, iMsvExtraSesionObserver = new(ELeave) CTestSessionObserver());
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, iExtraSession = CMsvSession::OpenSyncL(*iMsvExtraSesionObserver));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	// Non-global entry on Inbox.
	TRAP(err, iExtraEntry = CMsvEntry::NewL(*iExtraSession, KMsvGlobalInBoxIndexEntryIdValue, TMsvSelectionOrdering()));
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, iMsvExtraEntryObserver = new(ELeave) CTestEntryObserver);
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, iExtraEntry->AddObserverL(*iMsvExtraEntryObserver));
	MY_ASSERT_EQUALS(err, KErrNone);
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	if(iUnsupportedDrivesPresent)
		{
		CActiveScheduler::Start();
		CActiveScheduler::Start();
		ASSERT_EQUALS(iMsvSesionObserver->iType, MMsvSessionObserver::EMsvMessageStoreNotSupported);
		ASSERT_EQUALS(iMsvExtraSesionObserver->iType, MMsvSessionObserver::EMsvMessageStoreNotSupported);
		}
#endif
	}


void CTestMsvSession::SetupINIFile(TInt aPref)
	{
	_LIT(KFunction, "SetupINIFile");
	INFO_PRINTF1(KFunction);

	RFs fs;
	TParse parse;
	
	fs.Connect();
	TPtrC drive = TDriveUnit(RFs::GetSystemDrive()).Name();
	parse.Set(KPreferredDriveListFilePath, &drive, NULL);
	TInt err = fs.Delete(parse.FullName());
	
	CFileMan* file = NULL;
	TRAP_IGNORE(file = CFileMan::NewL(fs));
	parse.Set(KPreferredDriveListFilePath, &drive, NULL);
	TFileName fileName = parse.FullName();
	switch(aPref)
		{
		case 0: 		 //Empty driveList 	
			err = file->Copy(KMsgTestPriorityDriveListFile0, fileName);
			break;
		case 1:			 //driveList one drive 	
			err = file->Copy(KMsgTestPriorityDriveListFile1, fileName);
			break;
		case 2:			 //driveList with multiple drive.	
			err = file->Copy(KMsgTestPriorityDriveListFile2, fileName);
			break;
		default:
			MY_ASSERT_TRUE(EFalse);
		}
	MY_ASSERT_EQUALS(err, KErrNone);
	fs.Close();
	delete file;	
	}



void CTestMsvSession::CreateEntry(CMsvEntry *aEntry)
	{
	_LIT(KFunction, "CreateEntry");
	//INFO_PRINTF1(KFunction);
	
	TMsvEntry entry;
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm = KUidMsvLocalServiceMtm;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	TRAPD(err, aEntry->CreateL(entry));
	ASSERT_EQUALS(err, KErrNone);
	CActiveScheduler::Start();
	CActiveScheduler::Start();
	ASSERT_EQUALS(iMsvSesionObserver->iType, MMsvSessionObserver::EMsvEntriesCreated);
	ASSERT_EQUALS(iMsvExtraSesionObserver->iType, MMsvSessionObserver::EMsvEntriesCreated);
	ASSERT_EQUALS(iMsvExtraEntryObserver->iType, MMsvEntryObserver::EMsvNewChildren);
	}


inline TMsvId MaskTMsvId(TUint aDriveId, TMsvId aUnmaskedId)
	{
	__ASSERT_DEBUG(aDriveId>=0 && aDriveId <= 7, User::Invariant());
	return (aUnmaskedId | (aDriveId << 28));
	}
	

	
	
// -------------- Test methods ---------------

// Test cases when preferred drive list ini file is empty.
void CTestMsvSession::CTestDriveListWithNoDriveL()
	{
	_LIT(KFunction, "CTestDriveListWithNoDriveL");
	INFO_PRINTF1(KFunction);
	
	// Delete message store in C:
	TParse parse;
	TPtrC drive = TDriveUnit(EDriveC).Name();
	parse.Set(KDbFileName, &drive, NULL);
	TFileName dbFile = parse.FullName();
	RSqlDatabase::Delete(dbFile);		// Ignore error.
	RSqlDatabase::Copy(KBaseDBFileName, dbFile);
	
	SetupINIFile(0);
	InitialiseSessionObserver();
	
	// Cleanup all notifications.
	RArray<TDriveNumber> driveList;
	CleanupClosePushL(driveList);
	TRAPD(err, iSession->DriveListL(driveList));
	MY_ASSERT_EQUALS(err, KErrNone);
		
	// aDriveList should have at least one drive information of EDriveC	
	MY_ASSERT_EQUALS(driveList.Count(), 1);
	MY_ASSERT_EQUALS(driveList[0], EDriveC);	
	driveList.Reset();
	
	TRAP(err, iSession->AvailableDriveListL(driveList));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(1, driveList.Count());
	MY_ASSERT_EQUALS(driveList[0], EDriveC);	
	CleanupStack::PopAndDestroy(); //driveList
	
	TDriveNumber driveNumber;
	TUint priority;	
	TRAP(err, iSession->CurrentDriveInfoL(driveNumber, priority));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(EDriveC, driveNumber);
	MY_ASSERT_EQUALS(TInt(1), (TInt)priority);
	
	
	CMsvEntryFilter* filter = CMsvEntryFilter::NewLC();
	CMsvEntrySelection* selection = new(ELeave) CMsvEntrySelection;
	CleanupStack::PushL(selection);
	TRAP(err, iSession->GetChildIdsAllL(KMsvRootIndexEntryIdValue, *filter, *selection));
	MY_ASSERT_EQUALS(err, KErrArgument);
	selection->Reset();
	TRAP(err, iSession->GetChildIdsAllL(KMsvLocalServiceIndexEntryIdValue, *filter, *selection));
	MY_ASSERT_EQUALS(err, KErrArgument);
	selection->Reset();
	TRAP(err, iSession->GetChildIdsAllL(KMsvUnkownServiceIndexEntryIdValue, *filter, *selection));
	MY_ASSERT_EQUALS(err, KErrArgument);
	selection->Reset();
	TRAP(err, iSession->GetChildIdsAllL(MaskTMsvId((TUint)6, KMsvGlobalInBoxIndexEntryIdValue), *filter, *selection));
	MY_ASSERT_EQUALS(err, KErrArgument);
	selection->Reset();	
	TRAP(err, iSession->GetChildIdsAllL(KMsvGlobalInBoxIndexEntryIdValue, *filter, *selection));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	for(TInt i=0; i<2000; i++)
		{
		CreateEntry(iEntry);
		}
	
	selection->Reset();
	TRAP(err, iSession->GetChildIdsAllL(KMsvGlobalInBoxIndexEntryIdValue, *filter, *selection));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(selection->Count(), 2000);

	CleanupStack::PopAndDestroy(); // selection
	selection = NULL;
	// CMsvEntry related test cases.
	TRAP(err, selection = iEntry->ChildrenOfAvailableDrivesL());
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, selection->Find(MaskTMsvId(1, KFirstFreeEntryId)));
	MY_ASSERT_FALSE(err==KErrNotFound);
	TRAP(err, selection->Find(MaskTMsvId(1, KFirstFreeEntryId+1)));
	MY_ASSERT_FALSE(err==KErrNotFound);
	TRAP(err, selection->Find(MaskTMsvId(1, KFirstFreeEntryId+2)));
	MY_ASSERT_FALSE(err==KErrNotFound);
	TRAP(err, selection->Find(MaskTMsvId(1, KFirstFreeEntryId+3)));
	MY_ASSERT_FALSE(err==KErrNotFound);
	
	delete selection;
	selection = NULL;
	TRAP(err, iEntry->SetEntryL(KMsvRootIndexEntryId));
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, selection = iEntry->ChildrenOfAvailableDrivesL());
	MY_ASSERT_EQUALS(err, KErrArgument);
	
	TRAP(err, iEntry->SetEntryL(KMsvLocalServiceIndexEntryId));
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, selection = iEntry->ChildrenOfAvailableDrivesL());
	MY_ASSERT_EQUALS(err, KErrArgument);

	delete selection;
	selection = NULL;
	CleanupStack::PopAndDestroy(); // filter	
	}
	
	
	
	

void CTestMsvSession::CTestDriveListWithMultipleDriveL()
	{
	_LIT(KFunction, "CTestDriveListWithMultipleDriveL");
	INFO_PRINTF1(KFunction);
	
	// File has following drives in this order.
	// P (Non-existent), D (Existent, IndexFile), E (Existent, DB File)
	// F (Existent, Corrupt DB File), R (Non-existent),
	// J (Existent, empty), Q (Non-existent)
	
	// C (Internal Existent, DB File) is not present in the file.
	SetupINIFile(2);

	// 1. Creating necessary files.
	// Create index file in D:
	RFs fs;
	CleanupClosePushL(fs);
	TParse parse;
	TInt err = fs.Connect();
	MY_ASSERT_EQUALS(err, KErrNone);
	TPtrC drive = TDriveUnit(EDriveD).Name();
	parse.Set(KMsgIndexFile, &drive, NULL);
	fs.MkDirAll(parse.DriveAndPath());		// Ignore error.
	CFileMan* file = CFileMan::NewL(fs);
	CleanupStack::PushL(file);
	file->Delete(parse.FullName());
	err = file->Copy(KMsgTestIndexFile, parse.FullName());
	MY_ASSERT_EQUALS(err, KErrNone);	
	// Create DB file in E:
	TParse parse1;
	drive.Set(TDriveUnit(EDriveE).Name());
	parse1.Set(KDbFileName, &drive, NULL);
	TFileName dBFileE = parse1.FullName();
	err = RSqlDatabase::Delete(dBFileE);
	err = RSqlDatabase::Copy(KBaseDBFileName, dBFileE);
	MY_ASSERT_EQUALS(KErrNone, err);
	// Create corrupt DB file in F:
	drive.Set(TDriveUnit(EDriveF).Name());
	parse1.Set(KDbFileName, &drive, NULL);
	TFileName dBFileF = parse1.FullName();
	err = RSqlDatabase::Delete(dBFileF);
	err = file->Copy(KMsgTestIndexFile, dBFileF);
	MY_ASSERT_EQUALS(KErrNone, err);
	// Ensure J drive is empty.
	drive.Set(TDriveUnit(EDriveJ).Name());
	parse1.Set(KDbFileName, &drive, NULL);
	TFileName dBFileJ = parse1.FullName();
	err = RSqlDatabase::Delete(dBFileJ);
	parse1.Set(KMsgIndexFile, &drive, NULL);
	file->Delete(parse1.FullName());		// Ignore error.
	file->RmDir(parse1.DriveAndPath());		// Ignore error.
	// Creating DB file in C:
	drive.Set(TDriveUnit(EDriveC).Name());
	parse1.Set(KDbFileName, &drive, NULL);
	TFileName dBFileC = parse1.FullName();
	err = RSqlDatabase::Delete(dBFileC);
	err = RSqlDatabase::Copy(KBaseDBFileName, dBFileC);

	// 2. Create session.
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	iUnsupportedDrivesPresent = ETrue;
#endif
	InitialiseSessionObserver();

	RArray<TDriveNumber> driveList;
	CleanupClosePushL(driveList);
	TRAP(err, iSession->DriveListL(driveList));
	MY_ASSERT_EQUALS(err, KErrNone);
		
	// aDriveList should have at least one drive information of EDriveC	
	MY_ASSERT_EQUALS(driveList.Count(), 5);
	MY_ASSERT_EQUALS(driveList[0], EDriveD);	// IndexFile
	MY_ASSERT_EQUALS(driveList[1], EDriveE);	// DB
	MY_ASSERT_EQUALS(driveList[2], EDriveF);	// empty
	MY_ASSERT_EQUALS(driveList[3], EDriveJ);	// empty
	MY_ASSERT_EQUALS(driveList[4], EDriveC);	// DB
	driveList.Reset();
	
	TRAP(err, iSession->AvailableDriveListL(driveList));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(2, driveList.Count());
	MY_ASSERT_EQUALS(driveList[0], EDriveE);
	MY_ASSERT_EQUALS(driveList[1], EDriveC);
	driveList.Reset();
	
	TDriveNumber driveNumber;
	TUint priority;	
	TRAP(err, iSession->CurrentDriveInfoL(driveNumber, priority));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(EDriveE, driveNumber);
	MY_ASSERT_EQUALS(TInt(2), (TInt)priority);

	// Adding entries in current drives (E).
	CreateEntry(iEntry);
	CreateEntry(iEntry);
	CreateEntry(iEntry);
	CreateEntry(iEntry);
	
	CMsvEntryFilter* filter = CMsvEntryFilter::NewLC();
	CMsvEntrySelection* selection = new(ELeave) CMsvEntrySelection;
	CleanupStack::PushL(selection);
	TRAP(err, iSession->GetChildIdsAllL(KMsvGlobalInBoxIndexEntryIdValue, *filter, *selection));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(selection->Count(), 4);
	selection->Reset();
	
	CMsvEntrySelection* entrySelection = NULL;
	// CMsvEntry related test cases.
	TRAP(err, entrySelection = iEntry->ChildrenOfAvailableDrivesL());
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection->Count(), 4);
	delete entrySelection; entrySelection = NULL;
	TRAP(err, entrySelection = iEntry->ChildrenL());
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection->Count(), 4);
	delete entrySelection; entrySelection = NULL;
	TRAP(err, entrySelection = iExtraEntry->ChildrenOfAvailableDrivesL());
	MY_ASSERT_EQUALS(err, KErrArgument);
	TRAP(err, entrySelection = iExtraEntry->ChildrenL());
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection->Count(), 4);
	delete entrySelection; entrySelection = NULL;
	
	// 3. Removing S drive (Drive does not exists in phone).
	TRAP(err, iSession->RemoveDriveL(EDriveS));
	MY_ASSERT_EQUALS(err, KErrNotFound);
	
	
	// 4. Pass invalid drive number.
	TRAP(err, iSession->RemoveDriveL((TDriveNumber)40));
	MY_ASSERT_EQUALS(err, KErrArgument);
	
		
	// 5. Removing K drive (Drive present in device,
	// but not present in the drive list.)
	TRAP(err, iSession->RemoveDriveL(EDriveK));
	MY_ASSERT_EQUALS(err, KErrNotFound);
	
	
	// 6. Remove system drive.
	TRAP(err, iSession->RemoveDriveL(EDriveC));
	MY_ASSERT_EQUALS(err, KErrArgument);
	

	// 7. Removing J drive.
	TRAP(err, iSession->RemoveDriveL(EDriveJ));
	MY_ASSERT_EQUALS(err, KErrNone);
		
	TRAP(err, iSession->DriveListL(driveList));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(driveList.Count(), 4);
	MY_ASSERT_EQUALS(driveList[0], EDriveD);	// IndexFile
	MY_ASSERT_EQUALS(driveList[1], EDriveE);	// DB
	MY_ASSERT_EQUALS(driveList[2], EDriveF);	// empty
	MY_ASSERT_EQUALS(driveList[3], EDriveC);	// DB
	driveList.Reset();
	
	TRAP(err, iSession->AvailableDriveListL(driveList));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(2, driveList.Count());
	MY_ASSERT_EQUALS(driveList[0], EDriveE);
	MY_ASSERT_EQUALS(driveList[1], EDriveC);
	driveList.Reset();
	
	TRAP(err, iSession->CurrentDriveInfoL(driveNumber, priority));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(EDriveE, driveNumber);
	MY_ASSERT_EQUALS(TInt(2), (TInt)priority);

	TRAP(err, iSession->GetChildIdsAllL(KMsvGlobalInBoxIndexEntryIdValue, *filter, *selection));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(selection->Count(), 4);
	selection->Reset();
	
	// CMsvEntry related test cases.
	TRAP(err, entrySelection = iEntry->ChildrenOfAvailableDrivesL());
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection->Count(), 4);
	delete entrySelection; entrySelection = NULL;
	TRAP(err, entrySelection = iEntry->ChildrenL());
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection->Count(), 4);
	delete entrySelection; entrySelection = NULL;

	// 8. Remove highest priority drive (D - Index File)
	TRAP(err, iSession->RemoveDriveL(EDriveD));
	MY_ASSERT_EQUALS(err, KErrNone);
		
	TRAP(err, iSession->DriveListL(driveList));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(driveList.Count(), 3);
	MY_ASSERT_EQUALS(driveList[0], EDriveE);	// DB
	MY_ASSERT_EQUALS(driveList[1], EDriveF);	// empty
	MY_ASSERT_EQUALS(driveList[2], EDriveC);	// DB
	driveList.Reset();
	
	TRAP(err, iSession->AvailableDriveListL(driveList));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(2, driveList.Count());
	MY_ASSERT_EQUALS(driveList[0], EDriveE);
	MY_ASSERT_EQUALS(driveList[1], EDriveC);
	driveList.Reset();
	
	TRAP(err, iSession->CurrentDriveInfoL(driveNumber, priority));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(EDriveE, driveNumber);
	MY_ASSERT_EQUALS(TInt(1), (TInt)priority);

	TRAP(err, iSession->GetChildIdsAllL(KMsvGlobalInBoxIndexEntryIdValue, *filter, *selection));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(selection->Count(), 4);
	selection->Reset();
	
	// CMsvEntry related test cases.
	TRAP(err, entrySelection = iEntry->ChildrenOfAvailableDrivesL());
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection->Count(), 4);
	delete entrySelection; entrySelection = NULL;
	TRAP(err, entrySelection = iEntry->ChildrenL());
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection->Count(), 4);
	delete entrySelection; entrySelection = NULL;

	// 9. Remove current drive E (DB)
	TRAP(err, iSession->RemoveDriveL(EDriveE));
	MY_ASSERT_EQUALS(err, KErrNone);
	CActiveScheduler::Start();
	MY_ASSERT_EQUALS(iMsvSesionObserver->iType, MMsvSessionObserver::EMsvMediaChanged);
	MY_ASSERT_EQUALS(iMsvSesionObserver->iArg1, (TInt) EDriveE);
	MY_ASSERT_EQUALS(iMsvSesionObserver->iArg2, (TInt) EDriveF);
	CActiveScheduler::Start();
	MY_ASSERT_EQUALS(iMsvExtraSesionObserver->iType, MMsvSessionObserver::EMsvMediaChanged);
	MY_ASSERT_EQUALS(iMsvExtraSesionObserver->iArg1, (TInt) EDriveE);
	MY_ASSERT_EQUALS(iMsvExtraSesionObserver->iArg2, (TInt) EDriveF);

	TRAP(err, iSession->DriveListL(driveList));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(driveList.Count(), 2);
	MY_ASSERT_EQUALS(driveList[0], EDriveF);	// DB
	MY_ASSERT_EQUALS(driveList[1], EDriveC);	// DB
	driveList.Reset();
	
	TRAP(err, iSession->AvailableDriveListL(driveList));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(2, driveList.Count());
	MY_ASSERT_EQUALS(driveList[0], EDriveF);
	MY_ASSERT_EQUALS(driveList[1], EDriveC);
	driveList.Reset();
	
	TRAP(err, iSession->CurrentDriveInfoL(driveNumber, priority));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(EDriveF, driveNumber);
	MY_ASSERT_EQUALS(TInt(1), (TInt)priority);

	TRAP(err, iSession->GetChildIdsAllL(KMsvGlobalInBoxIndexEntryIdValue, *filter, *selection));
	MY_ASSERT_EQUALS(err, KErrNone);
	// It will have one entry created by mailinit.
	MY_ASSERT_EQUALS(selection->Count(), 0);
	selection->Reset();
	
	// CMsvEntry related test cases.
	TRAP(err, entrySelection = iEntry->ChildrenOfAvailableDrivesL());
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection->Count(), 0);
	delete entrySelection; entrySelection = NULL;
	TRAP(err, entrySelection = iEntry->ChildrenL());
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection->Count(), 0);
	delete entrySelection; entrySelection = NULL;
	TRAP(err, entrySelection = iExtraEntry->ChildrenOfAvailableDrivesL());
	MY_ASSERT_EQUALS(err, KErrArgument);
	TRAP(err, entrySelection = iExtraEntry->ChildrenL());
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection->Count(), 0);
	delete entrySelection; entrySelection = NULL;

	// Create entries in F:
	CreateEntry(iEntry);
	CreateEntry(iEntry);
	CreateEntry(iEntry);
	CreateEntry(iEntry);

	TRAP(err, iSession->GetChildIdsAllL(KMsvGlobalInBoxIndexEntryIdValue, *filter, *selection));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(selection->Count(), 4);
	selection->Reset();
	
	// CMsvEntry related test cases.
	TRAP(err, entrySelection = iEntry->ChildrenOfAvailableDrivesL());
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection->Count(), 4);
	delete entrySelection; entrySelection = NULL;
	TRAP(err, entrySelection = iEntry->ChildrenL());
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection->Count(), 4);
	delete entrySelection; entrySelection = NULL;
	TRAP(err, entrySelection = iExtraEntry->ChildrenOfAvailableDrivesL());
	MY_ASSERT_EQUALS(err, KErrArgument);
	TRAP(err, entrySelection = iExtraEntry->ChildrenL());
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection->Count(), 4);
	delete entrySelection; entrySelection = NULL;

	// 10. Adding E drive (DB) at lowest priority.
	priority = 6;
	TRAP(err, iSession->AddDriveL(EDriveE, priority));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(priority, (TUint) 3);
	CActiveScheduler::Start();
	MY_ASSERT_EQUALS(iMsvSesionObserver->iType, MMsvSessionObserver::EMsvRefreshMessageView);
	MY_ASSERT_EQUALS(iMsvSesionObserver->iArg1, (TInt) EDriveE);	
	CActiveScheduler::Start();
	MY_ASSERT_EQUALS(iMsvExtraSesionObserver->iType, MMsvSessionObserver::EMsvRefreshMessageView);
	MY_ASSERT_EQUALS(iMsvExtraSesionObserver->iArg1, (TInt) EDriveE);	
	
	TRAP(err, iSession->DriveListL(driveList));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(driveList.Count(), 3);
	MY_ASSERT_EQUALS(driveList[0], EDriveF);	// DB
	MY_ASSERT_EQUALS(driveList[1], EDriveC);	// DB
	MY_ASSERT_EQUALS(driveList[2], EDriveE);	// DB
	driveList.Reset();
	
	TRAP(err, iSession->AvailableDriveListL(driveList));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(driveList.Count(), 3);
	MY_ASSERT_EQUALS(driveList[0], EDriveF);	// DB
	MY_ASSERT_EQUALS(driveList[1], EDriveC);	// DB
	MY_ASSERT_EQUALS(driveList[2], EDriveE);	// DB
	driveList.Reset();
	
	TRAP(err, iSession->CurrentDriveInfoL(driveNumber, priority));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(EDriveF, driveNumber);
	MY_ASSERT_EQUALS(TInt(1), (TInt)priority);

	TRAP(err, iSession->GetChildIdsAllL(KMsvGlobalInBoxIndexEntryIdValue, *filter, *selection));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(selection->Count(), 8);
	selection->Reset();
	
	// CMsvEntry related test cases.
	TRAP(err, entrySelection = iEntry->ChildrenOfAvailableDrivesL());
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection->Count(), 8);
	delete entrySelection; entrySelection = NULL;
	TRAP(err, entrySelection = iEntry->ChildrenL());
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection->Count(), 8);
	delete entrySelection; entrySelection = NULL;
	TRAP(err, entrySelection = iExtraEntry->ChildrenOfAvailableDrivesL());
	MY_ASSERT_EQUALS(err, KErrArgument);
	TRAP(err, entrySelection = iExtraEntry->ChildrenL());
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection->Count(), 4);
	delete entrySelection; entrySelection = NULL;
	
	// 11. Adding D drive (Index file) at somewhere middle.
	priority = 3;
	TRAP(err, iSession->AddDriveL(EDriveD, priority));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(priority, (TUint)3);
	CActiveScheduler::Start();
	MY_ASSERT_EQUALS(iMsvSesionObserver->iType, MMsvSessionObserver::EMsvMessageStoreNotSupported);
	MY_ASSERT_EQUALS(iMsvSesionObserver->iArg1, (TInt) EDriveD);
	
	TRAP(err, iSession->DriveListL(driveList));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(driveList.Count(), 4);
	MY_ASSERT_EQUALS(driveList[0], EDriveF);	// DB
	MY_ASSERT_EQUALS(driveList[1], EDriveC);	// DB
	MY_ASSERT_EQUALS(driveList[2], EDriveD);	// Index
	MY_ASSERT_EQUALS(driveList[3], EDriveE);	// DB
	driveList.Reset();
	
	TRAP(err, iSession->AvailableDriveListL(driveList));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(driveList.Count(), 3);
	MY_ASSERT_EQUALS(driveList[0], EDriveF);	// DB
	MY_ASSERT_EQUALS(driveList[1], EDriveC);	// DB
	MY_ASSERT_EQUALS(driveList[2], EDriveE);	// DB
	driveList.Reset();
	
	TRAP(err, iSession->CurrentDriveInfoL(driveNumber, priority));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(EDriveF, driveNumber);
	MY_ASSERT_EQUALS(TInt(1), (TInt)priority);
	

	// 12. Remove non-current drive E (DB).
	TRAP(err, iSession->RemoveDriveL(EDriveE));
	MY_ASSERT_EQUALS(err, KErrNone);
	CActiveScheduler::Start();
	MY_ASSERT_EQUALS(iMsvSesionObserver->iType, MMsvSessionObserver::EMsvRefreshMessageView);
	MY_ASSERT_EQUALS(iMsvSesionObserver->iArg1, (TInt) EDriveE);
	CActiveScheduler::Start();
	MY_ASSERT_EQUALS(iMsvExtraSesionObserver->iType, MMsvSessionObserver::EMsvRefreshMessageView);
	MY_ASSERT_EQUALS(iMsvExtraSesionObserver->iArg1, (TInt) EDriveE);

	TRAP(err, iSession->DriveListL(driveList));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(driveList.Count(), 3);
	MY_ASSERT_EQUALS(driveList[0], EDriveF);	// DB
	MY_ASSERT_EQUALS(driveList[1], EDriveC);	// DB
	MY_ASSERT_EQUALS(driveList[2], EDriveD);	// Index
	driveList.Reset();
	
	TRAP(err, iSession->AvailableDriveListL(driveList));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(2, driveList.Count());
	MY_ASSERT_EQUALS(driveList[0], EDriveF);
	MY_ASSERT_EQUALS(driveList[1], EDriveC);
	driveList.Reset();
	
	TRAP(err, iSession->CurrentDriveInfoL(driveNumber, priority));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(EDriveF, driveNumber);
	MY_ASSERT_EQUALS(TInt(1), (TInt)priority);
	
	TRAP(err, iSession->GetChildIdsAllL(KMsvGlobalInBoxIndexEntryIdValue, *filter, *selection));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(selection->Count(), 4);
	selection->Reset();
	
	// CMsvEntry related test cases.
	TRAP(err, entrySelection = iEntry->ChildrenOfAvailableDrivesL());
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection->Count(), 4);
	delete entrySelection; entrySelection = NULL;
	TRAP(err, entrySelection = iEntry->ChildrenL());
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection->Count(), 4);
	delete entrySelection; entrySelection = NULL;
	TRAP(err, entrySelection = iExtraEntry->ChildrenOfAvailableDrivesL());
	MY_ASSERT_EQUALS(err, KErrArgument);
	TRAP(err, entrySelection = iExtraEntry->ChildrenL());
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection->Count(), 4);
	delete entrySelection; entrySelection = NULL;

	
	// 13. Adding drive E at the highest priority.
	priority = 1;
	TRAP(err, iSession->AddDriveL(EDriveE, priority));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(priority, (TUint)1);
	CActiveScheduler::Start();
	MY_ASSERT_EQUALS(iMsvSesionObserver->iType, MMsvSessionObserver::EMsvMediaChanged);
	MY_ASSERT_EQUALS(iMsvSesionObserver->iArg1, (TInt) EDriveF);	
	MY_ASSERT_EQUALS(iMsvSesionObserver->iArg2, (TInt) EDriveE);
	CActiveScheduler::Start();
	MY_ASSERT_EQUALS(iMsvExtraSesionObserver->iType, MMsvSessionObserver::EMsvMediaChanged);
	MY_ASSERT_EQUALS(iMsvExtraSesionObserver->iArg1, (TInt) EDriveF);	
	MY_ASSERT_EQUALS(iMsvExtraSesionObserver->iArg2, (TInt) EDriveE);
	
	TRAP(err, iSession->DriveListL(driveList));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(driveList.Count(), 4);
	MY_ASSERT_EQUALS(driveList[0], EDriveE);	// DB
	MY_ASSERT_EQUALS(driveList[1], EDriveF);	// DB
	MY_ASSERT_EQUALS(driveList[2], EDriveC);	// DB
	MY_ASSERT_EQUALS(driveList[3], EDriveD);	// Index
	driveList.Reset();
	
	TRAP(err, iSession->AvailableDriveListL(driveList));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(driveList.Count(), 3);
	MY_ASSERT_EQUALS(driveList[0], EDriveE);	// DB
	MY_ASSERT_EQUALS(driveList[1], EDriveF);	// DB
	MY_ASSERT_EQUALS(driveList[2], EDriveC);	// DB	
	driveList.Reset();
	
	TRAP(err, iSession->CurrentDriveInfoL(driveNumber, priority));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(EDriveE, driveNumber);
	MY_ASSERT_EQUALS(TInt(1), (TInt)priority);
	
	TRAP(err, iSession->GetChildIdsAllL(KMsvGlobalInBoxIndexEntryIdValue, *filter, *selection));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(selection->Count(), 8);
	selection->Reset();
	
	// CMsvEntry related test cases.
	TRAP(err, entrySelection = iEntry->ChildrenOfAvailableDrivesL());
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection->Count(), 8);
	delete entrySelection; entrySelection = NULL;
	TRAP(err, entrySelection = iEntry->ChildrenL());
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection->Count(), 8);
	delete entrySelection; entrySelection = NULL;
	TRAP(err, entrySelection = iExtraEntry->ChildrenOfAvailableDrivesL());
	MY_ASSERT_EQUALS(err, KErrArgument);
	TRAP(err, entrySelection = iExtraEntry->ChildrenL());
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection->Count(), 4);
	delete entrySelection; entrySelection = NULL;
	
	// 14. Adding drive J (Corrupt DB) at highest priority
	drive.Set(TDriveUnit(EDriveJ).Name());
	parse1.Set(KDbFileName, &drive, NULL);
	dBFileJ = parse1.FullName();
	err = RSqlDatabase::Delete(dBFileJ);
	err = file->Copy(KMsgTestIndexFile, dBFileJ);
	MY_ASSERT_EQUALS(KErrNone, err);

	priority = 1;
	TRAP(err, iSession->AddDriveL(EDriveJ, priority));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(priority, (TUint)1);
	CActiveScheduler::Start();
	MY_ASSERT_EQUALS(iMsvSesionObserver->iType, MMsvSessionObserver::EMsvMessageStoreCorrupt);
	MY_ASSERT_EQUALS(iMsvSesionObserver->iArg1, (TInt) EDriveJ);
	CActiveScheduler::Start();
	MY_ASSERT_EQUALS(iMsvSesionObserver->iType, MMsvSessionObserver::EMsvMediaChanged);
	MY_ASSERT_EQUALS(iMsvSesionObserver->iArg1, (TInt) EDriveE);	
	MY_ASSERT_EQUALS(iMsvSesionObserver->iArg2, (TInt) EDriveJ);
	CActiveScheduler::Start();
	MY_ASSERT_EQUALS(iMsvExtraSesionObserver->iType, MMsvSessionObserver::EMsvMediaChanged);
	MY_ASSERT_EQUALS(iMsvExtraSesionObserver->iArg1, (TInt) EDriveE);	
	MY_ASSERT_EQUALS(iMsvExtraSesionObserver->iArg2, (TInt) EDriveJ);
	
	TRAP(err, iSession->DriveListL(driveList));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(driveList.Count(), 5);
	MY_ASSERT_EQUALS(driveList[0], EDriveJ);	// DB
	MY_ASSERT_EQUALS(driveList[1], EDriveE);	// DB
	MY_ASSERT_EQUALS(driveList[2], EDriveF);	// DB
	MY_ASSERT_EQUALS(driveList[3], EDriveC);	// DB
	MY_ASSERT_EQUALS(driveList[4], EDriveD);	// Index
	driveList.Reset();
	
	TRAP(err, iSession->AvailableDriveListL(driveList));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(driveList.Count(), 4);
	MY_ASSERT_EQUALS(driveList[0], EDriveJ);	// DB
	MY_ASSERT_EQUALS(driveList[1], EDriveE);	// DB
	MY_ASSERT_EQUALS(driveList[2], EDriveF);	// DB
	MY_ASSERT_EQUALS(driveList[3], EDriveC);	// DB	
	driveList.Reset();
	
	TRAP(err, iSession->CurrentDriveInfoL(driveNumber, priority));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(EDriveJ, driveNumber);
	MY_ASSERT_EQUALS(TInt(1), (TInt)priority);
	
	TRAP(err, iSession->GetChildIdsAllL(KMsvGlobalInBoxIndexEntryIdValue, *filter, *selection));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(selection->Count(), 8);
	selection->Reset();
	
	// CMsvEntry related test cases.
	TRAP(err, entrySelection = iEntry->ChildrenOfAvailableDrivesL());
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection->Count(), 8);
	delete entrySelection; entrySelection = NULL;
	TRAP(err, entrySelection = iEntry->ChildrenL());
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection->Count(), 8);
	delete entrySelection; entrySelection = NULL;
	TRAP(err, entrySelection = iExtraEntry->ChildrenOfAvailableDrivesL());
	MY_ASSERT_EQUALS(err, KErrArgument);
	TRAP(err, entrySelection = iExtraEntry->ChildrenL());
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entrySelection->Count(), 0);
	delete entrySelection; entrySelection = NULL;
	
	
	
	// 15. Update priority of D (Index) to 1.
	priority = 1;
	TRAP(err, iSession->UpdateDrivePriorityL(EDriveD, priority));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(priority, (TUint)1);
	
	TRAP(err, iSession->DriveListL(driveList));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(driveList.Count(), 5);
	MY_ASSERT_EQUALS(driveList[0], EDriveD);	// Index
	MY_ASSERT_EQUALS(driveList[1], EDriveJ);	// DB
	MY_ASSERT_EQUALS(driveList[2], EDriveE);	// DB
	MY_ASSERT_EQUALS(driveList[3], EDriveF);	// DB
	MY_ASSERT_EQUALS(driveList[4], EDriveC);	// DB
	driveList.Reset();
	
	TRAP(err, iSession->AvailableDriveListL(driveList));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(driveList.Count(), 4);
	MY_ASSERT_EQUALS(driveList[0], EDriveJ);	// DB
	MY_ASSERT_EQUALS(driveList[1], EDriveE);	// DB
	MY_ASSERT_EQUALS(driveList[2], EDriveF);	// DB
	MY_ASSERT_EQUALS(driveList[3], EDriveC);	// DB	
	driveList.Reset();
	
	TRAP(err, iSession->CurrentDriveInfoL(driveNumber, priority));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(EDriveJ, driveNumber);
	MY_ASSERT_EQUALS(TInt(2), (TInt)priority);
	
	
	// 16. Update priority of C (DB) to 1.
	priority = 1;
	TRAP(err, iSession->UpdateDrivePriorityL(EDriveC, priority));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(priority, (TUint)1);
	CActiveScheduler::Start();
	MY_ASSERT_EQUALS(iMsvSesionObserver->iType, MMsvSessionObserver::EMsvMediaChanged);
	MY_ASSERT_EQUALS(iMsvSesionObserver->iArg1, (TInt) EDriveJ);	
	MY_ASSERT_EQUALS(iMsvSesionObserver->iArg2, (TInt) EDriveC);
	CActiveScheduler::Start();
	MY_ASSERT_EQUALS(iMsvExtraSesionObserver->iType, MMsvSessionObserver::EMsvMediaChanged);
	MY_ASSERT_EQUALS(iMsvExtraSesionObserver->iArg1, (TInt) EDriveJ);	
	MY_ASSERT_EQUALS(iMsvExtraSesionObserver->iArg2, (TInt) EDriveC);

	TRAP(err, iSession->DriveListL(driveList));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(driveList.Count(), 5);
	MY_ASSERT_EQUALS(driveList[0], EDriveC);	// DB
	MY_ASSERT_EQUALS(driveList[1], EDriveD);	// Index
	MY_ASSERT_EQUALS(driveList[2], EDriveJ);	// DB
	MY_ASSERT_EQUALS(driveList[3], EDriveE);	// DB
	MY_ASSERT_EQUALS(driveList[4], EDriveF);	// DB	
	driveList.Reset();
	
	TRAP(err, iSession->AvailableDriveListL(driveList));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(driveList.Count(), 4);
	MY_ASSERT_EQUALS(driveList[0], EDriveC);	// DB	
	MY_ASSERT_EQUALS(driveList[1], EDriveJ);	// DB
	MY_ASSERT_EQUALS(driveList[2], EDriveE);	// DB
	MY_ASSERT_EQUALS(driveList[3], EDriveF);	// DB	
	driveList.Reset();
	
	TRAP(err, iSession->CurrentDriveInfoL(driveNumber, priority));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(EDriveC, driveNumber);
	MY_ASSERT_EQUALS(TInt(1), (TInt)priority);
	
	
	// 17. Update priority of J (DB) to 6.
	priority = 6;
	TRAP(err, iSession->UpdateDrivePriorityL(EDriveJ, priority));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(priority, (TUint)5);
	
	TRAP(err, iSession->DriveListL(driveList));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(driveList.Count(), 5);
	MY_ASSERT_EQUALS(driveList[0], EDriveC);	// DB
	MY_ASSERT_EQUALS(driveList[1], EDriveD);	// Index
	MY_ASSERT_EQUALS(driveList[2], EDriveE);	// DB
	MY_ASSERT_EQUALS(driveList[3], EDriveF);	// DB	
	MY_ASSERT_EQUALS(driveList[4], EDriveJ);	// DB
	driveList.Reset();
	
	TRAP(err, iSession->AvailableDriveListL(driveList));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(driveList.Count(), 4);
	MY_ASSERT_EQUALS(driveList[0], EDriveC);	// DB	
	MY_ASSERT_EQUALS(driveList[1], EDriveE);	// DB
	MY_ASSERT_EQUALS(driveList[2], EDriveF);	// DB	
	MY_ASSERT_EQUALS(driveList[3], EDriveJ);	// DB
	driveList.Reset();
	
	TRAP(err, iSession->CurrentDriveInfoL(driveNumber, priority));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(EDriveC, driveNumber);
	MY_ASSERT_EQUALS(TInt(1), (TInt)priority);
	
	// For code coverage.
	iSession->Session().PrintCache();
	
	CleanupStack::PopAndDestroy(3); // driveList, selection, filter	
	CleanupStack::PopAndDestroy(file);
	CleanupStack::PopAndDestroy(); // fs
	}
	
	

void CTestMsvSession::CTestAddDriveWithNoDriveL()
	{
	_LIT(KFunction, "CTestAddDriveWithNoDriveL");
	INFO_PRINTF1(KFunction);
	
	TParse parse;
	SetupINIFile(0);
	InitialiseSessionObserver();
	
	// 1. Add Drive having valid message store.
	// Create DB for drive D.
	TPtrC drive = TDriveUnit(EDriveD).Name();
	parse.Set(KDbFileName, &drive, NULL);
	TFileName dBFile = parse.FullName();
	TInt err = RSqlDatabase::Delete(dBFile);
	err = RSqlDatabase::Copy(KBaseDBFileName, dBFile);
	MY_ASSERT_EQUALS(KErrNone, err);
		
	TDriveNumber driveNumber = EDriveD;
	TUint priority = 2;
	TRAP(err, iSession->AddDriveL(driveNumber, priority));
	MY_ASSERT_EQUALS(err, KErrNone);
	CActiveScheduler::Start();
	MY_ASSERT_EQUALS(iMsvSesionObserver->iType, MMsvSessionObserver::EMsvRefreshMessageView);
	MY_ASSERT_EQUALS(iMsvSesionObserver->iArg1, (TInt) EDriveD);
	CActiveScheduler::Start();
	MY_ASSERT_EQUALS(iMsvExtraSesionObserver->iType, MMsvSessionObserver::EMsvRefreshMessageView);
	MY_ASSERT_EQUALS(iMsvExtraSesionObserver->iArg1, (TInt) EDriveD);
	
	TRAP(err, iSession->CurrentDriveInfoL(driveNumber, priority));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(EDriveC, driveNumber);
	MY_ASSERT_EQUALS(TInt(1), (TInt)priority);
	
	RArray<TDriveNumber> driveList;
	TRAP(err, iSession->DriveListL(driveList));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(2, driveList.Count());
	MY_ASSERT_EQUALS(driveList[0], EDriveC);
	MY_ASSERT_EQUALS(driveList[1], EDriveD);
	driveList.Reset();
	
	TRAP(err, iSession->AvailableDriveListL(driveList));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(2, driveList.Count());
	MY_ASSERT_EQUALS(driveList[0], EDriveC);
	MY_ASSERT_EQUALS(driveList[1], EDriveD);
	driveList.Reset();
	
	// 2. Add drive which does not have a message store.
	drive.Set(TDriveUnit(4).Name());
	parse.Set(KDbFileName, &drive, NULL);
	dBFile = parse.FullName();
	RSqlDatabase::Delete(dBFile);
	
	driveNumber = EDriveE;
	priority = 2;
	TRAP(err, iSession->AddDriveL(driveNumber, priority));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	TRAP(err, iSession->DriveListL(driveList));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(3, driveList.Count());
	MY_ASSERT_EQUALS(driveList[0], EDriveC);
	MY_ASSERT_EQUALS(driveList[1], EDriveE);
	MY_ASSERT_EQUALS(driveList[2], EDriveD);
	driveList.Reset();
	
	TRAP(err, iSession->AvailableDriveListL(driveList));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(2, driveList.Count());
	MY_ASSERT_EQUALS(driveList[0], EDriveC);
	MY_ASSERT_EQUALS(driveList[1], EDriveD);
	driveList.Reset();
	
	// 3. Add non-existent drive.
	TRAP(err, iSession->AddDriveL((TDriveNumber)99, priority));
	MY_ASSERT_EQUALS(err, KErrArgument);
		
	// 4. Add already added drive.
	TRAP(err, iSession->AddDriveL(driveNumber, priority));
	MY_ASSERT_EQUALS(err, KErrAlreadyExists);
	
	// 5. Add drive not present in the device.
	TRAP(err, iSession->AddDriveL(EDriveS, priority));
	MY_ASSERT_EQUALS(err, KErrNotFound);
	
	// 6. Add drive with invalid priority.
	priority = 8;
	TRAP(err, iSession->AddDriveL(EDriveF, priority));
	MY_ASSERT_EQUALS(err, KErrArgument);
		
	// 7. Add drive at priority higher than number 
	// of drive present in the preferred drive list.
	drive.Set(TDriveUnit(5).Name());
	parse.Set(KDbFileName, &drive, NULL);
	dBFile = parse.FullName();
	RSqlDatabase::Delete(dBFile);
	priority = 7;
	TRAP(err, iSession->AddDriveL(EDriveF, priority));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(priority, (TUint)4);
	
	TRAP(err, iSession->DriveListL(driveList));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(4, driveList.Count());
	MY_ASSERT_EQUALS(driveList[0], EDriveC);
	MY_ASSERT_EQUALS(driveList[1], EDriveE);
	MY_ASSERT_EQUALS(driveList[2], EDriveD);
	MY_ASSERT_EQUALS(driveList[3], EDriveF);
	driveList.Reset();
	
	TRAP(err, iSession->AvailableDriveListL(driveList));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(2, driveList.Count());
	MY_ASSERT_EQUALS(driveList[0], EDriveC);
	MY_ASSERT_EQUALS(driveList[1], EDriveD);
	driveList.Close();
	}
	
	
void CTestMsvSession::CTestUpdateDrivePriorityWithNoDriveL()
	{
	_LIT(KFunction, "CTestUpdateDrivePriorityWithNoDriveL");
	INFO_PRINTF1(KFunction);
		
	SetupINIFile(0);
	InitialiseSessionObserver();
	
	RArray<TDriveNumber> driveList;
	TRAPD(err, iSession->DriveListL(driveList));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(driveList.Count(), 1);
	MY_ASSERT_EQUALS(driveList[0], EDriveC);
	driveList.Close();

	// 1. Sending wrong priority value.
	TDriveNumber driveNumber = EDriveC;
	TUint newPriority = 8;
	TRAP(err, iSession->UpdateDrivePriorityL(driveNumber, newPriority));
	MY_ASSERT_EQUALS(err, KErrArgument);
	
	// 2. Updating non-existent drive.
	driveNumber = EDriveD;
	newPriority = 4;
	TRAP(err, iSession->UpdateDrivePriorityL(driveNumber, newPriority));
	MY_ASSERT_EQUALS(err, KErrNotFound);
	
	// 3. Send non-existent value for driceNumber
	TRAP(err, iSession->UpdateDrivePriorityL((TDriveNumber)99, newPriority));
	MY_ASSERT_EQUALS(err, KErrArgument);
	
	// 4. Updating priority of the only drive present in the list.
	driveNumber = EDriveC;
	newPriority = 4;
	TRAP(err, iSession->UpdateDrivePriorityL(driveNumber, newPriority));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	TRAP(err, iSession->CurrentDriveInfoL(driveNumber, newPriority));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(EDriveC, driveNumber);
	MY_ASSERT_EQUALS(TInt(1), (TInt)newPriority);
	}



// Test cases when preferred drive list ini file has 
// just one drive (Non system drive).


void CTestMsvSession::CTestDriveListWithSingleDriveL()
	{
	_LIT(KFunction, "CTestDriveListWithSingleDriveL");
	INFO_PRINTF1(KFunction);
	
	// 1. Drive D is empty, should create a message store in D:
	RFs fs;
	CleanupClosePushL(fs);
	TParse parse;
	TInt err = fs.Connect();
	MY_ASSERT_EQUALS(err, KErrNone);	
	TPtrC drive = TDriveUnit(EDriveD).Name();
	parse.Set(KDbFileName, &drive, NULL);
	TFileName dBFile = parse.FullName();
	err = RSqlDatabase::Delete(dBFile);
		
	parse.Set(KMsgIndexFile, &drive, NULL);
	err = fs.MkDirAll(parse.DriveAndPath());
	CFileMan* file = CFileMan::NewL(fs);
	TFileName indexFile = parse.FullName();
	file->Delete(indexFile);	
	
	drive.Set(TDriveUnit(EDriveC).Name());
	parse.Set(KDbFileName, &drive, NULL);	
	RSqlDatabase::Delete(parse.FullName());
			
	SetupINIFile(1);
	InitialiseSessionObserver();
	
	RArray<TDriveNumber> driveList;
	TRAP(err, iSession->DriveListL(driveList));
	ASSERT_EQUALS(driveList.Count(), 2);
	ASSERT_EQUALS(driveList[0], EDriveD);
	ASSERT_EQUALS(driveList[1], EDriveC);
	driveList.Reset();
	
	TRAP(err, iSession->AvailableDriveListL(driveList));
	MY_ASSERT_EQUALS(driveList.Count(), 1);
	MY_ASSERT_EQUALS(driveList[0], EDriveD);
	driveList.Reset();
	
	TDriveNumber driveNumber;
	TUint priority;
	
	TRAP(err, iSession->CurrentDriveInfoL(driveNumber, priority));		
	ASSERT_EQUALS(EDriveD, driveNumber);
	ASSERT_EQUALS(TInt(1), (TInt)priority);
	
	// Resetting.
	delete file;
	TRAP(err, TearDownL());
	MY_ASSERT_EQUALS(err, KErrNone);
	file = CFileMan::NewL(fs);
	
	// 2. Drive D has index file.
	err = RSqlDatabase::Delete(dBFile);
	drive.Set(TDriveUnit(EDriveD).Name());
	parse.Set(KMsgIndexFile, &drive, NULL);
	err = fs.MkDirAll(parse.DriveAndPath());
	err = file->Copy(KMsgTestIndexFile, indexFile);
	MY_ASSERT_EQUALS(err, KErrNone);
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	iUnsupportedDrivesPresent = ETrue;
#endif
	InitialiseSessionObserver();
	
	TRAP(err, iSession->DriveListL(driveList));
	MY_ASSERT_EQUALS(driveList.Count(), 2);
	MY_ASSERT_EQUALS(driveList[0], EDriveD);
	MY_ASSERT_EQUALS(driveList[1], EDriveC);
	driveList.Reset();
	
	TRAP(err, iSession->AvailableDriveListL(driveList));
	MY_ASSERT_EQUALS(driveList.Count(), 1);
	MY_ASSERT_EQUALS(driveList[0], EDriveC);
	driveList.Close();
	
	TRAP(err, iSession->CurrentDriveInfoL(driveNumber, priority));		
	ASSERT_EQUALS(EDriveC, driveNumber);
	ASSERT_EQUALS(TInt(2), (TInt)priority);	

	delete file;
	CleanupStack::PopAndDestroy(); //fs
	}



void CTestMsvSession::CTestGetChildrenAll()
	{
	_LIT(KFunction, "CTestGetChildrenAll");
	INFO_PRINTF1(KFunction);
	
	SetupINIFile(1);
	InitialiseSessionObserver();
	
	CArrayPtrFlat<CMsvClientEntry>* entries = new (ELeave) CArrayPtrFlat<CMsvClientEntry>(8);
	TMsvSelectionOrdering order(0, EMsvSortByNone, ETrue);
	TInt err = iSession->Session().GetChildrenAll(KMsvRootIndexEntryIdValue, *entries, order);
	MY_ASSERT_EQUALS(err, KErrArgument);
	
	entries->Reset();
	err = iSession->Session().GetChildrenAll(KMsvLocalServiceIndexEntryIdValue, *entries, order);
	MY_ASSERT_EQUALS(err, KErrArgument);
	
	entries->Reset();
	err = iSession->Session().GetChildrenAll(KMsvUnkownServiceIndexEntryIdValue, *entries, order);
	MY_ASSERT_EQUALS(err, KErrArgument);
	
	entries->Reset();
	err = iSession->Session().GetChildrenAll(MaskTMsvId((TUint)6, KMsvGlobalInBoxIndexEntryIdValue), *entries, order);
	MY_ASSERT_EQUALS(err, KErrArgument);
	
	entries->Reset();
	for(TInt i=0; i<1500; i++)
		{
		CreateEntry(iEntry);
		}
	err = iSession->Session().GetChildrenAll(KMsvGlobalInBoxIndexEntryIdValue, *entries, order);
	MY_ASSERT_EQUALS(err, KErrNone);
	entries->Reset();
	delete entries;	
	}


void CTestMsvSession::CTestGetEntry()
	{
	_LIT(KFunction, "CTestGetEntry");
	INFO_PRINTF1(KFunction);

	CMsvEntry* entry = NULL;
	CMsvEntrySelection* selection = NULL;
	// Boot server with multiple drives.
	SetupINIFile(2);
	InitialiseSessionObserver();
	
	// 1. Negative tests related to aIsGlobalEntry parameter.
	// Create 2 children under Inbox to work with.
	CreateEntry(iEntry);
	CreateEntry(iEntry);
	// Cannot get a global entry for a non-standard Id.
	TRAPD(err, selection = iEntry->ChildrenL());
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, entry = iSession->GetEntryL(selection->At(0), ETrue));
	MY_ASSERT_EQUALS(err, KErrArgument);
	// Done.
	delete selection;
	
	// 2. Positive tests related to aIsGlobalEntry parameter.
	// Can get a global entry for a standard Id.
	TRAP(err, entry = iSession->GetEntryL(KMsvGlobalInBoxIndexEntryIdValue, ETrue));
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, selection = entry->ChildrenOfAvailableDrivesL());
	MY_ASSERT_EQUALS(err, KErrNone);
	delete selection;
	delete entry;
	// Can get a non-global entry for a standard Id.
	TRAP(err, entry = iSession->GetEntryL(KMsvGlobalInBoxIndexEntryIdValue, EFalse));
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, selection = entry->ChildrenOfAvailableDrivesL());
	MY_ASSERT_EQUALS(err, KErrArgument);
	delete entry;
	// Can get a non-global entry for a non-standard Id.
	TRAP(err, selection = iEntry->ChildrenL());
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, entry = iSession->GetEntryL(selection->At(0), EFalse));
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, selection = entry->ChildrenOfAvailableDrivesL());
	MY_ASSERT_EQUALS(err, KErrArgument);
	// Done.
	delete selection;
	delete entry;
	}



void CTestMsvSession::ResetRepositoryL()
	{
	const TInt KUidConifgFileValue = 0x10286a26; //duplicating these here for now
	const TUid KUidConfigFile = {KUidConifgFileValue};
	
	CRepository* repository = CRepository::NewL(KUidConfigFile);
	
	// Reset everything in the repository. Default values supplied will be
	// filled in.
	TInt err = repository->Reset();
	User::LeaveIfError(err);
	delete repository;
	}

CTestSuite* CTestMsvSession::CreateSuiteL(const TDesC& aName)
	{
	SUB_SUITE;

	// Test cases when preferred drive list ini file is empty.
	ADD_ASYNC_TEST_STEP(CTestDriveListWithNoDriveL);		// 	DriveListL()
	ADD_ASYNC_TEST_STEP(CTestGetChildrenAll);
	ADD_ASYNC_TEST_STEP(CTestAddDriveWithNoDriveL);
	ADD_ASYNC_TEST_STEP(CTestUpdateDrivePriorityWithNoDriveL);
	
	//Added for one drive available in INI file 
	ADD_ASYNC_TEST_STEP(CTestDriveListWithSingleDriveL);
	ADD_ASYNC_TEST_STEP(CTestDriveListWithMultipleDriveL);
	
	ADD_ASYNC_TEST_STEP(CTestGetEntry);
	
	END_SUITE;
	}
