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
#include "FT_CPosTp78.h"
#include <Lbs.h>
#include <EPos_CPosLandmarkDatabase.h>
#include <EPos_CPosLandmarkParser.h>
#include <EPos_CPosLandmarkEncoder.h>
#include <EPos_CPosLandmark.h>

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

LOCAL_C void AddLandmarksL(
    TInt aNrOfLmsToAdd,
    const TDesC& aName,
    CPosLandmarkDatabase* aDb, 
    RArray<TPosLmItemId>& aIdArray)
    {
    for (TInt i = 0; i < aNrOfLmsToAdd; i++)
        {
        CPosLandmark* landmark = CPosLandmark::NewLC();
        landmark->SetLandmarkNameL(aName);
        landmark->SetPositionL(TLocality(TCoordinate(64, 85, 20), 25, 53));
        landmark->SetCoverageRadius(18);
        landmark->AddCategoryL(1);
        landmark->SetIconL(_L("Icon"), 23, KPosLmIconMaskNotUsed);
        landmark->SetLandmarkDescriptionL(_L("Desc"));
        User::LeaveIfError(aIdArray.Append(aDb->AddLandmarkL(*landmark)));
        CleanupStack::PopAndDestroy(landmark);
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
// CPosTp78::GetName
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp78::GetName(TDes& aName) const
    {
    _LIT(KTestName, "TP78 - CPosLandmarkDatabase duration tests");
    aName = KTestName;
    }

// ---------------------------------------------------------
// CPosTp78::InitTestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp78::InitTestL()
    {
    MakeSurePanicDebugFileExistsL();
    InitDbL();
    }

// ---------------------------------------------------------
// CPosTp78::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp78::CloseTest()
    {
    }

// ---------------------------------------------------------
// CPosTp78::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp78::StartL()
    {
// Test Open database duration test
    ResumeThreadsAndVerifyExitL();
    }

// ---------------------------------------------------------
// CPosTp78::InitDbL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp78::InitDbL()
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
// CPosTp78::ResumeThreadsAndVerifyExitL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp78::ResumeThreadsAndVerifyExitL()
    {
    _LIT(KCreateThreadErr, "Creating thread failed with %d");
    TInt nrOfThreads = 0;
    RThread openDbThread, addRemoveLmThread, impExpLmThread, dbEventThread, readAndUpdateThread;
    TThreadParam params1, params2, params3, params4, params5;
    TBuf<200> threadMsg1, threadMsg2, threadMsg3, threadMsg4, threadMsg5;

    // Create open database thread
    _LIT(KOpenDatabase, "TP78 - Open database duration thread");
    params1.iMessage = &threadMsg1;
    params1.iTestFunction = RunOpenDbTestL;
    TInt err = openDbThread.Create(KOpenDatabase, ThreadFunction, 
        KDefaultStackSize, KMinHeapSize, KDefaultHeapSize, &params1);
    AssertTrueSecL(err == KErrNone, KCreateThreadErr, err);
    CleanupClosePushL(openDbThread);
    nrOfThreads++;

    // Create add remove LMs thread
    _LIT(KAddRemoveLandmarks, "TP78 - Add remove landmarks duration thread");
    params2.iMessage = &threadMsg2;
    params2.iTestFunction = RunAddRemoveLmTestL;
    err = addRemoveLmThread.Create(KAddRemoveLandmarks, ThreadFunction, 
        KDefaultStackSize, KMinHeapSize, 5 * KDefaultHeapSize, &params2);
    AssertTrueSecL(err == KErrNone, KCreateThreadErr, err);
    CleanupClosePushL(addRemoveLmThread);
    nrOfThreads++;

    // Create import/export LMs thread
    _LIT(KImpExpLandmarks, "TP78 - Import/Export landmarks duration thread");
    params3.iMessage = &threadMsg3;
    params3.iTestFunction = RunImportExportLmTestL;
    err = impExpLmThread.Create(KImpExpLandmarks, ThreadFunction, 
        KDefaultStackSize, KMinHeapSize, 5 * KDefaultHeapSize, &params3);
    AssertTrueSecL(err == KErrNone, KCreateThreadErr, err);
    CleanupClosePushL(impExpLmThread);
    nrOfThreads++;

    // Create db event thread
    _LIT(KDbEventThread, "TP78 - Database events duration thread");
    params4.iMessage = &threadMsg4;
    params4.iTestFunction = RunDbEventTestL;
    err = dbEventThread.Create(KDbEventThread, ThreadFunction, 
        KDefaultStackSize, KMinHeapSize, KDefaultHeapSize, &params4);
    AssertTrueSecL(err == KErrNone, KCreateThreadErr, err);
    CleanupClosePushL(dbEventThread);
    nrOfThreads++;

    // Create read and update thread
    _LIT(KReadAndUpdateThread, "TP78 - Read and update landmarks duration thread");
    params5.iMessage = &threadMsg5;
    params5.iTestFunction = RunReadAndUpdateTestL;
    err = readAndUpdateThread.Create(KReadAndUpdateThread, ThreadFunction, 
        KDefaultStackSize, KMinHeapSize, 5 * KDefaultHeapSize, &params5);
    AssertTrueSecL(err == KErrNone, KCreateThreadErr, err);
    CleanupClosePushL(readAndUpdateThread);
    nrOfThreads++;

    // Start threads.
    RThread* threads[] = {&openDbThread, &addRemoveLmThread, &impExpLmThread, &dbEventThread, &readAndUpdateThread};
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
    TDes* messages[] = {&threadMsg1, &threadMsg2, &threadMsg3, &threadMsg4, &threadMsg5};
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

    CleanupStack::PopAndDestroy(nrOfThreads, &openDbThread);
    }

// ---------------------------------------------------------
// CPosTp78::RunOpenDbTestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp78::RunOpenDbTestL(TDes* aMessage)
    {
    // Execute thread for a number of hours
    TTime startTime, stopTime;
    startTime.HomeTime();
    stopTime.HomeTime();
    TTimeIntervalMinutes executionTime;
    User::LeaveIfError(stopTime.MinutesFrom(startTime, executionTime));

    while (executionTime.Int() < KExectionTimeInMinutes)
        {
        CPosLandmarkDatabase* db = NULL;
        TRAPD(err, db = CPosLandmarkDatabase::OpenL());
        CleanupStack::PushL(db);
        VerifyErrorL(_L("<-- OpenDbThread: CPosLandmarkDatabase::OpenL() left with %d -->"), err, aMessage);
        if (err == KErrNone)
            {
            // Try to initialize the db
            db->IsInitializingNeeded(); // don't care about the result. Init anyway
            CPosLmOperation* operation = NULL;
            TRAP(err, operation = db->InitializeL());
            CleanupStack::PushL(operation);
            VerifyErrorL(_L("<-- OpenDbThread: InitializeL() left with %d -->"), err, aMessage);
            if (err == KErrNone)
                {
                TRAP(err, operation->ExecuteL());
                VerifyErrorL(_L("<-- OpenDbThread: ExecuteAndDeleteLD(InitializeL()) left with %d -->"), err, aMessage);
                }
            CleanupStack::PopAndDestroy(operation);
            }
        CleanupStack::PopAndDestroy(db);

        // Let other threads get access to the database
        User::After(10000);

        stopTime.HomeTime();
        User::LeaveIfError(stopTime.MinutesFrom(startTime, executionTime));
        }
    }

// ---------------------------------------------------------
// CPosTp78::RunAddRemoveLmTestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp78::RunAddRemoveLmTestL(TDes* aMessage)
    {
    // Execute thread for a number of hours
    TTime startTime, stopTime;
    startTime.HomeTime();
    stopTime.HomeTime();
    TTimeIntervalMinutes executionTime;
    User::LeaveIfError(stopTime.MinutesFrom(startTime, executionTime));

    CPosLandmarkDatabase* db = CPosLandmarkDatabase::OpenL();
    CleanupStack::PushL(db);
    while (executionTime.Int() < KExectionTimeInMinutes)
        {
        RArray<TPosLmItemId> dbItems;
        CleanupClosePushL(dbItems);

        // Remove all landmarks
        CPosLmOperation* operation = NULL;
        TRAPD(err, operation = db->RemoveAllLandmarksL());
        VerifyErrorL(_L("<-- AddRemoveThread: RemoveAllLandmarksL() left with %d -->"), err, aMessage);
        if (err == KErrNone)
            {
            TRAP(err, ExecuteAndDeleteLD(operation));
            VerifyErrorL(_L("<-- AddRemoveThread: ExecuteAndDeleteLD(RemoveAllLandmarksL()) left with %d -->"), err, aMessage);
            }

        // Add some landmarks
        const TInt KNrOfLms = 10;
        TRAP(err, AddLandmarksL(KNrOfLms, _L("AddRemoveThread"), db, dbItems));
        VerifyErrorL(_L("<-- AddRemoveThread: AddLandmarksL() left with %d -->"), err, aMessage);
        if (err == KErrNone)
            {
            // Remove some items one by one
            for (TInt i = 0; i < KNrOfLms / 2; i++)
                {
                dbItems.Remove(0);
                TRAP(err, db->RemoveLandmarkL(dbItems[0]));
                VerifyErrorL(_L("<-- AddRemoveThread: RemoveLandmarkL(TPosLmItemId) left with %d -->"), err, aMessage);
                }

            // Remove 5 items in one step
            operation = NULL;
            TRAPD(err, operation = db->RemoveLandmarksL(dbItems));
            VerifyErrorL(_L("<-- AddRemoveThread: RemoveLandmarksL(RArray) left with %d -->"), err, aMessage);
            if (err == KErrNone)
                {
                TRAP(err, ExecuteAndDeleteLD(operation));
                VerifyErrorL(_L("<-- AddRemoveThread: ExecuteAndDeleteLD(RemoveLandmarksL(dbItems)) left with %d -->"), err, aMessage);
                }
            dbItems.Reset();

            CPosLandmarkDatabase::TSize dbSize; dbSize.iUsage = 0;
            TRAP(err, dbSize = db->SizeL());
            VerifyErrorL(_L("<-- AddRemoveThread: SizeL() left with %d -->"), err, aMessage);
            if (dbSize.iUsage < 0.9)
                {
                operation = NULL;
                TRAP(err, operation = db->CompactL());
                VerifyErrorL(_L("<-- AddRemoveThread: CompactL() left with %d -->"), err, aMessage);
                if (err == KErrNone)
                    {
                    CleanupStack::PushL(operation);
                    TRAP(err, operation->ExecuteL());
                    VerifyErrorL(_L("<-- AddRemoveThread: ExecuteAndDeleteLD(CompactL()) left with %d -->"), err, aMessage);
                    CleanupStack::PopAndDestroy(operation);
                    }
                }

            HBufC* dbUri = NULL;
            TRAP(err, CleanupStack::Pop(dbUri = db->DatabaseUriLC()));
            CleanupStack::PushL(dbUri);
            VerifyErrorL(_L("<-- AddRemoveThread: DatabaseUriLC() left with %d -->"), err, aMessage);
            CleanupStack::PopAndDestroy(dbUri);
            }

        dbItems.Reset();
        CleanupStack::PopAndDestroy(&dbItems);

        // Let other threads get access to the database
        User::After(300000);

        stopTime.HomeTime();
        User::LeaveIfError(stopTime.MinutesFrom(startTime, executionTime));
        }

    CleanupStack::PopAndDestroy(db);
    }

// ---------------------------------------------------------
// CPosTp78::RunImportExportLmTestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp78::RunImportExportLmTestL(TDes* aMessage)
    {
    // Execute thread for a number of hours
    TTime startTime, stopTime;
    startTime.HomeTime();
    stopTime.HomeTime();
    TTimeIntervalMinutes executionTime;
    User::LeaveIfError(stopTime.MinutesFrom(startTime, executionTime));

    CPosLandmarkDatabase* db = CPosLandmarkDatabase::OpenL();
    CleanupStack::PushL(db);
    while (executionTime.Int() < KExectionTimeInMinutes)
        {
// Export landmarks
        TBool exportSuccessful = EFalse;
        CPosLmOperation* operation = NULL;
        CPosLandmarkEncoder* encoder = NULL;
        TRAPD(err, encoder = CPosLandmarkEncoder::NewL(KMimeType));
        VerifyErrorL(_L("<-- ImportExportThread: CPosLandmarkEncoder::NewL left with %d -->"), err, aMessage);
        CleanupStack::PushL(encoder);

        // Use buffer as output
        CBufBase* outputBuffer = NULL;
        TRAP(err, outputBuffer = encoder->SetUseOutputBufferL());
        VerifyErrorL(_L("<-- ImportExportThread: SetUseOutputBufferL left with %d -->"), err, aMessage);
        CleanupStack::PushL(outputBuffer);

        // Add collection data
        TRAP(err, encoder->AddCollectionDataL(EPosLmCollDataCollectionName, _L("collection Name")));
        VerifyErrorL(_L("<-- ImportExportThread: AddCollectionDataL(Name) left with %d -->"), err, aMessage);
        TRAP(err, encoder->AddCollectionDataL(EPosLmCollDataCollectionDescription, _L("collection Desc")));
        VerifyErrorL(_L("<-- ImportExportThread: AddCollectionDataL(Desc) left with %d -->"), err, aMessage);

        // Export landmarks
        RArray<TPosLmItemId> lmIdArray;
        CleanupClosePushL(lmIdArray);
        TRAP(err, AddLandmarksL(5, _L("ImportExportThread"), db, lmIdArray));
        VerifyErrorL(_L("<-- ImportExportThread: AddLandmarksL() left with %d -->"), err, aMessage);
        if (err == KErrNone)
            {
            TRAP(err, operation = db->ExportLandmarksL(*encoder, lmIdArray, 
                CPosLandmarkDatabase::EIncludeCategories | CPosLandmarkDatabase::EIncludeGlobalCategoryNames));
            VerifyErrorL(_L("<-- ImportExportThread: ExportLandmarksL left with %d -->"), err, aMessage);
            if (err == KErrNone)
                {
                CleanupStack::PushL(operation);
                TRAP(err, ExecuteAndDeleteLD(operation));
                CleanupStack::Pop(operation);
                VerifyErrorL(_L("<-- ImportExportThread: ExecuteAndDeleteLD(ExportLandmarksL) left with %d -->"), err, aMessage);

                // finalize encoding
                operation = NULL;
                TRAP(err, operation = encoder->FinalizeEncodingL());
                VerifyErrorL(_L("<-- ImportExportThread: FinalizeEncodingL left with %d -->"), err, aMessage);
                if (err == KErrNone)
                    {
                    CleanupStack::PushL(operation);
                    TRAP(err, ExecuteAndDeleteLD(operation));
                    CleanupStack::Pop(operation);
                    VerifyErrorL(_L("<-- ImportExportThread: ExecuteAndDeleteLD(FinalizeEncodingL) left with %d -->"), err, aMessage);
                    exportSuccessful = ETrue;
                    }
                }
            }
        CleanupStack::PopAndDestroy(&lmIdArray);

// Import landmarks
        if (exportSuccessful)
            {
            CPosLandmarkParser* parser = NULL;
            TRAP(err, parser = CPosLandmarkParser::NewL(KMimeType));
            CleanupStack::PushL(parser);
            VerifyErrorL(_L("<-- ImportExportThread: CPosLandmarkParser::NewL left with %d -->"), err, aMessage);

            // Use buffer as input
            TInt size = outputBuffer->Size();
            HBufC8* inputBuffer = HBufC8::New(size);
            if (!inputBuffer)
                {
                aMessage->Copy(_L8("Unable to allocate inputBuffer. Error: KErrNoMemory"));
                }
            CleanupStack::PushL(inputBuffer);
            TPtr8 inputBufferPtr = inputBuffer->Des();
            outputBuffer->Read(0, inputBufferPtr, size);
            parser->SetInputBuffer(inputBufferPtr);

            // Import landmarks
            operation = NULL;
            TRAP(err, operation = db->ImportLandmarksL(*parser, 
                CPosLandmarkDatabase::EDefaultOptions));
            CleanupStack::PushL(operation);
            VerifyErrorL(_L("<-- ImportExportThread: ImportLandmarksL left with %d -->"), err, aMessage);
            if (err == KErrNone)
                {
                TRAP(err, operation->ExecuteL());
                VerifyErrorL(_L("<-- ImportExportThread: ExecuteAndDeleteLD(ImportLandmarksL) left with %d -->"), err, aMessage);

                CPosLmItemIterator* iterator = NULL;
                TRAP(err, iterator = db->ImportedLandmarksIteratorL(operation));
                CleanupStack::PushL(iterator);
                VerifyErrorL(_L("<-- ImportExportThread: ImportedLandmarksIteratorL() left with %d -->"), err, aMessage);
                CleanupStack::PopAndDestroy(iterator);
                }
            CleanupStack::PopAndDestroy(operation);
            CleanupStack::PopAndDestroy(inputBuffer);
            CleanupStack::PopAndDestroy(parser);
            }
        CleanupStack::PopAndDestroy(outputBuffer);
        CleanupStack::PopAndDestroy(encoder);

        // Let other threads get access to the database
        User::After(100000);

        stopTime.HomeTime();
        User::LeaveIfError(stopTime.MinutesFrom(startTime, executionTime));
        }

    CleanupStack::PopAndDestroy(db);
    }

// ---------------------------------------------------------
// CPosTp78::RunDbEventTestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp78::RunDbEventTestL(TDes* aMessage)
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

    while (executionTime.Int() < KExectionTimeInMinutes - 1)
        {
        TRequestStatus status;
        TPosLmEvent event;

        // Listen for events for some time
        db->NotifyDatabaseEvent(event, status);
        User::After(100000);

        // Cancel listening
        TInt err = db->CancelNotifyDatabaseEvent();
        VerifyErrorL(_L("<-- DbEventThread: CancelNotifyDatabaseEvent returned %d -->"), err, aMessage);

        // Start listening again
        db->NotifyDatabaseEvent(event, status);
        User::WaitForRequest(status);
        VerifyErrorL(_L("<-- DbEventThread: NotifyDatabaseEvent returned %d -->"), status.Int(), aMessage);

        stopTime.HomeTime();
        User::LeaveIfError(stopTime.MinutesFrom(startTime, executionTime));
        }

    CleanupStack::PopAndDestroy(db);
    }

// ---------------------------------------------------------
// CPosTp78::UpdateLandmarksL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp78::UpdateLandmarksL(CPosLandmarkDatabase* aDb, TPosLmItemId aId, TDes* aMessage)
    {
    //CPosLandmark* landmark = NULL;
    CPosLandmark* landmark = aDb->ReadLandmarkLC(aId);
                //VerifyErrorL(_L("<-- ReadAndUpdateThread: ReadLandmarkLC() left with %d -->"), err, aMessage);
                
    TRAPD(err, landmark->SetLandmarkNameL(_L("UpdatedName")));
    VerifyErrorL(_L("<-- ReadAndUpdateThread: SetLandmarkNameL() left with %d -->"), err, aMessage);
    TRAP(err, aDb->UpdateLandmarkL(*landmark));
    VerifyErrorL(_L("<-- ReadAndUpdateThread: UpdateLandmarkL() left with %d -->"), err, aMessage);
    CleanupStack::PopAndDestroy(landmark);
    }

// ---------------------------------------------------------
// CPosTp78::RunReadAndUpdateTestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp78::RunReadAndUpdateTestL(TDes* aMessage)
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
        RArray<TPosLmItemId> dbItems;
        CleanupClosePushL(dbItems);
        
        const TInt KNrOfLms = 3;
        TRAPD(err, AddLandmarksL(KNrOfLms, _L("ReadAndUpdateThread"), db, dbItems));
        VerifyErrorL(_L("<-- ReadAndUpdateThread: AddLandmarksL() left with %d -->"), err, aMessage);

        if (err == KErrNone)
            {
            // Read and update
            for (TInt i = 0; i < KNrOfLms; i++)
                {
                /*
                CPosLandmark* landmark = NULL;
                TRAP(err, landmark = db->ReadLandmarkLC(dbItems[i]));
                VerifyErrorL(_L("<-- ReadAndUpdateThread: ReadLandmarkLC() left with %d -->"), err, aMessage);
                if (err == KErrNone)
                    {
                    TRAP(err, landmark->SetLandmarkNameL(_L("UpdatedName")));
                    VerifyErrorL(_L("<-- ReadAndUpdateThread: SetLandmarkNameL() left with %d -->"), err, aMessage);
                    TRAP(err, db->UpdateLandmarkL(*landmark));
                    VerifyErrorL(_L("<-- ReadAndUpdateThread: UpdateLandmarkL() left with %d -->"), err, aMessage);
                    CleanupStack::PopAndDestroy(landmark);
                    }
                    */
                    
                TRAPD(err, UpdateLandmarksL(db,dbItems[i], aMessage));
                VerifyErrorL(_L("<-- ReadAndUpdateThread: ReadLandmarkLC() left with %d -->"), err, aMessage);
                }

            // Set partial read parameters
            CPosLmPartialReadParameters* params = NULL;
            TRAP(err, CleanupStack::Pop(params = db->PartialReadParametersLC()));
            CleanupStack::PushL(params);
            VerifyErrorL(_L("<-- ReadAndUpdateThread: PartialReadParametersLC() left with %d -->"), err, aMessage);
            if (err == KErrNone)
                {
                params->SetRequestedAttributes(CPosLandmark::EIcon | CPosLandmark::ECoverageRadius);
                RArray<TUint> posFields;
                err = posFields.Append(EPositionFieldStreet);
                params->SetRequestedPositionFields(posFields);
                posFields.Close();
                VerifyErrorL(_L("<-- ReadAndUpdateThread: Append() returned %d -->"), err, aMessage);
                TRAP(err, db->SetPartialReadParametersL(*params));
                VerifyErrorL(_L("<-- ReadAndUpdateThread: SetPartialReadParametersL() left with %d -->"), err, aMessage);
                CleanupStack::PopAndDestroy(params);
                }

            // Read partial landmarks one by one
            for (TInt j = 0; j < KNrOfLms; j++)
                {
                CPosLandmark* lm = NULL;
                TRAP(err, CleanupStack::Pop(lm = db->ReadPartialLandmarkLC(dbItems[j])));
                CleanupStack::PushL(lm);
                VerifyErrorL(_L("<-- ReadAndUpdateThread: ReadPartialLandmarkLC() left with %d -->"), err, aMessage);
                CleanupStack::PopAndDestroy(lm);
                }
        
            // Read several partial landmarks in one chunk
            CPosLmOperation* operation = NULL;
            TRAP(err, operation = db->PreparePartialLandmarksL(dbItems));
            CleanupStack::PushL(operation);
            VerifyErrorL(_L("<-- ReadAndUpdateThread: PreparePartialLandmarksL() left with %d -->"), err, aMessage);

            if (err == KErrNone)
                {
                TRAP(err, operation->ExecuteL());
                VerifyErrorL(_L("<-- ReadAndUpdateThread: ExecuteAndDeleteLD(PreparePartialLandmarksL()) left with %d -->"), err, aMessage);
                if (err == KErrNone)
                    {
                    CArrayPtr<CPosLandmark>* landmarks = NULL;
                    TRAP(err, landmarks = db->TakePreparedPartialLandmarksL(operation));
                    if (landmarks)
                        {
                        landmarks->ResetAndDestroy();
                        delete landmarks;
                        }
                    VerifyErrorL(_L("<-- ReadAndUpdateThread: PreparePartialLandmarksL() left with %d -->"), err, aMessage);
                    }
                }
            CleanupStack::PopAndDestroy(operation);
            }
        CleanupStack::PopAndDestroy(&dbItems);

        // Let other threads get access to the database
        User::After(110000);

        stopTime.HomeTime();
        User::LeaveIfError(stopTime.MinutesFrom(startTime, executionTime));
        }
    CleanupStack::PopAndDestroy(db);
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

//  End of File
