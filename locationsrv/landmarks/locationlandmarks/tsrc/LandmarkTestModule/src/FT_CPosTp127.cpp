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
#include "FT_CPosTp127.h"
#include <Lbs.h>
#include <EPos_CPosLmDatabaseManager.h>
#include <EPos_CPosLandmarkParser.h>
#include <EPos_CPosLandmarkEncoder.h>
#include <EPos_CPosLandmark.h>
#include <EPos_CPosLmMultiDbSearch.h>
#include <EPos_CPosLmCatNameCriteria.h>
#include <EPos_CPosLmTextCriteria.h>
#include <EPos_CPosLmCategoryCriteria.h>
#include "FT_LandmarkConstants.h"

// CONSTANTS
const TInt KShortExecutionTimeInSeconds = 15;
const TInt KLongExecutionTimeInSeconds = 5 * 60; // Five minutes. The longer the more errors seem appear...
const TInt KNrOfDatabases = 5;
const TInt KLockedDbIndex = 2;
const TInt KMaxNrOfLms = 20;
const TInt KLockThreadStartupTime = 500000;
_LIT(KCreateThreadErr, "Creating thread failed with %d");
_LIT(KLockThreadSearchPattern, "*TP127_LockThread*"); 
_LIT(KTestDbUri, "TestDatabase.ldb");
_LIT(KTestDbUriUC, "TESTDATABASE.LDB");

// ================= LOCAL FUNCTIONS =======================

// ---------------------------------------------------------
// LOCAL_C ThreadFunction
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
LOCAL_C TInt ThreadFunction(TAny* aData)
    {
    CTrapCleanup* cleanup = CTrapCleanup::New(); 
    CActiveScheduler* actSch = new (ELeave) CActiveScheduler;
    CActiveScheduler::Install(actSch);
    
    TThreadParam* params = reinterpret_cast<TThreadParam*> (aData);
    TRAPD(err, (*params->iTestFunction)(params->iMessage));
       
    delete actSch;
    delete cleanup;
    return err;
    }

// ---------------------------------------------------------
// LOCAL_C AssertTrueL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
LOCAL_C void AssertTrueL(
    TBool aCondition, 
    const TDesC& aBuf, 
    TDes* aMessage, 
    TInt aError = KErrGeneral)
    {
    if (!aCondition)
        {
        _LIT(KFormat, "%S, error = %d");
        aMessage->Format(KFormat, &aBuf, aError);
        
        aError == KErrNone ? User::Leave(KErrGeneral) : User::Leave(aError);
        }
    }
    
// ---------------------------------------------------------
// LOCAL_C ThreadExistsL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
LOCAL_C TBool ThreadExistsL(const TDesC& aSearchPattern)
    {
    TFindThread threadSearcher;
    threadSearcher.Find(aSearchPattern);
    TFullName fullName;
    return (threadSearcher.Next(fullName) == KErrNone);
    }

// ---------------------------------------------------------
// LOCAL_C VerifyResultL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
LOCAL_C void VerifyResultL(
    TBool aLockThreadExists, 
    CPosLmMultiDbSearch* aMultiDbSearcher, 
    TDes* aDes)
    {
    TInt nrOfErrors = aMultiDbSearcher->NumOfSearchErrors();
    if (aLockThreadExists)
        {
        // Reading locked db should result in KErrLocked
        AssertTrueL(nrOfErrors == 1, _L("Not one error as expected"), aDes);
        CPosLmMultiDbSearch::TSearchError error;
        aMultiDbSearcher->GetSearchError(0, error);
        AssertTrueL(error.iDatabaseIndex == KLockedDbIndex, _L("Error in wrong db"), aDes);
        AssertTrueL(error.iErrorCode == KErrLocked, _L("Wrong error"), aDes, error.iErrorCode);
        }
    else
        {
        AssertTrueL(nrOfErrors == 0, _L("Search failed with errors"), aDes);
        }
        
    aDes->Zero();    
    }

// ================= CThreadMonitor MEMBER FUNCTIONS =======================

// Constructor
CThreadMonitor::CThreadMonitor(RThread* aThread) 
: CActive(CActive::EPriorityStandard), iThread(aThread)
    {
    CActiveScheduler::Add(this);
    }

// Destructor
CThreadMonitor::~CThreadMonitor()
    {
    Cancel();
    }

