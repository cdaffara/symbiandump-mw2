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
#include "FT_CPosTp46.h"

#include <EPos_CPosLandmarkDatabase.h> 
#include <EPos_CPosLandmarkDatabase.h>
#include <EPos_CPosLandmarkEncoder.h>
#include <EPos_CPosLandmarkParser.h>



#include <LbsPosition.h> 
#include <e32std.h>
#include <ss_std.h>

const TInt KNoMultipleClients = 10;
_LIT(KExportFileIndex,    "c:\\TP46-ExportedFileFromThread%d.xml");

#ifdef __WINS__
    _LIT(KImportFile, "z:\\system\\test\\testdata\\TP45-DefOptions.xml");
#else                   
    _LIT(KImportFile, "c:\\system\\test\\testdata\\TP45-DefOptions.xml");
#endif

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CPosTp46::GetName
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp46::GetName(TDes& aName) const
    {
    _LIT(KTestName, "TP46 - Multiple export operations");
    aName = KTestName;
    }

// ---------------------------------------------------------
// CPosTp46::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp46::CloseTest()
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
// CPosTp46::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp46::StartL()
    {
    // Make sure no logging from thread (causes crash)
    iUseLogFromThreadIsDisabled = ETrue;

    MakeSurePanicDebugFileExistsL();

    iDatabase = UseGeneratedDbFileL();
    if (iDatabase->IsInitializingNeeded())
       {
       ExecuteAndDeleteLD(iDatabase->InitializeL()); 
       }
           
    RemoveFilesL();

    iLog->Put(_L("Export landmarks syncronously simultaneously"));
    iTestStep=0;
    StartMultipleClientsL(KNoMultipleClients);
    
    iLog->Put(_L("Export landmarks incrementally simultaneously"));
    ++iTestStep;
    RemoveFilesL();
    StartMultipleClientsL(KNoMultipleClients);
   
    iLog->Put(_L("Export landmarks after import operation has taken write lock"));
    ++iTestStep;
    RemoveFilesL();
    CPosLandmarkParser* parser = CPosLandmarkParser::NewL(KMimeType);
    CleanupStack::PushL(parser);
    parser->SetInputFileL(KImportFile);
    // Take write lock
    CPosLmOperation* op = iDatabase->ImportLandmarksL(*parser, CPosLandmarkDatabase::EDefaultOptions); 
    CleanupStack::PushL(op);
    // Try to export
    StartMultipleClientsL(KNoMultipleClients);
    CleanupStack::PopAndDestroy(2, parser);

    // ESLI-64LLU3 - read/write lock only affects the same database instance.
    parser = CPosLandmarkParser::NewL(KMimeType);
    CleanupStack::PushL(parser);
    parser->SetInputFileL(KImportFile);

    CPosLandmarkDatabase* lmd = CPosLandmarkDatabase::OpenL();
    CleanupStack::PushL(lmd);
    
    if (lmd->IsInitializingNeeded())
       {
       ExecuteAndDeleteLD(lmd->InitializeL()); 
       }

    op = iDatabase->ImportLandmarksL(*parser, CPosLandmarkDatabase::EDefaultOptions); 
    CleanupStack::PushL(op);

    TRAPD(err, CleanupStack::PopAndDestroy(lmd->ReadLandmarkLC(1)));
    AssertTrueSecL(err == KErrLocked, _L("Didn't get KErrLocked but %d"), err);

    TRAP(err, CleanupStack::PopAndDestroy(iDatabase->ReadLandmarkLC(1))); 
    AssertTrueSecL(err == KErrLocked, _L("Didn't get KErrLocked but %d"), err);

    // Lock should prevent export threads to complete successfully
    StartMultipleClientsL(KNoMultipleClients);

    // Release lock - export threads should complete successfully
    CleanupStack::PopAndDestroy(op);
    iTestStep = ESync;
    StartMultipleClientsL(KNoMultipleClients);

    CleanupStack::PopAndDestroy(lmd);
    CleanupStack::PopAndDestroy(parser);
    }

// ---------------------------------------------------------
// CPosTp46::RemoveFiles
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp46::RemoveFilesL()
    {
    RFs fs;
    User::LeaveIfError(fs.Connect());
    for (TInt i=1; i <= KNoMultipleClients; i++)
        {
        TBuf<100> fileName;
        fileName.Format(KExportFileIndex, i);
        fs.Delete(fileName);
        }
    fs.Close();
    }

