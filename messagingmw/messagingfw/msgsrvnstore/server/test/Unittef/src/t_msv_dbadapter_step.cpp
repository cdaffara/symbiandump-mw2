// Copyright (c) 2008-2010 Nokia Corporation and/or its subsidiary(-ies).
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
// t_msv_dbadapter_step.cpp
//
#include "t_msv_dbadapter_step.h"
#include "msvcachevisiblefolder.h"
#include "msvindexadapter.h"
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS  
#include "msvconsts.h"
#endif

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



//Database used for testing
_LIT(KSTDMSGINI, "C:\\private\\1000484b\\msgcache.ini");
_LIT(KMSGINI_2, "C:\\private\\1000484b\\msgcache2.ini");

_LIT8(KId, "id ");
_LIT8(KBetween, "between ");
_LIT8(KAnd, " and ");
_LIT8(KOr, " or ");
_LIT8(KIn, " in ");
_LIT8(KRtBrace, ") ");
_LIT8(KLtBrace, "( ");
_LIT8(KComma, ", ");
_LIT8(KSemiColon, ";");
_LIT8(KOrder, " order by id ASC");

#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	_LIT(KTestDbFileName, "\\messaging.db");
	_LIT(KMainDummyDBName, "\\messaging_master.db");
#else
	_LIT(KTestDbFileName, "C:\\messaging.db");
#endif

//Test msv id values
#define KTestMsvId1Value	0x3039
#define KTestMsvId2Value	0x303A
#define KTestMsvId3Value	0x303B
#define KTestMsvId4Value	0x303C

/**
 * CTestDbAdapter
 *
 * Class used for unit testing of CMsvDBAdapter.
 *  - Preq1189 - The test class dealt with only a single database. This database
 *				 was created in SetupL() and deleted in TearDownL() after iMsvDbAdaptor
 *				 was destroyed.
 *  - Preq557 - The test class deals with multiple databases in simulated drives.
 *				SetupL() creates the dummy database (main.db) in the system
 *				default drive and also the messaging database (messaging.db) with
 *				the Root entry as per the design. Wherever other databases are
 *				required, they must be checked for existence in the test function
 *				and deleted if required.
 */


CTestDbAdapter::CTestDbAdapter()
	{
	}



CTestDbAdapter::~CTestDbAdapter()
	{
	
	}



void CTestDbAdapter::SetupL()
	{
	_LIT(KFunction, "SetupL");
	INFO_PRINTF1(KFunction);


    __UHEAP_MARK;
	 
	//Create and open the database	
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)	
	TParse parse;
	TPtrC drive = TDriveUnit(RFs::GetSystemDrive()).Name();
	parse.Set(KTestDbFileName, &drive, NULL);
	TFileName dbName = parse.FullName();
	RSqlDatabase::Delete(dbName);
	
	iMsvDbAdaptor = CMsvDBAdapter::NewL();
	TInt sysDrive = (TInt)RFs::GetSystemDrive();
	TMsvId maxId = 0;
	TRAPD(error, CMsvDBAdapter::CreateDBL(TDriveNumber(sysDrive)));
	ASSERT_EQUALS(error, KErrNone);
	//Let driveId for system drive be 1.
	TRAP(error, iMsvDbAdaptor->AttachDBL(TDriveUnit(sysDrive), 1, maxId));
	ASSERT_EQUALS(error, KErrNone);
	ASSERT_EQUALS(maxId, KMsvRootIndexEntryId); //root
#else
	TFileName dbName(KTestDbFileName);
	RSqlDatabase::Delete(dbName);
	iMsvDbAdaptor = CMsvDBAdapter::NewL(dbName);
#endif
	
	TInt err =	iFileHandle.Connect();
	MY_ASSERT_EQUALS(err, KErrNone);
	CFileMan* file = CFileMan::NewL(iFileHandle);
	file->Copy(KMSGINI_2, KSTDMSGINI);
	delete file;
	
	//Create entry free pool.
	TDblQue<CMsvCacheVisibleFolder> iEntryCache;
	iMsvEntryFreePool = CMsvEntryFreePool::CreateL(&iEntryCache);

#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)	
	//Create the preferred drive list.
	iDriveList = CMsvPreferredDriveList::CreateL();
	
	//Simulate a filled drive list.
	TMsvPreferredDrive driveEntry;
	driveEntry.driveNum = RFs::GetSystemDrive();
	driveEntry.driveId = 1;
	driveEntry.status = EMsvMessageStoreAvailableStatus;
	iDriveList->Insert(driveEntry, 0);
	
	driveEntry.driveNum = EDriveE;
	driveEntry.driveId = 2;
	driveEntry.status = EMsvMessageStoreAvailableStatus;
	iDriveList->Insert(driveEntry, 1);
	
	iDriveList->SetCurrentDriveIndex(0); //C: is default.
#endif
	}



void CTestDbAdapter::TearDownL()
	{
	_LIT(KFunction, "TearDownL");
	INFO_PRINTF1(KFunction);
	
	if(iMsvDbAdaptor)
		{
		delete iMsvDbAdaptor;
		}
	iMsvDbAdaptor = NULL;
	
	//Delete database
	TFileName dbName(KTestDbFileName);
	TInt err = RSqlDatabase::Delete(dbName);
	
	CFileMan* file = CFileMan::NewL(iFileHandle);
	file->Copy(KSTDMSGINI, KMSGINI_2);
	delete file;
	iFileHandle.Close();

#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)	
	delete iDriveList;
#endif
	
	// Delete entry free pool.
#ifdef _DEBUG
	CMsvEntryFreePool::Destroy(iMsvEntryFreePool);
	iMsvEntryFreePool = NULL;
#else
	delete iMsvEntryFreePool;
	iMsvEntryFreePool = NULL;
#endif

	__UHEAP_MARKEND;
	}



void CTestDbAdapter::TestDatabaseBasics()
	{
	_LIT(KFunction, "TestDatabaseBasics");
	INFO_PRINTF1(KFunction);

	// 1. Creation of new database.
	// Already tested in SetupL(). Close the database.	
	delete iMsvDbAdaptor;
	iMsvDbAdaptor = NULL;

#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	// 2. Test creation of already created database adapter.
	//TFileName dbName(KTestDbFileName);
	TRAPD(err, iMsvDbAdaptor = CMsvDBAdapter::NewL());
	MY_ASSERT_EQUALS(err, KErrNone);

	/*// 2.1 Attempt creating database in a drive that already has one.
	//ISSUE*/
	delete iMsvDbAdaptor;
	iMsvDbAdaptor = NULL;
	
	// 3. Test creation of a database, and attaching of another database to it.
	// 3.0 Cleanup: deleting the DB if it was created in an earlier test.
	TParse parse;
	TPtrC drive = TDriveUnit(EDriveE).Name();
	parse.Set(KTestDbFileName, &drive, NULL);
	TFileName dbName = parse.FullName();
	RSqlDatabase::Delete(dbName);
	TInt sysDrive = RFs::GetSystemDrive();
	drive.Set(TDriveUnit(sysDrive).Name());
	parse.Set(KTestDbFileName, &drive, NULL);
	dbName = parse.FullName();
	RSqlDatabase::Delete(dbName);
	
	// 3.1 Create the databases.
	TRAP(err, iMsvDbAdaptor = CMsvDBAdapter::NewL());
	MY_ASSERT_EQUALS(err, KErrNone);
	
	// 3.2 Attach default drive's database.
	TMsvId maxId = 0;
	TInt driveId = 1;
	TRAPD(error, CMsvDBAdapter::CreateDBL(TDriveNumber(sysDrive)));
	MY_ASSERT_EQUALS(error, KErrNone);
	TRAP(err, iMsvDbAdaptor->AttachDBL(TDriveUnit(sysDrive), driveId, maxId));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(maxId, KMsvRootIndexEntryId); //root
	
	// 3.3 Attach external drive's database.
	maxId = 0;
	driveId = 2;
	TRAP(err, CMsvDBAdapter::CreateDBL(TDriveNumber(EDriveE)));
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, iMsvDbAdaptor->AttachDBL(TDriveUnit(EDriveE), driveId, maxId));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	// 3.4 Detach a database.
	driveId = 2;
	TRAP(err, iMsvDbAdaptor->DetachDBL(driveId));
	MY_ASSERT_EQUALS(err, KErrNone);
#else
	// 2. Test creation of already created database.
	TInt err;
	TFileName dbName(KTestDbFileName);
	TRAP(err, iMsvDbAdaptor = CMsvDBAdapter::NewL(dbName));
	MY_ASSERT_EQUALS(err, KErrAlreadyExists);

	delete iMsvDbAdaptor;
	iMsvDbAdaptor = NULL;

	// 3. Test opening of already created database.
	TRAP(err, iMsvDbAdaptor = CMsvDBAdapter::OpenL(dbName));
	MY_ASSERT_EQUALS(err, KErrNone);

	// 4. Test opening of already opened database.
	/*
	CMsvDBAdapter* msvDBAdaptor = NULL;
	TRAP(err, msvDBAdaptor = CMsvDBAdapter::OpenL(dbName));
	MY_ASSERT_EQUALS(err, KErrInUse);
	delete msvDBAdaptor;
	msvDBAdaptor = NULL;
	*/
	delete iMsvDbAdaptor;
	iMsvDbAdaptor = NULL;
	
	
	// 5. Test Deleting database and reopening.
	// The testcase also test opening non-existent DB.
	CMsvDBAdapter::DeleteDB(dbName);

	TRAP(err, iMsvDbAdaptor = CMsvDBAdapter::OpenL(dbName));
	MY_ASSERT_EQUALS(err, KErrNotFound);
	
	delete iMsvDbAdaptor;
	iMsvDbAdaptor = NULL;
#endif
	}




// Create a new entry into DB.
// Does not test anything.
void CTestDbAdapter::CreateEntryL(TMsvId aId, TMsvId aParentId, TUid aUid, TMsvId& aVisibleFolder, TBool aIsVisible /* (Default = EFalse)*/, TBool isPreparationFlag /*(Default=EFalse)*/)
	{
	TMsvEntry sampleTMsvEntry;
	
	sampleTMsvEntry.SetId(aId);
	sampleTMsvEntry.SetParent(aParentId);
	sampleTMsvEntry.iDate.UniversalTime();
	sampleTMsvEntry.iType = aUid;
	sampleTMsvEntry.SetVisible(aIsVisible);
	sampleTMsvEntry.SetVisibleFolderFlag(aIsVisible);
	
	// Extra Information (Needed only in TestGetEntry()
	sampleTMsvEntry.SetUnread(ETrue);
	sampleTMsvEntry.iServiceId = aId+10;		// Some Random Id.
	sampleTMsvEntry.iRelatedId = aId+20;		// Some Random Id.
	sampleTMsvEntry.iMtm = aUid;		
	sampleTMsvEntry.iSize = 32;
	sampleTMsvEntry.iError = NULL;		
	sampleTMsvEntry.iBioType = 64;		
	sampleTMsvEntry.iMtmData1 = 64;		
	sampleTMsvEntry.iMtmData2 = 64;		
	sampleTMsvEntry.iMtmData3 = 64;	
	sampleTMsvEntry.SetInPreparation(isPreparationFlag);
	
	/*
	_LIT(KName, "Hello World");
	HBufC* data = HBufC::NewL(15); 
	TPtr ptr(data->Des());
	ptr.Copy(KName);
	sampleTMsvEntry.iDescription.Set(ptr);
	*/
	
	_LIT8(KName, "Hello World");
	sampleTMsvEntry.iDescription.Set(_L("Hello World"));
	
	iMsvDbAdaptor->CreateEntryL(sampleTMsvEntry, aVisibleFolder);	
	
	}




void CTestDbAdapter::TestCreateEntry()
	{
	_LIT(KFunction, "TestCreateEntry");
	INFO_PRINTF1(KFunction);
	
	TInt err;
	TBool ret = EFalse;
	TMsvId visibleFolder;
	// 1. Check if root entry was created when the database was created.
	visibleFolder = KMsvRootIndexEntryId;
#if (!defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	TRAP(err, CreateEntryL(KMsvRootIndexEntryId, KErrNotFound, KUidMsvRootEntry, visibleFolder, ETrue));
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolder);
#endif
	TRAP(err, ret = EntryExists(KMsvRootIndexEntryId));
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_TRUE(ret);
	
	// 2. Creating duplicate root entry.
	TRAP(err, CreateEntryL(KMsvRootIndexEntryId, KErrNotFound, KUidMsvRootEntry, visibleFolder));
	MY_ASSERT_EQUALS(KSqlErrConstraint, err);

	// 3. Creating service folder under root.
	visibleFolder = KMsvRootIndexEntryId;
	TRAP(err, CreateEntryL(KMsvLocalServiceIndexEntryId, KMsvRootIndexEntryId, KUidMsvServiceEntry, visibleFolder));
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolder);
	TRAP(err, ret = EntryExists(KMsvLocalServiceIndexEntryId));
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_TRUE(ret);
	
	// 4. Creating another service folder under root.
	TRAP(err, CreateEntryL(KFirstFreeEntryId, KMsvRootIndexEntryId, KUidMsvServiceEntry, visibleFolder));
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolder);
	TRAP(err, ret = EntryExists(KFirstFreeEntryId));
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_TRUE(ret);
	
	// 5. Creating normal folder under root.
	TRAP(err, CreateEntryL(KMsvGlobalInBoxIndexEntryId, KMsvRootIndexEntryId, KUidMsvFolderEntry, visibleFolder, ETrue));
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolder);
	TRAP(err, ret = EntryExists(KMsvGlobalInBoxIndexEntryId));
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_TRUE(ret);
	
	// 6. Creating Message entry under root.
	TRAP(err, CreateEntryL(KFirstFreeEntryId+1, KMsvRootIndexEntryId, KUidMsvMessageEntry, visibleFolder));
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolder);
	TRAP(err, ret = EntryExists(KFirstFreeEntryId+1));
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_TRUE(ret);
	
	// 7. Creating attachment entry under root.
	TRAP(err, CreateEntryL(KFirstFreeEntryId+2, KMsvRootIndexEntryId, KUidMsvAttachmentEntry, visibleFolder));
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolder);
	TRAP(err, ret = EntryExists(KFirstFreeEntryId+2));
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_TRUE(ret);
	
	// 8. Creating 100 message entries under Inbox.
	visibleFolder = KMsvGlobalInBoxIndexEntryId;
	for(int index=3; index<103; index++)
		{
		TRAP(err, CreateEntryL(KFirstFreeEntryId+index, KMsvGlobalInBoxIndexEntryId, KUidMsvMessageEntry, visibleFolder));
		//Avoid using MY_ASSERT_EQUALS as it will clutter the log file.
		ASSERT_EQUALS(KErrNone, err);
		ASSERT_EQUALS(KMsvGlobalInBoxIndexEntryId, visibleFolder);
		}

	MY_ASSERT_TRUE(EntryExists(KFirstFreeEntryId+3));
	MY_ASSERT_TRUE(EntryExists(KFirstFreeEntryId+4));
	MY_ASSERT_TRUE(EntryExists(KFirstFreeEntryId+40));
	MY_ASSERT_TRUE(EntryExists(KFirstFreeEntryId+70));
	MY_ASSERT_TRUE(EntryExists(KFirstFreeEntryId+101));
	MY_ASSERT_TRUE(EntryExists(KFirstFreeEntryId+102));
	MY_ASSERT_TRUE(!EntryExists(KFirstFreeEntryId+103));
	MY_ASSERT_TRUE(!EntryExists(KFirstFreeEntryId+104));
	
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	// 9. Attach a new database to it. Create entries in the first and the 
	//	  attached databases using appropriate TMsvId's which have appropriate
	//	  drive masks.
	// 9.0 Cleanup: deleting the DB if it was created in an earlier test.
	TParse parse;
	TPtrC drive = TDriveUnit(EDriveE).Name();
	parse.Set(KTestDbFileName, &drive, NULL);
	TFileName dbName = parse.FullName();
	RSqlDatabase::Delete(dbName);
	
	// 9.1 Create DB with driveId 2. System drive had driveId 1.
	TMsvId maxId = 0;
	TInt driveId = 2;
	TRAP(err, CMsvDBAdapter::CreateDBL(TDriveNumber(EDriveE)));
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, iMsvDbAdaptor->AttachDBL(TDriveUnit(EDriveE), driveId, maxId));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	// 9.2 Create an entry in the first database.
	visibleFolder = KMsvRootIndexEntryId;
	TRAP(err, CreateEntryL(KFirstFreeEntryId+150, KMsvRootIndexEntryId, KUidMsvAttachmentEntry, visibleFolder));
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolder);
	MY_ASSERT_TRUE(EntryExists(KFirstFreeEntryId+150));
	
	// 9.3 Create an entry in the attached database.
	visibleFolder = KMsvRootIndexEntryId;
	TRAP(err, CreateEntryL((driveId<<28)|KFirstFreeEntryId+2, KMsvRootIndexEntryId, KUidMsvAttachmentEntry, visibleFolder));
	MY_ASSERT_EQUALS(KErrNone, err);
	//Notice that the above Id also exists in the first database, and that
	//there is no conflict.
	MY_ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolder);
	MY_ASSERT_TRUE(EntryExists((driveId<<28)|KFirstFreeEntryId+2));
#endif
	}




TBool CTestDbAdapter::EntryExists(TMsvId aId)
	{
	TBool val = EFalse;
	TRAP_IGNORE(val = iMsvDbAdaptor->EntryExistsL(aId));
	return val;
	}
	

