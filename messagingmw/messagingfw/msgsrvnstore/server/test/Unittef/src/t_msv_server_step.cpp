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
// t_msv_server_step.cpp
//
/**
 @file 
 @internalTechnology
*/
 
#include "t_msv_server_step.h"
#include <msvapi.h>
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

_LIT(KBaseDBFileName, "C:\\messaging_base.db");
_LIT(KMsgTestIndexFile, "\\private\\1000484b\\Index");
_LIT(KMsgIndexFile, "\\private\\1000484b\\Mail2\\Index");
_LIT(KMsgOldIndexFile, "\\private\\1000484b\\Mail\\Index");
_LIT(KMsgMail2Folder, "\\private\\1000484b\\Mail2\\");

_LIT(KPreferredDriveListFilePath, "\\private\\1000484b\\msgprioritydrivelist.ini");
_LIT(KMsgTestPriorityDriveListFile, "C:\\private\\1000484b\\msgprioritydrivelist.ini"); //empty config file
_LIT(KMsgTestPriorityDriveListFile1, "C:\\private\\1000484b\\msgprioritydrivelist1.ini"); //empty config file
_LIT(KMsgTestPriorityDriveListFile2, "C:\\private\\1000484b\\msgprioritydrivelist2.ini"); //normal config file
_LIT(KMsgTestPriorityDriveListFile3, "C:\\private\\1000484b\\msgprioritydrivelist3.ini"); //no system drive
_LIT(KMsgTestPriorityDriveListFile4, "C:\\private\\1000484b\\msgprioritydrivelist4.ini"); //>7 drives
_LIT(KMsgTestPriorityDriveListFile5, "C:\\private\\1000484b\\msgprioritydrivelist5.ini"); //<7 drives
_LIT(KMsgTestPriorityDriveListFile6, "C:\\private\\1000484b\\msgprioritydrivelist6.ini"); //repetitions
_LIT(KMsgTestPriorityDriveListFile7, "C:\\private\\1000484b\\msgprioritydrivelist7.ini"); //invalid keys
_LIT(KMsgTestPriorityDriveListFile10, "C:\\private\\1000484b\\msgprioritydrivelist10.ini"); //invalid keys

const TInt KUidConifgFileValue = 0x10286a26;
const TUid KUidConfigFile = {KUidConifgFileValue};


CTestMsvServer::CTestMsvServer()
	{
	}



CTestMsvServer::~CTestMsvServer()
	{   
	}



void CTestMsvServer::SetupL()
	{
	_LIT(KFunction, "*_*SetupL");
	INFO_PRINTF1(KFunction);
	
	__UHEAP_MARK;

	RFs fs;
	User::LeaveIfError(fs.Connect());
	CFileMan* file = CFileMan::NewL(fs);
	CleanupStack::PushL(file);
	
	// Delete database
	//  from default system drive (C: as of now),
	TParse parse;
	TPtrC drive = TDriveUnit(RFs::GetSystemDrive()).Name();
	parse.Set(KDbFileName, &drive, NULL);
	TFileName dbName = parse.FullName();
	RSqlDatabase::Delete(dbName);
	//  from EDriveD,
	drive.Set(TDriveUnit(EDriveD).Name());
	parse.Set(KDbFileName, &drive, NULL);
	dbName = parse.FullName();
	RSqlDatabase::Delete(dbName);
	parse.Set(KMsgMail2Folder, &drive, NULL);
	file->RmDir(parse.DriveAndPath());
	//  from EDriveE,
	drive.Set(TDriveUnit(EDriveE).Name());
	parse.Set(KDbFileName, &drive, NULL);
	dbName = parse.FullName();
	RSqlDatabase::Delete(dbName);
	parse.Set(KMsgMail2Folder, &drive, NULL);
	file->RmDir(parse.DriveAndPath());
	//  from EDriveF,
	drive.Set(TDriveUnit(EDriveF).Name());
	parse.Set(KDbFileName, &drive, NULL);
	dbName = parse.FullName();
	RSqlDatabase::Delete(dbName);
	parse.Set(KMsgMail2Folder, &drive, NULL);
	file->RmDir(parse.DriveAndPath());
	//  from EDriveJ,
	drive.Set(TDriveUnit(EDriveJ).Name());
	parse.Set(KDbFileName, &drive, NULL);
	dbName = parse.FullName();
	RSqlDatabase::Delete(dbName);
	parse.Set(KMsgMail2Folder, &drive, NULL);
	file->RmDir(parse.DriveAndPath());
	//  from EDriveK,
	drive.Set(TDriveUnit(EDriveK).Name());
	parse.Set(KDbFileName, &drive, NULL);
	dbName = parse.FullName();
	RSqlDatabase::Delete(dbName);
	parse.Set(KMsgMail2Folder, &drive, NULL);
	file->RmDir(parse.DriveAndPath());
	//  from EDriveL, and
	drive.Set(TDriveUnit(EDriveL).Name());
	parse.Set(KDbFileName, &drive, NULL);
	dbName = parse.FullName();
	RSqlDatabase::Delete(dbName);
	parse.Set(KMsgMail2Folder, &drive, NULL);
	file->RmDir(parse.DriveAndPath());
	//  from EDriveM.
	drive.Set(TDriveUnit(EDriveM).Name());
	parse.Set(KDbFileName, &drive, NULL);
	dbName = parse.FullName();
	RSqlDatabase::Delete(dbName);
	parse.Set(KMsgMail2Folder, &drive, NULL);
	file->RmDir(parse.DriveAndPath());
	
	// Placing an invalid database in EDriveE.
	drive.Set(TDriveUnit(EDriveE).Name());
	parse.Set(KDbFileName, &drive, NULL);
	TInt err = fs.MkDirAll(parse.DriveAndPath()); //ignore error
	RSqlDatabase::Delete(parse.FullName());
	TRAP(err, CMsvDBAdapter::CreateDBL(EDriveE, ETrue));
	ASSERT_EQUALS(err, KErrNone);
	
	// Placing an old version Index file in EDriveF.
	drive.Set(TDriveUnit(EDriveF).Name());
	parse.Set(KMsgMail2Folder, &drive, NULL);
	err = fs.MkDirAll(parse.DriveAndPath()); //ignore error
	TFileName index = parse.FullName();
	err = file->Copy(KMsgTestIndexFile, index); //ignore error
		
	err = file->Copy(KMsgTestPriorityDriveListFile10, KMsgTestPriorityDriveListFile); //ignore error
	CleanupStack::PopAndDestroy(file);
	fs.Close();
	
	CMsvServer::ResetRepositoryL();
	
	iServer=0;
	TRAP(err, iServer = CMsvServer::NewL());
	ASSERT_EQUALS(err, KErrNone);
	
	iContext = &(const_cast<CMsvIndexContext&>(iServer->Context()));
	iStoreManager = iServer->iServerStoreManager;
	iActiveWaiter = new(ELeave) CActiveWaiter();
	}



void CTestMsvServer::TearDownL()
	{
	_LIT(KFunction, "*_*TearDownL");
	INFO_PRINTF1(KFunction);
	
	iServer->ResetRepositoryL();
	
	delete iServer;
	iServer = 0;
	
	delete iActiveWaiter;
	TParse parse;
	TPtrC drive = TDriveUnit(RFs::GetSystemDrive()).Name();
	parse.Set(KDbFileName, &drive, NULL);
	TFileName dbName = parse.FullName();
	RSqlDatabase::Delete(dbName);
	
	// restore the msgprioritydrivelist.ini file
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CFileMan* file = CFileMan::NewL(fs);
	CleanupStack::PushL(file);
	TInt err = file->Copy(KMsgTestPriorityDriveListFile2, KMsgTestPriorityDriveListFile); //ignore error
	CleanupStack::PopAndDestroy(file);
	fs.Close();
	
	__UHEAP_MARKEND;
	}



void CTestMsvServer::CreateEntryL(TDriveNumber aDriveNum, TMsvId aId)
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
	
	TInt driveId = aDriveNum;
	TMsvEntry entry;
	entry.iType = KUidMsvFolderEntry;
	entry.iMtm = KUidMsvFolderEntry;
	entry.iDate.UniversalTime();
	entry.SetId(aId); //
	entry.SetParent(KMsvGlobalInBoxIndexEntryIdValue);
	entry.iSize = 0;
	entry.iData = 65536;
		
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



