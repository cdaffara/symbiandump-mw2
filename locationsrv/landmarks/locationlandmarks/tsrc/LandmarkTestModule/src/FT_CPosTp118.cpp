/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description: 
*   ?description_line
*
*/


//  INCLUDES
#include "FT_CPosTp118.h"
#include <EPos_CPosLandmarkDatabase.h>
#include <EPos_CPosLmDatabaseManager.h>
#include <d32dbms.h>
#include "FT_DatabaseUtility.h"

//  CONSTANTS

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CPosTp118::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp118::CloseTest()
    {
	iLog->Log(_L("CloseTest"));
	
    TRAPD(err, RemoveAllLmDatabasesL());
    if (err != KErrNone) iLog->Log(_L("RemoveAllLmDatabasesL() leaved"));
    
    TRAP(err, DeleteTp118FilesL());
    if (err != KErrNone) iLog->Log(_L("DeleteTp118FilesL() leaved"));
    
    delete iDatabases;
    iDatabases=NULL;
    }
       
// ---------------------------------------------------------
// CPosTp118::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp118::StartL()
    {
    TInt err = KErrNone;
    // Remove all databases
    RemoveAllLmDatabasesL();
    
    DeleteTp118FilesL();
    TBuf<100> buf;
    
    CPosLandmarkDatabase* lmd = NULL;
    	
    iLog->Log(_L("1) Create a valid landmark db"));
    // 1) Create a valid landmark db, it should be listed
	FTDatabaseUtility::CreateDatabaseL(_L("c:TP118.ldb"));
	
	// 2) List an verify that db created in step 1 is listed
	iLog->Log(_L("2) Verify that db created in step1 is listed:"));
	CPosLmDatabaseManager* dbManager = CPosLmDatabaseManager::NewL();
    CleanupStack::PushL(dbManager);
	iDatabases = dbManager->ListDatabasesLC();
	CleanupStack::Pop(iDatabases);

	TInt res = VerifyList(iDatabases, _L("file://C:TP118.ldb"));
	AssertTrueSecL(res, _L("file://C:TP118.LDB was not found"));
	CleanupStack::PopAndDestroy(dbManager);

	// 3) Create a valid landmark db, but without correct file extension
	iLog->Log(_L("3) Create a valid landmark db, but without correct file extension"));
	FTDatabaseUtility::CreateDatabaseL(_L("c:TP118_NoExtension"), EFalse);
	dbManager = CPosLmDatabaseManager::NewL();
    CleanupStack::PushL(dbManager);
	
	// 4) List all databases and verify that db created in step 3 is not listed
	iLog->Log(_L("List all databases and verify that db created in step 3 is not listed"));
	delete iDatabases; iDatabases=NULL;
	iDatabases = dbManager->ListDatabasesLC();
	CleanupStack::Pop(iDatabases);

	res = VerifyList(iDatabases, _L("file://c:TP118_NoExtension"));
	AssertTrueSecL(!res, _L("file://c:TP118_NoExtension should not have been found"));
	CleanupStack::PopAndDestroy(dbManager);

	// 5) Create lm db without correct secure id
	iLog->Log(_L("5) Create lm db without correct secure id"));
	FTDatabaseUtility::CreateDatabaseL(_L("c:TP118_WrongId.ldb"), ETrue, ETrue);
	dbManager = CPosLmDatabaseManager::NewL();
    CleanupStack::PushL(dbManager);
    
    // 6) List all databases and verify that db created in step 5 is not listed
    iLog->Log(_L("6) List all databases and verify that db created in step 5 is not listed"));
    delete iDatabases; iDatabases=NULL;
	iDatabases = dbManager->ListDatabasesLC();
	CleanupStack::Pop(iDatabases);
	
	res = VerifyList(iDatabases, _L("c:TP118_WrongId.ldb"));
	AssertTrueSecL(!res, _L("file://c:TP118_WrongId.ldb should not have been found"));
	CleanupStack::PopAndDestroy(dbManager);
	
	// 7 Create lm db with invalid landmarks tables
	iLog->Log(_L("7) Create lm db with invalid landmarks tables"));
	FTDatabaseUtility::CreateDatabaseL(_L("C:TP118_INVALIDTABLES.ldb"), ETrue, EFalse, ETrue);
	dbManager = CPosLmDatabaseManager::NewL();
    CleanupStack::PushL(dbManager);
    
    // 8) List all databases and verify that db created in step 7 is listed
    iLog->Log(_L("8) List all databases and verify that db created in step 7 is listed"));
    delete iDatabases; iDatabases=NULL;
	iDatabases = dbManager->ListDatabasesLC();
	CleanupStack::Pop(iDatabases);
	res = VerifyList(iDatabases, _L("file://C:TP118_INVALIDTABLES.ldb"));
	AssertTrueSecL(res, _L("file://C:TP118_INVALIDTABLES.ldb should have been found"));
	CleanupStack::PopAndDestroy(dbManager);
	
	// CleanupStack::PopAndDestroy(dbManager);
	// This causes DBMS to panic UTFW (this TestProgram)
	// Adds landmarks but needed tables are already removed
	// CreateLandmarksInDbL(_L("C:TP118_INVALIDTABLES.LDB"));
	
	/*
	/////////////////////////////////////////////
	// Add extra test here, set this db to default
	iLog->Log(_L("SetDefaultDatabaseL"));
    dbManager->SetDefaultDatabaseL(_L("C:TP118_INVALIDTABLES.LDB"));
	CleanupStack::PopAndDestroy(dbManager);
    
    //  Try to open defaultdb which is the db created in step18
    // Check that the default db is re-created when it is corrupt
    CPosLandmarkDatabase* lmdb = NULL;
    TRAP(err, lmdb = CPosLandmarkDatabase::OpenL());
    if (err == KErrNone)
    	{
    	CleanupStack::PushL(lmdb);
        CheckLandmarkDbL(lmdb);
        CleanupStack::PopAndDestroy(lmdb);
        //This causes DBMS to panic UTFW (this TestProgram)
	    // Adds landmarks but needed tables are already removed
        CreateLandmarksInDbL(_L("C:TP118_INVALIDTABLES.LDB"));
    	}
    else
    	{
    	LogErrorAndLeave(_L("OpenL should not leave with err"), err);
    	}
    */
	/////////////////////////////////////////////

	// 9) Try to open db created in step7
	iLog->Log(_L("9) Open db created in step 7"));
	lmd = CPosLandmarkDatabase::OpenL(_L("file://C:TP118_INVALIDTABLES.ldb"));
	CleanupStack::PushL(lmd);
	CheckLandmarkDbL(lmd);
	CleanupStack::PopAndDestroy(lmd);

    // 10) Drop table in db
    iLog->Log(_L("10) Create db and then drop tables in db"));
    dbManager=CPosLmDatabaseManager::NewL();
    CleanupStack::PushL(dbManager);
    
    _LIT(KDropTableLmDb, "C:TP118_DROPTABLELMDB.ldb");
    HPosLmDatabaseInfo* dbInfo = HPosLmDatabaseInfo::NewLC(KDropTableLmDb);
    dbManager->CreateDatabaseL(*dbInfo);
    CleanupStack::PopAndDestroy(dbInfo);
    
    CreateLandmarksInDbL(KDropTableLmDb);
    DropTableL(KDropTableLmDb, KPosLmLandmarkTable);
    
    delete iDatabases; iDatabases=NULL;
    iDatabases = dbManager->ListDatabasesLC();
    CleanupStack::Pop(iDatabases);
    res = VerifyList(iDatabases, _L("file://C:TP118_DROPTABLELMDB.ldb"));
    AssertTrueSecL(res, _L("C:TP118_DropTableLmDb.ldb was not found"));
    CleanupStack::PopAndDestroy(dbManager);

    // 11) Try to open db created in step10
	iLog->Log(_L("11) Open db created in step 10"));
	lmd = CPosLandmarkDatabase::OpenL(_L("file://C:TP118_DROPTABLELMDB.ldb"));

	// Error when opening db
	CleanupStack::PushL(lmd);
	TRAP(err, CheckLandmarkDbL(lmd));
	AssertTrueSecL(err == KErrNotFound, _L("11) CheckLandmarkDbL did not leave with KErrNotFound"));

	CleanupStack::PopAndDestroy(lmd);
	// Try to add landmarks to this db
	// Leaves with KErrNotFound
	//CreateLandmarksInDbL(_L("C:TP118_DROPTABLELMDB.LDB"),1);
    
    // 12) DropIndexL
    iLog->Log(_L("Step12: DropIndexL"));
    dbManager=CPosLmDatabaseManager::NewL();
    CleanupStack::PushL(dbManager);
    
    _LIT(KTP118_DROPINDEXLMDB_LDB, "C:TP118_DROPINDEXLMDB.ldb");
    HPosLmDatabaseInfo* dbInfoDrop = HPosLmDatabaseInfo::NewLC(KTP118_DROPINDEXLMDB_LDB);
    dbManager->CreateDatabaseL(*dbInfoDrop);
    CleanupStack::PopAndDestroy(dbInfoDrop);
    
    CreateLandmarksInDbL(_L("C:TP118_DROPINDEXLMDB.ldb"));
    iLog->Log(_L("12) DropIndex"));
    DropIndexL(_L("C:TP118_DROPINDEXLMDB.LDB"), KPosLmCategoryTable, KPosLmGlobalCategoryIdIndex);   
    CleanupStack::PopAndDestroy(dbManager);
    
    // 13) Try to open db created in step12
	iLog->Log(_L("13) Open db created in step 12"));
	lmd = CPosLandmarkDatabase::OpenL(_L("C:TP118_DROPINDEXLMDB.ldb"));
	
    CleanupStack::PushL(lmd);
	CheckLandmarkDbL(lmd);
	CleanupStack::PopAndDestroy(lmd);
    // Try to add landmarks to this db
	CreateLandmarksInDbL(_L("C:TP118_DROPINDEXLMDB.LDB"), 1);
	
	// 14)
    iLog->Log(_L("14) DropIndex"));
    dbManager=CPosLmDatabaseManager::NewL();
    CleanupStack::PushL(dbManager);
    
    _LIT(KTP118_DROPINDEXLMDB2_LDB, "C:TP118_DROPINDEXLMDB2.ldb");
    HPosLmDatabaseInfo* dbInfoDrop2 = HPosLmDatabaseInfo::NewLC(KTP118_DROPINDEXLMDB2_LDB);
    dbManager->CreateDatabaseL(*dbInfoDrop2);
    CleanupStack::PopAndDestroy(dbInfoDrop2);
    
    CreateLandmarksInDbL(_L("C:TP118_DROPINDEXLMDB2.ldb"));
    DropIndexL(_L("C:TP118_DROPINDEXLMDB2.LDB"), KPosLmLandmarkTable, KPosLmLandmarkIdIndex);
    CleanupStack::PopAndDestroy(dbManager);
    
    // 15) Try to open db created in step13
	iLog->Log(_L("15) Open db created in step 14"));
	lmd = CPosLandmarkDatabase::OpenL(_L("C:TP118_DROPINDEXLMDB2.ldb"));
	
    CleanupStack::PushL(lmd);
	CheckLandmarkDbL(lmd);
	CleanupStack::PopAndDestroy(lmd);
	// Try to add landmarks to this db
	CreateLandmarksInDbL(_L("C:TP118_DROPINDEXLMDB2.ldb"), 1);
	
	////////////
    // 16) Create an ordinary database with some landmarks and then remove some parts of the binary file
    // Create db by manually removing some parts with notepad
    /*
    //Use this part to create db "TP118HalfLmDb.ldb" containing some landmarks, then use notepad to remove
    // some parts from physical file 
    dbManager=CPosLmDatabaseManager::NewL();
    CleanupStack::PushL(dbManager);
    dbManager->CreateDatabaseL(_L("TP118HalfLmDb.ldb"));
    CreateLandmarksInDbL(_L("TP118HalfLmDb.ldb"));
    CleanupStack::PopAndDestroy(dbManager);
    */
    // Use corrupted file
    iLog->Log(_L("16) Use corrupted file"));
    CopyTestDbFileL(_L("TP118HALFLMDB_2.ldb"));
    
    dbManager = CPosLmDatabaseManager::NewL();
    CleanupStack::PushL(dbManager);
    delete iDatabases; iDatabases = NULL;
    iDatabases = dbManager->ListDatabasesLC();
    CleanupStack::Pop(iDatabases);

    res = VerifyList(iDatabases, _L("file://C:TP118HALFLMDB_2.ldb"));
	AssertTrueSecL(res, _L("file://C:TP118HALFLMDB_2.LDB should have been found"));
    CleanupStack::PopAndDestroy(dbManager);
    
    // 17) Try to open db created in step16m it should not be possible
    // Before it returned KErrCorrupt(?) but now with Mars wk21 SDK it returns KErrNotSupported
    CPosLandmarkDatabase* someLmDb = NULL;
    TRAP(err, someLmDb = CPosLandmarkDatabase::OpenL(_L("TP118HalfLmDb_2.ldb")));
    if (err == KErrNone)
    	{
    	CleanupStack::PushL(someLmDb);
    	ExecuteAndDeleteLD(someLmDb->InitializeL());
    	CleanupStack::PopAndDestroy(someLmDb);
        AssertTrueSecL(EFalse,_L("OpenL should leave with error"));
    	}
    else
        {
        buf.Format(_L("OpenL leaved with %d"), err);
        iLog->Log(buf);
        }
    
    // 18) Use same file as in 16 and set this file to default db
    iLog->Log(_L("Use same file as in 16 and set this file to default db"));
    CopyTestDbFileL(_L("TP118HALFLMDB_2.LDB"));
    dbManager = CPosLmDatabaseManager::NewL();
    CleanupStack::PushL(dbManager);
    
    iLog->Log(_L("SetDefaultDatabaseL"));
    dbManager->SetDefaultDatabaseUriL(_L("Tp118HALFLMDB_2.ldb"));
	CleanupStack::PopAndDestroy(dbManager);
    
    // 19) Try to open defaultdb which is the db created in step18
    // Before Mars wk21 SDK the default db is re-created when it is corrupt
    // but now instead KErrNotSUpported is returned
    iLog->Log(_L("Try to open defaultdb which is the db created in step18"));
    TRAP(err, someLmDb = CPosLandmarkDatabase::OpenL());
    
    if (err == KErrNone)
    	{
    	CleanupStack::PushL(someLmDb);
        CheckLandmarkDbL(someLmDb);
        CleanupStack::PopAndDestroy(someLmDb);
        // Try to add landmarks to this db
	    CreateLandmarksInDbL(_L("Tp118HALFLMDB_2.ldb"));
    	}
    else
    	{
    	//LogErrorAndLeave(_L("OpenL should not leave with err"), err);
    	buf.Format(_L("open defaultdb leaved with %d"), err);
    	iLog->Log(buf);
    	}
	}

