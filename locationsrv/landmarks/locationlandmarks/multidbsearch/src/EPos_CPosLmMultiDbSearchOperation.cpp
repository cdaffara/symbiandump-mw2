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
* Description: Operation to search for landmarks or landmark categories from 
*							 multiple databases
*
*
*/


// INCLUDE FILES
#include    <e32std.h>
#include    <e32debug.h>
#include    <EPos_Landmarks.h>
#include    <EPos_LandmarksErrors.h>
#include    <EPos_CPosLandmarkSearch.h>
#include    <EPos_CPosLmSearchCriteria.h>
#include    <EPos_CPosLmMultiDbSearch.h>

#include    "EPos_CPosLmMultiDbSearchOperation.h"
#include    "EPos_CPosLmActiveOperation.h"
#include    "EPos_PosLmCriteriaHelper.h"

// CONSTANTS
const TReal32 KPosProgressShareOpenDbs          = 0.05;
const TReal32 KPosProgressShareInitDbs          = 0.10;
const TReal32 KPosProgressShareCreateSearchOps  = 0.05;
const TReal32 KPosProgressShareSearchDbs        = 0.80;
const TReal32 KPosProgressComplete              = 1.0;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPosLmMultiDbSearchOperation::CPosLmMultiDbSearchOperation
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//

CPosLmMultiDbSearchOperation::CPosLmMultiDbSearchOperation(
    CPosLmMultiDbSearch* aMultiDbSearch,
    TBool aSearchOnlyPreviousMatches)
    : iMultiDbSearch(aMultiDbSearch),
    iSearchOnlyPreviousMatches(aSearchOnlyPreviousMatches),
    iOperationStatus(EOperationNotStarted),
    iSearchStatus(ESearchNotStarted)
    {
    }

// -----------------------------------------------------------------------------
// CPosLmMultiDbSearchOperation::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPosLmMultiDbSearchOperation::ConstructL(
    const CPosLmSearchCriteria& aCriteria,
    CPosLmMultiDbSortPref* aSortPref)
    {
    TBool isLmSearch =
        aSortPref->SortType() != CPosLmMultiDbSortPref::ECategorySorting;

    TBool useDistanceSorting;
    iCriteria = PosLmCriteriaHelper::ValidateAndCopyL(
        aCriteria, isLmSearch, useDistanceSorting);

    if (useDistanceSorting &&
        aSortPref->SortType() == CPosLmMultiDbSortPref::ELandmarkNoSorting)
        {
        aSortPref->SetDistanceSorting();
        }

    iSortPref = aSortPref;
    }

// -----------------------------------------------------------------------------
// CPosLmMultiDbSearchOperation::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPosLmMultiDbSearchOperation* CPosLmMultiDbSearchOperation::NewL(
    CPosLmMultiDbSearch* aMultiDbSearch,
    const CPosLmSearchCriteria& aCriteria,
    CPosLmMultiDbSortPref* aSortPref,
    TBool aSearchOnlyPreviousMatches)
    {
    CPosLmMultiDbSearchOperation* self = new (ELeave)
        CPosLmMultiDbSearchOperation(aMultiDbSearch,
            aSearchOnlyPreviousMatches);

    CleanupStack::PushL(self);
    self->ConstructL(aCriteria, aSortPref);
    CleanupStack::Pop(self);
    return self;
    }

// Destructor
CPosLmMultiDbSearchOperation::~CPosLmMultiDbSearchOperation()
    {
    if (iClientStatus)
        {
        User::RequestComplete(iClientStatus, KErrCancel);
        }

    if (iMultiDbSearch)
        {
        iMultiDbSearch->HandleSearchOperationCompleted();
        }

    delete iCriteria;
    delete iSortPref;

    iActiveOperations.ResetAndDestroy();
    }

// -----------------------------------------------------------------------------
// CPosLmMultiDbSearchOperation::HandleMultiDbSearchDeleted
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmMultiDbSearchOperation::HandleMultiDbSearchDeleted()
    {
    iMultiDbSearch = NULL;
    }

// -----------------------------------------------------------------------------
// CPosLmMultiDbSearchOperation::OperationCallBack
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CPosLmMultiDbSearchOperation::OperationCallBack(TAny* aPtr)
    {
    CPosLmActiveOperation* activeOp = static_cast<CPosLmActiveOperation*>(aPtr);

    CPosLmMultiDbSearchOperation* thisOp =
        static_cast<CPosLmMultiDbSearchOperation*>(activeOp->Owner());

    return thisOp->DoOperationCallBack(activeOp);
    }