void CTestMsvServer::TestAddDriveToListL()
	{
	/* A snapshot of msgprioritydrivelist.ini is:
	
	PriorityDrive1 = C
	PriorityDrive2 = D
	PriorityDrive3 = E
	PriorityDrive4 = F
	PriorityDrive5 = P
	PriorityDrive6 = Q
	PriorityDrive7 = R
	*/
	
	_LIT(KFunction, "TestAddDriveToListL");
	INFO_PRINTF1(KFunction);
	
	TMsvPreferredDrive drive;
	TUint driveIndex = 0;
	TInt driveCount = CMsvPreferredDriveList::GetDriveList()->Count();
	MY_ASSERT_EQUALS(4, driveCount); //C, D, E and F are initially present in the drive list.
	
		
	// 1. Add drive which does not exist on the system.
	TUint priority = 1;
	TRAPD(err, iServer->AddDriveToListL(EDriveP, priority, NULL));
	MY_ASSERT_EQUALS(KErrNotFound, err);
	MY_ASSERT_EQUALS(iServer->iNotification, EMsvNullNotification);
	
	
	// 2. Add drive with an invalid priority.
	priority = 10;
	TRAP(err, iServer->AddDriveToListL(RFs::GetSystemDrive(), priority, NULL));
	MY_ASSERT_EQUALS(KErrArgument, err);
	MY_ASSERT_EQUALS(iServer->iNotification, EMsvNullNotification);
	
	priority = 0;
	TRAP(err, iServer->AddDriveToListL(RFs::GetSystemDrive(), priority, NULL));
	MY_ASSERT_EQUALS(KErrArgument, err);
	MY_ASSERT_EQUALS(iServer->iNotification, EMsvNullNotification);
	
	// 3. Add a drive which is already in the preferred drive list.
	priority = 1;
	TRAP(err, iServer->AddDriveToListL(RFs::GetSystemDrive(), priority, NULL));
	MY_ASSERT_EQUALS(KErrAlreadyExists, err);
	MY_ASSERT_EQUALS(iServer->iNotification, EMsvNullNotification);
	
	
	/**
	 * PRE-CONDITIONS - Drive list now is:
	 *  C - EMsvMessageStoreAvailableStatus
	 *  D - EMsvMessageStoreUnavailableStatus
	 *  E - EMsvMessageStoreUnavailableStatus
	 *  F - EMsvMessageStoreNotSupportedStatus
	 */
	// 4. Add an empty drive to the list, but of lower priority than the current drive.
	//Note: Remove a drive from the list and add it again. This is done because we cannot
	//  modify(?) epoc.ini during execution.
	TRAP(err, iServer->RemoveDriveFromListL(EDriveE));
	MY_ASSERT_EQUALS(KErrNone, err);
	driveCount = CMsvPreferredDriveList::GetDriveList()->Count();
	MY_ASSERT_EQUALS(3, driveCount); //C, D and F.
	
	priority = 2;
	TRAP(err, iServer->AddDriveToListL(EDriveE, priority, NULL));
	MY_ASSERT_EQUALS(KErrNone, err);
	driveCount = CMsvPreferredDriveList::GetDriveList()->Count();
	MY_ASSERT_EQUALS(4, driveCount); //C, E, D and F.
	MY_ASSERT_EQUALS(iServer->iNotification, EMsvNullNotification);
	
	err = CMsvPreferredDriveList::GetDriveList()->GetDriveIndex(RFs::GetSystemDrive(), driveIndex);
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS((TUint)0, driveIndex);
	err = CMsvPreferredDriveList::GetDriveList()->GetDriveIndex(EDriveD, driveIndex);
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS((TUint)2, driveIndex); //now D is at index 2 as E has priority 2, and hence index 1.
	err = CMsvPreferredDriveList::GetDriveList()->GetDriveIndex(EDriveF, driveIndex);
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS((TUint)3, driveIndex);
	
	err = CMsvPreferredDriveList::GetDriveList()->GetDriveIndex(EDriveE, driveIndex);
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS((TUint)1, driveIndex);
	CMsvPreferredDriveList::GetDriveList()->DriveInfoL(driveIndex, drive);
	MY_ASSERT_EQUALS(EDriveE, drive.driveNum);
	MY_ASSERT_EQUALS(EMsvMessageStoreUnavailableStatus, drive.status); //status not changed as we dont change drive to it.
	
	
	/**
	 * PRE-CONDITIONS - Drive list now is:
	 *  C - EMsvMessageStoreAvailableStatus
	 *  E - EMsvMessageStoreUnavailableStatus
	 *  D - EMsvMessageStoreUnavailableStatus
	 *  F - EMsvMessageStoreNotSupportedStatus
	 */
	// 5. Add a lower priority drive with valid message store to the list.
	//Note: We need to create a message store in EDriveD. So we change drive to it.
	//  For the test, we change back to system drive, remove EDriveD and then add
	//  it back.
	err = CMsvPreferredDriveList::GetDriveList()->GetDriveIndex(EDriveD, driveIndex);
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS((TUint)2, driveIndex);
	
	priority = 1;
	TRAP(err, iServer->UpdateDrivePriorityL(EDriveD, priority));
	MY_ASSERT_EQUALS(KErrNone, err); //now EDriveD will have a message store because we made it highest priority.
	priority = 1;
	TRAP(err, iServer->UpdateDrivePriorityL(RFs::GetSystemDrive(), priority));
	MY_ASSERT_EQUALS(KErrNone, err);
	TRAP(err, iServer->RemoveDriveFromListL(EDriveD));
	MY_ASSERT_EQUALS(KErrNone, err);
	TRAP(err, CreateEntryL(EDriveD, KFirstFreeEntryId+10));
	MY_ASSERT_EQUALS(KErrNone, err);
	driveCount = CMsvPreferredDriveList::GetDriveList()->Count();
	MY_ASSERT_EQUALS(3, driveCount); //C, E and F.
	
	priority = 2;
	TRAP(err, iServer->AddDriveToListL(EDriveD, priority, NULL));
	MY_ASSERT_EQUALS(KErrNone, err);
	driveCount = CMsvPreferredDriveList::GetDriveList()->Count();
	MY_ASSERT_EQUALS(4, driveCount); //C, D, E and F.
	MY_ASSERT_EQUALS(iServer->iNotification, EMsvRefreshMessageView);
	
	err = CMsvPreferredDriveList::GetDriveList()->GetDriveIndex(RFs::GetSystemDrive(), driveIndex);
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS((TUint)0, driveIndex);
	err = CMsvPreferredDriveList::GetDriveList()->GetDriveIndex(EDriveE, driveIndex);
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS((TUint)2, driveIndex); //now E is back at index 2 as D has priority 2, and hence index 1.
	err = CMsvPreferredDriveList::GetDriveList()->GetDriveIndex(EDriveF, driveIndex);
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS((TUint)3, driveIndex);
	
	err = CMsvPreferredDriveList::GetDriveList()->GetDriveIndex(EDriveD, driveIndex);
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS((TUint)1, driveIndex);
	CMsvPreferredDriveList::GetDriveList()->DriveInfoL(driveIndex, drive);
	MY_ASSERT_EQUALS(EDriveD, drive.driveNum);
	MY_ASSERT_EQUALS(EMsvMessageStoreAvailableStatus, drive.status); //D has a message store.
	
	
	/**
	 * PRE-CONDITIONS - Drive list now is:
	 *  C - EMsvMessageStoreAvailableStatus
	 *  D - EMsvMessageStoreAvailableStatus
	 *  E - EMsvMessageStoreUnavailableStatus
	 *  F - EMsvMessageStoreNotSupportedStatus
	 */
	// 6. Add a drive at the end of the drive list.
	//Note: We remove EDriveD and add it as a new drive at the end of the list.
	TRAP(err, iServer->RemoveDriveFromListL(EDriveD));
	MY_ASSERT_EQUALS(KErrNone, err);
	driveCount = CMsvPreferredDriveList::GetDriveList()->Count();
	MY_ASSERT_EQUALS(3, driveCount); //C, E and F.
	
	priority = 6;
	TRAP(err, iServer->AddDriveToListL(EDriveD, priority, NULL));
	MY_ASSERT_EQUALS(KErrNone, err);
	driveCount = CMsvPreferredDriveList::GetDriveList()->Count();
	MY_ASSERT_EQUALS(4, driveCount); //C, E, F and D.
	MY_ASSERT_EQUALS(iServer->iNotification, EMsvRefreshMessageView);
	
	err = CMsvPreferredDriveList::GetDriveList()->GetDriveIndex(RFs::GetSystemDrive(), driveIndex);
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS((TUint)0, driveIndex);
	err = CMsvPreferredDriveList::GetDriveList()->GetDriveIndex(EDriveE, driveIndex);
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS((TUint)1, driveIndex); //now E is back at index 1 as D has priority 6 and is now at the end.
	err = CMsvPreferredDriveList::GetDriveList()->GetDriveIndex(EDriveF, driveIndex);
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS((TUint)2, driveIndex);
	
	err = CMsvPreferredDriveList::GetDriveList()->GetDriveIndex(EDriveD, driveIndex);
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS((TUint)3, driveIndex);
	CMsvPreferredDriveList::GetDriveList()->DriveInfoL(driveIndex, drive);
	MY_ASSERT_EQUALS(EDriveD, drive.driveNum);
	MY_ASSERT_EQUALS(EMsvMessageStoreAvailableStatus, drive.status);
	
	
	/**
	 * PRE-CONDITIONS - Drive list now is:
	 *  C - EMsvMessageStoreAvailableStatus
	 *  E - EMsvMessageStoreUnavailableStatus
	 *  F - EMsvMessageStoreNotSupportedStatus
	 *  D - EMsvMessageStoreAvailableStatus
	 */
	// 7. Add a drive with priority high enough to make it current drive.
	//Note: Remove EDriveD and add it again with a high enough priority.
	TRAP(err, iServer->RemoveDriveFromListL(EDriveD));
	MY_ASSERT_EQUALS(KErrNone, err);
	// Add some entry to EDriveD whose preparation flag is set to ETrue.
	TRAP(err, CreateEntryL(EDriveD, KFirstFreeEntryId+11));
	MY_ASSERT_EQUALS(KErrNone, err);
	driveCount = CMsvPreferredDriveList::GetDriveList()->Count();
	MY_ASSERT_EQUALS(3, driveCount); //C, E and F.
	
	priority = 1;
	TRAP(err, iServer->AddDriveToListL(EDriveD, priority, NULL));
	MY_ASSERT_EQUALS(KErrNone, err);
	driveCount = CMsvPreferredDriveList::GetDriveList()->Count();
	MY_ASSERT_EQUALS(4, driveCount); //D, C, E and F.
	
	err = CMsvPreferredDriveList::GetDriveList()->GetDriveIndex(RFs::GetSystemDrive(), driveIndex);
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS((TUint)1, driveIndex);
	err = CMsvPreferredDriveList::GetDriveList()->GetDriveIndex(EDriveE, driveIndex);
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS((TUint)2, driveIndex);
	err = CMsvPreferredDriveList::GetDriveList()->GetDriveIndex(EDriveF, driveIndex);
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS((TUint)3, driveIndex);
	
	err = CMsvPreferredDriveList::GetDriveList()->GetDriveIndex(EDriveD, driveIndex);
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS((TUint)0, driveIndex); //D is the current drive now.
	CMsvPreferredDriveList::GetDriveList()->DriveInfoL(driveIndex, drive);
	MY_ASSERT_EQUALS(EDriveD, drive.driveNum);
	MY_ASSERT_EQUALS(EMsvMessageStoreAvailableStatus, drive.status);
	
	
	/**
	 * PRE-CONDITIONS - Drive list now is:
	 *  D - EMsvMessageStoreAvailableStatus
	 *  C - EMsvMessageStoreAvailableStatus
	 *  E - EMsvMessageStoreUnavailableStatus
	 *  F - EMsvMessageStoreNotSupportedStatus
	 */
	// 8. Add a drive with highest priority but which has an unsupported message store.
	//Note: We remove EDriveF and add it again. EDriveF has a Index file message store as can
	//  be seen in SetupL().
	TRAP(err, iServer->RemoveDriveFromListL(EDriveF));
	MY_ASSERT_EQUALS(KErrNone, err);
	driveCount = CMsvPreferredDriveList::GetDriveList()->Count();
	MY_ASSERT_EQUALS(3, driveCount); //D, C and E.
	
	priority = 1;
	TRAP(err, iServer->AddDriveToListL(EDriveF, priority, NULL));
	MY_ASSERT_EQUALS(KErrNone, err);
	driveCount = CMsvPreferredDriveList::GetDriveList()->Count();
	MY_ASSERT_EQUALS(4, driveCount); //F, D, C and E.
	
	TInt curDriveIndex = CMsvPreferredDriveList::GetDriveList()->CurrentDriveIndex();
	MY_ASSERT_EQUALS(1, curDriveIndex); //Current drive index is changed but current drive is not changed.
	CMsvPreferredDriveList::GetDriveList()->DriveInfoL(curDriveIndex, drive);
	MY_ASSERT_EQUALS(EDriveD, drive.driveNum); //D is still current drive as before.
	MY_ASSERT_EQUALS(EMsvMessageStoreAvailableStatus, drive.status);
	
	
	/**
	 * PRE-CONDITIONS - Drive list now is:
	 *  F - EMsvMessageStoreNotSupportedStatus
	 *  D - EMsvMessageStoreAvailableStatus
	 *  C - EMsvMessageStoreAvailableStatus
	 *  E - EMsvMessageStoreUnavailableStatus
	 */
	// 9. Add a drive with priority lower than current drive but with an unsupported message store.
	//Note: We remove EDriveF and add it again. EDriveF has a Index file message store as can
	//  be seen in SetupL().
	TRAP(err, iServer->RemoveDriveFromListL(EDriveF));
	MY_ASSERT_EQUALS(KErrNone, err);
	driveCount = CMsvPreferredDriveList::GetDriveList()->Count();
	MY_ASSERT_EQUALS(3, driveCount); //D, C and E.
	
	priority = 3;
	TRAP(err, iServer->AddDriveToListL(EDriveF, priority, NULL));
	MY_ASSERT_EQUALS(KErrNone, err);
	driveCount = CMsvPreferredDriveList::GetDriveList()->Count();
	MY_ASSERT_EQUALS(4, driveCount); //D, C, F and E.
	
	err = CMsvPreferredDriveList::GetDriveList()->GetDriveIndex(EDriveF, driveIndex);
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS((TUint)2, driveIndex);
	curDriveIndex = CMsvPreferredDriveList::GetDriveList()->CurrentDriveIndex();
	MY_ASSERT_EQUALS(0, curDriveIndex); //Current drive index is not changed.
	CMsvPreferredDriveList::GetDriveList()->DriveInfoL(curDriveIndex, drive);
	MY_ASSERT_EQUALS(EDriveD, drive.driveNum); //D is still current drive as before.
	MY_ASSERT_EQUALS(EMsvMessageStoreAvailableStatus, drive.status);
	
	// 10. Insert a drive with corrupt message store at the highest priority.
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CFileMan* file = CFileMan::NewL(fs);
	CleanupStack::PushL(file);
	
	TParse parse;
	TPtrC drivePtr = TDriveUnit(EDriveJ).Name();
	parse.Set(KDbFileName, &drivePtr, NULL);
	TFileName dbName = parse.FullName();
	RSqlDatabase::Delete(dbName);
	err = fs.MkDirAll(parse.DriveAndPath());
	TRAP(err, CMsvDBAdapter::CreateDBL(EDriveJ, ETrue));  //place an invalid database in EDriveE.
	MY_ASSERT_EQUALS(err, KErrNone);
	priority = 1;
	TRAP(err, iServer->AddDriveToListL(EDriveJ, priority, NULL));
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS(iServer->iNotification, EMsvMediaChanged);
	fs.Close();
	CleanupStack::PopAndDestroy();
	/**
	 * PRE-CONDITIONS - Drive list now is:
	 *  J - EMsvMessageStoreAvailableStatus
	 *  D - EMsvMessageStoreAvailableStatus
	 *  C - EMsvMessageStoreAvailableStatus
	 *  F - EMsvMessageStoreNotSupportedStatus
	 *  E - EMsvMessageStoreUnavailableStatus   
	 */
	// 11. Add a drive to the end of the list when drive list has reached its maximum possible size.
	
	priority = 6;
	TRAP(err, iServer->AddDriveToListL(EDriveK, priority, NULL));
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS(iServer->iNotification, EMsvNullNotification);
	
	priority = 7;
	TRAP(err, iServer->AddDriveToListL(EDriveL, priority, NULL));
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS(iServer->iNotification, EMsvNullNotification);
	
	priority = 7; //add a drive to exceed max drive count
	TRAP(err, iServer->AddDriveToListL(EDriveM, priority, NULL));
	MY_ASSERT_EQUALS(KErrNotSupported, err);
	MY_ASSERT_EQUALS(iServer->iNotification, EMsvNullNotification);
	
	}
	
	
	
