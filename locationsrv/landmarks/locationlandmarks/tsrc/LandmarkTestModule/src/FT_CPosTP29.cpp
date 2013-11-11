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
#include "FT_CPosTp29.h"
#include <utfw.h>
#include <f32file.h>
#include <EPos_CPosLMCategoryManager.h>
#include <centralrepository.h>
#include <UiklafInternalCRKeys.h>
//#include <eposlmintservices.rsg>
#include <barsread.h>
#include <barsc.h>
#include <bautils.h>

// CONSTANTS
_LIT(KFile1, "c:\\documents\\Tp29_1.txt");
_LIT(KFile2, "c:\\documents\\Tp29_2.txt");
//_LIT(KPosResourceFile, "\\resource\\eposlmintservices.rsc");
//const TInt KOneK = 1024;

// Size limit defined in "eposlmintservices.rss" to 250 kilobytes (kilo defined as 1024)
//const TInt KMaxSize = 250000;
// This is for KFile1, "fill" a file with contents until KSizeLimit1 limit is reached
const TInt KSizeLimitOffset1 = 350000;
// This is for KFile2, "fill" a file with contents until KSizeLimit2 limit is reached
const TInt KSizeLimitOffset2 = 50000; // In bytes: 250000 bytes + 50000 bytes
_LIT(KInternalServicesResFileRom, "z:\\resource\\eposlmintservices.rsc");
_LIT(KInternalServicesResFileBackup, "c:\\resource\\copy_of_eposlmintservices.rsc");
_LIT(KInternalServicesResFileCdrive, "c:\\resource\\eposlmintservices.rsc");
//_LIT(KInternalServicesLargeResFile, "c:\\system\\test\\testdata\\eposlmintservices_453KB.rsc");
//_LIT(KInternalServicesSmallResFile, "c:\\system\\test\\testdata\\eposlmintservices_198KB.rsc");

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CPosTp29::GetName
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp29::GetName(TDes& aName) const
    {
    _LIT(KTestName, "Tp29 - Out of disk");
    aName = KTestName;
    }

// ---------------------------------------------------------
// CPosTp29::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp29::CloseTest()
    {
    delete iLandmarksDb;
    iLandmarksDb = NULL;

    delete iCategoryManager;
    iCategoryManager = NULL;

    //TryRestoreResourceFile();

    iFile1.Close();
    iFile2.Close();
    iFileServer.Delete(KFile1);
    iFileServer.Delete(KFile2);
    iFileServer.Close();
    }

// ---------------------------------------------------------
// CPosTp29::InitTestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp29::InitTestL()
    {
    // File setup, create two files
    User::LeaveIfError(iFileServer.Connect());

    // Try to restore if previous execution of Tp55 panicked or was killed.
    //TryRestoreResourceFile();
    }

// ---------------------------------------------------------
// CPosTp29::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp29::StartL()
    {
#ifdef __WINS__
    _LIT(KNotSupported, "Test case is not supported on WINS, exucute it on target!");
    LogErrorAndLeave(KNotSupported);
#else
    
    //Retreive threshold from centralrepository
    CRepository* repository = CRepository::NewLC(KCRUidUiklaf);
    User::LeaveIfError(repository->Get(KUikOODDiskCriticalThreshold, iCriticalThreshold));
    CleanupStack::PopAndDestroy(repository);

    TBuf<100> buf;
    buf.Format(_L("Critical threshold from central repository %d"), iCriticalThreshold);
    iLog->Put(buf);

    // Test import/export LMs when critical level is set to MLFW defined value.
    InitiateCriticalLevelL();
    ReInitTestL();
    TestCriticalLevelL();

    /*
    // Test import/export LMs when critical level is set to a large value.
    iLog->Put(_L("\r\nRedefining critical level to 453 KB and running all tests again...."));
    CopyResourceFileL(KInternalServicesLargeResFile);
    InitiateCriticalLevelL();
    ReInitTestL();
    TestCriticalLevelL();

    // Test import/export LMs when critical level is set to a small value.
    iLog->Put(_L("\r\nRedefining critical level to 198 KB and running all tests again...."));
    CopyResourceFileL(KInternalServicesSmallResFile);
    InitiateCriticalLevelL();
    ReInitTestL();
    TestCriticalLevelL();
    */
#endif
    }