// ---------------------------------------------------------
// CPosTp118::CheckLandmarkDbL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp118::CheckLandmarkDbL(CPosLandmarkDatabase* aDb)
    {
    iLog->Log(_L("CheckLandmarkDbL"));
    
    if (aDb->IsInitializingNeeded()) ExecuteAndDeleteLD(aDb->InitializeL());
    
    CPosLmItemIterator* iter = aDb->LandmarkIteratorL();
    CleanupStack::PushL(iter);
    
    TInt size = iter->NumOfItemsL();
    TBuf<100> buf;
    buf.Format(_L("DB contains %d nr of landmarks"), size);
    iLog->Log(buf);
    
    TPosLmItemId id = iter->NextL();

    while (id != KPosLmNullItemId)
        {
        CPosLandmark* lm = aDb->ReadLandmarkLC(id);
        TPtrC name;
        lm->GetLandmarkName(name);
        // Debugging iLog->Log(name);
        id = iter->NextL();
        CleanupStack::PopAndDestroy(lm);
        }
        
    // Try to get an non existing landmark
    CPosLandmark* lm = NULL;
    TRAPD(err, lm = aDb->ReadLandmarkLC(123456));
    if (err != KErrNotFound) 
        {
        CleanupStack::PopAndDestroy(lm);
        CleanupStack::PopAndDestroy(iter);
        AssertTrueSecL(EFalse,_L("ReadLandmarkLC should leave with KErrNotFound"));
        }
    
    CleanupStack::PopAndDestroy(iter);
    
    // Check categories
    CPosLmCategoryManager* categoryManager = CPosLmCategoryManager::NewL(*aDb);
    CleanupStack::PushL(categoryManager);
	CPosLmItemIterator* catIter = categoryManager->CategoryIteratorL();
	CleanupStack::PushL(catIter);
	
	TInt catCount = catIter->NumOfItemsL();

	buf.Format(_L("Found %d nr of categories items in db"), catCount);
	iLog->Log(buf);

	id = catIter->NextL();
	while (id != KPosLmNullItemId)
    	{
    	CPosLandmarkCategory* cat = categoryManager->ReadCategoryLC(id);
    	TPtrC name;
    	cat->GetCategoryName(name);
    	// Debugging iLog->Log(name);
    	CleanupStack::PopAndDestroy(cat);
    	id = catIter->NextL();
    	}
    
    CleanupStack::PopAndDestroy(catIter);
    CleanupStack::PopAndDestroy(categoryManager);
    }
    
