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
#include "FT_CPosTp50.h"

#include <EPos_CPosLandmarkDatabase.h> 
#include <EPos_CPosLandmark.h> 
#include <EPos_CPosLandmarkDatabase.h>
#include <EPos_CPosLandmarkEncoder.h>
#include <EPos_CPosLandmarkParser.h>
#include <EPos_CPosLandmarkSearch.h> 
#include <EPos_CPosLmTextCriteria.h>
#include <LbsPosition.h> 
#include <e32std.h>
#include <ss_std.h>


const TInt KNoMultipleClients = 5;
_LIT(KExportFile,    "c:\\TP50-ExportedFile.xml");

_LIT(KAllLandmarks,         "c:\\TP50-AllLandmarks.xml");
        
_LIT(KAllLandmarksIndex,    "c:\\TP50-AllLandmarks%d.xml");

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CPosTp50::GetName
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp50::GetName(TDes& aName) const
    {
    _LIT(KTestName, "TP50 - Multiple import operations");
    aName = KTestName;
    }

// ---------------------------------------------------------
// CPosTp50::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp50::CloseTest()
    {
    for (TInt i=0; i<iThreads.Count(); i++)
        {
        iThreads[i].Close();
        }

    iThreads.Close();

    delete iDatabase;
    iDatabase=NULL;

    iUseLogFromThreadIsDisabled = EFalse;

    }

// ---------------------------------------------------------
// CPosTp50::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp50::StartL()
    {
    MakeSurePanicDebugFileExistsL();
    
    iDatabase = UseGeneratedDbFileL();
    if (iDatabase->IsInitializingNeeded())
       {
       ExecuteAndDeleteLD(iDatabase->InitializeL()); 
       }
       
    ExportLandmarksL();
    
    delete iDatabase;
    iDatabase=NULL;
    
    // We want an empty database
    RemoveDefaultDbL();
    
    iDatabase = CPosLandmarkDatabase::OpenL();
    
    if (iDatabase->IsInitializingNeeded())
       {
       ExecuteAndDeleteLD(iDatabase->InitializeL()); 
       }
       
    iUseLogFromThreadIsDisabled = ETrue;
    
    iLog->Put(_L("Imports landmarks syncronously simultaneously"));
    iTestStep=0;
    StartMultipleClientsL(KNoMultipleClients);
    
    iLog->Put(_L("Imports landmarks incrementally simultaneously"));
    ++iTestStep;
    StartMultipleClientsL(KNoMultipleClients);
    
    iLog->Put(_L("A search is started but no step is executed"));
    ++iTestStep;
    
    CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();
    textCriteria->SetTextL(_L("kalle"));
    
    CPosLandmarkSearch* landmarkSearch = CPosLandmarkSearch::NewL(*iDatabase);   
    CleanupStack::PushL(landmarkSearch);
        
    CPosLmOperation* op = landmarkSearch->StartLandmarkSearchL(*textCriteria);
    CleanupStack::PushL(op);
    
    iLog->Put(_L("Imports landmarks syncronously simultaneously (all should fail)"));
    StartMultipleClientsL(KNoMultipleClients);
    
    iLog->Put(_L("The search is cancelled"));
    CleanupStack::PopAndDestroy(op);
    
    iLog->Put(_L("Imports landmarks syncronously simultaneously (at least one should succed)"));
    ++iTestStep;
    StartMultipleClientsL(KNoMultipleClients);
       
    CleanupStack::PopAndDestroy(2, textCriteria);

    TestESLI_64LLU3L();
    }