// ---------------------------------------------------------
// CPosTp29::ReInitTestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp29::ReInitTestL()
    {
    iGlobalErr = KErrNone;
    iFile1.Close();
    iFile2.Close();

    CFileMan* fileMan = CFileMan::NewL(iFileServer);
    fileMan->Delete(KFile1);
    fileMan->Delete(KFile2);
    delete fileMan;

    User::LeaveIfError(iFile1.Create(iFileServer, KFile1, EFileWrite));
    User::LeaveIfError(iFile2.Create(iFileServer, KFile2, EFileWrite));
    }

// ---------------------------------------------------------
// CPosTp29::TestCriticalLevelL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp29::TestCriticalLevelL()
    {

    TTime startTime;
    TTime stopTime;

    startTime.UniversalTime();

    RemoveDefaultDbL();

    iLog->Put(_L("CPosLandmarkDatabase::OpenL();"));
    iLandmarksDb = CPosLandmarkDatabase::OpenL();
    if (iLandmarksDb->IsInitializingNeeded())
       {
       iLog->Put(_L("iLandmarksDb->InitializeL"));
       ExecuteAndDeleteLD(iLandmarksDb->InitializeL());        
       }

    iLog->Put(_L("CPosLmCategoryManager::NewL;"));
    iCategoryManager = CPosLmCategoryManager::NewL(*iLandmarksDb);

    // Asynchronous
    iLog->Put(_L("DoOutOfDiskTestL Asynchronous"));
//    iSearchCallback=ETrue;
    DoOutOfDiskTestL(ETrue);
  //  iSearchCallback=EFalse;

    // Delete "out-of-disk" file
    iFile1.Close();
    iFileServer.Delete(KFile1);
    iFile2.Close();
    iFileServer.Delete(KFile2);
    TInt err = iFile1.Open(iFileServer, KFile1, EFileWrite);
    if (err == KErrNotFound)
        {
        err = iFile1.Create(iFileServer, KFile1, EFileWrite);
        }
    User::LeaveIfError(err);
    
    err = iFile2.Open(iFileServer, KFile2, EFileWrite);
    if (err == KErrNotFound)
        {
        err = iFile2.Create(iFileServer, KFile2, EFileWrite);
        }
    User::LeaveIfError(err);

    // Synchronous
    iLog->Put(_L("DoOutOfDiskTestL Synchronous"));
    DoOutOfDiskTestL(EFalse);

    stopTime.UniversalTime();
    TTimeIntervalMicroSeconds executionTime = 
        stopTime.MicroSecondsFrom(startTime);

    //TInt64 resse = executionTime.Int64()/1000000;

    TInt resse = executionTime.Int64()/1000000;

    TBuf<50> buf;
    buf.Zero();
    buf.Format(_L("TP29 took %d seconds"), resse);
    iLog->Put(buf);
    }

