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
* Description: Provides searches for landmarks or landmark categories 
*							 in multiple databases.
*
*
*/


// INCLUDE FILES
#include    <EPos_LandmarksErrors.h>
#include    <epos_poslmsearchimplextension.h>
#include    <epos_poslmdisplaydatahandler.h>
#include    <EPos_CPosLmDisplayData.h>
#include    <EPos_CPosLmDisplayItem.h>
#include    <EPos_CPosLmMultiDbSearch.h>
#include    "EPos_CPosLmMultiDbSearchOperation.h"
#include    "EPos_CPosLmMultiDbSearchItem.h"
#include    "EPos_CPosLmEmptyIterator.h"

// CONSTANTS
const TInt KPosGranularity = 8;

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CPosLmMultiDbSearch::CPosLmMultiDbSearch
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPosLmMultiDbSearch::CPosLmMultiDbSearch()
    : iNewMaxNumOfMatches(KPosLmMaxNumOfMatchesUnlimited)
    {
    }

// -----------------------------------------------------------------------------
// CPosLmMultiDbSearch::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPosLmMultiDbSearch::ConstructL(const CDesCArray& aDatabaseList)
    {
    SetDatabasesToSearchL(aDatabaseList);
    }

// -----------------------------------------------------------------------------
// CPosLmMultiDbSearch::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CPosLmMultiDbSearch* CPosLmMultiDbSearch::NewL(
    const CDesCArray& aDatabaseList)
    {
    CPosLmMultiDbSearch* self = new (ELeave) CPosLmMultiDbSearch();
    CleanupStack::PushL(self);
    self->ConstructL(aDatabaseList);
    CleanupStack::Pop(self);
    return self;
    }

// Destructor
EXPORT_C CPosLmMultiDbSearch::~CPosLmMultiDbSearch()
    {
    if (iMultiSearchOperation)
        {
        iMultiSearchOperation->HandleMultiDbSearchDeleted();
        }

    iSearchItems.ResetAndDestroy();
    iSearchErrors.Close();
    }

// -----------------------------------------------------------------------------
// CPosLmMultiDbSearch::SetDatabasesToSearchL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CPosLmMultiDbSearch::SetDatabasesToSearchL(
    const CDesCArray& aDatabaseList)
    {
    __ASSERT_ALWAYS(!iMultiSearchOperation, User::Leave(KErrInUse));
    __ASSERT_ALWAYS(aDatabaseList.Count() != 0, User::Leave(KErrArgument));

    CloseDbsNotToSearch(aDatabaseList);

    // Create search items for new uri, re-sort for existing uri
    for (TInt j = 0; j < aDatabaseList.Count(); j++)
        {
        TPtrC dbListUri = aDatabaseList.MdcaPoint(j);

        // Does uri exist in search items?
        TBool foundUri = EFalse;
        TInt i;
        for (i = j; i < iSearchItems.Count() && !foundUri; i++)
            {
            HBufC* searchItemUri = iSearchItems[i]->iDbUriPtr;
            if (dbListUri == *searchItemUri)
                {
                foundUri = ETrue;
                break;
                }
            }

        if (foundUri)
            {
            if (i != j)
                {
                // Re-sort item
                CPosLmMultiDbSearchItem* item = iSearchItems[i];
                iSearchItems.Remove(i);
                CleanupStack::PushL(item);
                iSearchItems.InsertL(item, j);
                CleanupStack::Pop(item);
                }
            }
        else
            {
            HBufC* uriPtr = dbListUri.AllocLC();
            CPosLmMultiDbSearchItem* item =
                new (ELeave) CPosLmMultiDbSearchItem(uriPtr);
            CleanupStack::Pop(uriPtr);
            CleanupStack::PushL(item);

            iSearchItems.InsertL(item, j);
            CleanupStack::Pop(item);
            }
        }

    // Remove any remaining double uri:s
    for (TInt i = aDatabaseList.Count(); i < iSearchItems.Count(); )
        {
        CPosLmMultiDbSearchItem* item = iSearchItems[i];
        iSearchItems.Remove(i);
        delete item;
        }

    // Reset all search errors as their db indexes might be invalid now.
    iSearchErrors.Reset();
    iNoOfSearchErrors = 0;
    }