void CTestMsvServer::TestRemoveDriveFromListL()
	{
	/* A snapshot of msgprioritydrivelist.ini is:
	
	PriorityDrive1 = C
	PriorityDrive2 = D
	PriorityDrive3 = E
	PriorityDrive4 = F
	PriorityDrive5 = P
	PriorityDrive6 = Q
	PriorityDrive7 = R
	*/
	
	_LIT(KFunction, "TestRemoveDriveFromListL");
	INFO_PRINTF1(KFunction);
	
	TInt curDriveIndex = 0;
	TMsvPreferredDrive drive;
	TUint driveIndex = 0;
	TUint priority = 0;
	TInt driveCount = 0;
		
	/**
	 * PRE-CONDITIONS - Drive list now is:
	 *  C - EMsvMessageStoreAvailableStatus
	 *  D - EMsvMessageStoreUnavailableStatus
	 *  E - EMsvMessageStoreUnavailableStatus
	 *  F - EMsvMessageStoreNotSupportedStatus
	 */
	// 1. Remove system drive.
	driveCount = CMsvPreferredDriveList::GetDriveList()->Count();
	MY_ASSERT_EQUALS(4, driveCount); //C, D, E and F.
	
	TRAPD(err, iServer->RemoveDriveFromListL(RFs::GetSystemDrive()));
	MY_ASSERT_EQUALS(KErrArgument, err);
	
	driveCount = CMsvPreferredDriveList::GetDriveList()->Count();
	MY_ASSERT_EQUALS(4, driveCount); //C, D, E and F. - same as before.
	MY_ASSERT_EQUALS(iServer->iNotification, EMsvNullNotification);
	curDriveIndex = CMsvPreferredDriveList::GetDriveList()->CurrentDriveIndex();
	MY_ASSERT_EQUALS(0, curDriveIndex);
	CMsvPreferredDriveList::GetDriveList()->DriveInfoL(curDriveIndex, drive);
	MY_ASSERT_EQUALS(RFs::GetSystemDrive(), drive.driveNum); //System drive is still in the drive list
	MY_ASSERT_EQUALS(EMsvMessageStoreAvailableStatus, drive.status);
	
	
	// 2. Remove drive not on the preferred drive list.
	TRAP(err, iServer->RemoveDriveFromListL(EDriveK));
	MY_ASSERT_EQUALS(KErrNotFound, err);
	driveCount = CMsvPreferredDriveList::GetDriveList()->Count();
	MY_ASSERT_EQUALS(4, driveCount); //C, D, E and F. - same as before.
	MY_ASSERT_EQUALS(iServer->iNotification, EMsvNullNotification);
	
	// 3. Remove current drive such that a empty drive becomes current.
	// Current Status: C(A), D(U), E(U), F(NS).
	// Change this to: D(A), E(U), C(A), F(NS)
	// Change this to: D(A), F(NS), E(U), C(A)
	// Remove Drive D.
	priority = 3;
	TRAP(err, iServer->UpdateDrivePriorityL(EDriveC, priority));
	MY_ASSERT_EQUALS(KErrNone, err);
	priority = 2;
	TRAP(err, iServer->UpdateDrivePriorityL(EDriveF, priority));
	MY_ASSERT_EQUALS(KErrNone, err);
	
	TRAP(err, iServer->RemoveDriveFromListL(EDriveD));
	MY_ASSERT_EQUALS(KErrNone, err);
	
	curDriveIndex = CMsvPreferredDriveList::GetDriveList()->CurrentDriveIndex();
	MY_ASSERT_EQUALS(1, curDriveIndex);
	CMsvPreferredDriveList::GetDriveList()->DriveInfoL(curDriveIndex, drive);
	MY_ASSERT_EQUALS(EDriveE, drive.driveNum);
	MY_ASSERT_EQUALS(EMsvMessageStoreAvailableStatus, drive.status);
	
	/**
	 * PRE-CONDITIONS - Drive list now is:
	 * Current Status: F(NS), E(A), C(A).
	 * Remove Drive E (Current Drive).
	 */
	// 3. Remove a current drive.
	//Note: On startup system drive is current drive. Lets change to another drive
	//  and try to remove that.
	TRAP(err, iServer->RemoveDriveFromListL(EDriveE));
	MY_ASSERT_EQUALS(KErrNone, err);
	
	err = CMsvPreferredDriveList::GetDriveList()->GetDriveIndex(EDriveE, driveIndex);
	MY_ASSERT_EQUALS(KErrNotFound, err);
	
	driveCount = CMsvPreferredDriveList::GetDriveList()->Count();
	MY_ASSERT_EQUALS(2, driveCount); //F and C.
	MY_ASSERT_EQUALS(iServer->iNotification, EMsvMediaChanged);
	curDriveIndex = CMsvPreferredDriveList::GetDriveList()->CurrentDriveIndex();
	MY_ASSERT_EQUALS(1, curDriveIndex); //back to system drive that was on 0.
	CMsvPreferredDriveList::GetDriveList()->DriveInfoL(curDriveIndex, drive);
	MY_ASSERT_EQUALS(RFs::GetSystemDrive(), drive.driveNum);
	MY_ASSERT_EQUALS(EMsvMessageStoreAvailableStatus, drive.status);
	
	// Set up the precondition for next test cases
	// Current Status: F(NS), C(A)
	// New status should be: F(NS), C(A), D(A), G(U)
	priority = 3;
	TRAP(err, iServer->AddDriveToListL(EDriveD, priority, NULL));
	MY_ASSERT_EQUALS(err, KErrNone);
	priority = 3;
	TRAP(err, iServer->AddDriveToListL(EDriveG, priority, NULL));
	MY_ASSERT_EQUALS(err, KErrNone);
	// Test
	err = CMsvPreferredDriveList::GetDriveList()->GetDriveIndex(EDriveD, driveIndex);
	MY_ASSERT_EQUALS(KErrNone, err);
	CMsvPreferredDriveList::GetDriveList()->DriveInfoL(driveIndex, drive);
	MY_ASSERT_EQUALS(EDriveD, drive.driveNum);
	MY_ASSERT_EQUALS(EMsvMessageStoreAvailableStatus, drive.status);
	err = CMsvPreferredDriveList::GetDriveList()->GetDriveIndex(EDriveG, driveIndex);
	MY_ASSERT_EQUALS(KErrNone, err);
	CMsvPreferredDriveList::GetDriveList()->DriveInfoL(driveIndex, drive);
	MY_ASSERT_EQUALS(EDriveG, drive.driveNum);
	MY_ASSERT_EQUALS(EMsvMessageStoreUnavailableStatus, drive.status);
	
	/**
	 * PRE-CONDITIONS - Drive list now is:
	 * Current Status: F(NS), C(A), D(A), G(U)
	 * Remove Drive D.
	 */
	// 4. Remove a non-current drive which has a message store.
	//Note: We need to change drive to that drive and then back to system drive to
	//  create a message store.
	//Note: This test is just fine for a drive with no message store too.
	TRAP(err, iServer->RemoveDriveFromListL(EDriveD));
	MY_ASSERT_EQUALS(KErrNone, err);
	
	err = CMsvPreferredDriveList::GetDriveList()->GetDriveIndex(EDriveE, driveIndex);
	MY_ASSERT_EQUALS(KErrNotFound, err);
	
	driveCount = CMsvPreferredDriveList::GetDriveList()->Count();
	MY_ASSERT_EQUALS(3, driveCount); //F, C and G.
	MY_ASSERT_EQUALS(iServer->iNotification, EMsvRefreshMessageView);
	curDriveIndex = CMsvPreferredDriveList::GetDriveList()->CurrentDriveIndex();
	MY_ASSERT_EQUALS(1, curDriveIndex);
	CMsvPreferredDriveList::GetDriveList()->DriveInfoL(curDriveIndex, drive);
	MY_ASSERT_EQUALS(RFs::GetSystemDrive(), drive.driveNum);
	MY_ASSERT_EQUALS(EMsvMessageStoreAvailableStatus, drive.status);
	
	/**
	 * Current Status: F(NS), C(A), G(U)
	 * Remove Drive G
	 */
	TRAP(err, iServer->RemoveDriveFromListL(EDriveG));
	MY_ASSERT_EQUALS(KErrNone, err);
	
	err = CMsvPreferredDriveList::GetDriveList()->GetDriveIndex(EDriveG, driveIndex);
	MY_ASSERT_EQUALS(KErrNotFound, err);
	
	driveCount = CMsvPreferredDriveList::GetDriveList()->Count();
	MY_ASSERT_EQUALS(2, driveCount); //F & C.
	curDriveIndex = CMsvPreferredDriveList::GetDriveList()->CurrentDriveIndex();
	MY_ASSERT_EQUALS(1, curDriveIndex);
	CMsvPreferredDriveList::GetDriveList()->DriveInfoL(curDriveIndex, drive);
	MY_ASSERT_EQUALS(RFs::GetSystemDrive(), drive.driveNum);
	MY_ASSERT_EQUALS(EMsvMessageStoreAvailableStatus, drive.status);
	}



void CTestMsvServer::TestUpdateDrivePriorityL()
	{
	/* A snapshot of msgprioritydrivelist.ini is:
	
	PriorityDrive1 = C
	PriorityDrive2 = D
	PriorityDrive3 = E
	PriorityDrive4 = F
	PriorityDrive5 = G
	PriorityDrive6 = H
	PriorityDrive7 = I
	*/
	
	/**
	 * Priority ranges are from 1-7.
	 * So drive index will be from 0-6.
	 */
	
	_LIT(KFunction, "TestUpdateDrivePriorityL");
	INFO_PRINTF1(KFunction);
	
	TUint priority = 0;
	TUint driveIndex = 0;
	TUint oldIndex = 0;
	TInt curDriveIndex = 0;
	TMsvPreferredDrive drive;
	TInt driveCount = 0;
		
	
	/**
	 * PRE-CONDITIONS - Drive list now is:
	 *  C - EMsvMessageStoreAvailableStatus
	 *  D - EMsvMessageStoreUnavailableStatus
	 *  E - EMsvMessageStoreUnavailableStatus
	 *  F - EMsvMessageStoreNotSupportedStatus
	 */
	// 1. Update priority with an invalid priority value.
	priority = 10;
	TRAPD(err, iServer->UpdateDrivePriorityL(RFs::GetSystemDrive(), priority));
	MY_ASSERT_EQUALS(KErrArgument, err);
	MY_ASSERT_EQUALS(iServer->iNotification, EMsvNullNotification);
	
	// 1.1
	priority = 0;
	TRAP(err, iServer->UpdateDrivePriorityL(RFs::GetSystemDrive(), priority));
	MY_ASSERT_EQUALS(KErrArgument, err);
	MY_ASSERT_EQUALS(iServer->iNotification, EMsvNullNotification);
	
	// 2. Update priority of a drive not in the list.
	priority = 2;
	TRAP(err, iServer->UpdateDrivePriorityL(EDriveK, priority));
	MY_ASSERT_EQUALS(KErrNotFound, err);
	MY_ASSERT_EQUALS(iServer->iNotification, EMsvNullNotification);
	
	
	// 3. Update priority of a drive with the same priority as it had before.
	err = CMsvPreferredDriveList::GetDriveList()->GetDriveIndex(RFs::GetSystemDrive(), driveIndex);
	MY_ASSERT_EQUALS((TUint)0, driveIndex);
	ASSERT_EQUALS(KErrNone, err);
	
	priority = 1;
	TRAP(err, iServer->UpdateDrivePriorityL(RFs::GetSystemDrive(), priority));
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS(iServer->iNotification, EMsvNullNotification);
	
	err = CMsvPreferredDriveList::GetDriveList()->GetDriveIndex(RFs::GetSystemDrive(), driveIndex);
	MY_ASSERT_EQUALS(driveIndex, oldIndex);
	ASSERT_EQUALS(KErrNone, err);
	
	
	/**
	 * PRE-CONDITIONS - Drive list now is:
	 *  C - EMsvMessageStoreAvailableStatus
	 *  D - EMsvMessageStoreUnavailableStatus
	 *  E - EMsvMessageStoreUnavailableStatus
	 *  F - EMsvMessageStoreNotSupportedStatus
	 */
	// 4. Update priority of a non-current drive - Non-current to Non-current.
	err = CMsvPreferredDriveList::GetDriveList()->GetDriveIndex(EDriveE, oldIndex);
	ASSERT_EQUALS(KErrNone, err);
	curDriveIndex = CMsvPreferredDriveList::GetDriveList()->CurrentDriveIndex();
	CMsvPreferredDriveList::GetDriveList()->DriveInfoL(curDriveIndex, drive);
	MY_ASSERT_EQUALS(RFs::GetSystemDrive(), drive.driveNum); //C is current drive as of now.

	// Current Status
	// C(A), D(U), E(U), F(NS)
	// Move E to last position.
	priority = 5;
	TRAP(err, iServer->UpdateDrivePriorityL(EDriveE, priority));
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS(iServer->iNotification, EMsvNullNotification);
	
	err = CMsvPreferredDriveList::GetDriveList()->GetDriveIndex(EDriveE, driveIndex);
	ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS((TUint)3, driveIndex);
		
	priority = 2;
	TRAP(err, iServer->UpdateDrivePriorityL(EDriveE, priority));
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS(iServer->iNotification, EMsvNullNotification);
	
	err = CMsvPreferredDriveList::GetDriveList()->GetDriveIndex(EDriveE, driveIndex);
	ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS(oldIndex - 1, driveIndex);
		
	curDriveIndex = CMsvPreferredDriveList::GetDriveList()->CurrentDriveIndex();
	MY_ASSERT_EQUALS(0, curDriveIndex);
	CMsvPreferredDriveList::GetDriveList()->DriveInfoL(curDriveIndex, drive);
	MY_ASSERT_EQUALS(RFs::GetSystemDrive(), drive.driveNum); //current drive has not changed.
	MY_ASSERT_EQUALS(EMsvMessageStoreAvailableStatus, drive.status);
	
	CMsvPreferredDriveList::GetDriveList()->DriveInfoL(oldIndex, drive);
	MY_ASSERT_EQUALS(EDriveD, drive.driveNum); //D is at the index E was on before.
	CMsvPreferredDriveList::GetDriveList()->DriveInfoL(driveIndex, drive);
	MY_ASSERT_EQUALS(EDriveE, drive.driveNum); //E is at the index D was on before.
	
	/**
	 * PRE-CONDITIONS - Drive list now is:
	 *  C - EMsvMessageStoreAvailableStatus
	 *  E - EMsvMessageStoreUnavailableStatus
	 *  D - EMsvMessageStoreUnavailableStatus
	 *  F - EMsvMessageStoreNotSupportedStatus
	 */
	// 5. Update priority of a non-current drive - Non-current to Current. BUT the drive being updated
	//  has an unsupported version of the message store.
	err = CMsvPreferredDriveList::GetDriveList()->GetDriveIndex(EDriveF, oldIndex);
	ASSERT_EQUALS(KErrNone, err);
	curDriveIndex = CMsvPreferredDriveList::GetDriveList()->CurrentDriveIndex();
	CMsvPreferredDriveList::GetDriveList()->DriveInfoL(curDriveIndex, drive);
	MY_ASSERT_EQUALS(RFs::GetSystemDrive(), drive.driveNum); //C is current drive as of now.
	
	priority = 1;
	TRAP(err, iServer->UpdateDrivePriorityL(EDriveF, priority));
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS(iServer->iNotification, EMsvNullNotification);
	
	curDriveIndex = CMsvPreferredDriveList::GetDriveList()->CurrentDriveIndex();
	MY_ASSERT_EQUALS(1, curDriveIndex);
	CMsvPreferredDriveList::GetDriveList()->DriveInfoL(curDriveIndex, drive);
	MY_ASSERT_EQUALS(RFs::GetSystemDrive(), drive.driveNum); //current drive has not changed but its index has.
	MY_ASSERT_EQUALS(EMsvMessageStoreAvailableStatus, drive.status);
	
	CMsvPreferredDriveList::GetDriveList()->DriveInfoL(priority - 1, drive);
	MY_ASSERT_EQUALS(EDriveF, drive.driveNum); //F is at the top of the list now.
	MY_ASSERT_EQUALS(EMsvMessageStoreNotSupportedStatus, drive.status);
	
	
	/**
	 * PRE-CONDITIONS - Drive list now is:
	 *  F - EMsvMessageStoreNotSupportedStatus
	 *  C - EMsvMessageStoreAvailableStatus
	 *  E - EMsvMessageStoreUnavailableStatus
	 *  D - EMsvMessageStoreUnavailableStatus
	 */
	// 6. Update priority of a non-current drive - Non-current to Current. The drive being updated
	//  has valid version of the message store.
	//Note: Set D to current drive so that a message store is created on it. Then set C as current drive to test
	//  the functionality.
	//Note: This test can be considered a test for drive being updated where status is either
	//  EMsvMessageStoreAvailableStatus or EMsvMessageStoreUnavailableStatus.
	err = CMsvPreferredDriveList::GetDriveList()->GetDriveIndex(EDriveD, driveIndex);
	ASSERT_EQUALS(KErrNone, err);
	curDriveIndex = CMsvPreferredDriveList::GetDriveList()->CurrentDriveIndex();
	MY_ASSERT_EQUALS(1, curDriveIndex);
	
	priority = 1;
	TRAP(err, iServer->UpdateDrivePriorityL(EDriveD, priority));
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS(iServer->iNotification, EMsvMediaChanged);
	
	curDriveIndex = CMsvPreferredDriveList::GetDriveList()->CurrentDriveIndex();
	MY_ASSERT_EQUALS(0, curDriveIndex);
	CMsvPreferredDriveList::GetDriveList()->DriveInfoL(curDriveIndex, drive);
	MY_ASSERT_EQUALS(EDriveD, drive.driveNum); //current drive has been changed to D.
	MY_ASSERT_EQUALS(EMsvMessageStoreAvailableStatus, drive.status);
	
	err = CMsvPreferredDriveList::GetDriveList()->GetDriveIndex(RFs::GetSystemDrive(), driveIndex);
	ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS((TUint)2, driveIndex);
	
	priority = 1;
	TRAP(err, iServer->UpdateDrivePriorityL(RFs::GetSystemDrive(), priority));
	MY_ASSERT_EQUALS(KErrNone, err);
	
	curDriveIndex = CMsvPreferredDriveList::GetDriveList()->CurrentDriveIndex();
	MY_ASSERT_EQUALS(0, curDriveIndex);
	CMsvPreferredDriveList::GetDriveList()->DriveInfoL(curDriveIndex, drive);
	MY_ASSERT_EQUALS(RFs::GetSystemDrive(), drive.driveNum); //current drive has been changed to C.
	MY_ASSERT_EQUALS(EMsvMessageStoreAvailableStatus, drive.status);
	
	
	/**
	 * PRE-CONDITIONS - Drive list now is:
	 *  C - EMsvMessageStoreAvailableStatus
	 *  D - EMsvMessageStoreAvailableStatus
	 *  F - EMsvMessageStoreNotSupportedStatus
	 *  E - EMsvMessageStoreUnavailableStatus
	 */
	// 7. Update priority of a current drive - Current to Current when priority is decreased.
	//Note: We remove D and E so that C can be tested so as not to change the current drive when
	//  priority is updated.
	TRAP(err, iServer->RemoveDriveFromListL(EDriveD));
	MY_ASSERT_EQUALS(KErrNone, err);
	TRAP(err, iServer->RemoveDriveFromListL(EDriveE));
	MY_ASSERT_EQUALS(KErrNone, err);
	
	priority = 3; //**Note that this priority value is greater than the drive list count.
	TRAP(err, iServer->UpdateDrivePriorityL(RFs::GetSystemDrive(), priority));
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS((TUint)2, priority); //priority is modified because the index is.
	MY_ASSERT_EQUALS(iServer->iNotification, EMsvNullNotification);
	
	curDriveIndex = CMsvPreferredDriveList::GetDriveList()->CurrentDriveIndex();
	MY_ASSERT_EQUALS(1, curDriveIndex);
	CMsvPreferredDriveList::GetDriveList()->DriveInfoL(curDriveIndex, drive);
	MY_ASSERT_EQUALS(RFs::GetSystemDrive(), drive.driveNum); //current drive has not been changed but its index has.
	MY_ASSERT_EQUALS(EMsvMessageStoreAvailableStatus, drive.status);
	
	CMsvPreferredDriveList::GetDriveList()->DriveInfoL(0, drive);
	MY_ASSERT_EQUALS(EDriveF, drive.driveNum); //F is at the top of the drive list but is not current drive.
	MY_ASSERT_EQUALS(EMsvMessageStoreNotSupportedStatus, drive.status);
	
	
	/**
	 * PRE-CONDITIONS - Drive list now is:
	 *  F - EMsvMessageStoreNotSupportedStatus
	 *  C - EMsvMessageStoreAvailableStatus
	 */
	// 8. Update priority of a current drive - Current to Current when priority is increased.
	priority = 1;
	TRAP(err, iServer->UpdateDrivePriorityL(RFs::GetSystemDrive(), priority));
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS(iServer->iNotification, EMsvNullNotification);
	
	curDriveIndex = CMsvPreferredDriveList::GetDriveList()->CurrentDriveIndex();
	MY_ASSERT_EQUALS(0, curDriveIndex);
	CMsvPreferredDriveList::GetDriveList()->DriveInfoL(curDriveIndex, drive);
	MY_ASSERT_EQUALS(RFs::GetSystemDrive(), drive.driveNum); //current drive has not been changed but its index has.
	MY_ASSERT_EQUALS(EMsvMessageStoreAvailableStatus, drive.status);
	
	
	/**
	 * PRE-CONDITIONS - Drive list now is:
	 *  C - EMsvMessageStoreAvailableStatus
	 *  F - EMsvMessageStoreNotSupportedStatus
	 */
	// 9. Update priority of a current drive - Current to Non-current.
	//Note: We add the drives we removed above so as to change drive when priority is updated.
	priority = 2;
	TRAP(err, iServer->AddDriveToListL(EDriveE, priority, NULL)); //add E to 2nd position.
	MY_ASSERT_EQUALS(KErrNone, err);
	priority = 2;
	TRAP(err, iServer->AddDriveToListL(EDriveD, priority, NULL)); //add D to 2nd position.
	MY_ASSERT_EQUALS(KErrNone, err);
	driveCount = CMsvPreferredDriveList::GetDriveList()->Count();
	MY_ASSERT_EQUALS(4, driveCount); //C(A), D(A), E(U) and F(NS).
	
	priority = 2; // <-----** ISSUE
	TRAP(err, iServer->UpdateDrivePriorityL(RFs::GetSystemDrive(), priority));
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS(iServer->iNotification, EMsvMediaChanged);
	
	curDriveIndex = CMsvPreferredDriveList::GetDriveList()->CurrentDriveIndex();
	MY_ASSERT_EQUALS(0, curDriveIndex);
	CMsvPreferredDriveList::GetDriveList()->DriveInfoL(curDriveIndex, drive);
	MY_ASSERT_EQUALS(EDriveD, drive.driveNum); //current drive has been changed.
	MY_ASSERT_EQUALS(EMsvMessageStoreAvailableStatus, drive.status);
	
	err = CMsvPreferredDriveList::GetDriveList()->GetDriveIndex(RFs::GetSystemDrive(), driveIndex);
	ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS((TUint)1, driveIndex);
	
	/**
	 * Drive list now is:
	 *  D - EMsvMessageStoreAvailableStatus
	 *  C - EMsvMessageStoreAvailableStatus
	 *  E - EMsvMessageStoreUnavailableStatus
	 *  F - EMsvMessageStoreNotSupportedStatus   
	 */ 
	 // 10. Update priority of a current drive - Current to non-current.
	 // Difference between case 9: New message store is created in an empty drive.
	 
	 // Change drive list to D, E, C, F.
	priority = 2;
	TRAP(err, iServer->UpdateDrivePriorityL(EDriveE, priority));
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS(iServer->iNotification, EMsvNullNotification);
	 
	// Current status: D(A), E(U), C(A), F(NS)
	// Change it to: E(U), D(A), C(A), F(NS) : New message store will be created in E
	priority = 2;
	TRAP(err, iServer->UpdateDrivePriorityL(EDriveD, priority));
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS(iServer->iNotification, EMsvMediaChanged);
	}



