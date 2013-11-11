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
// t_msv_indexcontext_step.cpp
//
/**
 @file 
 @internalTechnology
*/
 
#include "t_msv_indexcontext_step.h"
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


#ifdef SYMBIAN_MESSAGESTORE_UNIT_TESTCODE
_LIT(KDbFileName, "\\messaging.db");
_LIT(KDBOpenFileName,"\\messaging.db");
#else
_LIT(KDbFileName, "[1000484B]messaging.db");
_LIT(KDBOpenFileName,"[1000484B]messaging.db");
#endif

_LIT(KMsgTestIndexFile, "\\private\\1000484b\\Index");
_LIT(KMsgTestIndexFile2, "\\private\\1000484b\\Index1");
_LIT(KMsgIndexFile, "\\private\\1000484b\\Mail2\\Index");
_LIT(KPreferredDriveListFilePath, "C:\\private\\1000484b\\msgprioritydrivelist.ini");
_LIT(KMsgTestPriorityDriveListFile2, "C:\\private\\1000484b\\msgprioritydrivelist2.ini");


CTestIndexContext::CTestIndexContext()
	{
	}



CTestIndexContext::~CTestIndexContext()
	{	
	}



/**
 * SetupL()
 * 
 * Starts up messaging server after creating drives in the following states:
 * 		- System drive: No message store exists.
 *		- EDriveD: No message store exists.
 *		- EDriveE: Old version database exists.
 *		- EDriveF: Old version Index file exists.
 */
void CTestIndexContext::SetupL()
	{
	_LIT(KFunction, "*_*SetupL");
	INFO_PRINTF1(KFunction);
	
	__UHEAP_MARK;
	
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CFileMan* file = CFileMan::NewL(fs);
	CleanupStack::PushL(file);
	
	// Delete database
	//	from EDriveC,
	TParse parse;
	TPtrC drive = TDriveUnit(RFs::GetSystemDrive()).Name();
	parse.Set(KDbFileName, &drive, NULL);
	TFileName dbName = parse.FullName();
	RSqlDatabase::Delete(dbName);
	parse.Set(KMsgIndexFile, &drive, NULL);
	file->Delete(parse.FullName());		// Ignore error.
	file->RmDir(parse.DriveAndPath());
	
	//	from EDriveD,
	drive.Set(TDriveUnit(EDriveD).Name());
	parse.Set(KDbFileName, &drive, NULL);
	dbName = parse.FullName();
	RSqlDatabase::Delete(dbName);
	parse.Set(KMsgIndexFile, &drive, NULL);
	file->Delete(parse.FullName());		// Ignore error.
	file->RmDir(parse.DriveAndPath());	
	//	from EDriveE, and
	drive.Set(TDriveUnit(EDriveE).Name());
	parse.Set(KDbFileName, &drive, NULL);
	dbName = parse.FullName();
	RSqlDatabase::Delete(dbName);
	//	from EDriveF.
	drive.Set(TDriveUnit(EDriveF).Name());
	parse.Set(KDbFileName, &drive, NULL);
	dbName = parse.FullName();
	RSqlDatabase::Delete(dbName);
	//	from EDriveG,
	drive.Set(TDriveUnit(EDriveG).Name());
	parse.Set(KDbFileName, &drive, NULL);
	dbName = parse.FullName();
	RSqlDatabase::Delete(dbName);
	parse.Set(KMsgIndexFile, &drive, NULL);
	file->Delete(parse.FullName());		// Ignore error.
	file->RmDir(parse.DriveAndPath());	
	//	from EDriveH,
	drive.Set(TDriveUnit(EDriveH).Name());
	parse.Set(KDbFileName, &drive, NULL);
	dbName = parse.FullName();
	RSqlDatabase::Delete(dbName);
	parse.Set(KMsgIndexFile, &drive, NULL);
	file->Delete(parse.FullName());		// Ignore error.
	file->RmDir(parse.DriveAndPath());	
	//	from EDriveI,
	drive.Set(TDriveUnit(EDriveI).Name());
	parse.Set(KDbFileName, &drive, NULL);
	dbName = parse.FullName();
	RSqlDatabase::Delete(dbName);
	parse.Set(KMsgIndexFile, &drive, NULL);
	file->Delete(parse.FullName());		// Ignore error.
	file->RmDir(parse.DriveAndPath());	

	TInt err = KErrNone;
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	// Placing an invalid database in EDriveE.
	drive.Set(TDriveUnit(EDriveE).Name());
	parse.Set(KDbFileName, &drive, NULL);
	err = fs.MkDirAll(parse.DriveAndPath()); //ignore error
	RSqlDatabase::Delete(parse.FullName());
	TRAP(err, CMsvDBAdapter::CreateDBL(EDriveE, ETrue));
	ASSERT_EQUALS(err, KErrNone);

	// Placing an old version Index file in EDriveF.
	drive.Set(TDriveUnit(EDriveF).Name());
	parse.Set(KMsgIndexFile, &drive, NULL);
	err = fs.MkDirAll(parse.DriveAndPath()); //ignore error
	TFileName index = parse.FullName();
	err = file->Copy(KMsgTestIndexFile, index); //ignore error
	
	drive.Set(TDriveUnit(RFs::GetSystemDrive()).Name());
	parse.Set(KPreferredDriveListFilePath, &drive, NULL);
	TFileName fileName = parse.FullName();
	err = file->Copy(KMsgTestPriorityDriveListFile2, fileName);
	MY_ASSERT_EQUALS(err, KErrNone);		
#endif

	CleanupStack::PopAndDestroy(file);
	fs.Close();			
	
	iServer=0;
	TRAP(err, iServer = CMsvServer::NewL());
	ASSERT_EQUALS(err, KErrNone);
	
	iContext = &(const_cast<CMsvIndexContext&>(iServer->Context()));
	}



