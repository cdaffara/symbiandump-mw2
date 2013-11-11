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
#include "FT_CPosTp28.h"
#include "FT_LandmarkConstants.h"

// CONSTANTS
const TInt KMaxQueueSize = 5;
_LIT(KTp28TestDb, "Tp28Test.ldb");
_LIT(KTp28LmFullDefaultDbPath, "c:\\private\\100012a5\\DBS_101FE978_TP28TEST.LDB");

// ---------------------------------------------------------
// CPosTp28::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp28::CloseTest()
    {
	delete iDatabase;
	iDatabase = NULL;
    delete iCategoryManager;
    iCategoryManager=NULL;
    delete iActiveLmEvent;
    iActiveLmEvent = NULL;
    }

// ---------------------------------------------------------
// CPosTp28::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp28::StartL()
    {
	// copy the test db to the private database path
	
    _LIT(KTestPath, "c:\\system\\test\\testdata\\");

    CFileMan* fileMan = CFileMan::NewL(iFileSession);
    CleanupStack::PushL(fileMan);

    TBuf<150> srcPath;

    srcPath.Append(KTestPath);
    srcPath.Append(KTp28TestDb);

    TInt err = fileMan->Copy(srcPath, KTp28LmFullDefaultDbPath,
            CFileMan::EOverWrite);
    if (err != KErrNone)
        iLog->Log(_L("Error when copying file"));

    CleanupStack::PopAndDestroy(fileMan);
    
    CPosLmDatabaseManager* manager = CPosLmDatabaseManager::NewL();
    CleanupStack::PushL(manager);

    // This db must exist
    manager->SetDefaultDatabaseUriL(KTp28TestDb);
    CleanupStack::PopAndDestroy(manager);

    iDatabase = CPosLandmarkDatabase::OpenL(KTp28TestDb);
    if (iDatabase->IsInitializingNeeded())
        {
        TRAPD( err, ExecuteAndDeleteLD( iDatabase->InitializeL() ) );
        AssertTrueSecL(err == KErrNone, _L("Init db failed"));
        }



    iActiveLmEvent = CActiveLmEvent::NewL(iLog);
	
    LandmarkEventTest();
    
    delete iActiveLmEvent;
    iActiveLmEvent = NULL;
    
    delete iDatabase;
    iDatabase = NULL;
    
    iDatabase = CPosLandmarkDatabase::OpenL(KTp28TestDb);   
    if (iDatabase->IsInitializingNeeded())
       {
       ExecuteAndDeleteLD(iDatabase->InitializeL());
       }

    iCategoryManager = CPosLmCategoryManager::NewL(*iDatabase); 
    
    iActiveLmEvent = CActiveLmEvent::NewL(iLog);

    LandmarkEventQueueTest();

    if (iActiveLmEvent->iGlobalErr != KErrNone)
        {
        
        iLog->Log(_L("Errors found in TP28"));
       	User::Leave(-1);
            
        }
    }

