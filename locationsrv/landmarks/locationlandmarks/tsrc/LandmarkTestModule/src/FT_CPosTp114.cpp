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

#include "FT_CPosTp114.h"
#include <EPos_CPosLandmarkDatabase.h>
#include <EPos_CPosLmDatabaseManager.h>
#include <connect/sbdefs.h>

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CPosTp114::GetName
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp114::GetName(TDes& aName) const
    {
    _LIT(KTestName, "Tp114 - Landmark database backup/restore");
    aName = KTestName;
    }

// ---------------------------------------------------------
// CPosTp114::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp114::CloseTest()
    {
    iLog->Put(_L("CloseTest"));
    }
    
/**
* Performes the test by connecting to the landmarks server

Test LMREQ152: Landmarks DB backup 
Backup and restore of all Landmarks databases using the system backup shall be possible.
The clients are responsible for release of databases during backup/restore. 
A database directly accessed by a client will not be possible to backup/restore.

*/
// ---------------------------------------------------------
// CPosTp114::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp114::StartL()
    {  
    TBool backupTest = FileExistL();
    
    iLog->Put(_L("DoTestL"));
    
    CPosTp114BackupListener* backupListener = CPosTp114BackupListener::NewL(iLog);
    CleanupStack::PushL(backupListener);
    
    if (backupTest)
        {
        iLog->Put(_L("Backup Test"));
        // Backup part
        InitBackupTestL();
        }
    else
        {
        iLog->Put(_L("Restore Test"));
        iLog->Put(_L("Connnect to eposlmserver"));
        // Restore part
        CPosLmDatabaseManager* dbManager = CPosLmDatabaseManager::NewL();
        CleanupStack::PushL(dbManager);

        CDesCArray* dbArray = dbManager->ListDatabasesLC();
        //TInt count = dbArray->Count();
        CleanupStack::PopAndDestroy(dbArray);
        CleanupStack::PopAndDestroy(dbManager);
        }
    
    // Check landmark server
	_LIT(KPosLandmarksServerName, "*eposlmserver*");
	
	User::After(5000000); // XXX Add some time here
	TBool result = ETrue;
	TBool alive = ServerAlive(KPosLandmarksServerName);
    if(alive) iLog->Put(_L("ERROR: Server is stil alive"));


    if (backupTest)
        {
        _LIT(KStartBackup, "Now start backup but first press OK");
        TUtfwUserAnswer answer = iUserInfo->ShowDialog(KStartBackup, EUtfwDialogTypeOk, EFalse);
   
        if(answer == EUtfwUserAnswerOk)
            {
            backupListener->StartCheckingL();
            User::After(10000000); // Wait some time here so that backup is really finished
            }
        }
    else
        {
        _LIT(KStartRestore, "Now start restore, but first press OK");
        TUtfwUserAnswer answer = iUserInfo->ShowDialog(KStartRestore, EUtfwDialogTypeOk, EFalse);
   
        if(answer == EUtfwUserAnswerOk)
            {
            backupListener->StartCheckingL();
            User::After(10000000); // Wait some time here so that restore is really finished
            }
        }

     if (backupTest)
        {
        // Prepare dbs for restore part
        PrepareForRestoreTestL();
        }
    else
        {
        // Check correct dbs and landmarks restored
        result = CheckDbAfterRestoreL();
        }
        
    backupListener->Cancel();
    CleanupStack::PopAndDestroy(backupListener);
    
    if (!result) LogErrorAndLeave(_L("Errors found when restoring"));
	}