// ---------------------------------------------------------
// CPosTp50::StartMultipleClientsL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp50::StartMultipleClientsL(const TUint aNoClients)
    {
    _LIT(KMultipleErr, "Error %d from thread %d");
    _LIT(KPanicErr, "Thread %d has panicked or is alive");
    _LIT(KAllErr, "Wrong number of threads left with error");
    _LIT(KSuccess, "Threads %d is successful");
    _LIT(KAllNotFailedErr, "All imports haven't failed");
      
    CreateThreadsL(aNoClients);

    RArray<TRequestStatus> statuses;
    CleanupClosePushL(statuses);
   
    for (TUint j=0; j<aNoClients; j++)
        {
        TRequestStatus status;
        statuses.Append(status);
        }

    TInt i=0; 
    for (i=0; i<iThreads.Count(); i++)
        { 
        iThreads[i].Logon(statuses[i]);
        iThreads[i].Resume();
        }
    
    for (i=0; i<iThreads.Count(); i++)
        {
        User::WaitForRequest(statuses[i]);
        }
    
    TInt errors=0; 
    for (i=0; i<iThreads.Count(); i++)
        {
        TInt exitReason = iThreads[i].ExitReason();
        TBuf<100> info;
            
        if (exitReason != KErrNone)
            {
            errors++;
            info.Format(KMultipleErr, exitReason, i+1);
            }  
         else
            {
            info.Format(KSuccess, i+1);
            }
        iLog->Put(info);

        AssertTrueSecL(iThreads[i].ExitType() == EExitKill, KPanicErr, i+1);
        AssertTrueSecL(exitReason == KErrNone || exitReason == KErrLocked, info);
        }
        
    if (iTestStep != EReadLockTaken)
        {
        AssertTrueSecL(errors == KNoMultipleClients - 1, KAllErr);        
        }
    else
        {
        // All imports should have failed since the started search should have taken a read lock
        AssertTrueSecL(errors == KNoMultipleClients, KAllNotFailedErr);        
        }
       
    for (i=0; i<iThreads.Count(); i++)
        {
        iThreads[i].Close();
        }
        
    iThreadIndex=0;
    iThreads.Close();
    CleanupStack::PopAndDestroy(&statuses);
    }

// ---------------------------------------------------------
// CPosTp50::RunTestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp50::RunTestL(TAny* aData)
    {
    CPosTp50* self = reinterpret_cast<CPosTp50*>(aData);
    TInt index = ++self->iThreadIndex;
    
    CPosLandmarkDatabase* lmd = CPosLandmarkDatabase::OpenL();
    CleanupStack::PushL(lmd);
    
    if (lmd->IsInitializingNeeded())
       {
       ExecuteAndDeleteLD(lmd->InitializeL()); 
       }
       
    CPosLandmarkParser* parser = CPosLandmarkParser::NewL(KMimeType);
    CleanupStack::PushL(parser);
    
    
    TBuf<100> fileName;
    fileName.Format(KAllLandmarksIndex, index);
    
    parser->SetInputFileL(fileName);
    
    _LIT(KInfo, "Thread %d");
    TBuf<100> info;
    info.Format(KInfo, index);
    RDebug::Print(info);
        
    CPosLmOperation* op = lmd->ImportLandmarksL(*parser, CPosLandmarkDatabase::EDefaultOptions); 
   
    _LIT(KInfo2, "Thread %d before execute");
    info.Format(KInfo2, index);
    RDebug::Print(info);
            
    switch (self->iTestStep)
        {
        //case self->ESync:
        case ESync:
        //case self->EReadLockTaken:
        case EReadLockTaken:
        //case self->ESearchCancelled:
        case ESearchCancelled:
            CleanupStack::PushL(op);
            op->ExecuteL(); 
            CleanupStack::PopAndDestroy(op);               
            break;
        //case self->EInc:
        case EInc:
            self->RunAsyncOperationLD(op);
            break;
        }
    
    _LIT(KInfo3, "Thread %d after execute");
    info.Format(KInfo3, index);
    RDebug::Print(info);
     
    CleanupStack::PopAndDestroy(parser);
    CleanupStack::PopAndDestroy(lmd);
    }


// ---------------------------------------------------------
// ThreadFunction
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
LOCAL_C TInt ThreadFunction(TAny* aData)
    {
    CTrapCleanup* cleanup=CTrapCleanup::New(); 

    CActiveScheduler* actSch = new (ELeave) CActiveScheduler;
    CActiveScheduler::Install(actSch);

    TRAPD(err, CPosTp50::RunTestL(aData));
       
    delete actSch;
    delete cleanup;
    return err;
    }

// ---------------------------------------------------------
// CPosTp50::CreateThreadsL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp50::CreateThreadsL(const TUint aNoThreads)
    {
    _LIT(KThreadName, "TP50 test thread%d");
    _LIT(KCreateThreadErr, "Create thread failed with %d");

    for (TUint i=0; i<aNoThreads; i++)
        {
        RThread thread;
        TBuf<32> name;
	    name.Format(KThreadName, 1+iThreads.Count());
                
        TInt err;
        err = thread.Create(name, ThreadFunction, KDefaultStackSize, KMinHeapSize, KMaxHeapSize, reinterpret_cast<TAny*>(this));

        AssertTrueSecL(err == KErrNone, KCreateThreadErr, err);                                                                  
        
        iThreads.Append(thread);
        }
    }
    