void CTestDbAdapter::TestEntryExists()
	{
	_LIT(KFunction, "TestEntryExists");
	INFO_PRINTF1(KFunction);
	
	TInt err;
	TMsvId visibleFolder;
	TUint32 start, stop, diff1;
	CMsvEntrySelection* selection = NULL;
	TRAP_IGNORE(selection = new(ELeave) CMsvEntrySelection());
	TInt bulkCount = 100;

	visibleFolder = KMsvRootIndexEntryId;		
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	// 1. Check if root entry was created when the database was created.
#else
	// 1. Creating root entry.
	TRAP(err, CreateEntryL(KMsvRootIndexEntryId, KErrNotFound, KUidMsvRootEntry, visibleFolder, ETrue));
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolder);
#endif
	start = User::FastCounter(); 
	MY_ASSERT_TRUE(EntryExists(KMsvRootIndexEntryId));
	stop = User::FastCounter();
	diff1 = stop - start;
	
	// 2. Create in bulk and test
	TRAP(err, CreateEntryL(KFirstFreeEntryId, KMsvRootIndexEntryId, KUidMsvMessageEntry, visibleFolder));
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP_IGNORE(selection->AppendL(KFirstFreeEntryId));
	visibleFolder = KFirstFreeEntryId;
	for(TInt index = 1; index < bulkCount; ++index)
		{
		TRAP(err, CreateEntryL(KFirstFreeEntryId+index, KFirstFreeEntryId, KUidMsvMessageEntry, visibleFolder));
		TRAP_IGNORE(selection->AppendL(KFirstFreeEntryId+index));
		ASSERT_EQUALS(err, KErrNone);
		}
		
	// 3. Test scalar select query method
	diff1 = 0;
	for(TInt index = 0; index < bulkCount; ++index)
		{
		start = User::FastCounter(); 
		ASSERT_TRUE(EntryExists(selection->At(index)));
		stop = User::FastCounter();
		diff1 += stop - start;
		}
	diff1 = diff1 / bulkCount;
	
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	// 4. Create and attach a second database. Create entries in it and test.
	// 4.0 Cleanup: deleting the DB if it was created in an earlier test.
	TParse parse;
	TPtrC drive = TDriveUnit(EDriveE).Name();
	parse.Set(KTestDbFileName, &drive, NULL);
	TFileName dbName = parse.FullName();
	RSqlDatabase::Delete(dbName);
	
	// 4.1 Create DB with driveId 2. System drive had driveId 1.
	TMsvId maxId = 0;
	TInt driveId = 2;
	TRAP(err, CMsvDBAdapter::CreateDBL(TDriveNumber(EDriveE)));
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, iMsvDbAdaptor->AttachDBL(TDriveUnit(EDriveE), driveId, maxId));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	// 4.2 Create an entry in the first database.
	visibleFolder = KMsvRootIndexEntryId;
	TRAP(err, CreateEntryL(KFirstFreeEntryId+150, KMsvRootIndexEntryId, KUidMsvAttachmentEntry, visibleFolder));
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolder);
	MY_ASSERT_TRUE(EntryExists(KFirstFreeEntryId+150));
	
	// 4.3 Create an entry in the attached database.
	visibleFolder = KMsvRootIndexEntryId;
	TRAP(err, CreateEntryL((driveId<<28)|KFirstFreeEntryId+2, KMsvRootIndexEntryId, KUidMsvAttachmentEntry, visibleFolder));
	MY_ASSERT_EQUALS(KErrNone, err);
	//Notice that the above Id also exists in the first database, and that
	//there is no conflict.
	MY_ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolder);
	MY_ASSERT_TRUE(EntryExists((driveId<<28)|KFirstFreeEntryId+2));
#endif
	delete selection;
		
	}


void CTestDbAdapter::TestDeleteEntry()
	{
	_LIT(KFunction, "TestDeleteEntry");
	INFO_PRINTF1(KFunction);

	TInt err;
	TMsvId visibleFolder = KMsvRootIndexEntryId;
	
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	// 1. Check if root entry was created when the database was created.
#else
	// 1. Creating root entry.
	TRAP(err, CreateEntryL(KMsvRootIndexEntryId, KErrNotFound, KUidMsvRootEntry, visibleFolder, ETrue));
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolder);
#endif
	
	MY_ASSERT_TRUE(EntryExists(KMsvRootIndexEntryId));
	
	// 2. Creating service folder under root.
	TRAP(err, CreateEntryL(KMsvLocalServiceIndexEntryId, KMsvRootIndexEntryId, KUidMsvServiceEntry, visibleFolder));
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolder);
	MY_ASSERT_TRUE(EntryExists(KMsvLocalServiceIndexEntryId));
	
	// 3. Creating normal folder under root.
	TRAP(err, CreateEntryL(KMsvGlobalInBoxIndexEntryId, KMsvRootIndexEntryId, KUidMsvFolderEntry, visibleFolder, ETrue));
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolder);
	MY_ASSERT_TRUE(EntryExists(KMsvGlobalInBoxIndexEntryId));

	// 4. Creating 6 messages, attachments and folders under Inbox.
	visibleFolder = KMsvGlobalInBoxIndexEntryId;
	for(int index=0; index<19; index++)
		{
		switch(index%3)
			{
			case 0: 			
				TRAP(err, CreateEntryL(KFirstFreeEntryId+index, KMsvGlobalInBoxIndexEntryId, KUidMsvMessageEntry, visibleFolder));
				break;
			case 1:
				TRAP(err, CreateEntryL(KFirstFreeEntryId+index, KMsvGlobalInBoxIndexEntryId, KUidMsvAttachmentEntry, visibleFolder));
				break;
			case 2:
				TRAP(err, CreateEntryL(KFirstFreeEntryId+index, KMsvGlobalInBoxIndexEntryId, KUidMsvFolderEntry, visibleFolder, ETrue));
			}	
		
		//Avoid using MY_ASSERT_EQUALS as it will clutter the log file.
		ASSERT_EQUALS(KErrNone, err);
		ASSERT_EQUALS(visibleFolder, KMsvGlobalInBoxIndexEntryId);
		}

	// ENTRY ID 19 DOES NOT EXISTS.
	
	// 5. Creating 10 message entries under KFirstFreeEntryId+5 (Folder Entry).
	visibleFolder = KFirstFreeEntryId+5;
	for(int index=20; index<30; index++)
		{
		TRAP(err, CreateEntryL(KFirstFreeEntryId+index, KFirstFreeEntryId+5, KUidMsvMessageEntry, visibleFolder));
		//Avoid using MY_ASSERT_EQUALS as it will clutter the log file.
		ASSERT_EQUALS(KErrNone, err);
		ASSERT_EQUALS(visibleFolder, KFirstFreeEntryId+5);
		}
	
	// 6. Actual test performed here
	MY_ASSERT_TRUE(EntryExists(KFirstFreeEntryId+9));
	TRAP(err, iMsvDbAdaptor->DeleteEntryL(KFirstFreeEntryId+9));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_TRUE(!EntryExists(KFirstFreeEntryId+9));
	
	MY_ASSERT_TRUE(EntryExists(KFirstFreeEntryId+10));
	TRAP(err, iMsvDbAdaptor->DeleteEntryL(KFirstFreeEntryId+10));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_TRUE(!EntryExists(KFirstFreeEntryId+10));
	
	MY_ASSERT_TRUE(EntryExists(KFirstFreeEntryId+11));
	TRAP(err, iMsvDbAdaptor->DeleteEntryL(KFirstFreeEntryId+11));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_TRUE(!EntryExists(KFirstFreeEntryId+11));
	
	for(int index=20; index<30; index++)
		{
		ASSERT_TRUE(EntryExists(KFirstFreeEntryId+index));
		TRAP(err, iMsvDbAdaptor->DeleteEntryL(KFirstFreeEntryId+index));
		ASSERT_EQUALS(err, KErrNone);
		ASSERT_TRUE(!EntryExists(KFirstFreeEntryId+index));
		}
	
	MY_ASSERT_TRUE(EntryExists(KFirstFreeEntryId+5));
	TRAP(err, iMsvDbAdaptor->DeleteEntryL(KFirstFreeEntryId+5));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_TRUE(!EntryExists(KFirstFreeEntryId+5));

	// Deleting entry whose children are still present.
	// This should not throw any error.
	MY_ASSERT_TRUE(EntryExists(KMsvGlobalInBoxIndexEntryId));
	TRAP(err, iMsvDbAdaptor->DeleteEntryL(KMsvGlobalInBoxIndexEntryId));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_TRUE(!EntryExists(KMsvGlobalInBoxIndexEntryId));

	MY_ASSERT_TRUE(EntryExists(KMsvLocalServiceIndexEntryId));
	TRAP(err, iMsvDbAdaptor->DeleteEntryL(KMsvLocalServiceIndexEntryId));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_TRUE(!EntryExists(KMsvLocalServiceIndexEntryId));
	
	CMsvCacheEntry *entry = NULL;
	TRAP(err, iMsvDbAdaptor->GetEntryL(KMsvLocalServiceIndexEntryId, entry, visibleFolder));
	MY_ASSERT_EQUALS(err, KErrNotFound);
	
	// Deleting non-existent entry.
	MY_ASSERT_TRUE(!EntryExists(KFirstFreeEntryId+19));
	TRAP(err, iMsvDbAdaptor->DeleteEntryL(KFirstFreeEntryId+19));
	MY_ASSERT_EQUALS(err, KErrNotFound);	
	
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	// 7. Create and attach a second database. Create entries in it and test.
	// 7.0 Cleanup: deleting the DB if it was created in an earlier test.
	TParse parse;
	TPtrC drive = TDriveUnit(EDriveE).Name();
	parse.Set(KTestDbFileName, &drive, NULL);
	TFileName dbName = parse.FullName();
	RSqlDatabase::Delete(dbName);
	
	// 7.1 Create DB with driveId 2. System drive had driveId 1.
	TMsvId maxId = 0;
	TInt driveId = 2;
	TRAP(err, CMsvDBAdapter::CreateDBL(TDriveNumber(EDriveE)));
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, iMsvDbAdaptor->AttachDBL(TDriveUnit(EDriveE), driveId, maxId));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	// 7.2 Create entries in the first database.
	visibleFolder = KMsvRootIndexEntryId;
	TRAP(err, CreateEntryL(KFirstFreeEntryId+150, KMsvRootIndexEntryId, KUidMsvAttachmentEntry, visibleFolder));
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolder);
	TRAP(err, CreateEntryL(KFirstFreeEntryId+151, KMsvRootIndexEntryId, KUidMsvAttachmentEntry, visibleFolder));
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolder);
	MY_ASSERT_TRUE(EntryExists(KFirstFreeEntryId+150));
	MY_ASSERT_TRUE(EntryExists(KFirstFreeEntryId+151));
	
	TRAP(err, iMsvDbAdaptor->DeleteEntryL(KFirstFreeEntryId+150));
	MY_ASSERT_EQUALS(err, KErrNone);
	//Attempting to delete again will result in KErrNotFound.
	TRAP(err, iMsvDbAdaptor->DeleteEntryL(KFirstFreeEntryId+150));
	MY_ASSERT_EQUALS(err, KErrNotFound);
	
	MY_ASSERT_TRUE(EntryExists(KFirstFreeEntryId+151));
	
	// 7.3 Create entries in the attached database.
	visibleFolder = KMsvRootIndexEntryId;
	TRAP(err, CreateEntryL((driveId<<28)|KFirstFreeEntryId+151, KMsvRootIndexEntryId, KUidMsvAttachmentEntry, visibleFolder));
	MY_ASSERT_EQUALS(KErrNone, err);
	TRAP(err, CreateEntryL((driveId<<28)|KFirstFreeEntryId+152, KMsvRootIndexEntryId, KUidMsvAttachmentEntry, visibleFolder));
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_TRUE(EntryExists((driveId<<28)|KFirstFreeEntryId+151));
	MY_ASSERT_TRUE(EntryExists((driveId<<28)|KFirstFreeEntryId+152));
	
	//Notice that the above Id also exists in the first database, and that
	//there is no conflict.
	MY_ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolder);
	MY_ASSERT_TRUE(EntryExists((driveId<<28)|KFirstFreeEntryId+151));
	TRAP(err, iMsvDbAdaptor->DeleteEntryL((driveId<<28)|KFirstFreeEntryId+151));
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, iMsvDbAdaptor->DeleteEntryL((driveId<<28)|KFirstFreeEntryId+151));
	MY_ASSERT_EQUALS(err, KErrNotFound);
	MY_ASSERT_TRUE(EntryExists((driveId<<28)|KFirstFreeEntryId+152));
#endif	
	}



void CTestDbAdapter::TestUpdateEntry()
	{
	_LIT(KFunction, "TestUpdateEntry");
	INFO_PRINTF1(KFunction);
	
	TInt err;
	TMsvId visibleFolder;
	TInt driveId = 1;
	// Creating entries.
	visibleFolder = KMsvRootIndexEntryId;

#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	// (i) Checking for root entry.
	MY_ASSERT_TRUE(EntryExists(KMsvRootIndexEntryId));
#else
	TRAP(err, CreateEntryL(KMsvRootIndexEntryId, KErrNotFound, KUidMsvRootEntry, visibleFolder, ETrue));
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolder);
#endif
	
	// (ii) Creating normal folder under root.
	TRAP(err, CreateEntryL(KMsvGlobalInBoxIndexEntryId, KMsvRootIndexEntryId, KUidMsvFolderEntry, visibleFolder, ETrue));
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolder);

	// (iii) Creating message entry under root.
	TRAP(err, CreateEntryL(KFirstFreeEntryId, KMsvRootIndexEntryId, KUidMsvMessageEntry, visibleFolder));
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolder);
	
	// 1. Test 1. Change all parts of TMsvEntry
	CMsvCacheEntry *cacheEntry = NULL; 
	TRAP(err, iMsvDbAdaptor->GetEntryL(KFirstFreeEntryId, cacheEntry, visibleFolder));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(visibleFolder, KMsvRootIndexEntryId);
	TMsvEntry *entry = &(cacheEntry->Entry());
	MY_ASSERT_FALSE(entry->Visible());
	MY_ASSERT_TRUE(entry->Unread());
	
	TMsvId inboxId = KMsvGlobalInBoxIndexEntryId;
	entry->SetParent(inboxId);
	entry->iDate.UniversalTime();
	entry->iType = KUidMsvFolderEntry;
	entry->SetVisible(ETrue);
	entry->SetUnread(EFalse);
	entry->iServiceId = KMsvRootIndexEntryId+1000;		// Some Random Id.
	entry->iRelatedId = KMsvRootIndexEntryId+2000;		// Some Random Id.
	entry->iMtm = KUidMsvFolderEntry;		
	entry->iSize = 128;
	entry->iError = -1;		
	entry->iBioType = 128;		
	entry->iMtmData1 = 128;		
	entry->iMtmData2 = 128;		
	entry->iMtmData3 = 128;	
	
	visibleFolder = KMsvGlobalInBoxIndexEntryId;
	TRAP(err, iMsvDbAdaptor->UpdateEntryL(cacheEntry->Entry(), visibleFolder));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(visibleFolder, KMsvGlobalInBoxIndexEntryId);

	CMsvEntryFreePool::Instance()->ReleaseEntryL(cacheEntry);
	visibleFolder = NULL;
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	TRAP(err, iMsvDbAdaptor->GetEntryL((driveId<<28)|KFirstFreeEntryId, cacheEntry, visibleFolder));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(visibleFolder, KMsvGlobalInBoxIndexEntryId);
	MY_ASSERT_EQUALS(entry->iId, (driveId<<28)|KFirstFreeEntryId);
	MY_ASSERT_EQUALS(entry->iParentId, KMsvGlobalInBoxIndexEntryId);
	MY_ASSERT_EQUALS(entry->iServiceId, (driveId<<28)|KMsvRootIndexEntryId+1000);
	MY_ASSERT_EQUALS(entry->iRelatedId, (driveId<<28)|KMsvRootIndexEntryId+2000);
#else
	TRAP(err, iMsvDbAdaptor->GetEntryL(KFirstFreeEntryId, cacheEntry, visibleFolder));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(visibleFolder, KMsvGlobalInBoxIndexEntryId);
	MY_ASSERT_EQUALS(entry->iId, KFirstFreeEntryId);
	MY_ASSERT_EQUALS(entry->iParentId, KMsvGlobalInBoxIndexEntryId);
	MY_ASSERT_EQUALS(entry->iServiceId, KMsvRootIndexEntryId+1000);
	MY_ASSERT_EQUALS(entry->iRelatedId, KMsvRootIndexEntryId+2000);
#endif
	MY_ASSERT_EQUALS(entry->iType, KUidMsvFolderEntry);
	MY_ASSERT_TRUE(entry->Visible());
	MY_ASSERT_TRUE(!entry->Unread());
	MY_ASSERT_EQUALS(entry->iMtm, KUidMsvFolderEntry);
	MY_ASSERT_EQUALS(entry->iSize, (long) 128);
	MY_ASSERT_EQUALS(entry->iError, (long) -1);
	MY_ASSERT_EQUALS(entry->iBioType, (long) 128);
	MY_ASSERT_EQUALS(entry->iMtmData1, (long) 128);
	MY_ASSERT_EQUALS(entry->iMtmData2, (long) 128);
	MY_ASSERT_EQUALS(entry->iMtmData3, (long) 128);
	
	
	// 2. Test 2. Change all but parent of the entry
	entry->iDate.UniversalTime();
	entry->iType = KUidMsvFolderEntry;
	entry->SetVisible(EFalse);
	entry->SetUnread(ETrue);
	entry->iServiceId = KMsvRootIndexEntryId+4000;		// Some Random Id.
	entry->iRelatedId = KMsvRootIndexEntryId+5000;		// Some Random Id.
	entry->iMtm = KUidMsvFolderEntry;		
	entry->iSize = 120;
	entry->iError = -2;		
	entry->iBioType = 120;		
	entry->iMtmData1 = 120;		
	entry->iMtmData2 = 120;		
	entry->iMtmData3 = 120;	
	
	visibleFolder = KMsvGlobalInBoxIndexEntryId;
	TRAP(err, iMsvDbAdaptor->UpdateEntryL(cacheEntry->Entry(), visibleFolder)); //parent not changed
	MY_ASSERT_EQUALS(err, KErrNone);
	
	CMsvEntryFreePool::Instance()->ReleaseEntryL(cacheEntry);
	visibleFolder = NULL;
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	TRAP(err, iMsvDbAdaptor->GetEntryL((driveId<<28)|KFirstFreeEntryId, cacheEntry, visibleFolder));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entry->iId, (driveId<<28)|KFirstFreeEntryId);
	MY_ASSERT_EQUALS(entry->iParentId, KMsvGlobalInBoxIndexEntryId);
	MY_ASSERT_EQUALS(entry->iServiceId, (driveId<<28)|KMsvRootIndexEntryId+4000);
	MY_ASSERT_EQUALS(entry->iRelatedId, (driveId<<28)|KMsvRootIndexEntryId+5000);
