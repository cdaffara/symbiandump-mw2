/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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
#include "FT_CPosTp27.h"
#include <EPos_CPosLmTextCriteria.h>
#include <EPos_CPosLMItemIterator.h>
#include <EPos_CPosLmCatNameCriteria.h>
#include <EPos_CPosLmCategoryCriteria.h>

// CONSTANTS

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CPosTp27::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp27::CloseTest()
    {
    delete iPeriodicTimer;
    iPeriodicTimer = NULL;

    delete iDatabase;
    iDatabase = NULL;
    }

// ---------------------------------------------------------
// CPosTp27::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp27::StartL()
    {
    iDatabase = UseGeneratedDbFileL();   

    if (iDatabase->IsInitializingNeeded())
       {
       CPosLmOperation* op = iDatabase->InitializeL();
       delete op;
       }

    CancelLandmarkCategorySearch();

    CancelLandmarkSearch();

    AssertTrueSecL(iGlobalErr != KErrNone, _L("Errors found in TP27"));
    }

// ---------------------------------------------------------
// CPosTp27::CancelLandmarkCategorySearch
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp27::CancelLandmarkCategorySearch()
    {
    iLog->Log(_L("CancelLandmarkCategorySearch"));
    CPosLmCatNameCriteria* searchCriteria = CPosLmCatNameCriteria::NewLC();

    // Use name specified in test landmarksdb
    searchCriteria->SetSearchPatternL(_L("Taxi"));       

    iLandmarkSearch = CPosLandmarkSearch::NewL(*iDatabase, this);
    // Get maximum number of search attempts
    iLandmarkSearch->StartCategorySearchL(*searchCriteria, CPosLmCategoryManager::ECategorySortOrderNone);

    iHandleOperationDisabled = EFalse;
    iActiveSchedulerWait->Start();
    iHandleOperationDisabled = ETrue;
    // Get the maximum of needed search rounds until search is complete
    TInt MaxNrOfSearchRounds = iNrSearchRounds;

    // First test to cancel the search before it even has started
    // use the callback StopActiveSchedulerWait to stop the active scheduler
    // just to make sure that no HandleOperationProgress take place
    iNrSearchRounds = 0;
    iLandmarkSearch->StartCategorySearchL(*searchCriteria, CPosLmCategoryManager::ECategorySortOrderNone);
    iLandmarkSearch->Cancel();

    TCallBack callback(StopActiveSchedulerWait, this);
    iPeriodicTimer = CPeriodic::NewL(CActive::EPriorityStandard);
    TTimeIntervalMicroSeconds32 stopTime = TTimeIntervalMicroSeconds32(4000000); // 4 sec
    iPeriodicTimer->Start(stopTime, stopTime, callback);
    iHandleOperationDisabled = ETrue;
    iActiveSchedulerWait->Start();

    delete iPeriodicTimer;
    iPeriodicTimer = NULL;

    iLog->Log(_L("Starting"));
    TInt i=0;
    for (i=0; i<=MaxNrOfSearchRounds; i++)
        {
        iNrSearchRounds = 0;
        iBreakSearch = i;
        // Set to 100 so that it differs from progress the first time
        iProgress=100;
        iLog->Log(_L("StartCategorySearchL->"));
        iLandmarkSearch->StartCategorySearchL(*searchCriteria, CPosLmCategoryManager::ECategorySortOrderNone);
        iHandleOperationDisabled = EFalse;
        iActiveSchedulerWait->Start();

        iLog->Log(_L("After iActiveSchedulerWait->Start() inside for loop"));
        iHandleOperationDisabled = ETrue;

        // Check if anything was found
        CPosLmItemIterator* iterator = iLandmarkSearch->MatchIteratorL();
        CleanupStack::PushL(iterator);
        
        TInt numOfItems = 0;
        numOfItems = iLandmarkSearch->NumOfMatches();
       
        RArray<TPosLmItemId> ids;
        CleanupClosePushL(ids);
        
        if (numOfItems != 0)
            {
            iLog->Log(_L("Found something"));
            // Maybe we should check this item
            iterator->GetItemIdsL(ids, 0, numOfItems);
            }
        
        CleanupStack::PopAndDestroy(&ids);
        CleanupStack::PopAndDestroy(iterator);
        }
    
    iNrSearchRounds=0;
    iBreakSearch = -1;
    iCancelOrDelete = ETrue;

    delete iLandmarkSearch;
    iLandmarkSearch = NULL;

    iLog->Log(_L("This time, delete the search object to cancel the search"));
    for (i=0; i<=MaxNrOfSearchRounds; i++)
        {
        iLandmarkSearch = CPosLandmarkSearch::NewL(*iDatabase, this);   
        // Set to 100 so that it differs from progress the first time
        iProgress=100;
        iNrSearchRounds = 0;
        iBreakSearch = i;
        iLog->Log(_L("StartCategorySearchL2->"));
        iLandmarkSearch->StartCategorySearchL(*searchCriteria, CPosLmCategoryManager::ECategorySortOrderNone);
        iHandleOperationDisabled = EFalse;
        iActiveSchedulerWait->Start();
        iHandleOperationDisabled = ETrue;
        iLog->Log(_L("After iActiveSchedulerWait->Start() inside for loop"));
        }
    iHandleOperationDisabled = EFalse;
    CleanupStack::PopAndDestroy(searchCriteria);

    iCancelOrDelete = EFalse;
    delete iLandmarkSearch;
    iLandmarkSearch = NULL;
    }