void CTestIndexContext::TearDownL()
	{
	_LIT(KFunction, "*_*TearDownL");
	INFO_PRINTF1(KFunction);

#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)	
	iServer->ResetRepositoryL();
#endif
	
	delete iServer;
	iServer = 0;
	
	TParse parse;
	TPtrC drive = TDriveUnit(RFs::GetSystemDrive()).Name();
	parse.Set(KDbFileName, &drive, NULL);
	TFileName dbName = parse.FullName();
	RSqlDatabase::Delete(dbName);
	
	__UHEAP_MARKEND;
	}



#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)	
void CTestIndexContext::CreateEntryL(TDriveNumber aDriveNum, TMsvId aId, TMsvId aParentId)
	{
	_LIT(KCreateEntryQuery, "INSERT INTO IndexEntry VALUES (");
	_LIT(KComma, ", ");
	_LIT(KDelimiter, " );");
	
	RSqlDatabase db;
	TParse parse;
	TPtrC drive = TDriveUnit(aDriveNum).Name();
	parse.Set(KDbFileName, &drive, NULL);
	TFileName dbFile = parse.FullName();
	TRAPD(err, db.OpenL(dbFile));
	
	if(KErrNotFound == err)
		{
		db.Close();
		TRAP(err, CMsvDBAdapter::CreateDBL(aDriveNum, ETrue, ETrue));
		ASSERT_EQUALS(err, KErrNone);
		TRAP(err, db.OpenL(dbFile));
		ASSERT_EQUALS(err, KErrNone);
		}
		
	TInt driveId = aDriveNum;
	TMsvEntry entry;
	entry.iType = KUidMsvFolderEntry;
	entry.iMtm = KUidMsvFolderEntry;
	entry.iDate.UniversalTime();
	entry.SetId(aId); //
	entry.SetParent(aParentId);
	entry.iSize = 0;
	
	RBuf queryBuf;
	queryBuf.Create(300);
	CleanupClosePushL(queryBuf);
	queryBuf.Append(KCreateEntryQuery);
	queryBuf.AppendNum(entry.iId);
	queryBuf.Append(KComma);
	queryBuf.AppendNum(entry.Parent());
	queryBuf.Append(KComma);
	queryBuf.AppendNum(entry.iServiceId);
	queryBuf.Append(KComma);
	queryBuf.AppendNum(entry.iMtm.iUid);
	queryBuf.Append(KComma);
	queryBuf.AppendNum(entry.iType.iUid);
	queryBuf.Append(KComma);
	queryBuf.AppendNum(entry.iDate.Int64());
	queryBuf.Append(KComma);
	queryBuf.AppendNum(entry.iData);
	queryBuf.Append(KComma);
	queryBuf.AppendNum(entry.iSize);
	queryBuf.Append(KComma);
	queryBuf.AppendNum(entry.iError);
	queryBuf.Append(KComma);
	queryBuf.AppendNum(entry.iMtmData1);
	queryBuf.Append(KComma);
	queryBuf.AppendNum(entry.iMtmData2);
	queryBuf.Append(KComma);
	queryBuf.AppendNum(entry.iMtmData3);
	queryBuf.Append(KComma);
	queryBuf.AppendNum(entry.iRelatedId);
	queryBuf.Append(KComma);
	queryBuf.AppendNum(entry.iBioType);
	queryBuf.Append(KComma);
	queryBuf.AppendNum(entry.iPcSyncCount);
	queryBuf.Append(KComma);
	queryBuf.AppendNum(entry.iReserved);
	queryBuf.Append(KComma);
	queryBuf.AppendNum(entry.Parent());
	queryBuf.Append(KComma);
	queryBuf.Append(_L("\"\""));
	queryBuf.Append(KComma);
	queryBuf.Append(_L("\"\""));
	queryBuf.Append(KDelimiter);
	
	err = db.Exec(queryBuf);
	if(err < 0)
		{
		User::Leave(err);
		}
	else if(!err)
		{
		User::Leave(KErrGeneral);
		}
	CleanupStack::PopAndDestroy(); //queryBuf
	
	db.Close();
	
	}