#else
	TRAP(err, iMsvDbAdaptor->GetEntryL(KFirstFreeEntryId, cacheEntry, visibleFolder));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entry->iId, KFirstFreeEntryId);
	MY_ASSERT_EQUALS(entry->iParentId, KMsvGlobalInBoxIndexEntryId);
	MY_ASSERT_EQUALS(entry->iServiceId, KMsvRootIndexEntryId+4000);
	MY_ASSERT_EQUALS(entry->iRelatedId, KMsvRootIndexEntryId+5000);
#endif
	MY_ASSERT_EQUALS(entry->iType, KUidMsvFolderEntry);
	MY_ASSERT_FALSE(entry->Visible());
	MY_ASSERT_TRUE(entry->Unread());
	MY_ASSERT_EQUALS(entry->iMtm, KUidMsvFolderEntry);
	MY_ASSERT_EQUALS(entry->iSize, (long) 120);
	MY_ASSERT_EQUALS(entry->iError, (long) -2);
	MY_ASSERT_EQUALS(entry->iBioType, (long) 120);
	MY_ASSERT_EQUALS(entry->iMtmData1, (long) 120);
	MY_ASSERT_EQUALS(entry->iMtmData2, (long) 120);
	MY_ASSERT_EQUALS(entry->iMtmData3, (long) 120);
	
	CMsvEntryFreePool::Instance()->ReleaseEntryL(cacheEntry);
	
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	// 3. Test update on an entry in an attached database.
	// 3.0 Cleanup: deleting the DB if it was created in an earlier test.
	TParse parse;
	TPtrC drive = TDriveUnit(EDriveE).Name();
	parse.Set(KTestDbFileName, &drive, NULL);
	TFileName dbName = parse.FullName();
	RSqlDatabase::Delete(dbName);
	
	// 3.1 Create DB with driveId 2. System drive had driveId 1.
	TMsvId maxId = 0;
	driveId = 2;
	TRAP(err, CMsvDBAdapter::CreateDBL(TDriveNumber(EDriveE)));
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, iMsvDbAdaptor->AttachDBL(TDriveUnit(EDriveE), driveId, maxId));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	// 3.2 Create an entry in the attached database.
	visibleFolder = KMsvRootIndexEntryId;
	TRAP(err, CreateEntryL((driveId<<28)|KFirstFreeEntryId, KMsvRootIndexEntryId, KUidMsvAttachmentEntry, visibleFolder));
	MY_ASSERT_EQUALS(KErrNone, err);
	//Notice that the above Id also exists in the first database, and that
	//there is no conflict.
	MY_ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolder);
	MY_ASSERT_TRUE(EntryExists((driveId<<28)|KFirstFreeEntryId));
	
	// 3.3 Change all parts of the entry.
	cacheEntry = NULL; 
	TRAP(err, iMsvDbAdaptor->GetEntryL((driveId<<28)|KFirstFreeEntryId, cacheEntry, visibleFolder));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(visibleFolder, KMsvRootIndexEntryId);
	entry = &(cacheEntry->Entry());
	MY_ASSERT_FALSE(entry->Visible());
	MY_ASSERT_TRUE(entry->Unread());
	
	entry->SetParent((driveId<<28)|KMsvGlobalInBoxIndexEntryId);
	entry->iDate.UniversalTime();
	entry->iType = KUidMsvMessageEntry;
	entry->SetVisible(ETrue);
	entry->SetUnread(EFalse);
	entry->iServiceId = KMsvRootIndexEntryId+1000;		// Some Random Id.
	entry->iRelatedId = KMsvRootIndexEntryId+2000;		// Some Random Id.
	entry->iMtm = KUidMsvFolderEntry;		
	entry->iSize = 128;
	entry->iError = -1;		
	entry->iBioType = 128;		
	entry->iMtmData1 = 128;		
	entry->iMtmData2 = 128;		
	entry->iMtmData3 = 128;	
	
	visibleFolder = KMsvGlobalInBoxIndexEntryId;
	TRAP(err, iMsvDbAdaptor->UpdateEntryL(cacheEntry->Entry(), visibleFolder));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(visibleFolder, KMsvGlobalInBoxIndexEntryId);

	CMsvEntryFreePool::Instance()->ReleaseEntryL(cacheEntry);
	visibleFolder = NULL;
	TRAP(err, iMsvDbAdaptor->GetEntryL((driveId<<28)|KFirstFreeEntryId, cacheEntry, visibleFolder));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(visibleFolder, KMsvGlobalInBoxIndexEntryId);
	MY_ASSERT_EQUALS(entry->iId, (driveId<<28)|KFirstFreeEntryId);
	MY_ASSERT_EQUALS(entry->iParentId, KMsvGlobalInBoxIndexEntryId);
	MY_ASSERT_EQUALS(entry->iType, KUidMsvMessageEntry);
	MY_ASSERT_TRUE(entry->Visible());
	MY_ASSERT_TRUE(!entry->Unread());
	MY_ASSERT_EQUALS(entry->iServiceId, (driveId<<28)|KMsvRootIndexEntryId+1000);
	MY_ASSERT_EQUALS(entry->iRelatedId, (driveId<<28)|KMsvRootIndexEntryId+2000);
	MY_ASSERT_EQUALS(entry->iMtm, KUidMsvFolderEntry);
	MY_ASSERT_EQUALS(entry->iSize, (long) 128);
	MY_ASSERT_EQUALS(entry->iError, (long) -1);
	MY_ASSERT_EQUALS(entry->iBioType, (long) 128);
	MY_ASSERT_EQUALS(entry->iMtmData1, (long) 128);
	MY_ASSERT_EQUALS(entry->iMtmData2, (long) 128);
	MY_ASSERT_EQUALS(entry->iMtmData3, (long) 128);
	
	
	
	//3.4 Change all but the parent of the entry.
	entry->iDate.UniversalTime();
	entry->iType = KUidMsvFolderEntry;
	entry->SetVisible(EFalse);
	entry->SetUnread(ETrue);
	entry->iServiceId = KMsvRootIndexEntryId+4000;		// Some Random Id.
	entry->iRelatedId = KMsvRootIndexEntryId+5000;		// Some Random Id.
	entry->iMtm = KUidMsvFolderEntry;		
	entry->iSize = 120;
	entry->iError = -2;		
	entry->iBioType = 120;		
	entry->iMtmData1 = 120;		
	entry->iMtmData2 = 120;		
	entry->iMtmData3 = 120;	
	
	visibleFolder = KMsvGlobalInBoxIndexEntryId;
	TRAP(err, iMsvDbAdaptor->UpdateEntryL(cacheEntry->Entry(), visibleFolder)); //parent not changed
	MY_ASSERT_EQUALS(err, KErrNone);
	
	CMsvEntryFreePool::Instance()->ReleaseEntryL(cacheEntry);
	visibleFolder = NULL;
	TRAP(err, iMsvDbAdaptor->GetEntryL((driveId<<28)|KFirstFreeEntryId, cacheEntry, visibleFolder));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(entry->iId, (driveId<<28)|KFirstFreeEntryId);
	MY_ASSERT_EQUALS(entry->iParentId, KMsvGlobalInBoxIndexEntryId);
//	MY_ASSERT_EQUALS(visibleFolder, (TMsvId)0); //visible folder Id is unchanged
	MY_ASSERT_EQUALS(entry->iType, KUidMsvFolderEntry);
	MY_ASSERT_FALSE(entry->Visible());
	MY_ASSERT_TRUE(entry->Unread());
	MY_ASSERT_EQUALS(entry->iServiceId, (driveId<<28)|KMsvRootIndexEntryId+4000);
	MY_ASSERT_EQUALS(entry->iRelatedId, (driveId<<28)|KMsvRootIndexEntryId+5000);
	MY_ASSERT_EQUALS(entry->iMtm, KUidMsvFolderEntry);
	MY_ASSERT_EQUALS(entry->iSize, (long) 120);
	MY_ASSERT_EQUALS(entry->iError, (long) -2);
	MY_ASSERT_EQUALS(entry->iBioType, (long) 120);
	MY_ASSERT_EQUALS(entry->iMtmData1, (long) 120);
	MY_ASSERT_EQUALS(entry->iMtmData2, (long) 120);
	MY_ASSERT_EQUALS(entry->iMtmData3, (long) 120);
	
	CMsvEntryFreePool::Instance()->ReleaseEntryL(cacheEntry);
#endif	
	}



void CTestDbAdapter::TestGetEntry()
	{
	_LIT(KFunction, "TestGetEntry");
	INFO_PRINTF1(KFunction);
	
	TInt err;
	TMsvId visibleFolder;
	visibleFolder = KMsvRootIndexEntryId;
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	// 1. Checking for root entry.
#else
	// 1. Creating root entry.
	TRAP(err, CreateEntryL(KMsvRootIndexEntryId, KErrNotFound, KUidMsvRootEntry, visibleFolder, ETrue));
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolder);
#endif
	MY_ASSERT_TRUE(EntryExists(KMsvRootIndexEntryId));
	
	// 2. Creating service folder under root.
	TRAP(err, CreateEntryL(KMsvLocalServiceIndexEntryId, KMsvRootIndexEntryId, KUidMsvServiceEntry, visibleFolder));
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolder);
	MY_ASSERT_TRUE(EntryExists(KMsvLocalServiceIndexEntryId));
	
	// 3. Creating normal folder under root.
	TRAP(err, CreateEntryL(KMsvGlobalInBoxIndexEntryId, KMsvRootIndexEntryId, KUidMsvFolderEntry, visibleFolder, ETrue));
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolder);
	MY_ASSERT_TRUE(EntryExists(KMsvGlobalInBoxIndexEntryId));

	// 4. Creating Message entry under root.
	TRAP(err, CreateEntryL(KFirstFreeEntryId+1, KMsvRootIndexEntryId, KUidMsvMessageEntry, visibleFolder));
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolder);
	MY_ASSERT_TRUE(EntryExists(KFirstFreeEntryId+1));

	// 5. Creating attachment entry under root.
	TRAP(err, CreateEntryL(KFirstFreeEntryId+2, KMsvRootIndexEntryId, KUidMsvAttachmentEntry, visibleFolder));
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolder);
	MY_ASSERT_TRUE(EntryExists(KFirstFreeEntryId+2));
	
	// 6. Creating 100 message entries under Inbox.
	visibleFolder = KMsvGlobalInBoxIndexEntryId;
	for(int index=3; index<103; index++)
		{
		TRAP(err, CreateEntryL(KFirstFreeEntryId+index, KMsvGlobalInBoxIndexEntryId, KUidMsvMessageEntry, visibleFolder));
		//Avoid using MY_ASSERT_EQUALS as it will clutter the log file.
		ASSERT_EQUALS(KErrNone, err);
		ASSERT_EQUALS(KMsvGlobalInBoxIndexEntryId, visibleFolder);
		}

	// 7. Creating 100 message entries under Inbox.
	visibleFolder = KMsvRootIndexEntryId;
	for(int index=103; index<203; index++)
		{
		TRAP(err, CreateEntryL(KFirstFreeEntryId+index, KMsvLocalServiceIndexEntryId, KUidMsvMessageEntry, visibleFolder));
		//Avoid using MY_ASSERT_EQUALS as it will clutter the log file.
		ASSERT_EQUALS(KErrNone, err);
		ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolder);
		}
	
	CMsvCacheEntry *cacheEntry = NULL;
	TRAP(err, iMsvDbAdaptor->GetEntryL(KMsvRootIndexEntryId, cacheEntry, visibleFolder));
	TMsvEntry *entry = &(cacheEntry->Entry());
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(visibleFolder, KMsvRootIndexEntryId);
	MY_ASSERT_EQUALS(entry->iId, KMsvRootIndexEntryId);
	MY_ASSERT_EQUALS(entry->iType, KUidMsvRootEntry);
	MY_ASSERT_EQUALS(entry->iMtm, KUidMsvRootEntry);
	
	_LIT(KName, "Hello World");
	HBufC* data = NULL;
	TRAP_IGNORE(data = HBufC::NewL(15)); 
	*data = KName;
	//MY_ASSERT_TRUE(entry->iDescription==data->Des());

	delete data;
	data = NULL;
	CMsvEntryFreePool::Instance()->ReleaseEntryL(cacheEntry);
	
	//TRAP(err, iMsvDbAdaptor->GetEntryL(KMsvLocalServiceIndexEntryId, entry, visibleFolder));
	TRAP(err, iMsvDbAdaptor->GetEntryL(KMsvLocalServiceIndexEntryId, cacheEntry, visibleFolder));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(visibleFolder, KMsvRootIndexEntryId);
	MY_ASSERT_EQUALS(cacheEntry->Entry().iType, KUidMsvServiceEntry);
	CMsvEntryFreePool::Instance()->ReleaseEntryL(cacheEntry);
	
	TRAP(err, iMsvDbAdaptor->GetEntryL(KMsvGlobalInBoxIndexEntryId, cacheEntry, visibleFolder));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(visibleFolder, KMsvRootIndexEntryId);
	MY_ASSERT_EQUALS(cacheEntry->Entry().iType, KUidMsvFolderEntry);
	CMsvEntryFreePool::Instance()->ReleaseEntryL(cacheEntry);
	
	TRAP(err, iMsvDbAdaptor->GetEntryL(KFirstFreeEntryId+1, cacheEntry, visibleFolder));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(visibleFolder, KMsvRootIndexEntryId);
	MY_ASSERT_EQUALS(cacheEntry->Entry().iType, KUidMsvMessageEntry);
	CMsvEntryFreePool::Instance()->ReleaseEntryL(cacheEntry);
	
	TRAP(err, iMsvDbAdaptor->GetEntryL(KFirstFreeEntryId+2, cacheEntry, visibleFolder));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(visibleFolder, KMsvRootIndexEntryId);
	MY_ASSERT_EQUALS(cacheEntry->Entry().iType, KUidMsvAttachmentEntry);
	CMsvEntryFreePool::Instance()->ReleaseEntryL(cacheEntry);
	
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	TInt driveId = (1<<28);
#else
	TInt driveId = 0x0;
#endif
	TRAP(err, iMsvDbAdaptor->GetEntryL(KFirstFreeEntryId+5, cacheEntry, visibleFolder));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(visibleFolder, KMsvGlobalInBoxIndexEntryId);
	MY_ASSERT_EQUALS(cacheEntry->Entry().iType, KUidMsvMessageEntry);
	CMsvEntryFreePool::Instance()->ReleaseEntryL(cacheEntry);
	
	TRAP(err, iMsvDbAdaptor->GetEntryL(KFirstFreeEntryId+100, cacheEntry, visibleFolder));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(visibleFolder, KMsvGlobalInBoxIndexEntryId);
	MY_ASSERT_EQUALS(cacheEntry->Entry().iType, KUidMsvMessageEntry);
	CMsvEntryFreePool::Instance()->ReleaseEntryL(cacheEntry);
	
	TRAP(err, iMsvDbAdaptor->GetEntryL(KFirstFreeEntryId+102, cacheEntry, visibleFolder));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(visibleFolder, KMsvGlobalInBoxIndexEntryId);
	MY_ASSERT_EQUALS(cacheEntry->Entry().iType, KUidMsvMessageEntry);
	CMsvEntryFreePool::Instance()->ReleaseEntryL(cacheEntry);
	
	TRAP(err, iMsvDbAdaptor->GetEntryL(KFirstFreeEntryId+103, cacheEntry, visibleFolder));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(visibleFolder, KMsvRootIndexEntryId);
	MY_ASSERT_EQUALS(cacheEntry->Entry().iType, KUidMsvMessageEntry);
	CMsvEntryFreePool::Instance()->ReleaseEntryL(cacheEntry);
	
	TRAP(err, iMsvDbAdaptor->GetEntryL(KFirstFreeEntryId+201, cacheEntry, visibleFolder));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(visibleFolder, KMsvRootIndexEntryId);
	MY_ASSERT_EQUALS(cacheEntry->Entry().iType, KUidMsvMessageEntry);
	CMsvEntryFreePool::Instance()->ReleaseEntryL(cacheEntry);
	
	TRAP(err, iMsvDbAdaptor->GetEntryL(KFirstFreeEntryId+202, cacheEntry, visibleFolder));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(visibleFolder, KMsvRootIndexEntryId);
	MY_ASSERT_EQUALS(cacheEntry->Entry().iType, KUidMsvMessageEntry);
	CMsvEntryFreePool::Instance()->ReleaseEntryL(cacheEntry);
	
	TRAP(err, iMsvDbAdaptor->GetEntryL(KFirstFreeEntryId+203, cacheEntry, visibleFolder));
	MY_ASSERT_EQUALS(err, KErrNotFound);
	
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	// 8. Testing in an attached database.
	// 8.0 Cleanup: deleting the DB if it was created in an earlier test.
	TParse parse;
	TPtrC drive = TDriveUnit(EDriveE).Name();
	parse.Set(KTestDbFileName, &drive, NULL);
	TFileName dbName = parse.FullName();
	RSqlDatabase::Delete(dbName);
	
	// 8.1 Create DB with driveId 1. System drive had driveId 0.
	TMsvId maxId = 0;
	driveId = 2;
	TRAP(err, CMsvDBAdapter::CreateDBL(TDriveNumber(EDriveE)));
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, iMsvDbAdaptor->AttachDBL(TDriveUnit(EDriveE), driveId, maxId));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	// 8.2 Create an entry in the attached database.
	visibleFolder = KMsvRootIndexEntryId;
	//Notice that the Id's also exists in the first database, and that
	//there is no conflict.
	//Create Inbox.
	TRAP(err, CreateEntryL((driveId<<28)|KMsvGlobalInBoxIndexEntryId, KMsvRootIndexEntryId, KUidMsvFolderEntry, visibleFolder, ETrue));
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolder);
	MY_ASSERT_TRUE(EntryExists((driveId<<28)|KMsvGlobalInBoxIndexEntryId));
	//Create 100 children of Inbox.
	visibleFolder = KMsvGlobalInBoxIndexEntryId;
	for(int index=3; index<103; index++)
		{
		TRAP(err, CreateEntryL((driveId<<28)|KFirstFreeEntryId+index, (driveId<<28)|KMsvGlobalInBoxIndexEntryId, KUidMsvMessageEntry, visibleFolder));
		//Avoid using MY_ASSERT_EQUALS as it will clutter the log file.
		ASSERT_EQUALS(KErrNone, err);
		ASSERT_EQUALS(KMsvGlobalInBoxIndexEntryId, visibleFolder);
		}
	//Test GetEntry on the children.
	TRAP(err, iMsvDbAdaptor->GetEntryL((driveId<<28)|KFirstFreeEntryId+3, cacheEntry, visibleFolder));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(visibleFolder, KMsvGlobalInBoxIndexEntryId);
	MY_ASSERT_EQUALS(cacheEntry->Entry().iType, KUidMsvMessageEntry);
	CMsvEntryFreePool::Instance()->ReleaseEntryL(cacheEntry);
	
	TRAP(err, iMsvDbAdaptor->GetEntryL((driveId<<28)|KFirstFreeEntryId+50, cacheEntry, visibleFolder));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(visibleFolder, KMsvGlobalInBoxIndexEntryId);
	MY_ASSERT_EQUALS(cacheEntry->Entry().iType, KUidMsvMessageEntry);
	CMsvEntryFreePool::Instance()->ReleaseEntryL(cacheEntry);
	
	TRAP(err, iMsvDbAdaptor->GetEntryL((driveId<<28)|KFirstFreeEntryId+102, cacheEntry, visibleFolder));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(visibleFolder, KMsvGlobalInBoxIndexEntryId);
	MY_ASSERT_EQUALS(cacheEntry->Entry().iType, KUidMsvMessageEntry);
	CMsvEntryFreePool::Instance()->ReleaseEntryL(cacheEntry);
	
	//A negative test on a non-existent entry.
	TRAP(err, iMsvDbAdaptor->GetEntryL((driveId<<28)|KFirstFreeEntryId+103, cacheEntry, visibleFolder));
	MY_ASSERT_EQUALS(err, KErrNotFound);
	
	//GetEntry from the first database.
	driveId = 1;
	TRAP(err, iMsvDbAdaptor->GetEntryL(KFirstFreeEntryId+102, cacheEntry, visibleFolder));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(visibleFolder, KMsvGlobalInBoxIndexEntryId);
	MY_ASSERT_EQUALS(cacheEntry->Entry().iType, KUidMsvMessageEntry);
	CMsvEntryFreePool::Instance()->ReleaseEntryL(cacheEntry);
	
	TRAP(err, iMsvDbAdaptor->GetEntryL(KFirstFreeEntryId+200, cacheEntry, visibleFolder));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(visibleFolder, KMsvRootIndexEntryId);
	MY_ASSERT_EQUALS(cacheEntry->Entry().iType, KUidMsvMessageEntry);
	CMsvEntryFreePool::Instance()->ReleaseEntryL(cacheEntry);
