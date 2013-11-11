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
* Description: This class is used to start searches for landmarks or landmark 
*  categories in a local landmark database.
*
*
*/


// INCLUDE FILES
#include    <epos_cposlmlocaldatabase.h>
#include    <epos_cposlmlocaldbaccess.h>
#include    <EPos_CPosLmSearchCriteria.h>
#include    <EPos_LandmarksErrors.h>
#include    "EPos_CPosLmLocalSearch.h"
#include    "EPos_CPosLmLocalSearchIterator.h"
#include    "EPos_CPosLmLocalSearchOperation.h"
#include    "EPos_CPosLmLocalSearchHandler.h"

// CONSTANTS

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPosLmLocalSearch::CPosLmLocalSearch
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPosLmLocalSearch::CPosLmLocalSearch()
:   CPosLandmarkSearch()
    {
    }

// -----------------------------------------------------------------------------
// CPosLmLocalSearch::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPosLmLocalSearch::ConstructL(
    TAny* aDatabase)
    {
    iDb = reinterpret_cast<CPosLmLocalDatabase*>(aDatabase);
    }

// -----------------------------------------------------------------------------
// CPosLmLocalSearch::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPosLmLocalSearch* CPosLmLocalSearch::NewL(
    TAny* aDatabase)
    {
    CPosLmLocalSearch* self = new (ELeave) CPosLmLocalSearch();
    CleanupStack::PushL(self);
    self->ConstructL(aDatabase);
    CleanupStack::Pop();
    return self;
    }

// Destructor
CPosLmLocalSearch::~CPosLmLocalSearch()
    {
    if (iSearchOperation)
        {
        iSearchOperation->HandleLocalSearchDeleted();
        }

    if (iOwnsSearchHandler)
        {
        delete iSearchHandler;
        }
    }

// -----------------------------------------------------------------------------
// CPosLmLocalSearch::SearchCompleted
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmLocalSearch::SearchCompleted()
    {
    // Take back ownership of iSearchHandler when operation has completed.
    iOwnsSearchHandler = ETrue;

    iSearchOperation = NULL;
    }

// -----------------------------------------------------------------------------
// CPosLmLocalSearch::StartLandmarkSearchL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPosLmOperation* CPosLmLocalSearch::StartLandmarkSearchL(
    const CPosLmSearchCriteria& aCriteria,
    TBool aSearchOnlyPreviousMatches)
    {
    TPosLmLocalSortPref sortPref;

    StartSearchL(ETrue, aCriteria, sortPref, aSearchOnlyPreviousMatches);

    return iSearchOperation;
    }

// -----------------------------------------------------------------------------
// CPosLmLocalSearch::StartLandmarkSearchL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPosLmOperation*  CPosLmLocalSearch::StartLandmarkSearchL(
    const CPosLmSearchCriteria& aCriteria,
    const TPosLmSortPref& aSortPref,
    TBool aSearchOnlyPreviousMatches)
    {
    if (aSortPref.LandmarkAttributeToSortBy() != CPosLandmark::ELandmarkName)
        {
        User::Leave(KErrNotSupported);
        }

    TPosLmLocalSortPref sortPref(aSortPref);

    StartSearchL(ETrue, aCriteria, sortPref, aSearchOnlyPreviousMatches);

    return iSearchOperation;
    }

// -----------------------------------------------------------------------------
// CPosLmLocalSearch::StartCategorySearchL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPosLmOperation*  CPosLmLocalSearch::StartCategorySearchL(
    const CPosLmSearchCriteria& aCriteria,
    CPosLmCategoryManager::TCategorySortPref aSortPref,
    TBool aSearchOnlyPreviousMatches)
    {
    __ASSERT_ALWAYS(
        aSortPref >= CPosLmCategoryManager::ECategorySortOrderNone &&
        aSortPref <= CPosLmCategoryManager::ECategorySortOrderNameDescending,
        Panic(KPosLandmarksClientPanic, EPosInvalidEnumValue));

    TPosLmLocalSortPref sortPref(aSortPref);

    StartSearchL(EFalse, aCriteria, sortPref, aSearchOnlyPreviousMatches);

    return iSearchOperation;
    }

