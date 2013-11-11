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
* Description: Class for handling Composite search.
*
*
*/


#include    <EPos_CPosLmCompositeCriteria.h>
#include    <EPos_CPosLmIdListCriteria.h>
#include    <EPos_CPosLandmarkSearch.h>
#include    <epos_landmarkdatabasestructure.h>
#include    "EPos_CPosLmLocalCompositeSearch.h"
#include     "EPos_CPosLmLocalTextSearch.h"

const TInt KPosLmDefaultGranularity = 128;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmLocalCompositeSearch::CPosLmLocalCompositeSearch(
    CPosLmLocalDatabase& aDatabase,
    const CPosLmSearchCriteria& aCriteria,
    const TPosLmLocalSortPref& aSortPref,
    TInt aMaxNumOfMatches )
:   CPosLmLocalSearchHandler(
        aDatabase, aCriteria.CriteriaType(), aSortPref, aMaxNumOfMatches ),
    iSearchStatus( EPosSearchNotStarted ),
    iIdList( KPosLmDefaultGranularity )
    {
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalCompositeSearch::ConstructL(
    CPosLmLocalDatabase& aDatabase,
    const CPosLmSearchCriteria& aCriteria,
    const TPosLmLocalSortPref& aSortPref,
    CPosLmDisplayData* aDisplayData,
    TInt aMaxNumOfMatches)
    {
    const CPosLmCompositeCriteria& composite =
        static_cast<const CPosLmCompositeCriteria&>(aCriteria);

    TUint noOfArguments = composite.NumOfArguments();
    if (noOfArguments == 0)
        {
        User::Leave(KErrArgument);
        }

    // First take care of any id list criteria.
    ConstructIdListL( composite );

    TInt noOfArgumentsNotIdList = noOfArguments;
    if ( iIdList.Count() )
        {
        noOfArgumentsNotIdList -= 1;
        }

	
	//now create all the text criterias
	TInt textCriteriaCount = CreateAllTextCriteriaL(
			aDatabase, composite, aSortPref, aDisplayData,
	        aMaxNumOfMatches, noOfArgumentsNotIdList );
	 
	 //Number of component will be availble in icomponent will be 
	 //noOfArgumentsNotIdList - textCriteriaCount + iComponent.Count()
	 //Because for all the text criteria there will be only one component
	TInt noOfComponent = noOfArgumentsNotIdList - textCriteriaCount + iComponents.Count();
		
	    // Then create handlers for all other criteria except nearest.
	    TBool lastCriteriaFound = ConstructAllButNearestL(
	        aDatabase, composite, aSortPref, aDisplayData,
	        aMaxNumOfMatches, noOfComponent);

	    if (!lastCriteriaFound)
	        {
	        // At last, create handlers for nearest criteria.
	        ConstructNearestL(
	            aDatabase, composite, aSortPref, aDisplayData,
	            aMaxNumOfMatches, noOfArgumentsNotIdList);
	        }
        
    
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmLocalCompositeSearch* CPosLmLocalCompositeSearch::NewL(
    CPosLmLocalDatabase& aDatabase,
    const CPosLmSearchCriteria& aCriteria,
    const TPosLmLocalSortPref& aSortPref,
    CPosLmDisplayData* aDisplayData,
    TInt aMaxNumOfMatches)
    {
    CPosLmLocalCompositeSearch* self = new (ELeave) CPosLmLocalCompositeSearch(
        aDatabase, aCriteria, aSortPref, aMaxNumOfMatches);
    CleanupStack::PushL(self);
    self->ConstructL(
        aDatabase, aCriteria, aSortPref, aDisplayData, aMaxNumOfMatches);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmLocalCompositeSearch::~CPosLmLocalCompositeSearch()
    {
    iComponents.ResetAndDestroy();
    iIdList.Close();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool CPosLmLocalCompositeSearch::IsValidForLandmarkSearch() const
    {
    TBool isValid = ETrue;

    for (TInt i = 0; i < iComponents.Count() && isValid; i++)
        {
        isValid = iComponents[i]->IsValidForLandmarkSearch();
        }

    return isValid;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TInt CPosLmLocalCompositeSearch::NextStepL(
    TReal32& aProgress)
    {
    if (iSearchStatus == EPosSearchNotStarted)
        {
        PrepareSearchL();
        }

    switch (iSearchStatus)
        {
        case EPosSearch:
            SearchL(aProgress);
            break;
        default:
            break;
        }

    TInt status = KPosLmOperationNotComplete;
    if (iSearchStatus == EPosSearchCompleted)
        {
        CleanupSources();
        aProgress = KProgressCompleted;
        status = KErrNone;
        }

    return status;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalCompositeSearch::CancelSearch()
    {
    if (iComponentIndex < iComponents.Count())
        {
        iComponents[iComponentIndex]->CancelSearch();
        }
    else
        {
        CPosLmLocalSearchHandler::CancelSearch();
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalCompositeSearch::GetMatchesL(
    RArray<TPosLmItemId>& aMatches) const
    {
    iComponents[iComponents.Count() - 1]->GetMatchesL(aMatches);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
const RArray<TPosLmItemId>& CPosLmLocalCompositeSearch::Matches() const
    {
    return iComponents[iComponents.Count() - 1]->Matches();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalCompositeSearch::ConstructIdListL(
    const CPosLmCompositeCriteria& aComposite)
    {
    for ( TUint i = 0; i < aComposite.NumOfArguments(); i++ )
        {
        const CPosLmSearchCriteria& component = aComposite.Argument( i );

        if ( component.CriteriaType() == CPosLmSearchCriteria::ECriteriaIdList )
            {
            if ( iHasIdList )
                {
                // Not allowed to have several idListCriteria.
                User::Leave( KErrArgument );
                }
            iHasIdList = ETrue;

            const CPosLmIdListCriteria& idListCriteria =
                static_cast<const CPosLmIdListCriteria&>( component );

            iIdList.Reset();
            idListCriteria.GetLandmarkIdsL( iIdList );
            if ( iIdList.Count() == 0 )
                {
                // Not allowed to have empty IdListCriteria.
                User::Leave( KErrArgument );
                }
            }
        }

    if ( iHasIdList && aComposite.NumOfArguments() == 1 )
        {
        // Not allowed to have idListCriteria as the only criterion.
        User::Leave( KErrArgument );
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool CPosLmLocalCompositeSearch::ConstructAllButNearestL(
    CPosLmLocalDatabase& aDatabase,
    const CPosLmCompositeCriteria& aComposite,
    const TPosLmLocalSortPref& aSortPref,
    CPosLmDisplayData* aDisplayData,
    TInt aMaxNumOfMatches,
    TInt aNoOfCriteriaNotIdList)
    {
    TBool lastCriteriaFound = EFalse;

    TUint i;
    for (i = 0; i < aComposite.NumOfArguments() && !lastCriteriaFound; i++)
        {
        const CPosLmSearchCriteria& component = aComposite.Argument(i);

        CPosLmSearchCriteria::TCriteriaType type = component.CriteriaType();

        if (type != CPosLmSearchCriteria::ECriteriaIdList &&
            type != CPosLmSearchCriteria::ECriteriaFindNearest &&
            type != CPosLmSearchCriteria::ECriteriaText )
            {
            lastCriteriaFound = CreateAndAppendHandlerL(
                aDatabase, component, aSortPref, aDisplayData,
                aMaxNumOfMatches, aNoOfCriteriaNotIdList);
            }
        }

    return lastCriteriaFound;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalCompositeSearch::ConstructNearestL(
    CPosLmLocalDatabase& aDatabase,
    const CPosLmCompositeCriteria& aComposite,
    const TPosLmLocalSortPref& aSortPref,
    CPosLmDisplayData* aDisplayData,
    TInt aMaxNumOfMatches,
    TInt aNoOfCriteriaNotIdList)
    {
    TBool lastCriteriaFound = EFalse;

    TUint i;
    for (i = 0; i < aComposite.NumOfArguments() && !lastCriteriaFound; i++)
        {
        const CPosLmSearchCriteria& component = aComposite.Argument(i);

        if (component.CriteriaType() ==
            CPosLmSearchCriteria::ECriteriaFindNearest)
            {
            lastCriteriaFound = CreateAndAppendHandlerL(
                aDatabase, component, aSortPref, aDisplayData,
                aMaxNumOfMatches, aNoOfCriteriaNotIdList);
            }
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool CPosLmLocalCompositeSearch::CreateAndAppendHandlerL(
    CPosLmLocalDatabase& aDatabase,
    const CPosLmSearchCriteria& aCriteria,
    const TPosLmLocalSortPref& aSortPref,
    CPosLmDisplayData* aDisplayData,
    TInt aMaxNumOfMatches,
    TInt aNoOfCriteriaNotIdList)
    {
    TBool isLastCriteria = EFalse;
    if (iComponents.Count() + 1 == aNoOfCriteriaNotIdList)
        {
        isLastCriteria = ETrue;
        }

    CPosLmLocalSearchHandler* handler;
    if (isLastCriteria)
        {
        handler = CPosLmLocalSearchHandler::CreateHandlerL(
            aDatabase, aCriteria, aSortPref, aDisplayData, aMaxNumOfMatches);
        }
    else
        {
        TPosLmLocalSortPref unsorted;

        handler = CPosLmLocalSearchHandler::CreateHandlerL(
            aDatabase, aCriteria, unsorted, NULL,
            KPosLmMaxNumOfMatchesUnlimited);
        }

    CleanupStack::PushL(handler);
    User::LeaveIfError(iComponents.Append(handler));
    CleanupStack::Pop(handler);

    return isLastCriteria;
    }

// -----------------------------------------------------------------------------
// ID List criteria is is handled as previous matches for the first search handler.
// If previous matches set too, then they should be merged with ID list by AND
// -----------------------------------------------------------------------------
//
void CPosLmLocalCompositeSearch::PrepareSearchL()
    {
    iSearchStatus = EPosSearch;
    iComponentIndex = 0;

    if ( iHasIdList )
        {
        if ( PreviousMatches().Count() != 0 )
            {
            // Go through idArray and check that the items are
            // included in previous matches, if not remove the item.
            for ( TInt i = iIdList.Count() - 1; i >= 0 ; i-- )
                {
                if ( !MatchesPreviousMatches( iIdList[i] ) )
                    {
                    iIdList.Remove( i );
                    }
                }
            }

        if ( iIdList.Count() == 0 )
            {
            // matching to empty ID list criteria will yield no results
            iSearchStatus = EPosSearchCompleted;
            }
        else
            {
            PrepareNextHandlerL( iIdList );
            }
        }
    else if ( PreviousMatches().Count() != 0 )
        {
        // copy previous matches to iIdList
        iIdList.Reset();
        iIdList.ReserveL( PreviousMatches().Count() );
        for ( int i = 0; i < PreviousMatches().Count(); ++i )
            {
            iIdList.AppendL( PreviousMatches()[i] );
            }
        PrepareNextHandlerL( iIdList );
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalCompositeSearch::PrepareNextHandlerL(
    const RArray<TPosLmItemId>& aPreviousMatches)
    {
    iComponents[iComponentIndex]->SetPreviousMatchesToSearchL(aPreviousMatches);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalCompositeSearch::SearchL(TReal32& aProgress)
    {
    TReal32 componentProgress;
    TInt result = iComponents[iComponentIndex]->NextStepL(componentProgress);

    const RArray<TPosLmItemId>& matches =
        iComponents[iComponentIndex]->Matches();

    if (iComponentIndex == iComponents.Count() - 1)
        {
        // Add matches found in last search handler
        AddMatchesAsCurrentMatchesL(matches);
        }

    aProgress = CalculateCompositeProgressL(componentProgress);

    if (result == KErrNone)
        {
        if (matches.Count() == 0)
            {
            iSearchStatus = EPosSearchCompleted;
            }
        else if (++iComponentIndex < iComponents.Count())
            {
            // Prepare next search handler
            PrepareNextHandlerL(matches);
            }
        else
            {
            // All matches found
            // Search is successfully completed.
            iSearchStatus = EPosSearchCompleted;
            }
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TReal32 CPosLmLocalCompositeSearch::CalculateCompositeProgressL(
    TReal32 aComponentProgress)
    {
    TInt denominator = iComponents.Count();

    return (iComponentIndex + aComponentProgress) / denominator;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLocalCompositeSearch::AddMatchesAsCurrentMatchesL(
    const RArray<TPosLmItemId>& aMatches)
    {
    // Note: all search handlers must have been used before the matches are
    // stored in the current matches array. The reason for this is that once a
    // match is added to current matches it must remain there. For this class,
    // matches found in one search handler might disappear again in next search
    // handler.
    RArray<TPosLmItemId>& compositeMatches = CurrentMatches();

    while (iArrayIndex < aMatches.Count())
        {
        User::LeaveIfError(compositeMatches.Append(aMatches[iArrayIndex]));
        iArrayIndex++;
        }
    }
//----------------------------------------------------------------
//--------------------------------------------------------------------------------
//
TInt CPosLmLocalCompositeSearch::CreateAllTextCriteriaL(
			CPosLmLocalDatabase& aDatabase,
            const CPosLmCompositeCriteria& aCriteria,
            const TPosLmLocalSortPref& aSortPref,
            CPosLmDisplayData* aDisplayData,
            TInt aMaxNumOfMatches,
            TInt /*aNoOfCriteriaNotIdList*/ )
{
	RPointerArray<CPosLmSearchCriteria> aSearchTextCriteriaArray;
	for(int i=0; i< aCriteria.NumOfArguments(); i++)
	{
		if(aCriteria.Argument(i).CriteriaType() == CPosLmSearchCriteria::ECriteriaText)
		{
			aSearchTextCriteriaArray.AppendL(&(aCriteria.Argument(i)));
		
		}
			
	}
	
	if(aSearchTextCriteriaArray.Count() > 0)
	{
		if(aSearchTextCriteriaArray.Count() == aCriteria.NumOfArguments())
		{
			
			CPosLmLocalTextSearch* handler = CPosLmLocalTextSearch::NewL(
            aDatabase, aSearchTextCriteriaArray, aSortPref, aDisplayData, aMaxNumOfMatches);
	
			handler->SetCompositionType(aCriteria.CompositionType());
			CleanupStack::PushL(handler);
    		User::LeaveIfError(iComponents.Append(handler));
    		CleanupStack::Pop(handler);
		}
		else
		{
			TPosLmLocalSortPref unsorted;
			CPosLmLocalTextSearch* handler = CPosLmLocalTextSearch::NewL(
            aDatabase, aSearchTextCriteriaArray, unsorted, NULL, aMaxNumOfMatches);
	
			handler->SetCompositionType(aCriteria.CompositionType());
			CleanupStack::PushL(handler);
    		User::LeaveIfError(iComponents.Append(handler));
    		CleanupStack::Pop(handler);
		}
		
		
	}
	
	TInt textCriteriaCount = aSearchTextCriteriaArray.Count();
	aSearchTextCriteriaArray.Reset();
	return textCriteriaCount;
	
	
}