void CTestIndexContext::TestCreateStoreDeleteFileL()
	{
	_LIT(KFunction, "TestCreateStoreDeleteFileL");
	INFO_PRINTF1(KFunction);
	
	_LIT(KStoreDeletedFile, "\\private\\1000484b\\storedeleted.tmp");
	
	TDriveNumber driveNum;
	
	// 1. Test on a valid drive number.
	// 1.0 Create the Mail2 folder.
	RFs dirCreator;
	User::LeaveIfError(dirCreator.Connect());
	driveNum = RFs::GetSystemDrive();
	TPtrC drive = TDriveUnit(driveNum).Name();
	TParse parse;
	parse.Set(KMsvDefaultIndexFile2, &drive, NULL);
	TFileName mail2Folder = parse.DriveAndPath();
	dirCreator.MkDirAll(mail2Folder);
	
	// 1.1 The test.
	iContext->CreateStoreDeleteFile(driveNum);
	
	// 1.2 Verification.
	parse.Set(KStoreDeletedFile, &drive, NULL);
	RFs& fs = iServer->FileSession();
	MY_ASSERT_TRUE(BaflUtils::FileExists(fs, parse.FullName()));
	
	dirCreator.Delete(parse.FullName());
	dirCreator.Close();
	
	// 2. Test on an invalid drive number.
	// 2.1 The test.
	driveNum = EDriveP;
	iContext->CreateStoreDeleteFile(driveNum);
	
	// 2.2 Verification.
	drive.Set(TDriveUnit(driveNum).Name());
	parse.Set(KStoreDeletedFile, &drive, NULL);
	//err = fs.SetSessionToPrivate(driveNum);
	//MY_ASSERT_TRUE(KErrNone != err);
	TBool val = BaflUtils::FileExists(fs, parse.FullName());
	MY_ASSERT_TRUE(!val);
	
	}
	
	
	
void CTestIndexContext::TestUpdateDriveStatusL()
	{
	_LIT(KFunction, "TestUpdateDriveStatusL");
	INFO_PRINTF1(KFunction);
	
	// Priority drive list config file has drives that do not exist
	// on the device. Make use of this to test UpdateDriveStatusL.
	
	/* A snapshot of msgprioritydrivelist.ini is:
	
	PriorityDrive1 = C
	PriorityDrive2 = D
	PriorityDrive3 = E
	PriorityDrive4 = F
	PriorityDrive5 = G
	PriorityDrive6 = H
	PriorityDrive7 = I
	*/
	
	/*
	Preferred drive list created below will be:
	 - System drive (EDriveC)
	 - EDriveD
	 - EDriveK
	 - EDriveE
	 - EDriveF
	*/
	
	TMsvPreferredDrive drive;
	CMsvPreferredDriveList* driveList = CMsvPreferredDriveList::GetDriveList();
	TUint driveIndex = 0;
	TInt err = KErrNone;
	
	// 1.
	// 1.1 First drive added to the driveList is EDriveC.
	//	   EDriveC will have database deleted in SetupL() above. But the status
	//	   should be EMsvMessageStoreAvailable as it will be created in
	//	   CreateIndexL().
	drive = (*driveList)[driveIndex];
	MY_ASSERT_EQUALS(RFs::GetSystemDrive(), drive.driveNum);
	MY_ASSERT_EQUALS(EMsvMessageStoreAvailableStatus, drive.status);
	MY_ASSERT_EQUALS((TUint)1, drive.driveId);
	
	// 1.2 Second drive added to the driveList is EDriveD. This does not have a
	//	   message store in it and will have status as EMsvMessageStoreUnavailable
	drive = (*driveList)[++driveIndex];
	MY_ASSERT_EQUALS(EDriveD, drive.driveNum);
	MY_ASSERT_EQUALS(EMsvMessageStoreUnavailableStatus, drive.status);
	MY_ASSERT_EQUALS(KMsvInvalidDriveId, drive.driveId);
	
	/*// 1.3 We add a drive that is not created on the device yet. Drive
	//	   status should be EMsvDriveMediaUnavailable.
	TUint priority = 3;
	//iServer->AddDriveToListL(EDriveK, priority);
	drive.driveNum = EDriveK;
	drive.driveId = KMsvInvalidDriveId;
	drive.status = EMsvDriveMediaUnavailable;
	iServer->iDriveList.InsertL(drive, priority-1);
	err = iServer->NextDrive(drive);
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS(EDriveK, drive.driveNum);
	MY_ASSERT_EQUALS(EMsvDriveMediaUnavailable, drive.status);
	MY_ASSERT_EQUALS(KMsvInvalidDriveId, drive.driveId);*/
	
	// 1.4 Drive that has a corrupt database. (no standard folders)
	//	   Drive status should be EMsvMessageStoreUnavailableStatus.
	drive = (*driveList)[++driveIndex];
	MY_ASSERT_EQUALS(EDriveE, drive.driveNum);
	MY_ASSERT_EQUALS(EMsvMessageStoreUnavailableStatus, drive.status);
	MY_ASSERT_EQUALS(KMsvInvalidDriveId, drive.driveId);
	
	// 1.4.1 Add standard folders to that database. The database version is
	//		 still the old one. So status should be EMsvMessageStoreNotSupported.
	// Note: We need to close the server to add the entries into that database
	//		 and restart it to get the drive info. (NextDrive x 3 also)
	delete iServer;
	iServer = 0;
	CreateEntryL(EDriveE, KMsvLocalServiceIndexEntryIdValue, KMsvRootIndexEntryIdValue);
	CreateEntryL(EDriveE, KMsvGlobalInBoxIndexEntryIdValue, KMsvLocalServiceIndexEntryIdValue);
	CreateEntryL(EDriveE, KMsvGlobalOutBoxIndexEntryIdValue, KMsvLocalServiceIndexEntryIdValue);
	CreateEntryL(EDriveE, KMsvDraftEntryIdValue, KMsvLocalServiceIndexEntryIdValue);
	CreateEntryL(EDriveE, KMsvSentEntryIdValue, KMsvLocalServiceIndexEntryIdValue);
	CreateEntryL(EDriveE, KMsvDeletedEntryFolderEntryIdValue, KMsvLocalServiceIndexEntryIdValue);
	
	TRAP(err, iServer = CMsvServer::NewL());
	ASSERT_EQUALS(err, KErrNone);
	driveList = CMsvPreferredDriveList::GetDriveList();
	iContext = &(const_cast<CMsvIndexContext&>(iServer->Context()));
	driveIndex = 2;
	drive = (*driveList)[driveIndex];
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS(EDriveE, drive.driveNum);
	MY_ASSERT_EQUALS(EMsvMessageStoreNotSupportedStatus, drive.status);
	MY_ASSERT_EQUALS(KMsvInvalidDriveId, drive.driveId);
	
		
	// 1.5 Drive that has an old version Index file.
	//	   Drive status should be EMsvMessageStoreNotSupported.
	drive = (*driveList)[++driveIndex];
	MY_ASSERT_EQUALS(EDriveF, drive.driveNum);
	MY_ASSERT_EQUALS(EMsvMessageStoreNotSupportedStatus, drive.status);
	MY_ASSERT_EQUALS(KMsvInvalidDriveId, drive.driveId);
	
	// 1.5.1 Drive that has a corrupted Index file.
	//		Drive status should be EMsvMessageStoreUnavailable.
	delete iServer;
	iServer = 0;
	
	RFs fs;
	User::LeaveIfError(fs.Connect());
	// Placing an old version Index file in EDriveF.
	TPtrC drivePtr(TDriveUnit(EDriveF).Name());
	TParse parse;
	parse.Set(KMsgIndexFile, &drivePtr, NULL);
	err = fs.MkDirAll(parse.DriveAndPath()); //ignore error
	CFileMan* file = CFileMan::NewL(fs);
	CleanupStack::PushL(file);
	TFileName index = parse.FullName();
	err = file->Copy(KMsgTestIndexFile2, index); //ignore error
	CleanupStack::PopAndDestroy(file);
	fs.Close();
	// Test
	TRAP(err, iServer = CMsvServer::NewL());
	ASSERT_EQUALS(err, KErrNone);
	driveList = CMsvPreferredDriveList::GetDriveList();
	driveIndex = 3;
	drive = (*driveList)[driveIndex];
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS(EDriveF, drive.driveNum);
	MY_ASSERT_EQUALS(EMsvMessageStoreUnavailableStatus, drive.status);
	MY_ASSERT_EQUALS(KMsvInvalidDriveId, drive.driveId);
	
	}
	
	
	