#endif	
	}



void CTestDbAdapter::ResetAndDestroy(RPointerArray<CMsvCacheEntry>& childArray)
	{
	for(int index=0; index<childArray.Count(); index++)
		{
		CMsvEntryFreePool::Instance()->ReleaseEntryL(childArray[index]);
		}
	childArray.Reset();
	}



void CTestDbAdapter::TestDeleteBulkEntry()
{
	_LIT(KFunction, "TestDeleteBulkEntry");
	INFO_PRINTF1(KFunction);

	// 0. Create Test Entries
	TInt err;
	TMsvId visibleFolder;
	visibleFolder = KMsvRootIndexEntryId;
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	// (i) Checking for root entry.
#else
	// (i) Creating root entry.
	TRAP(err, CreateEntryL(KMsvRootIndexEntryId, KErrNotFound, KUidMsvRootEntry, visibleFolder, ETrue));
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolder);
#endif
	MY_ASSERT_TRUE(EntryExists(KMsvRootIndexEntryId));
	
	// (ii) Creating normal folder under root.
	TRAP(err, CreateEntryL(KMsvGlobalInBoxIndexEntryId, KMsvRootIndexEntryId, KUidMsvFolderEntry, visibleFolder, ETrue));
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolder);
	MY_ASSERT_TRUE(EntryExists(KMsvGlobalInBoxIndexEntryId));

	CMsvEntrySelection* selection = NULL;
	TRAP_IGNORE(selection = new(ELeave) CMsvEntrySelection());
	
	// (iii) Creating 100 message entries under root.
	// Root will have 101 entries under it.
	for(int index=0; index<100; index++)
		{
		TRAP(err, CreateEntryL(KFirstFreeEntryId+index, KMsvRootIndexEntryId, KUidMsvMessageEntry, visibleFolder));
		//Avoid using MY_ASSERT_EQUALS as it will clutter the log file.
		ASSERT_EQUALS(KErrNone, err);
		ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolder);
		
		TRAP(err, selection->AppendL(KFirstFreeEntryId+index));
		ASSERT_EQUALS(KErrNone, err);
		}

	// Performing GetChildren() on root.
	RPointerArray<CMsvCacheEntry> childArray;
	TRAP(err, iMsvDbAdaptor->GetChildrenL(KMsvRootIndexEntryId, childArray));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(childArray.Count(), 101);
	ResetAndDestroy(childArray);
	
	// 1. Deleting 100 children of root.
	TRAP(err, iMsvDbAdaptor->DeleteEntryL(*selection));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(100, selection->Count());
	selection->Reset();
	
	TRAP(err, iMsvDbAdaptor->GetChildrenL(KMsvRootIndexEntryId, childArray));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(childArray.Count(), 1);
	ResetAndDestroy(childArray);
	
	// 2. Deleting through an empty list.
	TRAP(err, iMsvDbAdaptor->DeleteEntryL(*selection));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	// 3. List has only one element.
	TRAP_IGNORE(selection->AppendL(KMsvGlobalInBoxIndexEntryId));
	TRAP(err, iMsvDbAdaptor->DeleteEntryL(*selection));
	MY_ASSERT_EQUALS(err, KErrNone);
	selection->Reset();
	MY_ASSERT_TRUE(!EntryExists(KMsvGlobalInBoxIndexEntryId));

	// 4. Deleting through a list having same ids more than once.
	// Creating 100 message entries under root.
	for(int index=0; index<100; index++)
		{
		TRAP(err, CreateEntryL(KFirstFreeEntryId+index, KMsvRootIndexEntryId, KUidMsvMessageEntry, visibleFolder));
		//Avoid using MY_ASSERT_EQUALS as it will clutter the log file.
		ASSERT_EQUALS(KErrNone, err);
		ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolder);
		
		// selection list has last 40 ids.
		if(index >= 60)
			{
			TRAP(err, selection->AppendL(KFirstFreeEntryId+index));
			ASSERT_EQUALS(KErrNone, err);
			// Introduce redundency: Add already added entries.
			if(index%5)
				{
				TRAP(err, selection->AppendL(KFirstFreeEntryId+index));
				ASSERT_EQUALS(KErrNone, err);				
				}
			}
		}
	
	TRAP(err, iMsvDbAdaptor->GetChildrenL(KMsvRootIndexEntryId, childArray));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(childArray.Count(), 100);
	ResetAndDestroy(childArray);

	// Deleting last 40 entries.
	TRAP(err, iMsvDbAdaptor->DeleteEntryL(*selection));
	MY_ASSERT_EQUALS(err, KErrNone);
	selection->Reset();
	
	TRAP(err, iMsvDbAdaptor->GetChildrenL(KMsvRootIndexEntryId, childArray));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(childArray.Count(), 60);
	ResetAndDestroy(childArray);

	// 5. Entry list has few non existing entry Ids.
	for(int index=50; index<70; index++)
		{
		TRAP(err, selection->AppendL(KFirstFreeEntryId+index));
		ASSERT_EQUALS(KErrNone, err);
		}
	
	// Deleting another 10 entries. The list has 20 entries
	// among which 10 does not exists.
	TRAP(err, iMsvDbAdaptor->DeleteEntryL(*selection));
	MY_ASSERT_EQUALS(err, KErrNone);
	selection->Reset();
	
	TRAP(err, iMsvDbAdaptor->GetChildrenL(KMsvRootIndexEntryId, childArray));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(childArray.Count(), 50);
	ResetAndDestroy(childArray);
	
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	// 6. Test deletion of entries from different drives.
	// 6.0 Cleanup: deleting the DB if it was created in an earlier test.
	//	   Also delete the first database to start afresh.
	delete iMsvDbAdaptor;
	iMsvDbAdaptor = NULL;
	TParse parse;
	TInt sysDrive = (TInt)RFs::GetSystemDrive();
	TPtrC drive = TDriveUnit(sysDrive).Name();
	parse.Set(KTestDbFileName, &drive, NULL);
	TFileName dbName = parse.FullName();
	RSqlDatabase::Delete(dbName);
	
	TRAP_IGNORE(iMsvDbAdaptor = CMsvDBAdapter::NewL());
	TMsvId maxId = 0;
	TRAPD(error, CMsvDBAdapter::CreateDBL(TDriveNumber(sysDrive)));
	ASSERT_EQUALS(error, KErrNone);
	//Let driveId for system drive be 1.
	TRAP(error, iMsvDbAdaptor->AttachDBL(TDriveUnit(sysDrive), 1, maxId));
	ASSERT_EQUALS(error, KErrNone);
	
	drive.Set(TDriveUnit(EDriveE).Name());
	parse.Set(KTestDbFileName, &drive, NULL);
	dbName = parse.FullName();
	RSqlDatabase::Delete(dbName);
	// (b) Create DB with driveId 2. System drive had driveId 1.
	maxId = 0;
	TInt driveId = 2;
	TRAP(err, CMsvDBAdapter::CreateDBL(TDriveNumber(EDriveE)));
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, iMsvDbAdaptor->AttachDBL(TDriveUnit(EDriveE), driveId, maxId));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	// 6.1 Add entries into the attached database.
	for(int index=0; index<25; index++)
		{
		TRAP(err, CreateEntryL((driveId<<28)|KFirstFreeEntryId+index, KMsvRootIndexEntryId, KUidMsvMessageEntry, visibleFolder));
		//Avoid using MY_ASSERT_EQUALS as it will clutter the log file.
		ASSERT_EQUALS(KErrNone, err);
		ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolder);
		
		TRAP(err, selection->AppendL((driveId<<28)|KFirstFreeEntryId+index));
		ASSERT_EQUALS(KErrNone, err);
		}
	
	// 6.2 Add entries into the first database.
	for(int index=250; index<275; index++)
		{
		TRAP(err, CreateEntryL(KFirstFreeEntryId+index, KMsvRootIndexEntryId, KUidMsvMessageEntry, visibleFolder));
		//Avoid using MY_ASSERT_EQUALS as it will clutter the log file.
		ASSERT_EQUALS(KErrNone, err);
		ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolder);
		
		TRAP(err, selection->AppendL(KFirstFreeEntryId+index));
		ASSERT_EQUALS(KErrNone, err);
		}
	
	for(int index=25; index<50; index++)
		{
		TRAP(err, CreateEntryL((driveId<<28)|KFirstFreeEntryId+index, KMsvRootIndexEntryId, KUidMsvMessageEntry, visibleFolder));
		//Avoid using MY_ASSERT_EQUALS as it will clutter the log file.
		ASSERT_EQUALS(KErrNone, err);
		ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolder);
		
		TRAP(err, selection->AppendL((driveId<<28)|KFirstFreeEntryId+index));
		ASSERT_EQUALS(KErrNone, err);
		}
	//Now selection has 25 from driveId 2, 25 from driveId 1 and 25 from driveId 2
	//again.
	TRAP(err, iMsvDbAdaptor->DeleteEntryL(*selection));
	MY_ASSERT_EQUALS(err, KErrNone);
	selection->Reset();
	
	//Confirm that everything has been deleted.
	driveId = 2;
	for(int index=0; index<25; index++)
		{
		ASSERT_TRUE(!EntryExists((driveId<<28)|KFirstFreeEntryId+index));
		}
	for(int index=250; index<275; index++)
		{
		ASSERT_TRUE(!EntryExists(KFirstFreeEntryId+index));
		}
#endif
	delete selection;
	}




void CTestDbAdapter::TestGetInPreparationId()
	{
	_LIT(KFunction, "TestGetInPreparationId");
	INFO_PRINTF1(KFunction);
	
	// 0. Create Test Entries
	TInt err;
	TMsvId visibleFolder;
	TInt driveId = 1;
	visibleFolder = KMsvRootIndexEntryId;
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	// (i) Checking for root entry.
#else
	TRAP(err, CreateEntryL(KMsvRootIndexEntryId, KErrNotFound, KUidMsvRootEntry, visibleFolder, ETrue));
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolder);
#endif
	MY_ASSERT_TRUE(EntryExists(KMsvRootIndexEntryId));
	
	// (ii) Creating 100 message entries under root.
	for(int index=0; index<100; index++)
		{
		if( !(index%3) )
			{
			TRAP(err, CreateEntryL(KFirstFreeEntryId+index, KMsvRootIndexEntryId, KUidMsvMessageEntry, visibleFolder, EFalse, ETrue));
			}
		else
			{
			TRAP(err, CreateEntryL(KFirstFreeEntryId+index, KMsvRootIndexEntryId, KUidMsvMessageEntry, visibleFolder));
			}
		//Avoid using MY_ASSERT_EQUALS as it will clutter the log file.
		ASSERT_EQUALS(KErrNone, err);
		ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolder);
		}

	CMsvEntrySelection *selection = NULL;
	TRAP_IGNORE(selection = new(ELeave)CMsvEntrySelection); 
	TRAP(err, iMsvDbAdaptor->GetInPreparationIdL(*selection));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(34, selection->Count());
	
	for(int index=0, Id=0; index<100; index+=3, Id++)
		{
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
		TMsvId temp = (*selection)[Id];
		TUint drivesId = (temp&~0x0fffffff) >> 28;
		ASSERT_EQUALS((*selection)[Id], (driveId<<28)|KFirstFreeEntryId+index);
#else
		ASSERT_EQUALS((*selection)[Id], KFirstFreeEntryId+index);
#endif
		}
	delete selection;
	
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	// (iii) Test in an attached database.
	// Cleanup: deleting the DB if it was created in an earlier test.
	TParse parse;
	TPtrC drive = TDriveUnit(EDriveE).Name();
	parse.Set(KTestDbFileName, &drive, NULL);
	TFileName dbName = parse.FullName();
	RSqlDatabase::Delete(dbName);
	
	// Create DB with driveId 2. System drive had driveId 1.
	TMsvId maxId = 0;
	driveId = 2;
	TRAP(err, CMsvDBAdapter::CreateDBL(TDriveNumber(EDriveE)));
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, iMsvDbAdaptor->AttachDBL(TDriveUnit(EDriveE), driveId, maxId));
	MY_ASSERT_EQUALS(err, KErrNone);
	visibleFolder = KMsvRootIndexEntryId;
	for(TInt index = 0; index < 50; ++index)
		{
		if(index%2)
			{
			TRAP(err, CreateEntryL((driveId<<28)|KFirstFreeEntryId+index, KMsvRootIndexEntryId, KUidMsvMessageEntry, visibleFolder, EFalse, ETrue));
			}
		else
			{
			TRAP(err, CreateEntryL((driveId<<28)|KFirstFreeEntryId+index, KMsvRootIndexEntryId, KUidMsvMessageEntry, visibleFolder));
			}
		ASSERT_EQUALS(KErrNone, err);
		}
	
	TRAP(err, selection = new(ELeave)CMsvEntrySelection); 
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, iMsvDbAdaptor->GetInPreparationIdL(*selection));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(34+25, selection->Count());
	delete selection;
#endif	
	}