void CTestMsvServer::TestUpdateRepositoryL()
	{
	/* A snapshot of msgprioritydrivelist.ini is:
	
	PriorityDrive1 = C
	PriorityDrive2 = D
	PriorityDrive3 = E
	PriorityDrive4 = F
	PriorityDrive5 = G
	PriorityDrive6 = H
	PriorityDrive7 = I
	*/
	
	_LIT(KFunction, "TestUpdateRepositoryL");
	INFO_PRINTF1(KFunction);
	
	/**
	Note: We won't use the server functions of adding, removing or updating priority of
		  the drives so that changes are reflected in the drive list and thus the repository.
		  Instead, we can just test using the drive list directly and calling UpdateRepository()
		  as the other functions do a lot more than is to be tested.
	*/
	
	CMsvPreferredDriveList *iDriveList = CMsvPreferredDriveList::GetDriveList();
	TUint driveIndex = 0;
	CRepository* repository = NULL;
	TInt err = 0;
	TInt driveNum = 0;
	TMsvPreferredDrive drive;
		
	/**
	 * Drive list now is:
	 *  C - EMsvMessageStoreAvailableStatus
	 *  D - EMsvMessageStoreAvailableStatus
	 *  E - EMsvMessageStoreUnavailableStatus
	 *  F - EMsvMessageStoreNotSupportedStatus
	 */
	
	// 1. Delete a drive and update respository.
	//Note: Removing a drive as in RemoveDriveFromDriveListL().
	repository = CRepository::NewL(KUidConfigFile);
	CleanupStack::PushL(repository);
	
	driveIndex = 1; //also is the key index.
	err = repository->Get(driveIndex, driveNum);
	MY_ASSERT_TRUE(driveNum == EDriveD); //before deletion.
	
	iDriveList->Remove(driveIndex);
	TRAP(err, iServer->UpdateRepositoryL());
	MY_ASSERT_EQUALS(KErrNone, err);
	
	driveIndex = 1;
	err = repository->Get(driveIndex, driveNum);
	MY_ASSERT_TRUE(driveNum != EDriveD); //after deletion.
	
	
	// 2. Add a drive and update repository.
	//Note: Adding a drive as in AddDriveToDriveListL().
	driveIndex = 1;
	err = repository->Get(driveIndex, driveNum);
	MY_ASSERT_TRUE(driveNum != EDriveD); //before addition.
	
	drive.driveNum = EDriveD;
	iDriveList->Insert(drive, driveIndex);
	TRAP(err, iServer->UpdateRepositoryL());
	MY_ASSERT_EQUALS(KErrNone, err);
	
	driveIndex = 1;
	err = repository->Get(driveIndex, driveNum);
	MY_ASSERT_TRUE(driveNum == EDriveD); //after addition.
	
	
	// 3. Update drive index of a drive.
	//Note: Updating drive index implies removing and inserting a drive
	//  as done in UpdateDrivePriorityL().
	driveIndex = 1;
	err = repository->Get(driveIndex, driveNum);
	MY_ASSERT_TRUE(driveNum == EDriveD); //before updation.
	
	iDriveList->Remove(driveIndex);
	driveIndex = 0; //insert at top of the list.
	iDriveList->Insert(drive, driveIndex);
	TRAP(err, iServer->UpdateRepositoryL());
	MY_ASSERT_EQUALS(KErrNone, err);
	
	driveIndex = 1;
	err = repository->Get(driveIndex, driveNum);
	MY_ASSERT_TRUE(driveNum != EDriveD); //after updation.
	driveIndex = 0;
	err = repository->Get(driveIndex, driveNum);
	MY_ASSERT_TRUE(driveNum == EDriveD);
	
	CleanupStack::PopAndDestroy(repository);
	
	}