void CTestIndexContext::TestChangeDriveL()
	{
	_LIT(KFunction, "TestChangeDriveL");
	INFO_PRINTF1(KFunction);
	
	TMsvPreferredDrive drive;
	TUint driveIndex = 0;
	TInt err = KErrNone;
	CMsvIndexAdapter* indexAdapter = iContext->IndexAdapter();
	TUint driveId = 0;
	TBool ret = EFalse;
	TMsvEntry entry;
	TSecureId owner = 0x999;
	CMsvCacheEntry* cacheEntry = NULL;
	
	TPtrC drivePtr(NULL, 0);
	TParse parse;
	
	// 1. Change drive to a drive with status EMsvMessageStoreUnavailableStatus.
	//Create a non-standard folder under Local in driveId 1.
	driveId = 1;
	entry.iId = MaskTMsvId(driveId, KFirstFreeEntryId + 1);
	entry.iParentId = KMsvLocalServiceIndexEntryIdValue;
	entry.SetVisible(ETrue);
	// Creating entry in C:
	err = indexAdapter->AddEntry(entry, owner, EFalse);
	MY_ASSERT_EQUALS(KErrNone, err);
	ret = indexAdapter->FindEntryInCache(KMsvGlobalInBoxIndexEntryId, cacheEntry);
	MY_ASSERT_TRUE(ret);
	ret = indexAdapter->FindEntryInCache(MaskTMsvId(driveId, KFirstFreeEntryId + 1), cacheEntry);
	MY_ASSERT_TRUE(ret);
	
	// Change drive.
	err = CMsvPreferredDriveList::GetDriveList()->GetDriveIndex(EDriveD, driveIndex);
	MY_ASSERT_EQUALS(KErrNone, err);
	err = iContext->ChangeDrive(driveIndex, ETrue);
	MY_ASSERT_EQUALS(KErrNone, err);
	CMsvPreferredDriveList* iDriveList = CMsvPreferredDriveList::GetDriveList();
	TMsvPreferredDrive driveEntry = (*iDriveList)[driveIndex];
	iDriveList->Remove(driveIndex);
	iDriveList->Insert(driveEntry, 0);

	// D: is the current drive.
	driveIndex = 0;
	CMsvPreferredDriveList::GetDriveList()->SetCurrentDriveIndex(driveIndex);
	CMsvPreferredDriveList::GetDriveList()->DriveInfoL(driveIndex, drive);
	MY_ASSERT_EQUALS(drive.driveNum, EDriveD);
	MY_ASSERT_EQUALS(drive.driveId, (TUint)1);
	MY_ASSERT_EQUALS(drive.status, EMsvMessageStoreAvailableStatus);
	
	// Check cache.
	driveId = 1;
	ret = indexAdapter->FindEntryInCache(KMsvGlobalInBoxIndexEntryId, cacheEntry);
	MY_ASSERT_TRUE(ret); //Inbox from driveId 1
	ret = indexAdapter->FindEntryInCache(MaskTMsvId(driveId, KFirstFreeEntryId + 1), cacheEntry);
	MY_ASSERT_FALSE(ret); //non-standard folder from driveId 1
		
	// 2. Change drive to a drive with a corrupted database.
	//Create some non-standard folders in driveId 2 before the drive change.
	driveId = 1;
	entry.iId = MaskTMsvId(driveId, KFirstFreeEntryId + 2);
	entry.iParentId = KMsvLocalServiceIndexEntryIdValue;
	entry.SetVisible(ETrue);
	// Creating entry in D:
	err = indexAdapter->AddEntry(entry, owner, EFalse);
	MY_ASSERT_EQUALS(KErrNone, err);
	ret = indexAdapter->FindEntryInCache(KMsvGlobalInBoxIndexEntryId, cacheEntry);
	MY_ASSERT_TRUE(ret);
	ret = indexAdapter->FindEntryInCache(MaskTMsvId(driveId, KFirstFreeEntryId + 2), cacheEntry);
	MY_ASSERT_TRUE(ret);
	
	// Change drive.
	//Note: EDriveE had a corrupt database.
	err = CMsvPreferredDriveList::GetDriveList()->GetDriveIndex(EDriveE, driveIndex);
	MY_ASSERT_EQUALS(KErrNone, err);
	err = iContext->ChangeDrive(driveIndex, ETrue);
	MY_ASSERT_EQUALS(KErrNone, err);
	driveEntry = (*iDriveList)[driveIndex];
	iDriveList->Remove(driveIndex);
	iDriveList->Insert(driveEntry, 0);
		
	// E: is the current drive.
	driveIndex = 0;
	CMsvPreferredDriveList::GetDriveList()->SetCurrentDriveIndex(driveIndex);
	CMsvPreferredDriveList::GetDriveList()->DriveInfoL(driveIndex, drive);
	MY_ASSERT_EQUALS(drive.driveNum, EDriveE);
	MY_ASSERT_EQUALS(drive.driveId, (TUint)1);
	MY_ASSERT_EQUALS(drive.status, EMsvMessageStoreAvailableStatus);
	
	// Check cache.
	driveId = 1;
	ret = indexAdapter->FindEntryInCache(KMsvGlobalInBoxIndexEntryId, cacheEntry);
	MY_ASSERT_TRUE(ret);
	ret = indexAdapter->FindEntryInCache(MaskTMsvId(driveId, KFirstFreeEntryId + 2), cacheEntry);
	MY_ASSERT_FALSE(ret); //non-standard folder from driveId 2
		
	// 3. Change to a drive with message store available. Keep old drive in the list.
	//Create some non-standard folders in driveId 3 before the drive change.
	driveId = 1;
	entry.iId = MaskTMsvId(driveId, KFirstFreeEntryId + 3);
	entry.iParentId = KMsvLocalServiceIndexEntryIdValue;
	entry.SetVisible(ETrue);
	// Create entry in E:
	err = indexAdapter->AddEntry(entry, owner, EFalse);
	MY_ASSERT_EQUALS(KErrNone, err);
	ret = indexAdapter->FindEntryInCache(KMsvGlobalInBoxIndexEntryId, cacheEntry);
	MY_ASSERT_TRUE(ret);
	ret = indexAdapter->FindEntryInCache(MaskTMsvId(driveId, KFirstFreeEntryId + 3), cacheEntry);
	MY_ASSERT_TRUE(ret);
	
	// Change drive.
	err = CMsvPreferredDriveList::GetDriveList()->GetDriveIndex(EDriveC, driveIndex);
	MY_ASSERT_EQUALS(KErrNone, err);
	err = iContext->ChangeDrive(driveIndex, ETrue);
	MY_ASSERT_EQUALS(KErrNone, err);
	driveEntry = (*iDriveList)[driveIndex];
	iDriveList->Remove(driveIndex);
	iDriveList->Insert(driveEntry, 0);
	
	// C: is the current drive.
	driveIndex = 0;
	CMsvPreferredDriveList::GetDriveList()->SetCurrentDriveIndex(driveIndex);
	CMsvPreferredDriveList::GetDriveList()->DriveInfoL(driveIndex, drive);
	MY_ASSERT_EQUALS(drive.driveNum, EDriveC);
	MY_ASSERT_EQUALS(drive.driveId, (TUint)1);
	MY_ASSERT_EQUALS(drive.status, EMsvMessageStoreAvailableStatus);
	
	// Check cache.
	driveId = 1;
	ret = indexAdapter->FindEntryInCache(KMsvGlobalInBoxIndexEntryId, cacheEntry);
	MY_ASSERT_TRUE(ret);
	ret = indexAdapter->FindEntryInCache(MaskTMsvId(driveId, KFirstFreeEntryId + 3), cacheEntry);
	MY_ASSERT_FALSE(ret); //non-standard folder from driveId 2
	// Check entry created under C:
	ret = indexAdapter->FindEntryInCache(MaskTMsvId(driveId, KFirstFreeEntryId + 1), cacheEntry);
	MY_ASSERT_TRUE(ret);
	
	
	// 4. Change drive to a drive with message store available. Remove older drive from the list.
	//Note: change to EDriveD and then to C as we should not remove EDriveC. 
	//		While changing to C the second time, remove D from the drive list.
	err = CMsvPreferredDriveList::GetDriveList()->GetDriveIndex(EDriveD, driveIndex);
	MY_ASSERT_EQUALS(KErrNone, err);
	err = iContext->ChangeDrive(driveIndex, ETrue);
	MY_ASSERT_EQUALS(KErrNone, err);
	driveEntry = (*iDriveList)[driveIndex];
	iDriveList->Remove(driveIndex);
	iDriveList->Insert(driveEntry, 0);
	
	// D: is the current drive.
	driveIndex = 0;
	CMsvPreferredDriveList::GetDriveList()->SetCurrentDriveIndex(driveIndex);
	err = CMsvPreferredDriveList::GetDriveList()->GetDriveIndex(EDriveC, driveIndex);
	MY_ASSERT_EQUALS(KErrNone, err);
	
	driveId = 1;
	ret = indexAdapter->FindEntryInCache(KMsvGlobalInBoxIndexEntryId, cacheEntry);
	MY_ASSERT_TRUE(ret);
	// Check entry created under D:
	ret = indexAdapter->FindEntryInCache(MaskTMsvId(driveId, KFirstFreeEntryId + 2), cacheEntry);
	MY_ASSERT_TRUE(ret); //non-standard folder from driveId 2
	// Check entry created under C:
	ret = indexAdapter->FindEntryInCache(MaskTMsvId(driveId, KFirstFreeEntryId + 1), cacheEntry);
	MY_ASSERT_FALSE(ret); //non-standard folder from driveId 1
	
	// Change drive to C:
	err = iContext->ChangeDrive(driveIndex, EFalse); //the actual call to be tested
	MY_ASSERT_EQUALS(KErrNone, err);
	driveEntry = (*iDriveList)[driveIndex];
	iDriveList->Remove(driveIndex);
	iDriveList->Insert(driveEntry, 0);
	
	// C: is the current drive.
	driveIndex = 0;
	CMsvPreferredDriveList::GetDriveList()->SetCurrentDriveIndex(driveIndex);
	driveIndex = CMsvPreferredDriveList::GetDriveList()->CurrentDriveIndex();
	CMsvPreferredDriveList::GetDriveList()->DriveInfoL(driveIndex, drive);
	MY_ASSERT_EQUALS(drive.driveNum, EDriveC);
	MY_ASSERT_EQUALS(drive.driveId, (TUint)1);
	MY_ASSERT_EQUALS(drive.status, EMsvMessageStoreAvailableStatus);
	
	// Check cache.
	driveId = 1;
	ret = indexAdapter->FindEntryInCache(KMsvGlobalInBoxIndexEntryId, cacheEntry);
	MY_ASSERT_TRUE(ret); //standard folder from driveId 1
	ret = indexAdapter->FindEntryInCache(MaskTMsvId(driveId, KFirstFreeEntryId + 1), cacheEntry);
	MY_ASSERT_TRUE(ret); //non-standard folder from driveId 1
	ret = indexAdapter->FindEntryInCache(MaskTMsvId(driveId, KFirstFreeEntryId + 2), cacheEntry);
	MY_ASSERT_FALSE(ret); //non-standard folder from driveId 2	
	}