void CTestDbAdapter::TestUpdateVisibleFolder()
	{
	_LIT(KFunction, "TestUpdateVisibleFolder");
	INFO_PRINTF1(KFunction);
	
	TInt err;
	TInt bulkCount = 60;
	TMsvId visibleFolderId, id = KFirstFreeEntryId;
	
	//1. Create root and some other folders.
	visibleFolderId = KMsvRootIndexEntryId;
#if (!defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	//1.1 Inbox.
	TRAP(err, CreateEntryL(KMsvGlobalInBoxIndexEntryId, KMsvRootIndexEntryId, KUidMsvFolderEntry, visibleFolderId, ETrue));
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolderId);
#endif
	//1.2 Outbox.
	TRAP(err, CreateEntryL(KMsvGlobalOutBoxIndexEntryId, KMsvRootIndexEntryId, KUidMsvFolderEntry, visibleFolderId, ETrue));
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolderId);

	
	//2. Create child entries for visible folder.
	//2.1 Add children of Inbox.
	CMsvEntrySelection* entryList = NULL;
	TRAP_IGNORE(entryList = new(ELeave) CMsvEntrySelection);
	TRAP(err, CreateEntryL(id+1, KMsvGlobalInBoxIndexEntryId, KUidMsvMessageEntry, visibleFolderId));
	ASSERT_EQUALS(err, KErrNone);
	TRAP_IGNORE(entryList->AppendL(id+1));
	visibleFolderId = KMsvGlobalInBoxIndexEntryId;
	for(TInt index = 2; index <= bulkCount; ++index) //60 children
		{
		TRAP(err, CreateEntryL(id+index, KMsvGlobalInBoxIndexEntryId, KUidMsvMessageEntry, visibleFolderId));
		ASSERT_EQUALS(err, KErrNone);
		TRAP_IGNORE(entryList->AppendL(id+index));
		}

	//2.2 Change visible folder to Outbox for all the children.
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	TInt driveId = 1;
	TRAP(err, iMsvDbAdaptor->UpdateVisibleFolderL(entryList, KMsvGlobalOutBoxIndexEntryId));
	ASSERT_EQUALS(err, KErrNone);
	
	for(TInt index = 1; index <= bulkCount; ++index) //60 children
		{
		TRAP_IGNORE(iMsvDbAdaptor->GetVisibleFolderIdL(id+index, visibleFolderId));
		ASSERT_EQUALS(visibleFolderId, KMsvGlobalOutBoxIndexEntryId);
		}
	delete entryList;
		
	// 3. Test in an attached database.
	// 3.0 Cleanup: deleting the DB if it was created in an earlier test.
	TParse parse;
	TPtrC drive = TDriveUnit(EDriveE).Name();
	parse.Set(KTestDbFileName, &drive, NULL);
	TFileName dbName = parse.FullName();
	RSqlDatabase::Delete(dbName);
	
	// 3.1 Create DB with driveId 2. System drive had driveId 1.
	TMsvId maxId = 0;
	driveId = 2;
	TRAP(err, CMsvDBAdapter::CreateDBL(TDriveNumber(EDriveE)));
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, iMsvDbAdaptor->AttachDBL(TDriveUnit(EDriveE), driveId, maxId));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	// 3.2 Create entries in both first and attached databases.
	TRAP(err, entryList = new(ELeave) CMsvEntrySelection);
	MY_ASSERT_EQUALS(err, KErrNone);
	for(TInt index = 100; index <130; ++index) //30 entries
		{
		if(index%2)
			{
			driveId = 1;
			}
		else
			{
			driveId = 2;			
			}
		visibleFolderId = (driveId<<28)|KMsvDraftEntryId;
		TRAP(err, CreateEntryL((driveId<<28)|id+index, KMsvDraftEntryId, KUidMsvMessageEntry, visibleFolderId));
		ASSERT_EQUALS(err, KErrNone);
		TRAP_IGNORE(entryList->AppendL((driveId<<28)|id+index));
		}

	// 3.3 Change visible folder to Sent for all the children.
	TRAP(err, iMsvDbAdaptor->UpdateVisibleFolderL(entryList, KMsvSentEntryId));
	ASSERT_EQUALS(err, KErrNone);
	
	for(TInt index =100; index <130; ++index)
		{
		if(index%2)
			{
			driveId = 1;
			}
		else
			{
			driveId = 2;			
			}
		TRAP_IGNORE(iMsvDbAdaptor->GetVisibleFolderIdL((driveId<<28)|id+index, visibleFolderId));
		ASSERT_EQUALS(visibleFolderId, KMsvSentEntryId);
		}
#else		// #if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)

	TRAP(err, iMsvDbAdaptor->UpdateVisibleFolderL(entryList, KMsvGlobalOutBoxIndexEntryId));
	ASSERT_EQUALS(err, KErrNone);
	
	for(TInt index = 1; index <= bulkCount; ++index) //60 children
		{
		TRAP_IGNORE(iMsvDbAdaptor->GetVisibleFolderIdL(id+index, visibleFolderId));
		MY_ASSERT_EQUALS(visibleFolderId, KMsvGlobalOutBoxIndexEntryId);
		}
#endif
	delete entryList;
	
	}



void CTestDbAdapter::TestUpdateOwnerStatus()
	{
	_LIT(KFunction, "TestUpdateOwnerStatus");
	INFO_PRINTF1(KFunction);
	
	TInt err;
	TMsvId visibleFolderId, id = KFirstFreeEntryId;
	
	//1. Create root and some other folders.
	visibleFolderId = KMsvRootIndexEntryId;
#if (!defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	TRAP(err, CreateEntryL(KMsvRootIndexEntryId, KErrNotFound, KUidMsvRootEntry, visibleFolderId, ETrue));
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolderId);
#endif
	//1.1 Inbox.
	TRAP(err, CreateEntryL(KMsvGlobalInBoxIndexEntryId, KMsvRootIndexEntryId, KUidMsvFolderEntry, visibleFolderId, ETrue));
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolderId);
	//1.2 Outbox.
	TRAP(err, CreateEntryL(KMsvGlobalOutBoxIndexEntryId, KMsvRootIndexEntryId, KUidMsvFolderEntry, visibleFolderId, ETrue));
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolderId);
	
	//2. Create child entries for Inbox, update and test setting of the flag.
	TRAP(err, CreateEntryL(id+1, KMsvGlobalInBoxIndexEntryId, KUidMsvMessageEntry, visibleFolderId));
	MY_ASSERT_EQUALS(err, KErrNone);
	CMsvCacheEntry* parentEntry = NULL;
	TRAP_IGNORE(iMsvDbAdaptor->GetEntryL(KMsvGlobalInBoxIndexEntryId, parentEntry, visibleFolderId));
	TRAP_IGNORE(iMsvDbAdaptor->UpdateOwnerStatusL(KMsvGlobalInBoxIndexEntryId, parentEntry->Entry(), ETrue));
	CMsvEntryFreePool::Instance()->ReleaseEntryL(parentEntry);
	TRAP(err, iMsvDbAdaptor->GetEntryL(KMsvGlobalInBoxIndexEntryId, parentEntry, visibleFolderId));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_TRUE(parentEntry->Entry().Owner());
	CMsvEntryFreePool::Instance()->ReleaseEntryL(parentEntry);
	
	//3. Test resetting of the flag
	TRAP_IGNORE(iMsvDbAdaptor->GetEntryL(KMsvGlobalInBoxIndexEntryId, parentEntry, visibleFolderId));
	TRAP_IGNORE(iMsvDbAdaptor->UpdateOwnerStatusL(KMsvGlobalInBoxIndexEntryId, parentEntry->Entry(), EFalse));
	CMsvEntryFreePool::Instance()->ReleaseEntryL(parentEntry);
	TRAP_IGNORE(iMsvDbAdaptor->GetEntryL(KMsvGlobalInBoxIndexEntryId, parentEntry, visibleFolderId));
	MY_ASSERT_FALSE(parentEntry->Entry().Owner());
	CMsvEntryFreePool::Instance()->ReleaseEntryL(parentEntry);
	
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	// 4. Create and attach a second database. Create entries in it and test.
	// 4.0 Cleanup: deleting the DB if it was created in an earlier test.
	TParse parse;
	TPtrC drive = TDriveUnit(EDriveE).Name();
	parse.Set(KTestDbFileName, &drive, NULL);
	TFileName dbName = parse.FullName();
	RSqlDatabase::Delete(dbName);
	
	// 4.1 Create DB with driveId 2. System drive had driveId 1.
	TMsvId maxId = 0;
	TInt driveId = 2;
	TRAP(err, CMsvDBAdapter::CreateDBL(TDriveNumber(EDriveE)));
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, iMsvDbAdaptor->AttachDBL(TDriveUnit(EDriveE), driveId, maxId));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	// 4.2 Create an entry to test setting and resetting of the flag.
	visibleFolderId = KMsvRootIndexEntryId;
	TRAP(err, CreateEntryL((driveId<<28)|KMsvGlobalInBoxIndexEntryId, KMsvRootIndexEntryId, KUidMsvFolderEntry, visibleFolderId, ETrue));
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolderId);
	//Setting the flag.
	parentEntry = NULL;
	TRAP_IGNORE(iMsvDbAdaptor->GetEntryL((driveId<<28)|KMsvGlobalInBoxIndexEntryId, parentEntry, visibleFolderId));
	TRAP_IGNORE(iMsvDbAdaptor->UpdateOwnerStatusL((driveId<<28)|KMsvGlobalInBoxIndexEntryId, parentEntry->Entry(), ETrue));
	CMsvEntryFreePool::Instance()->ReleaseEntryL(parentEntry);
	TRAP_IGNORE(iMsvDbAdaptor->GetEntryL((driveId<<28)|KMsvGlobalInBoxIndexEntryId, parentEntry, visibleFolderId));
	MY_ASSERT_TRUE(parentEntry->Entry().Owner());
	CMsvEntryFreePool::Instance()->ReleaseEntryL(parentEntry);
	//Flag already set, attempt to set again.
	parentEntry = NULL;
	TRAP_IGNORE(iMsvDbAdaptor->GetEntryL((driveId<<28)|KMsvGlobalInBoxIndexEntryId, parentEntry, visibleFolderId));
	TRAP_IGNORE(iMsvDbAdaptor->UpdateOwnerStatusL((driveId<<28)|KMsvGlobalInBoxIndexEntryId, parentEntry->Entry(), ETrue));
	CMsvEntryFreePool::Instance()->ReleaseEntryL(parentEntry);
	TRAP_IGNORE(iMsvDbAdaptor->GetEntryL((driveId<<28)|KMsvGlobalInBoxIndexEntryId, parentEntry, visibleFolderId));
	MY_ASSERT_TRUE(parentEntry->Entry().Owner());
	CMsvEntryFreePool::Instance()->ReleaseEntryL(parentEntry);
	//Resetting of the flag.
	TRAP_IGNORE(iMsvDbAdaptor->GetEntryL((driveId<<28)|KMsvGlobalInBoxIndexEntryId, parentEntry, visibleFolderId));
	TRAP_IGNORE(iMsvDbAdaptor->UpdateOwnerStatusL((driveId<<28)|KMsvGlobalInBoxIndexEntryId, parentEntry->Entry(), EFalse));
	CMsvEntryFreePool::Instance()->ReleaseEntryL(parentEntry);
	TRAP_IGNORE(iMsvDbAdaptor->GetEntryL((driveId<<28)|KMsvGlobalInBoxIndexEntryId, parentEntry, visibleFolderId));
	MY_ASSERT_FALSE(parentEntry->Entry().Owner());
	CMsvEntryFreePool::Instance()->ReleaseEntryL(parentEntry);
	//Flag already reset, attempt to reset again.
	TRAP_IGNORE(iMsvDbAdaptor->GetEntryL((driveId<<28)|KMsvGlobalInBoxIndexEntryId, parentEntry, visibleFolderId));
	TRAP_IGNORE(iMsvDbAdaptor->UpdateOwnerStatusL((driveId<<28)|KMsvGlobalInBoxIndexEntryId, parentEntry->Entry(), EFalse));
	CMsvEntryFreePool::Instance()->ReleaseEntryL(parentEntry);
	TRAP_IGNORE(iMsvDbAdaptor->GetEntryL((driveId<<28)|KMsvGlobalInBoxIndexEntryId, parentEntry, visibleFolderId));
	MY_ASSERT_FALSE(parentEntry->Entry().Owner());
	CMsvEntryFreePool::Instance()->ReleaseEntryL(parentEntry);
#endif	
	}



void CTestDbAdapter::TestGetChildren()
	{
	_LIT(KFunction, "TestGetChildren");
	INFO_PRINTF1(KFunction);

	// Create Test Entries
	TInt err;
	TMsvId visibleFolder;
	// 1. Checking for root entry.
	visibleFolder = KMsvRootIndexEntryId;
#if (!defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	TRAP(err, CreateEntryL(KMsvRootIndexEntryId, KErrNotFound, KUidMsvRootEntry, visibleFolder, ETrue));
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolder);
#endif
	MY_ASSERT_TRUE(EntryExists(KMsvRootIndexEntryId));
	
	// 2. Creating normal folder under root.
	TRAP(err, CreateEntryL(KMsvGlobalInBoxIndexEntryId, KMsvRootIndexEntryId, KUidMsvFolderEntry, visibleFolder, ETrue));
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolder);
	MY_ASSERT_TRUE(EntryExists(KMsvGlobalInBoxIndexEntryId));

	// 3. Creating 100 message entries under root.
	for(int index=0; index<100; index++)
		{
		TRAP(err, CreateEntryL(KFirstFreeEntryId+index, KMsvRootIndexEntryId, KUidMsvMessageEntry, visibleFolder));
		//Avoid using MY_ASSERT_EQUALS as it will clutter the log file.
		ASSERT_EQUALS(KErrNone, err);
		ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolder);
		}

	RPointerArray<CMsvCacheEntry> childArray;
	
	// Probing children of root.
	TRAP(err, iMsvDbAdaptor->GetChildrenL(KMsvRootIndexEntryId, childArray));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(childArray.Count(), 101);
	
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	TInt driveId = (1<<28);
#else
	TInt driveId = 0x0;
#endif
	MY_ASSERT_EQUALS(childArray[0]->Entry().Id(), KMsvGlobalInBoxIndexEntryId);
	for(int index=1; index<101; index++)
		{
		CMsvCacheEntry *entry = childArray[index];
		ASSERT_EQUALS(entry->Entry().Id(), driveId|(KFirstFreeEntryId+index-1));
		}
	ResetAndDestroy(childArray);

	// Probing parent entry with zero children.
	TRAP(err, iMsvDbAdaptor->GetChildrenL(driveId|KMsvGlobalInBoxIndexEntryId, childArray));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(childArray.Count(), 0);
	ResetAndDestroy(childArray);
	
	// 4. Creating 70 message entries under root.
	visibleFolder = KMsvGlobalInBoxIndexEntryId;
	for(int index=100; index<170; index++)
		{
		TRAP(err, CreateEntryL(KFirstFreeEntryId+index, KMsvGlobalInBoxIndexEntryId, KUidMsvMessageEntry, visibleFolder));
		//Avoid using MY_ASSERT_EQUALS as it will clutter the log file.
		ASSERT_EQUALS(KErrNone, err);
		ASSERT_EQUALS(KMsvGlobalInBoxIndexEntryId, visibleFolder);
		}

	TRAP(err, iMsvDbAdaptor->GetChildrenL(driveId|KMsvGlobalInBoxIndexEntryId, childArray));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(childArray.Count(), 70);
	ResetAndDestroy(childArray);
	
	// Deleting 40 children of root entry.
	for(int index=0; index<40; index++)
		{
		TRAP(err, iMsvDbAdaptor->DeleteEntryL(driveId|KFirstFreeEntryId+index));
		ASSERT_EQUALS(err, KErrNone);
		}
	TRAP(err, iMsvDbAdaptor->DeleteEntryL(driveId|KMsvGlobalInBoxIndexEntryId));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	TRAP(err, iMsvDbAdaptor->GetChildrenL(KMsvRootIndexEntryId, childArray));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(childArray.Count(), 60);	
	MY_ASSERT_EQUALS(childArray[0]->Entry().Id(), driveId|KFirstFreeEntryId+40);
	ResetAndDestroy(childArray);
	
	// Creating another 1000 message entries under root.
	// >>> If we test with 1000 entries the test case fails.
	// >>> May be because of limitation of entry free pool...
	// >>> Needs to be checked later.
	
	// Test for just 100 entries.
	visibleFolder = KMsvRootIndexEntryId;
	for(int index=1000; index<1100; index++)
		{
		TRAP(err, CreateEntryL(KFirstFreeEntryId+index, KMsvRootIndexEntryId, KUidMsvMessageEntry, visibleFolder));
		//Avoid using MY_ASSERT_EQUALS as it will clutter the log file.
		ASSERT_EQUALS(KErrNone, err);
		ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolder);
		}
	
	// GetChildren() on a folder with more then 100 children.
	TRAP(err, iMsvDbAdaptor->GetChildrenL(KMsvRootIndexEntryId, childArray));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(childArray.Count(), 160);	
	MY_ASSERT_EQUALS(childArray[0]->Entry().Id(), driveId|KFirstFreeEntryId+40);
	ResetAndDestroy(childArray);
	
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	// 5. Create and attach a second database. Create entries in it and test.
	// 5.0 Cleanup: deleting the DB if it was created in an earlier test.
	TParse parse;
	TPtrC drive = TDriveUnit(EDriveE).Name();
	parse.Set(KTestDbFileName, &drive, NULL);
	TFileName dbName = parse.FullName();
	RSqlDatabase::Delete(dbName);
	
	// 5.1 Create DB with driveId 2. System drive had driveId 1.
	TMsvId maxId = 0;
	driveId = 2;  // E:
	TRAP(err, CMsvDBAdapter::CreateDBL(TDriveNumber(EDriveE)));
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, iMsvDbAdaptor->AttachDBL(TDriveUnit(EDriveE), driveId, maxId));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	// 5.2 Create Inbox and children under it.
	visibleFolder = KMsvRootIndexEntryId;
	TRAP(err, CreateEntryL((driveId<<28)|KMsvLocalServiceIndexEntryId, KMsvRootIndexEntryId, KUidMsvFolderEntry, visibleFolder, ETrue));
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolder);
	MY_ASSERT_TRUE(EntryExists((driveId<<28)|KMsvLocalServiceIndexEntryId));
	TRAP(err, CreateEntryL((driveId<<28)|KFirstFreeEntryId, KMsvLocalServiceIndexEntryId, KUidMsvFolderEntry, visibleFolder, ETrue));
	MY_ASSERT_EQUALS(KErrNone, err);
	TRAP(err, CreateEntryL((driveId<<28)|(KFirstFreeEntryId+1), KMsvLocalServiceIndexEntryId, KUidMsvMessageEntry, visibleFolder, ETrue));
	MY_ASSERT_EQUALS(KErrNone, err);
	TRAP(err, CreateEntryL((driveId<<28)|(KFirstFreeEntryId+2), KMsvLocalServiceIndexEntryId, KUidMsvFolderEntry, visibleFolder, EFalse));
	MY_ASSERT_EQUALS(KErrNone, err);
	TRAP(err, CreateEntryL((driveId<<28)|(KFirstFreeEntryId+3), KMsvLocalServiceIndexEntryId, KUidMsvFolderEntry, visibleFolder, EFalse));
	MY_ASSERT_EQUALS(KErrNone, err);
	visibleFolder = KMsvGlobalInBoxIndexEntryId;
	TRAP(err, CreateEntryL((driveId<<28)|KMsvGlobalInBoxIndexEntryId, KMsvLocalServiceIndexEntryId, KUidMsvFolderEntry, visibleFolder, ETrue));
	MY_ASSERT_EQUALS(KErrNone, err);
	TRAP(err, CreateEntryL((driveId<<28)|KMsvGlobalOutBoxIndexEntryId, KMsvLocalServiceIndexEntryId, KUidMsvMessageEntry, visibleFolder, ETrue));
	MY_ASSERT_EQUALS(KErrNone, err);
	TRAP(err, CreateEntryL((driveId<<28)|KMsvDraftEntryId, KMsvLocalServiceIndexEntryId, KUidMsvFolderEntry, visibleFolder, EFalse));
	MY_ASSERT_EQUALS(KErrNone, err);
	TRAP(err, CreateEntryL((driveId<<28)|KMsvSentEntryId, KMsvLocalServiceIndexEntryId, KUidMsvFolderEntry, visibleFolder, EFalse));
	MY_ASSERT_EQUALS(KErrNone, err);
	TRAP(err, iMsvDbAdaptor->GetChildrenL((driveId<<28)|KMsvLocalServiceIndexEntryId, childArray));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(childArray.Count(), 7);	
	ResetAndDestroy(childArray);
	
	driveId = 1;  // C:
	visibleFolder = KMsvRootIndexEntryId;
	TRAP(err, CreateEntryL((driveId<<28)|KMsvLocalServiceIndexEntryId, KMsvRootIndexEntryId, KUidMsvFolderEntry, visibleFolder, ETrue));
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolder);
	MY_ASSERT_TRUE(EntryExists((driveId<<28)|KMsvLocalServiceIndexEntryId));
	TRAP(err, CreateEntryL((driveId<<28)|KFirstFreeEntryId, KMsvLocalServiceIndexEntryId, KUidMsvFolderEntry, visibleFolder, ETrue));
	MY_ASSERT_EQUALS(KErrNone, err);
	TRAP(err, CreateEntryL((driveId<<28)|(KFirstFreeEntryId+1), KMsvLocalServiceIndexEntryId, KUidMsvMessageEntry, visibleFolder, ETrue));
	MY_ASSERT_EQUALS(KErrNone, err);
	TRAP(err, CreateEntryL((driveId<<28)|(KFirstFreeEntryId+2), KMsvLocalServiceIndexEntryId, KUidMsvFolderEntry, visibleFolder, EFalse));
	MY_ASSERT_EQUALS(KErrNone, err);
	TRAP(err, CreateEntryL((driveId<<28)|(KFirstFreeEntryId+3), KMsvLocalServiceIndexEntryId, KUidMsvFolderEntry, visibleFolder, EFalse));
	MY_ASSERT_EQUALS(KErrNone, err);
	visibleFolder = KMsvGlobalInBoxIndexEntryId;
	TRAP(err, CreateEntryL((driveId<<28)|KMsvGlobalInBoxIndexEntryId, KMsvLocalServiceIndexEntryId, KUidMsvFolderEntry, visibleFolder, ETrue));
	MY_ASSERT_EQUALS(KErrNone, err);
	TRAP(err, CreateEntryL((driveId<<28)|KMsvGlobalOutBoxIndexEntryId, KMsvLocalServiceIndexEntryId, KUidMsvMessageEntry, visibleFolder, ETrue));
	MY_ASSERT_EQUALS(KErrNone, err);
	TRAP(err, CreateEntryL((driveId<<28)|KMsvDraftEntryId, KMsvLocalServiceIndexEntryId, KUidMsvFolderEntry, visibleFolder, EFalse));
	MY_ASSERT_EQUALS(KErrNone, err);
	TRAP(err, CreateEntryL((driveId<<28)|KMsvSentEntryId, KMsvLocalServiceIndexEntryId, KUidMsvFolderEntry, visibleFolder, EFalse));
	MY_ASSERT_EQUALS(KErrNone, err);
	TRAP(err, iMsvDbAdaptor->GetChildrenL((driveId<<28)|KMsvLocalServiceIndexEntryId, childArray));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(childArray.Count(), 8);	
	ResetAndDestroy(childArray);

	driveId = 2;
	visibleFolder = KMsvGlobalInBoxIndexEntryId;
	for(int index=1000; index<1100; index++)
		{
		TRAP(err, CreateEntryL((driveId<<28)|KFirstFreeEntryId+index, (driveId<<28)|KMsvGlobalInBoxIndexEntryId, KUidMsvMessageEntry, visibleFolder));
		//Avoid using MY_ASSERT_EQUALS as it will clutter the log file.
		ASSERT_EQUALS(KErrNone, err);
		ASSERT_EQUALS(KMsvGlobalInBoxIndexEntryId, visibleFolder);
		}
	
	// 5.3 GetChildren() on Inbox.
	TRAP(err, iMsvDbAdaptor->GetChildrenL((driveId<<28)|KMsvGlobalInBoxIndexEntryId, childArray));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(childArray.Count(), 100);	
	MY_ASSERT_EQUALS(childArray[0]->Entry().Id(), (driveId<<28)|KFirstFreeEntryId+1000);
	ResetAndDestroy(childArray);