// ---------------------------------------------------------
// CPosTp28::LandmarkEventTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp28::LandmarkEventTest()
    {
   
    iActiveLmEvent->Start();
    
    /////////////////////////////////////////////
	// Add three landmarks #1 #2 #3
	/////////////////////////////////////////////
	CPosLandmark* landmark = CPosLandmark::NewLC();
	landmark->SetLandmarkNameL(_L("Zu Hause"));
	CPosLandmark* landmark2 = CPosLandmark::NewLC();
	landmark->SetLandmarkNameL(_L("Zu Hause jetzt"));
	CPosLandmark* landmark3 = CPosLandmark::NewLC();
	landmark->SetLandmarkNameL(_L("Nicht Zu Hause"));

    iActiveLmEvent->iLandmarkId = iDatabase->AddLandmarkL(*landmark);
	iActiveLmEvent->iLandmarkId2 = iDatabase->AddLandmarkL(*landmark2);
	iActiveLmEvent->iLandmarkId3 = iDatabase->AddLandmarkL(*landmark3);
	
	CleanupStack::PopAndDestroy(landmark3);
	CleanupStack::PopAndDestroy(landmark2);
	
	/////////////////////////////////////////////
	// Update landmark #4
	/////////////////////////////////////////////
	landmark->SetLandmarkNameL(_L("Nicht zu Hause"));
	iDatabase->UpdateLandmarkL(*landmark);

	/////////////////////////////////////////////
	// Remove landmark #5
	/////////////////////////////////////////////
	iDatabase->RemoveLandmarkL(iActiveLmEvent->iLandmarkId);
	
    CActiveScheduler::Start();

	/////////////////////////////////////////////
	// Add removed landmark again #6
	/////////////////////////////////////////////
	iActiveLmEvent->iLandmarkId4 = iDatabase->AddLandmarkL(*landmark);
	CleanupStack::PopAndDestroy(landmark);

    CActiveScheduler::Start();
	/////////////////////////////////////////////
	// Remove a set of landmarks sync  #7
	/////////////////////////////////////////////
	RArray<TPosLmItemId> array;
    CleanupClosePushL(array);    
    User::LeaveIfError(array.Append(iActiveLmEvent->iLandmarkId));
	User::LeaveIfError(array.Append(iActiveLmEvent->iLandmarkId2));
	User::LeaveIfError(array.Append(iActiveLmEvent->iLandmarkId3));
    ExecuteAndDeleteLD(iDatabase->RemoveLandmarksL(array));
    
	//array.Reset();
	
	CleanupStack::PopAndDestroy(&array); 
    
    CActiveScheduler::Start();
	/////////////////////////////////////////////
	// Add new category #8
	/////////////////////////////////////////////
	CPosLmCategoryManager* categoryManager = CPosLmCategoryManager::NewL(*iDatabase); 
    CleanupStack::PushL(categoryManager);
    
    CPosLandmarkCategory* category = CPosLandmarkCategory::NewLC();
    category->SetCategoryNameL(_L("My Category"));
    
    iActiveLmEvent->iCategoryId = categoryManager->AddCategoryL(*category); 
	
    AssertTrueSecL(iActiveLmEvent->iCategoryId == category->CategoryId(), _L("CATEGORY ERROR"));
    
	/////////////////////////////////////////////
	// Update category #9
	/////////////////////////////////////////////
	categoryManager->UpdateCategoryL(*category);

	/////////////////////////////////////////////
	// Delete a non-existing category #10
	/////////////////////////////////////////////
	iActiveLmEvent->iNonExistingCategoryId = 998;
	ExecuteAndDeleteLD(categoryManager->RemoveCategoryL(iActiveLmEvent->iNonExistingCategoryId));
	
    /////////////////////////////////////////////
	// Delete a category that exists in several landmarks #11
	/////////////////////////////////////////////
	ExecuteAndDeleteLD(categoryManager->RemoveCategoryL(iActiveLmEvent->iDeleteCategoryId));
	
    CActiveScheduler::Start();
	/////////////////////////////////////////////
	// Delete several categories
	/////////////////////////////////////////////
	CPosLmItemIterator* iter = categoryManager->CategoryIteratorL();
    CleanupStack::PushL(iter);
	const TInt startIndex=1;
    const TInt numOfItems=5;
    RArray<TPosLmItemId> ids;
    CleanupClosePushL(ids);
    iter->GetItemIdsL(ids, startIndex, numOfItems);    
	ExecuteAndDeleteLD(categoryManager->RemoveCategoriesL(ids));
	
	CleanupStack::PopAndDestroy(2, iter); //ids
    
    CActiveScheduler::Start();
	/////////////////////////////////////////////
	// Add new category to several landmarks
	/////////////////////////////////////////////
	CPosLandmarkCategory* newCategory = CPosLandmarkCategory::NewLC();
    newCategory->SetCategoryNameL(_L("My New Category"));
    iActiveLmEvent->iCategoryId2= categoryManager->AddCategoryL(*newCategory); 
	CleanupStack::PopAndDestroy(newCategory);
	
	iter = iDatabase->LandmarkIteratorL();
	CleanupStack::PushL(iter);
	const TInt addStartIndex=5;
    const TInt addNumOfItems=7;
    CleanupClosePushL(ids);
    iter->GetItemIdsL(ids, addStartIndex, addNumOfItems);   

	ExecuteAndDeleteLD(categoryManager->AddCategoryToLandmarksL(iActiveLmEvent->iCategoryId2, ids));
    
    CActiveScheduler::Start();
    /////////////////////////////////////////////
	// Delete a category in several landmarks
	/////////////////////////////////////////////
	ExecuteAndDeleteLD(categoryManager->RemoveCategoryFromLandmarksL(iActiveLmEvent->iCategoryId2, ids));

	CleanupStack::PopAndDestroy(4, categoryManager); // iter, ids, category

     CActiveScheduler::Start();
	/////////////////////////////////////////////
	// Remove all landmarks
	/////////////////////////////////////////////
	ExecuteAndDeleteLD(iDatabase->RemoveAllLandmarksL());

	CActiveScheduler::Start(); // For notifier
	
    }