// -----------------------------------------------------------------------------
// CPosLmMultiDbSearch::DatabasesToSearchL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C CDesCArray* CPosLmMultiDbSearch::DatabasesToSearchL()
    {
    CDesCArray* databaseList = new (ELeave) CDesCArrayFlat(KPosGranularity);
    CleanupStack::PushL(databaseList);

    for (TInt i = 0; i < iSearchItems.Count(); i++)
        {
        databaseList->AppendL(*(iSearchItems[i]->iDbUriPtr));
        }

    CleanupStack::Pop(databaseList);
    return databaseList;
    }

// -----------------------------------------------------------------------------
// CPosLmMultiDbSearch::SetMaxNumOfMatches
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CPosLmMultiDbSearch::SetMaxNumOfMatches(
    TInt aMaxNumOfMatches)
    {
    __ASSERT_ALWAYS(
        (aMaxNumOfMatches == KPosLmMaxNumOfMatchesUnlimited ||
        aMaxNumOfMatches > 0),
        Panic(KPosLandmarksClientPanic, EPosLmInvalidArgument));

    iNewMaxNumOfMatches = aMaxNumOfMatches;
    }

// -----------------------------------------------------------------------------
// CPosLmMultiDbSearch::StartLandmarkSearchL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C CPosLmOperation* CPosLmMultiDbSearch::StartLandmarkSearchL(
    const CPosLmSearchCriteria& aCriteria,
    TBool aSearchOnlyPreviousMatches)
    {
    return StartSearchL(aCriteria, CPosLmMultiDbSortPref::NewL(),
                        aSearchOnlyPreviousMatches);
    }

// -----------------------------------------------------------------------------
// CPosLmMultiDbSearch::StartLandmarkSearchL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C CPosLmOperation* CPosLmMultiDbSearch::StartLandmarkSearchL(
    const CPosLmSearchCriteria& aCriteria,
    const TPosLmSortPref& aSortPref,
    TBool aSearchOnlyPreviousMatches)
    {
    return StartSearchL(aCriteria, CPosLmMultiDbSortPref::NewL(aSortPref),
                        aSearchOnlyPreviousMatches);
    }

// -----------------------------------------------------------------------------
// CPosLmMultiDbSearch::StartCategorySearchL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C CPosLmOperation* CPosLmMultiDbSearch::StartCategorySearchL(
    const CPosLmSearchCriteria& aCriteria,
    CPosLmCategoryManager::TCategorySortPref aSortPref,
    TBool aSearchOnlyPreviousMatches)
    {
    __ASSERT_ALWAYS(
        aSortPref >= CPosLmCategoryManager::ECategorySortOrderNone &&
        aSortPref <= CPosLmCategoryManager::ECategorySortOrderNameDescending,
        Panic(KPosLandmarksClientPanic, EPosInvalidEnumValue));

    return StartSearchL(aCriteria, CPosLmMultiDbSortPref::NewL(aSortPref),
                        aSearchOnlyPreviousMatches);
    }

// -----------------------------------------------------------------------------
// CPosLmMultiDbSearch::NumOfSearchErrors
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TUint CPosLmMultiDbSearch::NumOfSearchErrors() const
    {
    return iNoOfSearchErrors;
    }

// -----------------------------------------------------------------------------
// CPosLmMultiDbSearch::GetSearchError
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CPosLmMultiDbSearch::GetSearchError(
    TUint aErrorIndex,
    TSearchError& aSearchError) const
    {
    __ASSERT_ALWAYS(aErrorIndex < iNoOfSearchErrors,
        Panic(KPosLandmarksClientPanic, EPosInvalidIndex));

    aSearchError.iDatabaseIndex = iSearchErrors[aErrorIndex].iDatabaseIndex;
    aSearchError.iErrorCode = iSearchErrors[aErrorIndex].iErrorCode;
    }