// ---------------------------------------------------------
// CPosTp118::DeleteTp118FilesL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp118::DeleteTp118FilesL()
	{
	iLog->Log(_L("DeleteTp118FilesL"));
    RFs fs;
    TInt err = fs.Connect();
    AssertTrueSecL(err == KErrNone, _L("fs.Connect returned with error"), err);
    CleanupClosePushL(fs);

    CFileMan* fileMan = CFileMan::NewL(fs);
    CleanupStack::PushL(fileMan);
    
    // delete all TP118 files
    err = fileMan->Delete(_L("c:\\private\\100012a5\\*TP118*.*"));
    AssertTrueSecL(err == KErrNone || err == KErrNotFound, _L("CFileMan could not delete files1"), err);
    
    err = fileMan->Delete(_L("c:\\private\\100012a5\\*TP118*"));
    AssertTrueSecL(err == KErrNone || err == KErrNotFound, _L("CFileMan could not delete files2"), err);
        
    CleanupStack::PopAndDestroy(1); //fileMan
    CleanupStack::PopAndDestroy(1); //fs
    iLog->Log(_L("DeleteTp118FilesL Done"));
    
	}
	
// ---------------------------------------------------------
// CPosTp118::CreateLandmarksInDbL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp118::CreateLandmarksInDbL(const TDesC& aDbName, TInt aIndex)
	{
	iLog->Log(_L("CreateLandmarksInDbL"));
	const TInt KNrOfLandmarks = 50;
	const TInt KNrOfCategories = 10;
	
	CPosLandmarkDatabase* lmd = CPosLandmarkDatabase::OpenL(aDbName);
	CleanupStack::PushL(lmd);
	
    ExecuteAndDeleteLD(lmd->InitializeL());
    
    CPosLmCategoryManager* cm = CPosLmCategoryManager::NewL(*lmd);
    CleanupStack::PushL(cm);
    
    RArray<TPosLmItemId> arr;
   	CleanupClosePushL(arr);
    TInt i=0;
    TBuf<150> buf;
    
    // Create categories
   	for (i=0; i<KNrOfCategories; i++)
   		{
    	CPosLandmarkCategory* category = CPosLandmarkCategory::NewLC();
    	buf.Zero();
    	buf.Format(_L("VeryVeryVeryVeryVeryVeryVeryVeryVeryVeryVeryVeryLongCategoryName%d_%d"), i, aIndex);
    	
    	category->SetCategoryNameL(buf);
    	TPosLmItemId id=0;
    	TInt err = KErrLocked;
    	while (err == KErrLocked)
    	    {
    	    TRAP(err, id = cm->AddCategoryL(*category));
    	    }
    	arr.Append(id);
    	CleanupStack::PopAndDestroy(category);
   		}

	// Create landmarks and attach every category to every landmark
    for (i=0;i<KNrOfLandmarks;i++)
    	{
    	buf.Zero();
    	buf.Format(_L("VeryVeryVeryVeryVeryVeryVeryVeryVeryVeryVeryVeryLongLandmarkName%d_%d"), i, aIndex);
    	CPosLandmark* lm1 = CPosLandmark::NewLC();
    	lm1->SetLandmarkNameL(buf);
    	lm1->SetLandmarkDescriptionL(_L("SomeVeryVeryVeryVeryVeryVeryVeryVeryVeryVeryVeryVeryVeryVeryLongDescription"));
    	for (TInt j=0;j<arr.Count();j++)
    		{
    		lm1->AddCategoryL(arr[j]);
    		}
   		TInt err = KErrLocked;
   		while (err==KErrLocked)
   		    {
   		    TRAP(err, lmd->AddLandmarkL(*lm1));
   		    }
   		
   		CleanupStack::PopAndDestroy(lm1);
   		}
   		   	
    CleanupStack::PopAndDestroy(&arr);
    
    CleanupStack::PopAndDestroy(cm);
    
    ExecuteAndDeleteLD(lmd->CompactL());
    CleanupStack::PopAndDestroy(lmd);
    
	}
	