// ---------------------------------------------------------
// CPosTp114::CheckDbAfterRestoreL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TBool CPosTp114::CheckDbAfterRestoreL()
    {
    // Check that three landmark dbs exist
    // Check that all dbs contain three landmarks
    _LIT(DB1, "file://C:backupDb1.ldb");
    _LIT(DB2, "file://C:backupDb2.ldb");
    _LIT(DB3, "file://C:backupDb3.ldb");
    
    iLog->Put(_L("CheckDbAfterRestoreL"));
    
    CPosLmDatabaseManager* dbManager = CPosLmDatabaseManager::NewL();
    CleanupStack::PushL(dbManager);
    
    TBool result = ETrue;

    CDesCArray* dbArray = dbManager->ListDatabasesLC();
    TInt count = dbArray->Count();
    //AssertTrueL(count == 3, _L("Wrong nr of dbs after restore"));
    if (count != 3)
        {
        iLog->Put(_L("ERROR: Wrong nr of dbs after restore"));
        result = EFalse;
        }
    
    TInt pos;

    if (dbArray->Find(DB1, pos) != 0 ||
        dbArray->Find(DB2, pos) != 0 ||
        dbArray->Find(DB3, pos) != 0)
            {
            iLog->Put(_L("Could not find db"));
            result = EFalse;
            }
    
    CleanupStack::PopAndDestroy(dbArray);
    CleanupStack::PopAndDestroy(dbManager);
    
    CPosLandmarkDatabase* db = CPosLandmarkDatabase::OpenL(DB1);
    CleanupStack::PushL(db);
    
    CPosLmItemIterator* iter = db->LandmarkIteratorL();
    CleanupStack::PushL(iter);
    
    TInt numberOfElements = iter->NumOfItemsL();    
    if (numberOfElements != 3) 
        {
        iLog->Put(_L("Wrong number of landmarks in DB1 "));
        result = EFalse;
        }
    
    CleanupStack::PopAndDestroy(iter);
    CleanupStack::PopAndDestroy(db);
        
    return result;
    }

// ---------------------------------------------------------
// CPosTp114::InitBackupTestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp114::InitBackupTestL()
    {
    iLog->Put(_L("InitBackupTestL"));
    // Use this part to create three landmark dbs with some landmarks
    _LIT(DB1, "backupDb1.ldb");
    _LIT(DB2, "backupDb2.ldb");
    _LIT(DB3, "backupDb3.ldb");
    
    // Firts remove all landmark dbs
    CPosLmDatabaseManager* dbManager = CPosLmDatabaseManager::NewL();
    CleanupStack::PushL(dbManager);

    CDesCArray* dbArray = dbManager->ListDatabasesLC();
    TInt count = dbArray->Count();

    for (TInt i=0;i<count;i++)
        {
        dbManager->DeleteDatabaseL((*dbArray)[i]);
        }

    CleanupStack::PopAndDestroy(dbArray);
    
    HPosLmDatabaseInfo* dbInfo = HPosLmDatabaseInfo::NewLC(DB1);
	dbManager->CreateDatabaseL(*dbInfo);
	CleanupStack::PopAndDestroy(dbInfo);
	
	dbInfo = HPosLmDatabaseInfo::NewLC(DB2);
	dbManager->CreateDatabaseL(*dbInfo);
	CleanupStack::PopAndDestroy(dbInfo);
	
	dbInfo = HPosLmDatabaseInfo::NewLC(DB3);
	dbManager->CreateDatabaseL(*dbInfo);
	CleanupStack::PopAndDestroy(dbInfo);
	
    // Add landmarks to db1
    CPosLandmarkDatabase* db = CPosLandmarkDatabase::OpenL(DB1);
    CleanupStack::PushL(db);
    if (db->IsInitializingNeeded()) ExecuteAndDeleteLD(db->InitializeL());
    
	CPosLandmark* lm1 = CPosLandmark::NewLC();
    lm1->SetLandmarkNameL(_L("TP114_Db1_Lm1"));
   	db->AddLandmarkL(*lm1);
   	CleanupStack::PopAndDestroy(lm1);
   	CPosLandmark* lm2 = CPosLandmark::NewLC();
    lm2->SetLandmarkNameL(_L("TP114_Db1_Lm2"));
   	db->AddLandmarkL(*lm2);
   	CleanupStack::PopAndDestroy(lm2);
   	CPosLandmark* lm3 = CPosLandmark::NewLC();
    lm3->SetLandmarkNameL(_L("TP114_Db1_Lm3"));
   	db->AddLandmarkL(*lm3);
   	CleanupStack::PopAndDestroy(lm3);
   	ExecuteAndDeleteLD(db->CompactL());
    CleanupStack::PopAndDestroy(db);
    
    // Add landmarks to db2
    db = CPosLandmarkDatabase::OpenL(DB2);
    CleanupStack::PushL(db);
    if (db->IsInitializingNeeded()) ExecuteAndDeleteLD(db->InitializeL());
    
	lm1 = CPosLandmark::NewLC();
    lm1->SetLandmarkNameL(_L("TP114_Db2_Lm1"));
   	db->AddLandmarkL(*lm1);
   	CleanupStack::PopAndDestroy(lm1);
   	lm2 = CPosLandmark::NewLC();
    lm2->SetLandmarkNameL(_L("TP114_Db2_Lm2"));
   	db->AddLandmarkL(*lm2);
   	CleanupStack::PopAndDestroy(lm2);
   	lm3 = CPosLandmark::NewLC();
    lm3->SetLandmarkNameL(_L("TP114_Db2_Lm3"));
   	db->AddLandmarkL(*lm3);
   	CleanupStack::PopAndDestroy(lm3);
   	ExecuteAndDeleteLD(db->CompactL());
    CleanupStack::PopAndDestroy(db);
    
    // Add landmarks to db3
    db = CPosLandmarkDatabase::OpenL(DB3);
    CleanupStack::PushL(db);
    if (db->IsInitializingNeeded()) ExecuteAndDeleteLD(db->InitializeL());
    
	lm1 = CPosLandmark::NewLC();
    lm1->SetLandmarkNameL(_L("TP114_Db3_Lm1"));
   	db->AddLandmarkL(*lm1);
   	CleanupStack::PopAndDestroy(lm1);
   	lm2 = CPosLandmark::NewLC();
    lm2->SetLandmarkNameL(_L("TP114_Db3_Lm2"));
   	db->AddLandmarkL(*lm2);
   	CleanupStack::PopAndDestroy(lm2);
   	lm3 = CPosLandmark::NewLC();
    lm3->SetLandmarkNameL(_L("TP114_Db3_Lm3"));
   	db->AddLandmarkL(*lm3);
   	CleanupStack::PopAndDestroy(lm3);
   	ExecuteAndDeleteLD(db->CompactL());
    CleanupStack::PopAndDestroy(db);
    
	CleanupStack::PopAndDestroy(dbManager);
	iLog->Put(_L("InitBackupTestL Done"));
    }