// ---------------------------------------------------------
// CPosTp27::CancelLandmarkSearch
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp27::CancelLandmarkSearch()
    {
    iLog->Log(_L("CancelLandmarkSearch"));

    CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();
    TUint attr = CPosLandmark::ELandmarkName | CPosLandmark::EDescription;
    textCriteria->SetAttributesToSearch(attr);
    // Use name specified in test landmarksdb
    textCriteria->SetTextL(_L("enator"));
        
    iLandmarkSearch = CPosLandmarkSearch::NewL(*iDatabase, this);
    iLandmarkSearch->StartLandmarkSearchL(*textCriteria);
    iHandleOperationDisabled = EFalse;
    iActiveSchedulerWait->Start();
    iHandleOperationDisabled = ETrue;

    // Get the maximum of needed search rounds until search is complete
    TInt MaxNrOfSearchRounds = iNrSearchRounds;

    // First test to cancel the search before it even has started
    // use the callback StopActiveSchedulerWait to stop the active scheduler
    // just to make sure that no HandleOperationProgress take place
    iNrSearchRounds = 0;
    iLandmarkSearch->StartLandmarkSearchL(*textCriteria);
    iLandmarkSearch->Cancel();

    TCallBack callback(StopActiveSchedulerWait, this);
    iPeriodicTimer = CPeriodic::NewL(CActive::EPriorityStandard);
    TTimeIntervalMicroSeconds32 stopTime = TTimeIntervalMicroSeconds32(4000000); // 4 sec
    iPeriodicTimer->Start(stopTime, stopTime, callback);
    iHandleOperationDisabled = ETrue;
    iActiveSchedulerWait->Start();

    delete iPeriodicTimer;
    iPeriodicTimer = NULL;

    iLog->Log(_L("Starting"));
    TInt i=0;
    for (i=0; i<=MaxNrOfSearchRounds; i++)
        {
        iNrSearchRounds = 0;
        iBreakSearch = i;
        // Set to 100 so that it differs from progress the first time
        iProgress=100;
        iLog->Log(_L("StartLandmarkSearchL->"));
        iLandmarkSearch->StartLandmarkSearchL(*textCriteria);
        iHandleOperationDisabled = EFalse;
        iActiveSchedulerWait->Start();

        iLog->Log(_L("After iActiveSchedulerWait->Start() inside for loop"));
        iHandleOperationDisabled = ETrue;

        // Check if anything was found
        CPosLmItemIterator* iterator = iLandmarkSearch->MatchIteratorL();
        CleanupStack::PushL(iterator);
        
        TInt numOfItems = 0;
        numOfItems = iLandmarkSearch->NumOfMatches();
       
        RArray<TPosLmItemId> ids;
        CleanupClosePushL(ids);
        
        if (numOfItems != 0)
            {
            iLog->Log(_L("Found something"));
            iterator->GetItemIdsL(ids, 0, numOfItems);
            }
        
        CleanupStack::PopAndDestroy(&ids);
        CleanupStack::PopAndDestroy(iterator);
        }
    
    iNrSearchRounds=0;
    iBreakSearch = -1;
    iCancelOrDelete = ETrue;

    delete iLandmarkSearch;
    iLandmarkSearch = NULL;

    iLog->Log(_L("This time, delete the search object to cancel the search"));
    for (i=0; i<=MaxNrOfSearchRounds; i++)
        {
        iLandmarkSearch = CPosLandmarkSearch::NewL(*iDatabase, this);   
        // Set to 100 so that it differs from progress the first time
        iProgress=100;
        iNrSearchRounds = 0;
        iBreakSearch = i;
        iLog->Log(_L("StartLandmarkSearchL2->"));
        iLandmarkSearch->StartLandmarkSearchL(*textCriteria);
        iHandleOperationDisabled = EFalse;
        iActiveSchedulerWait->Start();
        iHandleOperationDisabled = ETrue;
        iLog->Log(_L("After iActiveSchedulerWait->Start() inside for loop"));
        }
    iHandleOperationDisabled = EFalse;
    CleanupStack::PopAndDestroy(textCriteria);

    iCancelOrDelete = EFalse;

    delete iLandmarkSearch;
    iLandmarkSearch = NULL;    
    }


