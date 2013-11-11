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
#include "FT_CPosTp58.h"
#include <EPos_CPosLandmarkDatabase.h>
#include <EPos_CPosLandmarkParser.h>

// CONSTANTS
#ifdef __WINS__
_LIT(KXMLFile, "z:\\system\\test\\testdata\\Tp49ImportInput.xml"); // re-use
#else
_LIT(KXMLFile, "c:\\system\\test\\testdata\\Tp49ImportInput.xml"); // re-use
#endif
_LIT(KTp58Panic, "Timeout Panic");
const TInt KTp58PanicNumber = 58;

// ================= LOCAL FUNCTIONS =======================

// ---------------------------------------------------------
// LOCAL_C ImportThreadFunction
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
LOCAL_C TInt ImportThreadFunction(TAny* aData)
    {
    CTrapCleanup* cleanup = CTrapCleanup::New(); 

    CActiveScheduler* actSch = new (ELeave) CActiveScheduler;
    CActiveScheduler::Install(actSch);

    TRAPD(err, CPosTp58::RunImportTestL(aData));
       
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

    TRAPD(err, CPosTp58::RunThreadKillerL(reinterpret_cast <RThread*> (aData)));
       
    delete actSch;
    delete cleanup;
    return err;
    }

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CPosTp58::GetName
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp58::GetName(TDes& aName) const
    {
    _LIT(KTestName, "TP58 - Import landmarks using User::WaitForRequest()");
    aName = KTestName;
    }

// ---------------------------------------------------------
// CPosTp58::InitTestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp58::InitTestL()
    {
    MakeSurePanicDebugFileExistsL();
    }

// ---------------------------------------------------------
// CPosTp58::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp58::CloseTest()
    {
    }

// ---------------------------------------------------------
// CPosTp58::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp58::StartL()
    {
// Test Import all landmarks
    iLog->Put(_L("Importing all landmarks"));
    iTestCase = ETestImportAllLandmarks;
    iExpectedErrorCode = KErrGeneral;
    ResumeThreadsAndVerifyExitL();

// Test Import subset of landmarks
    iLog->Put(_L("Importing subset of all landmarks"));
    iTestCase = ETestImportSubsetOfLandmarks;
    iExpectedErrorCode = KErrGeneral;
    ResumeThreadsAndVerifyExitL();
    }

// ---------------------------------------------------------
// CPosTp58::InitDbLC
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
CPosLandmarkDatabase* CPosTp58::InitDbLC()
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
// CPosTp58::RunImportTestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp58::RunImportTestL(TAny* aData)
    {
    CPosTp58* tp58 = reinterpret_cast <CPosTp58*> (aData);

    switch (tp58->iTestCase)
        {
        case ETestImportAllLandmarks:
            tp58->ImportLandmarksL(NULL);
            break;
        case ETestImportSubsetOfLandmarks:
            {
            RArray<TUint> subset;
            CleanupClosePushL(subset);    
            subset.Append(5);
            subset.Append(1);
            subset.Append(9);
            subset.Append(3);

            tp58->ImportLandmarksL(&subset);

            CleanupStack::PopAndDestroy(&subset);
            }
            break;
        };
    }

// ---------------------------------------------------------
// CPosTp58::RunThreadKillerL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp58::RunThreadKillerL(RThread* aThread)
    {
    TTimeIntervalMicroSeconds32 oneMinute = 60 * 1000000;

    // Enter a infinite loop. This reason to this is that I want to be sure 
    // that it is killed by the TP58-thread.
    TBool forever = ETrue;
    while (forever)
        {
        User::After(oneMinute);

        // Panic importThread if it is still alive
        if (aThread->ExitType() == EExitPending)
            {
            TRequestStatus status;
            aThread->Logon(status);
            aThread->Panic(KTp58Panic, KTp58PanicNumber);
            User::WaitForRequest(status);
            }
        }
    }

// ---------------------------------------------------------
// CPosTp58::ImportLandmarksL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp58::ImportLandmarksL(const RArray<TUint>* aLandmarkSubSet)
    {
    CPosLandmarkDatabase* db = InitDbLC();

    // Recreate parser
    CPosLandmarkParser* landmarkParser = CPosLandmarkParser::NewL(KMimeType);
    CleanupStack::PushL(landmarkParser);
    landmarkParser->SetInputFileL(KXMLFile);

    CPosLmOperation* operation = NULL;
    if (aLandmarkSubSet)
        {
        // Import only a subset of the landmarks in parser
        operation = db->ImportLandmarksL(
            *landmarkParser, 
            *aLandmarkSubSet,  
            CPosLandmarkDatabase::EDefaultOptions);
        }
    else
        {
        operation = db->ImportLandmarksL(
            *landmarkParser, 
            CPosLandmarkDatabase::EDefaultOptions);
        }
    TRequestStatus status(KPosLmOperationNotComplete);
    TReal32 progress(0);
    operation->NextStep(status, progress);

    // Wait for NextStep to complete - should hang
    User::WaitForRequest(status);

    // Cancel the import operation
    delete operation;

    CleanupStack::PopAndDestroy(2, db);
    }

