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

#include "FT_CPosTp39.h"
#include <EPos_CPosLandmarkDatabase.h> 
#include <EPos_CPosLandmark.h> 
#include <EPos_CPosLMCategoryManager.h>
#include <EPos_CPosLandmarkCategory.h>  
#include <e32std.h>
#include <ss_std.h>
#include <bautils.h>
#include "FT_LandmarkConstants.h"

//  CONSTANTS
const TInt KNoListeners=5;
const TInt KMaxQueueSize = 5;
const TInt KLmServerMaxHeapSize = 40000;

_LIT(KLandmarkServerName, "eposlmserver*");


// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CPosTp39::GetName
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp39::GetName(TDes& aName) const
    {
    _LIT(KTestName, "TP39 - Heap Usage (Goofy)");
    aName = KTestName;
    }

// ---------------------------------------------------------
// CPosTp39::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp39::StartL()
    {  
    _LIT(KMultipleErr, "Error %d when multiple clients listen for events");
    _LIT(KEventErr, "%d Error(s) when multiple clients listen for events");
    MakeSurePanicDebugFileExistsL();
    iUseLogFromThreadIsDisabled = ETrue;
    
    _LIT(KCanceledErr, "Test canceled");
    _LIT(KStartGoofy, "Start up Goofy and start a test (Options->Test->Start).");

    RemoveDefaultDbL();
   
#ifndef __WINS__    
    TUtfwUserAnswer answer;
    answer = iUserInfo->ShowDialog(KStartGoofy, EUtfwDialogTypeOkCancel, EFalse);
    AssertTrueSecL(answer == EUtfwUserAnswerOk, KCanceledErr, KErrCancel);
#endif  
    
    RemoveDefaultDbL();

    SetupGlobalCategoriesL();
    
    CPosLandmarkDatabase* database = CPosLandmarkDatabase::OpenL();
    delete database;

    TFixedArray<TRequestStatus, KNoListeners+1> statuses;

    CreateThreadsL();

    for (TInt i=0; i<iThreads.Count(); i++)
        { 
        iThreads[i].Logon(statuses[i]);
        iThreads[i].Resume();
        }  
    for (TInt j=0; j<iThreads.Count(); j++)
        {
        User::WaitForRequest(statuses[j]);
        }
    
    for (TInt t=0; t<iThreads.Count(); t++)
        {
        TInt exitReason = iThreads[t].ExitReason();
      
        AssertTrueSecL(exitReason == KErrNone, KMultipleErr, exitReason);
        }
    
    if (iErrors > 0)
        {
        TBuf<100> info;
	    info.Format(KEventErr, iErrors);
        LogErrorAndLeave(iErrorLog);
        }

    
#ifndef __WINS__

    TInt heapSize, stackSize=0;
    
    TBuf<127> info;
    TInt err = GetHeapSizeL(KLandmarkServerName, heapSize, stackSize);
    if (err)
        {
        info.Format(_L("GetHeapSizeL error, %d"), err);
        LogErrorAndLeave(info);
        }
        
    info.Format(_L("LandmarkServer Heap: %d LandmarkServer Stack:"), heapSize, stackSize);
    iLog->Put(info);
    
    if (heapSize > KLmServerMaxHeapSize)
        {
        LogErrorAndLeave(_L("The memory consumption for the LandmarkServer is too high!"));
        }        
#endif

/*
    _LIT(KCatId, "category_id = %d;");
    _LIT(KCatName, "category_name = \"%S\";");
    
    // 201-300
    _LIT(KName, "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901%d");
    
    for (TInt i=201; i<=300; i++)
        {
        iLog->Put(_L("POS_LM_CATEGORY_INFO"));
        iLog->Put(_L("{"));
        
        TBuf<150> info;    
        
        info.Format(KCatId, (i+1));
        
        iLog->Put(info);
        
        info.Format(KName, i);
            
        HBufC* name = info.Alloc();

        info.Format(KCatName, name);
        iLog->Put(info);
        iLog->Put(_L("},"));
        }   
*/
    }