// ---------------------------------------------------------
// CPosTp29::DoOutOfDiskTestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp29::DoOutOfDiskTestL(TBool aSync)
    {
    TVolumeInfo volumeInfo;
    TBuf<200> buf;
    CPosLmOperation* op = NULL;

    _LIT(KUID,"Unique ID: %08x\r\n");
	_LIT(KSize,"Size: %d bytes\r\n");
	_LIT(KFree,"Free space: %d bytes\r\n");
	//_LIT(KVolName,"Volume name: %S\r\n");

    TInt err=iFileServer.Volume(volumeInfo, EDriveC);
    if (err != KErrNone) iLog->Put(_L("Error from iFileServer"));

    buf.AppendFormat(KUID, volumeInfo.iUniqueID);
	buf.AppendFormat(KSize, volumeInfo.iSize);
	buf.AppendFormat(KFree, volumeInfo.iFree);
	//buf.AppendFormat(KVolName, &volumeInfo.iName);
    iLog->Put(buf);

    TTime startTime;
    TTime stopTime;

    // Just in case
    ExecuteAndDeleteLD(iLandmarksDb->CompactL());

    RArray<TPosLmItemId> lmIdArray;
    CleanupClosePushL(lmIdArray);

    // 1) Create a low disc space situation
    startTime.UniversalTime();
    ConsumeDiscSpace1L(iCriticalLevel + KSizeLimitOffset1);
    ConsumeDiscSpace2L(iCriticalLevel + KSizeLimitOffset2);
    
    err=iFileServer.Volume(volumeInfo, EDriveC);
    if (err != KErrNone) iLog->Put(_L("Error from iFileServer"));
    buf.AppendFormat(KUID, volumeInfo.iUniqueID);
	buf.AppendFormat(KSize, volumeInfo.iSize);
	buf.AppendFormat(KFree, volumeInfo.iFree);
    iLog->Put(buf);

    stopTime.UniversalTime();
    TTimeIntervalMicroSeconds executionTime = 
        stopTime.MicroSecondsFrom(startTime);

    TInt resse = executionTime.Int64()/1000000;
    buf.Zero();
    buf.Format(_L("ConsumeDiscSpaceL took %d seconds"), resse);
    iLog->Put(buf);

    // 2) Add Landmarks until "disk full" error occur
    if (iLandmarksDb->IsInitializingNeeded())
        {
        iLog->Put(_L("InitializeL"));
        // Synchronous 
        ExecuteAndDeleteLD(iLandmarksDb->InitializeL());        
        }

    CPosLandmark* landmark = CPosLandmark::NewLC();
    landmark->SetLandmarkNameL(_L("LandMarkName1"));
    iLog->Put(_L("Adding landmark1"));
    lmIdArray.Append(iLandmarksDb->AddLandmarkL(*landmark));
    CleanupStack::Pop(landmark);

    landmark = CPosLandmark::NewLC();
    landmark->SetLandmarkNameL(_L("LandMarkName2"));
    iLog->Put(_L("Adding landmark2"));
    lmIdArray.Append(iLandmarksDb->AddLandmarkL(*landmark));
    CleanupStack::Pop(landmark);
    err = KErrNone;
    TPosLmItemId idde;
    TInt i=0;
    
    startTime.UniversalTime();
    iLog->Put(_L("Adding landmark until KErrDiskFull.....1"));
    // some while loop adding landmarks here until "disk full" error occur!!!
    while (err != KErrDiskFull)
        {
        buf.Zero();
        buf.Format(_L("Land Mark___________%d"), i++);
        landmark = CPosLandmark::NewLC();
        landmark->SetLandmarkNameL(buf);
        TRAP(err, idde = iLandmarksDb->AddLandmarkL(*landmark));
        if (err == KErrNone)
            {
            lmIdArray.Append(idde);
            }
        else if (err == KErrLocked) 
            {
            iLog->Put(_L("KErrLocked when adding 1"));
            User::After(100000);
            }
        CleanupStack::Pop(landmark);
        }
    stopTime.UniversalTime();
    executionTime = stopTime.MicroSecondsFrom(startTime);
    resse = executionTime.Int64()/1000000;
    buf.Zero();
    buf.Format(_L("Added %d nr of landmarks, took %d seconds"), i, resse);
    iLog->Put(buf);

    // Check that the filesize limit has not been exceeded 
    err=iFileServer.Volume(volumeInfo, EDriveC);
    buf.Zero();
	buf.AppendFormat(KFree, volumeInfo.iFree);
    iLog->Put(buf);
    if (volumeInfo.iFree < iCriticalLevel)
        {
        LogErrorAndLeave(_L("The max size limit has been exceeded!!!"));
        }

    //Check that the added landmarks can be read
    iLog->Put(_L("Check that it is possible to read landmark"));
    TInt nrOfLandmarks = lmIdArray.Count();
    TInt p=0;
    for (p=0;p<nrOfLandmarks;p++)
        {
        landmark = iLandmarksDb->ReadLandmarkLC(lmIdArray[p]);
        CleanupStack::Pop(landmark);
        }

    // 3) Remove two landmarks and compress landmarks db
    TInt nr = lmIdArray.Count()-1;
    TPosLmItemId id = lmIdArray[nr];
    iLog->Put(_L("Removing landmark1"));
    lmIdArray.Remove(nr);
    TRAP(err, iLandmarksDb->RemoveLandmarkL(id));
    if (err == KErrDiskFull) 
        {
        ExecuteAndDeleteLD(iLandmarksDb->CompactL());

        iLandmarksDb->RemoveLandmarkL(id);
        iLog->Put(_L("DiskFull when removing1"));
        }

    nr = lmIdArray.Count() - 1;
    id = lmIdArray[nr];
    iLog->Put(_L("Removing landmark2"));
    lmIdArray.Remove(nr);
    iLandmarksDb->RemoveLandmarkL(id);
    TRAP(err, iLandmarksDb->RemoveLandmarkL(id));
    if (err == KErrDiskFull) 
        {
        ExecuteAndDeleteLD(iLandmarksDb->CompactL());
        
        iLandmarksDb->RemoveLandmarkL(id);
        iLog->Put(_L("DiskFull when removing2"));
        }
    err = KErrNone;
    iLog->Put(_L("Compacting"));
    ExecuteAndDeleteLD(iLandmarksDb->CompactL());
    
    startTime.UniversalTime();
    iLog->Put(_L("Adding landmark until KErrDiskFull.....2"));
    // 4) Add landmarks until "disk full" error occur!!!
    while (err != KErrDiskFull)
        {
        buf.Zero();
        buf.Format(_L("Land Mark___________%d"), i++);
        landmark = CPosLandmark::NewLC();
        landmark->SetLandmarkNameL(buf);
        //iLog->Put(_L("Adding landmark id to idArray"));
        TRAP(err, idde = iLandmarksDb->AddLandmarkL(*landmark));
        if (err == KErrNone)
            {
            lmIdArray.Append(idde);
            }
        else if (err == KErrLocked) 
            {
            iLog->Put(_L("KErrLocked when adding 2"));
            User::After(100000);
            }
        CleanupStack::Pop(landmark);
        }

    stopTime.UniversalTime();
    executionTime = stopTime.MicroSecondsFrom(startTime);
    resse = executionTime.Int64()/1000000;
    buf.Zero();
    buf.Format(_L("Added %d nr of landmarks, took %d seconds"), i, resse);
    iLog->Put(buf);

    iLog->Put(_L("Check that it is possible to read landmark"));
    //Check that the added landmarks can be read
    nrOfLandmarks = lmIdArray.Count();
    for (p=0;p<nrOfLandmarks;p++)
        {
        landmark = iLandmarksDb->ReadLandmarkLC(lmIdArray[p]);
        CleanupStack::Pop(landmark);
        }

    iLog->Put(_L("Remove all landmarks"));
    err = KErrNone;
    // 5) Remove all landmarks and compact database
    if (!aSync)
        {
        TRAP(err, op = iLandmarksDb->RemoveAllLandmarksL());
        if (err == KErrNone) 
            {
            ExecuteAndDeleteLD(op);
            }
        else if (err == KErrDiskFull)
            {
            iLog->Put(_L("KErrDiskFull when removing all landmarks"));
            ExecuteAndDeleteLD(iLandmarksDb->CompactL());
            
            // Delete KFile2 to free more space on disc, method also open connection to iFile2 again
            DeleteFile2L();

            iLog->Put(_L("Removing after Compact sync"));
            TRAP(err, op = iLandmarksDb->RemoveAllLandmarksL());
            if (err == KErrDiskFull)
                {
                iLog->Put(_L("KErrDiskFull when Removing all landmarks after compact1"));
                RemoveAllLandmarksL();
                }
            else
                {
                ExecuteAndDeleteLD(op);
                }
            }
        }
    else
        {
        iLog->Put(_L("Before RemoveAllLandmarksL(this)"));
        TRAP(err, op = iLandmarksDb->RemoveAllLandmarksL());
        
        if (err == KErrNone) 
            {
            //***iActiveSchedulerWait->Start();
            //***delete op; op=NULL;
            RunAsyncOperationLD(op);
            }
        else if (err == KErrDiskFull)
            {
            iLog->Put(_L("KErrDiskFull when removing all landmarks"));
            ExecuteAndDeleteLD(iLandmarksDb->CompactL());
            
            // Delete KFile2 to free more space on disc, method also open connection to iFile2 again
            DeleteFile2L();

            iLog->Put(_L("Removing after Compact async"));
            TRAP(err, op = iLandmarksDb->RemoveAllLandmarksL());
            if (err == KErrDiskFull)
                {
                iLog->Put(_L("KErrDiskFull when removing all landmarks after compact2"));
                RemoveAllLandmarksL();
                }
            else
                {
                //***iActiveSchedulerWait->Start();
                //***delete op; op=NULL;
                RunAsyncOperationLD(op);
                }
            }
        iLog->Put(_L("After RemoveAllLandmarksL(this)"));
        }
    
    ConsumeDiscSpace2L(iCriticalLevel + KSizeLimitOffset2);

    if (err == KErrDiskFull) iLog->Put(_L("KErrDiskFull when RemoveAllLandmarksL"));
    iLog->Put(_L("Compacting"));
    ExecuteAndDeleteLD(iLandmarksDb->CompactL());
    
    lmIdArray.Reset();
    i=0;
    err = KErrNone;
    startTime.UniversalTime();
    iLog->Put(_L("Adding landmarkcategories until KErrDiskFull....."));

    // 6) Add LandmarkCategories until "disk full" error occur
    while (err != KErrDiskFull)
        {
        buf.Zero();
        buf.Format(_L("Landmark Category _________ %d"), i++);
        CPosLandmarkCategory* lmc = CPosLandmarkCategory::NewLC();
        lmc->SetCategoryNameL(buf);
        lmc->SetIconL(_L("Pelles Icon"), 111, KPosLmIconMaskNotUsed);

        TRAP(err, idde = iCategoryManager->AddCategoryL(*lmc));
        if (err == KErrNone)
            {
            lmIdArray.Append(idde);
            }
        else if (err== KErrLocked)
            {
            iLog->Put(_L("KErrLocked when adding category"));
            User::After(100000);
            }
        CleanupStack::PopAndDestroy(lmc);
        }

    stopTime.UniversalTime();
    executionTime = stopTime.MicroSecondsFrom(startTime);

    resse = executionTime.Int64()/1000000;
    buf.Zero();
    buf.Format(_L("Added %d nr of landmarkcategories, took %d seconds"), i, resse);
    iLog->Put(buf);

    //Check that the added landmarkcategories can be read
    TInt nrOfCategories = lmIdArray.Count();
    for (p=0;p<nrOfCategories;p++)
        {
        CPosLandmarkCategory* lmc = iCategoryManager->ReadCategoryLC(lmIdArray[p]);
        CleanupStack::Pop(lmc);
        }

    // 7) Remove two categories and compress landmarks db
    nr = lmIdArray.Count()-1;
    id = lmIdArray[nr];
    iLog->Put(_L("Removing landmarkcategory1"));
    lmIdArray.Remove(nr);
    op=NULL;
    TRAP(err, op = iCategoryManager->RemoveCategoryL(id));
    if (err == KErrDiskFull)
        {
        ExecuteAndDeleteLD(iLandmarksDb->CompactL());
        ExecuteAndDeleteLD(iCategoryManager->RemoveCategoryL(id));
        iLog->Put(_L("KErrDiskFull when removing landmarkcategory1"));
        }

    if (err == KErrNone)
        {
        ExecuteAndDeleteLD(op);
        }
    else
        {
        delete op;
        op = NULL;
        }

    TBuf<50> buffe;
    buffe.Format(_L("1) Err after RemoveCategoryL: %d"), err);
    iLog->Put(buffe);

    nr = lmIdArray.Count()-1;
    id = lmIdArray[nr];
    iLog->Put(_L("Removing landmarkcategory2"));
    lmIdArray.Remove(nr);

    if (!aSync)
        {
        TRAP(err, op = iCategoryManager->RemoveCategoryL(id));
        if (err == KErrDiskFull)
            {
            delete op;
            op = NULL;
            iLog->Put(_L("KErrDiskFull when removing landmarkcategory2"));
            ExecuteAndDeleteLD(iLandmarksDb->CompactL());
            
            iLog->Put(_L("After compact before removing landmarkcategory2"));
            ExecuteAndDeleteLD(iCategoryManager->RemoveCategoryL(id));
            }
        else if (err == KErrNone)
            {
            ExecuteAndDeleteLD(op);
            }
        else if (err == KErrLocked)
            {
            iLog->Put(_L("KErrLocked returned from iCategoryManager->RemoveCategoryL"));
            delete op;
            op = NULL;
            }
        } 
    else
        {
        TRAP(err, op = iCategoryManager->RemoveCategoryL(id));
        if (err == KErrNone)
            {
            //***iActiveSchedulerWait->Start();
            //***delete op;
            RunAsyncOperationLD(op);
            }
        else if (err == KErrDiskFull)
            {
            delete op;
            op = NULL;
            iLog->Put(_L("KErrDiskFull when removing landmarkcategory2 async"));
            ExecuteAndDeleteLD(iLandmarksDb->CompactL());
            
            iLog->Put(_L("After compact before removing landmarkcategory2 async"));
            op = iCategoryManager->RemoveCategoryL(id);
            
            //***iActiveSchedulerWait->Start();
            //***delete op;
            RunAsyncOperationLD(op);
            }
        else if (err == KErrLocked)
            {
            delete op;
            op = NULL;
            iLog->Put(_L("KErrLocked returned from iCategoryManager->RemoveCategoryL"));
            }
        }

    buffe.Zero();
    buffe.Format(_L("2) Err after RemoveCategoryL: %d"), err);
    iLog->Put(buffe);

    iLog->Put(_L("Compacting"));
    ExecuteAndDeleteLD(iLandmarksDb->CompactL());
    iLog->Put(_L("After Compacting"));
    
    lmIdArray.Reset();
    i=0;
    err = KErrNone;

    startTime.UniversalTime();
    // 8) Add LandmarkCategories until "disk full" error occur
    while (err != KErrDiskFull)
        {
        buf.Zero();
        buf.Format(_L("LandmarkCategory %d"), i++);

        CPosLandmarkCategory* lmc = CPosLandmarkCategory::NewLC();
        lmc->SetCategoryNameL(buf);
        lmc->SetIconL(_L("PellesIcon"), 111, KPosLmIconMaskNotUsed);
        iLog->Put(buf);
        
        TRAP(err, idde = iCategoryManager->AddCategoryL(*lmc));
        if (err == KErrNone)
            {
            lmIdArray.Append(idde);
            }
        else if (err == KErrLocked)
            {
            iLog->Put(_L("KerrLocked when addCategory"));
            User::After(100000);
            }
        CleanupStack::PopAndDestroy(lmc); 
    }
    stopTime.UniversalTime();
    executionTime = stopTime.MicroSecondsFrom(startTime);

    resse = executionTime.Int64()/1000000;
    buf.Zero();
    buf.Format(_L("Added %d nr of landmarkcategories, took %d seconds"), i, resse);
    iLog->Put(buf);

    // 9) Remove all categories and compact db
    // sync method
    // XXXX add async check here
    iLog->Put(_L("Removing all categories"));
    TRAP(err, iCategoryManager->RemoveCategoriesL(lmIdArray));
    if (err == KErrDiskFull) iLog->Put(_L("KErrDiskFull when RemoveCategoriesL"));
    iLog->Put(_L("Compacting"));
    ExecuteAndDeleteLD(iLandmarksDb->CompactL());
    

    // 10) Consume more space then the limit specified in resource file
    ConsumeDiscSpace1L(iCriticalLevel - KSizeLimitOffset2);

    // 11) Try to add a landmark
    buf.Zero();
    buf.Format(_L("LandMark%d"), i++);
    landmark = CPosLandmark::NewLC();
    landmark->SetLandmarkNameL(buf);

    TRAP(err, idde = iLandmarksDb->AddLandmarkL(*landmark));
    if (err != KErrDiskFull)
        {
        buf.Zero();
        buf.Format(_L("Should leave with KErrDiskFull instead %d"), err);
        //LogErrorAndLeave(buf);
        LogErrorAndLeave(buf);
        }
    CleanupStack::PopAndDestroy(landmark);
    CleanupStack::PopAndDestroy(&lmIdArray);
    }