// -----------------------------------------------------------------------------
// CPosLmMultiDbSearchOperation::NextStep
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmMultiDbSearchOperation::NextStep(
    TRequestStatus& aStatus,
    TReal32& aProgress)
    {
    if (iOperationStatus == ESyncStarted ||
        iOperationStatus == EOperationCompleted ||
        iClientStatus)
        {
        Panic(KPosLandmarksClientPanic, EPosInvalidOperationMode);
        }
    iOperationStatus = EAsyncStarted;

    aStatus = KRequestPending;
    aProgress = iProgress;
    iClientStatus = &aStatus;
    iClientProgress = &aProgress;

    // We can't continue if client has deleted iMultiDbSearch.
    if (!iMultiDbSearch)
        {
        CompleteClientRequest();
        return;
        }

    if (iSearchStatus == ESearchNotStarted)
        {
        IncrementSearchStep();
        }

    if (iSearchStatus == EInitDbs || iSearchStatus == ESearchDbs)
        {
        ExecuteAsyncStep();
        }
    else
        {
        ExecuteSyncStep();
        IncrementSearchStep();
        CompleteClientRequest();
        }
    }

// -----------------------------------------------------------------------------
// CPosLmMultiDbSearchOperation::ExecuteL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmMultiDbSearchOperation::ExecuteL()
    {
    if (iOperationStatus != EOperationNotStarted)
        {
        Panic(KPosLandmarksClientPanic, EPosInvalidOperationMode);
        }
    iOperationStatus = ESyncStarted;

    // We can't continue if client has deleted iMultiDbSearch.
    if (!iMultiDbSearch)
        {
        return;
        }

    while (iCurrentDbIndex < iMultiDbSearch->NumOfDatabasesToSearch())
        {
        if (iMultiDbSearch->ToBeSearched(iCurrentDbIndex))
            {
            ExecuteSyncStep();
            }

        iCurrentDbIndex++;
        }

    iOperationStatus = EOperationCompleted;
    HandleOperationCompleted();
    }

// -----------------------------------------------------------------------------
// CPosLmMultiDbSearchOperation::HandleOperationCompleted
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmMultiDbSearchOperation::HandleOperationCompleted()
    {
    if (iMultiDbSearch)
        {
        iMultiDbSearch->HandleSearchOperationCompleted();
        iMultiDbSearch = NULL;
        }
    }

// -----------------------------------------------------------------------------
// CPosLmMultiDbSearchOperation::CompleteClientRequest
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmMultiDbSearchOperation::CompleteClientRequest()
    {
    if (!iClientStatus)
        {
        return;
        }

    if (!iMultiDbSearch)
        {
        iSearchStatus = ESearchCompleted;
        }

    TInt status = KPosLmOperationNotComplete;
    if (iSearchStatus == ESearchCompleted)
        {
        status = KErrNone;

        iOperationStatus = EOperationCompleted;
        HandleOperationCompleted();
        }

    CalculateProgress();
    *iClientProgress = iProgress;

    User::RequestComplete(iClientStatus, status);
    }

// -----------------------------------------------------------------------------
// CPosLmMultiDbSearchOperation::IncrementSearchStep
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmMultiDbSearchOperation::IncrementSearchStep()
    {
    IncrementIndexAndSearchStatus();

    while (((iSearchStatus == EOpenDbs || iSearchStatus == ECreateSearchOps) &&
            !iMultiDbSearch->ToBeSearched(iCurrentDbIndex)) ||
           ((iSearchStatus == EInitDbs || iSearchStatus == ESearchDbs) &&
            iActiveOperations.Count() == 0))
        {
        IncrementIndexAndSearchStatus();
        }
    }

// -----------------------------------------------------------------------------
// CPosLmMultiDbSearchOperation::IncrementIndexAndSearchStatus
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmMultiDbSearchOperation::IncrementIndexAndSearchStatus()
    {
    switch (iSearchStatus)
        {
        case ESearchNotStarted:
        case EInitDbs:
        case ESearchDbs:
            iCurrentDbIndex = 0;
            IncrementSearchStatus();
            break;

        case EOpenDbs:
        case ECreateSearchOps:
            iCurrentDbIndex++;
            if (iCurrentDbIndex == iMultiDbSearch->NumOfDatabasesToSearch())
                {
                iCurrentDbIndex = 0;
                IncrementSearchStatus();
                }
            break;
        }
    }

