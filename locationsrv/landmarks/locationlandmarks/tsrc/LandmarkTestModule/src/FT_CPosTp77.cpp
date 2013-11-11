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
#include "FT_CPosTp77.h"
#include <EPos_CPosLandmarkDatabase.h>

// CONSTANTS
// _LIT(KTp77Panic, "Timeout Panic");
const TInt KTp77PanicNumber = 77;

// ================= LOCAL FUNCTIONS =======================

// ---------------------------------------------------------
// LOCAL_C BadThreadFunction
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
LOCAL_C TInt BadClientThreadFunction(TAny* aData)
    {
    CTrapCleanup* cleanup = CTrapCleanup::New(); 
    CActiveScheduler* actSch = new (ELeave) CActiveScheduler;
    CActiveScheduler::Install(actSch);

    TRAPD(err, CPosTp77::RunBadClientTestL(aData));
       
    delete actSch;
    delete cleanup;
    return err;
    }

// ---------------------------------------------------------
// LOCAL_C KillerThreadFunction
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
LOCAL_C TInt KillerThreadFunction(TAny* aData)
    {
    CTrapCleanup* cleanup = CTrapCleanup::New(); 
    CActiveScheduler* actSch = new (ELeave) CActiveScheduler;
    CActiveScheduler::Install(actSch);

    TRAPD(err, CPosTp77::RunThreadKillerL(aData));
       
    delete actSch;
    delete cleanup;
    return err;
    }

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CPosTp77::GetName
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp77::GetName(TDes& aName) const
    {
    _LIT(KTestName, "TP77 - Bad server usage");
    aName = KTestName;
    }

// ---------------------------------------------------------
// CPosTp77::InitTestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp77::InitTestL()
    {
    MakeSurePanicDebugFileExistsL();
    }

// ---------------------------------------------------------
// CPosTp77::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp77::CloseTest()
    {
    iBadThread.Close();
    }