// ---------------------------------------------------------
// CPosTp114::PrepareForRestoreTestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp114::PrepareForRestoreTestL()
    {
    _LIT(DB1, "backupDb1.ldb");
    _LIT(DB2, "backupDb2.ldb");
    
    iLog->Put(_L("PrepareForRestoreTestL"));
        
    // Remove db2 and remove some landmarks from db1
    CPosLmDatabaseManager* dbManager = CPosLmDatabaseManager::NewL();
    CleanupStack::PushL(dbManager);
    
    //dbManager->DeleteDatabaseL(DB2);
    DeleteFileL(_L("c:\\private\\100012a5\\DBS_101FE978_backupDb2.ldb"));
    
    CPosLandmarkDatabase* db = CPosLandmarkDatabase::OpenL(DB1);
    CleanupStack::PushL(db);
    
    CPosLmItemIterator* iter = db->LandmarkIteratorL();
    CleanupStack::PushL(iter);
        
    RArray<TPosLmItemId> arrayOfIds;
    CleanupClosePushL(arrayOfIds);
    
    TInt numberOfElements = iter->NumOfItemsL();    
    iter->GetItemIdsL(arrayOfIds, 0, (numberOfElements-1));
    
    ExecuteAndDeleteLD(db->RemoveLandmarksL(arrayOfIds));
    TBuf<100> buf;
    buf.Format(_L("Removing %d nr of landmarks from db1"), arrayOfIds.Count());
    iLog->Put(buf);
    
    CleanupStack::PopAndDestroy(&arrayOfIds);
    CleanupStack::PopAndDestroy(iter);
    CleanupStack::PopAndDestroy(db);
	CleanupStack::PopAndDestroy(dbManager);
	
	iLog->Put(_L("PrepareForRestoreTestL Done"));
    }