// ---------------------------------------------------------
// CPosTp29::RemoveAllLandmarksL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp29::RemoveAllLandmarksL()
    {
    iLog->Put(_L("CPosTp29::RemoveAllLandmarksL"));
    CPosLmItemIterator* iter = iLandmarksDb->LandmarkIteratorL();
    CleanupStack::PushL(iter);

    TInt counter=0;
    TTime startTime;
    TTime stopTime;
    TPosLmItemId id = iter->NextL();

    startTime.UniversalTime();

    while (id != KPosLmNullItemId)
        {
        TRAPD(err, iLandmarksDb->RemoveLandmarkL(id));
        if (err == KErrDiskFull)
            {
            ExecuteAndDeleteLD(iLandmarksDb->CompactL());
            
            iLog->Put(_L(":-( DiskFull when removing single landmark entry !!!"));
            iLandmarksDb->RemoveLandmarkL(id);
            }
        else if (err == KErrLocked)
            {
            iLog->Put(_L("KErrLocked when removing landmark"));
            User::After(100000);
            }
        id = iter->NextL();
        counter++;
        }

    stopTime.UniversalTime();
    TTimeIntervalMicroSeconds executionTime = 
        stopTime.MicroSecondsFrom(startTime);

    TInt64 resse = executionTime.Int64()/1000000;
    TBuf<50> buf;
    buf.Zero();
    buf.Format(_L("Removing %d nr of landmarks, took %d seconds"), counter, resse);
    iLog->Put(buf);

    CleanupStack::PopAndDestroy(iter);
    }

