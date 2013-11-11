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
#include "FT_CPosTp79.h"
#include "FT_CPosTp78.h"
#include <Lbs.h>
#include <EPos_CPosLandmarkDatabase.h>
#include <EPos_CPosLmCategoryManager.h>
#include <EPos_CPosLmTextCriteria.h>
#include <EPos_CPosLandmarkCategory.h>
#include <EPos_CPosLmCatNameCriteria.h>

// CONSTANTS
const TInt KExectionTimeInMinutes = 60 * 10; // 10 hours

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

LOCAL_C void AddCategoriesL(
    TInt aNrOfLmsToAdd,
    const TDesC& aName,
    CPosLmCategoryManager* aCatMan, 
    RArray<TPosLmItemId>& aIdArray)
    {
    for (TInt i = 0; i < aNrOfLmsToAdd; i++)
        {
        CPosLandmarkCategory* category = CPosLandmarkCategory::NewLC();
        HBufC* name = HBufC::NewLC(aName.Length() + 5);
        TPtr namePtr = name->Des();
        namePtr.Format(aName, i);
        category->SetCategoryNameL(namePtr);
        category->SetIconL(_L("Icon"), 23, KPosLmIconMaskNotUsed);
        TPosLmItemId id;
        TRAPD(err, id = aCatMan->AddCategoryL(*category));
        if (err != KErrNone && err != KErrAlreadyExists)
            {
            User::Leave(err);
            }
        else
            {
            if (err == KErrAlreadyExists)
                {
                id = aCatMan->GetCategoryL(namePtr);
                }
            User::LeaveIfError(aIdArray.Append(id));
            }
        CleanupStack::PopAndDestroy(2, category);
        }
    }

LOCAL_C void VerifyErrorL(const TDesC& aFormat, TInt aErr, TDes* aMessage)
    {
    if (aErr != KErrNone && aErr != KErrLocked)
        {
        aMessage->Format(aFormat, aErr);

        // Append time
        _LIT(KTimeFormat, " %F%Y-%M-%D at %H.%T:%S");
        TTime now;
        now.HomeTime();
        TBuf<50> time;
        now.FormatL(time, KTimeFormat);
        aMessage->Append(time);

        User::Leave(aErr);
        }
    }

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CPosTp79::GetName
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp79::GetName(TDes& aName) const
    {
    _LIT(KTestName, "TP79 - Category and search duration tests");
    aName = KTestName;
    }

// ---------------------------------------------------------
// CPosTp79::InitTestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp79::InitTestL()
    {
    MakeSurePanicDebugFileExistsL();
    InitDbL();
    }

// ---------------------------------------------------------
// CPosTp79::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp79::CloseTest()
    {
    }

// ---------------------------------------------------------
// CPosTp79::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp79::StartL()
    {
// Test Open database duration test
    ResumeThreadsAndVerifyExitL();
    }

// ---------------------------------------------------------
// CPosTp79::InitDbL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp79::InitDbL()
    {
    RemoveDefaultDbL();
    CPosLandmarkDatabase* database = CPosLandmarkDatabase::OpenL();
    CleanupStack::PushL(database);
    
    if (database->IsInitializingNeeded())
        {
        ExecuteAndDeleteLD(database->InitializeL());
        }

    CleanupStack::PopAndDestroy(database);

    // Wait for LM server to die
    User::After(2000000);
    }

