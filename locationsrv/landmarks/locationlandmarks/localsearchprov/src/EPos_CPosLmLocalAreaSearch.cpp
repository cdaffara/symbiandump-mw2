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
* Description: Class for handling area searching for landmarks.
*
*
*/


#include <lbs.h>
#include <EPos_CPosLmAreaCriteria.h>
#include <epos_landmarkdatabasestructure.h>
#include "EPos_CPosLmLocalAreaSearch.h"
#include "EPos_PosLmSqlQueryHelper.h"
#include "epos_cposlmnameindexiterator.h"

const TInt KLmIdColumn = 1;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmLocalAreaSearch::CPosLmLocalAreaSearch(
    CPosLmLocalDatabase& aDatabase,
    const CPosLmSearchCriteria& aCriteria,
    const TPosLmLocalSortPref& aSortPref,
    TInt aMaxNumOfMatches)
:   CPosLmLocalSearchHandler(
        aDatabase, aCriteria.CriteriaType(), aSortPref, aMaxNumOfMatches ),
    iSearchStatus( ESearchNotStarted )
    {
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalAreaSearch::ConstructL(
    const CPosLmSearchCriteria& aCriteria,
    CPosLmDisplayData* aDisplayData)
    {
    const CPosLmAreaCriteria& criteria =
        static_cast<const CPosLmAreaCriteria&>( aCriteria );

    criteria.GetSearchArea( iSouth, iNorth, iWest, iEast );

    if ( aDisplayData )
        {
        SetDisplayDataL( aDisplayData );
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmLocalAreaSearch* CPosLmLocalAreaSearch::NewL(
    CPosLmLocalDatabase& aDatabase,
    const CPosLmSearchCriteria& aCriteria,
    const TPosLmLocalSortPref& aSortPref,
    CPosLmDisplayData* aDisplayData,
    TInt aMaxNumOfMatches)
    {
    CPosLmLocalAreaSearch* self = new (ELeave) CPosLmLocalAreaSearch(
        aDatabase, aCriteria, aSortPref, aMaxNumOfMatches);
    CleanupStack::PushL(self);
    self->ConstructL(aCriteria, aDisplayData);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmLocalAreaSearch::~CPosLmLocalAreaSearch()
    {
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool CPosLmLocalAreaSearch::IsValidForLandmarkSearch() const
    {
    return ETrue;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TInt CPosLmLocalAreaSearch::NextStepL( TReal32& aProgress )
    {
    if ( iSearchStatus == ESearchNotStarted )
        {
        PrepareSearchL();
        }

    switch ( iSearchStatus )
        {
        case EPrepareUnsortedSearch:
        case EUnsortedSearch:
            UnsortedSearchL();
            break;
        case EPrepareSortedSearch:
        case ESortedSearch:
            SortedSearchL();
            break;
        default:
            iSearchStatus = ESearchCompleted;
            break;
        }

    return SearchStepCompletedL( iSearchStatus == ESearchCompleted, aProgress );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalAreaSearch::PrepareSearchL()
    {
    TInt numRowsInTable = GetRowCountL( KPosLmLandmarkTable );

    if ( numRowsInTable == 0 )
        {
        iSearchStatus = ESearchCompleted;
        }
    else
        {
        PrepareCalculateProgress( numRowsInTable );
        if ( SortPref().SortType() == TPosLmLocalSortPref::ELandmarkSorting )
            {
            iSearchStatus = EPrepareSortedSearch;
            }
        else
            {
            iSearchStatus = EPrepareUnsortedSearch;
            }
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalAreaSearch::PrepareQueryL( TBool aFullData )
    {
    HBufC* query = HBufC::NewLC( KPosLmSqlStatementMaxLen );
    if ( aFullData )
    	{
    	query->Des().Format( KPosLmSqlSelect, &KPosLmSqlAll, &KPosLmLandmarkTable );
    	}
    else
    	{
    	query->Des().Format( KPosLmSqlSelect, &KPosLmLandmarkIdCol, &KPosLmLandmarkTable );
    	}

    PosLmSqlQueryHelper::AppendAreaCriteriaToQueryL(
        query, iSouth, iNorth, iWest, iEast );

    PrepareViewL( iView, *query );
    CleanupStack::PopAndDestroy( query );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalAreaSearch::UnsortedSearchL()
    {
    if ( iSearchStatus == EPrepareUnsortedSearch )
        {
        PrepareQueryL( HasDisplayData() );
        iSearchStatus = EUnsortedSearch;
        }

    if ( iSearchStatus == EUnsortedSearch )
        {
        if ( SearchViewL( iView, KLmIdColumn ) || IsMaxMatchesFound() )
            {
            iSearchStatus = ESearchCompleted; 
            }
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalAreaSearch::SortedSearchL()
    {
    if ( iSearchStatus == EPrepareSortedSearch )
        {
        PrepareLandmarkIndexL();
        iSearchStatus = ESortedSearch;
        }

    if ( iSearchStatus == ESortedSearch )
        {
        EvaluateLandmarkIndexL();
        TInt ddMatches = DisplayDataCount();
        while ( !iLandmarkIndex->AtEnd() && !IsMaxMatchesFound() && !BreakOnDisplayData( ddMatches ) )
            {
            const RPosLmLocalNameIndex::CIndexItem& item = iLandmarkIndex->Current();
            if ( GetLandmarkRowL( item.Id() ) && CurrentRowMatch( iTable ) )
                {
                AddMatchL( item.Id(), &iTable );
                }
            iLandmarkIndex->Next();
            }

        if ( !MoreInLandmarkIndex() || IsMaxMatchesFound() )
            {
            iSearchStatus = ESearchCompleted;
            }
        }
    }

//-------------------------------------------------------------
//-------------------------------------------------------------
//
TBool CPosLmLocalAreaSearch::CurrentRowMatch( RDbRowSet& aView )
    {
    if ( !aView.IsColNull( EPosLmLcLatitudeCol ) &&
         !aView.IsColNull( EPosLmLcLongitudeCol ) )
        {
        TRealX nan;
        nan.SetNaN();
        TReal lat = nan, lon = nan;
        
        lat = aView.ColReal( EPosLmLcLatitudeCol );
        lon = aView.ColReal( EPosLmLcLongitudeCol );
    
        if ( !Math::IsNaN( lat ) && !Math::IsNaN( lon ) )
            {
            if ( ( lat >= iSouth ) && ( lat <= iNorth ) )
                {
                if ( iWest <= iEast )
                    {
                    return ( lon >= iWest ) && ( lon <= iEast );
                    }
                else // cross 180 meridian
                    {
                    return ( lon >= iWest ) || ( lon <= iEast );                
                    }
                }
            }
        }
    return EFalse;
    }