void CTestMsvServer::TestCreatePreferredDriveListL()
	{
	_LIT(KFunction, "TestCreatePreferredDriveListL");
	INFO_PRINTF1(KFunction);
	
	_LIT(KRepositoryPath, "\\private\\10202be9\\persists\\10286a26.cre");
	
	/**
	 * AVAILABLE DRIVES declared in epoc.ini are:
	 *  - EDriveD
	 *  - EDriveE
	 *  - EDriveF and
	 *  - EDriveC (implicitly available as it is the system drive.)
	 */
	
	
	CMsvPreferredDriveList *iDriveList = CMsvPreferredDriveList::GetDriveList();
	
	TUint driveIndex = 0;
	TInt driveNum;
	TInt err = KErrNone;
	TParse parse;
	CRepository* repository = NULL;
	
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CFileMan* file = NULL;
	
	// 1. Config file not present.
	iServer->ResetRepositoryL(); //resetting it is as good as deleting the repository to simulate fresh startup.
	TPtrC drive = TDriveUnit(RFs::GetSystemDrive()).Name();
	parse.Set(KPreferredDriveListFilePath, &drive, NULL);
	err = fs.Delete(parse.FullName()); //ignore error
	
	delete iServer;
	TRAP(err, iServer = CMsvServer::NewL());
	MY_ASSERT_EQUALS(err, KErrNone);
	
	iDriveList = CMsvPreferredDriveList::GetDriveList();
	MY_ASSERT_EQUALS(1, iDriveList->Count());
	
	err = CMsvPreferredDriveList::GetDriveList()->GetDriveIndex(RFs::GetSystemDrive(), driveIndex);
	MY_ASSERT_EQUALS((TUint)0, driveIndex);
	
	// 1.2 Check CenRep.
	repository = CRepository::NewL(KUidConfigFile);
	CleanupStack::PushL(repository);
	User::LeaveIfError(repository->Get(driveIndex, driveNum));
	MY_ASSERT_EQUALS(driveNum, (TInt)RFs::GetSystemDrive()); //system drive in CenRep.
	
	User::LeaveIfError(repository->Get(driveIndex + 1, driveNum));
	MY_ASSERT_TRUE(driveNum > EDriveZ); //no other drive in CenRep. the CenRep will instead have values > EDriveZ
										//to represent invalid or unset values.
	CleanupStack::PopAndDestroy(); //repository
	
	
	// 2. Empty config file present.
	iServer->ResetRepositoryL(); //resetting it is as good as deleting the repository to simulate fresh startup.
	drive.Set(TDriveUnit(RFs::GetSystemDrive()).Name());
	parse.Set(KPreferredDriveListFilePath, &drive, NULL);
	err = fs.Delete(parse.FullName()); //ignore error
	
	file = CFileMan::NewL(fs);
	CleanupStack::PushL(file);
	parse.Set(KPreferredDriveListFilePath, &drive, NULL);
	TFileName config = parse.FullName();
	err = file->Copy(KMsgTestPriorityDriveListFile1, config); //ignore error
	CleanupStack::PopAndDestroy(file);
	
	delete iServer;
	TRAP(err, iServer = CMsvServer::NewL());
	MY_ASSERT_EQUALS(err, KErrNone);

	iDriveList = CMsvPreferredDriveList::GetDriveList();
	MY_ASSERT_EQUALS(1, iDriveList->Count());
	
	err = CMsvPreferredDriveList::GetDriveList()->GetDriveIndex(RFs::GetSystemDrive(), driveIndex);
	MY_ASSERT_EQUALS((TUint)0, driveIndex)
	// 2.2 Check CenRep.
	repository = CRepository::NewL(KUidConfigFile);
	CleanupStack::PushL(repository);
	User::LeaveIfError(repository->Get(driveIndex, driveNum));
	MY_ASSERT_EQUALS(driveNum, (TInt)RFs::GetSystemDrive()); //system drive in CenRep.
	
	User::LeaveIfError(repository->Get(driveIndex + 1, driveNum));
	MY_ASSERT_TRUE(driveNum > EDriveZ); //no other drive in CenRep. the CenRep will instead have values > EDriveZ
										//to represent invalid or unset values.
	CleanupStack::PopAndDestroy(); //repository
	
	// 3. No system drive present in 'valid' config file.
	/** 
	PriorityDrive1 = P
	PriorityDrive2 = D
	PriorityDrive3 = E
	PriorityDrive4 = F
	PriorityDrive5 = P
	PriorityDrive6 = Q
	PriorityDrive7 = R
	*/

	drive.Set(TDriveUnit(RFs::GetSystemDrive()).Name());
	parse.Set(KPreferredDriveListFilePath, &drive, NULL);
	err = fs.Delete(parse.FullName()); //ignore error
	iServer->ResetRepositoryL(); //resetting it is as good as deleting the repository to simulate fresh startup.
	
	file = CFileMan::NewL(fs);
	CleanupStack::PushL(file);
	parse.Set(KPreferredDriveListFilePath, NULL, NULL);
	config = parse.FullName();
	err = file->Copy(KMsgTestPriorityDriveListFile3, config); //ignore error
	CleanupStack::PopAndDestroy(file);
	
	delete iServer;
	TRAP(err, iServer = CMsvServer::NewL());
	MY_ASSERT_EQUALS(err, KErrNone);

	iDriveList = CMsvPreferredDriveList::GetDriveList();
	MY_ASSERT_EQUALS(4, iDriveList->Count()); //D, E, F and C.
	
	err = CMsvPreferredDriveList::GetDriveList()->GetDriveIndex(EDriveD, driveIndex);
	MY_ASSERT_EQUALS((TUint)0, driveIndex);
	err = CMsvPreferredDriveList::GetDriveList()->GetDriveIndex(EDriveE, driveIndex);
	MY_ASSERT_EQUALS((TUint)1, driveIndex);
	err = CMsvPreferredDriveList::GetDriveList()->GetDriveIndex(EDriveF, driveIndex);
	MY_ASSERT_EQUALS((TUint)2, driveIndex);
	err = CMsvPreferredDriveList::GetDriveList()->GetDriveIndex(RFs::GetSystemDrive(), driveIndex);
	MY_ASSERT_EQUALS((TUint)3, driveIndex);
	// 3.2 Check CenRep.
	repository = CRepository::NewL(KUidConfigFile);
	CleanupStack::PushL(repository);
	User::LeaveIfError(repository->Get(driveIndex, driveNum));
	MY_ASSERT_EQUALS(driveNum, (TInt)RFs::GetSystemDrive()); //system drive is in CenRep.
	
	User::LeaveIfError(repository->Get(driveIndex + 1, driveNum));
	MY_ASSERT_TRUE(driveNum > EDriveZ); //no other drive after system drive in CenRep. the CenRep will instead 
										//have values > EDriveZ to represent invalid or unset values.
										
	User::LeaveIfError(repository->Get(--driveIndex, driveNum));
	MY_ASSERT_EQUALS(driveNum, (TInt)EDriveF);
	User::LeaveIfError(repository->Get(--driveIndex, driveNum));
	MY_ASSERT_EQUALS(driveNum, (TInt)EDriveE);
	User::LeaveIfError(repository->Get(--driveIndex, driveNum));
	MY_ASSERT_EQUALS(driveNum, (TInt)EDriveD);
	
	CleanupStack::PopAndDestroy(); //repository
	
	
	// 4. More than 7 drives in config file.
	/**
	PriorityDrive1 = C
	PriorityDrive2 = O
	PriorityDrive3 = E
	PriorityDrive4 = F
	PriorityDrive5 = P
	PriorityDrive6 = Q
	PriorityDrive7 = R
	PriorityDrive8 = D
	*/

	drive.Set(TDriveUnit(RFs::GetSystemDrive()).Name());
	parse.Set(KPreferredDriveListFilePath, &drive, NULL);
	err = fs.Delete(parse.FullName()); //ignore error
	iServer->ResetRepositoryL(); //resetting it is as good as deleting the repository to simulate fresh startup.
	
	file = CFileMan::NewL(fs);
	CleanupStack::PushL(file);
	parse.Set(KPreferredDriveListFilePath, NULL, NULL);
	config = parse.FullName();
	err = file->Copy(KMsgTestPriorityDriveListFile4, config); //ignore error
	CleanupStack::PopAndDestroy(file);
	
	delete iServer;
	TRAP(err, iServer = CMsvServer::NewL());
	MY_ASSERT_EQUALS(err, KErrNone);

	iDriveList = CMsvPreferredDriveList::GetDriveList();
	MY_ASSERT_EQUALS(3, iDriveList->Count()); //C, E and F.
	
	err = CMsvPreferredDriveList::GetDriveList()->GetDriveIndex(EDriveD, driveIndex);
	MY_ASSERT_EQUALS(KErrNotFound, err); //EDriveD does not exist in the drive list.
	err = CMsvPreferredDriveList::GetDriveList()->GetDriveIndex(EDriveE, driveIndex);
	MY_ASSERT_EQUALS((TUint)1, driveIndex);
	err = CMsvPreferredDriveList::GetDriveList()->GetDriveIndex(EDriveF, driveIndex);
	MY_ASSERT_EQUALS((TUint)2, driveIndex);
	err = CMsvPreferredDriveList::GetDriveList()->GetDriveIndex(RFs::GetSystemDrive(), driveIndex);
	MY_ASSERT_EQUALS((TUint)0, driveIndex);
	// 4.2 Check CenRep.
	repository = CRepository::NewL(KUidConfigFile);
	CleanupStack::PushL(repository);
	
	User::LeaveIfError(repository->Get(driveIndex, driveNum));
	MY_ASSERT_EQUALS(driveNum, (TInt)RFs::GetSystemDrive());
	User::LeaveIfError(repository->Get(++driveIndex, driveNum));
	MY_ASSERT_EQUALS(driveNum, (TInt)EDriveE);
	User::LeaveIfError(repository->Get(++driveIndex, driveNum));
	MY_ASSERT_EQUALS(driveNum, (TInt)EDriveF);
	User::LeaveIfError(repository->Get(++driveIndex, driveNum));
	MY_ASSERT_TRUE(driveNum > EDriveZ); //no other drive after EDriveF in CenRep. the CenRep will instead 
										//have values > EDriveZ to represent invalid or unset values.
	
	CleanupStack::PopAndDestroy(); //repository
	
	
	// 5. Less than 7 drives in the config file.
	/**
	PriorityDrive1 = C
	PriorityDrive2 = E
	*/

	drive.Set(TDriveUnit(RFs::GetSystemDrive()).Name());
	parse.Set(KPreferredDriveListFilePath, &drive, NULL);
	err = fs.Delete(parse.FullName()); //ignore error
	iServer->ResetRepositoryL(); //resetting it is as good as deleting the repository to simulate fresh startup.
	
	file = CFileMan::NewL(fs);
	CleanupStack::PushL(file);
	parse.Set(KPreferredDriveListFilePath, NULL, NULL);
	config = parse.FullName();
	err = file->Copy(KMsgTestPriorityDriveListFile5, config); //ignore error
	CleanupStack::PopAndDestroy(file);

	delete iServer;
	TRAP(err, iServer = CMsvServer::NewL());
	MY_ASSERT_EQUALS(err, KErrNone);

	iDriveList = CMsvPreferredDriveList::GetDriveList();
	MY_ASSERT_EQUALS(2, iDriveList->Count()); //C and E.
	
	err = CMsvPreferredDriveList::GetDriveList()->GetDriveIndex(EDriveD, driveIndex);
	MY_ASSERT_EQUALS(KErrNotFound, err); //EDriveD does not exist in the drive list.
	err = CMsvPreferredDriveList::GetDriveList()->GetDriveIndex(EDriveF, driveIndex);
	MY_ASSERT_EQUALS(KErrNotFound, err); //EDriveF does not exist in the drive list.
	err = CMsvPreferredDriveList::GetDriveList()->GetDriveIndex(EDriveE, driveIndex);
	MY_ASSERT_EQUALS((TUint)1, driveIndex);
	err = CMsvPreferredDriveList::GetDriveList()->GetDriveIndex(RFs::GetSystemDrive(), driveIndex);
	MY_ASSERT_EQUALS((TUint)0, driveIndex);
	// 5.2 Check CenRep.
	repository = CRepository::NewL(KUidConfigFile);
	CleanupStack::PushL(repository);
	
	User::LeaveIfError(repository->Get(driveIndex, driveNum));
	MY_ASSERT_EQUALS(driveNum, (TInt)RFs::GetSystemDrive());
	User::LeaveIfError(repository->Get(++driveIndex, driveNum));
	MY_ASSERT_EQUALS(driveNum, (TInt)EDriveE);
	User::LeaveIfError(repository->Get(++driveIndex, driveNum));
	MY_ASSERT_TRUE(driveNum > EDriveZ);
	User::LeaveIfError(repository->Get(++driveIndex, driveNum));
	MY_ASSERT_TRUE(driveNum > EDriveZ); //no other drive after EDriveE in CenRep. the CenRep will instead 
										//have values > EDriveZ to represent invalid or unset values.
	
	CleanupStack::PopAndDestroy(); //repository
	
	
	// 6. Config file with repetitions.
	/**
	PriorityDrive1 = C
	PriorityDrive2 = D *
	PriorityDrive3 = D *
	PriorityDrive4 = F
	PriorityDrive5 = P
	PriorityDrive6 = Q
	PriorityDrive7 = R
	*/

	drive.Set(TDriveUnit(RFs::GetSystemDrive()).Name());
	parse.Set(KPreferredDriveListFilePath, &drive, NULL);
	err = fs.Delete(parse.FullName()); //ignore error
	iServer->ResetRepositoryL(); //resetting it is as good as deleting the repository to simulate fresh startup.
	
	file = CFileMan::NewL(fs);
	CleanupStack::PushL(file);
	parse.Set(KPreferredDriveListFilePath, NULL, NULL);
	config = parse.FullName();
	err = file->Copy(KMsgTestPriorityDriveListFile6, config); //ignore error
	CleanupStack::PopAndDestroy(file);

	delete iServer;
	TRAP(err, iServer = CMsvServer::NewL());
	MY_ASSERT_EQUALS(err, KErrNone);

	iDriveList = CMsvPreferredDriveList::GetDriveList();
	MY_ASSERT_EQUALS(3, iDriveList->Count()); //C, D and F.
	
	err = CMsvPreferredDriveList::GetDriveList()->GetDriveIndex(EDriveE, driveIndex);
	MY_ASSERT_EQUALS(KErrNotFound, err); //EDriveE does not exist in the drive list.
	err = CMsvPreferredDriveList::GetDriveList()->GetDriveIndex(EDriveF, driveIndex);
	MY_ASSERT_EQUALS((TUint)2, driveIndex);
	err = CMsvPreferredDriveList::GetDriveList()->GetDriveIndex(EDriveD, driveIndex);
	MY_ASSERT_EQUALS((TUint)1, driveIndex);
	err = CMsvPreferredDriveList::GetDriveList()->GetDriveIndex(RFs::GetSystemDrive(), driveIndex);
	MY_ASSERT_EQUALS((TUint)0, driveIndex);
	// 6.2 Check CenRep.
	repository = CRepository::NewL(KUidConfigFile);
	CleanupStack::PushL(repository);
	
	User::LeaveIfError(repository->Get(driveIndex, driveNum));
	MY_ASSERT_EQUALS(driveNum, (TInt)RFs::GetSystemDrive());
	User::LeaveIfError(repository->Get(++driveIndex, driveNum));
	MY_ASSERT_EQUALS(driveNum, (TInt)EDriveD);
	User::LeaveIfError(repository->Get(++driveIndex, driveNum));
	MY_ASSERT_EQUALS(driveNum, (TInt)EDriveF);
	User::LeaveIfError(repository->Get(++driveIndex, driveNum));
	MY_ASSERT_TRUE(driveNum > EDriveZ); //no other drive after EDriveF in CenRep. the CenRep will instead 
										//have values > EDriveZ to represent invalid or unset values.
	
	CleanupStack::PopAndDestroy(); //repository
	
	
	// 7. Config file that has 7 drives but has invalid keys.
	/**
	PriorityDrive10 = D
	PriorityDrive2 = C
	PriorityDrive3 = E
	PriorityDrive4 = F
	PriorityDrive5 = P
	PriorityDrive6 = Q
	PriorityDrive7 = R
	*/

	drive.Set(TDriveUnit(RFs::GetSystemDrive()).Name());
	parse.Set(KPreferredDriveListFilePath, &drive, NULL);
	err = fs.Delete(parse.FullName()); //ignore error
	iServer->ResetRepositoryL(); //resetting it is as good as deleting the repository to simulate fresh startup.
	
	file = CFileMan::NewL(fs);
	CleanupStack::PushL(file);
	parse.Set(KPreferredDriveListFilePath, NULL, NULL);
	config = parse.FullName();
	err = file->Copy(KMsgTestPriorityDriveListFile7, config); //ignore error
	CleanupStack::PopAndDestroy(file);

	delete iServer;
	TRAP(err, iServer = CMsvServer::NewL());
	MY_ASSERT_EQUALS(err, KErrNone);

	iDriveList = CMsvPreferredDriveList::GetDriveList();
	MY_ASSERT_EQUALS(3, iDriveList->Count()); //C, E and F.
	
	err = CMsvPreferredDriveList::GetDriveList()->GetDriveIndex(EDriveD, driveIndex);
	MY_ASSERT_EQUALS(KErrNotFound, err); //EDriveE does not exist in the drive list.
	err = CMsvPreferredDriveList::GetDriveList()->GetDriveIndex(EDriveF, driveIndex);
	MY_ASSERT_EQUALS((TUint)2, driveIndex);
	err = CMsvPreferredDriveList::GetDriveList()->GetDriveIndex(EDriveE, driveIndex);
	MY_ASSERT_EQUALS((TUint)1, driveIndex);
	err = CMsvPreferredDriveList::GetDriveList()->GetDriveIndex(RFs::GetSystemDrive(), driveIndex);
	MY_ASSERT_EQUALS((TUint)0, driveIndex);
	// 7.2 Check CenRep.
	repository = CRepository::NewL(KUidConfigFile);
	CleanupStack::PushL(repository);
	
	User::LeaveIfError(repository->Get(driveIndex, driveNum));
	MY_ASSERT_EQUALS(driveNum, (TInt)RFs::GetSystemDrive());
	User::LeaveIfError(repository->Get(++driveIndex, driveNum));
	MY_ASSERT_EQUALS(driveNum, (TInt)EDriveE);
	User::LeaveIfError(repository->Get(++driveIndex, driveNum));
	MY_ASSERT_EQUALS(driveNum, (TInt)EDriveF);
	User::LeaveIfError(repository->Get(++driveIndex, driveNum));
	MY_ASSERT_TRUE(driveNum > EDriveZ); //no other drive after EDriveF in CenRep. the CenRep will instead 
										//have values > EDriveZ to represent invalid or unset values.
	
	CleanupStack::PopAndDestroy(); //repository
	
	// Restore.
	drive.Set(TDriveUnit(RFs::GetSystemDrive()).Name());
	parse.Set(KPreferredDriveListFilePath, &drive, NULL);
	err = fs.Delete(parse.FullName()); //ignore error
	file = CFileMan::NewL(fs);
	CleanupStack::PushL(file);
	parse.Set(KPreferredDriveListFilePath, NULL, NULL);
	config = parse.FullName();
	err = file->Copy(KMsgTestPriorityDriveListFile2, config); //ignore error
	CleanupStack::PopAndDestroy(file);
	
	fs.Close();
	
	}
	