// -----------------------------------------------------------------------------
// CPosLmMultiDbSearchOperation::IncrementSearchStatus
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmMultiDbSearchOperation::IncrementSearchStatus()
    {
    switch (iSearchStatus)
        {
        case ESearchNotStarted:
            iSearchStatus = EOpenDbs;
            break;

        case EOpenDbs:
            iSearchStatus = EInitDbs;
            break;

        case EInitDbs:
            iSearchStatus = ECreateSearchOps;
            break;

        case ECreateSearchOps:
            iSearchStatus = ESearchDbs;
            break;

        case ESearchDbs:
            iSearchStatus = ESearchCompleted;
            break;
        }
    }

// -----------------------------------------------------------------------------
// CPosLmMultiDbSearchOperation::ExecuteSyncStep
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmMultiDbSearchOperation::ExecuteSyncStep()
    {
    TRAPD(error, ExecuteSyncStepL());
    if (error != KErrNone)
        {
        iMultiDbSearch->AddSearchError(iCurrentDbIndex, error);
        }
    }

// -----------------------------------------------------------------------------
// CPosLmMultiDbSearchOperation::ExecuteSyncStepL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmMultiDbSearchOperation::ExecuteSyncStepL()
    {
    CPosLmOperation* initOp;
    if (iOperationStatus == ESyncStarted)
        {
        initOp = OpenDbAndCreateInitOpL();
        if (initOp)
            {
            ExecuteAndDeleteLD(initOp);
            }

        ExecuteAndDeleteLD(CreateSearchOpL());

        iMultiDbSearch->SearchExecutedL(iCurrentDbIndex, *iSortPref);
        }
    else
        {
        switch (iSearchStatus)
            {
            case EOpenDbs:
                initOp = OpenDbAndCreateInitOpL();
                if (initOp)
                    {
                    AppendAsActiveOperationL(initOp);
                    }
                break;

            case ECreateSearchOps:
                AppendAsActiveOperationL(CreateSearchOpL());
                break;

            default:
                break;
            }
        }
    }

// -----------------------------------------------------------------------------
// CPosLmMultiDbSearchOperation::OpenDbAndCreateInitOpL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPosLmOperation* CPosLmMultiDbSearchOperation::OpenDbAndCreateInitOpL()
    {
    CPosLandmarkDatabase* db = iMultiDbSearch->DatabaseL(iCurrentDbIndex);

    CPosLmOperation* initOp = NULL;
    if (db->IsInitializingNeeded())
        {
        initOp = db->InitializeL();
        }

    return initOp;
    }

// -----------------------------------------------------------------------------
// CPosLmMultiDbSearchOperation::CreateSearchOpL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPosLmOperation* CPosLmMultiDbSearchOperation::CreateSearchOpL()
    {
    CPosLandmarkSearch* searcher = iMultiDbSearch->SearcherL(iCurrentDbIndex);
    CPosLmOperation* searchOp;

    if (iSortPref->SortType() == CPosLmMultiDbSortPref::ECategorySorting)
        {
        searchOp = searcher->StartCategorySearchL(
            *iCriteria, iSortPref->CategorySortPref(),
            iSearchOnlyPreviousMatches);
        }
    else if (iSortPref->SortType() == CPosLmMultiDbSortPref::ELandmarkSorting)
        {
        searchOp = searcher->StartLandmarkSearchL(
            *iCriteria, iSortPref->LandmarkSortPref(),
            iSearchOnlyPreviousMatches);
        }
    else
        {
        searchOp = searcher->StartLandmarkSearchL(
            *iCriteria, iSearchOnlyPreviousMatches);
        }

    iMultiDbSearch->SearchStarted(iCurrentDbIndex);

    return searchOp;
    }

// -----------------------------------------------------------------------------
// CPosLmMultiDbSearchOperation::AppendAsActiveOperationL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmMultiDbSearchOperation::AppendAsActiveOperationL(
    CPosLmOperation* aOperation)
    {
    CleanupStack::PushL(aOperation);
    CPosLmActiveOperation* activeOp =
        CPosLmActiveOperation::NewL(aOperation, iCurrentDbIndex, this);
    CleanupStack::Pop(aOperation);

    CleanupStack::PushL(activeOp);
    iActiveOperations.AppendL(activeOp);
    CleanupStack::Pop(activeOp);
    }