// ---------------------------------------------------------
// CPosTp77::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp77::StartL()
    {
// Test multiple Notify Db events
    iLog->Put(_L("1st part, testing multiple notify Db event"));
    iTestCase = ETestNotifyDbEvent;
    CPosLandmarkDatabase* db = InitDbLC();
    iTimeToWait = 3000000;
    ResumeThreadsAndVerifyExitL();
    TBool errors = VerifyThreadExitResultsL(KTp77PanicNumber);
    (errors) ?
        VerifyLmServerIsAliveL(db) :
        errors = VerifyLmServerIsAliveL(db);
    CleanupStack::PopAndDestroy(db);

// Test non terminated single Notify Db event
    iLog->Put(_L("2nd part, testing non terminated sinlge notify Db event"));
    iTestCase = ETestNotifyDbEvent2;
    db = InitDbLC();
    ResumeThreadsAndVerifyExitL();
    (errors) ?
        VerifyThreadExitResultsL(0) :
        errors = VerifyThreadExitResultsL(0);
    (errors) ?
        VerifyLmServerIsAliveL(db) :
        errors = VerifyLmServerIsAliveL(db);
    CleanupStack::PopAndDestroy(db);

// Test non terminated single InitializeL()
    iLog->Put(_L("3rd part, testing non terminated single Initialize db operations"));
    iTestCase = ETestInitialize;
    RemoveDefaultDbL();    
    db = CPosLandmarkDatabase::OpenL();
    CleanupStack::PushL(db);
    ResumeThreadsAndVerifyExitL();
    (errors) ?
        VerifyThreadExitResultsL(0) :
        errors = VerifyThreadExitResultsL(0);
    CPosLmOperation* initOp = db->InitializeL();
    CleanupStack::PushL(initOp);
    TRAPD(err, initOp->ExecuteL());
    if (err != KErrNone)
        {
        _LIT(KError, "Trying to initialize after badThread failed with %d");
        iBuf.Format(KError, err);
        iLog->PutError(iBuf);
        errors = ETrue;
        }
    CleanupStack::PopAndDestroy(initOp);
    (errors) ?
        VerifyLmServerIsAliveL(db) :
        errors = VerifyLmServerIsAliveL(db);
    CleanupStack::PopAndDestroy(db);

// Test non terminated multiple InitializeL()
    iLog->Put(_L("4th part, testing non terminated multiple Initialize db operations"));
    iTestCase = ETestInitialize2;
    RemoveDefaultDbL();    
    db = CPosLandmarkDatabase::OpenL();
    CleanupStack::PushL(db);
    ResumeThreadsAndVerifyExitL();
    (errors) ?
        VerifyThreadExitResultsL(0) :
        errors = VerifyThreadExitResultsL(0);
    initOp = db->InitializeL();
    CleanupStack::PushL(initOp);
    TRAP(err, initOp->ExecuteL());
    if (err != KErrNone)
        {
        _LIT(KError, "Trying to initialize after badThread failed with %d");
        iBuf.Format(KError, err);
        iLog->PutError(iBuf);
        errors = ETrue;
        }
    CleanupStack::PopAndDestroy(initOp);
    (errors) ?
        VerifyLmServerIsAliveL(db) :
        errors = VerifyLmServerIsAliveL(db);
    CleanupStack::PopAndDestroy(db);

// Test write lock on non terminated import operation
    iLog->Put(_L("5th part, testing write lock on non terminated import operation"));
    iTestCase = ETestWriteLock;
    db = UseGeneratedDbFileL();
    CleanupStack::PushL(db);
    ResumeThreadsAndVerifyExitL();
    (errors) ?
        VerifyThreadExitResultsL(0) :
        errors = VerifyThreadExitResultsL(0);
    (errors) ?
        VerifyLmServerIsAliveL(db) :
        errors = VerifyLmServerIsAliveL(db);
    CleanupStack::PopAndDestroy(db);

// The test below should be successful. This is the case on WINS but not in THUMB. On
// THUMB it runs for a while and then fails with KErrNoMemory. A duration test is needed!
/*
// Test RegisterUri (executed during OpenL())
    iLog->Put(_L("2nd part, testing registering URI..."));
    iTestCase = ETestRegisterUri;
    db = InitDbLC();

    TBool ready = EFalse;
    TInt diff;
    iTimeToWait = 3000000;
    while (!ready)
        {
        diff = iTimeToWait.Int() - iLastTimeToWait.Int();
        iLastTimeToWait = iTimeToWait;
        ResumeThreadsAndVerifyExitL();
        VerifyLmServerIsAliveL(db);
        if (iBadThreadExitReason == KTp77PanicNumber &&
            iBadThreadExitType == EExitKill)
            {
            // Increase iTimeToWait
            iTimeToWait = iTimeToWait.Int() + Abs(diff / 2);
            }
        else if (iBadThreadExitReason == 0 &&
            iBadThreadExitType == EExitKill)
            {
            // Decrease iTimeToWait
            iTimeToWait = iTimeToWait.Int() - Abs(diff / 2);
            }
        else
            {
            LogErrorAndLeave(_L("Got unexpected exit code from badThread"));
            }
        ready = iTimeToWait == iLastTimeToWait;
        }
    iBuf.Format(_L("Estimated critical code after %d microseconds"), iTimeToWait.Int());
    iLog->Put(buf);

    // Execute vulnarable code
    iTimeToWait = iTimeToWait.Int() - 3000;
    diff = 20;
    ready = EFalse;
    TInt i = 0;
    while (!ready)
        {
        iTimeToWait = iTimeToWait.Int() + diff;
        ResumeThreadsAndVerifyExitL();
        VerifyLmServerIsAliveL(db);
        ready = iBadThreadExitReason != KTp77PanicNumber;
        i++;
        }

    buf.Format(_L("Tested %d loops"), i);
    iLog->Put(buf);

    CleanupStack::PopAndDestroy(db);*/

    if (errors)
        {
        User::Leave(KErrGeneral);
        }
    }

// ---------------------------------------------------------
// CPosTp77::InitDbLC
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
CPosLandmarkDatabase* CPosTp77::InitDbLC()
    {
    RemoveDefaultDbL();
    CPosLandmarkDatabase* database = CPosLandmarkDatabase::OpenL();
    CleanupStack::PushL(database);
    
    if (database->IsInitializingNeeded())
        {
        ExecuteAndDeleteLD(database->InitializeL());
        }

    return database;
    }