// ---------------------------------------------------------
// CPosTp79::ResumeThreadsAndVerifyExitL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp79::ResumeThreadsAndVerifyExitL()
    {
    _LIT(KCreateThreadErr, "Creating thread failed with %d");
    TInt nrOfThreads = 0;
    RThread searchThread, readAndUpdateThread, addRemoveCatsThread;
    TThreadParam params1, params2, params3;
    TBuf<200> threadMsg1, threadMsg2, threadMsg3;

    // Create search thread
    _LIT(KSearch, "TP79 - Search duration thread");
    params1.iMessage = &threadMsg1;
    params1.iTestFunction = RunSearchTestL;
    TInt err = searchThread.Create(KSearch, ThreadFunction, 
        KDefaultStackSize, KMinHeapSize, KDefaultHeapSize, &params1);
    AssertTrueSecL(err == KErrNone, KCreateThreadErr, err);
    CleanupClosePushL(searchThread);
    nrOfThreads++;

    // Create read and update thread
    _LIT(KReadAndUpdateThread, "TP79 - Read and update categories duration thread");
    params2.iMessage = &threadMsg2;
    params2.iTestFunction = RunReadAndUpdateTestL;
    err = readAndUpdateThread.Create(KReadAndUpdateThread, ThreadFunction, 
        KDefaultStackSize, KMinHeapSize, KDefaultHeapSize, &params2);
    AssertTrueSecL(err == KErrNone, KCreateThreadErr, err);
    CleanupClosePushL(readAndUpdateThread);
    nrOfThreads++;

    // Create add remove categories thread
    _LIT(KAddRemoveCategories, "TP79 - Add remove categories duration thread");
    params3.iMessage = &threadMsg3;
    params3.iTestFunction = RunAddRemoveCatsTestL;
    err = addRemoveCatsThread.Create(KAddRemoveCategories, ThreadFunction, 
        KDefaultStackSize, KMinHeapSize, KDefaultHeapSize, &params3);
    AssertTrueSecL(err == KErrNone, KCreateThreadErr, err);
    CleanupClosePushL(addRemoveCatsThread);
    nrOfThreads++;

    // Start threads.
    RThread* threads[] = {&searchThread, &readAndUpdateThread, &addRemoveCatsThread};
    RPointerArray<CThreadMonitor> threadMonitors;
    for (TInt j = 0; j < nrOfThreads; j++)
        {
        threadMonitors.Append(CThreadMonitor::NewL(threads[j]));
        }

    // Log time stamp
    _LIT(KTimeFormat, "Threads started on %F%Y-%M-%D at %H.%T:%S");
    TTime now;
    now.HomeTime();    
    now.FormatL(iBuf, KTimeFormat);
    iLog->Put(iBuf);

    // Wait until any of the threads exits.
    TDes* messages[] = {&threadMsg1, &threadMsg2, &threadMsg3};
    TInt errors = KErrNone;
    TBool allThreadsTerminated = EFalse;
    while (!allThreadsTerminated && errors == KErrNone)
        {
        CActiveScheduler::Start();

        // Assume all threads are terminated
        allThreadsTerminated = ETrue;
        for (TInt i = 0; i < nrOfThreads && errors == KErrNone; i++)
            {
            if (threadMonitors[i]->IsActive())
                {
                // Thread is still alive
                allThreadsTerminated = EFalse;
                }
            else
                {
                // Thread is terminated, check exit codes
                if (*messages[i] != KNullDesC)
                    {
                    iLog->Put(*(messages[i]));
                    }
                if (threads[i]->ExitType() != EExitKill)
                    {
                    iBuf.Format(_L("Thread %i was not killed!"), i);
                    iLog->PutError(iBuf);
                    TExitCategoryName category = threads[i]->ExitCategory();
                    iBuf.Format(_L("Exit Category = %S %d"), &category, threads[i]->ExitReason());
                    iLog->Put(iBuf);
                    errors = KErrGeneral;
                    }
                else
                    {
                    TInt exitReason = threads[i]->ExitReason();
                    if (exitReason != 0)
                        {
                        iBuf.Format(_L("Thread's exit reason wasn't 0 but %d."), exitReason);
                        iLog->PutError(iBuf);
                        errors = exitReason;
                        }
                    }
                }
            }
        }

    // Terminate all remaining threads
    threadMonitors.ResetAndDestroy();

    // Log time stamp
    _LIT(KTimeFormat2, "Threads terminated on %F%Y-%M-%D at %H.%T:%S");
    now.HomeTime();    
    now.FormatL(iBuf, KTimeFormat2);
    iLog->Put(iBuf);


    User::LeaveIfError(errors);

    CleanupStack::PopAndDestroy(nrOfThreads, &searchThread);
    }