void CTestMsvServer::TestChangeDriveWithFullDriveListL()
	{
	_LIT(KFunction, "TestChangeDriveWithFullDriveListL");
	INFO_PRINTF1(KFunction);

	TInt err = KErrNone;
	TUint curDriveIndex = 0;
	TMsvPreferredDrive drive;
	TUint driveIndex = 0;
	
	// 1. Change drive to a drive not in the drive list.
	// 1.1 Drive list is not full.
	err = iServer->ChangeDrive(EDriveK, &(iActiveWaiter->iStatus));
	iActiveWaiter->WaitActive();
	MY_ASSERT_EQUALS(KErrNone, err);
	curDriveIndex = CMsvPreferredDriveList::GetDriveList()->CurrentDriveIndex();
	MY_ASSERT_EQUALS((TUint)0, curDriveIndex);
	CMsvPreferredDriveList::GetDriveList()->DriveInfoL(curDriveIndex, drive);
	MY_ASSERT_EQUALS(drive.driveNum, EDriveK);
	MY_ASSERT_EQUALS(drive.status, EMsvMessageStoreAvailableStatus);
	
	// 1.2 Drive list is full, current drive is not the last drive in the list.
	// Filling up the drive list..
	TUint priority = 6;
	TRAP(err, iServer->AddDriveToListL(EDriveG, priority, NULL));
	MY_ASSERT_EQUALS(KErrNone, err);
	++priority;
	TRAP(err, iServer->AddDriveToListL(EDriveH, priority, NULL));
	MY_ASSERT_EQUALS(KErrNone, err);
	// Note down index of the last drive, EDriveH.
	err = CMsvPreferredDriveList::GetDriveList()->GetDriveIndex(EDriveH, driveIndex);
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS(driveIndex, (TUint)6);
	// Test Change drive.
	err = iServer->ChangeDrive(EDriveI, &(iActiveWaiter->iStatus));
	iActiveWaiter->WaitActive();
	MY_ASSERT_EQUALS(KErrNone, err);
	curDriveIndex = CMsvPreferredDriveList::GetDriveList()->CurrentDriveIndex();
	MY_ASSERT_EQUALS((TUint)0, curDriveIndex);
	CMsvPreferredDriveList::GetDriveList()->DriveInfoL(curDriveIndex, drive);
	MY_ASSERT_EQUALS(drive.driveNum, EDriveI);
	MY_ASSERT_EQUALS(drive.status, EMsvMessageStoreAvailableStatus);
	// EDriveI is also the first drive in the drive list.
	err = CMsvPreferredDriveList::GetDriveList()->GetDriveIndex(EDriveI, driveIndex);
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS(driveIndex, (TUint)0);
	// EDriveH was the last drive and has been removed from the list.
	err = CMsvPreferredDriveList::GetDriveList()->GetDriveIndex(EDriveH, driveIndex);
	MY_ASSERT_EQUALS(KErrNotFound, err);
	
	// 1.3 Drive list is full, current drive is the last drive in the list.
	// Reboot server with all but the last drive, EDriveC, having unsupported message store.
	delete iServer;
	CMsvServer::ResetRepositoryL();
	RFs fs;
	CleanupClosePushL(fs);
	User::LeaveIfError(fs.Connect());
	CFileMan* file = CFileMan::NewL(fs);
	err = file->Copy(KMsgTestPriorityDriveListFile1, KMsgTestPriorityDriveListFile); //ignore error
	TRAP(err, iServer = CMsvServer::NewL());
	MY_ASSERT_EQUALS(KErrNone, err);
	// Place Index files to make the drives unsupported.
	for(TInt i = EDriveI; i >= EDriveD; --i)
		{
		TParse parse;
		TPtrC drivePtr(TDriveUnit(i).Name());
		// Place an Index file in Mail2 folder.
		parse.Set(KMsgMail2Folder, &drivePtr, NULL); 
		err = fs.MkDirAll(parse.DriveAndPath()); //ignore error
		TFileName index = parse.FullName();
		err = file->Copy(KMsgTestIndexFile, index); //ignore error
		// Delete the DB if there was any.
		parse.Set(KDbFileName, &drivePtr, NULL);
		err = fs.Delete(parse.FullName()); // ignore error
		priority = 1;
		TRAP(err, iServer->AddDriveToListL((TDriveNumber)i, priority, NULL));
		MY_ASSERT_EQUALS(KErrNone, err);
		}
	// Note down index of the second last drive, EDriveI.
	err = CMsvPreferredDriveList::GetDriveList()->GetDriveIndex(EDriveI, driveIndex);
	MY_ASSERT_EQUALS(KErrNone, err);
	MY_ASSERT_EQUALS(driveIndex, (TUint)5);
	// Test change drive.
	err = iServer->ChangeDrive(EDriveK, &(iActiveWaiter->iStatus));
	iActiveWaiter->WaitActive();
	MY_ASSERT_EQUALS(KErrNone, err);
	curDriveIndex = CMsvPreferredDriveList::GetDriveList()->CurrentDriveIndex();
	MY_ASSERT_EQUALS((TUint)0, curDriveIndex);
	CMsvPreferredDriveList::GetDriveList()->DriveInfoL(curDriveIndex, drive);
	MY_ASSERT_EQUALS(drive.driveNum, EDriveK);
	MY_ASSERT_EQUALS(drive.status, EMsvMessageStoreAvailableStatus);
	// EDriveC, the last drive, was the current drive. So the second last drive, EDriveF, was removed.
	err = CMsvPreferredDriveList::GetDriveList()->GetDriveIndex(EDriveI, driveIndex);
	MY_ASSERT_EQUALS(KErrNotFound, err);
	
	// 1.4 Revert all these manipulations to a state before this test.
	delete iServer;
	iServer = NULL;
	CMsvServer::ResetRepositoryL();
	err = file->Copy(KMsgTestPriorityDriveListFile10, KMsgTestPriorityDriveListFile); //ignore error
	TParse parse;
	TPtrC drivePtr = TDriveUnit(EDriveG).Name();
	parse.Set(KDbFileName, &drivePtr, NULL);
	TFileName dbFile = parse.FullName();
	RSqlDatabase::Delete(dbFile);	// Ignore error.
	drivePtr.Set(TDriveUnit(EDriveH).Name());
	parse.Set(KDbFileName, &drivePtr, NULL);
	dbFile = parse.FullName();
	RSqlDatabase::Delete(dbFile);	// Ignore error.
	drivePtr.Set(TDriveUnit(EDriveI).Name());
	parse.Set(KDbFileName, &drivePtr, NULL);
	dbFile = parse.FullName();
	RSqlDatabase::Delete(dbFile);	// Ignore error.
	drivePtr.Set(TDriveUnit(EDriveK).Name());
	parse.Set(KDbFileName, &drivePtr, NULL);
	dbFile = parse.FullName();
	RSqlDatabase::Delete(dbFile);	// Ignore error.

	for(TInt i = EDriveK; i >= EDriveD; --i)
		{
		TParse parse;
		TPtrC drivePtr(TDriveUnit(i).Name());
		parse.Set(KMsgIndexFile, &drivePtr, NULL); 
		err = fs.Delete(parse.FullName()); //ignore error
		err = file->RmDir(parse.DriveAndPath());
		}
	
	delete file;
	CleanupStack::PopAndDestroy(); // fs
	}
	


void CTestMsvServer::TestChangeDriveL()
	{
	_LIT(KFunction, "TestChangeDriveL");
	INFO_PRINTF1(KFunction);
	
	TInt err = KErrNone;
	TUint curDriveIndex = 0;
	TMsvPreferredDrive drive;
	TUint driveIndex = 0;
	
	// 1. Change drive to a drive not in the drive list.
	/* Moved to TestChangeDriveWithFullDriveListL() */

	// 2. Change drive to a drive with old version of the message store.
	err = iServer->ChangeDrive(EDriveF, &(iActiveWaiter->iStatus));
	MY_ASSERT_EQUALS(KErrNotSupported, err);

	err = iServer->ChangeDrive(EDriveF, NULL);
	MY_ASSERT_EQUALS(KErrNotSupported, err);
	
	// 3. Change drive to a drive with EMsvMessageStoreUnavailableStatus.
	err = iServer->ChangeDrive(EDriveD, &(iActiveWaiter->iStatus));
	iActiveWaiter->WaitActive();
	curDriveIndex = CMsvPreferredDriveList::GetDriveList()->CurrentDriveIndex();
	MY_ASSERT_EQUALS((TUint)0, curDriveIndex);
	
	CMsvPreferredDriveList::GetDriveList()->DriveInfoL(curDriveIndex, drive);
	MY_ASSERT_EQUALS(drive.driveNum, EDriveD);
	MY_ASSERT_EQUALS(drive.status, EMsvMessageStoreAvailableStatus);
	
	
	// 4. Change drive to a drive with EMsvMessageStoreAvailableStatus.
	iServer->iDebug = EFalse;  // To ensure mailinit is run (Code Coverage).
	err = iServer->ChangeDrive(RFs::GetSystemDrive(), &(iActiveWaiter->iStatus));
	iActiveWaiter->WaitActive();
	MY_ASSERT_EQUALS(KErrNone, err);
	curDriveIndex = CMsvPreferredDriveList::GetDriveList()->CurrentDriveIndex();
	MY_ASSERT_EQUALS((TUint)0, curDriveIndex);
	iServer->iDebug = ETrue;
	
	CMsvPreferredDriveList::GetDriveList()->DriveInfoL(curDriveIndex, drive);
	MY_ASSERT_EQUALS(drive.driveNum, RFs::GetSystemDrive());
	MY_ASSERT_EQUALS(drive.status, EMsvMessageStoreAvailableStatus);
	
	// 4.1 Same as 4 but synchronous.
	delete iServer->iSearchSortCacheManager;
	iServer->iSearchSortCacheManager = NULL;
	err = iServer->ChangeDrive(EDriveD, NULL);
	MY_ASSERT_EQUALS(KErrNone, err);
	curDriveIndex = CMsvPreferredDriveList::GetDriveList()->CurrentDriveIndex();
	MY_ASSERT_EQUALS((TUint)0, curDriveIndex);
	
	CMsvPreferredDriveList::GetDriveList()->DriveInfoL(curDriveIndex, drive);
	MY_ASSERT_EQUALS(drive.driveNum, EDriveD);
	MY_ASSERT_EQUALS(drive.status, EMsvMessageStoreAvailableStatus);
	
	
	// 5. Change drive to the same drive.
	err = iServer->ChangeDrive(EDriveD, NULL);
	MY_ASSERT_EQUALS(KErrNotSupported, err);
	curDriveIndex = CMsvPreferredDriveList::GetDriveList()->CurrentDriveIndex();
	MY_ASSERT_EQUALS((TUint)0, curDriveIndex);
	
	err = iServer->ChangeDrive(EDriveD, &(iActiveWaiter->iStatus));
	MY_ASSERT_EQUALS(KErrNotSupported, err);
	curDriveIndex = CMsvPreferredDriveList::GetDriveList()->CurrentDriveIndex();
	MY_ASSERT_EQUALS((TUint)0, curDriveIndex);
	
	CMsvPreferredDriveList::GetDriveList()->DriveInfoL(curDriveIndex, drive);
	MY_ASSERT_EQUALS(drive.driveNum, EDriveD);
	MY_ASSERT_EQUALS(drive.status, EMsvMessageStoreAvailableStatus);
	
	// 6. Simulating change drive fail.
	// Creating message store in G:
	TUint priority = 3;
	driveIndex = 0;
	TRAP(err, iServer->AddDriveToListL(EDriveG, priority, NULL));
	MY_ASSERT_EQUALS(err, KErrNone);
	err = CMsvPreferredDriveList::GetDriveList()->GetDriveIndex(EDriveG, driveIndex);
	MY_ASSERT_EQUALS(KErrNone, err);
	CMsvPreferredDriveList::GetDriveList()->DriveInfoL(driveIndex, drive);
	MY_ASSERT_EQUALS(EDriveG, drive.driveNum);
	MY_ASSERT_EQUALS(EMsvMessageStoreUnavailableStatus, drive.status);  
	
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CFileMan* file = CFileMan::NewL(fs);
	CleanupStack::PushL(file);
	
	TPtrC drivePtr(TDriveUnit(EDriveG).Name());
	TParse parse;
	parse.Set(KDbFileName, &drivePtr, NULL);
	TFileName dbFile = parse.FullName();
	RSqlDatabase::Delete(dbFile);	 // Ignore error.
	RSqlDatabase::Copy(KBaseDBFileName, dbFile);
	
	err = iServer->ChangeDrive(EDriveG, NULL);
	MY_ASSERT_EQUALS(KErrAlreadyExists, err);
	curDriveIndex = CMsvPreferredDriveList::GetDriveList()->CurrentDriveIndex();
	MY_ASSERT_EQUALS((TUint)0, curDriveIndex);
	
	RSqlDatabase::Delete(dbFile);
	fs.Close();
	CleanupStack::PopAndDestroy();  
	}
	
	
	
	
	