// ---------------------------------------------------------
// CPosTp77::RunBadClientTestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp77::RunBadClientTestL(TAny* aData)
    {
    CPosTp77* tp77 = reinterpret_cast <CPosTp77*> (aData);
    switch (tp77->iTestCase)
        {
        case ETestNotifyDbEvent:
            {
            TPosLmEvent event1, event2, event3, event4, event5;
            TRequestStatus status1 = KErrNone;
            TRequestStatus status2 = KErrNone; 
            TRequestStatus status3 = KErrNone;
            TRequestStatus status4 = KErrNone;
            TRequestStatus status5 = KErrNone;
            TRequestStatus status[5] = {status1, status2, status3, status4, status5};

            CPosLandmarkDatabase* db1 = CPosLandmarkDatabase::OpenL();
            CleanupStack::PushL(db1);
            CPosLandmarkDatabase* db2 = CPosLandmarkDatabase::OpenL();
            CleanupStack::PushL(db2);
            CPosLandmarkDatabase* db3 = CPosLandmarkDatabase::OpenL();
            CleanupStack::PushL(db3);
            CPosLandmarkDatabase* db4 = CPosLandmarkDatabase::OpenL();
            CleanupStack::PushL(db4);
            CPosLandmarkDatabase* db5 = CPosLandmarkDatabase::OpenL();
            CleanupStack::PushL(db5);

            db1->NotifyDatabaseEvent(event1, status[0]);
            db2->NotifyDatabaseEvent(event2, status[1]);
            db3->NotifyDatabaseEvent(event3, status[2]);
            db4->NotifyDatabaseEvent(event4, status[3]);
            db5->NotifyDatabaseEvent(event5, status[4]);

            for (TInt i = 0; i < 5; i++)
                {
                User::WaitForRequest(status[i]);
                }

            CleanupStack::PopAndDestroy(5, db1);
            }
            break;

        case ETestNotifyDbEvent2:
            {
            CPosLandmarkDatabase* db1 = CPosLandmarkDatabase::OpenL();
            
            // Observe events
            TPosLmEvent event1;
            TRequestStatus status1;
            db1->NotifyDatabaseEvent(event1, status1);
            }
            break;

        case ETestInitialize:
            {
            CPosLandmarkDatabase* db = CPosLandmarkDatabase::OpenL();
            
            // Start to initialize db
            CPosLmOperation* op = db->InitializeL();
            TRequestStatus status;
            TReal32 progress;
            op->NextStep(status, progress);
            }
            break;

        case ETestInitialize2:
            {
            TRequestStatus status1, status2, status3, status4, status5;
            TReal32 progress1, progress2, progress3, progress4, progress5;
            CPosLandmarkDatabase* db1 = CPosLandmarkDatabase::OpenL();
            CPosLandmarkDatabase* db2 = CPosLandmarkDatabase::OpenL();
            CPosLandmarkDatabase* db3 = CPosLandmarkDatabase::OpenL();
            CPosLandmarkDatabase* db4 = CPosLandmarkDatabase::OpenL();
            CPosLandmarkDatabase* db5 = CPosLandmarkDatabase::OpenL();
        
            // Start to initialize db
            CPosLmOperation* op1 = db1->InitializeL();
            CPosLmOperation* op2 = db2->InitializeL();
            CPosLmOperation* op3 = db3->InitializeL();
            CPosLmOperation* op4 = db4->InitializeL();
            CPosLmOperation* op5 = db5->InitializeL();
            op1->NextStep(status1, progress1);
            op2->NextStep(status2, progress2);
            op3->NextStep(status3, progress3);
            op4->NextStep(status4, progress4);
            op5->NextStep(status5, progress5);
            }
            break;

        case ETestWriteLock:
            {
            CPosLandmarkDatabase* db = CPosLandmarkDatabase::OpenL();
            CleanupStack::PushL(db);
            CPosLmOperation* removeOp = db->RemoveAllLandmarksL();
            TReal32 progress(0);
            TRequestStatus status;
            while (progress < 0.4)
                {
                removeOp->NextStep(status, progress);
                User::WaitForRequest(status);
                }
            CleanupStack::Pop(db);
            }
            break;

        case ETestRegisterUri:
            {
            CPosLandmarkDatabase* db1 = CPosLandmarkDatabase::OpenL();
            delete db1;
            }
            break;

        };
    }

// ---------------------------------------------------------
// CPosTp77::RunThreadKillerL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp77::RunThreadKillerL(TAny* aData)
    {
    CPosTp77* tp77 = reinterpret_cast <CPosTp77*>(aData);

    // Enter a infinite loop. This reason to this is that I want to be sure 
    // that it is killed by the TP77-thread.
    TBool forever = ETrue;
    while (forever)
        {
        // Panic badThread if it is still alive
        if (tp77->iBadThread.ExitType() == EExitPending)
            {
            TRequestStatus status;
            tp77->iBadThread.Logon(status);

            switch (tp77->iTestCase)
                {
                case ETestNotifyDbEvent:
                    {
                    // Make sure that the badThread has started 5 observers of db events
                    User::After(tp77->iTimeToWait);

                    // Generate db event
                    CPosLandmarkDatabase* db = CPosLandmarkDatabase::OpenL();
                    CleanupStack::PushL(db);
                    CPosLandmark* lm = CPosLandmark::NewLC();
                    db->AddLandmarkL(*lm);

                    // Kill Event observing thread
                    tp77->iBadThread.Kill(KTp77PanicNumber);
                    User::WaitForRequest(status);

                    CleanupStack::PopAndDestroy(2, db);
                    }
                    break;

                case ETestNotifyDbEvent2:
                case ETestInitialize:
                case ETestInitialize2:
                case ETestWriteLock:
                    {
                    // Do nothing
                    User::LeaveIfError(tp77->iBadThread.LogonCancel(status));
                    forever = EFalse;
                    }
                    break;

                case ETestRegisterUri:
                    {
                    User::After(tp77->iTimeToWait);

                    // Kill Event observing thread
                    tp77->iBadThread.Kill(KTp77PanicNumber);
                    User::WaitForRequest(status);
                    }
                    break;

                };
            }
        else
            {
            forever = EFalse;
            }
        }
    }