// ---------------------------------------------------------
// CPosTp39::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp39::CloseTest()
    {
    for (TInt i=0; i<iThreads.Count(); i++)
        {
        iThreads[i].Close();
        }

    iThreads.Close();
    iUseLogFromThreadIsDisabled = EFalse;
    
    RemoveGlobalCategoriesL();
    }

// ---------------------------------------------------------
// CPosTp39::SetupGlobalCategoriesL
// 
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp39::SetupGlobalCategoriesL()
    {
    #ifdef __WINS__
        _LIT(KGlobalCategoryResFile, "z:\\system\\test\\testdata\\EPOSLMGLOBALCATEGORIES.R03");
    #else
        _LIT(KGlobalCategoryResFile, "c:\\system\\test\\testdata\\EPOSLMGLOBALCATEGORIES.R03");
    #endif
    //_LIT(KGlobalCategoryResPath, "c:\\system\\data\\");
    //_LIT(KGlobalCategoryResFileOld, "c:\\system\\data\\EPOSLMGLOBALCATEGORIES.R03");
    //_LIT(KGlobalCategoryResFileNew, "c:\\system\\data\\EPOSLMGLOBALCATEGORIES.R01");
    
        _LIT(KGlobalCategoryResFileOld, "c:\\resource\\EPOSLMGLOBALCATEGORIES.R03");
    _LIT(KGlobalCategoryResFileNew, "c:\\resource\\EPOSLMGLOBALCATEGORIES.R01");
    _LIT(KGlobalCategoryResPath, "c:\\resource\\");
    
    RFs fs;
    User::LeaveIfError(fs.Connect());
    CleanupClosePushL(fs);
    
    CFileMan* fileMan = CFileMan::NewL(fs);
    CleanupStack::PushL(fileMan);
    
    //check if landmark is flashed  
    if (!BaflUtils::FileExists(fs, KGlobalCategoryResFileROM))
        {
        iLog->Put(_L("Landmark is NOT flashed, rename global categories"));
        //Landmark is not flashed rename the file before copy a own defiend file.
        //since landmark is not flashed the file should exist hence leaving if it is not found!
        User::LeaveIfError(fileMan->Rename(KGlobalCategoryResFileWINS, KGlobalCategoryResFileCOPY, CFileMan::EOverWrite));
        }
     else
        {
        iLog->Put(_L("Landmark is flashed, copy global categories"));
        }

    User::LeaveIfError(fileMan->Copy(KGlobalCategoryResFile, KGlobalCategoryResPath, CFileMan::EOverWrite));
    User::LeaveIfError(fileMan->Rename(KGlobalCategoryResFileOld, KGlobalCategoryResFileNew, CFileMan::EOverWrite));  
       
    CleanupStack::PopAndDestroy(2, &fs);
    
    }
    
// ---------------------------------------------------------
// CPosTp39::RunReceiveEventTestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp39::RunEventsListenerL(TAny* aData)
    {
    CPosTp39* self = reinterpret_cast<CPosTp39*>(aData);

    CEventObserverTp39* eventObserver = CEventObserverTp39::NewL(self, ++self->iThreadIndex);
    CleanupStack::PushL(eventObserver);
    eventObserver->Start();

    eventObserver->StartTimer();
    CActiveScheduler::Start();
   
    CleanupStack::PopAndDestroy(eventObserver);
    }

LOCAL_C TInt ReceiveEventsThreadFunction(TAny* aData)
    {    
    CTrapCleanup* cleanup=CTrapCleanup::New(); 
    
    CActiveScheduler* actSch = new (ELeave) CActiveScheduler;
    CActiveScheduler::Install(actSch);

    TRAPD(err, CPosTp39::RunEventsListenerL(aData));
    
    delete actSch;
    delete cleanup;
    return err;
    }