void CTestIndexContext::TestGetInPreparationIdsL()
	{
	_LIT(KFunction, "TestGetInPreparationIdsL");
	INFO_PRINTF1(KFunction);
	
	TMsvEntry entry;
	TUint driveId = 0;
	TUint driveIndex = 0;
	TInt err = KErrNone;
	TUint priority = 0;
	TSecureId owner = 0x999;
	CMsvIndexAdapter* indexAdapter = iContext->IndexAdapter();
	
	
	// 1. Get prepared entry Ids from valid current drive.
	//Note: Create 2 entries being prepared before the test.
	driveId = 1;
	entry.iId = MaskTMsvId(driveId, KFirstFreeEntryId + 1);
	entry.iParentId = MaskTMsvId(driveId, KMsvGlobalInBoxIndexEntryId);
	entry.SetInPreparation(ETrue);
	err = indexAdapter->AddEntry(entry, owner, EFalse); //entry#1
	MY_ASSERT_EQUALS(KErrNone, err);
	entry.iId = MaskTMsvId(driveId, KFirstFreeEntryId + 2);
	err = indexAdapter->AddEntry(entry, owner, EFalse); //entry#2
	MY_ASSERT_EQUALS(KErrNone, err);
	
	iContext->GetInPreparationIds(driveId);
	MY_ASSERT_EQUALS(iContext->Remove().Count(), 2);
	
	
	// 2. Get prepated Ids from non-current drive.
	//Note: We test this by making EDriveD the current drive.
	priority = 1;
	TRAP(err, iServer->UpdateDrivePriorityL(EDriveD, priority));
	MY_ASSERT_EQUALS(KErrNone, err);
	
	iContext->GetInPreparationIds(driveId);
	MY_ASSERT_TRUE(iContext->Remove().Count()!=2);
	
	iContext->GetInPreparationIds(2);
	MY_ASSERT_EQUALS(iContext->Remove().Count(), 2);
	}