// ---------------------------------------------------------
// CPosTp114::FileExistL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TBool CPosTp114::FileExistL()
    {
    _LIT(KIniFile, "c:\\TP114backupTest.ini");
    
    RFs fileserver;
	RFile file;
	TBool exist = EFalse;
	
	User::LeaveIfError(fileserver.Connect());
	TInt err = file.Open(fileserver, KIniFile, EFileWrite);
	if (err == KErrNotFound)
		{
		TInt err = file.Create(fileserver, KIniFile, EFileWrite);
		exist = ETrue;
	    file.Close();
	    iLog->Put(_L("File has been created"));
		}
    else
        {
        file.Close();
        TInt err = fileserver.Delete(KIniFile);
        if (err != KErrNone) 
            {
            TBuf<50> buf;
            buf.Format(_L("Could not delete c:\\TP114backupTest.ini, err %d"), err);
            iLog->Put(buf);
            }
        else
            {
            iLog->Put(_L("File has been deleted"));
            }
            
        exist = EFalse;
        }

	fileserver.Close();
	return exist;
    }


// -----------------------------------------------------------------------------
// CPosTp114BackupListener
// 
// -----------------------------------------------------------------------------
//
CPosTp114BackupListener::CPosTp114BackupListener()
    : CActive(EPriorityLow)
    {
    }

// -----------------------------------------------------------------------------
// CPosTp114BackupListener::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPosTp114BackupListener::ConstructL(MUtfwLog* aLog)
    {
    TUid categoryUid;
    categoryUid.iUid = KUidSystemCategoryValue;
    User::LeaveIfError(iProperty.Attach(categoryUid,
                                        conn::KUidBackupRestoreKey));
    CActiveScheduler::Add(this);
    iLog = aLog;
    iBackupIsRunning = EFalse;

    // initial subscription and process current property value
    RunL();
    }

// Two-phased constructor
CPosTp114BackupListener* CPosTp114BackupListener::NewL(MUtfwLog* aLog)
    {
    CPosTp114BackupListener* self =
        new (ELeave) CPosTp114BackupListener;
    CleanupStack::PushL(self);
    self->ConstructL(aLog);
    
    CleanupStack::Pop(self);
    return self;
    }

// Destructor
CPosTp114BackupListener::~CPosTp114BackupListener()
    {
    Cancel();
    iProperty.Close();
    }

// -----------------------------------------------------------------------------
// CPosTp114BackupListener::RunL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosTp114BackupListener::RunL()
    {
   iLog->Put(_L("RunL"));
    // resubscribe before processing new value to prevent missing updates.
    iProperty.Subscribe(iStatus);
    SetActive();
        
    if (IsBackupRunning())
        {
        iLog->Put(_L("Backup (Restore) is Running...."));
        iBackupIsRunning = ETrue;
        }
        
    //Check if back to normal (after backup/restore)
    if (!IsBackupRunning() && iBackupIsRunning)
        {
        iLog->Put(_L("After backup/restore"));
        iBackupIsRunning = EFalse;
        CActiveScheduler::Stop();
        }
    }
 
// -----------------------------------------------------------------------------
// CPosTp114BackupListener::StartCheckingL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosTp114BackupListener::StartCheckingL()
    {
    CActiveScheduler::Start();
    }

// -----------------------------------------------------------------------------
// CPosTp114BackupListener::IsBackupRunning
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CPosTp114BackupListener::IsBackupRunning()
    {
    TBool backupIsRunning = EFalse;
    TInt value = 0;
    TInt err = iProperty.Get(value);

    if (err == KErrNone)
        {
        // Use mask to get rid of unwanted info
        TInt partType = conn::KBURPartTypeMask & value;

        // The values EBURUnset or EBURNormal mean
        // that no backup/restore is ongoing.
        if (partType == conn::EBURUnset || partType == conn::EBURNormal)
            {
            backupIsRunning = EFalse;
            }
        else
            {
            backupIsRunning = ETrue;
            }
        }
    else if (err == KErrNotFound)
        {
        // If property is not found, assume that no backup is in progress
        backupIsRunning = EFalse;
        }
    else
        {
        // Some other error occured. In this case,
        // assume that backup is running.
        backupIsRunning = ETrue;
        }

    return backupIsRunning;
    }

// -----------------------------------------------------------------------------
// CPosTp114BackupListener::DoCancel
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosTp114BackupListener::DoCancel()
    {
    iLog->Put(_L("DoCancel"));
    iProperty.Cancel();
    }
    
//  End of File
