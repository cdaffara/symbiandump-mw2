// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// MACRO Declaration
// 
//

/**
 @file t_cmsventry_step.cpp
 @internalTechnology
*/

#include "t_cmsventry_step.h"
#include <sqldb.h>
#include <msvruids.h>
#include <msvuids.h>
#include <msvids.h>

#define DEFAULT_RETURN KErrNone
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
_LIT(KMsgIndexFile, "\\private\\1000484b\\Mail2\\Index");
_LIT(KMsvServerPattern, "!MsvServer*");
_LIT(KPreferredDriveListFilePath, "C:\\private\\1000484b\\msgprioritydrivelist.ini");
_LIT(KMsgTestPriorityDriveListFile10, "C:\\private\\1000484b\\msgprioritydrivelist10.ini");
_LIT(KMsgTestPriorityDriveListFile2, "C:\\private\\1000484b\\msgprioritydrivelist2.ini");


CTestCMsvEntry::CTestCMsvEntry()
	{
	}

CTestCMsvEntry::~CTestCMsvEntry()
	{
	}



// ---------- Utility methods. ---------------

void CTestCMsvEntry::SetupL()
	{
	_LIT(KFunction, "SetupL");
	INFO_PRINTF1(KFunction);

	__UHEAP_MARK;
	
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
	
	CleanupStack::PopAndDestroy();
	fs.Close();
	}


void CTestCMsvEntry::ResetRepositoryL()
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



void CTestCMsvEntry::TearDownL()
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

		delete iSession;
		iSession = NULL;	
		delete iMsvSessionObserver;
		iMsvSessionObserver = NULL;
		
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
	
	// C:
	TParse parse;
	TPtrC drive = TDriveUnit(EDriveC).Name();
	parse.Set(KDbFileName, &drive, NULL);
	TFileName dbFile = parse.FullName();
	RSqlDatabase::Delete(dbFile);		// Ignore error.
	parse.Set(KMsgIndexFile, &drive, NULL);
	TFileName filename = parse.DriveAndPath();
	err = file->RmDir(parse.DriveAndPath());
	// D:
	drive.Set(TDriveUnit(EDriveD).Name());
	parse.Set(KDbFileName, &drive, NULL);
	dbFile = parse.FullName();
	RSqlDatabase::Delete(dbFile);		// Ignore error.
	parse.Set(KMsgIndexFile, &drive, NULL);
	file->RmDir(parse.DriveAndPath());
	// E:
	drive.Set(TDriveUnit(EDriveE).Name());
	parse.Set(KDbFileName, &drive, NULL);
	dbFile = parse.FullName();
	RSqlDatabase::Delete(dbFile);		// Ignore error.
	parse.Set(KMsgIndexFile, &drive, NULL);
	file->RmDir(parse.DriveAndPath());
	// F:
	drive.Set(TDriveUnit(EDriveF).Name());
	parse.Set(KDbFileName, &drive, NULL);
	dbFile = parse.FullName();
	RSqlDatabase::Delete(dbFile);		// Ignore error.
	parse.Set(KMsgIndexFile, &drive, NULL);
	file->RmDir(parse.DriveAndPath());

	delete file;
	fs.Close();
	
	SetupINIFile(2);
	
	__UHEAP_MARKEND;
	}


void CTestCMsvEntry::SetupINIFile(TInt aPref)
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
		case 2:		//driveList with multiple drive.	
			err = file->Copy(KMsgTestPriorityDriveListFile2, fileName);
			break;
		case 10:	//driveList with multiple drive.	
			err = file->Copy(KMsgTestPriorityDriveListFile10, fileName);
			break;
		default:
			MY_ASSERT_TRUE(EFalse);
		}
	MY_ASSERT_EQUALS(err, KErrNone);
	fs.Close();
	delete file;	
	}