// ---------------------------------------------------------
// CPosTp29::DeleteFile2L
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp29::DeleteFile2L()
    {
    iFile2.Close();
    iFileServer.Delete(KFile2);
    TInt err = iFile2.Open(iFileServer, KFile2, EFileWrite);
    if (err == KErrNotFound)
        {
        err = iFile2.Create(iFileServer, KFile2, EFileWrite);
        }
    }

// ---------------------------------------------------------
// CPosTp29::ConsumeDiscSpace1L
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp29::ConsumeDiscSpace1L(TInt aSizeLimit)
    {
    TInt err = KErrNone;

    // Approx 2160 bytes
    _LIT8(KNonsense, "nonsensenonsensenonsensenonsensenonsensenon \
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense \
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense \
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense \
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense \
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense \
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense \
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense \
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense \
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense \
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense \
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense \
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense \
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense \
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense \
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense \
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense \
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense \
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense \
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense \
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense \
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense \
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense \
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense \
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense \
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense \
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense \
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense \
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense \
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense \
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense \
sensenonsensenonsensenonsensenonsensenonsensenonsense");

    TVolumeInfo volumeInfo;
    err=iFileServer.Volume(volumeInfo, EDriveC);

    // Fill file fast
    HBufC8* bigBuf = (&KNonsense)->AllocLC();
    while (volumeInfo.iFree > aSizeLimit)
        {
        err = iFile1.Write(*bigBuf);
        if (err != KErrNone) iLog->Put(_L("Problem writing"));
        err = iFileServer.Volume(volumeInfo, EDriveC);
        if (err != KErrNone) iLog->Put(_L("Problem voluming"));
        }

    CleanupStack::PopAndDestroy(1); // bigBuf
    }

