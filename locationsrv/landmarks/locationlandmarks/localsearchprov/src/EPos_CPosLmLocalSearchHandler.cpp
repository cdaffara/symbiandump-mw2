/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Base class for criteria based searches.
*
*
*/


#include    <epos_cposlmlocaldbaccess.h>
#include    <EPos_Landmarks.h>
#include    <EPos_LandmarksErrors.h>
#include    <epos_landmarkdatabasestructure.h>
#include    <epos_poslmlandmarkhandler.h>
#include    <epos_poslmcategoryhandler.h>
#include    <EPos_PosLmImplExtension.h>
#include    <epos_poslmsearchimplextension.h>
#include    <epos_poslmdisplaydatahandler.h>
#include    <epos_rposlmlocalnameindex.h>
#include    <epos_cposlmlocaldatabase.h>
#include    <epos_cposlmnameindexiterator.h>
#include    <EPos_CPosLandmarkSearch.h>
#include    <epos_poslmdatabaseutility.h>

#include    "EPos_CPosLmLocalAreaSearch.h"
#include    "EPos_CPosLmLocalTextSearch.h"
#include    "EPos_CPosLmLocalCategorySearch.h"
#include    "EPos_CPosLmLocalCatNameSearch.h"
#include    "EPos_CPosLmLocalNearestSearch.h"
#include    "EPos_CPosLmLocalCompositeSearch.h"
#include    "EPos_CPosLmDisplayItem.h"
#include    "EPos_CPosLmDisplayData.h"
#include    "EPos_LandmarksLocalSearchProviderPanic.h"
#include    "EPos_CPosLmLocalSearchHandler.h"

_LIT(KPosLmSqlAppendOrderBy,        " ORDER BY %S");
_LIT(KPosLmSqlAppendDesc,           " DESC");

const TInt KMaxDisplayDataMatchesPerStep = 10;
const TInt KPosLmSortedLandmarkBatchSize = 100;
const TInt KPosLmMatchesGranularity = 32;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmLocalSearchHandler::CPosLmLocalSearchHandler(
    CPosLmLocalDatabase& aDatabase,
    CPosLmSearchCriteria::TCriteriaType aCriteriaType,
    const TPosLmLocalSortPref& aSortPref,
    TInt aMaxNumOfMatches)