#endif	
	}



void CTestDbAdapter::TestGetChildrenCondL()
	{
	_LIT(KFunction, "TestGetChildrenCondL");
	INFO_PRINTF1(KFunction);

	// Create Test Entries
	TInt err;
	TMsvId visibleFolder;
	// 1. Checking for root entry.
	visibleFolder = KMsvRootIndexEntryId;
#if (!defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	TRAP(err, CreateEntryL(KMsvRootIndexEntryId, KErrNotFound, KUidMsvRootEntry, visibleFolder, ETrue));
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolder);
#endif
	MY_ASSERT_TRUE(EntryExists(KMsvRootIndexEntryId));
	
	// 2. Creating 100 message entries under root.
	for(int index=0; index<100; index++)
		{
		TRAP(err, CreateEntryL(KFirstFreeEntryId+index, KMsvRootIndexEntryId, KUidMsvMessageEntry, visibleFolder));
		//Avoid using MY_ASSERT_EQUALS as it will clutter the log file.
		ASSERT_EQUALS(KErrNone, err);
		ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolder);
		}

	RPointerArray<CMsvCacheEntry> childArray;
	
	// Probing children of root.
	//_LIT8(KQueryString, "and (id between 1048580 and 1048585) order by id ASC;");
	RBuf8 buf;
	
	buf.Create(200);
	CleanupClosePushL(buf);

	buf.Append(KAnd);
	buf.Append(KLtBrace);
	buf.Append(KId);
	buf.Append(KBetween);
	buf.AppendNum(1048580);
	buf.Append(KAnd);
	buf.AppendNum(1048585);
	buf.Append(KRtBrace);
	buf.Append(KOrder);
	buf.Append(KSemiColon);
	
	TRAP(err, iMsvDbAdaptor->GetChildrenL(buf, KMsvRootIndexEntryId, childArray));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(childArray.Count(), 6);
	
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	TInt driveId = 1<<28;	
#else
	TInt driveId = 0x0;
#endif
	for(int index=0; index<6; index++)
		{
		CMsvCacheEntry *entry = childArray[index];
		ASSERT_EQUALS(entry->Entry().Id(), driveId|(KFirstFreeEntryId + index + 4));
		}
	ResetAndDestroy(childArray);
	CleanupStack::PopAndDestroy();

	// Two intervals.
	//_LIT8(KQueryString1, "and (id between 1048580 and 1048585 or id between 1048600 and 1048700) order by id ASC;");

	RBuf8 buf1;
	buf1.Create(200);
	CleanupClosePushL(buf1);

	buf1.Append(KAnd);
	buf1.Append(KLtBrace);
	buf1.Append(KId);
	buf1.Append(KBetween);
	buf1.AppendNum(1048580);
	buf1.Append(KAnd);
	buf1.AppendNum(1048585);
	buf1.Append(KOr);
	buf1.Append(KId);
	buf1.Append(KBetween);
	buf1.AppendNum(1048600);
	buf1.Append(KAnd);
	buf1.AppendNum(1048700);
	buf1.Append(KRtBrace);
	buf1.Append(KOrder);
	buf1.Append(KSemiColon);
	
	TRAP(err, iMsvDbAdaptor->GetChildrenL(buf1, KMsvRootIndexEntryId, childArray));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(childArray.Count(), 82);
	
	for(int index=0; index<82; index++)
		{
		CMsvCacheEntry *entry = childArray[index];
		if( index < 6 )
			{
			ASSERT_EQUALS(entry->Entry().Id(), driveId|(KFirstFreeEntryId + index + 4));
			}
		else
			{
			ASSERT_EQUALS(entry->Entry().Id(), driveId|(KFirstFreeEntryId + index + 18));					
			}
		}
	
	ResetAndDestroy(childArray);
	CleanupStack::PopAndDestroy();
	
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)	
	// 3. Test in an attached database.
	// 3.0 Cleanup: deleting the DB if it was created in an earlier test.
	TParse parse;
	TPtrC drive = TDriveUnit(EDriveE).Name();
	parse.Set(KTestDbFileName, &drive, NULL);
	TFileName dbName = parse.FullName();
	RSqlDatabase::Delete(dbName);
	
	// 3.1 Create DB with driveId 2. System drive had driveId 1.
	TMsvId maxId = 0;
	driveId = 2;
	TRAP(err, CMsvDBAdapter::CreateDBL(TDriveNumber(EDriveE)));
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, iMsvDbAdaptor->AttachDBL(TDriveUnit(EDriveE), driveId, maxId));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	// 3.2 Create entries.
	visibleFolder = KMsvRootIndexEntryId;
	TRAP(err, CreateEntryL((driveId<<28)|KMsvGlobalInBoxIndexEntryId, KMsvRootIndexEntryId, KUidMsvFolderEntry, visibleFolder, ETrue));
	MY_ASSERT_EQUALS(KErrNone, err);
	
	visibleFolder = (driveId<<28)|KMsvGlobalInBoxIndexEntryId;
	for(int index=0; index<100; index++)
		{
		TRAP(err, CreateEntryL((driveId<<28)|KFirstFreeEntryId+index, (driveId<<28)|KMsvGlobalInBoxIndexEntryId, KUidMsvMessageEntry, visibleFolder));
		//Avoid using MY_ASSERT_EQUALS as it will clutter the log file.
		ASSERT_EQUALS(KErrNone, err);
		ASSERT_EQUALS((driveId<<28)|KMsvGlobalInBoxIndexEntryId, visibleFolder);
		}

	// Two intervals.
	//_LIT8(KQueryString1, "and (id between 1048580 and 1048585 or id between 1048600 and 1048700) order by id ASC;");

	RBuf8 buf2;
	buf2.Create(200);
	CleanupClosePushL(buf2);

	buf2.Append(KAnd);
	buf2.Append(KLtBrace);
	buf2.Append(KId);
	buf2.Append(KBetween);
	buf2.AppendNum(1048580);
	buf2.Append(KAnd);
	buf2.AppendNum(1048585);
	buf2.Append(KOr);
	buf2.Append(KId);
	buf2.Append(KBetween);
	buf2.AppendNum(1048600);
	buf2.Append(KAnd);
	buf2.AppendNum(1048700);
	buf2.Append(KRtBrace);
	buf2.Append(KOrder);
	buf2.Append(KSemiColon);
	
	TRAP(err, iMsvDbAdaptor->GetChildrenL(buf2, (driveId<<28)|KMsvGlobalInBoxIndexEntryId, childArray));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(childArray.Count(), 82);
	
	for(int index=0; index<82; index++)
		{
		CMsvCacheEntry *entry = childArray[index];
		if( index < 6 )
			{
			ASSERT_EQUALS(entry->Entry().Id(), (driveId<<28)|(KFirstFreeEntryId + index + 4));
			}
		else
			{
			ASSERT_EQUALS(entry->Entry().Id(), (driveId<<28)|(KFirstFreeEntryId + index + 18));					
			}
		}
	
	ResetAndDestroy(childArray);
	CleanupStack::PopAndDestroy();
#endif
	}



#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
// Test code only for 557.
void CTestDbAdapter::TestMaxTMsvId()
	{
	_LIT(KFunction, "TestGetMaxTMsvId");
	INFO_PRINTF1(KFunction);

	TInt err;
	TMsvId maxId;
	TMsvId visibleFolder;
	
	// 2. Test in attached drive.
	// 2.0 Cleanup: deleting the DB if it was created in an earlier test.
	TParse parse;
	TPtrC drive = TDriveUnit(EDriveE).Name();
	parse.Set(KTestDbFileName, &drive, NULL);
	TFileName dbName = parse.FullName();
	RSqlDatabase::Delete(dbName);
	
	// 2.1 Create DB with driveId 2. System drive had driveId 1.
	maxId = 0;
	TInt driveId = 2;
	TRAP(err, CMsvDBAdapter::CreateDBL(TDriveNumber(EDriveE)));
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, iMsvDbAdaptor->AttachDBL(TDriveUnit(EDriveE), driveId, maxId));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(maxId, KMsvRootIndexEntryId); //root
	maxId = 0;
	TRAP(err, maxId = iMsvDbAdaptor->MaxTMsvIdL(driveId)); //new function
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(maxId, KMsvRootIndexEntryId);
	
	// 2.2 Create an entry in the first database.
	driveId = 1;
	visibleFolder = KMsvRootIndexEntryId;
	TRAP(err, CreateEntryL(KFirstFreeEntryId+198, KMsvRootIndexEntryId, KUidMsvAttachmentEntry, visibleFolder));
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolder);
	MY_ASSERT_TRUE(EntryExists(KFirstFreeEntryId+198));
	TRAP(err, maxId = iMsvDbAdaptor->MaxTMsvIdL(driveId)); //new function
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(maxId, (driveId<<28)|KFirstFreeEntryId+198);
	
	// 2.3 Create an entry in the attached database.
	driveId = 2;
	visibleFolder = KMsvRootIndexEntryId;
	TRAP(err, CreateEntryL((driveId<<28)|KFirstFreeEntryId+1, KMsvRootIndexEntryId, KUidMsvAttachmentEntry, visibleFolder));
	MY_ASSERT_EQUALS(KErrNone, err);
	//Notice that the above Id also exists in the first database, and that
	//there is no conflict.
	MY_ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolder);
	MY_ASSERT_TRUE(EntryExists((driveId<<28)|KFirstFreeEntryId+1));
	TRAP(err, maxId = iMsvDbAdaptor->MaxTMsvIdL(driveId)); //new function
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(maxId, (driveId<<28)|KFirstFreeEntryId+1);
	
	TRAP(err, CreateEntryL((driveId<<28)|KFirstFreeEntryId, KMsvRootIndexEntryId, KUidMsvAttachmentEntry, visibleFolder));
	MY_ASSERT_EQUALS(KErrNone, err);
	TRAP(err, maxId = iMsvDbAdaptor->MaxTMsvIdL(driveId)); //new function
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(maxId, (driveId<<28)|KFirstFreeEntryId+1);
	
	}



void CTestDbAdapter::TestValidateDatabase()
	{
	_LIT(KFunction, "TestValidateDatabase");
	INFO_PRINTF1(KFunction);

	// 0. Cleanup: deleting the DB if it was created in an earlier test.
	TParse parse;
	TPtrC drive = TDriveUnit(EDriveE).Name();
	parse.Set(KTestDbFileName, &drive, NULL);
	TFileName dbName = parse.FullName();
	RSqlDatabase::Delete(dbName);
	
	// 1. Database not present.
	TRAPD(err, CMsvDBAdapter::ValidateDatabaseL(EDriveE));
	MY_ASSERT_EQUALS(err, KErrNotFound);
	
	// 2. Database does not have required standard folders.
	// 2.0 Create DB with driveId 2. System drive had driveId 1.
	TMsvId maxId = 0;
	TInt driveId = 2;
	TRAP(err, CMsvDBAdapter::CreateDBL(TDriveNumber(EDriveE), ETrue, ETrue));
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, iMsvDbAdaptor->AttachDBL(TDriveUnit(EDriveE), driveId, maxId));
	MY_ASSERT_EQUALS(err, KErrNone);
	// 2.1 Test validity.
	TRAP(err, CMsvDBAdapter::ValidateDatabaseL(EDriveE));
	MY_ASSERT_EQUALS(err, KSqlErrCorrupt);
	
	// 3. Database is of an older version.
	// 3.0 Insert standard folders. We had created the database with an older
	// 	   version number. Test if the version is correctly detected.
	TMsvId visibleFolder = KMsvRootIndexEntryId;
	TRAP_IGNORE(CreateEntryL((driveId<<28)|KMsvLocalServiceIndexEntryId, KMsvRootIndexEntryId, KUidMsvServiceEntry, visibleFolder));
	visibleFolder = (driveId<<28)|KMsvLocalServiceIndexEntryId;
	TMsvId parentId = (driveId<<28)|KMsvLocalServiceIndexEntryId;
	TRAP_IGNORE(CreateEntryL((driveId<<28)|KMsvGlobalInBoxIndexEntryId, parentId, KUidMsvServiceEntry, visibleFolder));
	TRAP_IGNORE(CreateEntryL((driveId<<28)|KMsvGlobalOutBoxIndexEntryId, parentId, KUidMsvServiceEntry, visibleFolder));
	TRAP_IGNORE(CreateEntryL((driveId<<28)|KMsvDraftEntryId, parentId, KUidMsvServiceEntry, visibleFolder));
	TRAP_IGNORE(CreateEntryL((driveId<<28)|KMsvSentEntryId, parentId, KUidMsvServiceEntry, visibleFolder));
	TRAP_IGNORE(CreateEntryL((driveId<<28)|KMsvDeletedEntryFolderEntryId, parentId, KUidMsvServiceEntry, visibleFolder));
	// 3.1 Test validity.
	TRAP(err, CMsvDBAdapter::ValidateDatabaseL(EDriveE));
	MY_ASSERT_EQUALS(err, KErrNotSupported);
	
	// 4. Valid database.
	driveId = 1;
	TInt sysDrive = (TInt)RFs::GetSystemDrive();
	visibleFolder = KMsvRootIndexEntryId;
	TRAP_IGNORE(CreateEntryL(KMsvLocalServiceIndexEntryId, KMsvRootIndexEntryId, KUidMsvServiceEntry, visibleFolder));
	visibleFolder = KMsvLocalServiceIndexEntryId;
	parentId = KMsvLocalServiceIndexEntryId;
	TRAP_IGNORE(CreateEntryL(KMsvGlobalInBoxIndexEntryId, parentId, KUidMsvServiceEntry, visibleFolder));
	TRAP_IGNORE(CreateEntryL(KMsvGlobalOutBoxIndexEntryId, parentId, KUidMsvServiceEntry, visibleFolder));
	TRAP_IGNORE(CreateEntryL(KMsvDraftEntryId, parentId, KUidMsvServiceEntry, visibleFolder));
	TRAP_IGNORE(CreateEntryL(KMsvSentEntryId, parentId, KUidMsvServiceEntry, visibleFolder));
	TRAP_IGNORE(CreateEntryL(KMsvDeletedEntryFolderEntryId, parentId, KUidMsvServiceEntry, visibleFolder));
	
	TRAP(err, CMsvDBAdapter::ValidateDatabaseL(TDriveNumber(sysDrive)));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	}
	