// ---------------------------------------------------------
// CPosTp79::RunSearchTestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp79::RunSearchTestL(TDes* aMessage)
    {
    _LIT(KName, "SearchThread %d");

    // Execute thread for a number of hours
    TTime startTime, stopTime;
    startTime.HomeTime();
    stopTime.HomeTime();
    TTimeIntervalMinutes executionTime;
    User::LeaveIfError(stopTime.MinutesFrom(startTime, executionTime));

    CPosLandmarkDatabase* db = CPosLandmarkDatabase::OpenL();
    CleanupStack::PushL(db);
    CPosLmCategoryManager* catMan = CPosLmCategoryManager::NewL(*db);
    CleanupStack::PushL(catMan);

    while (executionTime.Int() < KExectionTimeInMinutes)
        {
        CPosLandmarkSearch* searcher = NULL;
        TRAPD(err, searcher = CPosLandmarkSearch::NewL(*db));
        CleanupStack::PushL(searcher);
        VerifyErrorL(_L("<-- SearchThread: CPosLandmarkSearch::NewL() left with %d -->"), err, aMessage);

        RArray<TPosLmItemId> dbItems;
        CleanupClosePushL(dbItems);

        const TInt KNrOfCats = 3;
        TRAP(err, AddCategoriesL(KNrOfCats, KName, catMan, dbItems));
        VerifyErrorL(_L("<-- SearchThread: AddCategoriesL() left with %d -->"), err, aMessage);
        if (err == KErrNone)
            {
            // StartLandmarkSearchL
            CPosLmTextCriteria* textCriteria = NULL;
            TRAP(err, CleanupStack::Pop(textCriteria = CPosLmTextCriteria::NewLC()));
            CleanupStack::PushL(textCriteria);
            VerifyErrorL(_L("<-- SearchThread: CPosLmTextCriteria::NewLC() left with %d -->"), err, aMessage);
            TRAP(err, textCriteria->SetTextL(KName().Left(5)));
            VerifyErrorL(_L("<-- SearchThread: SetTextL() left with %d -->"), err, aMessage);
            textCriteria->SetAttributesToSearch(CPosLandmark::EDescription & CPosLandmark::ELandmarkName);
            TPosLmSortPref sortPref(CPosLandmark::ELandmarkName, TPosLmSortPref::EDescending);

            CPosLmOperation* operation = NULL;
            TRAPD(err, operation = searcher->StartLandmarkSearchL(*textCriteria, sortPref));
            CleanupStack::PushL(operation);
            VerifyErrorL(_L("<-- SearchThread: StartLandmarkSearchL() left with %d -->"), err, aMessage);
            if (operation)
                {
                TRAP(err, operation->ExecuteL());
                VerifyErrorL(_L("<-- SearchThread: ExecuteLD(StartLandmarkSearchL()) left with %d -->"), err, aMessage);
                }
            CleanupStack::PopAndDestroy(operation);

            // StartCategorySearchL
            CPosLmCatNameCriteria* nameCriteria = NULL;
            TRAP(err, CleanupStack::Pop(nameCriteria = CPosLmCatNameCriteria::NewLC()));
            CleanupStack::PushL(nameCriteria);
            VerifyErrorL(_L("<-- SearchThread: CPosLmCatNameCriteria::NewLC() left with %d -->"), err, aMessage);
            TRAP(err, nameCriteria->SetSearchPatternL(KName().Left(5)));
            VerifyErrorL(_L("<-- SearchThread: SetSearchPatternL() left with %d -->"), err, aMessage);
            operation = NULL;
            TRAP(err, operation = searcher->StartCategorySearchL(*nameCriteria, CPosLmCategoryManager::ECategorySortOrderNameAscending));
            CleanupStack::PushL(operation);
            VerifyErrorL(_L("<-- SearchThread: StartCategorySearchL() left with %d -->"), err, aMessage);
            if (operation)
                {
                TRAP(err, operation->ExecuteL());
                VerifyErrorL(_L("<-- SearchThread: ExecuteLD(StartCategorySearchL()) left with %d -->"), err, aMessage);
                }
            CleanupStack::PopAndDestroy(3, textCriteria);
            }

        // Let other threads get access to the database
        User::After(300000);

        stopTime.HomeTime();
        User::LeaveIfError(stopTime.MinutesFrom(startTime, executionTime));

        CleanupStack::PopAndDestroy(2, searcher);
        }

    CleanupStack::PopAndDestroy(2, db);
    }