// ---------------------------------------------------------
// CPosTp39::CreateEventsTestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp39::RunEventsGeneratorL(TAny* /*aData*/)
    {
    //CPosTp39* self = reinterpret_cast<CPosTp39*>(aData);

    CPosLandmarkDatabase* lmd = CPosLandmarkDatabase::OpenL();
    CleanupStack::PushL(lmd);
    
    if (lmd->IsInitializingNeeded())
       {
       ExecuteAndDeleteLD(lmd->InitializeL()); 
       }

    CPosLmCategoryManager* cm = CPosLmCategoryManager::NewL(*lmd); 
    CleanupStack::PushL(cm);
    
    /////////////////////////////////////////////
	// Add one landmark #1
    //////////////////////////////////////////////
    CPosLandmark* landmark = CPosLandmark::NewLC();
	landmark->SetLandmarkNameL(_L("TP39"));
	lmd->AddLandmarkL(*landmark);
    CleanupStack::PopAndDestroy(landmark);
   
    /////////////////////////////////////////////
	// Add 5 categories #2
    /////////////////////////////////////////////

    _LIT(KCategoryName, "TP39 Category %d");
    for (TInt j =0; j < KMaxQueueSize; j++)
        {
        TBuf<100> name;
        name.Format(KCategoryName, j);
        CPosLandmarkCategory* category = CPosLandmarkCategory::NewLC();
        category->SetCategoryNameL(name);
        cm->AddCategoryL(*category); 
        CleanupStack::PopAndDestroy(category); 
        }

    CleanupStack::PopAndDestroy(2, lmd);
    }

LOCAL_C TInt CreateEventsThreadFunction(TAny* aData)
    {    
    CTrapCleanup* cleanup=CTrapCleanup::New(); 
    
    CActiveScheduler* actSch = new (ELeave) CActiveScheduler;
    CActiveScheduler::Install(actSch);

    TRAPD(err, CPosTp39::RunEventsGeneratorL(aData));
       
    delete actSch;
    delete cleanup;
    return err;
    }

// ---------------------------------------------------------
// CPosTp39::CreateThreadsL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp39::CreateThreadsL()
    {
    _LIT(KThreadName, "TP39 thread%d");
    _LIT(KCreateThreadErr, "Create thread failed with %d");

    for (TInt i=0; i<=KNoListeners; i++)
        {
        RThread thread;
        TBuf<32> name;
	    name.Format(KThreadName, 1+iThreads.Count());
                
        TInt err;
        if (i==KNoListeners)
            {
            err = thread.Create(name, CreateEventsThreadFunction, KDefaultStackSize, KMinHeapSize, KMaxHeapSize, reinterpret_cast<TAny*>(this));
            }
        else
            {
            err = thread.Create(name, ReceiveEventsThreadFunction, KDefaultStackSize, KMinHeapSize, KMaxHeapSize, reinterpret_cast<TAny*>(this));
            }

        AssertTrueSecL(err == KErrNone, KCreateThreadErr, err);                                                                  
        
        iThreads.Append(thread);
        }
    }

// ---------------------------------------------------------
// CPosTp39::LogError
//
// (other items were commented in a header).
// ---------------------------------------------------------
//     
void CPosTp39::LogError(const TDesC& aError)
    {
    iErrorLog.Append(aError);
    iErrors++;
    }

// ---------------------------------------------------------
// CEventObserverTp39::CheckEventL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp39::CheckEvent(const TInt aThreadIndex,
                          const TPosLmEventType  aExpectedEventType,
	                      const TPosLmEventType  aEventType,
	                      const TPosLmItemId aExpectedItemId,
	                      const TPosLmItemId aItemId)
	{
	if (aExpectedEventType != aEventType)
		{
		_LIT(KError,"Thread %d received an unexpected eventtype. Excpected %d got %d\r\n");
		TBuf<200> error;
		error.Format(KError, aThreadIndex, aExpectedEventType, aEventType);
		LogError(error);
		}
	if (aExpectedItemId != aItemId)
		{
		_LIT(KError,"Thread %d received an unexpected item id. Excpected %d got %d\r\n");
		TBuf<200> error;
		error.Format(KError, aThreadIndex, aExpectedItemId, aItemId);
		LogError(error);
		}
    }