// ---------------------------------------------------------
// CThreadMonitor::NewL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
CThreadMonitor* CThreadMonitor::NewL(RThread* aThread)
    {
    CThreadMonitor* self = new (ELeave) CThreadMonitor(aThread);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------
// CThreadMonitor::ConstructL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CThreadMonitor::ConstructL()
    {
    iThread->Resume();
    iThread->Logon(iStatus);
    SetActive();
    }

// ---------------------------------------------------------
// CThreadMonitor::DoCancel
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CThreadMonitor::DoCancel()
    {
    iThread->LogonCancel(iStatus);

    // Kill thread
    TRequestStatus status;
    iThread->Logon(status);
    iThread->Kill(KErrCancel);
    User::WaitForRequest(status);
    }

// ---------------------------------------------------------
// CThreadMonitor::RunL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CThreadMonitor::RunL()
    {
    CActiveScheduler::Stop();
    }

// ================= CPosTp127 MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CPosTp127::GetName
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp127::GetName(TDes& aName) const
    {
    _LIT(KTestName, "TP127 - Multiple databases, multiple clients");
    aName = KTestName;
    }

// ---------------------------------------------------------
// CPosTp127::InitTestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp127::InitTestL()
    {
    MakeSurePanicDebugFileExistsL();
    
    // PrepareDatabases
    RemoveAllLmDatabasesL();
    CopyTestDbFileL(KDb20);
    CopyTestDbFileL(KDb40);
    CopyTestDbFileL(KDb60);
    CopyTestDbFileL(KDb80);
    CopyTestDbFileL(KDb105);

    // Initialize databases - is this really needed?
    CPosLmDatabaseManager* dbMan = CPosLmDatabaseManager::NewL();
    CleanupStack::PushL(dbMan);
    CDesCArray* dbList = dbMan->ListDatabasesLC();
    for (TInt i = 0; i < KNrOfDatabases; i++)
        {
        CPosLandmarkDatabase* db = CPosLandmarkDatabase::OpenL(dbList->operator[](i));
        CleanupStack::PushL(db);
        ExecuteAndDeleteLD(db->InitializeL());
        CleanupStack::PopAndDestroy(db);
        }

    // Remove the last 5 lms of the the last db
    CPosLandmarkDatabase* db = CPosLandmarkDatabase::OpenL(KDb105);
    CleanupStack::PushL(db);
    CPosLmItemIterator* iter = db->LandmarkIteratorL();
    CleanupStack::PushL(iter);
    RIdArray idArray;
    CleanupClosePushL(idArray);
    iter->GetItemIdsL(idArray, 20, iter->NumOfItemsL() - 20);
    ExecuteAndDeleteLD(db->RemoveLandmarksL(idArray));
    CleanupStack::PopAndDestroy(3, db);
    
    CleanupStack::PopAndDestroy(2, dbMan);
    }
    
// ---------------------------------------------------------
// CPosTp127::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp127::CloseTest()
    {
    }

// ---------------------------------------------------------
// CPosTp127::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp127::StartL()
    {
    // 1. Create multiple db search threads 
    iLog->Put(_L("<<<< 1. Create multiple db search threads >>>>"));
    TBool lockDb = ETrue;
    TBool successful = MultipleDbSearchClientsL(!lockDb);
    //TBool successful = MultipleDbSearchClientsL(lockDb);
    
    // Sometimes error ESLI-69LDJD DBMS - Error occurs
    // 2. One of the databases is locked
    iLog->Put(_L("<<<< 2. One of the databases is locked >>>>"));
    User::After(KLockThreadStartupTime); // Let previous thread instances really die
    if (!MultipleDbSearchClientsL(lockDb))
        {        
        successful = EFalse;
        }
        
    // 3. Copy database in use
    iLog->Put(_L("<<<< 3. Copy database in use >>>>"));
    if (!VerifyInUseWhenCopyL())
        {        
        successful = EFalse;
        }
   
    // 4. Copy databases continuously in parallel
    iLog->Put(_L("<<<< 4. Copy databases continuously in parallel >>>>"));
    if (!VerifyCopyStressL())
        {        
        successful = EFalse;
        }
   
    // 5. Use all LM APIs
    iLog->Put(_L("<<<< 5. Use all LM APIs simultaneously >>>>"));
    if (!MultipleLmClientsL())
        {        
        successful = EFalse;
        }
    // Leave if error    
    if (!successful)
        {        
        User::Leave(KErrGeneral);
        }
    }

// ---------------------------------------------------------
// CPosTp127::MultipleDbSearchClientsL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TBool CPosTp127::MultipleDbSearchClientsL(TBool aLockDb)
    {
    RThread searchThread1, searchThread2, searchThread3, lockThread;
    TThreadParam params1, params2, params3, params4;
    TBuf<200> threadMsg1, threadMsg2, threadMsg3, threadMsg4;
    RArray<TThreadParam> threadArray;
    CleanupClosePushL(threadArray);

    // Create search thread 1
    _LIT(KSearchThread1, "TP127 - Search thread 1");
    TInt err = searchThread1.Create(KSearchThread1, ThreadFunction, 
        KDefaultStackSize, KMinHeapSize, KMaxHeapSize, &params1);
    AssertTrueSecL(err == KErrNone, KCreateThreadErr, err);
    CleanupClosePushL(searchThread1);
    params1.iMessage = &threadMsg1;
    params1.iTestFunction = MultipleDbCategorySearchL;
    params1.iThread = &searchThread1;
    threadArray.AppendL(params1);

    // Create search thread 2
    _LIT(KSearchThread2, "TP127 - Search thread 2");
    err = searchThread2.Create(KSearchThread2, ThreadFunction, 
        KDefaultStackSize, KMinHeapSize, KMaxHeapSize, &params2);
    AssertTrueSecL(err == KErrNone, KCreateThreadErr, err);
    CleanupClosePushL(searchThread2);
    params2.iMessage = &threadMsg2;
    params2.iTestFunction = MultipleDbLandmarkSearchL;
    params2.iThread = &searchThread2;
    threadArray.AppendL(params2);

    // Create search thread 3
    _LIT(KSearchThread3, "TP127 - Search thread 3");
    err = searchThread3.Create(KSearchThread3, ThreadFunction, 
        KDefaultStackSize, KMinHeapSize, KMaxHeapSize, &params3);
    AssertTrueSecL(err == KErrNone, KCreateThreadErr, err);
    CleanupClosePushL(searchThread3);
    params3.iMessage = &threadMsg3;
    params3.iTestFunction = MultipleDbLandmarkWithoutCategorySearchL;
    params3.iThread = &searchThread3;
    threadArray.AppendL(params3);
    
    if (aLockDb)
        {
        // Create db lock thread
        _LIT(KLockThread, "TP127 - TP127_LockThread");        
        err = lockThread.Create(KLockThread, ThreadFunction, 
            KDefaultStackSize, KMinHeapSize, KMaxHeapSize, &params4);
        AssertTrueSecL(err == KErrNone, KCreateThreadErr, err);
        CleanupClosePushL(lockThread);
        params4.iMessage = &threadMsg4;
        params4.iTestFunction = LockDatabaseL;
        params4.iThread = &lockThread;
        threadArray.AppendL(params4);
        }

    TBool successful = ResumeThreadsAndVerifyExitL(threadArray); 
    
    if (aLockDb)
        {
        CleanupStack::PopAndDestroy(&lockThread);
        }
    CleanupStack::PopAndDestroy(4, &threadArray);
    return successful;
    }
    
// ---------------------------------------------------------
// CPosTp127::MultipleLmClientsL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TBool CPosTp127::MultipleLmClientsL()
    {
    // Set default db
    CPosLmDatabaseManager* dbMan = CPosLmDatabaseManager::NewL();
    CleanupStack::PushL(dbMan);
    HPosLmDatabaseInfo* dbInfo = HPosLmDatabaseInfo::NewLC(KTestDbUri);
    dbMan->CreateDatabaseL(*dbInfo);
    CleanupStack::PopAndDestroy(dbInfo);
    dbMan->SetDefaultDatabaseUriL(KTestDbUri);
    CleanupStack::PopAndDestroy(dbMan);
    
    RThread dbManThread, multiSearchThread, dbThread, singleSearchThread;
    TThreadParam params1, params2, params3, params4;
    TBuf<200> threadMsg1, threadMsg2, threadMsg3, threadMsg4;
    RArray<TThreadParam> threadArray;
    CleanupClosePushL(threadArray);

    // Create db management thread
    _LIT(KDbManThread, "TP127 - Db manager thread");
    TInt err = dbManThread.Create(KDbManThread, ThreadFunction, 
        KDefaultStackSize, KMinHeapSize, KMaxHeapSize, &params1);
    AssertTrueSecL(err == KErrNone, KCreateThreadErr, err);
    CleanupClosePushL(dbManThread);
    params1.iMessage = &threadMsg1;
    params1.iTestFunction = DbManagementL;
    params1.iThread = &dbManThread;
    threadArray.AppendL(params1);

    // Create multi db search thread
    _LIT(KMultiSearchThread, "TP127 - Multi search thread");
    err = multiSearchThread.Create(KMultiSearchThread, ThreadFunction, 
        KDefaultStackSize, KMinHeapSize, KMaxHeapSize, &params2);
    AssertTrueSecL(err == KErrNone, KCreateThreadErr, err);
    CleanupClosePushL(multiSearchThread);
    params2.iMessage = &threadMsg2;
    params2.iTestFunction = MultipleDbSearchL;
    params2.iThread = &multiSearchThread;
    threadArray.AppendL(params2);

    // Create LM API thread
    _LIT(KDbThread, "TP127 - db thread");
    err = dbThread.Create(KDbThread, ThreadFunction, 
        KDefaultStackSize, KMinHeapSize, KMaxHeapSize, &params3);
    AssertTrueSecL(err == KErrNone, KCreateThreadErr, err);
    CleanupClosePushL(dbThread);
    params3.iMessage = &threadMsg3;
    params3.iTestFunction = DbAccessL;
    params3.iThread = &dbThread;
    threadArray.AppendL(params3);
    
    // Create single db search thread
    _LIT(KSingleSearchThread, "TP127 - Single search thread");        
    err = singleSearchThread.Create(KSingleSearchThread, ThreadFunction, 
        KDefaultStackSize, KMinHeapSize, KMaxHeapSize, &params4);
    AssertTrueSecL(err == KErrNone, KCreateThreadErr, err);
    CleanupClosePushL(singleSearchThread);
    params4.iMessage = &threadMsg4;
    params4.iTestFunction = SingleDbSearchL;
    params4.iThread = &singleSearchThread;
    threadArray.AppendL(params4);

    TBool successful = ResumeThreadsAndVerifyExitL(threadArray); 
    
    CleanupStack::PopAndDestroy(5, &threadArray);
    return successful;
    }
    
// ---------------------------------------------------------
// CPosTp127::VerifyInUseWhenCopyL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TBool CPosTp127::VerifyInUseWhenCopyL()
    {
    RThread copyThread, lockThread;
    TThreadParam params1, params2;
    TBuf<200> threadMsg1, threadMsg2;
    RArray<TThreadParam> threadArray;
    CleanupClosePushL(threadArray);

    // Create copy thread
    _LIT(KCopyThread, "TP127 - Copy thread");
    TInt err = copyThread.Create(KCopyThread, ThreadFunction, 
        KDefaultStackSize, KMinHeapSize, KMaxHeapSize, &params1);
    AssertTrueSecL(err == KErrNone, KCreateThreadErr, err);
    CleanupClosePushL(copyThread);
    params1.iMessage = &threadMsg1;
    params1.iTestFunction = CopyLockedDatabaseL;
    params1.iThread = &copyThread;
    threadArray.AppendL(params1);

    // Create db lock thread
    _LIT(KLockThread, "TP127 - TP127_LockThread");
    err = KErrAlreadyExists;
    while (err == KErrAlreadyExists)
        {
        err = lockThread.Create(KLockThread, ThreadFunction, 
            KDefaultStackSize, KMinHeapSize, KMaxHeapSize, &params2);
        }
    AssertTrueSecL(err == KErrNone, KCreateThreadErr, err);
    CleanupClosePushL(lockThread);
    params2.iMessage = &threadMsg2;
    params2.iTestFunction = LockDatabaseL;
    params2.iThread = &lockThread;
    threadArray.AppendL(params2);

    TBool successful = ResumeThreadsAndVerifyExitL(threadArray); 
    
    CleanupStack::PopAndDestroy(3, &threadArray);
    return successful;
    }
    
// ---------------------------------------------------------
// CPosTp127::VerifyCopyStressL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TBool CPosTp127::VerifyCopyStressL()
    {
    RThread copyThread1, copyThread2, copyThread3, copyThread4, copyThread5;
    TThreadParam params1, params2, params3, params4, params5;
    TBuf<200> threadMsg1, threadMsg2, threadMsg3, threadMsg4, threadMsg5;
    RArray<TThreadParam> threadArray;
    CleanupClosePushL(threadArray);

    // Create copy thread1
    _LIT(KCopyThread1, "TP127 - Copy thread1");
    TInt err = copyThread1.Create(KCopyThread1, ThreadFunction, 
        KDefaultStackSize, KMinHeapSize, KMaxHeapSize, &params1);
    AssertTrueSecL(err == KErrNone, KCreateThreadErr, err);
    CleanupClosePushL(copyThread1);
    params1.iMessage = &threadMsg1;
    params1.iTestFunction = CopyDatabaseL;
    params1.iThread = &copyThread1;
    threadArray.AppendL(params1);

    // Create copy thread2
    _LIT(KCopyThread2, "TP127 - Copy thread2");
    err = copyThread2.Create(KCopyThread2, ThreadFunction, 
        KDefaultStackSize, KMinHeapSize, KMaxHeapSize, &params2);
    AssertTrueSecL(err == KErrNone, KCreateThreadErr, err);
    CleanupClosePushL(copyThread2);
    params2.iMessage = &threadMsg2;
    params2.iTestFunction = CopyDatabaseL;
    params2.iThread = &copyThread2;
    threadArray.AppendL(params2);

    // Create copy thread3
    _LIT(KCopyThread3, "TP127 - Copy thread3");
    err = copyThread3.Create(KCopyThread3, ThreadFunction, 
        KDefaultStackSize, KMinHeapSize, KMaxHeapSize, &params3);
    AssertTrueSecL(err == KErrNone, KCreateThreadErr, err);
    CleanupClosePushL(copyThread3);
    params3.iMessage = &threadMsg3;
    params3.iTestFunction = CopyDatabaseL;
    params3.iThread = &copyThread3;
    threadArray.AppendL(params3);

    // Create copy thread4
    _LIT(KCopyThread4, "TP127 - Copy thread4");
    err = copyThread4.Create(KCopyThread4, ThreadFunction, 
        KDefaultStackSize, KMinHeapSize, KMaxHeapSize, &params4);
    AssertTrueSecL(err == KErrNone, KCreateThreadErr, err);
    CleanupClosePushL(copyThread4);
    params4.iMessage = &threadMsg4;
    params4.iTestFunction = CopyDatabaseL;
    params4.iThread = &copyThread4;
    threadArray.AppendL(params4);

    // Create copy thread5
    _LIT(KCopyThread5, "TP127 - Copy thread5");
    err = copyThread5.Create(KCopyThread5, ThreadFunction, 
        KDefaultStackSize, KMinHeapSize, KMaxHeapSize, &params5);
    AssertTrueSecL(err == KErrNone, KCreateThreadErr, err);
    CleanupClosePushL(copyThread5);
    params5.iMessage = &threadMsg5;
    params5.iTestFunction = CopyDatabaseL;
    params5.iThread = &copyThread5;
    threadArray.AppendL(params5);

    TBool successful = ResumeThreadsAndVerifyExitL(threadArray); 
    
    CleanupStack::PopAndDestroy(6, &threadArray);
    return successful;
    }
    
// ---------------------------------------------------------
// CPosTp127::ResumeThreadsAndVerifyExitL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TBool CPosTp127::ResumeThreadsAndVerifyExitL(const RArray<TThreadParam>& aThreadParams)
    {
    // Start threads.
    RPointerArray<CThreadMonitor> threadMonitors;    
    CleanupClosePushL(threadMonitors);
    TInt nrOfThreads = aThreadParams.Count();
    for (TInt j = 0; j < nrOfThreads; j++)
        {
        threadMonitors.AppendL(CThreadMonitor::NewL(aThreadParams[j].iThread));
        }

// <<<< Commented code below can be enabled for immediate interruption when any thread fails >>>>

    // Wait until any of the threads exits.
    TInt error = KErrNone;
    TBool allThreadsTerminated = EFalse;
    while (!allThreadsTerminated
/* && error == KErrNone*/)
        {
        CActiveScheduler::Start();

        // Assume all threads are terminated
        allThreadsTerminated = ETrue;
        for (TInt i = 0; i < nrOfThreads
/* && error == KErrNone*/; i++)
            {
            if (threadMonitors[i]->IsActive())
                {
                // Thread is still alive
                allThreadsTerminated = EFalse;
                }
            else
                {
/*                if (aThreadParams[i].iThread->ExitType() != EExitKill)
                    {
                    error = KErrGeneral;
                    }
                else if (aThreadParams[i].iThread->ExitReason())
                    {
                    error = aThreadParams[i].iThread->ExitReason();
                    }
*/                }
            }
        }

// <<<< Commented code above can be enabled for immediate interruption when any thread fails >>>>

    // Terminate all remaining threads (in case of errors)
    threadMonitors.ResetAndDestroy();
    CleanupStack::PopAndDestroy(&threadMonitors);

    // Log summary
    for (TInt i = 0; i < nrOfThreads; i++)
        {
        if (*aThreadParams[i].iMessage != KNullDesC)
            {
            iLog->Put(*aThreadParams[i].iMessage);
            }
        TName threadName = aThreadParams[i].iThread->Name();
        if (aThreadParams[i].iThread->ExitType() != EExitKill)
            {
            iBuf.Format(_L("%S was not killed!"), &threadName);
            iLog->PutError(iBuf);
            TExitCategoryName category = aThreadParams[i].iThread->ExitCategory();
            iBuf.Format(_L("Exit Category = %S %d"), &category, aThreadParams[i].iThread->ExitReason());
            iLog->Put(iBuf);
            error = KErrGeneral;
            }
        else
            {
            TInt exitReason = aThreadParams[i].iThread->ExitReason();
            if (exitReason != 0)
                {
                iBuf.Format(_L("%S's exit reason wasn't 0 but %d."), &threadName, exitReason);
                iLog->PutError(iBuf);
                error = exitReason;
                }
            }
        }
    
    return (error == KErrNone);
    }

// ================= STATIC THREAD FUNCTIONS =======================

// ---------------------------------------------------------
// CPosTp127::MultipleDbCategorySearchL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp127::MultipleDbCategorySearchL(TDes* aMessage)
    {
    TBool lockThreadExists = ThreadExistsL(KLockThreadSearchPattern);
    if (lockThreadExists)
        {
        // Let lock thread launch first
        User::After(KLockThreadStartupTime);
        }
        
    // Search multiple databases for some seconds
    TTime startTime, stopTime;
    startTime.HomeTime();
    stopTime.HomeTime();
    TTimeIntervalSeconds executionTime;
    User::LeaveIfError(stopTime.SecondsFrom(startTime, executionTime));

    while (executionTime.Int() < KShortExecutionTimeInSeconds)
        {
        CPosLmDatabaseManager* dbMan = CPosLmDatabaseManager::NewL();
        CleanupStack::PushL(dbMan);
        CDesCArray* dbList = dbMan->ListDatabasesLC();
        AssertTrueL(dbList->Count() == KNrOfDatabases, _L("MultipleDbCategorySearchL: Wrong number of databases"), aMessage);
        CPosLmMultiDbSearch* dbSearcher = CPosLmMultiDbSearch::NewL(*dbList);
        CleanupStack::PushL(dbSearcher);
        
        // Search for categories containing a specific string
        CPosLmCatNameCriteria* criteria = CPosLmCatNameCriteria::NewLC();
        criteria->SetSearchPatternL(_L("*Log*"));
        CPosLmOperation* operation = dbSearcher->StartCategorySearchL(*criteria, CPosLmCategoryManager::ECategorySortOrderNone);
        ExecuteAndDeleteLD(operation);
        
        aMessage->Copy(_L("MultipleDbCategorySearchL: "));
        VerifyResultL(lockThreadExists, dbSearcher, aMessage);
        
        stopTime.HomeTime();
        User::LeaveIfError(stopTime.SecondsFrom(startTime, executionTime));
        
        CleanupStack::PopAndDestroy(4, dbMan);
        }
    }

// ---------------------------------------------------------
// CPosTp127::MultipleDbLandmarkSearchL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp127::MultipleDbLandmarkSearchL(TDes* aMessage)
    {
    TBool lockThreadExists = ThreadExistsL(KLockThreadSearchPattern);
    if (lockThreadExists)
        {
        // Let lock thread launch first
        User::After(KLockThreadStartupTime);
        }

    // Execute thread for a number of seconds
    TTime startTime, stopTime;
    startTime.HomeTime();
    stopTime.HomeTime();
    TTimeIntervalSeconds executionTime;
    User::LeaveIfError(stopTime.SecondsFrom(startTime, executionTime));

    while (executionTime.Int() < KShortExecutionTimeInSeconds)
        {
        CPosLmDatabaseManager* dbMan = CPosLmDatabaseManager::NewL();
        CleanupStack::PushL(dbMan);
        CDesCArray* dbList = dbMan->ListDatabasesLC();
        CPosLmMultiDbSearch* dbSearcher = CPosLmMultiDbSearch::NewL(*dbList);
        CleanupStack::PushL(dbSearcher);
        
        // Search for landmarks containing a specific string
        CPosLmTextCriteria* criteria = CPosLmTextCriteria::NewLC();
        criteria->SetTextL(_L("*as*"));
        CPosLmOperation* operation = dbSearcher->StartLandmarkSearchL(*criteria);
        ExecuteAndDeleteLD(operation);
        
        aMessage->Copy(_L("MultipleDbLandmarkSearchL: "));
        VerifyResultL(lockThreadExists, dbSearcher, aMessage);

        stopTime.HomeTime();
        User::LeaveIfError(stopTime.SecondsFrom(startTime, executionTime));
        
        CleanupStack::PopAndDestroy(4, dbMan);
        }
    }

// ---------------------------------------------------------
// CPosTp127::MultipleDbLandmarkWithoutCategorySearchL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp127::MultipleDbLandmarkWithoutCategorySearchL(TDes* aMessage)
    {
    TBool lockThreadExists = ThreadExistsL(KLockThreadSearchPattern);
    if (lockThreadExists)
        {
        // Let lock thread launch first
        User::After(KLockThreadStartupTime);
        }

    // Execute thread for a number of seconds
    TTime startTime, stopTime;
    startTime.HomeTime();
    stopTime.HomeTime();
    TTimeIntervalSeconds executionTime;
    User::LeaveIfError(stopTime.SecondsFrom(startTime, executionTime));

    while (executionTime.Int() < KShortExecutionTimeInSeconds)
        {
        CPosLmDatabaseManager* dbMan = CPosLmDatabaseManager::NewL();
        CleanupStack::PushL(dbMan);
        CDesCArray* dbList = dbMan->ListDatabasesLC();
        AssertTrueL(dbList->Count() == KNrOfDatabases, _L("MultipleDbLandmarkWithoutCategorySearchL: Wrong number of databases"), aMessage);
        CPosLmMultiDbSearch* dbSearcher = CPosLmMultiDbSearch::NewL(*dbList);
        CleanupStack::PushL(dbSearcher);
        
        // Search for uncategorized landmarks
        CPosLmCategoryCriteria* criteria = CPosLmCategoryCriteria::NewLC();
        CPosLmOperation* operation = dbSearcher->StartLandmarkSearchL(*criteria);
        ExecuteAndDeleteLD(operation);
        
        aMessage->Copy(_L("MultipleDbLandmarkWithoutCategorySearchL: "));
        VerifyResultL(lockThreadExists, dbSearcher, aMessage);

        stopTime.HomeTime();
        User::LeaveIfError(stopTime.SecondsFrom(startTime, executionTime));
        
        CleanupStack::PopAndDestroy(4, dbMan);
        }
    }

// ---------------------------------------------------------
// CPosTp127::LockDatabaseL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp127::LockDatabaseL(TDes* aMessage)
    {
    CPosLmDatabaseManager* dbMan = CPosLmDatabaseManager::NewL();
    CleanupStack::PushL(dbMan);
    CDesCArray* dbList = dbMan->ListDatabasesLC();
    AssertTrueL(dbList->Count() == KNrOfDatabases, _L("MultipleDbLandmarkWithoutCategorySearchL: Wrong number of databases"), aMessage);
    CPosLandmarkDatabase* db = CPosLandmarkDatabase::OpenL((*dbList)[KLockedDbIndex]);
    CleanupStack::PushL(db);
    CPosLmOperation* operation = db->RemoveAllLandmarksL();
    User::After((KShortExecutionTimeInSeconds + 1) * 1000000);
    delete operation;
    
    CleanupStack::PopAndDestroy(3, dbMan);
    }

// ---------------------------------------------------------
// CPosTp127::DbManagementL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp127::DbManagementL(TDes* aMessage)
    {
    // Execute thread for a number of seconds
    TTime startTime, stopTime;
    startTime.HomeTime();
    stopTime.HomeTime();
    TTimeIntervalSeconds executionTime;
    User::LeaveIfError(stopTime.SecondsFrom(startTime, executionTime));
    TInt i(0);
    TBool errorsFound = EFalse;
    
    while (executionTime.Int() < KLongExecutionTimeInSeconds)
        {
        CPosLmDatabaseManager* dbMan = CPosLmDatabaseManager::NewL();
        CleanupStack::PushL(dbMan);
        
        // List dbs
        CDesCArray* dbList = dbMan->ListDatabasesLC();
        
        // Create db
        HPosLmDatabaseInfo* dbInfo = HPosLmDatabaseInfo::NewLC(KTestDbUri);
        TRAPD(err, dbMan->CreateDatabaseL(*dbInfo));
        CleanupStack::PopAndDestroy(dbInfo);
        if (err != KErrAlreadyExists)
            {
            AssertTrueL(dbMan->DatabaseExistsL(KTestDbUri), _L("DbManagementL: Created database does not exist"), aMessage);        
            }
            
        // Try to read default URI
        HBufC* defaultDb = dbMan->DefaultDatabaseUriLC();
        
        // Set default db to the created one
        dbMan->SetDefaultDatabaseUriL(KTestDbUri);
        
        // Remove default db
        TRAP(err, dbMan->DeleteDatabaseL(KTestDbUri));
        AssertTrueL(err == KErrNone || err == KErrInUse, _L("DbManagementL: Wrong error code when removing created database"), aMessage, err);
        if (err == KErrNone)
            {
            AssertTrueL(!dbMan->DatabaseExistsL(KTestDbUri), _L("DbManagementL: Removed database exists"), aMessage);
            }
                
        // Create a copy of a db
        if (dbList->Count() > 0)
            {
            TRAP(err, dbMan->CopyDatabaseL((*dbList)[0], KTestDbUri));
// TBD: How can err == KErrLocked happen? Refer to ESLI-68HKE6
// TBD: Sometimes KErrDied is returned from this function. Why? Write error report?
            AssertTrueL(err == KErrNone || err == KErrAlreadyExists || err == KErrInUse || err == KErrLocked, _L("DbManagementL: Wrong error code when copying database"), aMessage, err);
            if (err == KErrNone || err == KErrAlreadyExists)
                {
                AssertTrueL(dbMan->DatabaseExistsL(KTestDbUri), _L("DbManagementL: Copied database does not exist"), aMessage);
                
                // Remove copy
                TRAP(err, dbMan->DeleteDatabaseL(KTestDbUri));
                AssertTrueL(err == KErrNone || err == KErrInUse, _L("DbManagementL: Wrong error code when removing copied database"), aMessage, err);
                if (err == KErrNone)
                    {
                    AssertTrueL(!dbMan->DatabaseExistsL(KTestDbUri), _L("DbManagementL: Removed database exists"), aMessage);
                    }
                }
            else if (err == KErrLocked) // To be removed when ESLI-68HKE6 is solved
                {
                aMessage->Copy(_L("DbManagementL: CPosLmDatabaseManager::CopyDatabaseL() left with KErrLocked - check ESLI-68HKE6"));
                errorsFound = ETrue;
                }
            }
            
        // Set default db to another uri
        dbMan->SetDefaultDatabaseUriL((*dbList)[i]);
        if (++i == 5)
            {
            i = 0;
            }
            
        // Increase probability that (*dbList)[i] and not KTestDbUri is default db most of the time
        User::After(10000);
        
        CleanupStack::PopAndDestroy(3, dbMan);
        
        stopTime.HomeTime();
        User::LeaveIfError(stopTime.SecondsFrom(startTime, executionTime));
        }
        
    if (errorsFound)
        {
        User::Leave(KErrGeneral);
        }
    }

// ---------------------------------------------------------
// CPosTp127::DbAccessL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp127::DbAccessL(TDes* aMessage)
    {
    // Execute thread for a number of seconds
    TTime startTime, stopTime;
    startTime.HomeTime();
    stopTime.HomeTime();
    TTimeIntervalSeconds executionTime;
    User::LeaveIfError(stopTime.SecondsFrom(startTime, executionTime));
    TBool errorsFound = EFalse;
    
    while (executionTime.Int() < KLongExecutionTimeInSeconds)
        {
        CPosLandmarkDatabase* db = NULL;
        TRAPD(err, db = CPosLandmarkDatabase::OpenL());
// TBD: How can KErrNotFound happen? Refer to ESLI-68HE4P
        if (err == KErrLocked || err == KErrInUse || err == KErrNotFound)
            {
            if (err == KErrNotFound) // To be removed when ESLI-68HE4P is solved
                {
                aMessage->Copy(_L("DbAccessL: CPosLandmarkDatabase::OpenL() left with KErrNotFound - check ESLI-68HE4P"));
                errorsFound = ETrue;
                }
            // Some client is writing(KErrLocked)/deleting(KErrInUse) the db. Try again.
            continue;
            }
        AssertTrueL(err == KErrNone, _L("DbAccessL: Wrong error code when opening db"), aMessage, err);
        CleanupStack::PushL(db);
        
        // We don't want to use the empty test database
        HBufC* dbUri = db->DatabaseUriLC();
// TBD: Both UC and original case need to be checked here due to SDK bug! To be updated. (refer to INC053631 in teamtrack and ESLI-68HK3W in TSW)
        if (dbUri->Find(KTestDbUriUC) != KErrNotFound || dbUri->Find(KTestDbUri) != KErrNotFound)
            {
            if (dbUri->Find(KTestDbUriUC) != KErrNotFound) // To be removed when ESLI-68HK3W is solved
                {
                aMessage->Copy(_L("DbAccessL: UPPER CASE of db URI found - check ESLI-68HK3W"));
                errorsFound = ETrue;
                }            
            CleanupStack::PopAndDestroy(2, db);
            stopTime.HomeTime();
            User::LeaveIfError(stopTime.SecondsFrom(startTime, executionTime));
            continue;
            }
            
        // Fetch db iterator
        CPosLmItemIterator* iterator = NULL;
        TRAP(err, iterator = db->LandmarkIteratorL());
// TBD: KErrPosLmNotInitialized should not be accepted. Refer to ESLI-68JAMD
        if (err == KErrCorrupt || err == KErrPosLmNotInitialized)
            {
            if (err == KErrPosLmNotInitialized) // To be removed when ESLI-68JAMD is solved
                {
                aMessage->Copy(_L("DbAccessL: CPosLandmarkDatabase::LandmarkIteratorL() left with KErrPosLmNotInitialized - check ESLI-68JAMD"));
                errorsFound = ETrue;
                }            
// TBD: Databases need to be re-initialized, because they haved been damaged. Why do they become damaged?
            err = KErrLocked;
            while (err == KErrLocked && db->IsInitializingNeeded())
                {
                TRAP(err, ExecuteAndDeleteLD(db->InitializeL()));
                AssertTrueL(err == KErrNone || err == KErrLocked, _L("DbAccessL: Wrong error code when initializing"), aMessage, err);
                }
            TRAP(err, iterator = db->LandmarkIteratorL());
            }
        TBuf<100> buf;
        _LIT(KFormat, "DbAccessL: Wrong error code when creating iterator for db %S");
        buf.Format(KFormat, dbUri);
        AssertTrueL(err == KErrNone, buf, aMessage, err);
        CleanupStack::PopAndDestroy(dbUri);
        CleanupStack::PushL(iterator);
        
        // Add, read and update a landmark
        CPosLandmark* lm = CPosLandmark::NewLC();
        TPosLmItemId id = KPosLmNullItemId;
        TRAP(err, id = db->AddLandmarkL(*lm));
        CleanupStack::PopAndDestroy(lm);        
        if (err == KErrNone)
            {            
            TRAP(err, CleanupStack::Pop(lm = db->ReadLandmarkLC(id)));
            CleanupStack::PushL(lm);
            AssertTrueL(err == KErrNone, _L("DbAccessL: Wrong error code when reading created landmark"), aMessage, err);
            lm->SetLandmarkNameL(_L("A landmark"));
            TRAP(err, db->UpdateLandmarkL(*lm));
            AssertTrueL(err == KErrNone || err == KErrLocked, _L("DbAccessL: Wrong error code when updating lm"), aMessage, err);
            CleanupStack::PopAndDestroy(lm);
            }
        else
            {
            AssertTrueL(err == KErrLocked, _L("DbAccessL: Wrong error code when adding lm"), aMessage, err);
            }
        
        // Remove possible added landmarks
        TInt nrOfItems = iterator->NumOfItemsL();
        if (nrOfItems > KMaxNrOfLms)
            {
            RIdArray idArray;
            CleanupClosePushL(idArray);
            iterator->GetItemIdsL(idArray, KMaxNrOfLms, nrOfItems - KMaxNrOfLms);
            err = KErrLocked;
            while (err != KErrNone)
                {
                TRAP(err, ExecuteAndDeleteLD(db->RemoveLandmarksL(idArray)));
                AssertTrueL(err == KErrNone || err == KErrLocked, _L("DbAccessL: Wrong error code when removing lm"), aMessage, err);
                }
            CleanupStack::PopAndDestroy(&idArray);
            }

        CleanupStack::PopAndDestroy(2, db);
        
        stopTime.HomeTime();
        User::LeaveIfError(stopTime.SecondsFrom(startTime, executionTime));
        }
        
    if (errorsFound)
        {
        User::Leave(KErrGeneral);
        }
    }

// ---------------------------------------------------------
// CPosTp127::SingleDbSearchL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp127::SingleDbSearchL(TDes* aMessage)
    {
    // Execute thread for a number of seconds
    TTime startTime, stopTime;
    startTime.HomeTime();
    stopTime.HomeTime();
    TTimeIntervalSeconds executionTime;
    User::LeaveIfError(stopTime.SecondsFrom(startTime, executionTime));
    TInt64 seed = startTime.Int64();
    TBool errorsFound = EFalse;
    
    while (executionTime.Int() < KLongExecutionTimeInSeconds)
        {
        CPosLmDatabaseManager* dbMan = CPosLmDatabaseManager::NewL();
        CleanupStack::PushL(dbMan);
        CDesCArray* dbList = dbMan->ListDatabasesLC();
        
        // Open one of the pre-defined databases
        TInt index = (TInt) (Math::FRand(seed) * dbList->Count());
        TPtrC uri = (*dbList)[index];
// TBD: Both UC and original case need to be checked here due to SDK bug! To be updated. (refer to INC053631 in teamtrack and ESLI-68HK3W in TSW)
        if (uri.Find(KTestDbUriUC) != KErrNotFound || uri.Find(KTestDbUri) != KErrNotFound)
            {
            if (uri.Find(KTestDbUriUC) != KErrNotFound) // To be removed when ESLI-68HK3W is solved
                {
                aMessage->Copy(_L("SingleDbSearchL: UPPER CASE of db URI found - check ESLI-68HK3W"));
                errorsFound = ETrue;
                }                            
            // We don't want to search the empty test database
            CleanupStack::PopAndDestroy(2, dbMan);
            continue;
            }
        CPosLandmarkDatabase* db = NULL;
        TRAPD(err, db = CPosLandmarkDatabase::OpenL(uri));
        if (err == KErrLocked)
            {
            // Some client is writing to the db. Try again.
            CleanupStack::PopAndDestroy(2, dbMan);
            continue;
            }
        _LIT(KFormat, "SingleDbSearchL: Wrong error code when opening db %S");
        TBuf<100> buf;        
        buf.Format(KFormat, &uri);
        AssertTrueL(err == KErrNone, buf, aMessage, err);
        CleanupStack::PushL(db);

        CPosLandmarkSearch* dbSearcher = NULL;
        TRAP(err, dbSearcher = CPosLandmarkSearch::NewL(*db));
        AssertTrueL(err == KErrNone, _L("SingleDbSearchL: Wrong error code when calling CPosLandmarkSearch::NewL"), aMessage, err);
        CleanupStack::PushL(dbSearcher);
        
        // Search for landmarks containing a specific string
        CPosLmTextCriteria* criteria = CPosLmTextCriteria::NewLC();
        criteria->SetTextL(_L("*as*"));
        CPosLmOperation* operation = NULL;
        TRAP(err, operation = dbSearcher->StartLandmarkSearchL(*criteria));
        AssertTrueL(err == KErrNone || err == KErrLocked, _L("SingleDbSearchL: Wrong error code when creating search op"), aMessage, err);
        if (operation)
            {
            CleanupStack::PushL(operation);
            TRAP(err, operation->ExecuteL());
            AssertTrueL(err == KErrNone || err == KErrLocked, _L("SingleDbSearchL: Wrong error code when executing search op"), aMessage, err);
            CleanupStack::PopAndDestroy(operation);
            }
        
        CleanupStack::PopAndDestroy(5, dbMan);

        stopTime.HomeTime();
        User::LeaveIfError(stopTime.SecondsFrom(startTime, executionTime));
        }

    if (errorsFound)
        {
        User::Leave(KErrGeneral);
        }
    }

// ---------------------------------------------------------
// CPosTp127::MultipleDbSearchL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp127::MultipleDbSearchL(TDes* aMessage)
    {
    // Execute thread for a number of seconds
    TTime startTime, stopTime;
    startTime.HomeTime();
    stopTime.HomeTime();
    TTimeIntervalSeconds executionTime;
    User::LeaveIfError(stopTime.SecondsFrom(startTime, executionTime));
/*    
    RArray<RIdArray> expectedResult; // contains expected matches when search patern is *e*
    CleanupClosePushL(expectedResult);
    
    RIdArray array1;
    CleanupClosePushL(array1);
    array1.AppendL(4); // vegetariskt
    array1.AppendL(5); // Inte sa bra
    array1.AppendL(8); // Medel
    array1.AppendL(9); // Medel2
    array1.AppendL(12); // GatesB    
    array1.AppendL(13); // Text"'1
    array1.AppendL(14); // GatesF
    array1.AppendL(15); // xStekt
    array1.AppendL(18); // enat?r
    array1.AppendL(19); // enator
    expectedResult.AppendL(array1);
    
    RIdArray array2;
    CleanupClosePushL(array2);
    for (TInt i = 29; i <= 40; i++)
        {
        array2.AppendL(i); // Unique - Text10
        }
    expectedResult.AppendL(array2);
    
    RIdArray array3;
    CleanupClosePushL(array3);
    array3.AppendL(45); // Several
    array3.AppendL(51); // PellesPizza
    array3.AppendL(52); // Kalles Hundgård
    for (TInt i = 54; i <= 60; i++)
        {
        array3.AppendL(i); // TE, Gårda - TE, Oulu
        }
    expectedResult.AppendL(array3);

    RIdArray array4;
    CleanupClosePushL(array4);
    array4.AppendL(61); // TE, Tampere
    array4.AppendL(62); // Nokia, Köpenhamn
    array4.AppendL(65); // Tampere
    array4.AppendL(66); // Helsinki
    array4.AppendL(67); // Bengtfors
    array4.AppendL(70); // Göteborg
    array4.AppendL(71); // PallesPalör
    array4.AppendL(72); // Läkare utan gränser
    array4.AppendL(74); // Läkargruppen
    array4.AppendL(75); // Läkarhuset
    array4.AppendL(76); // Sahlgrenska
    array4.AppendL(77); // östra sjukhuset
    array4.AppendL(80); // GöteborgsTaxi
    expectedResult.AppendL(array4);

    RIdArray array5;
    CleanupClosePushL(array5);
    array5.AppendL(81); // LandvetterTaxi
    array5.AppendL(84); // Länsmuseumet
    array5.AppendL(85); // Sjöfartsmuseumet
    array5.AppendL(86); // KinaMuseumet
    array5.AppendL(88); // Etnogfrafiska
    array5.AppendL(89); // TekniskaMuseumet
    array5.AppendL(91); // Centrum dagiset
    array5.AppendL(93); // Lackarbäcks daghem
    for (TInt i = 95; i <= 100; i++)
        {
        array5.AppendL(i); // TE dagis - krokslätts frisörerna
        }
    expectedResult.AppendL(array5);
*/
    RArray<TInt> nrOfExpectedMatches; // contains the number of matches for each db when search pattern is *e*
    CleanupClosePushL(nrOfExpectedMatches);
    nrOfExpectedMatches.AppendL(10);
    nrOfExpectedMatches.AppendL(12);
    nrOfExpectedMatches.AppendL(10);
    nrOfExpectedMatches.AppendL(13);
    nrOfExpectedMatches.AppendL(14);
    
    while (executionTime.Int() < KLongExecutionTimeInSeconds)
        {
        CDesCArray* dbList = new (ELeave) CDesCArrayFlat(5);
        CleanupStack::PushL(dbList);
        dbList->AppendL(KDb20);
        dbList->AppendL(KDb40);
        dbList->AppendL(KDb60);
        dbList->AppendL(KDb80);
        dbList->AppendL(KDb105);
        CPosLmMultiDbSearch* dbSearcher = CPosLmMultiDbSearch::NewL(*dbList);
        CleanupStack::PushL(dbSearcher);
        
        // Search for landmarks containing a specific string in name
        CPosLmTextCriteria* criteria = CPosLmTextCriteria::NewLC();
        criteria->SetTextL(_L("*e*"));
        criteria->SetAttributesToSearch(CPosLandmark::ELandmarkName);
        CPosLmOperation* operation = NULL;
        TRAPD(err, operation = dbSearcher->StartLandmarkSearchL(*criteria))
        AssertTrueL(err == KErrNone || err == KErrLocked, _L("MultipleDbSearchL: Wrong error code when creating search op"), aMessage, err);
        if (operation)
            {
            CleanupStack::PushL(operation);
            TRAP(err, operation->ExecuteL());
            AssertTrueL(err == KErrNone || err == KErrLocked, _L("MultipleDbSearchL: Wrong error code when executing search op"), aMessage, err);
            CleanupStack::PopAndDestroy(operation);
            }
        
        // Check errors
        RArray<TInt> dbsWithErrors;
        CleanupClosePushL(dbsWithErrors);
        TUint nrOfSearchErrors = dbSearcher->NumOfSearchErrors();
        for (TUint i = 0; i < nrOfSearchErrors; i++)
            {
            CPosLmMultiDbSearch::TSearchError error;
            dbSearcher->GetSearchError(i, error);
            AssertTrueL(error.iErrorCode == KErrLocked, _L("MultipleDbSearchL: Wrong error code when searching"), aMessage, error.iErrorCode);
            dbsWithErrors.AppendL(error.iDatabaseIndex);
            }
            
        // Check number of matches
        for (TInt i = 0; i < dbList->Count(); i++)
            {
            if (dbsWithErrors.Find(i) == KErrNotFound)
                {
                TInt nrOfMatches = dbSearcher->NumOfMatches(i);
                TInt expectedResult = nrOfExpectedMatches[i];
				AssertTrueL(nrOfMatches == expectedResult, _L("MultipleDbSearchL: Wrong number of matches"), aMessage);
                }
            }
        
        CleanupStack::PopAndDestroy(4, dbList);
        
        stopTime.HomeTime();
        User::LeaveIfError(stopTime.SecondsFrom(startTime, executionTime));
        }
        
    CleanupStack::PopAndDestroy(&nrOfExpectedMatches);
    }

// ---------------------------------------------------------
// CPosTp127::CopyLockedDatabaseL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp127::CopyLockedDatabaseL(TDes* aMessage)
    {
    TBool lockThreadExists = ThreadExistsL(KLockThreadSearchPattern);
    if (lockThreadExists)
        {
        // Let lock thread launch first
        User::After(KLockThreadStartupTime);
        }

    CPosLmDatabaseManager* dbMan = CPosLmDatabaseManager::NewL();
    CleanupStack::PushL(dbMan);
    CDesCArray* dbList = dbMan->ListDatabasesLC();
    AssertTrueL(dbList->Count() == KNrOfDatabases, _L("CopyLockedDatabaseL: Wrong number of databases"), aMessage);
    
    // Try to copy database. Should result in KErrInUse
    TRAPD(err, dbMan->CopyDatabaseL((*dbList)[KLockedDbIndex], KTestDbUri));
// TBD: KErrLocked should not be accepted. Refer to ESLI-68HKE6
    AssertTrueL(err != KErrLocked, _L("CopyLockedDatabaseL: Wrong error code when copying database - check ESLI-68HKE6"), aMessage, err);
    AssertTrueL(err == KErrInUse , _L("CopyLockedDatabaseL: Wrong error code when copying database"), aMessage, err);
    
    CleanupStack::PopAndDestroy(2, dbMan);
    }
    
// ---------------------------------------------------------
// CPosTp127::CopyDatabaseL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp127::CopyDatabaseL(TDes* aMessage)
    {
    // Execute thread for a number of seconds
    TTime startTime, stopTime;
    startTime.HomeTime();
    stopTime.HomeTime();
    TTimeIntervalSeconds executionTime;
    User::LeaveIfError(stopTime.SecondsFrom(startTime, executionTime));
    TInt64 seed = startTime.Int64();
    TBool errorsFound = EFalse;

    CPosLmDatabaseManager* dbMan = CPosLmDatabaseManager::NewL();
    CleanupStack::PushL(dbMan);
    while (executionTime.Int() < KShortExecutionTimeInSeconds)
        {
        CDesCArray* dbList = dbMan->ListDatabasesLC();
        
        // Create a source and a target URI
        _LIT(KCopy, "_copy");
        TInt copyLength = KCopy().Length();
        TInt index = (TInt) (Math::FRand(seed) * dbList->Count());
        TPtrC sourceUri = (*dbList)[index];
        HBufC* targetUriBuffer = HBufC::NewLC(sourceUri.Length() + copyLength);
        TPtr targetUri = targetUriBuffer->Des();
        targetUri.Copy(sourceUri);
        targetUri.Insert(targetUri.Length() - 4, KCopy); // 4 == length of ".ldb"

        // Try to copy database. Should result in KErrInUse if locked, KErrNone otherwise
        TRAPD(err, dbMan->CopyDatabaseL(sourceUri, targetUri));
        AssertTrueL(err == KErrNone || err == KErrAlreadyExists, _L("CopyDatabaseL: Wrong error code when copying database"), aMessage, err);
        
        // Set default database to the created one
        TRAP(err, dbMan->SetDefaultDatabaseUriL(targetUri));
        AssertTrueL(err == KErrNone || err == KErrNotFound, _L("CopyDatabaseL: Wrong error code when setting default db to target"), aMessage, err);
        
        // Remove the created database
        err = KErrLocked;
        while (err != KErrNone && err != KErrNotFound)
            {
            TRAP(err, dbMan->DeleteDatabaseL(targetUri));
            }
        
        // Set default database to the source
        TRAP(err, dbMan->SetDefaultDatabaseUriL(sourceUri));
        AssertTrueL(err == KErrNone, _L("CopyDatabaseL: Wrong error code when setting default db to source"), aMessage, err);
        
        CleanupStack::PopAndDestroy(2, dbList);
        
        stopTime.HomeTime();
        User::LeaveIfError(stopTime.SecondsFrom(startTime, executionTime));
        }
        
    if (errorsFound)
        {
        User::Leave(KErrGeneral);
        }
    CleanupStack::PopAndDestroy(dbMan);
    }
        
//  End of File