// ---------------------------------------------------------
// CPosTp29::ConsumeDiscSpace2L
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp29::ConsumeDiscSpace2L(TInt aSizeLimit)
    {
    iLog->Put(_L("ConsumeDiscSpace2L"));
    TInt err = KErrNone;

    // Approx 2160 bytes
    _LIT8(KNonsense, "nonsensenonsensenonsensenonsensenonsensenon \
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense \
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense \
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense \
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense \
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense \
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense \
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense \
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense \
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense \
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense \
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense \
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense \
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense \
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense \
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense \
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense \
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense \
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense \
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense \
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense \
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense \
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense \
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense \
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense \
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense \
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense \
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense \
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense \
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense \
nonsensenonsensenonsensenonsensenonsensenonsensenonsensenonsense \
sensenonsensenonsensenonsensenonsensenonsensenonsense");

    TVolumeInfo volumeInfo;
    err=iFileServer.Volume(volumeInfo, EDriveC);

    // Fill file fast
    HBufC8* bigBuf = (&KNonsense)->AllocLC();
    while (volumeInfo.iFree > aSizeLimit)
        {
        err = iFile2.Write(*bigBuf);
        if (err != KErrNone) iLog->Put(_L("Problem writing"));
        err = iFileServer.Volume(volumeInfo, EDriveC);
        if (err != KErrNone) iLog->Put(_L("Problem voluming"));
        }

    CleanupStack::PopAndDestroy(1); // bigBuf
    }

