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
*/


#ifndef CPOSLMLOCALCOMPOSITESEARCH_H
#define CPOSLMLOCALCOMPOSITESEARCH_H

#include "EPos_CPosLmLocalSearchHandler.h"

class CPosLmLocalDatabase;
class CPosLmCompositeCriteria;
class CPosLmIdListCriteria;

/**
*  Class for handling Composite search.
*/
class CPosLmLocalCompositeSearch : public CPosLmLocalSearchHandler
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        *
        * @param[in] aDbAccess The access handle to the database to search in.
        * @param[in] aCriteria The criteria for the searching.
        * @param[in] aSortPref A sort preference object.
        * @param[in] aDisplayData The displayable data.
        * @param[in] aMaxNumOfMatches The max number of items that will be
        *   searched for.
        */
        static CPosLmLocalCompositeSearch* NewL(
            CPosLmLocalDatabase& aDatabase,
            const CPosLmSearchCriteria& aCriteria,
            const TPosLmLocalSortPref& aSortPref,
            CPosLmDisplayData* aDisplayData,
            TInt aMaxNumOfMatches );

        /**
        * Destructor.
        */
        ~CPosLmLocalCompositeSearch();

    public: // From CPosLmLocalSearchHandler.

        /**
        * @return ETrue if all criteria are valid for landmark search, otherwise
        *   EFalse.
        */
        TBool IsValidForLandmarkSearch() const;

        /**
        * @param[out] aProgress Will be set to the progress of the search when
        *   the step has finished.
        * @return @p KPosLmOperationNotComplete if the step has completed but
        *   more steps are needed before the search has finished,
        *   @p KErrNone if the search has finished successfully.
        */
        TInt NextStepL( TReal32& aProgress );

        void CancelSearch();

        /**
        * @param[out] aMatches Array containing the landmark item IDs found
        *   in the search. The caller is responsible to close the array.
        */
        void GetMatchesL( RArray<TPosLmItemId>& aMatches ) const;

        /**
        * @return Array containing the landmark item IDs found
        *   in the search. Do NOT call Close() or Reset().
        */
        const RArray<TPosLmItemId>& Matches() const;

    private:

        void ConstructIdListL( const CPosLmCompositeCriteria& aComposite );

        TBool ConstructAllButNearestL(
            CPosLmLocalDatabase& aDatabase,
            const CPosLmCompositeCriteria& aComposite,
            const TPosLmLocalSortPref& aSortPref,
            CPosLmDisplayData* aDisplayData,
            TInt aMaxNumOfMatches,
            TInt aNoOfCriteriaNotIdList );

        void ConstructNearestL(
            CPosLmLocalDatabase& aDatabase,
            const CPosLmCompositeCriteria& aComposite,
            const TPosLmLocalSortPref& aSortPref,
            CPosLmDisplayData* aDisplayData,
            TInt aMaxNumOfMatches,
            TInt aNoOfCriteriaNotIdList );

        TBool CreateAndAppendHandlerL(
            CPosLmLocalDatabase& aDatabase,
            const CPosLmSearchCriteria& aCriteria,
            const TPosLmLocalSortPref& aSortPref,
            CPosLmDisplayData* aDisplayData,
            TInt aMaxNumOfMatches,
            TInt aNoOfCriteriaNotIdList );

		TInt CreateAllTextCriteriaL(
			CPosLmLocalDatabase& aDatabase,
            const CPosLmCompositeCriteria& aCriteria,
            const TPosLmLocalSortPref& aSortPref,
            CPosLmDisplayData* aDisplayData,
            TInt aMaxNumOfMatches,
            TInt aNoOfCriteriaNotIdList );
        void PrepareSearchL();
        void PrepareNextHandlerL( const RArray<TPosLmItemId>& aPreviousMatches );
        void SearchL( TReal32& aProgress );

        TReal32 CalculateCompositeProgressL( TReal32 aComponentProgress );
        void AddMatchesAsCurrentMatchesL( const RArray<TPosLmItemId>& aMatches );

        /**
        * C++ default constructor.
        * @param[in] aDbAccess The access handle to the database to search in.
        * @param[in] aCriteria The criteria for the searching.
        * @param[in] aSortPref A sort preference object.
        * @param[in] aMaxNumOfMatches The max number of items that will be
        *   searched for.
        */
        CPosLmLocalCompositeSearch(
            CPosLmLocalDatabase& aDatabase,
            const CPosLmSearchCriteria& aCriteria,
            const TPosLmLocalSortPref& aSortPref,
            TInt aMaxNumOfMatches );

        /**
        * By default Symbian 2nd phase constructor is private.
        * @param[in] aDbAccess The access handle to the database to search in.
        * @param[in] aCriteria The criteria for the searching.
        * @param[in] aSortPref A sort preference object.
        * @param[in] aDisplayData The displayable data.
        * @param[in] aMaxNumOfMatches The max number of items that will be
        *   searched for.
        */
        void ConstructL(
            CPosLmLocalDatabase& aDatabase,
            const CPosLmSearchCriteria& aCriteria,
            const TPosLmLocalSortPref& aSortPref,
            CPosLmDisplayData* aDisplayData,
            TInt aMaxNumOfMatches );

        // By default, prohibit copy constructor
        CPosLmLocalCompositeSearch( const CPosLmLocalCompositeSearch& );
        // Prohibit assigment operator
        CPosLmLocalCompositeSearch& operator= ( const CPosLmLocalCompositeSearch& );

    private:

        enum TPosCompositeSearchStatus
            {
            EPosSearchNotStarted = 0,
            EPosSearch,
            EPosSearchCompleted
            };

        TPosCompositeSearchStatus iSearchStatus;

        RArray<TPosLmItemId> iIdList;
        TBool iHasIdList;

        RPointerArray<CPosLmLocalSearchHandler> iComponents;

        TInt iComponentIndex;

        TInt iArrayIndex;
    };

#endif      // CPOSLMLOCALCOMPOSITESEARCH_H