// ---------------------------------------------------------
// CPosTp39::GetHeapSizeL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CPosTp39::GetHeapSizeL(const TDesC& aThreadName, TInt& aHeapSize, TInt& aStackSize)
    {
    RThread thread;
    TFindThread findThread(aThreadName);

    TFullName fullName;
    
    TInt err = findThread.Next(fullName);
     
    if (err)
        {
        iLog->Put(_L("Thread not found"));
        return err;
        }
    err = thread.Open(fullName);
    if (err)
        {
        iLog->Put(_L("Thread could not be opened"));
        return err;
        }
        
    // FrBo Commented since not supported on ARMV5 (on SDK Series60_30_2005_wk02_incl_LBSpre4)
    //err = thread.GetRamSizes(aHeapSize, aStackSize);
    // Use below for now
    err =KErrNone;aHeapSize=-1;aStackSize=-1;

    if (err)
        {
        iLog->Put(_L("GetRameSize fails"));
        thread.Close();
        return err;
        }
    
    return KErrNone;
    }
    
// -----------------------------------------------------------------------------
// CEventObserverTp39::NewL
//
//(other items were commented in a header).
// -----------------------------------------------------------------------------
//
CEventObserverTp39* CEventObserverTp39::NewL(CPosTp39* aTp39, TInt aThreadIndex)
    {
    CEventObserverTp39* self = new(ELeave) CEventObserverTp39(aTp39, aThreadIndex);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// -----------------------------------------------------------------------------
// CEventObserverTp39::ConstructL
//
//(other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CEventObserverTp39::ConstructL()
    {
    iDatabase = CPosLandmarkDatabase::OpenL();
    iPeriodicTimer = CPeriodic::NewL(EPriorityNormal);

    if (iDatabase->IsInitializingNeeded())
       {
       ExecuteAndDeleteLD(iDatabase->InitializeL()); 
       }
    }

// C++ Constructor
CEventObserverTp39::CEventObserverTp39(CPosTp39* aTp39, TInt aThreadIndex) :
    CActive(EPriorityNormal),
    iTp39(aTp39),
    iThreadIndex(aThreadIndex), 
    iCallback(StopEventNotification, this)
    {
    CActiveScheduler::Add(this);
    }

// C++ destructor 
CEventObserverTp39::~CEventObserverTp39() 
    {
    Cancel();
    delete iDatabase;
    iDatabase = NULL;
    if (iPeriodicTimer)
        {
        iPeriodicTimer->Cancel();
        }
    delete iPeriodicTimer;
    iPeriodicTimer = NULL;
    }

// ---------------------------------------------------------
// CEventObserverTp39::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CEventObserverTp39::Start()                          
    {
    iDatabase->NotifyDatabaseEvent(iEvent, iStatus); 
    SetActive();
    }

//---------------------------------------------------------
// CEventObserverTp39::DoCancel
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CEventObserverTp39::DoCancel()
    {
    iDatabase->CancelNotifyDatabaseEvent();
    }
   
//---------------------------------------------------------
// CEventObserverTp39::StartTimer
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CEventObserverTp39::StartTimer()
    {
    iPeriodicTimer->Start(10000000, 10000000, TCallBack(StopEventNotification, this));
    }                                                   

//---------------------------------------------------------
// CPosEventObserverTp39::TimeoutCheck
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CEventObserverTp39::StopEventNotification(TAny* aSelf)
    {
    CEventObserverTp39* self = reinterpret_cast<CEventObserverTp39*>(aSelf);
    
    self->Cancel();
    self->iPeriodicTimer->Cancel();
    CActiveScheduler::Stop();
    
    return 0;
    }

// ---------------------------------------------------------
// CPosEventObserver::RunL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CEventObserverTp39::RunL()
    {
    iEventNumber++;
    
	switch (iEventNumber)
		{
		case 1:
            iTp39->CheckEvent(iThreadIndex,
                              EPosLmEventLandmarkCreated, 
                              iEvent.iEventType, 
                              iEventNumber, 
                              iEvent.iLandmarkItemId);
            // Don't renew the event notification so the server will have to start queueing the events                     
			break;
        default: 
            iTp39->LogError(_L("Test logic error, no more events should be received since the queue on the server should be filled")); 
            iPeriodicTimer->Cancel();
            Cancel();
            CActiveScheduler::Stop();
            break;
        }
     //iDatabase->NotifyDatabaseEvent(iEvent, iStatus); 
    //SetActive();
    }

//  End of File