:   iCurrentMatches( KPosLmMatchesGranularity ),
    iDatabase( aDatabase ),
    iLocalSortPref(aSortPref),
    iMaxNumOfMatches(aMaxNumOfMatches),
    iSearchOnlyPreviousMatches(EFalse),
    iCriteriaType(aCriteriaType)
    {
    iDbAccess = iDatabase.DatabaseAccess();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmLocalSearchHandler::~CPosLmLocalSearchHandler()
    {
    iCurrentMatches.Close();
    iPreviousMatches.Close();
    delete iPartialReadParameters;
    CleanupSources();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmLocalSearchHandler* CPosLmLocalSearchHandler::CreateHandlerL(
    CPosLmLocalDatabase& aDatabase,
    const CPosLmSearchCriteria& aCriteria,
    const TPosLmLocalSortPref& aSortPref,
    CPosLmDisplayData* aDisplayData,
    TInt aMaxNumOfMatches)
    {
    CPosLmLocalSearchHandler* handler = NULL;

    // Create the proper searchHandler
    switch (aCriteria.CriteriaType())
        {
        case CPosLmSearchCriteria::ECriteriaArea:
            handler = CPosLmLocalAreaSearch::NewL(
                aDatabase, aCriteria, aSortPref, aDisplayData,
                aMaxNumOfMatches);
            break;
        case CPosLmSearchCriteria::ECriteriaText:
            handler = CPosLmLocalTextSearch::NewL(
                aDatabase, aCriteria, aSortPref, aDisplayData,
                aMaxNumOfMatches);
            break;
        case CPosLmSearchCriteria::ECriteriaCategory:
            handler = CPosLmLocalCategorySearch::NewL(
                aDatabase, aCriteria, aSortPref, aDisplayData,
                aMaxNumOfMatches);
            break;
        case CPosLmSearchCriteria::ECriteriaFindNearest:
            handler = CPosLmLocalNearestSearch::NewL(
                aDatabase, aCriteria, aSortPref, aDisplayData,
                aMaxNumOfMatches);
            break;
        case CPosLmSearchCriteria::ECriteriaCategoryByName:
            handler = CPosLmLocalCatNameSearch::NewL(
                aDatabase, aCriteria, aSortPref, aDisplayData,
                aMaxNumOfMatches);
            break;
        case CPosLmSearchCriteria::ECriteriaComposite:
            handler = CPosLmLocalCompositeSearch::NewL(
                aDatabase, aCriteria, aSortPref, aDisplayData,
                aMaxNumOfMatches);
            break;
        default:
            User::Leave(KErrNotSupported);
        }

    return handler;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalSearchHandler::SetPreviousMatchesToSearchL(
    const RArray<TPosLmItemId>& aPreviousMatches )
    {
    iSearchOnlyPreviousMatches = ETrue;

    iPreviousMatches.Reset();
    iPreviousMatches.ReserveL( aPreviousMatches.Count() );
    for ( TInt i = 0; i < aPreviousMatches.Count(); i++ )
        {
        iPreviousMatches.AppendL( aPreviousMatches[i] );
        }
    // this allows faster comparisons during search
    // in MatchesPreviousMatches() method
    iPreviousMatches.Sort();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool CPosLmLocalSearchHandler::IsValidForLandmarkSearch() const
    {
    return EFalse;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool CPosLmLocalSearchHandler::IsValidForCategorySearch() const
    {
    return EFalse;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalSearchHandler::CancelSearch()
    {
    CleanupSources();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalSearchHandler::CleanupSources()
    {
    iView.Close();
    iTable.Close();
    delete iLandmarkIndex;
    iLandmarkIndex = NULL;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalSearchHandler::GetMatchesL( RArray<TPosLmItemId>& aMatches ) const
    {
    aMatches.Reset();
    aMatches.ReserveL( iCurrentMatches.Count() );
    for ( TInt i = 0; i < iCurrentMatches.Count(); i++ )
        {
        aMatches.AppendL( iCurrentMatches[i] );
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
const RArray<TPosLmItemId>& CPosLmLocalSearchHandler::Matches() const
    {
    return iCurrentMatches;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalSearchHandler::SetDisplayDataL(
    CPosLmDisplayData* aData)
    {
    __ASSERT_DEBUG(!iDisplayData, User::Panic(
        KPosLandmarksLocalSearchProviderPanic,
        LandmarksLocalSearchProviderDisplayDataAlreadySet));
    iDisplayData = aData;

    if (PosLmSearchImplExtension::PartialReadParameters(*iDisplayData))
        {
        iPartialReadParameters = CPosLmPartialReadParameters::NewLC();
        CleanupStack::Pop();

        if (iLocalSortPref.SortType() ==  TPosLmLocalSortPref::ELandmarkSorting)
            {
            iPartialReadParameters->SetRequestedAttributes(
            PosLmSearchImplExtension::PartialReadParameters(
                *iDisplayData)->RequestedAttributes() |
                CPosLandmark::ELandmarkName);
            }
        else
            {
            iPartialReadParameters->SetRequestedAttributes(
            PosLmSearchImplExtension::PartialReadParameters(
                *iDisplayData)->RequestedAttributes());
            }

        RArray<TUint> requestedFields;
        CleanupClosePushL(requestedFields);
        PosLmSearchImplExtension::PartialReadParameters(
            *iDisplayData)->GetRequestedPositionFields(requestedFields);

        iPartialReadParameters->SetRequestedPositionFields(requestedFields);
        CleanupStack::PopAndDestroy(&requestedFields);
        }

    iDisplayData->Reset();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool CPosLmLocalSearchHandler::HasDisplayData() const
    {
    return (iDisplayData != NULL);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmDisplayData& CPosLmLocalSearchHandler::DisplayData()
    {
    ASSERT( iDisplayData );
    return *iDisplayData;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
const CPosLmDisplayData& CPosLmLocalSearchHandler::DisplayData() const
    {
    ASSERT( iDisplayData );
    return *iDisplayData;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
RArray<TPosLmItemId>& CPosLmLocalSearchHandler::CurrentMatches()
    {
    return iCurrentMatches;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TInt CPosLmLocalSearchHandler::MaxNumOfMatches() const
    {
    return iMaxNumOfMatches;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool CPosLmLocalSearchHandler::IsMaxMatchesFound() const
    {
    if ( iMaxNumOfMatches != KPosLmMaxNumOfMatchesUnlimited )
    	return iCurrentMatches.Count() >= iMaxNumOfMatches;
	else
		return EFalse;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool CPosLmLocalSearchHandler::BreakOnDisplayData( TInt aInitialDdMatches ) const
    {
    if ( HasDisplayData() )
        return DisplayData().Count() - aInitialDdMatches >= KMaxDisplayDataMatchesPerStep;
    else
        return EFalse;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TInt CPosLmLocalSearchHandler::DisplayDataCount() const
    {
    if ( HasDisplayData() )
        return DisplayData().Count();
    else
        return 0;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
const TPosLmLocalSortPref& CPosLmLocalSearchHandler::SortPref() const
    {
    return iLocalSortPref;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
const RArray<TUint>& CPosLmLocalSearchHandler::PreviousMatches() const
    {
    return iPreviousMatches;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool CPosLmLocalSearchHandler::MatchesPreviousMatches(
    TPosLmItemId aCurrentMatch ) const
    {
    TBool matches = ETrue;

    if ( iSearchOnlyPreviousMatches )
        {
        if ( iPreviousMatches.FindInOrder( aCurrentMatch ) == KErrNotFound )
            {
            // Current match is not included in previous matches.
            matches = EFalse;
            }
        }

    return matches;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalSearchHandler::AddMatchL( 
    TPosLmItemId aMatchId, 
    RDbRowSet* aDataRow )
    {
    if ( !MatchesPreviousMatches( aMatchId ) )
        {
        // Skip current match if it is not included in previous matches.
        return;
        }

    if ( iCurrentMatches.Find( aMatchId ) != KErrNotFound )
        {
        // Skip current match if it is already included.
        return;
        }

    if ( iDisplayData )
        {
        TInt sortPosition = AddMatchToDisplayDataL( aMatchId, aDataRow );
        User::LeaveIfError( iCurrentMatches.Insert( aMatchId, sortPosition ) );
        }
    else
        {
        User::LeaveIfError( iCurrentMatches.Append( aMatchId ) );
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalSearchHandler::AppendSortToQueryL(
    HBufC* aDbQuery)
    {
    if (iLocalSortPref.SortType() == TPosLmLocalSortPref::ELandmarkSorting)
        {
        __ASSERT_DEBUG(
            iLocalSortPref.LandmarkAttributeToSortBy() ==
                CPosLandmark::ELandmarkName,
            Panic(KPosLandmarksClientPanic, EPosInvalidLandmarkAttribute));

        aDbQuery->Des().AppendFormat(KPosLmSqlAppendOrderBy, &KPosLmNameCol);
        }
    else
        {
        aDbQuery->Des().AppendFormat(KPosLmSqlAppendOrderBy,
                                     &KPosLmCategoryNameCol);
        }

    if (iLocalSortPref.SortOrder() == TPosLmLocalSortPref::ESortDescending)
        {
        aDbQuery->Des().Append(KPosLmSqlAppendDesc);
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalSearchHandler::PrepareViewL(
    RDbView& aView,
    const TDesC& aDbQuery,
    TDbTextComparison aTextComparison)
    {
    aView.Close();
    RDbNamedDatabase db;
    iDbAccess->GetDatabase(db);

    User::LeaveIfError(aView.Prepare(db,
                                     TDbQuery(aDbQuery,aTextComparison),
                                     TDbWindow::EUnlimited,
                                     RDbRowSet::EReadOnly));
    iProgressHelper.HandleViewPrepared();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalSearchHandler::EvaluateViewL( RDbView& aView )
    {
    if ( aView.Unevaluated() && !aView.AtRow() )
        {
        TInt r = aView.Evaluate();
        if ( r < 0 ) User::Leave( r );
        iProgressHelper.HandleViewEvaluatedL( aView.CountL() );

        if ( !aView.AtRow() && !aView.AtEnd() )
            {
            aView.NextL();
            }
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool CPosLmLocalSearchHandler::MoreInView( RDbView& aView ) const
    {
    return aView.Unevaluated() || !aView.AtEnd();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalSearchHandler::GetRowL( RDbRowSet& aView )
    {
    ASSERT( aView.AtRow() );
    aView.GetL();
    iProgressHelper.HandleViewGetRow();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool CPosLmLocalSearchHandler::SearchViewL( 
    RDbView& aMatchView,
    TInt aMatchIdColumn,
    TBool aUseMatchViewForDisplayData )
    {
    EvaluateViewL( aMatchView );

    TInt ddMatches = DisplayDataCount();
    while ( aMatchView.AtRow() && !IsMaxMatchesFound() && !BreakOnDisplayData( ddMatches ) )
        {
        GetRowL( aMatchView );
        AddMatchL( 
            aMatchView.ColUint32( aMatchIdColumn ),
            aUseMatchViewForDisplayData ? &aMatchView : NULL );
        aMatchView.NextL();
        }
    return !MoreInView( aMatchView );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TInt CPosLmLocalSearchHandler::SearchStepCompletedL(
    TBool aSearchFinished,
    TReal32& aProgress)
    {
    TInt status;

    if ( aSearchFinished )
        {
        CleanupSources();
        aProgress = KProgressCompleted;
        status = KErrNone;
        }
    else
        {
        aProgress = CalculateProgressL();
        status = KPosLmOperationNotComplete;
        }

    return status;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TInt CPosLmLocalSearchHandler::GetRowCountL( const TDesC& aTable )
    {
    return PosLmDatabaseUtility::GetTableRowCountL( *iDbAccess, aTable );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalSearchHandler::PrepareCalculateProgress(
    TInt aNoOfRowsToEvaluate,
    TBool aSqlQueryIncludesSearchCondition)
    {
    iProgressHelper.PrepareCalculateProgress(
        aNoOfRowsToEvaluate, aSqlQueryIncludesSearchCondition);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TReal32 CPosLmLocalSearchHandler::CalculateProgressL()
    {
    return iProgressHelper.CalculateProgressL();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TInt CPosLmLocalSearchHandler::AddMatchToDisplayDataL(
    TPosLmItemId aMatchId,
    RDbRowSet* aDataRow,
    TBool aSuppressSorting,
    TReal32 aDistance )
    {
    ASSERT( iDisplayData );
    CPosLmDisplayItem* item = CreateItemLC( aMatchId, aDataRow );

    TInt pos = -1;
    if ( aSuppressSorting  )
        {
        PosLmDisplayDataHandler::InsertItemL( *iDisplayData, item );
        pos = iDisplayData->Count() - 1;
        }
    // Check if category sorting
    else if ( iLocalSortPref.SortType() == TPosLmLocalSortPref::ECategorySorting )
        {
        if ( iLocalSortPref.SortOrder() == TPosLmLocalSortPref::ESortAscending )
            {
            PosLmDisplayDataHandler::InsertItemInOrderL(
                *iDisplayData, item,
                CPosLmCategoryManager::ECategorySortOrderNameAscending );
            }
        else  //TPosLmLocalSortPref::ESortDescending
            {
            PosLmDisplayDataHandler::InsertItemInOrderL(
                *iDisplayData, item,
                CPosLmCategoryManager::ECategorySortOrderNameDescending );
            }
        }
    // Check if landmark sorting
    else if ( iLocalSortPref.SortType() == TPosLmLocalSortPref::ELandmarkSorting )
        {
        if ( iLocalSortPref.SortOrder() == TPosLmLocalSortPref::ESortAscending )
            {
            TPosLmSortPref sortPref(
                iLocalSortPref.LandmarkAttributeToSortBy(),
                TPosLmSortPref::EAscending );
            PosLmDisplayDataHandler::InsertItemInOrderL(
                *iDisplayData, item, sortPref );
            }
        else //TPosLmLocalSortPref::ESortDescending
            {
            TPosLmSortPref sortPref(
                iLocalSortPref.LandmarkAttributeToSortBy(),
                TPosLmSortPref::EDescending );
            PosLmDisplayDataHandler::InsertItemInOrderL(
                *iDisplayData, item, sortPref );
            }
        }
    // distance sorting?
    else if ( iCriteriaType == CPosLmSearchCriteria::ECriteriaFindNearest )
        {
        __ASSERT_DEBUG( aDistance >= 0,
            Panic( KPosLandmarksClientPanic, EPosLmInvalidArgument ) );

        PosLmSearchImplExtension::SetDistance( *item, aDistance );
        PosLmDisplayDataHandler::InsertInDistanceOrderL(
            *iDisplayData, item );
        }
    else // no sorting requested
        {
        PosLmDisplayDataHandler::InsertItemL( *iDisplayData, item );
        pos = iDisplayData->Count() - 1;
        }

    CleanupStack::Pop( item );

    // Return the insertion position of item.
    if ( pos < 0 )
        return PosLmDisplayDataHandler::FindItemIndex( *iDisplayData, item );
    else
        return pos;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmDisplayItem* CPosLmLocalSearchHandler::CreateItemLC( 
    TPosLmItemId aMatchId,
    RDbRowSet* aRow )
    {
    CPosLmDisplayItem* item = NULL;

    // Need to check if the dispay item is category or landmark.
    if (iCriteriaType == CPosLmSearchCriteria::ECriteriaCategoryByName)
        {
        CPosLandmarkCategory* category = CPosLandmarkCategory::NewLC();
        PosLmImplExtension::SetCategoryIdL( *category, aMatchId );
        if ( aRow )
            {
            PosLmCategoryHandler::ReadCategoryL( *aRow, *iDbAccess, *category );
            }
        else
            {
            PosLmCategoryHandler::ReadCategoryL( *iDbAccess, *category );
            }
        item = CPosLmDisplayItem::NewL( category );
        CleanupStack::Pop( category );
        }
    else
        {
        __ASSERT_DEBUG(iCriteriaType !=
            CPosLmSearchCriteria::ECriteriaComposite,
            User::Leave(KErrNotSupported));

        CPosLandmark* landmark = CPosLandmark::NewLC();
        PosLmImplExtension::SetLandmarkIdL( *landmark, aMatchId );

        if ( iPartialReadParameters )
            {
            if ( aRow )
                {
                PosLmLandmarkHandler::ReadPartialLandmarkL(
                    *aRow, *iDbAccess, *landmark, *iPartialReadParameters);
                }
            else
                {
                PosLmLandmarkHandler::ReadPartialLandmarkL(
                    *iDbAccess, *landmark, *iPartialReadParameters);
                }
            }
        else
            {
            if ( aRow )
                {
                PosLmLandmarkHandler::ReadLandmarkL( *aRow, *iDbAccess, *landmark );
                }
            else
                {
                PosLmLandmarkHandler::ReadLandmarkL( *iDbAccess, *landmark );
                }
            }

        item = CPosLmDisplayItem::NewL( landmark );
        CleanupStack::Pop( landmark );
        }

    CleanupStack::PushL( item );

    return item;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalSearchHandler::PrepareLandmarkIndexL()
    {
    delete iLandmarkIndex;
    iLandmarkIndex = NULL;
    iLandmarkIndex = CPosLmNameIndexIterator::NewL( 
        iDatabase.NameIndex(),
        iLocalSortPref.SortOrder() == TPosLmSortPref::EAscending );

    iTable.Close();
    RDbNamedDatabase db;
    iDbAccess->GetDatabase(db);
    User::LeaveIfError( iTable.Open( db, KPosLmLandmarkTable, RDbRowSet::EReadOnly ) );
    User::LeaveIfError( iTable.SetIndex( KPosLmLandmarkIdIndex ) );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalSearchHandler::EvaluateLandmarkIndexL()
    {
    ASSERT( iLandmarkIndex );
    if ( ( iLandmarkIndex->LoadedCount() == 0 || iLandmarkIndex->AtEnd() ) && 
        iLandmarkIndex->HasMore() )
        {
        GetMoreSortedLandmarksL();
        iProgressHelper.HandleViewEvaluatedL( iLandmarkIndex->Count() );
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalSearchHandler::LoadFullLandmarkIndexL()
    {
    ASSERT( iLandmarkIndex );
    iLandmarkIndex->SetMode( CPosLmNameIndexIterator::EModeFull );
    while ( iLandmarkIndex->HasMore() )
        {
        GetMoreSortedLandmarksL();
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool CPosLmLocalSearchHandler::MoreInLandmarkIndex() const
    {
    ASSERT( iLandmarkIndex );
    return iLandmarkIndex->HasMore() || !iLandmarkIndex->AtEnd();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalSearchHandler::GetMoreSortedLandmarksL()
    {
    ASSERT( iLandmarkIndex );
    iLandmarkIndex->LoadMoreL( KPosLmSortedLandmarkBatchSize );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool CPosLmLocalSearchHandler::GetLandmarkRowL( TPosLmItemId aLmid )
    {
    if ( iTable.SeekL( TDbSeekKey( TUint( aLmid ) ) ) )
        {
        iTable.GetL();
        return ETrue;
        }
    return EFalse;
    }