// ---------------------------------------------------------
// CPosTp58::ResumeThreadsAndVerifyExitL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp58::ResumeThreadsAndVerifyExitL()
    {
    iLog->Put(_L("ResumeThreadAndVerifyExitL"));
    _LIT(KCreateThreadErr, "Creating thread failed with %d");

	TBuf<100> threadName;
    // Create import thread
    RThread importThread;
    CleanupClosePushL(importThread);
    _LIT(KImportThreadName, "TP58 - Import thread, part %d");
    threadName.Format(KImportThreadName, iTestCase);
    iLog->Put(_L("Creating Import thread"));
    //TInt err = importThread.Create(KImportThreadName, ImportThreadFunction, 
    TInt err = importThread.Create(threadName, ImportThreadFunction, 
        KDefaultStackSize, KMinHeapSize, KMaxHeapSize, this);

    AssertTrueSecL(err == KErrNone, KCreateThreadErr, err);

    // Create killer thread
    RThread killerThread;
    CleanupClosePushL(killerThread);
    _LIT(KKillerThreadName, "TP58 - Killer thread, part %d");
    threadName.Format(KKillerThreadName, iTestCase);
    iLog->Put(_L("Creating Killer thread"));
    err = killerThread.Create(threadName, KillerThreadFunction, 
        KDefaultStackSize, KMinHeapSize, KMaxHeapSize, &importThread);
    AssertTrueSecL(err == KErrNone, KCreateThreadErr, err);

    // Start threads and wait until either of them exits.
    TRequestStatus importThreadStatus, killerThreadStatus;
    importThread.Logon(importThreadStatus);
    importThread.Resume();
    killerThread.Logon(killerThreadStatus);
    killerThread.Resume();
    User::WaitForRequest(importThreadStatus, killerThreadStatus);

    // Kill the thread that might still be alive
    TExitType importThreadExitType = importThread.ExitType();
    TExitCategoryName importThreadExitCategory = importThread.ExitCategory();
    TInt importThreadExitReason = importThread.ExitReason();
    if (importThreadExitType == EExitPending)
        {
        importThread.Kill(KErrCancel);
        User::WaitForRequest(importThreadStatus);
        }
    TExitType killerThreadExitType = killerThread.ExitType();
    TExitCategoryName killerThreadExitCategory = killerThread.ExitCategory();
    TInt killerThreadExitReason = killerThread.ExitReason();
    if (killerThreadExitType == EExitPending)
        {
        killerThread.Kill(KErrCancel);
        User::WaitForRequest(killerThreadStatus);
        }
    
    // Log threads' exit results
    TBuf<100> buf;
    buf.Format(
        _L("ImportThread ended with exit type: %d, exit reason: %d, exit category: %S"), 
        importThreadExitType, importThreadExitReason, &importThreadExitCategory);
    iLog->Put(buf);
    buf.Format(
        _L("KillerThread ended with exit type: %d, exit reason: %d, exit category: %S"), 
        killerThreadExitType, killerThreadExitReason, &killerThreadExitCategory);
    iLog->Put(buf);

    // Verify thread exit results
    TBool error = VerifyThreadExitResults(killerThreadExitType, importThreadExitType, 
        importThreadExitCategory, importThreadExitReason);
    if (error)
        {
        User::Leave(KErrGeneral);
        }

    CleanupStack::PopAndDestroy(2, &importThread);
    }

// ---------------------------------------------------------
// CPosTp58::VerifyThreadExitResults
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TBool CPosTp58::VerifyThreadExitResults(
    TExitType aKillerThreadExitType,
    TExitType aImportThreadExitType,
    const TExitCategoryName& aImportThreadExitCategory,
    TInt aImportThreadExitReason)
    {
    TBool error = EFalse;

    if (aImportThreadExitType != EExitPanic)
        {
        _LIT(KExitTypeErr, "Import thread was not panicked as expected.");
        iLog->PutError(KExitTypeErr);
        error = ETrue;
        }
    if (aImportThreadExitCategory != KTp58Panic)
        {
        _LIT(KPanicErr, "Import thread had unexpected panic category");
        iLog->PutError(KPanicErr);
        error = ETrue;
        }
    if (aImportThreadExitReason != KTp58PanicNumber)
        {
        _LIT(KPanicCodeErr, "Import thread had unexpected panic number");
        iLog->PutError(KPanicCodeErr);
        error = ETrue;
        }
    if (aKillerThreadExitType != EExitPending)
        {
        _LIT(KExitTypeErr, "Killer thread was not alive as expected.");
        iLog->PutError(KExitTypeErr);
        error = ETrue;
        }

    return error;
    }

//  End of File