// ---------------------------------------------------------
// CPosTp28::LandmarkEventQueueTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp28::LandmarkEventQueueTest()
    {
    
    TInt i=0;
    iActiveLmEvent->iQueueTest = ETrue;
    iActiveLmEvent->Start();

    /////////////////////////////////////////////
	// Add one landmark #1
    //////////////////////////////////////////////
  
    CPosLandmark* landmark = CPosLandmark::NewLC();
    landmark->SetLandmarkNameL(_L("TP28 event queue"));  
    iActiveLmEvent->iLandmarkId = iDatabase->AddLandmarkL(*landmark);
    CleanupStack::PopAndDestroy(landmark);
    CActiveScheduler::Start();

    /////////////////////////////////////////////
	// Add 6 landmarks #2
    /////////////////////////////////////////////
    _LIT(KLmName, "LmTP28");
    for (i=0; i <= KMaxQueueSize; i++)
        {
        CPosLandmark* landmark = CPosLandmark::NewLC();
        landmark->SetLandmarkNameL(KLmName);  
        iDatabase->AddLandmarkL(*landmark);
        CleanupStack::PopAndDestroy(landmark); 
        }
    
    iActiveLmEvent->Start();
    CActiveScheduler::Start();
 
    /////////////////////////////////////////////
	// Updates landmark 6 times #3
    /////////////////////////////////////////////
    for (i=0; i <= KMaxQueueSize; i++)
        {
        CPosLandmark* landmark = iDatabase->ReadLandmarkLC(iActiveLmEvent->iLandmarkId);   
        iDatabase->UpdateLandmarkL(*landmark);
        CleanupStack::PopAndDestroy(landmark);
        }
    
    iActiveLmEvent->Start();
    CActiveScheduler::Start();
    
    /////////////////////////////////////////////
	// Add 3 landmarks and 3 categories #4, #5 #6 #7 
    /////////////////////////////////////////////
    
    for (i=0; i < 3; i++)
        {
        CPosLandmark* landmark = CPosLandmark::NewLC();
        landmark->SetLandmarkNameL(KLmName);  
        iDatabase->AddLandmarkL(*landmark);
        CleanupStack::PopAndDestroy(landmark); 
        }
    
    _LIT(KCategoryName, "TP28 Category %d");
    for (i =0; i < 3; i++)
        {
        TBuf<100> name;
        name.Format(KCategoryName, i);
        CPosLandmarkCategory* category = CPosLandmarkCategory::NewLC();
        category->SetCategoryNameL(name);
        if (i == 0)
            {
            iActiveLmEvent->iCategoryId = iCategoryManager->AddCategoryL(*category); 
            }
        else
            {
            iCategoryManager->AddCategoryL(*category); 
            }
        CleanupStack::PopAndDestroy(category); 
        }
    
   
    iActiveLmEvent->Start();
    CActiveScheduler::Start();
  
    }