// -----------------------------------------------------------------------------
// CPosLmMultiDbSearch::TotalNumOfMatches
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TUint CPosLmMultiDbSearch::TotalNumOfMatches() const
    {
    TUint totalNum = 0;

    for (TInt i = 0; i < iSearchItems.Count(); i++)
        {
        totalNum += NumOfMatches(i);
        }

    return totalNum;
    }

// -----------------------------------------------------------------------------
// CPosLmMultiDbSearch::NumOfMatches
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TUint CPosLmMultiDbSearch::NumOfMatches(
    TUint aDatabaseIndex) const
    {
    __ASSERT_ALWAYS(aDatabaseIndex < static_cast<TUint>(iSearchItems.Count()),
        Panic(KPosLandmarksClientPanic, EPosInvalidIndex));

    CPosLmMultiDbSearchItem* item = iSearchItems[aDatabaseIndex];

    TUint numOfMatches = 0;

    CPosLandmarkSearch* searcher = item->iSearcher;
    if (searcher && item->iHasValidSearchResult)
        {
        numOfMatches = searcher->NumOfMatches();
        }

    return numOfMatches;
    }

// -----------------------------------------------------------------------------
// CPosLmMultiDbSearch::NumOfDatabasesToSearch
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TUint CPosLmMultiDbSearch::NumOfDatabasesToSearch() const
    {
    return iSearchItems.Count();
    }

// -----------------------------------------------------------------------------
// CPosLmMultiDbSearch::DatabaseUriPtr
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TPtrC CPosLmMultiDbSearch::DatabaseUriPtr(
    TUint aDatabaseIndex) const
    {
    __ASSERT_ALWAYS(aDatabaseIndex < static_cast<TUint>(iSearchItems.Count()),
        Panic(KPosLandmarksClientPanic, EPosInvalidIndex));

    return *(iSearchItems[aDatabaseIndex]->iDbUriPtr);
    }

// -----------------------------------------------------------------------------
// CPosLmMultiDbSearch::MatchIteratorL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C CPosLmItemIterator* CPosLmMultiDbSearch::MatchIteratorL(
    TUint aDatabaseIndex)
    {
    __ASSERT_ALWAYS(aDatabaseIndex < static_cast<TUint>(iSearchItems.Count()),
        Panic(KPosLandmarksClientPanic, EPosInvalidIndex));

    CPosLmMultiDbSearchItem* item = iSearchItems[aDatabaseIndex];
    CPosLandmarkSearch* searcher = item->iSearcher;

    CPosLmItemIterator* iterator = NULL;
    if (searcher && item->iHasValidSearchResult)
        {
        iterator = searcher->MatchIteratorL();
        }
    else
        {
        iterator = CPosLmEmptyIterator::NewL();
        }

    return iterator;
    }

// -----------------------------------------------------------------------------
// CPosLmMultiDbSearch::SetDisplayData
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CPosLmMultiDbSearch::SetDisplayData(
    CPosLmDisplayData& aData)
    {
    __ASSERT_ALWAYS(!iMultiSearchOperation,
        Panic(KPosLandmarksClientPanic, EPosSearchOperationInUse));

    iClientDisplayData = &aData;
    }

// -----------------------------------------------------------------------------
// CPosLmMultiDbSearch::UnsetDisplayData
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CPosLmMultiDbSearch::UnsetDisplayData()
    {
    __ASSERT_ALWAYS(!iMultiSearchOperation,
        Panic(KPosLandmarksClientPanic, EPosSearchOperationInUse));

    iClientDisplayData = NULL;
    }

// -----------------------------------------------------------------------------
// CPosLmMultiDbSearch::MaxNumOfMatches
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CPosLmMultiDbSearch::MaxNumOfMatches() const
    {
    return iNewMaxNumOfMatches;
    }