void CTestMsvServer::TestDriveOperationsL()
	{
	_LIT(KFunction, "TestDriveOperationsL");
	INFO_PRINTF1(KFunction);
	
	/* A snapshot of msgprioritydrivelist.ini is:
	
	PriorityDrive1 = C
	PriorityDrive2 = D
	PriorityDrive3 = E
	PriorityDrive4 = F
	PriorityDrive5 = P
	PriorityDrive6 = R
	PriorityDrive7 = Q
	*/
		
	/**
	 * The function tests the disk change observer functionality
	 * implemented by CMsvServer.
	 * We call the implemented functions directly and test outcomes.
	 */
	
	TUint priority = 0;
	TInt err = KErrNone;
	TUint driveIndex = 0;
	CFileMan* file = NULL;
	TFileName index;
	RFs fs;
	User::LeaveIfError(fs.Connect());
	
	
	/**
	 * PRE-CONDITIONS - Drive list now is:
	 *  C - EMsvMessageStoreAvailableStatus
	 *  D - EMsvMessageStoreUnavailableStatus
	 *  E - EMsvMessageStoreUnavailableStatus
	 *  F - EMsvMessageStoreNotSupportedStatus
	 */
	// ** I. DISK REMOVAL TESTS:
	// 1. Removal of a disk from drive with EMsvMessageStoreUnavailableStatus.
	iServer->DiskRemoved(EDriveE, ETrue);
	MY_ASSERT_EQUALS(iServer->iNotification, EMsvNullNotification);
	
	
	// 2. Removal of a disk from drive with EMsvMessageStoreNotSupportedStatus.
	iServer->DiskRemoved(EDriveF, ETrue);
	MY_ASSERT_EQUALS(iServer->iNotification, EMsvNullNotification);
	
	
	// 3. Removal of a disk from drive with EMsvMessageStoreAvailableStatus - Non-current drive.
	//Note: Change drive to EDriveD to create message store in it, and change back to EDriveC.
	priority = 1;
	TRAP(err, iServer->UpdateDrivePriorityL(EDriveD, priority));
	MY_ASSERT_EQUALS(KErrNone, err); //D, C, E and F.
	priority = 1;
	TRAP(err, iServer->UpdateDrivePriorityL(RFs::GetSystemDrive(), priority));
	MY_ASSERT_EQUALS(KErrNone, err); //C, D, E and F.
	
	iServer->DiskRemoved(EDriveD, ETrue);
	MY_ASSERT_EQUALS(iServer->iNotification, EMsvRefreshMessageView);
	
	//Restore the drive status. We didn't really remove the drive in the test above.
	CMsvPreferredDriveList* driveList = CMsvPreferredDriveList::GetDriveList();
	err = driveList->GetDriveIndex(EDriveD, driveIndex);
	ASSERT_EQUALS(KErrNone, err);
	(*driveList)[driveIndex].driveId = KMsvInvalidDriveId;
	(*driveList)[driveIndex].status = EMsvMessageStoreUnavailableStatus;
	TParse parse;
	TPtrC drive = TDriveUnit(EDriveD).Name();
	parse.Set(KDbFileName, &drive, NULL);
	TFileName dbName = parse.FullName();
	RSqlDatabase::Delete(dbName);
	
	
	// 4. Removal of a disk from a drive with EMsvMessageStoreAvailableStatus - Current drive.
	//Note: Change drive to EDriveD to make it the current drive.
	priority = 1;
	TRAP(err, iServer->UpdateDrivePriorityL(EDriveD, priority));
	MY_ASSERT_EQUALS(KErrNone, err); //D, C, E and F.
	
	iServer->DiskRemoved(EDriveD, ETrue);
	MY_ASSERT_EQUALS(iServer->iNotification, EMsvMediaChanged);
	
	//Restore the drive status. We didn't really remove the drive in the test above.
	err = driveList->GetDriveIndex(EDriveD, driveIndex);
	ASSERT_EQUALS(KErrNone, err);
	(*driveList)[driveIndex].driveId = KMsvInvalidDriveId;
	(*driveList)[driveIndex].status = EMsvMessageStoreUnavailableStatus;
	parse.Set(KDbFileName, &drive, NULL);
	dbName = parse.FullName();
	err = RSqlDatabase::Delete(dbName);
	
	// 4.1. (Code Coverage)
	iServer->DiskChanged(EDriveD);
		
	//Restore the drive status. We didn't really remove the drive in the test above.
	err = driveList->GetDriveIndex(EDriveD, driveIndex);
	ASSERT_EQUALS(KErrNone, err);
	(*driveList)[driveIndex].driveId = KMsvInvalidDriveId;
	(*driveList)[driveIndex].status = EMsvDriveDiskNotAvailableStatus;
	parse.Set(KDbFileName, &drive, NULL);
	dbName = parse.FullName();
	err = RSqlDatabase::Delete(dbName);
	
		
	// ** II. DISK INSERTION TESTS:
	// 5. Insert a disk with message store in a highest priority drive.
	//Note: Same test can be applied to an empty disk. So here we test with empty disk.
	iServer->DiskInserted(EDriveD, EFalse);
	MY_ASSERT_EQUALS(iServer->iNotification, EMsvMediaChanged);
	
	
	// 6. Insert a disk with message store in a lower priority drive.
	//Note: Same test can be applied to an empty disk. So here we test with non-empty disk.
	//Note: Change drive to EDriveD to create a message store on it and then back to EDriveC.
	priority = 1;
	TRAP(err, iServer->UpdateDrivePriorityL(EDriveD, priority));
	MY_ASSERT_EQUALS(KErrNone, err); //D, C, E and F.
	priority = 1;
	TRAP(err, iServer->UpdateDrivePriorityL(RFs::GetSystemDrive(), priority));
	MY_ASSERT_EQUALS(KErrNone, err); //C, D, E and F.
	
	//Restore the drive status. We didn't really remove the drive in the test above.
	err = driveList->GetDriveIndex(EDriveD, driveIndex);
	ASSERT_EQUALS(KErrNone, err);
	(*driveList)[driveIndex].driveId = KMsvInvalidDriveId;
	(*driveList)[driveIndex].status = EMsvDriveDiskNotAvailableStatus;
	iServer->DiskInserted(EDriveD, EFalse);
	MY_ASSERT_EQUALS(iServer->iNotification, EMsvRefreshMessageView);
	
	
	// 7. Insert a disk with corrupt message store in lower priority drive.
	//Note: Remove EDriveE from the drive list and add it back in a lower position.
	TRAP(err, iServer->RemoveDriveFromListL(EDriveE));
	MY_ASSERT_EQUALS(KErrNone, err);
	drive.Set(TDriveUnit(EDriveE).Name());
	parse.Set(KDbFileName, &drive, NULL);
	err = fs.MkDirAll(parse.DriveAndPath());
	RSqlDatabase::Delete(parse.FullName()); //delete anything that might have been here.
	TRAP(err, CMsvDBAdapter::CreateDBL(EDriveE, ETrue));  //place an invalid database in EDriveE.
	MY_ASSERT_EQUALS(err, KErrNone);
	priority = 3;
	TRAP(err, iServer->AddDriveToListL(EDriveE, priority, NULL)); //now E has no message store in it as it would
	MY_ASSERT_EQUALS(KErrNone, err);							  //have been wiped.
	TRAP(err, CMsvDBAdapter::CreateDBL(EDriveE, ETrue)); //create an invalid DB again to simulate disk insertion in
	MY_ASSERT_EQUALS(err, KErrNone);					 //the next step.
	
	iServer->DiskInserted(EDriveE, EFalse);
	MY_ASSERT_EQUALS(iServer->iNotification, EMsvMessageStoreCorrupt);
	
	
	// 8. Insert a disk with corrupt message store in higher priority drive.
	//Note: Update priority of EDriveF to be the highest.
	//  We had placed an unsupported store in EDriveF. Adding it in the highest position will not
	//  create a valid message store on it. We can then perform our tests on it.
	priority = 1;
	TRAP(err, iServer->UpdateDrivePriorityL(EDriveF, priority)); //EDriveF added to highest position
	MY_ASSERT_EQUALS(KErrNone, err);
	
	drive.Set(TDriveUnit(EDriveF).Name());
	parse.Set(KDbFileName, &drive, NULL);
	err = fs.Delete(parse.FullName()); //delete the database file to make the disk 'empty'
	
	iServer->DiskInserted(EDriveF, ETrue);
	MY_ASSERT_EQUALS(iServer->iNotification, EMsvMediaChanged);
	
	
	// 9. Insert a disk with old version message store in lower priority drive.
	//Note: Remove EDriveF from the drive list and add it back in a lower position after placing
	//  an older version Index file in it.
	TRAP(err, iServer->RemoveDriveFromListL(EDriveF));
	MY_ASSERT_EQUALS(KErrNone, err);
	
	drive.Set(TDriveUnit(EDriveF).Name());
	parse.Set(KDbFileName, &drive, NULL);
	err = fs.Delete(parse.FullName());  //delete the database file previously created
	parse.Set(KMsgMail2Folder, &drive, NULL);
	err = fs.MkDirAll(parse.DriveAndPath());
	file = CFileMan::NewL(fs);
	index = parse.FullName();
	err = file->Copy(KMsgTestIndexFile, index); //place index file in mail2
	delete file;
	
	priority = 3;
	TRAP(err, iServer->AddDriveToListL(EDriveF, priority, NULL)); //EDriveF added to a lower position
	MY_ASSERT_EQUALS(KErrNone, err);
	
	iServer->DiskInserted(EDriveF, EFalse);
	MY_ASSERT_EQUALS(iServer->iNotification, EMsvMessageStoreNotSupported);
	
	
	// 10. Insert a disk with old version message store in higher priority drive.
	//Note: Remove EDriveF from the drive list and add it back in the highest position after placing
	//  an older version Index file in it.
	TRAP(err, iServer->RemoveDriveFromListL(EDriveF));
	MY_ASSERT_EQUALS(KErrNone, err);
	drive.Set(TDriveUnit(EDriveF).Name());
	parse.Set(KMsgMail2Folder, &drive, NULL);
	err = fs.MkDirAll(parse.DriveAndPath()); //ignore error
	file = CFileMan::NewL(fs);
	index = parse.FullName();
	err = file->Copy(KMsgTestIndexFile, index); //ignore error
	delete file;
	priority = 1;
	TRAP(err, iServer->AddDriveToListL(EDriveF, priority, NULL)); //EDriveF added to a lower position
	MY_ASSERT_EQUALS(KErrNone, err);
	
	iServer->DiskInserted(EDriveF, EFalse);
	MY_ASSERT_EQUALS(iServer->iNotification, EMsvMessageStoreNotSupported);
	fs.Close();
	
	}
	
	