// ---------------------------------------------------------
// CPosTp118::VerifyList
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TBool CPosTp118::VerifyList(CDesCArray* aList, const TDesC& aDbName)
	{
	TBool found = EFalse;
	TInt count = aList->Count();
	TBuf<100> buf;
	buf.Format(_L("Found %d nr of dbs"), count);
	iLog->Log(buf);
	for (TInt i=0;i<count;i++)
		{
		iLog->Log((*aList)[i]);
		if ((*aList)[i].Compare(aDbName) == KErrNone)
			{
			found = ETrue;
			}
		}
		return found;
	}
    
// ---------------------------------------------------------
// CPosTp118::DropTableL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp118::DropTableL(const TDesC& aDbName, const TDesC& aTable)
    {
    /*
    // Incremental
    iLog->Log(_L("DropTableL"));
    RDbs dbSession;
    CleanupClosePushL(dbSession);
    User::LeaveIfError(dbSession.Connect());

    RDbNamedDatabase db;
    CleanupClosePushL(db);
    TBuf<100> buf;
    
    TInt err = db.Open(dbSession, aDbName ,KPosLmDbSecureFormat);
    if (err != KErrNone) LogErrorAndLeave(_L("Database could not be opened"), err);

	RDbIncremental dbInc;
	CleanupClosePushL(dbInc);

    TInt step = 100;
    err = dbInc.DropTable(db, aTable, step);
    if (err != KErrNone) LogErrorAndLeave(_L("DropTable could not be performed"), err);
    
    while (step > 50)
    	{
    	buf.Format(_L("ProgressStep %d "), step);
    	iLog->Log(buf);
    	dbInc.Next(step);
    	}
    	
    dbInc.Close();
    db.Close();
    	
    CleanupStack::PopAndDestroy(&dbInc);
    CleanupStack::PopAndDestroy(&db);
    CleanupStack::PopAndDestroy(&dbSession);
    */
    
    iLog->Log(_L("DropTableL"));
    RDbs dbSession;
    CleanupClosePushL(dbSession);
    User::LeaveIfError(dbSession.Connect());

    RDbNamedDatabase db;
    CleanupClosePushL(db);
    TBuf<100> buf;
    
    TInt err = db.Open(dbSession, aDbName ,KPosLmDbSecureFormat);
    AssertTrueSecL(err == KErrNone, _L("Database could not be opened"), err);
    err = db.DropTable(aTable);
    AssertTrueSecL(err == KErrNone, _L("DropTable could not be performed"), err);
    db.Close();
    	
    CleanupStack::PopAndDestroy(&db);
    CleanupStack::PopAndDestroy(&dbSession);

    }

