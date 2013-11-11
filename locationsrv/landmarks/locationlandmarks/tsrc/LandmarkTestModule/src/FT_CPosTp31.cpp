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

#include "FT_CPosTp31.h"
#include <EPos_CPosLandmarkDatabase.h> 
#include <EPos_CPosLandmark.h> 
#include <EPos_CPosLMCategoryManager.h>
#include <EPos_CPosLandmarkCategory.h>  
#include <e32std.h>
#include <ss_std.h>

//  CONSTANTS
const TInt KNoListeners=2;

const TInt KNoLandmarks = 10; 
const TInt KNoCategories = 10;

const TInt KCancelledThreadIndex = 2;

const TInt KNoEvents = 25;

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CPosTp31::GetName
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp31::GetName(TDes& aName) const
    {
    _LIT(KTestName, "TP31 - Multiple event listeners");
    aName = KTestName;
    
    }

// ---------------------------------------------------------
// CPosTp31::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp31::StartL()
    {  
    _LIT(KMultipleErr, "Error %d when multiple clients listen for events");
    _LIT(KEventErr, "%d Error(s) when multiple clients listen for events");
    _LIT(KExitErr, "Thread was panicked or is still alive");
    iUseLogFromThreadIsDisabled = ETrue;
    MakeSurePanicDebugFileExistsL();
    RemoveDefaultDbL();
    RemoveAllLmDatabasesL();

    //We should use an empty database for this test case
    CPosLandmarkDatabase* database = UseEmptyDbFileL();
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
        AssertTrueSecL(iThreads[t].ExitType() == EExitKill, KExitErr);
        }
    
    if (iErrors > 0)
        {
        TBuf<100> info;
	    info.Format(KEventErr, iErrors);
        LogErrorAndLeave(iErrorLog);
        }
    }

// ---------------------------------------------------------
// CPosTp31::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp31::CloseTest()
    {
    for (TInt i=0; i<iThreads.Count(); i++)
        {
        iThreads[i].Close();
        }

    iThreads.Close();
    iUseLogFromThreadIsDisabled = EFalse;
    }

// ---------------------------------------------------------
// CPosTp31::RunReceiveEventTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp31::RunReceiveEventsTestL(TAny* aData)
    {
    CPosTp31* self = reinterpret_cast<CPosTp31*>(aData);

    RDebug::Print(_L(">>>>>>>>>RunReceiveEventsTestL<<<<<<<<<"));

    CEventObserver* eventObserver = CEventObserver::NewL(self, ++self->iThreadIndex);
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

    TRAPD(err, CPosTp31::RunReceiveEventsTestL(aData));
    
    delete actSch;
    delete cleanup;
    return err;
    }