void CTestDbAdapter::TestTransaction()
	{
	_LIT(KFunction, "TestTransaction");
	INFO_PRINTF1(KFunction);
	
	TInt err;
	TMsvId visibleFolder;
	// Creating entries.
	// 1. Create an entry using transaction.
	// 1.1
	// (a) Open the transaction.
	TRAP(err, iMsvDbAdaptor->BeginTransactionL());	
	MY_ASSERT_EQUALS(KErrNone, err);
	// (b) Create the entry.
	visibleFolder = KMsvRootIndexEntryId;
	TRAP(err, CreateEntryL(KMsvGlobalInBoxIndexEntryId, KMsvRootIndexEntryId, KUidMsvFolderEntry, visibleFolder, ETrue));
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolder);
	// (c) Commit the transaction.
	TRAP(err, iMsvDbAdaptor->CommitTransactionL());	
	MY_ASSERT_EQUALS(KErrNone, err);
	// (d) Check if the entry exists.
	MY_ASSERT_TRUE(EntryExists(KMsvRootIndexEntryId));
	MY_ASSERT_TRUE(EntryExists(KMsvGlobalInBoxIndexEntryId));
	//1.2 Test in an attached database
	//(a) Cleanup: deleting the DB if it was created in an earlier test.
	TParse parse;
	TPtrC drive = TDriveUnit(EDriveE).Name();
	parse.Set(KTestDbFileName, &drive, NULL);
	TFileName dbName = parse.FullName();
	RSqlDatabase::Delete(dbName);
	// (b) Create DB with driveId 2. System drive had driveId 1.
	TMsvId maxId = 0;
	TInt driveId = 2;
	TRAP(err, CMsvDBAdapter::CreateDBL(TDriveNumber(EDriveE)));
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, iMsvDbAdaptor->AttachDBL(TDriveUnit(EDriveE), driveId, maxId));
	MY_ASSERT_EQUALS(err, KErrNone);
	// (c) Open the transaction.
	TRAP(err, iMsvDbAdaptor->BeginTransactionL());	
	MY_ASSERT_EQUALS(KErrNone, err);
	// (d) Create an entry in the attached database.
	visibleFolder = KMsvRootIndexEntryId;
	//Notice that the Id's also exists in the first database, and that
	//there is no conflict.
	//Create Inbox.
	TRAP(err, CreateEntryL((driveId<<28)|KMsvGlobalInBoxIndexEntryId, KMsvRootIndexEntryId, KUidMsvFolderEntry, visibleFolder, ETrue));
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolder);
	// (e) Commit the transaction.
	TRAP(err, iMsvDbAdaptor->CommitTransactionL());	
	MY_ASSERT_EQUALS(KErrNone, err);
	// (f) Check if the entry exists.
	MY_ASSERT_TRUE(EntryExists((driveId<<28)|KMsvGlobalInBoxIndexEntryId));
	// (g) Add another entry (outside a transaction).
	visibleFolder = KMsvRootIndexEntryId;
	TRAP(err, CreateEntryL((driveId<<28)|KFirstFreeEntryId, KMsvRootIndexEntryId, KUidMsvAttachmentEntry, visibleFolder));
	MY_ASSERT_EQUALS(KErrNone, err);
	// (h) Open another transaction.
	TRAP(err, iMsvDbAdaptor->BeginTransactionL());	
	MY_ASSERT_EQUALS(KErrNone, err);
	// (i) Update an entry.
	CMsvCacheEntry *cacheEntry = NULL; 
	TRAP(err, iMsvDbAdaptor->GetEntryL((driveId<<28)|KFirstFreeEntryId, cacheEntry, visibleFolder));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(visibleFolder, KMsvRootIndexEntryId);
	TMsvEntry *entry = &(cacheEntry->Entry());
	MY_ASSERT_FALSE(entry->Visible());
	MY_ASSERT_TRUE(entry->Unread());
	
	entry->SetParent((driveId<<28)|KMsvGlobalInBoxIndexEntryId);
	entry->iDate.UniversalTime();
	entry->iType = KUidMsvFolderEntry;
	entry->SetVisible(ETrue);
	entry->SetUnread(EFalse);
	entry->iServiceId = KMsvRootIndexEntryId+1000;		// Some Random Id.
	entry->iRelatedId = KMsvRootIndexEntryId+2000;		// Some Random Id.
	entry->iMtm = KUidMsvFolderEntry;		
	entry->iSize = 128;
	entry->iError = -1;		
	entry->iBioType = 128;		
	entry->iMtmData1 = 128;		
	entry->iMtmData2 = 128;		
	entry->iMtmData3 = 128;	
	
	visibleFolder = (driveId<<28)|KMsvGlobalInBoxIndexEntryId;
	TRAP(err, iMsvDbAdaptor->UpdateEntryL(cacheEntry->Entry(), visibleFolder));
	MY_ASSERT_EQUALS(err, KErrNone);
	// (j) Close the database without performing rollback/commit.
	delete iMsvDbAdaptor;
	iMsvDbAdaptor = NULL;
	CMsvEntryFreePool::Instance()->ReleaseEntryL(cacheEntry);
	// (k) Create the DBAdapter again.
	TRAP_IGNORE(iMsvDbAdaptor = CMsvDBAdapter::NewL());
	TInt sysDrive = (TInt)RFs::GetSystemDrive();
	TRAPD(error, iMsvDbAdaptor->AttachDBL(TDriveUnit(sysDrive), 1, maxId));
	ASSERT_EQUALS(error, KErrNone);
	TRAP(err, iMsvDbAdaptor->AttachDBL(TDriveUnit(EDriveE), 2, maxId));
	MY_ASSERT_EQUALS(err, KErrNone);
	// (h) Entry should not be updated in the attached drive.
	TRAP(err, iMsvDbAdaptor->GetEntryL((driveId<<28)|KFirstFreeEntryId, cacheEntry, visibleFolder));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(cacheEntry->Entry().Parent(), KMsvRootIndexEntryId); //old parent
	CMsvEntryFreePool::Instance()->ReleaseEntryL(cacheEntry);
	
	
	// 2. Create an entry and rollback.
	// (a) Open the transaction.
	TRAP(err, iMsvDbAdaptor->BeginTransactionL());	
	MY_ASSERT_EQUALS(KErrNone, err);
	// (b) Create the entry.
	visibleFolder = KMsvRootIndexEntryId;
	TRAP(err, CreateEntryL(KFirstFreeEntryId, KMsvRootIndexEntryId, KUidMsvRootEntry, visibleFolder, ETrue));
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolder);
	MY_ASSERT_TRUE(EntryExists(KFirstFreeEntryId));
	// (c) Rollback the transaction.
	TRAP(err, iMsvDbAdaptor->RollbackTransactionL());	
	MY_ASSERT_EQUALS(KErrNone, err);
	// (d) Check if the entry exists.
	MY_ASSERT_TRUE(!EntryExists(KFirstFreeEntryId));
	// (e) Create the entry again.
	TRAP(err, iMsvDbAdaptor->BeginTransactionL());	
	MY_ASSERT_EQUALS(KErrNone, err);
	TRAP(err, CreateEntryL(KFirstFreeEntryId, KMsvRootIndexEntryId, KUidMsvRootEntry, visibleFolder, ETrue));
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolder);
	MY_ASSERT_TRUE(EntryExists(KFirstFreeEntryId));
	// (f) Close the database without performing rollback/commit.
	delete iMsvDbAdaptor;
	iMsvDbAdaptor = NULL;
	// (g) Create the DBAdapter again.
	TRAP_IGNORE(iMsvDbAdaptor = CMsvDBAdapter::NewL());
	sysDrive = (TInt)RFs::GetSystemDrive();
	driveId = 1;
	TRAP(error, iMsvDbAdaptor->AttachDBL(TDriveUnit(sysDrive), driveId, maxId));
	ASSERT_EQUALS(error, KErrNone);
	ASSERT_EQUALS(maxId, (driveId<<28)|KMsvGlobalInBoxIndexEntryId);
	// (h) Entry should not exist.
	MY_ASSERT_TRUE(!EntryExists(KFirstFreeEntryId));
	}


void CTestDbAdapter::TestGetVisibleFlag()
	{
	_LIT(KFunction, "TestGetVisibleFlag");
	INFO_PRINTF1(KFunction);
	
	// PreSetup.
	TUint driveId = 1;
	TMsvId visibleFolder = KMsvRootIndexEntryId;
	TRAPD(err, CreateEntryL((driveId<<28)|KFirstFreeEntryId, KMsvRootIndexEntryId, KUidMsvFolderEntry, visibleFolder, ETrue));
	MY_ASSERT_EQUALS(KErrNone, err);
	TRAP(err, CreateEntryL((driveId<<28)|(KFirstFreeEntryId+1), KMsvRootIndexEntryId, KUidMsvFolderEntry, visibleFolder, EFalse));
	MY_ASSERT_EQUALS(KErrNone, err);

	TBool res = EFalse;
	TRAP(err, res = iMsvDbAdaptor->GetVisibleFlagL((driveId<<28)|KFirstFreeEntryId));
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_TRUE(res);
	TRAP(err, res = iMsvDbAdaptor->GetVisibleFlagL((driveId<<28)|(KFirstFreeEntryId+1)));
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_FALSE(res);
	TRAP(err, res = iMsvDbAdaptor->GetVisibleFlagL((driveId<<28)|(KFirstFreeEntryId+5)));
	MY_ASSERT_EQUALS(KErrNotFound, err);	
	}
	
	

void CTestDbAdapter::TestGetChildrenId()
	{
	_LIT(KFunction, "TestGetChildrenId");
	INFO_PRINTF1(KFunction);
	
	// PreSetup.
	
	// Add another drive E.
	TParse parse;
	TPtrC drive = TDriveUnit(EDriveE).Name();
	parse.Set(KTestDbFileName, &drive, NULL);
	TFileName dbName = parse.FullName();
	RSqlDatabase::Delete(dbName);
	// Create DB with driveId 2. System drive had driveId 1.
	TMsvId maxId = 0;
	TInt driveId = 2;
	TRAPD(err, CMsvDBAdapter::CreateDBL(TDriveNumber(EDriveE)));
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, iMsvDbAdaptor->AttachDBL(TDriveUnit(EDriveE), driveId, maxId));
	MY_ASSERT_EQUALS(err, KErrNone);
	// Create entries under root folder in C:
	driveId = 1;
	TMsvId visibleFolder = KMsvRootIndexEntryId;
	TRAP(err, CreateEntryL((driveId<<28)|KFirstFreeEntryId, KMsvRootIndexEntryId, KUidMsvFolderEntry, visibleFolder, ETrue));
	MY_ASSERT_EQUALS(KErrNone, err);
	TRAP(err, CreateEntryL((driveId<<28)|(KFirstFreeEntryId+1), KMsvRootIndexEntryId, KUidMsvMessageEntry, visibleFolder, ETrue));
	MY_ASSERT_EQUALS(KErrNone, err);
	TRAP(err, CreateEntryL((driveId<<28)|(KFirstFreeEntryId+2), KMsvRootIndexEntryId, KUidMsvFolderEntry, visibleFolder, EFalse));
	MY_ASSERT_EQUALS(KErrNone, err);
	TRAP(err, CreateEntryL((driveId<<28)|(KFirstFreeEntryId+3), KMsvRootIndexEntryId, KUidMsvFolderEntry, visibleFolder, EFalse));
	MY_ASSERT_EQUALS(KErrNone, err);
	TRAP(err, CreateEntryL((driveId<<28)|KMsvGlobalInBoxIndexEntryId, KMsvRootIndexEntryId, KUidMsvFolderEntry, visibleFolder, ETrue));
	MY_ASSERT_EQUALS(KErrNone, err);
	TRAP(err, CreateEntryL((driveId<<28)|KMsvGlobalOutBoxIndexEntryId, KMsvRootIndexEntryId, KUidMsvMessageEntry, visibleFolder, ETrue));
	MY_ASSERT_EQUALS(KErrNone, err);
	TRAP(err, CreateEntryL((driveId<<28)|KMsvDraftEntryId, KMsvRootIndexEntryId, KUidMsvFolderEntry, visibleFolder, EFalse));
	MY_ASSERT_EQUALS(KErrNone, err);
	TRAP(err, CreateEntryL((driveId<<28)|KMsvSentEntryId, KMsvRootIndexEntryId, KUidMsvFolderEntry, visibleFolder, EFalse));
	MY_ASSERT_EQUALS(KErrNone, err);

	// Create entries under root folder in D:
	driveId = 2;
	TRAP(err, CreateEntryL((driveId<<28)|KFirstFreeEntryId, KMsvRootIndexEntryId, KUidMsvFolderEntry, visibleFolder, ETrue));
	MY_ASSERT_EQUALS(KErrNone, err);
	TRAP(err, CreateEntryL((driveId<<28)|(KFirstFreeEntryId+1), KMsvRootIndexEntryId, KUidMsvMessageEntry, visibleFolder, ETrue));
	MY_ASSERT_EQUALS(KErrNone, err);
	TRAP(err, CreateEntryL((driveId<<28)|(KFirstFreeEntryId+2), KMsvRootIndexEntryId, KUidMsvFolderEntry, visibleFolder, EFalse));
	MY_ASSERT_EQUALS(KErrNone, err);
	TRAP(err, CreateEntryL((driveId<<28)|(KFirstFreeEntryId+3), KMsvRootIndexEntryId, KUidMsvFolderEntry, visibleFolder, EFalse));
	MY_ASSERT_EQUALS(KErrNone, err);
	TRAP(err, CreateEntryL((driveId<<28)|KMsvGlobalInBoxIndexEntryId, KMsvRootIndexEntryId, KUidMsvFolderEntry, visibleFolder, ETrue));
	MY_ASSERT_EQUALS(KErrNone, err);
	TRAP(err, CreateEntryL((driveId<<28)|KMsvGlobalOutBoxIndexEntryId, KMsvRootIndexEntryId, KUidMsvMessageEntry, visibleFolder, ETrue));
	MY_ASSERT_EQUALS(KErrNone, err);
	TRAP(err, CreateEntryL((driveId<<28)|KMsvDraftEntryId, KMsvRootIndexEntryId, KUidMsvFolderEntry, visibleFolder, EFalse));
	MY_ASSERT_EQUALS(KErrNone, err);
	TRAP(err, CreateEntryL((driveId<<28)|KMsvSentEntryId, KMsvRootIndexEntryId, KUidMsvFolderEntry, visibleFolder, EFalse));
	MY_ASSERT_EQUALS(KErrNone, err);
	
	driveId = 1;   // C:
	CMsvEntrySelection* selection = NULL;
	TRAP_IGNORE(selection = new(ELeave) CMsvEntrySelection());
	TRAP(err, iMsvDbAdaptor->GetChildrenIdL((driveId<<28)|KMsvRootIndexEntryId, *selection));
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS(selection->Count(), 8);
	selection->Reset();
	
	driveId = 2;
	TRAP(err, iMsvDbAdaptor->GetChildrenIdL((driveId<<28)|KMsvRootIndexEntryId, *selection));
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS(selection->Count(), 7);
	
	TRAP(err, iMsvDbAdaptor->RollbackTransactionL());
	MY_ASSERT_EQUALS(err, KErrNone);
	
	delete selection;
	}
	
	
	
// As part of code coverage.
void CTestDbAdapter::TestCreateRootEntry()
	{
	_LIT(KFunction, "TestCreateRootEntry");
	INFO_PRINTF1(KFunction);
	_LIT8(KEncodingText, "encoding=UTF-8");
	_LIT8(KCreateIndexEntryTableQuery,	"CREATE TABLE IF NOT EXISTS IndexEntry ("
								"id INTEGER PRIMARY KEY,"
						  		"parentId INT,"
								"serviceId INT,"
								"mtmId INT,"
								"type INT,"
								"date INT64,"
								"data INT,"
								"size INT,"
								"error INT,"
								"mtmData1 INT,"
								"mtmData2 INT,"
								"mtmData3 INT,"
								"relatedId INT,"
								"bioType INT,"
								"pcSyncCount INT,"
								"reserved INT,"
								"visibleParent INT,"
								"description TEXT,"
						  		"details TEXT" ");"
	  );
	  
	TParse parse;
	TPtrC drive = TDriveUnit(EDriveE).Name();
	parse.Set(KTestDbFileName, &drive, NULL);
	TFileName dbName = parse.FullName();
	RSqlDatabase::Delete(dbName);

	// Create fresh DB in E:	
	RSqlDatabase tempDB;
	HBufC8* heapBuf = KEncodingText().AllocLC();

	//Create the database.
	TInt err = tempDB.Create(dbName, heapBuf);
	MY_ASSERT_EQUALS(err, KErrNone);
	CleanupStack::PopAndDestroy(); //heapBuf

	// Add root entry in DB (IndexEntry table not already created).
	TRAP(err, CMsvDBAdapter::CreateRootEntryL(tempDB));
	MY_ASSERT_EQUALS(err, KSqlErrGeneral);
	
	//Create tables for the database
	err = tempDB.Exec(KCreateIndexEntryTableQuery);
	MY_ASSERT_EQUALS(err, 1);
	
	// Add root entry in DB.
	TRAP(err, CMsvDBAdapter::CreateRootEntryL(tempDB));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	// Add root entry in DB again! (Entry already exists).
	TRAP(err, CMsvDBAdapter::CreateRootEntryL(tempDB));
	MY_ASSERT_EQUALS(err, KSqlErrConstraint);
	tempDB.Close();
	}
	
	