// -----------------------------------------------------------------------------
// CPosLmLocalSearch::NumOfMatches
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TUint CPosLmLocalSearch::NumOfMatches() const
    {
    TUint numOfMatches = 0;

    if (iSearchHandler)
        {
        numOfMatches = iSearchHandler->Matches().Count();
        }

    return numOfMatches;
    }

// -----------------------------------------------------------------------------
// CPosLmLocalSearch::MatchIteratorL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPosLmItemIterator* CPosLmLocalSearch::MatchIteratorL()
    {
    if (!iSearchHandler)
        {
        return CPosLmLocalSearchIterator::NewL();
        }

    return CPosLmLocalSearchIterator::NewL(iSearchHandler->Matches());
    }

// -----------------------------------------------------------------------------
// CPosLmLocalSearch::SetDisplayData
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmLocalSearch::SetDisplayData(CPosLmDisplayData& aData)
    {
    __ASSERT_ALWAYS(!iSearchOperation,
                    Panic(KPosLandmarksClientPanic, EPosSearchOperationInUse));

    // Set display data reference
    iDisplayData = &aData;
    }

// -----------------------------------------------------------------------------
// CPosLmLocalSearch::UnsetDisplayData
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmLocalSearch::UnsetDisplayData()
    {
    __ASSERT_ALWAYS(!iSearchOperation,
                    Panic(KPosLandmarksClientPanic, EPosSearchOperationInUse));

    // Set display data reference to null
    iDisplayData = NULL;
    }

// -----------------------------------------------------------------------------
// CPosLmLocalSearch::StartSearchL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmLocalSearch::StartSearchL(
    TBool aSearchForLandmarks,
    const CPosLmSearchCriteria& aCriteria,
    const TPosLmLocalSortPref& aSortPref,
    TBool aSearchOnlyPreviousMatches)
    {
    if (iSearchOperation)
        {
        User::Leave(KErrInUse);
        }

    if (!iSearchHandler && aSearchOnlyPreviousMatches)
        {
        User::Leave(KErrArgument);
        }

    if (aCriteria.CriteriaType() == CPosLmSearchCriteria::ECriteriaIdList)
        {
        User::Leave(KErrArgument);
        }

    if (!aSearchForLandmarks &&
        aCriteria.CriteriaType() == CPosLmSearchCriteria::ECriteriaComposite)
        {
        User::Leave(KErrNotSupported);
        }

    // Get previous matches before deleting the previous search handler.
    RArray<TPosLmItemId> previousMatches;
    if (aSearchOnlyPreviousMatches)
        {
        CleanupClosePushL(previousMatches);
        iSearchHandler->GetMatchesL(previousMatches);

        if (previousMatches.Count() == 0)
            {
            User::Leave(KErrArgument);
            }
        }

    delete iSearchHandler;
    iSearchHandler = NULL;

    // Create the proper searchHandler
    iSearchHandler = CPosLmLocalSearchHandler::CreateHandlerL(
        *iDb, aCriteria, aSortPref,
        iDisplayData, MaxNumOfMatches());

    iOwnsSearchHandler = ETrue;

    if ((aSearchForLandmarks && !iSearchHandler->IsValidForLandmarkSearch()) ||
        (!aSearchForLandmarks && !iSearchHandler->IsValidForCategorySearch()))
        {
        User::Leave(KErrArgument);
        }

    // Set previous matches
    if (aSearchOnlyPreviousMatches)
        {
        iSearchHandler->SetPreviousMatchesToSearchL(previousMatches);
        CleanupStack::PopAndDestroy(&previousMatches);
        }

    // Create the search operation
    iSearchOperation = CPosLmLocalSearchOperation::NewL(
        iDb, this, iSearchHandler);

    iOwnsSearchHandler = EFalse;
    }

//  End of File