// ---------------------------------------------------------
// CPosTp31::CreateEventsTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp31::CreateEventsTestL(TAny* aData)
    {
    CPosTp31* self = reinterpret_cast<CPosTp31*>(aData);

    CPosLandmarkDatabase* lmd = CPosLandmarkDatabase::OpenL();
    CleanupStack::PushL(lmd);
    
    if (lmd->IsInitializingNeeded())
       {
       ExecuteAndDeleteLD(lmd->InitializeL());
       }
    ExecuteAndDeleteLD(lmd->CompactL());

    CPosLmCategoryManager* cm = CPosLmCategoryManager::NewL(*lmd); 
    CleanupStack::PushL(cm);

    RArray<TPosLmItemId> landmarksIds;
    CleanupClosePushL(landmarksIds);
    RDebug::Print(_L(">>>>>>>>>CreateEventsTestL<<<<<<<<<"));
    for (TInt i=0; i<KNoLandmarks; i++)
        {
        CPosLandmark* landmark = CPosLandmark::NewLC();
	    landmark->SetLandmarkNameL(_L("TP31"));
	    TPosLmItemId idde;
	    TInt err = KErrLocked;
	    while (err == KErrLocked)
	        {
	        TRAP(err, idde = lmd->AddLandmarkL(*landmark));
	        }
	    landmarksIds.Append(idde);
        CleanupStack::PopAndDestroy(landmark);
        }
    
    RArray<TPosLmItemId> catIds;
    CleanupClosePushL(catIds);

    _LIT(KCategoryName, "TP31 Category %d");
    for (TInt j =0; j < KNoCategories; j++)
        {
        TBuf<100> name;
        name.Format(KCategoryName, j);
        CPosLandmarkCategory* category = CPosLandmarkCategory::NewLC();
        category->SetCategoryNameL(name);
        TPosLmItemId idde;
        TInt err = KErrLocked;
        while (err == KErrLocked)
            {
            TRAP(err, idde = cm->AddCategoryL(*category));
            }
        catIds.Append(idde);
        CleanupStack::PopAndDestroy(category); 
        }
    ExecuteAndDeleteLD(lmd->CompactL());

    // Add categories to landmarks #21
    self->RunAsyncOperationLD(cm->AddCategoryToLandmarksL(catIds[0], landmarksIds));

    // Remove category #22
    self->RunAsyncOperationLD(cm->RemoveCategoryL(catIds[0])); 

    // Remove categories #23
    catIds.Remove(0);
    self->RunAsyncOperationLD(cm->RemoveCategoriesL(catIds)); 

    // Remove category from landmarks #24
    TRAPD(err, self->RunAsyncOperationLD(cm->RemoveCategoryFromLandmarksL(catIds[0], landmarksIds)));
    User::After(200000);

    // Remove landmarks #25
    landmarksIds.Remove(0);
    landmarksIds.Remove(1);
    self->RunAsyncOperationLD(lmd->RemoveLandmarksL(landmarksIds));
    User::After(200000);

    // Remove all landmarks #26
    self->RunAsyncOperationLD(lmd->RemoveAllLandmarksL());
    User::After(200000);

    CleanupStack::PopAndDestroy(4, lmd);
    }

LOCAL_C TInt CreateEventsThreadFunction(TAny* aData)
    {    
    CTrapCleanup* cleanup=CTrapCleanup::New(); 
    
    CActiveScheduler* actSch = new (ELeave) CActiveScheduler;
    CActiveScheduler::Install(actSch);

    TRAPD(err, CPosTp31::CreateEventsTestL(aData));
       
    delete actSch;
    delete cleanup;
    return err;
    }