// ---------------------------------------------------------
// CPosTp29::InitiateCriticalLevelL
// Extracts Critical Dsik Level. (value is extracted from an MLFW 
// resource file that can reside on any drive)
// (other items were commented in a header).
// ---------------------------------------------------------
//	
void CPosTp29::InitiateCriticalLevelL()
    {
    iLog->Put(_L("InitiateCriticalLevelL"));
    /*
    // Find drive that the resource file resides on.
    TFindFile* filefinder = new (ELeave) TFindFile(iFileServer);
    CleanupStack::PushL(filefinder);
    TParse* fileparser = new (ELeave) TParse;
    CleanupStack::PushL(fileparser);
    fileparser->Set(KPosResourceFile, NULL, NULL);
    User::LeaveIfError(
        filefinder->FindByDir(fileparser->NameAndExt(), fileparser->DriveAndPath()));

    // Open resource file
    RResourceFile resFile;
    resFile.OpenL(iFileServer, filefinder->File());
    CleanupClosePushL(resFile);

    // Allocate the critical level as a buffer
    //TInt R_POS_LANDMARK_INTERNAL_CRITICAL_DISK_LEVEL = 250;
    HBufC8* resourceBuffer = resFile.AllocReadLC(iCriticalThreshold);

    // Convert the buffer to an int
    TResourceReader resReader;
    resReader.SetBuffer(resourceBuffer);
    iCriticalLevel = KOneK * resReader.ReadInt16();
    CleanupStack::PopAndDestroy(4, filefinder);
    */
    
    //iCriticalLevel = iCriticalThreshold * KOneK;
    iCriticalLevel = iCriticalThreshold;
    
    TBuf<50> msg;
    msg.Format(_L("CriticalLevel = %d"), iCriticalLevel);
    iLog->Put(msg);
    }