// ---------------------------------------------------------
// CPosTp79::RunAddRemoveCatsTestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp79::RunAddRemoveCatsTestL(TDes* aMessage)
    {
    // Execute thread for a number of hours
    TTime startTime, stopTime;
    startTime.HomeTime();
    stopTime.HomeTime();
    TTimeIntervalMinutes executionTime;
    User::LeaveIfError(stopTime.MinutesFrom(startTime, executionTime));

    // Open db
    CPosLandmarkDatabase* db = CPosLandmarkDatabase::OpenL();
    CleanupStack::PushL(db);

    while (executionTime.Int() < KExectionTimeInMinutes)
        {
        CPosLmOperation* operation = NULL;
        CPosLmCategoryManager* catMan = NULL;
        TRAPD(err, catMan = CPosLmCategoryManager::NewL(*db));
        CleanupStack::PushL(catMan);
        VerifyErrorL(_L("<-- AddRemoveCatsThread: CPosLmCategoryManager::NewL() left with %d -->"), err, aMessage);

        RArray<TPosLmItemId> dbItems;
        CleanupClosePushL(dbItems);

        // RemoveCategoriesL / remove all categories
        CPosLmItemIterator* iterator = NULL;
        TRAP(err, iterator = catMan->CategoryIteratorL());
        CleanupStack::PushL(iterator);
        VerifyErrorL(_L("<-- AddRemoveCatsThread: CategoryIteratorL() left with %d -->"), err, aMessage);
        if (iterator)
            {
            TRAP(err, iterator->GetItemIdsL(dbItems, 0, iterator->NumOfItemsL()));
            VerifyErrorL(_L("<-- AddRemoveCatsThread: GetItemIdsL() left with %d -->"), err, aMessage);

            if (err == KErrNone)
                {
                operation = NULL;
                TRAP(err, operation = catMan->RemoveCategoriesL(dbItems));
                CleanupStack::PushL(operation);
                VerifyErrorL(_L("<-- AddRemoveCatsThread: RemoveCategoriesL() left with %d -->"), err, aMessage);
                if (operation)
                    {
                    TRAP(err, operation->ExecuteL());
                    VerifyErrorL(_L("<-- AddRemoveCatsThread: ExecuteLD(RemoveCategoriesL()) left with %d -->"), err, aMessage);
                    }
                CleanupStack::PopAndDestroy(operation);
                }
            dbItems.Reset();
            }
        CleanupStack::PopAndDestroy(iterator);

        // ResetGlobalCategoriesL
        operation = NULL;
        TRAP(err, operation = catMan->ResetGlobalCategoriesL());
        VerifyErrorL(_L("<-- ReadAndUpdateThread: ResetGlobalCategoriesL() left with %d -->"), err, aMessage);
        if (operation)
            {
            CleanupStack::PushL(operation);
            TRAP(err, operation->ExecuteL());
            VerifyErrorL(_L("<-- ReadAndUpdateThread: ExecuteLD(ResetGlobalCategoriesL()) left with %d -->"), err, aMessage);
            CleanupStack::PopAndDestroy(operation);
            }

        const TInt KNrOfCats = 3;
        TRAP(err, AddCategoriesL(KNrOfCats, _L("AddRemoveCatsThread %d"), catMan, dbItems));
        VerifyErrorL(_L("<-- AddRemoveCatsThread: AddCategoriesL() left with %d -->"), err, aMessage);
        if (err == KErrNone)
            {
            // AddCategoryToLandmarksL
            operation = NULL;
            TRAP(err, operation = catMan->AddCategoryToLandmarksL(dbItems[0], dbItems));
            CleanupStack::PushL(operation);
            VerifyErrorL(_L("<-- AddRemoveCatsThread: AddCategoryToLandmarksL() left with %d -->"), err, aMessage);
            if (operation)
                {
                TRAP(err, operation->ExecuteL());
                VerifyErrorL(_L("<-- AddRemoveCatsThread: ExecuteLD(AddCategoryToLandmarksL()) left with %d -->"), err, aMessage);
                }
            CleanupStack::PopAndDestroy(operation);

            // RemoveCategoryFromLandmarksL
            operation = NULL;
            TRAP(err, operation = catMan->RemoveCategoryFromLandmarksL(dbItems[0], dbItems));
            CleanupStack::PushL(operation);
            VerifyErrorL(_L("<-- AddRemoveCatsThread: RemoveCategoryFromLandmarksL() left with %d -->"), err, aMessage);
            if (operation)
                {
                TRAP(err, operation->ExecuteL());
                VerifyErrorL(_L("<-- AddRemoveCatsThread: ExecuteLD(RemoveCategoryFromLandmarksL()) left with %d -->"), err, aMessage);
                }
            CleanupStack::PopAndDestroy(operation);

            // RemoveCategoryL
            operation = NULL;
            TRAP(err, operation = catMan->RemoveCategoryL(dbItems[0]));
            CleanupStack::PushL(operation);
            VerifyErrorL(_L("<-- AddRemoveCatsThread: RemoveCategoryL() left with %d -->"), err, aMessage);
            if (operation)
                {
                TRAP(err, operation->ExecuteL());
                VerifyErrorL(_L("<-- AddRemoveCatsThread: ExecuteLD(RemoveCategoryL()) left with %d -->"), err, aMessage);
                }
            CleanupStack::PopAndDestroy(operation);
            dbItems.Remove(0);
            }

        // Let other threads get access to the database
        User::After(300000);

        stopTime.HomeTime();
        User::LeaveIfError(stopTime.MinutesFrom(startTime, executionTime));

        CleanupStack::PopAndDestroy(2, catMan);
        }

    CleanupStack::PopAndDestroy(db);
    }