void CTestCMsvEntry::OpenASession()
	{
	_LIT(KFunction, "OpenASession");
	INFO_PRINTF1(KFunction);

	// Resetting repository through the session first..
	TRAPD(err, iMsvSessionObserver = new(ELeave) CDummyObserver());	
	MY_ASSERT_EQUALS(err, KErrNone);
	
	TRAP(err, iSession = CMsvSession::OpenSyncL(*iMsvSessionObserver));
	MY_ASSERT_EQUALS(err, KErrNone);
	}


void CTestCMsvEntry::CreateEntry(CMsvEntry *aEntry)
	{
	_LIT(KFunction, "CreateEntry");
	INFO_PRINTF1(KFunction);
	
	TMsvEntry entry;
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm = KUidMsvLocalServiceMtm;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	TRAPD(err, aEntry->CreateL(entry));
	ASSERT_EQUALS(err, KErrNone);
	}


void CTestCMsvEntry::CTestGlobalEntryL()
	{
	_LIT(KFunction, "CTestGlobalEntryL");
	INFO_PRINTF1(KFunction);

	// 1. Test Count() and [] functions with multiple drives in the drive list.
	// 1.0 Boot server with multiple drives in the drive list.
	SetupINIFile(10);
	OpenASession();
	
	// 1.1 Create 2 entries in each drive.
	for(TInt drive = EDriveE; drive >=EDriveC; --drive)
		{
		// Change drive to the target drive.
		CMsvOperationWait* active = CMsvOperationWait::NewLC();
		CMsvOperation* operation = iSession->ChangeDriveL(drive, active->iStatus);
		active->Start();
		CActiveScheduler::Start();
		delete operation;
		CleanupStack::PopAndDestroy(); // active
		// Get the parent, Inbox.
		CMsvEntry* parent = iSession->GetEntryL(KMsvGlobalInBoxIndexEntryId);
		// Create 2 entries in it.
		CreateEntry(parent);
		CreateEntry(parent);
		delete parent;
		}
	
	// 1.2 From the current drive, EDriveC, test Count() and [] by constructing CMsvEntry
	//	   on Inbox appropriately.
	TMsvSelectionOrdering ordering;
	// 1.2.1 Test from EDriveC.
	CMsvEntry* inbox = CMsvEntry::NewL(*iSession, KMsvGlobalInBoxIndexEntryId, ordering, ETrue);
	MY_ASSERT_EQUALS(inbox->Count(), 6);
	delete inbox;
	inbox = CMsvEntry::NewL(*iSession, KMsvGlobalInBoxIndexEntryId, ordering, EFalse);
	MY_ASSERT_EQUALS(inbox->Count(), 2);
	delete inbox;
	// 1.2.3 Test from EDriveD.
	CMsvOperationWait* active = CMsvOperationWait::NewLC();
	CMsvOperation* operation = iSession->ChangeDriveL(EDriveD, active->iStatus);
	active->Start();
	CActiveScheduler::Start();
	delete operation;
	CleanupStack::PopAndDestroy(); // active
	inbox = CMsvEntry::NewL(*iSession, KMsvGlobalInBoxIndexEntryId, ordering, ETrue);
	MY_ASSERT_EQUALS(inbox->Count(), 6);
	delete inbox;
	inbox = CMsvEntry::NewL(*iSession, KMsvGlobalInBoxIndexEntryId, ordering, EFalse);
	MY_ASSERT_EQUALS(inbox->Count(), 2);
	delete inbox;
	// 1.2.3 Test from EDriveE.
	active = CMsvOperationWait::NewLC();
	operation = iSession->ChangeDriveL(EDriveE, active->iStatus);
	active->Start();
	CActiveScheduler::Start();
	delete operation;
	CleanupStack::PopAndDestroy(); // active
	inbox = CMsvEntry::NewL(*iSession, KMsvGlobalInBoxIndexEntryId, ordering, ETrue);
	MY_ASSERT_EQUALS(inbox->Count(), 6);
	delete inbox;
	inbox = CMsvEntry::NewL(*iSession, KMsvGlobalInBoxIndexEntryId, ordering, EFalse);
	MY_ASSERT_EQUALS(inbox->Count(), 2);
	delete inbox;
	
	
	
	// 2. Test the Children() functions with multiple drives in the list.
	// Carrying forward the setup from test 1 above..
	// 2.1 Negative tests related to iIsGlobalEntry.
	// Non-global entry (Inbox) from the current drive, EDriveE.
	inbox = CMsvEntry::NewL(*iSession, KMsvGlobalInBoxIndexEntryId, ordering, EFalse); // Inbox from EDriveE
	CMsvEntrySelection* selection = NULL;
	// Cannot get children from all drives when not global entry.
	TRAPD(err, selection = inbox->ChildrenOfAvailableDrivesL());
	MY_ASSERT_EQUALS(err, KErrArgument);
	MY_ASSERT_TRUE(selection == NULL);
	// Cannot construct a global entry on non-standard TMsvId.
	TRAP(err, selection = inbox->ChildrenL());
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(inbox->Count(), 2);
	MY_ASSERT_EQUALS(selection->Count(), 2);
	// Index operator.
	for(TInt idIndex = 0; idIndex < 2; ++idIndex)
		{
		MY_ASSERT_TRUE((*inbox)[idIndex].Id() == selection->At(idIndex));
		}
	/* Note that [] won't work on indexes > 1 for this as iSortedChildren now has only 2 children*/
	CMsvEntry* child = NULL;
	TRAP(err, child = CMsvEntry::NewL(*iSession, selection->At(0), ordering, ETrue)); // a child entry
	MY_ASSERT_EQUALS(err, KErrArgument);
	// Cannot SetGlobalEntryL on a non-standard TMsvId.
	TRAP(err, inbox->SetStandardFolderEntryL(selection->At(0)));
	MY_ASSERT_EQUALS(err, KErrArgument);
	// Done.
	delete selection;
	delete inbox;
	
	// 2.2 Positive tests related to iIsGlobalEntry.
	// global entry (Inbox) from EDriveE.
	inbox = CMsvEntry::NewL(*iSession, KMsvGlobalInBoxIndexEntryId, ordering, ETrue); // Inbox from EDriveE
	selection = NULL;
	// Can get children from all drives when global entry.
	TRAP(err, selection = inbox->ChildrenOfAvailableDrivesL());
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_TRUE(selection != NULL);
	MY_ASSERT_EQUALS(inbox->Count(), 6);
	MY_ASSERT_EQUALS(selection->Count(), 6);
	// ChildrenL() and ChildrenOfAvailableDrivesL() return the same set of children.
	CMsvEntrySelection* selection1 = NULL;
	TRAP(err, selection1 = inbox->ChildrenL());
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_TRUE(selection1 != NULL);
	MY_ASSERT_EQUALS(selection1->Count(), 6);
	// Index operator.
	for(TInt idIndex = 0; idIndex < 6; ++idIndex)
		{
		MY_ASSERT_TRUE((*inbox)[idIndex].Id() == selection->At(idIndex));
		MY_ASSERT_TRUE((*inbox)[idIndex].Id() == selection1->At(idIndex));
		}
	// Can construct a non-global entry on a non-standard Id.
	TRAP(err, child = CMsvEntry::NewL(*iSession, selection->At(0), ordering, EFalse)); // a child entry
	MY_ASSERT_EQUALS(err, KErrNone);
	// Can set context to a global entry from a non-global entry.
	CMsvEntry* root = CMsvEntry::NewL(*iSession, KMsvRootIndexEntryIdValue, ordering);
	TRAP(err, root->SetStandardFolderEntryL(KMsvGlobalInBoxIndexEntryId));
	MY_ASSERT_EQUALS(err, KErrNone);
	delete root;
	// Done.
	delete selection;
	delete selection1;
	delete child;
	delete inbox;
	}


CTestSuite* CTestCMsvEntry::CreateSuiteL(const TDesC& aName)
// static
	{
	SUB_SUITE;
	ADD_ASYNC_TEST_STEP(CTestGlobalEntryL);
	END_SUITE;
	}