// ---------------------------------------------------------
// CPosTp31::CreateThreadsL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp31::CreateThreadsL()
    {
    
    _LIT(KThreadName, "TP31 test thread%d");
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
// CPosTp31::LogError
//
// (other items were commented in a header).
// ---------------------------------------------------------
//     
void CPosTp31::LogError(const TDesC& aError)
    {
    iErrorLog.Append(aError);
    iErrors++;
    }

// ---------------------------------------------------------
// CEventObserver::CheckEventL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp31::CheckEvent(const TInt aThreadIndex,
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

// -----------------------------------------------------------------------------
// CEventObserver::NewL
//
//(other items were commented in a header).
// -----------------------------------------------------------------------------
//
CEventObserver* CEventObserver::NewL(CPosTp31* aTp31, TInt aThreadIndex)
    {
    CEventObserver* self = new(ELeave) CEventObserver(aTp31, aThreadIndex);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// -----------------------------------------------------------------------------
// CEventObserver::ConstructL
//
//(other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CEventObserver::ConstructL()
    {
    iDatabase = CPosLandmarkDatabase::OpenL();
    if (iDatabase->IsInitializingNeeded())
       {
       ExecuteAndDeleteLD(iDatabase->InitializeL()); 
       }

    iPeriodicTimer = CPeriodic::NewL(EPriorityNormal);
    }

// C++ Constructor
CEventObserver::CEventObserver(CPosTp31* aTp31, TInt aThreadIndex) :
    CActive(EPriorityNormal),
    iTp31(aTp31),
    iThreadIndex(aThreadIndex), 
    iCallback(TimeoutCheck, this)
    {
    CActiveScheduler::Add(this);
    }

// C++ destructor 
CEventObserver::~CEventObserver() 
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
// CEventObserver::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CEventObserver::Start()                          
    {
    
    iDatabase->NotifyDatabaseEvent(iEvent, iStatus); 
 
    SetActive();
    }

//---------------------------------------------------------
// CEventObserver::DoCancel
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CEventObserver::DoCancel()
    {
    iDatabase->CancelNotifyDatabaseEvent();
    }
   
//---------------------------------------------------------
// CEventObserver::StartTimer
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CEventObserver::StartTimer()
    {
    iPeriodicTimer->Start(10000000, 10000000, TCallBack(TimeoutCheck, this));
    }

//---------------------------------------------------------
// CPosEventObserver::TimeoutCheck
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CEventObserver::TimeoutCheck(TAny* aSelf)
    {
    CEventObserver* self = reinterpret_cast<CEventObserver*>(aSelf);

  
    if ((self->iEventNumber == KNoEvents && self->iThreadIndex != KCancelledThreadIndex) ||
        (self->iEventNumber == KNoEvents && self->iThreadIndex == KCancelledThreadIndex))
        {
        self->iPeriodicTimer->Cancel();
        self->Cancel();

        CActiveScheduler::Stop();
        }
    else if (self->iEventNumber != KNoEvents)
        {
        _LIT(KError,"Incorrect number of events received by Thread %d, events received %d \r\n");
		TBuf<200> error;
		error.Format(KError, self->iThreadIndex, self->iEventNumber);
		self->iTp31->LogError(error);
        
        self->iPeriodicTimer->Cancel();
        self->Cancel();
        CActiveScheduler::Stop();
        }
    
    return 0;
    }

// ---------------------------------------------------------
// CPosEventObserver::RunL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CEventObserver::RunL()
    {
    iEventNumber++;
    
    if (!(iEventNumber == KNoEvents && iThreadIndex == KCancelledThreadIndex))
        {
        if (iStatus != KErrNone)
            {
            iTp31->LogError(_L("Unexpected status in RunL"));
            }
        }
	switch (iEventNumber)
		{
		case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
            iTp31->CheckEvent(iThreadIndex,
                              EPosLmEventLandmarkCreated, 
                              iEvent.iEventType, 
                              iEventNumber, 
                              iEvent.iLandmarkItemId); 
			break;
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:
        case 18:
        case 19:
        case 20:
	        iTp31->CheckEvent(iThreadIndex,
                              EPosLmEventCategoryCreated, 
                              iEvent.iEventType, 
                              (iEventNumber-KNoLandmarks), 
                              iEvent.iLandmarkItemId);
            break;
        case 21:
        case 24:
        case 25:
        
           iTp31->CheckEvent(iThreadIndex,
                              EPosLmEventLandmarkUnknownChanges, 
                              iEvent.iEventType, 
                              0, 
                              iEvent.iLandmarkItemId); 
            break;
        case 22:
	        iTp31->CheckEvent(iThreadIndex,
                              EPosLmEventCategoryDeleted, 
                              iEvent.iEventType, 
                              1, 
                              iEvent.iLandmarkItemId); 
            break;
        case 23:
	        iTp31->CheckEvent(iThreadIndex,
                              EPosLmEventCategoryUnknownChanges, 
                              iEvent.iEventType, 
                              0, 
                              iEvent.iLandmarkItemId);        
            break;
        case 26:
            if (iThreadIndex == KCancelledThreadIndex)
                {
                if (iStatus != KErrCancel)
                    {
                    iTp31->LogError(_L("Unexpected status when cancelled"));
                    }
                }
            else
                {
                iTp31->CheckEvent(iThreadIndex,
                              EPosLmEventLandmarkUnknownChanges, 
                              iEvent.iEventType, 
                              0, 
                              iEvent.iLandmarkItemId); 
                }
            break;
        default: 
            iTp31->LogError(_L("Unexpected event received")); 
            iPeriodicTimer->Cancel();
            Cancel();
            CActiveScheduler::Stop();
            break;
        }
    
    
    if (iEventNumber == KNoEvents-1 && iThreadIndex == KCancelledThreadIndex)
        {
        iDatabase->NotifyDatabaseEvent(iEvent, iStatus); 
        iDatabase->CancelNotifyDatabaseEvent();
        
        SetActive();
        }
    else if(!(iEventNumber == KNoEvents && iThreadIndex == KCancelledThreadIndex))
        {
        iDatabase->NotifyDatabaseEvent(iEvent, iStatus); 
        SetActive();
        }
    }

//  End of File