void CTestMsvServer::TestMsvUtils()
	{
	_LIT(KFunction, "TestMsvUtils");
	INFO_PRINTF1(KFunction);
	
	/* A snapshot of msgprioritydrivelist.ini is:
	
	PriorityDrive1 = C
	PriorityDrive2 = D
	PriorityDrive3 = E
	PriorityDrive4 = F
	PriorityDrive5 = G
	PriorityDrive6 = H
	PriorityDrive7 = I
	*/
	
	/**
	 * The function tests CMsvUtils changes required for Preq557.
	 * Including the testing here to avoid another test step for a very minor change.
	 */
	
	TMsvId id = 0;
	TUint driveId = 0;
	TUint priority = 0;
	TInt err = KErrNone;
	TFileName file;
	
	
	// 1. Check unmasking of the TMsvIds and getting driveId from a masked driveId.
	id = KMsvGlobalInBoxIndexEntryId;
	driveId = 1;
	id = MaskTMsvId(driveId, id);
	driveId = MsvUtils::GetDriveId(id);
	MY_ASSERT_EQUALS(driveId, (TUint)1);
	
	id = MsvUtils::UnmaskTMsvId(id);
	MY_ASSERT_EQUALS(KMsvGlobalInBoxIndexEntryId, id);
	
	id = MsvUtils::UnmaskTMsvId(id);
	MY_ASSERT_EQUALS(KMsvGlobalInBoxIndexEntryId, id); //id unmasked is the same as original non-masked id.
	
	/*driveId = MsvUtils::GetDriveId(id);
	MY_ASSERT_EQUALS(driveId, (TUint)1);*/ // invalidated after fix for DEF131167
	
	// 2. Get driveId of the current drive.
	//driveId = MsvUtils::GetCurrentDriveId();
	//MY_ASSERT_EQUALS(driveId, (TUint)1); //is currently one, C:.
	
	priority = 1;
	TRAP(err, iServer->UpdateDrivePriorityL(EDriveD, priority));
	MY_ASSERT_EQUALS(KErrNone, err); //D, C, E and F.
	
	//driveId = MsvUtils::GetCurrentDriveId();
	//MY_ASSERT_EQUALS(driveId, (TUint)2); //is currently one, D:.
	
	
	// 3. Test construction of entry names.
	TParse parse;
	TPtrC drivePtr(TDriveUnit(EDriveD).Name());
	parse.Set(iContext->MessageFolder(), NULL, NULL);
	file = parse.FullName(); //should already be seeded with Mail2 folder fully qualified path
	MsvUtils::ConstructEntryName(KMsvLocalServiceIndexEntryId, KMsvLocalServiceIndexEntryId, file, MsvUtils::EPath);
	MY_ASSERT_TRUE(file == _L("D:\\Private\\1000484b\\Mail2\\00001001_S\\")); //service folder path
	
	file.Zero();
	file = parse.FullName();
	MsvUtils::ConstructEntryName(KMsvGlobalInBoxIndexEntryId, KMsvGlobalInBoxIndexEntryId, file, MsvUtils::EFolder);
	MY_ASSERT_TRUE(file == _L("D:\\Private\\1000484b\\Mail2\\00001002_F\\")); //folder entry
	
	file.Zero();
	file = parse.FullName();
	MsvUtils::ConstructEntryName(KMsvLocalServiceIndexEntryId, KMsvGlobalInBoxIndexEntryId, file, MsvUtils::EFolder);
	MY_ASSERT_TRUE(file == _L("D:\\Private\\1000484b\\Mail2\\00001001_S\\2\\00001002_F\\")); //folder entry
	
	file.Zero();
	file = parse.FullName();
	MsvUtils::ConstructEntryName(KMsvLocalServiceIndexEntryId, KFirstFreeEntryId, file, MsvUtils::EStore);
	MY_ASSERT_TRUE(file == _L("D:\\Private\\1000484b\\Mail2\\00001001_S\\0\\00100000")); //message entry
	
	
	// 4. Testing entry validation.
	TMsvEntry entry;
	//Negative tests
	entry.iType = KUidMsvNullEntry;
	MY_ASSERT_FALSE(MsvUtils::ValidEntry(entry)); //invalid type
	
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm = KUidMsvNullEntry;
	MY_ASSERT_FALSE(MsvUtils::ValidEntry(entry)); //invalid mtm
	
	entry.iMtm = KUidMsvMessageEntry;
	entry.iParentId = KMsvNullIndexEntryId;
	MY_ASSERT_FALSE(MsvUtils::ValidEntry(entry)); //invalid parent
	
	entry.iType = KUidMsvServiceEntry;
	entry.iParentId = KMsvRootIndexEntryId;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	entry.iId = KMsvGlobalInBoxIndexEntryId;
	MY_ASSERT_FALSE(MsvUtils::ValidEntry(entry)); //service entry with serviceId != id
	
	entry.iType = KUidMsvMessageEntry;
	MY_ASSERT_FALSE(MsvUtils::ValidEntry(entry)); //non-service entry owned by root
	
	entry.iParentId = KMsvLocalServiceIndexEntryId;
	entry.iType = KUidMsvServiceEntry;
	MY_ASSERT_FALSE(MsvUtils::ValidEntry(entry)); //service entry not owned by root
	
	//Positive tests
	entry.iType = KUidMsvMessageEntry;
	entry.iParentId = KMsvGlobalInBoxIndexEntryId;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	MY_ASSERT_TRUE(MsvUtils::ValidEntry(entry)); //message entry not under root
	
	entry.iType = KUidMsvServiceEntry;
	entry.iParentId = KMsvRootIndexEntryId;
	MY_ASSERT_TRUE(MsvUtils::ValidEntry(entry, ETrue)); //iServiceId need not be iId for a new entry
	
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	entry.iId = KMsvLocalServiceIndexEntryId;
	MY_ASSERT_TRUE(MsvUtils::ValidEntry(entry, ETrue)); //iServiceId should be iId for an already created entry
	
	}
	
	
	
void CTestMsvServer::TestStoreManager()
	{
	_LIT(KFunction, "TestStoreManager");
	INFO_PRINTF1(KFunction);
	
	TMsvId id = 0;
	TFileName file;
	TParse parse;
	TPtrC drivePtr(TDriveUnit(RFs::GetSystemDrive()).Name());
	file = _L("Some junk name that should be wiped");
	TMsvId owningService = 0;
	TUint priority = 0;
	TInt err = KErrNone;
	TUint driveId = 0;
	
	
	// 1. Test file path generation - pass some file path to the function.
	id = KMsvLocalServiceIndexEntryId;
	iStoreManager.GetOwningServiceAndFilePath(id, file, owningService); //current drive.
	MY_ASSERT_TRUE(file == iContext->MessageFolder());
	MY_ASSERT_EQUALS(owningService, KMsvLocalServiceIndexEntryId);
	
	
	// 2. Test creation - pass empty file path.
	//Note: Lets change the drive to EDriveD so that we can test file path generation
	//  in external media.
	priority = 1;
	TRAP(err, iServer->UpdateDrivePriorityL(EDriveD, priority));
	MY_ASSERT_EQUALS(KErrNone, err); //D, C, E and F.
	
	file = _L("");
	iStoreManager.GetOwningServiceAndFilePath(id, file, owningService); //current drive.
	MY_ASSERT_TRUE(file == iContext->MessageFolder());
	MY_ASSERT_EQUALS(owningService, KMsvLocalServiceIndexEntryId);  
	}



void CTestMsvServer::TestContextComplete()
	{
	_LIT(KFunction, "TestContextComplete");
	INFO_PRINTF1(KFunction);
	
	iServer->ResetRepositoryL();
	delete iServer;
	iServer = 0;
	
	TRAPD(err, iServer = CMsvServer::NewL(EFalse));
	ASSERT_EQUALS(err, KErrNone);   
	}
	
	
	

void CTestMsvServer::TestIsMessageStoreDrivePresent()
	{
	_LIT(KFunction, "TestIsMessageStoreDrivePresent");
	INFO_PRINTF1(KFunction);

	RFs fs;
	fs.Connect();
	MY_ASSERT_EQUALS((TInt)ETrue, MessageServer::IsMessageStoreDrivePresentL(fs));
	fs.Close();
	}


void CTestMsvServer::TestDriveContainsStore()
	{
	_LIT(KFunction, "TestDriveContainsStore");
	INFO_PRINTF1(KFunction);

	RFs fs;
	fs.Connect();
	// Index file in EDriveF.
	MY_ASSERT_EQUALS((TInt)ETrue, MessageServer::DriveContainsStore(fs, EDriveF));
	
	// Old version Index file in EDriveD
	TParse parse;
	CFileMan* file = CFileMan::NewL(fs);
	CleanupStack::PushL(file);
	TPtrC drive = TDriveUnit(EDriveD).Name();
	parse.Set(KMsgOldIndexFile, &drive, NULL);
	TInt err = fs.MkDirAll(parse.DriveAndPath()); //ignore error
	TFileName index = parse.FullName();
	err = file->Copy(KMsgTestIndexFile, index); //ignore error
	MY_ASSERT_EQUALS((TInt)ETrue, MessageServer::DriveContainsStore(fs,  EDriveD));
	err = fs.Delete(index);
	err = file->RmDir(parse.DriveAndPath());
	CleanupStack::PopAndDestroy();   // file
	
	// Empty Drive
	MY_ASSERT_EQUALS((TInt)EFalse, MessageServer::DriveContainsStore(fs,  EDriveJ));
	
	// DB File.
	MY_ASSERT_EQUALS((TInt)ETrue, MessageServer::DriveContainsStore(fs,  EDriveC));
	
	fs.Close(); 
	}
	


void CTestMsvServer::TestIsMessageStoreSupported()
	{
	_LIT(KFunction, "TestIsMessageStoreSupported");
	INFO_PRINTF1(KFunction);
	
	// Empty Drive.
	MY_ASSERT_EQUALS((TInt)EFalse, MessageServer::IsMessageStoreSupported(EDriveJ));
	
	// DB already in use.
	MY_ASSERT_EQUALS((TInt)ETrue, MessageServer::IsMessageStoreSupported(EDriveC));
	
	// Version table does not exists.
	CMsvDBAdapter::CreateDBL(EDriveJ, EFalse);
	MY_ASSERT_EQUALS((TInt)EFalse, MessageServer::IsMessageStoreSupported(EDriveJ));
	
	// Version table has version invalid version (2).
	CMsvDBAdapter::CreateDBL(EDriveK, ETrue, ETrue);
	MY_ASSERT_EQUALS((TInt)EFalse, MessageServer::IsMessageStoreSupported(EDriveK));
	
	// Version table has valid version.
	CMsvDBAdapter::CreateDBL(EDriveL);
	MY_ASSERT_EQUALS((TInt)ETrue, MessageServer::IsMessageStoreSupported(EDriveL)); 
	}
	
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)  
void CTestMsvServer::TestHeaderTableMetadata()
	{
	_LIT(KFunction, "TestHeaderTableMetadata");
	INFO_PRINTF1(KFunction);

	TMsvId srcId = 0;
	TMsvId destId = 0;
	TUid mtmId = TUid::Null();
	RPointerArray<CFieldPair> fieldStruct;
	CMsvMessageDBAdapter* messageDBAdapter = &iServer->MessageDBAdapter();
	

	// 1. Change drive to an empty drive.
	TInt err = iServer->ChangeDrive(EDriveD, &(iActiveWaiter->iStatus));
	iActiveWaiter->WaitActive();
	MY_ASSERT_EQUALS(KErrNone, err);

	
	// 2. Create a header table.
	_LIT(KField1, "Field1");
	_LIT(KField2, "Field2");
	_LIT(KField3, "Field3");
	TPtrC errorMessage;
	mtmId = TUid::Uid(10);
	CFieldPair* field = new CFieldPair();
	TRAP(err, field->iFieldName = KField1().AllocL());
	MY_ASSERT_EQUALS(err, KErrNone);
	field->iFieldType = EIntegerField;
	TRAP(err, fieldStruct.AppendL(field));
	ASSERT_EQUALS(err, KErrNone);
	
	field = new CFieldPair();
	TRAP(err, field->iFieldName = KField2().AllocL());
	ASSERT_EQUALS(err, KErrNone);
	field->iFieldType = ETextField;
	TRAP(err, fieldStruct.AppendL(field));
	ASSERT_EQUALS(err, KErrNone);
	
	field = new CFieldPair();
	TRAP(err, field->iFieldName = KField3().AllocL());
	ASSERT_EQUALS(err, KErrNone);
	field->iFieldType = EDateField;
	TRAP(err, fieldStruct.AppendL(field));
	ASSERT_EQUALS(err, KErrNone);

	TRAP(err, messageDBAdapter->CreateHeaderTableL(mtmId, fieldStruct, errorMessage));
	MY_ASSERT_EQUALS(err, KErrNone);
	
	
	// 3. Check if metadata is created or not.
	TInt metadataIndex = messageDBAdapter->GetMetadataEntryIndex(mtmId);
	MY_ASSERT_TRUE(metadataIndex != KErrNotFound);
	
	
	// 4. Change drive to another drive and create same header table there.
	// Metadata entry should not be duplicated.
	err = iServer->ChangeDrive(EDriveC, &(iActiveWaiter->iStatus));
	iActiveWaiter->WaitActive();
	MY_ASSERT_EQUALS(KErrNone, err);
	
	TRAP(err, messageDBAdapter->CreateHeaderTableL(mtmId, fieldStruct, errorMessage));
	MY_ASSERT_EQUALS(err, KErrNone);
	// Delete the metadata entry. We should not be able to find it again.
	metadataIndex = messageDBAdapter->GetMetadataEntryIndex(mtmId);
	MY_ASSERT_TRUE(metadataIndex != KErrNotFound);
	
	delete messageDBAdapter->iHeaderDataList[metadataIndex];
	messageDBAdapter->iHeaderDataList.Remove(metadataIndex);
	
	metadataIndex = messageDBAdapter->GetMetadataEntryIndex(mtmId);
	MY_ASSERT_EQUALS(metadataIndex, KErrNotFound);
	
	
	fieldStruct.ResetAndDestroy();
	}
#endif


CTestSuite* CTestMsvServer::CreateSuiteL(const TDesC& aName)
// static
	{
	SUB_SUITE;
	ADD_ASYNC_TEST_STEP(TestAddDriveToListL);
	ADD_ASYNC_TEST_STEP(TestRemoveDriveFromListL);  
	ADD_ASYNC_TEST_STEP(TestUpdateRepositoryL);
	ADD_ASYNC_TEST_STEP(TestChangeDriveWithFullDriveListL);
	ADD_ASYNC_TEST_STEP(TestChangeDriveL);
	ADD_ASYNC_TEST_STEP(TestContextComplete);
	ADD_ASYNC_TEST_STEP(TestUpdateDrivePriorityL);
	ADD_ASYNC_TEST_STEP(TestDriveOperationsL);
	ADD_ASYNC_TEST_STEP(TestMsvUtils);
	ADD_ASYNC_TEST_STEP(TestStoreManager);
	ADD_ASYNC_TEST_STEP(TestIsMessageStoreDrivePresent);
	ADD_ASYNC_TEST_STEP(TestDriveContainsStore);
	ADD_ASYNC_TEST_STEP(TestIsMessageStoreSupported);
	ADD_ASYNC_TEST_STEP(TestCreatePreferredDriveListL);
	
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)  
	ADD_ASYNC_TEST_STEP(TestHeaderTableMetadata);
#endif
	END_SUITE;
	}