// ---------------------------------------------------------
// CPosTp118::AlterTableL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp118::AlterTableL(const TDesC& aDbName)
    {
    iLog->Log(_L("AlterTableL"));
    // Incremental
    RDbs dbSession;
    CleanupClosePushL(dbSession);
    User::LeaveIfError(dbSession.Connect());

    RDbNamedDatabase db;
    CleanupClosePushL(db);
    
    TBuf<100> buf;
    
    TInt err = db.Open(dbSession, aDbName ,KPosLmDbSecureFormat);
    AssertTrueSecL(err == KErrNone, _L("Database could not be opened"), err);

	RDbIncremental dbInc;
	CleanupClosePushL(dbInc);
	    
    
    CDbColSet* newColumns = CDbColSet::NewLC();

    TDbCol col(KPosLmIconIdCol, EDbColUint32);
    col.iAttributes = col.EAutoIncrement;
    newColumns->AddL(col);

    newColumns->AddL(TDbCol(KPosLmIconPathCol, EDbColLongText, KMaxFileName));
    //User::LeaveIfError(aDb.CreateTable(KPosLmIconTable, *columns));
    
    TInt step = 100;
    err = dbInc.AlterTable(db, KPosLmIconTable, *newColumns, step);
    if (err != KErrNone) 
    {
    TBuf<100> buf;
    buf.Format(_L("AlterTable could not be performed, err %d"), err);
    iLog->Log(buf);
    }
    else
    {
    
    while (step > 1)
    	{
    	buf.Format(_L("ProgressStep %d "), step);
    	iLog->Log(buf);
    	dbInc.Next(step);
    	}
    }	
    dbInc.Close();
    db.Close();
    
    CleanupStack::PopAndDestroy(newColumns);	
    CleanupStack::PopAndDestroy(&dbInc);
    CleanupStack::PopAndDestroy(&db);
    CleanupStack::PopAndDestroy(&dbSession);
    /*
    
    RDbs dbSession;
    CleanupClosePushL(dbSession);
    User::LeaveIfError(dbSession.Connect());

    RDbNamedDatabase db;
    CleanupClosePushL(db);
    
    TBuf<100> buf;
    
    TInt err = db.Open(dbSession, aDbName ,KPosLmDbSecureFormat);
    if (err != KErrNone) LogErrorAndLeave(_L("Database could not be opened"), err);
    
    
    CDbColSet* newColumns = CDbColSet::NewLC();

    TDbCol col(KPosLmIconIdCol, EDbColUint32);
    col.iAttributes = col.EAutoIncrement;
    newColumns->AddL(col);

    newColumns->AddL(TDbCol(KPosLmIconPathCol, EDbColLongText, KMaxFileName));
    //User::LeaveIfError(aDb.CreateTable(KPosLmIconTable, *columns));
    
    TInt step = 100;
    err = db.AlterTable(KPosLmIconTable, *newColumns);
    if (err != KErrNone) 
        {
        TBuf<100> buf;
        buf.Format(_L("AlterTable could not be performed, err %d"), err);
        iLog->Log(buf);
        }

    db.Close();
    
    CleanupStack::PopAndDestroy(newColumns);	
    CleanupStack::PopAndDestroy(&db);
    CleanupStack::PopAndDestroy(&dbSession);
    */
    }
    
        