// ---------------------------------------------------------
// CPosTp27::StopActiveSchedulerWait
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CPosTp27::StopActiveSchedulerWait(TAny* aSelf)
    {
    //do not forget to cancel the timer
    CPosTp27* self = static_cast<CPosTp27*> (aSelf);
    self->iPeriodicTimer->Cancel();
    self->iActiveSchedulerWait->AsyncStop();
    return KErrNone;
    }

// ---------------------------------------------------------
// CPosTp27::HandleOperationProgress
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp27::HandleOperationProgress(CPosLmOperation*  aOperation)
    {
    //if (iDebug) iLog->Log(_L("HandleOperationProgress"));

    // If no HandleOperationProgress should occur, eg no search is started
    if (iHandleOperationDisabled)
        {
        iLog->Log(_L("ERROR: No HandleOperationProgress should occur"));
        iGlobalErr++;
        }
    else
    {
    // If progress is 100%, always stop the search
    // Check that progress is moving
    TReal32 progress = aOperation->Progress();
    if (iProgress == progress)
        {
        iLog->Log(_L("MyError:Progressindicator not moving"));
        iGlobalErr++;
        }
    iProgress = progress;
    TBuf<50> buf;
    buf.Format(_L("****Progress %f"), progress);
    if (iDebug) iLog->Log(buf);

    // Not sure about this condition, is it ok to compare TReal32 like this
    if (progress == 1)
        {
        if (iDebug) iLog->Log(_L("Stopping"));
        iActiveSchedulerWait->AsyncStop();
        return;
        }

    if (iNrSearchRounds == iBreakSearch)
        {
        if (!iCancelOrDelete)
            {
            if (iDebug) iLog->Log(_L("Cancelling"));
            aOperation->Cancel();
            iActiveSchedulerWait->AsyncStop();
            if (aOperation->Status() != KErrCancel)
                {
                TBuf<50> buf;
                buf.Format(_L("MyError: Wrong status, should be KErrCancel, but was %d"), aOperation->Status());
                iLog->Log(buf);
                iGlobalErr++;
                }
            }
        else
            {
            if (iDebug) iLog->Log(_L("Cancel by deleting search object"));
            delete iLandmarkSearch;
            iLandmarkSearch = NULL;
            iActiveSchedulerWait->AsyncStop();
            }
        }
        iNrSearchRounds++;
    }
    }
// End of file