// -----------------------------------------------------------------------------
// CActiveLmEvent::NewL
//
//(other items were commented in a header).
// -----------------------------------------------------------------------------
//
CActiveLmEvent* CActiveLmEvent::NewL(CStifLogger* aLog)
    {
    CActiveLmEvent* self = new(ELeave) CActiveLmEvent(aLog);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// -----------------------------------------------------------------------------
// CActiveLmEvent::ConstructL
//
//(other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CActiveLmEvent::ConstructL()
    {
    iDatabase = CPosLandmarkDatabase::OpenL();
    }

// -----------------------------------------------------------------------------
// CActiveLmEvent::CActiveLmEvent
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//

CActiveLmEvent::CActiveLmEvent(CStifLogger* aLog) : 
    CActive(EPriorityNormal),
    iLog(aLog)
    {   
    CActiveScheduler::Add(this);
    }

// Destructor
CActiveLmEvent::~CActiveLmEvent()
    {
    Cancel();
    delete iDatabase;
    iDatabase = NULL;
    }

// ---------------------------------------------------------
// CActiveLmEvent::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CActiveLmEvent::Start()                          
    {
    iDatabase->NotifyDatabaseEvent(iEvent, iStatus); 
 
    SetActive();
    }   

// ---------------------------------------------------------
// CActiveLmEvent::RunL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CActiveLmEvent::RunL()
    {  
    
    if (iQueueTest)
        {
        VerifyMergedEvent();
        }
    else
        {
        VerifyEvent();
        }

    if (!iQueueTest)
        {
        iDatabase->NotifyDatabaseEvent(iEvent, iStatus); 
        SetActive();
        }
    }

// ---------------------------------------------------------
// CActiveLmOperation::DoCancel
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CActiveLmEvent::DoCancel()
    {
    iDatabase->CancelNotifyDatabaseEvent();
    }

// ---------------------------------------------------------
// CActiveLmEvent::VerifyEvent
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CActiveLmEvent::VerifyEvent()
    {
   
    iEventNumber++;
	switch (iEventNumber)
		{
					// Added three landmarks
		case 1:		AssertCorrectEvent(EPosLmEventLandmarkCreated, iEvent.iEventType, iLandmarkId, iEvent.iLandmarkItemId); 
					break;
		case 2:		AssertCorrectEvent(EPosLmEventLandmarkCreated, iEvent.iEventType, iLandmarkId2, iEvent.iLandmarkItemId); 
					break;
		case 3:		AssertCorrectEvent(EPosLmEventLandmarkCreated, iEvent.iEventType, iLandmarkId3, iEvent.iLandmarkItemId); 
					break;
					// Updated landmark
		case 4:		AssertCorrectEvent(EPosLmEventLandmarkUpdated, iEvent.iEventType, iLandmarkId, iEvent.iLandmarkItemId); 
					break;
					// Deleted landmark
		case 5:		AssertCorrectEvent(EPosLmEventLandmarkDeleted, iEvent.iEventType, iLandmarkId, iEvent.iLandmarkItemId); 
				    CActiveScheduler::Stop();
                    break;
					// Added removed landmark again
		case 6:		AssertCorrectEvent(EPosLmEventLandmarkCreated, iEvent.iEventType, iLandmarkId4, iEvent.iLandmarkItemId); 
                    CActiveScheduler::Stop();
					break;
					// Deleted several landmarks
		case 7:		AssertCorrectEvent(EPosLmEventLandmarkUnknownChanges, iEvent.iEventType, 0, iEvent.iLandmarkItemId); 
					CActiveScheduler::Stop();
                    break;
					// Added new category
		case 8:		AssertCorrectEvent(EPosLmEventCategoryCreated, iEvent.iEventType, iCategoryId, iEvent.iLandmarkItemId); 
					break;
					//Updated category
		case 9:		AssertCorrectEvent(EPosLmEventCategoryUpdated, iEvent.iEventType, iCategoryId, iEvent.iLandmarkItemId); 
					break;
					// Deleted non existing category
		case 10:	AssertCorrectEvent(EPosLmEventCategoryDeleted, iEvent.iEventType, iNonExistingCategoryId, iEvent.iLandmarkItemId); 
					break;
					// Deleted existing category
		case 11:	AssertCorrectEvent(EPosLmEventCategoryDeleted, iEvent.iEventType, iDeleteCategoryId, iEvent.iLandmarkItemId); 
					CActiveScheduler::Stop();
                    break;
					// Deleted several categories
		case 12:	AssertCorrectEvent(EPosLmEventCategoryUnknownChanges, iEvent.iEventType, 0, iEvent.iLandmarkItemId); 
					CActiveScheduler::Stop();
                    break;
					// Added new category
		case 13:	AssertCorrectEvent(EPosLmEventCategoryCreated, iEvent.iEventType, iCategoryId2, iEvent.iLandmarkItemId); 
					break;
					// Added created category to several landmarks
		case 14:	AssertCorrectEvent(EPosLmEventLandmarkUnknownChanges, iEvent.iEventType, 0, iEvent.iLandmarkItemId); 
					CActiveScheduler::Stop();
                    break;
					// Removed category from several landmarks
		case 15:	AssertCorrectEvent(EPosLmEventLandmarkUnknownChanges, iEvent.iEventType, 0, iEvent.iLandmarkItemId); 
					CActiveScheduler::Stop();
                    break;
					// Removed all landmarks
		case 16:	AssertCorrectEvent(EPosLmEventLandmarkUnknownChanges, iEvent.iEventType, 0, iEvent.iLandmarkItemId); 
					CActiveScheduler::Stop();				
					break;
		default: 
            iLog->Log(_L("Unexpected event received"));
            ++iGlobalErr;
            break;
        }
    	
    }

//---------------------------------------------------------
// CActiveLmEvent::VerifyMergedEvent
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CActiveLmEvent::VerifyMergedEvent()
    {
    ++iMergedEventNumber;    
    switch (iMergedEventNumber)
		{
        case 1:
            AssertCorrectEvent(
                            EPosLmEventLandmarkCreated, 
                            iEvent.iEventType, 
                            iLandmarkId, 
                            iEvent.iLandmarkItemId);
            
            CActiveScheduler::Stop();
			break;
		case 2:
        case 3:
            AssertCorrectEvent(
                            EPosLmEventLandmarkUnknownChanges, 
                            iEvent.iEventType, 
                            0, 
                            iEvent.iLandmarkItemId); 
            
            CActiveScheduler::Stop();
			break;
        case 4:
            AssertCorrectEvent(
                            EPosLmEventLandmarkUnknownChanges, 
                            iEvent.iEventType, 
                            0, 
                            iEvent.iLandmarkItemId); 
            Start();
            break;
        case 5:
        case 6:
        case 7:
            AssertCorrectEvent(EPosLmEventCategoryCreated,
                            iEvent.iEventType, 
                            iCategoryId++, 
                            iEvent.iLandmarkItemId);
           
            if (iMergedEventNumber == 7)   
                {
                CActiveScheduler::Stop();
                }
            else
                {
                Start();
                }
			break;
        default: 
            _LIT(KError,"Unexpected event received, %d\r\n");
		    TBuf<200> error;
		    error.Format(KError, iEvent.iEventType);
		    iLog->Log(error);
		    iGlobalErr++;
            Start();
            break;
        }
    }

// ---------------------------------------------------------
// CActiveLmEvent::AssertCorrectEvent
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CActiveLmEvent::AssertCorrectEvent(
	TPosLmEventType  aExpectedEventType,
	TPosLmEventType  aEventType,
	TPosLmItemId aExpectedLandMarkItemId,
	TPosLmItemId aLandMarkItemId)
	{
	if (aExpectedEventType != aEventType)
		{
		_LIT(KError,"Unexpected eventtype. Excpected %d got %d\r\n");
		TBuf<200> error;
		error.Format(KError, aExpectedEventType, aEventType);
		iLog->Log(error);
		iGlobalErr++;
		}
	if (aExpectedLandMarkItemId != aLandMarkItemId)
		{
		_LIT(KError,"Unexpected item id. Excpected %d got %d\r\n");
		TBuf<200> error;
		error.Format(KError, aExpectedLandMarkItemId, aLandMarkItemId);
		iLog->Log(error);
		iGlobalErr++;
		}

	}


// End of file