// ---------------------------------------------------------
// CPosTp118::DropIndexL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp118::DropIndexL(const TDesC& aDbName, const TDesC& aTable, const TDesC& aIndex)
    {
    iLog->Log(_L("DropIndexL()"));
    
    //Incremental
    RDbs dbSession;
    CleanupClosePushL(dbSession);
    User::LeaveIfError(dbSession.Connect());

    RDbNamedDatabase db;
    CleanupClosePushL(db);
    TBuf<100> buf;
    
    TInt err = db.Open(dbSession, aDbName ,KPosLmDbSecureFormat);
    AssertTrueSecL(err == KErrNone, _L("Database could not be opened"), err);

	RDbIncremental dbInc;
	CleanupClosePushL(dbInc);

    TInt step = 100;
    err = dbInc.DropIndex(db, aIndex, aTable, step);
    AssertTrueSecL(err == KErrNone, _L("DropIndex could not be performed"), err);
    
    while (step  > 1)
    	{
    	buf.Format(_L("ProgressStep %d "), step);
    	iLog->Log(buf);
    	dbInc.Next(step);
    	}
    	
    CleanupStack::PopAndDestroy(&dbInc);
    CleanupStack::PopAndDestroy(&db);
    CleanupStack::PopAndDestroy(&dbSession);
    
    /*
    RDbs dbSession;
    CleanupClosePushL(dbSession);
    User::LeaveIfError(dbSession.Connect());

    RDbNamedDatabase db;
    CleanupClosePushL(db);
    TBuf<100> buf;
    
    TInt err = db.Open(dbSession, aDbName ,KPosLmDbSecureFormat);
    if (err != KErrNone) LogErrorAndLeave(_L("Database could not be opened"), err);

    TInt step = 100;
    err = db.DropIndex(aIndex, aTable);
    if (err != KErrNone) LogErrorAndLeave(_L("DropIndex could not be performed"), err);
    	
    CleanupStack::PopAndDestroy(&db);
    CleanupStack::PopAndDestroy(&dbSession);
    */
    }

//  End of File