// ---------------------------------------------------------
// CPosTp29::CopyResourceFileL
// 
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp29::CopyResourceFileL(const TDesC& aResourceFile)
    {
    // Release all landmark references to make DLLs be unloaded.
    delete iLandmarksDb;
    iLandmarksDb = NULL;

    CFileMan* fileMan = CFileMan::NewL(iFileServer);
    CleanupStack::PushL(fileMan);
    
    // Check if landmark is flashed
     if (!BaflUtils::FileExists(iFileServer, KInternalServicesResFileRom))
        {
         iLog->Put(_L("Landmark is NOT flashed, backing up eposlmintservices.rsc by renaming it bofore copying rsc-file."));
        // Landmark FW is not flashed. Rename the file before copying an own defiend file.
        // Since landmark FW is not flashed the file should exist hence leaving if it is not found!
         User::LeaveIfError(fileMan->Rename(KInternalServicesResFileCdrive, KInternalServicesResFileBackup, CFileMan::EOverWrite));
        }
     else
        {
        iLog->Put(_L("Landmark framework is flashed, copying global categories to c:"));
        }

    User::LeaveIfError(fileMan->Copy(aResourceFile, KInternalServicesResFileCdrive, CFileMan::EOverWrite));
 
    CleanupStack::PopAndDestroy(fileMan);
    }

// ---------------------------------------------------------
// CPosTp29::TryRestoreResourceFile
// 
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp29::TryRestoreResourceFile()
    {
    iLog->Put(_L("TryRestoreResourceFile"));
    CFileMan* fileMan = NULL;
    TRAPD(err, fileMan = CFileMan::NewL(iFileServer));
    User::LeaveIfError(err);
    if (fileMan)
        {
        // Try to restore the backup up rsc-file (only possible when LM FW is not flashed.
        TInt result = fileMan->Copy(KInternalServicesResFileBackup, KInternalServicesResFileCdrive, CFileMan::EOverWrite);
        if (result == KErrNotFound)
            {
            if (BaflUtils::FileExists(iFileServer, KInternalServicesResFileRom))
                {
                // LM FW is flashed. We need to remove the rsc-file on c:
                fileMan->Delete(KInternalServicesResFileCdrive);
                }
            }
        delete fileMan;
        }
    }

// End of file