// -----------------------------------------------------------------------------
// CPosLmMultiDbSearchOperation::ExecuteAsyncStep
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmMultiDbSearchOperation::ExecuteAsyncStep()
    {
    for (TInt i = 0; i < iActiveOperations.Count(); i++)
        {
        CPosLmActiveOperation* activeOp = iActiveOperations[i];
        if (!activeOp->IsActive())
            {
            TCallBack callback(OperationCallBack, activeOp);
            activeOp->ExecuteStep(callback);
            }
        }
    }

// -----------------------------------------------------------------------------
// CPosLmMultiDbSearchOperation::DoOperationCallBack
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CPosLmMultiDbSearchOperation::DoOperationCallBack(
    CPosLmActiveOperation* aActiveOp)
    {
    // We can't continue if client has deleted iMultiDbSearch.
    if (!iMultiDbSearch)
        {
        CompleteClientRequest();
        return 0;
        }

    TInt status = aActiveOp->Status();
    if (status != KErrNone && status != KPosLmOperationNotComplete)
        {
        iMultiDbSearch->AddSearchError(aActiveOp->DatabaseIndex(), status);
        }
    else if (iSearchStatus == ESearchDbs)
        {
        TRAPD(err, iMultiDbSearch->SearchExecutedL(aActiveOp->DatabaseIndex(),
                                                   *iSortPref));
        if (err != KErrNone)
            {
            iMultiDbSearch->AddSearchError(aActiveOp->DatabaseIndex(), err);
            status = err;
            }
        }

    if (status == KPosLmOperationNotComplete)
        {
        CompleteClientRequest();
        }
    else
        {
        TInt index = iActiveOperations.Find(aActiveOp);
        __ASSERT_DEBUG(index != KErrNotFound,
            Panic(KPosLandmarksClientPanic, EPosInvalidIndex));
        iActiveOperations.Remove(index);
        delete aActiveOp;

        if (iActiveOperations.Count() == 0)
            {
            IncrementSearchStep();
            CompleteClientRequest();
            }
        }

    return 0; // Do not call again
    }

// -----------------------------------------------------------------------------
// CPosLmMultiDbSearchOperation::CalculateProgress
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmMultiDbSearchOperation::CalculateProgress()
    {
    switch (iSearchStatus)
        {
        case ESearchNotStarted:
            iProgress = 0;
            break;

        case EOpenDbs:
            iProgress = KPosProgressShareOpenDbs *
                        CalculateSyncStepProgress();
            break;

        case EInitDbs:
            iProgress = KPosProgressShareOpenDbs +
                        KPosProgressShareInitDbs *
                        CalculateAsyncStepProgress();
            break;

        case ECreateSearchOps:
            iProgress = KPosProgressShareOpenDbs +
                        KPosProgressShareInitDbs +
                        KPosProgressShareCreateSearchOps *
                        CalculateSyncStepProgress();
            break;

        case ESearchDbs:
            iProgress = KPosProgressShareOpenDbs +
                        KPosProgressShareInitDbs +
                        KPosProgressShareCreateSearchOps +
                        KPosProgressShareSearchDbs *
                        CalculateAsyncStepProgress();
            break;

        case ESearchCompleted:
            iProgress = KPosProgressComplete;
            break;
        }
    }

// -----------------------------------------------------------------------------
// CPosLmMultiDbSearchOperation::CalculateSyncStepProgress
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TReal32 CPosLmMultiDbSearchOperation::CalculateSyncStepProgress()
    {
    return TReal32(iCurrentDbIndex) /
        TReal32(iMultiDbSearch->NumOfDatabasesToSearch());
    }

// -----------------------------------------------------------------------------
// CPosLmMultiDbSearchOperation::CalculateAsyncStepProgress
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TReal32 CPosLmMultiDbSearchOperation::CalculateAsyncStepProgress()
    {
    TUint numOfDbs = iMultiDbSearch->NumOfDatabasesToSearch();

    TReal32 stepProgress = TReal32(numOfDbs - iActiveOperations.Count()) /
                           TReal32(numOfDbs);

    for (TInt i = 0; i < iActiveOperations.Count(); i++)
        {
        CPosLmActiveOperation* activeOp = iActiveOperations[i];
        stepProgress += activeOp->Progress() / TReal32(numOfDbs);
        }

    return stepProgress;
    }

//  End of File