// ---------------------------------------------------------
// CPosTp77::ResumeThreadsAndVerifyExitL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp77::ResumeThreadsAndVerifyExitL()
    {
    _LIT(KCreateThreadErr, "Creating thread failed with %d");

    // Create bad thread
    _LIT(KBadThreadName, "TP77 - Bad thread");
    iBadThread.Close();
    TInt err = iBadThread.Create(KBadThreadName, BadClientThreadFunction, 
        KDefaultStackSize, KMinHeapSize, KMaxHeapSize, this);
    AssertTrueSecL(err == KErrNone, KCreateThreadErr, err);

    // Create killer thread
    RThread killerThread;
    CleanupClosePushL(killerThread);
    _LIT(KKillerThreadName, "TP77 - Killer thread");
    err = killerThread.Create(KKillerThreadName, KillerThreadFunction, 
        KDefaultStackSize, KMinHeapSize, KMaxHeapSize, this);
    AssertTrueSecL(err == KErrNone, KCreateThreadErr, err);

    // Start threads and wait until either of them exits.
    TRequestStatus badThreadStatus, killerThreadStatus;
    iBadThread.Logon(badThreadStatus);
    iBadThread.Resume();
    killerThread.Logon(killerThreadStatus);
    killerThread.Resume();
    User::WaitForRequest(badThreadStatus);
    User::WaitForRequest(killerThreadStatus);

    // Fetch exit data
    iBadThreadExitType = iBadThread.ExitType();
    iBadThreadExitCategory = iBadThread.ExitCategory();
    iBadThreadExitReason = iBadThread.ExitReason();
    iKillerThreadExitType = killerThread.ExitType();
    iKillerThreadExitCategory = killerThread.ExitCategory();
    iKillerThreadExitReason = killerThread.ExitReason();

    // Log threads' exit results
    iBuf.Format(
        _L("BadThread ended with exit type: %d, exit reason: %d, exit category: %S"), 
        iBadThreadExitType, iBadThreadExitReason, &iBadThreadExitCategory);
//    iLog->Put(buf);
    iBuf.Format(
        _L("KillerThread ended with exit type: %d, exit reason: %d, exit category: %S"), 
        iKillerThreadExitType, iKillerThreadExitReason, &iKillerThreadExitCategory);
//    iLog->Put(buf);

    CleanupStack::PopAndDestroy(&killerThread);
    }

// ---------------------------------------------------------
// CPosTp77::VerifyThreadExitResultsL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TBool CPosTp77::VerifyThreadExitResultsL(TInt aBadThreadExitReason)
    {
    TBool error = EFalse;
    _LIT(KKillCategory, "Kill");

    if (iBadThreadExitType != EExitKill)
        {
        _LIT(KExitTypeErr, "BadThread was not killed as expected.");
        iLog->PutError(KExitTypeErr);
        error = ETrue;
        }
    if (iBadThreadExitCategory != KKillCategory)
        {
        _LIT(KPanicErr, "BadThread had unexpected exit category");
        iLog->PutError(KPanicErr);
        error = ETrue;
        }
    if (iBadThreadExitReason != aBadThreadExitReason)
        {
        _LIT(KPanicCodeErr, "BadThread had unexpected exit reason");
        iLog->PutError(KPanicCodeErr);
        error = ETrue;
        }
    if (iKillerThreadExitType != EExitKill)
        {
        _LIT(KExitTypeErr, "KillerThread was not killed as expected.");
        iLog->PutError(KExitTypeErr);
        error = ETrue;
        }
    if (iKillerThreadExitCategory != KKillCategory)
        {
        _LIT(KPanicErr, "KillerThread had unexpected exit category");
        iLog->PutError(KPanicErr);
        error = ETrue;
        }
    if (iKillerThreadExitReason != 0)
        {
        _LIT(KPanicCodeErr, "KillerThread had unexpected exit reason");
        iLog->PutError(KPanicCodeErr);
        error = ETrue;
        }

    return error;
    }

// ---------------------------------------------------------
// CPosTp77::VerifyLmServerIsAliveL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TBool CPosTp77::VerifyLmServerIsAliveL(CPosLandmarkDatabase* aDb)
    {
    TRAPD(err, aDb->RemoveLandmarkL(1));
    TBool errorExists = err != KErrNone && err != KErrNotFound;
    if (errorExists)
        {
        _LIT(KError, "Unable to use db instance. Using db failed with %d.");
        iBuf.Format(KError, err);
        iLog->PutError(iBuf);
        }

    return errorExists;
    }

//  End of File