// ---------------------------------------------------------
// CPosTp46::StartMultipleClientsL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp46::StartMultipleClientsL(const TUint aNoClients)
    {
    _LIT(KMultipleErr, "Error %d from thread %d");
    _LIT(KPanicErr, "Thread %d has panicked or is alive");
    
    _LIT(KSuccess, "Threads %d is successful");
     
    CreateThreadsL(aNoClients);

    RArray<TRequestStatus> statuses;
    CleanupClosePushL(statuses);
   
    for (TUint j=0; j<aNoClients; j++)
        {
        TRequestStatus status;
        statuses.Append(status);
        }

    TUint i=0; 
    for (i=0; i<aNoClients; i++)
        { 
        iThreads[i].Logon(statuses[i]);
        iThreads[i].Resume();
        }
    
    for (i=0; i<aNoClients; i++)
        {
        User::WaitForRequest(statuses[i]);
        }
    
    TUint errors=0; 
    for (i=0; i<aNoClients; i++)
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
        
    if (iTestStep != EImportStarted)
        {
        AssertTrueSecL(errors == 0, _L("No thread should leave with error, all should be able to read from the db"));
        }
    else
        {
        AssertTrueSecL(errors == aNoClients, _L("All threads should leave with error, none should be able to read from the db"));
        }
    
       
    for (i=0; i<aNoClients; i++)
        {
        iThreads[i].Close();
        }
        
    iThreadIndex=0;
    iThreads.Close();
    CleanupStack::PopAndDestroy(&statuses);
    }

// ---------------------------------------------------------
// CPosTp46::RunTestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp46::RunTestL(TAny* aData)
    {
    CPosTp46* self = reinterpret_cast<CPosTp46*>(aData);
    TInt index = ++self->iThreadIndex;
    
    CPosLandmarkDatabase* lmd = CPosLandmarkDatabase::OpenL();
    CleanupStack::PushL(lmd);
    
    if (lmd->IsInitializingNeeded())
       {
       ExecuteAndDeleteLD(lmd->InitializeL()); 
       }
   /*
    _LIT(KInfo, "Thread %d");
    TBuf<100> info;
    info.Format(KInfo, index);
    RDebug::Print(info);
    */ 

    CPosLandmarkEncoder* encoder = CPosLandmarkEncoder::NewL(KMimeType);
    CleanupStack::PushL(encoder);
    
    CPosLmItemIterator* iter = lmd->LandmarkIteratorL();
    CleanupStack::PushL(iter);

    RArray<TPosLmItemId> arrayOfIds;
    CleanupClosePushL(arrayOfIds);
        
    TInt numberOfElements = iter->NumOfItemsL();    
    iter->GetItemIdsL(arrayOfIds, 0, (numberOfElements));   

    TBuf<100> fileName;
    fileName.Format(KExportFileIndex, index);
    encoder->SetOutputFileL(fileName);

    switch (self->iTestStep)
        {
        //case self->ESync:
        	case 0:
        //case self->EImportStarted:
        	case 2:
            {
            CPosLmOperation* op = lmd->ExportLandmarksL(*encoder, arrayOfIds, CPosLandmarkDatabase::EIncludeCategories); 
    
            CleanupStack::PushL(op);
            op->ExecuteL();
            CleanupStack::PopAndDestroy(op);
    
            op = encoder->FinalizeEncodingL(); 
            CleanupStack::PushL(op);
            op->ExecuteL();
            CleanupStack::PopAndDestroy(op);
            break;
            }
        //case self->EInc:
        	case 1:
            {
            CPosLmOperation* op2 = lmd->ExportLandmarksL(*encoder, arrayOfIds, CPosLandmarkDatabase::EIncludeCategories);  
            self->RunAsyncOperationLD(op2);

            op2 = encoder->FinalizeEncodingL(); 
            self->RunAsyncOperationLD(op2);
            break;
            }  
        }
    /*
    _LIT(KInfo3, "Thread %d after execute");
    info.Format(KInfo3, index);
    RDebug::Print(info);
    */
    CleanupStack::PopAndDestroy(4, lmd);
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

    TRAPD(err, CPosTp46::RunTestL(aData));
       
    delete actSch;
    delete cleanup;
    return err;
    }

// ---------------------------------------------------------
// CPosTp46::CreateThreadsL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp46::CreateThreadsL(const TUint aNoThreads)
    {
    _LIT(KThreadName, "TP46 test thread%d");
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

//  End of File