// -----------------------------------------------------------------------------
// CPosLmMultiDbSearch::ToBeSearched
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CPosLmMultiDbSearch::ToBeSearched(TUint aDatabaseIndex) const
    {
    return iSearchItems[aDatabaseIndex]->iToBeSearched;
    }

// -----------------------------------------------------------------------------
// CPosLmMultiDbSearch::DatabaseL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPosLandmarkDatabase* CPosLmMultiDbSearch::DatabaseL(TUint aDatabaseIndex)
    {
    CPosLmMultiDbSearchItem* item = iSearchItems[aDatabaseIndex];

    if (!item->iDb)
        {
        item->iDb = CPosLandmarkDatabase::OpenL(*(item->iDbUriPtr));
        }

    return item->iDb;
    }

// -----------------------------------------------------------------------------
// CPosLmMultiDbSearch::SearcherL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPosLandmarkSearch* CPosLmMultiDbSearch::SearcherL(TUint aDatabaseIndex)
    {
    CPosLmMultiDbSearchItem* item = iSearchItems[aDatabaseIndex];

    if (!item->iSearcher)
        {
        item->iSearcher = CPosLandmarkSearch::NewL(*(item->iDb));
        }

    item->iSearcher->SetMaxNumOfMatches(iMaxNumOfMatches);

    if (iClientDisplayData)
        {
        item->iSearcher->SetDisplayData(*(item->iDisplayData));
        }

    return item->iSearcher;
    }

// -----------------------------------------------------------------------------
// CPosLmMultiDbSearch::SearchStarted
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmMultiDbSearch::SearchStarted(TUint aDatabaseIndex)
    {
    iSearchItems[aDatabaseIndex]->iHasValidSearchResult = ETrue;
    }

// -----------------------------------------------------------------------------
// CPosLmMultiDbSearch::SearchExecutedL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmMultiDbSearch::SearchExecutedL(
    TUint aDatabaseIndex,
    const CPosLmMultiDbSortPref& aSortPref)
    {
    CPosLmMultiDbSearchItem* item = iSearchItems[aDatabaseIndex];

    if (iClientDisplayData)
        {
        TInt newDisplayItemIndex = item->iDisplayData->NewItemIndex();
        while (newDisplayItemIndex != KPosLmNoNewItems)
            {
            CPosLmDisplayItem* displayItem =
                PosLmDisplayDataHandler::CopyItemLC(
                *(item->iDisplayData), newDisplayItemIndex);

            PosLmSearchImplExtension::SetDatabaseIndex(
                *displayItem, aDatabaseIndex);

            switch (aSortPref.SortType())
                {
                case CPosLmMultiDbSortPref::ELandmarkNoSorting:
                    PosLmDisplayDataHandler::InsertItemL(
                        *iClientDisplayData, displayItem);
                    break;

                case CPosLmMultiDbSortPref::ELandmarkDistanceSorting:
                    PosLmDisplayDataHandler::InsertInDistanceOrderL(
                        *iClientDisplayData, displayItem);
                    break;

                case CPosLmMultiDbSortPref::ELandmarkSorting:
                    PosLmDisplayDataHandler::InsertItemInOrderL(
                        *iClientDisplayData, displayItem,
                        aSortPref.LandmarkSortPref());
                    break;

                case CPosLmMultiDbSortPref::ECategorySorting:
                    PosLmDisplayDataHandler::InsertItemInOrderL(
                        *iClientDisplayData, displayItem,
                        aSortPref.CategorySortPref());
                    break;
                }
            CleanupStack::Pop(displayItem);

            newDisplayItemIndex = item->iDisplayData->NewItemIndex();
            }
        }
    }