void CTestIndexContext::TestLocalizeStandardFoldersL()
	{
	_LIT(KFunction, "TestLocalizeStandardFoldersL");
	INFO_PRINTF1(KFunction);
	
	TInt err = KErrNone;
	TFileName messageFolder;
	TFileName localServiceFolder;
	TBool found = EFalse;
	CMsvIndexAdapter* indexAdapter = NULL;
	CMsvCacheEntry* cacheEntry = NULL;
	TUint driveId = 0;
	
	// 1. Test creation of the message entries and folders in message store.
	//Note: We first delete the server, reset everything and start the server again.
	//		The message folder with folders for standard folders should be created.
	iServer->ResetRepositoryL();
	delete iServer;
	iServer = NULL;
	TParse parse;
	TPtrC drive = TDriveUnit(RFs::GetSystemDrive()).Name();
	parse.Set(KDbFileName, &drive, NULL);
	TFileName dbName = parse.FullName();
	RSqlDatabase::Delete(dbName);
	
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CFileMan* fileMan = CFileMan::NewL(fs);
	parse.Set(KMsvDefaultIndexFile2(), &drive, NULL);
	messageFolder = parse.DriveAndPath(); // \mail2
	localServiceFolder = messageFolder; // \mail2\00001001_S
	localServiceFolder.Append(_L("00001001_S\\"));
	MY_ASSERT_TRUE(BaflUtils::FolderExists(fs, messageFolder));
	MY_ASSERT_TRUE(BaflUtils::FolderExists(fs, localServiceFolder));
	
	(void)fileMan->Attribs(messageFolder, 0, KEntryAttReadOnly, TTime(0), CFileMan::ERecurse); //remove readonly
	err = fileMan->RmDir(messageFolder);
	delete fileMan;
	
	//Everything has been wiped in EDriveC.
	MY_ASSERT_FALSE(BaflUtils::FolderExists(fs, messageFolder));
	MY_ASSERT_FALSE(BaflUtils::FolderExists(fs, localServiceFolder));
	
	//On starting the server again, the entries and folders should be created.
	TRAP(err, iServer = CMsvServer::NewL());
	MY_ASSERT_EQUALS(KErrNone, err);
	iContext = &(const_cast<CMsvIndexContext&>(iServer->Context()));
	indexAdapter = iContext->IndexAdapter();
	MY_ASSERT_TRUE(BaflUtils::FolderExists(fs, messageFolder));
	MY_ASSERT_TRUE(indexAdapter->FindEntryInCache(KMsvLocalServiceIndexEntryIdValue, cacheEntry));
	MY_ASSERT_TRUE(BaflUtils::FolderExists(fs, localServiceFolder));
	MY_ASSERT_TRUE(indexAdapter->FindEntryInCache(KMsvGlobalInBoxIndexEntryIdValue, cacheEntry));
	MY_ASSERT_TRUE(indexAdapter->FindEntryInCache(KMsvGlobalOutBoxIndexEntryIdValue, cacheEntry));
	MY_ASSERT_TRUE(indexAdapter->FindEntryInCache(KMsvDraftEntryIdValue, cacheEntry));
	MY_ASSERT_TRUE(indexAdapter->FindEntryInCache(KMsvSentEntryIdValue, cacheEntry));
	MY_ASSERT_TRUE(indexAdapter->FindEntryInCache(KMsvDeletedEntryFolderEntryIdValue, cacheEntry));
	
	
	// 2. Test when there are some standard folders missing.
	driveId = 1;
	err = indexAdapter->LockEntry(KMsvGlobalInBoxIndexEntryIdValue);
	MY_ASSERT_EQUALS(KErrNone, err);
	err = indexAdapter->DeleteEntry(KMsvGlobalInBoxIndexEntryIdValue);
	MY_ASSERT_EQUALS(KErrNone, err);
	err = indexAdapter->LockEntry(KMsvGlobalOutBoxIndexEntryIdValue);
	MY_ASSERT_EQUALS(KErrNone, err);
	err = indexAdapter->DeleteEntry(KMsvGlobalOutBoxIndexEntryIdValue);
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_FALSE(indexAdapter->FindEntryInCache(KMsvGlobalInBoxIndexEntryIdValue, cacheEntry));
	MY_ASSERT_FALSE(indexAdapter->FindEntryInCache(KMsvGlobalOutBoxIndexEntryIdValue, cacheEntry));
	
	TRAP(err, iContext->LocalizeStandardFoldersL());
	MY_ASSERT_EQUALS(KErrNone, err);
	
	MY_ASSERT_TRUE(indexAdapter->FindEntryInCache(KMsvGlobalInBoxIndexEntryIdValue, cacheEntry));
	MY_ASSERT_TRUE(indexAdapter->FindEntryInCache(KMsvGlobalOutBoxIndexEntryIdValue, cacheEntry));
	}