// ---------------------------------------------------------
// CPosTp50::ExportAllLandmarksL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//	
void CPosTp50::ExportLandmarksL() 
    {
    RFs fs;
    User::LeaveIfError(fs.Connect());
    CleanupClosePushL(fs);
    
    fs.Delete(KAllLandmarks);
    
    CPosLmItemIterator* iter = iDatabase->LandmarkIteratorL();
    CleanupStack::PushL(iter);

    RArray<TPosLmItemId> arrayOfIds;
    CleanupClosePushL(arrayOfIds);
    
    TInt numberOfElements = iter->NumOfItemsL();    
    iter->GetItemIdsL(arrayOfIds, 0, (numberOfElements));
    
    CPosLandmarkEncoder* encoder = CPosLandmarkEncoder::NewL(KMimeType);
    CleanupStack::PushL(encoder);

    encoder->SetOutputFileL(KAllLandmarks);
   
    CPosLmOperation* op = iDatabase->ExportLandmarksL(*encoder, arrayOfIds,  CPosLandmarkDatabase::EDefaultOptions); 
    CleanupStack::PushL(op);
    op->ExecuteL();
    CleanupStack::PopAndDestroy(op);
    
    op = encoder->FinalizeEncodingL(); 
    CleanupStack::PushL(op);
    op->ExecuteL();
    
    // Create a file to be used from each thread
    CFileMan* fileMan = CFileMan::NewL(fs);
    CleanupStack::PushL(fileMan);
   
    for (TInt i=0; i<KNoMultipleClients; i++)
        {
        TBuf<100> fileName;
        fileName.Format(KAllLandmarksIndex, i+1);
        fs.Delete(fileName);
        
        User::LeaveIfError(fileMan->Copy(KAllLandmarks, fileName, CFileMan::EOverWrite));     
        }
        
    CleanupStack::PopAndDestroy(6, &fs); 
    }

// ---------------------------------------------------------
// CPosTp50::TestESLI_64LLU3L
// ESLI-64LLU3 - read/write lock only affects the same database instance.
//
// (other items were commented in a header).
// ---------------------------------------------------------
//	
void CPosTp50::TestESLI_64LLU3L()
    {
    RFs fs;
    User::LeaveIfError(fs.Connect());
    fs.Delete(KExportFile);
    fs.Close();

    // Create encoder that will take read lock
    CPosLandmarkEncoder* encoder = CPosLandmarkEncoder::NewL(KMimeType);
    CleanupStack::PushL(encoder);
    encoder->SetOutputFileL(KExportFile);

    // Create another db handle
    CPosLandmarkDatabase* lmd = CPosLandmarkDatabase::OpenL();
    CleanupStack::PushL(lmd);
    if (lmd->IsInitializingNeeded())
       {
       ExecuteAndDeleteLD(lmd->InitializeL()); 
       }

    // Create array of ids to export
    RArray<TPosLmItemId> arrayOfIds;
    CleanupClosePushL(arrayOfIds);
	User::LeaveIfError(arrayOfIds.Append(1));
	User::LeaveIfError(arrayOfIds.Append(2));
	User::LeaveIfError(arrayOfIds.Append(3));

    // Take read lock by starting exporting
    CPosLmOperation* op = lmd->ExportLandmarksL(*encoder, arrayOfIds, CPosLandmarkDatabase::EDefaultOptions); 
    CleanupStack::PushL(op);

    // Verify take write lock fails on db instance 2
    TRAPD(err, lmd->RemoveLandmarkL(1));
    AssertTrueSecL(err == KErrLocked, _L("Didn't get KErrLocked but %d"), err);

    // Verify take write lock fails on db instance 1
    TRAP(err, iDatabase->RemoveLandmarkL(1)); 
    AssertTrueSecL(err == KErrLocked, _L("Didn't get KErrLocked but %d"), err);

    // Lock should prevent import threads to complete successfully
    iTestStep = EReadLockTaken;
    StartMultipleClientsL(KNoMultipleClients);

    // Release lock - import threads should complete successfully
    CleanupStack::PopAndDestroy(op);
    iTestStep = ESync;
    StartMultipleClientsL(KNoMultipleClients);

    CleanupStack::PopAndDestroy(&arrayOfIds);
    CleanupStack::PopAndDestroy(lmd);
    CleanupStack::PopAndDestroy(encoder);
    }
    
//  End of File