// -----------------------------------------------------------------------------
// CPosLmMultiDbSearch::AddSearchError
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmMultiDbSearch::AddSearchError(TUint aDatabaseIndex, TInt aErrorCode)
    {
    iSearchItems[aDatabaseIndex]->iToBeSearched = EFalse;

    iSearchErrors[iNoOfSearchErrors].iDatabaseIndex = aDatabaseIndex;
    iSearchErrors[iNoOfSearchErrors].iErrorCode = aErrorCode;
    iNoOfSearchErrors++;
    }

// -----------------------------------------------------------------------------
// CPosLmMultiDbSearch::HandleSearchOperationCompleted
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmMultiDbSearch::HandleSearchOperationCompleted()
    {
    iMultiSearchOperation = NULL;
    }

// -----------------------------------------------------------------------------
// CPosLmMultiDbSearch::CloseDbsNotToSearch
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmMultiDbSearch::CloseDbsNotToSearch(const CDesCArray& aDatabaseList)
    {
    // Close any dbs not in databaseList
    for (TInt i = 0; i < iSearchItems.Count(); i++)
        {
        HBufC* searchItemUri = iSearchItems[i]->iDbUriPtr;

        // Does uri exist in databaseList?
        TInt pos;
        TInt foundUri = aDatabaseList.Find(*searchItemUri, pos, ECmpNormal);

        if (foundUri != 0)
            {
            CPosLmMultiDbSearchItem* item = iSearchItems[i];
            iSearchItems.Remove(i);
            delete item;
            i--;
            }
        }
    }

// -----------------------------------------------------------------------------
// CPosLmMultiDbSearch::StartSearchL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPosLmOperation* CPosLmMultiDbSearch::StartSearchL(
    const CPosLmSearchCriteria& aCriteria,
    CPosLmMultiDbSortPref* aSortPref,
    TBool aSearchOnlyPreviousMatches)
    {
    CleanupStack::PushL(aSortPref);

    __ASSERT_ALWAYS(!iMultiSearchOperation, User::Leave(KErrInUse));
    __ASSERT_ALWAYS(!aSearchOnlyPreviousMatches || TotalNumOfMatches() != 0,
        User::Leave(KErrArgument));

    iMaxNumOfMatches = iNewMaxNumOfMatches;

    CPosLmPartialReadParameters* partial = NULL;
    if (iClientDisplayData)
        {
        iClientDisplayData->Reset();

        partial = PosLmSearchImplExtension::PartialReadParameters(
            *iClientDisplayData);
        }

    TSearchError dummyError;
    dummyError.iDatabaseIndex = 0;
    dummyError.iErrorCode = 0;
    iSearchErrors.Reset();
    iNoOfSearchErrors = 0;

    for (TInt i = 0; i < iSearchItems.Count(); i++)
        {
        // We don't want to handle allocation problems if errors are found
        // during the search. Allocate memory for search errors before search
        // is started.
        iSearchErrors.AppendL(dummyError);

        CPosLmMultiDbSearchItem* item = iSearchItems[i];

        if (aSearchOnlyPreviousMatches && NumOfMatches(i) == 0)
            {
            item->iToBeSearched = EFalse;
            item->iHasValidSearchResult = ETrue;
            }
        else
            {
            item->iToBeSearched = ETrue;
            item->iHasValidSearchResult = EFalse;

            if (iClientDisplayData)
                {
                // We must set partial read parameters before the search is
                // started as the client might change the value during the
                // search.
                if (!item->iDisplayData)
                    {
                    item->iDisplayData = CPosLmDisplayData::NewL();
                    }
                else
                    {
                    item->iDisplayData->Reset();
                    }

                if (partial)
                    {
                    item->iDisplayData->SetPartialReadParametersL(*partial);
                    }
                }
            else if (item->iSearcher)
                {
                item->iSearcher->UnsetDisplayData();
                }
            }
        }

    iMultiSearchOperation = CPosLmMultiDbSearchOperation::NewL(
        this, aCriteria, aSortPref, aSearchOnlyPreviousMatches);
    CleanupStack::Pop(aSortPref);

    return iMultiSearchOperation;
    }

//  End of File