// ---------------------------------------------------------
// CPosTp79::RunReadAndUpdateTestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp79::RunReadAndUpdateTestL(TDes* aMessage)
    {
    // Execute thread for a number of hours
    TTime startTime, stopTime;
    startTime.HomeTime();
    stopTime.HomeTime();
    TTimeIntervalMinutes executionTime;
    User::LeaveIfError(stopTime.MinutesFrom(startTime, executionTime));

    // Open db
    CPosLandmarkDatabase* db = CPosLandmarkDatabase::OpenL();
    CleanupStack::PushL(db);
    CPosLmCategoryManager* catMan = CPosLmCategoryManager::NewL(*db);
    CleanupStack::PushL(catMan);

    while (executionTime.Int() < KExectionTimeInMinutes)
        {
        RArray<TPosLmItemId> dbItems;
        CleanupClosePushL(dbItems);

        const TInt KNrOfCats = 3;
        TRAPD(err, AddCategoriesL(KNrOfCats, _L("ReadAndUpdateThread %d"), catMan, dbItems));
        VerifyErrorL(_L("<-- ReadAndUpdateThread: AddCategoriesL() left with %d -->"), err, aMessage);
        if (err == KErrNone)
            {
            // Read and update
            for (TInt i = 0; i < KNrOfCats; i++)
                {
                CPosLandmarkCategory* category = NULL;
                TRAP(err, CleanupStack::Pop(category = catMan->ReadCategoryLC(dbItems[i])));
                CleanupStack::PushL(category);
                VerifyErrorL(_L("<-- ReadAndUpdateThread: ReadCategoryLC() left with %d -->"), err, aMessage);
                if (category)
                    {
                    TPtrC namePtr;
                    err = category->GetCategoryName(namePtr);
                    VerifyErrorL(_L("<-- ReadAndUpdateThread: GetCategoryName() returned %d -->"), err, aMessage);
                    if (err == KErrNone)
                        {
                        TBuf<KPosLmMaxCategoryNameLength> name(namePtr);
                        name.Insert(0, name.Left(1));
                        TRAP(err, category->SetCategoryNameL(name));
                        VerifyErrorL(_L("<-- ReadAndUpdateThread: SetCategoryNameL() left with %d -->"), err, aMessage);
                        TRAP(err, catMan->UpdateCategoryL(*category));
                        if (err != KErrAlreadyExists)
                            {
                            VerifyErrorL(_L("<-- ReadAndUpdateThread: UpdateCategoryL() left with %d -->"), err, aMessage);
                            }
                        }
                    }
                CleanupStack::PopAndDestroy(category);
                }

            // GetCategoryL
            if (err != KErrInUse)
                {
                TRAP(err, catMan->GetCategoryL(_L("RReadAndUpdateThread 3")));
                VerifyErrorL(_L("<-- ReadAndUpdateThread: GetCategoryL() left with %d -->"), err, aMessage);
                }
            }
        CleanupStack::PopAndDestroy(&dbItems);

        // CategoryIteratorL
        CPosLmItemIterator* iterator = NULL;
        TRAP(err, iterator = catMan->CategoryIteratorL(CPosLmCategoryManager::ECategorySortOrderNameAscending));
        delete iterator;
        VerifyErrorL(_L("<-- ReadAndUpdateThread: CategoryIteratorL() left with %d -->"), err, aMessage);

        // ReferencedCategoryIteratorL
        iterator = NULL;
        TRAP(err, iterator = catMan->ReferencedCategoryIteratorL(CPosLmCategoryManager::ECategorySortOrderNameDescending));
        delete iterator;
        VerifyErrorL(_L("<-- ReadAndUpdateThread: ReferencedCategoryIteratorL() left with %d -->"), err, aMessage);

        // GetGlobalCategoryL
        TRAP(err, catMan->GetGlobalCategoryL(3000));
        VerifyErrorL(_L("<-- ReadAndUpdateThread: GetGlobalCategoryL() left with %d -->"), err, aMessage);

        // GlobalCategoryNameL
        HBufC* name = NULL;
        TRAP(err, name = catMan->GlobalCategoryNameL(3000));
        delete name;
        VerifyErrorL(_L("<-- ReadAndUpdateThread: GlobalCategoryNameL() left with %d -->"), err, aMessage);

        // Let other threads get access to the database
        User::After(110000);

        stopTime.HomeTime();
        User::LeaveIfError(stopTime.MinutesFrom(startTime, executionTime));
        }

    CleanupStack::PopAndDestroy(2, db);
    }

//  End of File