void CTestDbAdapter::TestDBPointerCreation()
	{
	_LIT(KFunction, "TestDBPointerCreation");
	INFO_PRINTF1(KFunction);
	
	_LIT(KMsgTestIndexFile, "C:\\private\\1000484b\\Index");
	delete iMsvDbAdaptor;
	
	TParse parse;
	TPtrC drive = TDriveUnit(EDriveC).Name();
	parse.Set(KMainDummyDBName, &drive, NULL);
	TFileName dbName = parse.FullName();
	RSqlDatabase::Delete(dbName);

	// Main DB does not exists.
	TRAPD(err, iMsvDbAdaptor = CMsvDBAdapter::NewL());
	MY_ASSERT_EQUALS(err, KErrNone);
	delete iMsvDbAdaptor;
	
	// Main DB exists.
	TRAP(err, iMsvDbAdaptor = CMsvDBAdapter::NewL());
	MY_ASSERT_EQUALS(err, KErrNone);
	delete iMsvDbAdaptor;
	
	// Main DB is not a DB.
	RSqlDatabase::Delete(dbName);
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CFileMan* file = CFileMan::NewL(fs);
	CleanupStack::PushL(file);
	err = file->Copy(KMsgTestIndexFile, dbName); //ignore error
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, iMsvDbAdaptor = CMsvDBAdapter::NewL());
	MY_ASSERT_EQUALS(err, KErrNone);
	delete iMsvDbAdaptor;
	
	// Main DB 
	RSqlDatabase::Delete(dbName);
	TRAP(err, iMsvDbAdaptor = CMsvDBAdapter::NewL());
	MY_ASSERT_EQUALS(err, KErrNone);
	fs.Close();
	CleanupStack::PopAndDestroy();
	}
	
	
void CTestDbAdapter::TestGetVisibleFolderId()
	{
	_LIT(KFunction, "TestGetVisibleFolderId");
	INFO_PRINTF1(KFunction);

	TMsvId visibleFolderId;
	TRAPD(err, iMsvDbAdaptor->GetVisibleFolderIdL(KErrNotFound, visibleFolderId));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolderId);
		
	TRAP(err, iMsvDbAdaptor->GetVisibleFolderIdL(KMsvRootIndexEntryId, visibleFolderId));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolderId);
		
	TRAP(err, iMsvDbAdaptor->GetVisibleFolderIdL(KFirstFreeEntryId, visibleFolderId));
	MY_ASSERT_EQUALS(err, KErrNotFound);
	
	TUint driveId = 1;
	TMsvId visibleFolder = KMsvGlobalInBoxIndexEntryId;
	TRAP(err, CreateEntryL((driveId<<28)|KFirstFreeEntryId, KMsvRootIndexEntryId, KUidMsvFolderEntry, visibleFolder, EFalse));
	MY_ASSERT_EQUALS(KErrNone, err);
	visibleFolder = KFirstFreeEntryId;
	TRAP(err, CreateEntryL((driveId<<28)|(KFirstFreeEntryId+1), KMsvGlobalInBoxIndexEntryId, KUidMsvFolderEntry, visibleFolder, EFalse));
	MY_ASSERT_EQUALS(KErrNone, err);
	
	TRAP(err, iMsvDbAdaptor->GetVisibleFolderIdL((driveId<<28)|KFirstFreeEntryId, visibleFolderId));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(visibleFolderId, KMsvGlobalInBoxIndexEntryId);
	TRAP(err, iMsvDbAdaptor->GetVisibleFolderIdL((driveId<<28)|(KFirstFreeEntryId+1), visibleFolderId));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(UnmaskTMsvId(visibleFolderId), KFirstFreeEntryId);
	MY_ASSERT_EQUALS(1, (TInt) GetDriveId(visibleFolderId));
	
	visibleFolder = KMsvGlobalInBoxIndexEntryId;	
	TRAP(err, CreateEntryL((driveId<<28)|KMsvDraftEntryId, KMsvRootIndexEntryId, KUidMsvFolderEntry, visibleFolder, ETrue));
	MY_ASSERT_EQUALS(KErrNone, err);
	visibleFolder = KFirstFreeEntryId;
	TRAP(err, CreateEntryL((driveId<<28)|(KFirstFreeEntryId+2), KMsvGlobalInBoxIndexEntryId, KUidMsvFolderEntry, visibleFolder, ETrue));
	MY_ASSERT_EQUALS(KErrNone, err);
	
	TRAP(err, iMsvDbAdaptor->GetVisibleFolderIdL((driveId<<28)|KMsvDraftEntryId, visibleFolderId));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(visibleFolderId, KMsvDraftEntryId);
	TRAP(err, iMsvDbAdaptor->GetVisibleFolderIdL((driveId<<28)|(KFirstFreeEntryId+2), visibleFolderId));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(UnmaskTMsvId(visibleFolderId), KFirstFreeEntryId+2);
	MY_ASSERT_EQUALS(1, (TInt) GetDriveId(visibleFolderId));
	}
	
	

#else		// #if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)

// TEST CODE NOT VALID POST PREQ 557.


void CTestDbAdapter::TestGetMaxTMsvId()
	{
	_LIT(KFunction, "TestGetMaxTMsvId");
	INFO_PRINTF1(KFunction);

	TInt err;
	TMsvId maxId;
	TMsvId visibleId;
	// No entry in database.
	TRAP(err, iMsvDbAdaptor->GetMaxTMsvIdL(maxId));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(maxId, (long)NULL);
	
	visibleId = KMsvRootIndexEntryIdValue;
	TRAP(err, CreateEntryL(KFirstFreeEntryId, KMsvRootIndexEntryIdValue, KUidMsvFolderEntry, visibleId));
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, iMsvDbAdaptor->GetMaxTMsvIdL(maxId));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(maxId, KFirstFreeEntryId);

	// Creating 100 entries
	for(int index=1; index<200; index+=2)
		{
		TRAP(err, CreateEntryL(KFirstFreeEntryId+index, KMsvRootIndexEntryIdValue, KUidMsvMessageEntry, visibleId));
		//Avoid using MY_ASSERT_EQUALS as it will clutter the log file.
		ASSERT_EQUALS(KErrNone, err);
		TRAP(err, iMsvDbAdaptor->GetMaxTMsvIdL(maxId));
		ASSERT_EQUALS(err, KErrNone);
		ASSERT_EQUALS(maxId, KFirstFreeEntryId+index)
		}
	
	TRAP(err, CreateEntryL(KFirstFreeEntryId+20, KMsvRootIndexEntryIdValue, KUidMsvFolderEntry, visibleId));
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, iMsvDbAdaptor->GetMaxTMsvIdL(maxId));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(maxId, KFirstFreeEntryId+199);
	
	TRAP(err, CreateEntryL(KFirstFreeEntryId+120, KMsvRootIndexEntryIdValue, KUidMsvFolderEntry, visibleId));
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, iMsvDbAdaptor->GetMaxTMsvIdL(maxId));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(maxId, KFirstFreeEntryId+199);

	TRAP(err, CreateEntryL(KFirstFreeEntryId+1720, KMsvRootIndexEntryIdValue, KUidMsvFolderEntry, visibleId));
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, iMsvDbAdaptor->GetMaxTMsvIdL(maxId));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(maxId, KFirstFreeEntryId+1720);
	
	TRAP(err, iMsvDbAdaptor->DeleteEntryL(KFirstFreeEntryId+120));
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, iMsvDbAdaptor->GetMaxTMsvIdL(maxId));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(maxId, KFirstFreeEntryId+1720);
	
	TRAP(err, iMsvDbAdaptor->DeleteEntryL(KFirstFreeEntryId+199));
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, iMsvDbAdaptor->GetMaxTMsvIdL(maxId));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(maxId, KFirstFreeEntryId+1720);
	
	TRAP(err, iMsvDbAdaptor->DeleteEntryL(KFirstFreeEntryId+1720));
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, iMsvDbAdaptor->GetMaxTMsvIdL(maxId));
	MY_ASSERT_EQUALS(err, KErrNone);
	MY_ASSERT_EQUALS(maxId, KFirstFreeEntryId+197);
	}



void CTestDbAdapter::TestTransaction()
	{
	_LIT(KFunction, "TestTransaction");
	INFO_PRINTF1(KFunction);
	
	TInt err;
	TMsvId visibleFolder;
	TFileName dbName(KTestDbFileName);
	// Creating entries.
	// 1. Create an entry using transaction.

	// (a) Open the transaction.
	TRAP(err, iMsvDbAdaptor->BeginTransactionL());	
	MY_ASSERT_EQUALS(KErrNone, err);
	// (b) Create the entry.
	visibleFolder = KMsvRootIndexEntryId;
	TRAP(err, CreateEntryL(KMsvRootIndexEntryId, KErrNotFound, KUidMsvRootEntry, visibleFolder, ETrue));
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolder);
	// (c) Commit the transaction.
	TRAP(err, iMsvDbAdaptor->CommitTransactionL());	
	MY_ASSERT_EQUALS(KErrNone, err);
	// (d) Check if the entry exists.
	MY_ASSERT_TRUE(EntryExists(KMsvRootIndexEntryId));
	// (e) Close the database.
	delete iMsvDbAdaptor;
	iMsvDbAdaptor = NULL;
	// (f) Open the database.
	TRAP(err, iMsvDbAdaptor = CMsvDBAdapter::OpenL(dbName));
	MY_ASSERT_EQUALS(err, KErrNone);
	// (g) Check if the entry exists.
	MY_ASSERT_TRUE(EntryExists(KMsvRootIndexEntryId));
	
	// 2. Create an entry and rollback.
	// (a) Open the transaction.
	TRAP(err, iMsvDbAdaptor->BeginTransactionL());	
	MY_ASSERT_EQUALS(KErrNone, err);
	// (b) Create the entry.
	TRAP(err, CreateEntryL(KFirstFreeEntryId, KMsvRootIndexEntryId, KUidMsvRootEntry, visibleFolder, ETrue));
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolder);
	MY_ASSERT_TRUE(EntryExists(KFirstFreeEntryId));
	// (c) Rollback the transaction.
	TRAP(err, iMsvDbAdaptor->RollbackTransactionL());	
	MY_ASSERT_EQUALS(KErrNone, err);
	// (d) Check if the entry exists.
	MY_ASSERT_TRUE(!EntryExists(KFirstFreeEntryId));
	// (e) Create the entry again.
	TRAP(err, iMsvDbAdaptor->BeginTransactionL());	
	MY_ASSERT_EQUALS(KErrNone, err);
	TRAP(err, CreateEntryL(KFirstFreeEntryId, KMsvRootIndexEntryId, KUidMsvRootEntry, visibleFolder, ETrue));
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS(KMsvRootIndexEntryId, visibleFolder);
	MY_ASSERT_TRUE(EntryExists(KFirstFreeEntryId));
	// (f) Close the database without performing rollback/commit.
	delete iMsvDbAdaptor;
	iMsvDbAdaptor = NULL;
	// (g) Open the database.
	TRAP(err, iMsvDbAdaptor = CMsvDBAdapter::OpenL(dbName));
	MY_ASSERT_EQUALS(err, KErrNone);
	// (h) Entry should not exist.
	MY_ASSERT_TRUE(!EntryExists(KFirstFreeEntryId));
	}


void CTestDbAdapter::TestDeletePerf()
	{
	_LIT(KFunction, "TestDeletePerf");
	INFO_PRINTF1(KFunction);
	
	
	//Performance-related tests
	CMsvEntrySelection* selection = NULL;
	TRAP_IGNORE(selection = new(ELeave) CMsvEntrySelection());
	TUint32 start, stop, diff2;
	TInt err;
	TMsvId visibleFolder;
	TInt bulkCount = 215;
	
	//Add 100 entries
	TRAP(err, CreateEntryL(KFirstFreeEntryId, KMsvRootIndexEntryId, KUidMsvMessageEntry, visibleFolder));
	MY_ASSERT_EQUALS(err, KErrNone);
	TRAP(err, selection->AppendL(KFirstFreeEntryId));
	MY_ASSERT_EQUALS(err, KErrNone);
	for(TInt index = 1; index < bulkCount; ++index)
		{
		TRAP(err, CreateEntryL(KFirstFreeEntryId+index, KFirstFreeEntryId, KUidMsvMessageEntry, visibleFolder));
		ASSERT_EQUALS(err, KErrNone);
		TRAP_IGNORE(selection->AppendL(KFirstFreeEntryId+index));		
		}
	
	//test incremental method
	start = User::FastCounter(); 
	TRAP(err, iMsvDbAdaptor->DeleteEntryL(*selection));
	stop = User::FastCounter();
	MY_ASSERT_EQUALS(err, KErrNone);
	diff2 = stop - start;
	for(TInt index = 0; index < bulkCount; ++index)
		{
		ASSERT_FALSE(EntryExists(selection->At(index)));
		}
	delete selection;
	}
	




void CTestDbAdapter::TestValidateDatabaseL()
	{
	_LIT(KFunction, "TestValidateDatabaseL");
	INFO_PRINTF1(KFunction);
	
	_LIT8(KDeleteVersion, "DELETE FROM VersionTable WHERE version = 1;");
	_LIT8(KInsertBadVersion, "INSERT INTO VersionTable VALUES (5);");
	
	// 1. Database is created with the correct version. Test it.
	// Note: We need to close the database that has been opened to test this.
	_LIT(KDbName, "\\messaging.db");
	TParse parse;
	TPtrC drive(TDriveUnit(RFs::GetSystemDrive()).Name());
	parse.Set(KDbName(), &drive, NULL);
	TFileName dbFile = parse.FullName();
	delete iMsvDbAdaptor;
	iMsvDbAdaptor = NULL;
	
	TRAPD(err, CMsvDBAdapter::ValidateDatabaseL(dbFile));
	MY_ASSERT_EQUALS(KErrNone, err);
	
	// 2. Delete the version and insert a bad version to test negative.
	// 2.0 Create a separate database for this test and
	//     create the version table to store the message store version.
	_LIT8(KCreateVersionTableQuery, "CREATE TABLE IF NOT EXISTS VersionTable ( "
									"version INTEGER PRIMARY KEY );"
		);
	_LIT(KTestDb, "\\testdb.db");
	parse.Set(KTestDb(), &drive, NULL);
	dbFile = parse.FullName();
	RSqlDatabase::Delete(dbFile); //delete the file it was created previously.
	
	_LIT8(TEXT, "encoding=UTF-8");
	HBufC8* heapBuf = NULL;
	heapBuf = TEXT().AllocLC();
	RSqlDatabase database;
	err = database.Create(dbFile, heapBuf);
	MY_ASSERT_EQUALS(err, KErrNone);
	
	database.Close();
	CleanupStack::PopAndDestroy(); //heapBuf
	
	CleanupClosePushL(database);
	TRAP(err, database.OpenL(dbFile));
	if(KErrNone != err)
		{
		User::Leave(err);
		}
	
	err = database.Exec(KCreateVersionTableQuery);
	if(err < 0)
		{
		User::Leave(err);
		}
	TPtrC errorMsg(database.LastErrorMessage());
	
	// 2.1 Insert the version number into the above table.
	err = database.Exec(KInsertBadVersion);
	if(err < 0)
		{
		User::Leave(err);
		}
	else if(!err)
		{
		User::Leave(KErrGeneral);
		}
	
	// 2.2 Check validity. Should not be supported.
	// Note: Database must be closed to test this.
	CleanupStack::PopAndDestroy();
	TRAP(err, CMsvDBAdapter::ValidateDatabaseL(dbFile));
	MY_ASSERT_EQUALS(KErrNotSupported, err);	
	}
	
	
	
#endif		// #if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)


	
CTestSuite* CTestDbAdapter::CreateSuiteL(const TDesC& aName)
	{
	SUB_SUITE;
	ADD_ASYNC_TEST_STEP(TestDatabaseBasics);
	ADD_ASYNC_TEST_STEP(TestCreateEntry);
	ADD_ASYNC_TEST_STEP(TestEntryExists);
	ADD_ASYNC_TEST_STEP(TestDeleteEntry);
	ADD_ASYNC_TEST_STEP(TestUpdateEntry);
	ADD_ASYNC_TEST_STEP(TestGetEntry);
	ADD_ASYNC_TEST_STEP(TestTransaction);
	ADD_ASYNC_TEST_STEP(TestDeleteBulkEntry);
	ADD_ASYNC_TEST_STEP(TestGetInPreparationId);
	ADD_ASYNC_TEST_STEP(TestUpdateVisibleFolder);
	ADD_ASYNC_TEST_STEP(TestUpdateOwnerStatus);
	ADD_ASYNC_TEST_STEP(TestGetChildren);
	ADD_ASYNC_TEST_STEP(TestGetChildrenCondL);

#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	ADD_ASYNC_TEST_STEP(TestValidateDatabase);
	ADD_ASYNC_TEST_STEP(TestMaxTMsvId);
	ADD_ASYNC_TEST_STEP(TestGetVisibleFlag);
	ADD_ASYNC_TEST_STEP(TestGetChildrenId);
	ADD_ASYNC_TEST_STEP(TestCreateRootEntry);
	ADD_ASYNC_TEST_STEP(TestDBPointerCreation);
	ADD_ASYNC_TEST_STEP(TestGetVisibleFolderId);
#else
	ADD_ASYNC_TEST_STEP(TestDeletePerf);
	ADD_ASYNC_TEST_STEP(TestGetMaxTMsvId);
	ADD_ASYNC_TEST_STEP(TestValidateDatabaseL);
#endif

	END_SUITE;
	}
