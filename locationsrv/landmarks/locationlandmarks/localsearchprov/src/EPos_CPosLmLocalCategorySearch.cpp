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
* Description: PRovides search handling related to category
*
*
*/


#include <EPos_CPosLmCategoryCriteria.h>
#include <epos_landmarkdatabasestructure.h>
#include <epos_poslmdatabaseutility.h>
#include "EPos_CPosLmLocalCategorySearch.h"
#include "epos_cposlmnameindexiterator.h"

const TInt KLmIdColumn = 1;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmLocalCategorySearch::CPosLmLocalCategorySearch(
    CPosLmLocalDatabase& aDatabase,
    const CPosLmSearchCriteria& aCriteria,
    const TPosLmLocalSortPref& aSortPref,
    TInt aMaxNumOfMatches)
:   CPosLmLocalSearchHandler(
        aDatabase, aCriteria.CriteriaType(), aSortPref, aMaxNumOfMatches),
    iSearchStatus(ESearchNotStarted)
    {
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalCategorySearch::ConstructL(
    const CPosLmSearchCriteria& aCriteria,
    CPosLmDisplayData* aDisplayData )
    {
    const CPosLmCategoryCriteria& criteria =
        static_cast<const CPosLmCategoryCriteria&>( aCriteria );

    iCriteria = CPosLmCategoryCriteria::NewLC();
    CleanupStack::Pop( iCriteria );

    if ( criteria.CategoryItemId() != KPosLmNullItemId )
        {
        iCriteria->SetCategoryItemId( criteria.CategoryItemId() );
        iSearchType = ECategoryItemIdSearch;
        }
    else if ( criteria.CategoryName().Length() > 0 )
        {
        HBufC* embeddedText = PosLmDatabaseUtility::EmbedSingleQuoteCharactersLC(
            criteria.CategoryName() );
        
        iCriteria->SetCategoryNameL( *embeddedText );
        CleanupStack::PopAndDestroy( embeddedText );

        iSearchType = ECategoryNameSearch;
        }
    else if ( criteria.GlobalCategory() != KPosLmNullGlobalCategory )
        {
        iCriteria->SetGlobalCategory( criteria.GlobalCategory() );
        iSearchType = EGlobalCategorySearch;
        }
    else
        {
        iSearchType = EUncategorizedSearch;
        }

    if ( aDisplayData )
        {
        SetDisplayDataL( aDisplayData );
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmLocalCategorySearch* CPosLmLocalCategorySearch::NewL(
    CPosLmLocalDatabase& aDatabase,
    const CPosLmSearchCriteria& aCriteria,
    const TPosLmLocalSortPref& aSortPref,
    CPosLmDisplayData* aDisplayData,
    TInt aMaxNumOfMatches)
    {
    CPosLmLocalCategorySearch* self = new (ELeave) CPosLmLocalCategorySearch(
        aDatabase, aCriteria, aSortPref, aMaxNumOfMatches);
    CleanupStack::PushL(self);
    self->ConstructL(aCriteria, aDisplayData);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmLocalCategorySearch::~CPosLmLocalCategorySearch()
    {
    iLmids.Close();
    delete iCriteria;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool CPosLmLocalCategorySearch::IsValidForLandmarkSearch() const
    {
    return ETrue;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TInt CPosLmLocalCategorySearch::NextStepL( TReal32& aProgress )
    {
    if ( iSearchStatus == ESearchNotStarted )
    	{
    	PrepareSearchL();
    	}
    
    switch ( iSearchStatus )
        {
        case EPrepareUnsortedSearch:
        case EPrepareUnsortedCategorySearch:
        case EUnsortedCategorySearch:
            UnsortedSearchL();
            break;
            
        case EPrepareSortedSearch:
        case EPrepareSortedCategorySearch:
        case EPrepareSortedLandmarkSearch:
        case ESortedCategorySearch:
        case ESortedLandmarkSearch:
            SortedSearchL();
            break;
            
        default:
            break;
        }
    
    return SearchStepCompletedL( iSearchStatus == ESearchCompleted, aProgress );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalCategorySearch::CancelSearch()
    {
    iLmids.Close();
    CPosLmLocalSearchHandler::CancelSearch();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalCategorySearch::PrepareSearchL()
    {
    TInt numRows = GetRowCountL( KPosLmLandmarkCategoryTable );
    
    if ( SortPref().SortType() == TPosLmLocalSortPref::ELandmarkSorting ||
        iSearchType == EUncategorizedSearch )
       {
       // Uncategorized search requires evaluation of all 
       // Lm IDs from LmCat table and comparing them
       // to full set of Lm IDs. But no checks in
       // Lm table needed, so we can just take Lm IDs from index,
       // which is faster than taking them from Lm table.
       // Therefore, uncategorized search is done in the same way
       // as sorted search, regardless of sorting preference.
       iSortSearch = ETrue;
       iSearchStatus = EPrepareSortedSearch;

       // All Lm IDs will be analyzed also
       numRows += GetRowCountL( KPosLmLandmarkTable );
       }
   else
       {
       iSortSearch = EFalse;
       iSearchStatus = EPrepareUnsortedSearch;
       }

    if ( numRows == 0 )
        {
        iSearchStatus = ESearchCompleted;
        }
    else
        {
        PrepareCalculateProgress( numRows );
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalCategorySearch::PrepareCategorizedSearchL( TPosLmItemId aCategoryId )
    {
    HBufC* query = CreateCategoryItemIdQueryLC( aCategoryId );
    PrepareViewL( iView, *query, EDbCompareNormal );
    CleanupStack::PopAndDestroy( query );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalCategorySearch::PrepareUncategorizedSearchL()
    {
    HBufC* query = HBufC::NewLC( KPosLmSqlStatementMaxLen );

    query->Des().Format(
    	KPosLmSqlSelectOrderByString,
        &KPosLmLandmarkIdCol,
        &KPosLmLandmarkCategoryTable,
        &KPosLmLandmarkIdCol );

    PrepareViewL( iView, *query, EDbCompareNormal );
    CleanupStack::PopAndDestroy( query );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TPosLmItemId CPosLmLocalCategorySearch::FindCategoryIdL()
    {
    TPosLmItemId catItemId = KPosLmNullItemId;

    if ( iSearchType == ECategoryItemIdSearch )
        {
        catItemId = iCriteria->CategoryItemId();
        }
    else
        {
        HBufC* query = HBufC::NewLC( KPosLmSqlStatementMaxLen );
        if ( iSearchType == ECategoryNameSearch )
            {
            TPtrC catName = iCriteria->CategoryName();
            query->Des().Format(
            	KPosLmSqlFindString,
                &KPosLmCategoryIdCol,
                &KPosLmCategoryTable,
                &KPosLmCategoryNameCol,
                &catName );
            }
        else //iSearchType == EGlobalCategorySearch
            {
            query->Des().Format(
            	KPosLmSqlFindUint,
                &KPosLmCategoryIdCol,
                &KPosLmCategoryTable,
                &KPosLmGlobalCategoryIdCol,
                iCriteria->GlobalCategory() );
            }

        PrepareViewL( iView, *query, EDbCompareNormal );
        CleanupStack::PopAndDestroy( query );

        User::LeaveIfError( iView.EvaluateAll() );

        if ( iView.NextL() )
            {
            iView.GetL();
            catItemId = iView.ColUint32( 1 ); // Category ID
            }
        iView.Close();
        }

    return catItemId;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
HBufC* CPosLmLocalCategorySearch::CreateCategoryItemIdQueryLC(
    const TPosLmItemId aItemId)
    {
    HBufC* query = HBufC::NewLC(KPosLmSqlStatementMaxLen);

    query->Des().Format(KPosLmSqlFindUint,
                    &KPosLmLandmarkIdCol,
                    &KPosLmLandmarkCategoryTable,
                    &KPosLmCategoryIdCol,
                    aItemId);
    return query;
    }

// -----------------------------------------------------------------------------
// Sequence of this case:
// Search LmCat table, results added to the end
// Search stops when all records are analyzed, or once max number of matches reached
// -----------------------------------------------------------------------------
//
void CPosLmLocalCategorySearch::UnsortedSearchL()
    {
    if ( iSearchStatus == EPrepareUnsortedCategorySearch  ||
         iSearchStatus == EPrepareUnsortedSearch )
        {
        TPosLmItemId catId = FindCategoryIdL(); 
        if ( catId != KPosLmNullItemId )
        	{
        	PrepareCategorizedSearchL( catId );
            iSearchStatus = EUnsortedCategorySearch;
        	}
        else
        	{
        	iSearchStatus = ESearchCompleted;
        	}
        }

    if ( iSearchStatus == EUnsortedCategorySearch )
        {
        TBool done = SearchViewL( iView, KLmIdColumn, EFalse ); 
        if ( done || IsMaxMatchesFound() )
            {
            iView.Close();
            iSearchStatus = ESearchCompleted; 
            }
        }
    }

// -----------------------------------------------------------------------------
// The trick of this case is that results cannot disappear from "current matches"
// (intermediate iterator of matches). Therefore need to search all LmCat table first 
// and merge results with landmarks.
// -----------------------------------------------------------------------------
//
void CPosLmLocalCategorySearch::SortedSearchL()
    {
    TInt err = KErrNone;
    
    if ( iSearchStatus == EPrepareSortedCategorySearch ||
         iSearchStatus == EPrepareSortedSearch )
        {
        if ( iSearchType != EUncategorizedSearch )
        	{
            TPosLmItemId catId = FindCategoryIdL(); 
            if ( catId != KPosLmNullItemId )
            	{
            	PrepareCategorizedSearchL( catId );
                iSearchStatus = ESortedCategorySearch;
            	}
            else
            	{
            	iSearchStatus = ESearchCompleted;
            	}
        	}
        else
        	{	
        	PrepareUncategorizedSearchL();
        	iSearchStatus = ESortedCategorySearch;
        	}
        }

    if ( iSearchStatus == EPrepareSortedLandmarkSearch )
        {
        PrepareLandmarkIndexL();
        iSearchStatus = ESortedLandmarkSearch;
        }

    if ( iSearchStatus == ESortedCategorySearch )
        {
        EvaluateViewL( iView );
        iLmids.ReserveL( iView.CountL() );

        while ( iView.AtRow() )
            {
            GetRowL( iView );
            err = iLmids.InsertInOrder( iView.ColUint32( KLmIdColumn ) ); // Lm ID
            if ( err && err != KErrAlreadyExists )
                {
                User::Leave( err );
                }
            iView.NextL();
            }
        
        if ( !MoreInView( iView ) )
            {
            iView.Close();
            iSearchStatus = EPrepareSortedLandmarkSearch;
            }
        }
    
    if ( iSearchStatus == ESortedLandmarkSearch )
        {
        EvaluateLandmarkIndexL();
        TInt ddMatches = DisplayDataCount();
        while ( !iLandmarkIndex->AtEnd() && !IsMaxMatchesFound() && !BreakOnDisplayData( ddMatches ) )
            {
            const RPosLmLocalNameIndex::CIndexItem& item = iLandmarkIndex->Current();

            // check results from LmCat table
            TBool found = ( iLmids.FindInOrder( item.Id() ) >= 0 );
            if ( ( found && iSearchType != EUncategorizedSearch ) ||
                 ( !found && iSearchType == EUncategorizedSearch ) )
                {
                AddMatchL( item.Id() );
                }

            iLandmarkIndex->Next();
            }

        if ( !MoreInLandmarkIndex() || IsMaxMatchesFound() )
            {
            iSearchStatus = ESearchCompleted;
            }
        }
    }