void CTestIndexContext::TestProgress()
	{
	_LIT(KFunction, "TestProgress");
	INFO_PRINTF1(KFunction);
	
	// When indexAdaptor not null.
	iContext->Progress();
	
	// When indexAdaptor is NULL.
	CMsvIndexAdapter* adapter = iContext->iIndexAdapter;
	iContext->iIndexAdapter = NULL;
	iContext->Progress();
	iContext->iIndexAdapter = adapter;
	}
	
	
	
	
#else			// #if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)	


void CTestIndexContext::TestValidationL()
	{
	_LIT(KFunction, "TestValidationL");
	INFO_PRINTF1(KFunction);
	
	/**
	 * For the tests that follow, we need to: 
	 * 1. Test when database is present.
	 * - start server (this will create the database for us)
	 * - test if everything went fine.
	 * [NOTE: Database with a different version has been tested at the
	 *  lower level in CTestDbAdapter.]
	 *
	 * 2. Test when old index file is present.
	 * - close the server;
	 * - delete the database, if any;
	 * - copy test files (mail2 folder and index file) to the proper location;
	 * - start the server again to catch the error thrown.
	 */ 
	
	// 1. Test when database is present.
	//		Database has already been created when the server was started
	//		in SetupL(). We only need to check validity. But this can be checked
	//		only if the server is closed. (Database validity is checked 
	//		before it has been opened by the server in CMsvIndexContext.)
	delete iServer;
	iServer = 0;
	
	_LIT(KDbName, "\\messaging.db");
	TParse parse;
	TPtrC drive(TDriveUnit(RFs::GetSystemDrive()).Name());
	parse.Set(KDbName, &drive, NULL);
	TFileName dbFile = parse.FullName();
	TRAPD(err, CMsvDBAdapter::ValidateDatabaseL(dbFile));
	MY_ASSERT_EQUALS(KErrNone, err);
	
	
	
	// 2. Test when old index file is present.
	//		Close the server, delete DB (if any), copy test files to the 
	//		mail2 folder and start the server again. An error will be thrown.
	RFs fs;
	err = fs.Connect();
	MY_ASSERT_EQUALS(err, KErrNone);
	err = fs.Replace(KMsgTestIndexFile, KMsgIndexFile);
	MY_ASSERT_EQUALS(err, KErrNone);
	RSqlDatabase::Delete(dbFile);
	
	TRAP(err, iServer = CMsvServer::NewL());
	MY_ASSERT_EQUALS(err, KErrNotSupported);
	
	err = fs.Replace(KMsgIndexFile, KMsgTestIndexFile);
	MY_ASSERT_EQUALS(err, KErrNone);
	delete iServer;
	iServer = 0;
	
	
	// 3. Test when old corrupt index file is present.
	//		There will be no error thrown as we go ahead with database
	//		creation after deleting the old corrupt index file.
	CFileMan* fileMan = CFileMan::NewL(fs);
	CleanupStack::PushL(fileMan);
	fileMan->Copy(KMsgTestIndexFile2, KMsgIndexFile);
	TRAP(err, iServer = CMsvServer::NewL());
	MY_ASSERT_EQUALS(err, KErrNone);
	
	CleanupStack::PopAndDestroy(); //fileMan
	fs.Close();	
	}

#endif		// #if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)	

	


CTestSuite* CTestIndexContext::CreateSuiteL(const TDesC& aName)
// static
	{
	SUB_SUITE;
	
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	ADD_ASYNC_TEST_STEP(TestCreateStoreDeleteFileL);
	ADD_ASYNC_TEST_STEP(TestUpdateDriveStatusL);
	ADD_ASYNC_TEST_STEP(TestChangeDriveL);
	ADD_ASYNC_TEST_STEP(TestGetInPreparationIdsL);
	ADD_ASYNC_TEST_STEP(TestLocalizeStandardFoldersL);
	ADD_ASYNC_TEST_STEP(TestProgress);
#else
	ADD_ASYNC_TEST_STEP(TestValidationL);
#endif
	END_SUITE;
	}

