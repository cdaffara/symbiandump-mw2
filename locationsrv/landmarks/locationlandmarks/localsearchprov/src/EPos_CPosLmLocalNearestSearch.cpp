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
* Description: Class for handling searching for landmarks nearest a specified position.
*
*
*/


#include <lbs.h>
#include <EPos_CPosLmNearestCriteria.h>
#include <EPos_CPosLandmarkSearch.h>
#include <epos_landmarkdatabasestructure.h>
#include "epos_cposlmnameindexiterator.h"
#include "EPos_CPosLmLocalNearestSearch.h"
#include "EPos_PosLmSqlQueryHelper.h"

const TInt KPosMaxNoOfRowsInStep = 16;
const TInt KPosLmDefaultGranularity = 128;

const TReal32 KPosSearchProgressPart = 0.50;
const TReal32 KPosSortingProgressPart = 0.50;

const TReal64 KEarthRadius = 6371010; // metres

const TReal64 KMinLatitude = -90;
const TReal64 KMaxLatitude = 90;
const TReal64 KMinLongitude = -180;
const TReal64 KMaxLongitude = 180;
const TReal64 KLongitudePeriod = 360;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmLocalNearestSearch::CPosLmLocalNearestSearch(
    CPosLmLocalDatabase& aDatabase,
    const CPosLmSearchCriteria& aCriteria,
    const TPosLmLocalSortPref& aSortPref,
    TInt aMaxNumOfMatches)
    : CPosLmLocalSearchHandler(
        aDatabase, aCriteria.CriteriaType(), aSortPref, aMaxNumOfMatches),
    iSearchStatus(ESearchNotStarted),
    iArray( KPosLmDefaultGranularity )
    {
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalNearestSearch::ConstructL(
    const CPosLmSearchCriteria& aCriteria,
    CPosLmDisplayData* aDisplayData)
    {
    const CPosLmNearestCriteria& criteria =
        static_cast<const CPosLmNearestCriteria&>(aCriteria);

    TCoordinate coordinate;
    criteria.GetCoordinate(coordinate);

    CPosLmNearestCriteria* tmpCriteria = CPosLmNearestCriteria::NewLC(
        coordinate,
        criteria.UseCoverageRadius());
    iCriteria = tmpCriteria;
    CleanupStack::Pop(tmpCriteria);

    iCriteria->SetMaxDistance(criteria.MaxDistance());

    iIncludeAreaCondition = !Math::IsNaN(TRealX(iCriteria->MaxDistance())) &&
                            !iCriteria->UseCoverageRadius();

    if ( aDisplayData )
        {
        SetDisplayDataL( aDisplayData );
        }

    if ( SortPref().SortType() == TPosLmLocalSortPref::ELandmarkSorting )
        {
        iSortSearch = ETrue;
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmLocalNearestSearch* CPosLmLocalNearestSearch::NewL(
    CPosLmLocalDatabase& aDatabase,
    const CPosLmSearchCriteria& aCriteria,
    const TPosLmLocalSortPref& aSortPref,
    CPosLmDisplayData* aDisplayData,
    TInt aMaxNumOfMatches)
    {
    CPosLmLocalNearestSearch* self = new (ELeave) CPosLmLocalNearestSearch(
        aDatabase, aCriteria, aSortPref, aMaxNumOfMatches);
    CleanupStack::PushL(self);
    self->ConstructL(aCriteria, aDisplayData);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmLocalNearestSearch::~CPosLmLocalNearestSearch()
    {
    iArray.Close();
    delete iCriteria;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool CPosLmLocalNearestSearch::IsValidForLandmarkSearch() const
    {
    return ETrue;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TInt CPosLmLocalNearestSearch::NextStepL( TReal32& aProgress )
    {
    if ( iSearchStatus == ESearchNotStarted )
        {
        PrepareSearchL();
        }

    switch ( iSearchStatus )
        {
        case EPrepareNearestSearch:
        case ENearestSearch:
        case EReadDisplayData:
        case EPrepareSorting:
        case ESorting:
            SearchL();
            break;

        default:
            break;
        }

    if ( iSearchStatus == ESearchCompleted )
        {
        iView.Close();
        aProgress = KProgressCompleted;
        return KErrNone;
        }
    else
        {
        if ( !iSortSearch && HasDisplayData() )
            {
            switch ( iSearchStatus )
                {
                case ENearestSearch:
                    aProgress = KPosSearchProgressPart * CalculateProgressL();
                    break;

                case EReadDisplayData:
                    aProgress = KPosSearchProgressPart +
                                KPosSortingProgressPart *
                                iCurrentResult / iArray.Count();
                    break;

                default:
                    ASSERT( 0 ); // This should never happen.
                }
            }
        else
            {
            aProgress = CalculateProgressL();
            }
        return KPosLmOperationNotComplete;
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalNearestSearch::PrepareSearchL()
    {
    TInt numRowsInTable = GetRowCountL( KPosLmLandmarkTable );
    iCurrentResult = 0;

    if ( numRowsInTable == 0 )
        {
        iSearchStatus = ESearchCompleted;
        }
    else
        {
        iSearchStatus = EPrepareNearestSearch;
        if ( iSortSearch ) numRowsInTable *= 2;
        PrepareCalculateProgress( numRowsInTable, iIncludeAreaCondition );
        }
    }

// -----------------------------------------------------------------------------
// Note: all landmarks must have been read before the matches are stored in
// the current matches array. The reason for this is that once a match is
// added to current matches it must remain there. For this class matches
// found during a search might disappear again if max number of matches is
// exceeded. Therefore reading display data or sorting by name is done only
// once all matches are found.
// -----------------------------------------------------------------------------
//
void CPosLmLocalNearestSearch::SearchL()
    {
    if ( iSearchStatus == EPrepareNearestSearch )
    	{
    	PrepareSearchQueryL();
    	iSearchStatus = ENearestSearch;
    	}
    
    TLinearOrder<TPosLmNearestLandmark> order( TPosLmNearestLandmark::CompareByID );
    if ( iSearchStatus == EPrepareSorting )
        {
        PrepareLandmarkIndexL();
        iArray.Sort( order );
        iSearchStatus = ESorting;
        }
    
    if ( iSearchStatus == ENearestSearch )
    	{
    	TInt noOfRows = 0;
	    EvaluateViewL( iView );
	    while ( iView.AtRow() && ( noOfRows++ < KPosMaxNoOfRowsInStep ) )
	        {
	        GetRowL( iView );
	        ReadFromDbAndCalculateAndAddIfMatchL();
	        iView.NextL();
	        }
	
	    if ( !MoreInView( iView ) ) // No more landmarks in db
	        {
	        iView.Close();
	        if ( iArray.Count() == 0 )
	            {
	            iSearchStatus = ESearchCompleted;
	            }
	        else if ( iSortSearch )
	        	{
	        	iSearchStatus = EPrepareSorting;
	        	}
	        else if ( HasDisplayData() )
	            {
	            iSearchStatus = EReadDisplayData;
	            }
	        else
	        	{
		        AddMatchesAsCurrentMatchesL();
	            iSearchStatus = ESearchCompleted;
	        	}
	        }
    	}

    if ( iSearchStatus == EReadDisplayData )
    	{
        TInt ddMatches = DisplayDataCount();
    	while ( iCurrentResult < iArray.Count() && !BreakOnDisplayData( ddMatches ) )
    		{
            TPosLmItemId matchId = iArray[iCurrentResult].iLmId;
            TInt sortPosition = AddMatchToDisplayDataL(
            	matchId, NULL, EFalse, iArray[iCurrentResult].iDistance );
            
            CurrentMatches().InsertL( matchId, sortPosition );
            iCurrentResult++;
    		}

    	if ( iCurrentResult == iArray.Count() )
            {
            // Search is successfully completed.
            iSearchStatus = ESearchCompleted;
            }
    	}

    if ( iSearchStatus == ESorting )
    	{
	    EvaluateLandmarkIndexL();
        TInt ddMatches = DisplayDataCount();
        while ( !iLandmarkIndex->AtEnd() && !BreakOnDisplayData( ddMatches ) )
            {
            const RPosLmLocalNameIndex::CIndexItem& item = iLandmarkIndex->Current();
	
	        TPosLmNearestLandmark sample;
	        sample.iLmId = item.Id();
	        TInt pos = ( iArray.FindInOrder( sample, order ) );
	        if ( pos >= 0 )
	            {
	            TPosLmNearestLandmark nearest = iArray[pos];
            	CurrentMatches().AppendL( nearest.iLmId );
	            if ( HasDisplayData() )
	            	{
	            	AddMatchToDisplayDataL( nearest.iLmId, NULL, ETrue, nearest.iDistance );
	            	}
	            }

            iLandmarkIndex->Next();
            }

	    if ( !MoreInLandmarkIndex() )
	        {
	        iSearchStatus = ESearchCompleted;
	        }
    	}
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalNearestSearch::PrepareSearchQueryL()
    {
    _LIT( KPosLmSql3ColumnsQuery, "SELECT %S, %S, %S FROM %S" );
    _LIT( KPosLmSql4ColumnsQuery, "SELECT %S, %S, %S, %S FROM %S" );

    HBufC* query = HBufC::NewLC( KPosLmSqlStatementMaxLen );

    if ( iCriteria->UseCoverageRadius() )
        {
        query->Des().Format( KPosLmSql4ColumnsQuery,
                            &KPosLmLandmarkIdCol,
                            &KPosLmLatitudeCol,
                            &KPosLmLongitudeCol,
                            &KPosLmCoverageRadiusCol,
                            &KPosLmLandmarkTable );
        }
    else
        {
        query->Des().Format( KPosLmSql3ColumnsQuery,
                            &KPosLmLandmarkIdCol,
                            &KPosLmLatitudeCol,
                            &KPosLmLongitudeCol,
                            &KPosLmLandmarkTable );
        }

    if ( iIncludeAreaCondition )
        {
        TReal64 minLatitude;
        TReal64 maxLatitude;
        TReal64 minLongitude;
        TReal64 maxLongitude;
        CalculateAreaRestrictionsL(
            minLatitude, maxLatitude, minLongitude, maxLongitude );

        PosLmSqlQueryHelper::AppendAreaCriteriaToQueryL(
            query, minLatitude, maxLatitude, minLongitude, maxLongitude );
        }

    PrepareViewL( iView, *query );
    CleanupStack::PopAndDestroy( query );
    }

// -----------------------------------------------------------------------------
// CPosLmLocalNearestSearch::CalculateAreaRestrictionsL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmLocalNearestSearch::CalculateAreaRestrictionsL(
    TReal64& aMinLatitude,
    TReal64& aMaxLatitude,
    TReal64& aMinLongitude,
    TReal64& aMaxLongitude)
    {
    TCoordinate centreCoordinate;
    iCriteria->GetCoordinate(centreCoordinate);

    // Calculate latitude values
    TReal64 centreLatitude = centreCoordinate.Latitude();
    TReal64 dLat = KRadToDeg * iCriteria->MaxDistance() / KEarthRadius;

    aMinLatitude = centreLatitude - dLat;
    aMinLatitude = Max( aMinLatitude, KMinLatitude );

    aMaxLatitude = centreLatitude + dLat;
    aMaxLatitude = Min( aMaxLatitude, KMaxLatitude );

    // Calculate longitude values
    if ( Abs( centreLatitude ) + dLat < KMaxLatitude )
        {
        TReal64 dLatSin;
        User::LeaveIfError( Math::Sin( dLatSin, dLat * KDegToRad ) );

        TReal64 latCos;
        User::LeaveIfError( Math::Cos( latCos, centreLatitude * KDegToRad ) );

        TReal64 squareroot;
        User::LeaveIfError(
            Math::Sqrt( squareroot, latCos * latCos - dLatSin * dLatSin ) );

        TReal64 dLonInRad;
        User::LeaveIfError( Math::ATan( dLonInRad, dLatSin, squareroot ) );

        TReal64 centreLongitude = centreCoordinate.Longitude();
        TReal64 dLon = dLonInRad * KRadToDeg;

        aMinLongitude = centreLongitude - dLon;
        if ( aMinLongitude < KMinLongitude )
            {
            aMinLongitude += KLongitudePeriod;
            }

        aMaxLongitude = centreLongitude + dLon;
        if (aMaxLongitude > KMaxLongitude)
            {
            aMaxLongitude -= KLongitudePeriod;
            }
        }
    else
        {
        aMinLongitude = KMinLongitude;
        aMaxLongitude = KMaxLongitude;
        }
    }

// -----------------------------------------------------------------------------
// CPosLmLocalNearestSearch::ReadFromDbAndCalculateAndAddIfMatchL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmLocalNearestSearch::ReadFromDbAndCalculateAndAddIfMatchL()
    {
    const TInt KLmIdCol = 1;
    const TInt KLatitudeCol = 2;
    const TInt KLongitudeCol = 3;
    const TInt KRadiusCol = 4;
    
    TPosLmNearestLandmark nearestLandmark;
    nearestLandmark.iLmId = iView.ColUint32(KLmIdCol);

    if (!MatchesPreviousMatches(nearestLandmark.iLmId))
        {
        // Skip landmark when it is not included in previous matches.
        return;
        }

    if (iView.IsColNull(KLatitudeCol) || iView.IsColNull(KLongitudeCol))
        {
        // Skip landmark that has no latitude and/or longitude.
        return;
        }

    TReal64 latitude = iView.ColReal64(KLatitudeCol);
    TReal64 longitude = iView.ColReal64(KLongitudeCol);
    TCoordinate lmCoordinate(latitude, longitude);

    TCoordinate coordinate;
    iCriteria->GetCoordinate(coordinate);

    // Calculate the distance between the two coordinates.
    User::LeaveIfError(coordinate.Distance(lmCoordinate,
                                           nearestLandmark.iDistance));

    if (iCriteria->UseCoverageRadius() &&
        !iView.IsColNull(KRadiusCol))
        {
        TReal32 coverageRadius = iView.ColReal32(KRadiusCol);

        if (nearestLandmark.iDistance > coverageRadius)
            {
            // Calculate the distance to the landmark coverage area border.
            nearestLandmark.iDistance -= coverageRadius;
            }
        else
            {
            // When coordinate is inside the landmark coverage area,
            // the distance will be considered zero.
            nearestLandmark.iDistance = 0;
            }
        }

    if (!Math::IsNaN(TRealX(iCriteria->MaxDistance())) &&
        nearestLandmark.iDistance > iCriteria->MaxDistance())
        {
        // Skip current match when its distance exceeds the max distance.
        return;
        }

    // Add landmark in distance ascending order.
    User::LeaveIfError(iArray.InsertInOrderAllowRepeats(nearestLandmark,
        TLinearOrder<TPosLmNearestLandmark>(TPosLmNearestLandmark::Compare)));

    if (MaxNumOfMatches() != KPosLmMaxNumOfMatchesUnlimited &&
        iArray.Count() > MaxNumOfMatches())
        {
        // Remove last item if max number of matches has been exceeded.
        iArray.Remove(iArray.Count() - 1);
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalNearestSearch::AddMatchesAsCurrentMatchesL()
    {
    // Note: all landmarks must have been read before the matches are stored in
    // the current matches array. The reason for this is that once a match is
    // added to current matches it must remain there. For this class matches
    // found during a search might disappear again if max number of matches is
    // exceeded.

    RArray<TPosLmItemId>& currentMatches = CurrentMatches();

    currentMatches.Reset();
    currentMatches.ReserveL( iArray.Count() ); // to avoid heap defragmentation
    for ( TInt i = 0; i < iArray.Count(); i++ )
        {
        currentMatches.Append( iArray[i].iLmId );
        }
    }

//  End of File
