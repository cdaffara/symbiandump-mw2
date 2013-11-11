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
*/


#ifndef CPOSLMLOCALNEARESTSEARCH_H
#define CPOSLMLOCALNEARESTSEARCH_H

#include "EPos_CPosLmLocalSearchHandler.h"
#include "EPos_TPosLmNearestLandmark.h"

class CPosLmNearestCriteria;
class TPosLmLocalSortPref;

/**
*  Class for handling searching for landmarks nearest a specified position.
*/
class CPosLmLocalNearestSearch : public CPosLmLocalSearchHandler
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        *
        * @param[in] aDbAccess The access handle to the database to search in.
        * @param[in] aCriteria The search criteria.
        * @param[in] aSortPref A sort preference object.
        * @param[in] aDisplayData The displayable data.
        * @param[in] aMaxNumOfMatches The max number of items that will be
        *   searched for.
        */
        static CPosLmLocalNearestSearch* NewL(
            CPosLmLocalDatabase& aDatabase,
            const CPosLmSearchCriteria& aCriteria,
            const TPosLmLocalSortPref& aSortPref,
            CPosLmDisplayData* aDisplayData,
            TInt aMaxNumOfMatches );

        /**
        * Destructor.
        */
        ~CPosLmLocalNearestSearch();

    public: // New functions

    public: // From CPosLmLocalSearchHandler

        /**
        * @return ETrue.
        */
        TBool IsValidForLandmarkSearch() const;

        /**
        * @param[out] aProgress Will be set to the progress of the search when
        * the step has finished.
        * @return @p KPosLmOperationNotComplete if the step has completed but
        *   more steps are needed before the search has finished,
        *   @p KErrNone if the search has finished successfully.
        */
        TInt NextStepL( TReal32& aProgress );

    protected:

    private:

        void PrepareSearchL();
        void SearchL();

        void PrepareSearchQueryL();
        void PrepareSortQueryIfNeededL();

        /** Finds area restriction for this search. If coverage radius 
         * is not used in comparisons, then it is possible to define 
         * bounding area for given center and mas distance. To improve
         * search performance this filter is used to calculate distances 
         * only for those landmarks, which fall into such bounding area.
         */
        void CalculateAreaRestrictionsL(
        	TReal64& aMinLatitude,
        	TReal64& aMaxLatitude,
        	TReal64& aMinLongitude,
        	TReal64& aMaxLongitude );

        void ReadFromDbAndCalculateAndAddIfMatchL();

        void AddMatchesAsCurrentMatchesL();

        /**
        * C++ default constructor.
        * @param[in] aDbAccess The access handle to the database to search in.
        * @param[in] aCriteria The search criteria.
        * @param[in] aSortPref A sort preference object.
        * @param[in] aMaxNumOfMatches The max number of items that will be
        *   searched for.
        */
        CPosLmLocalNearestSearch(
            CPosLmLocalDatabase& aDatabase,
            const CPosLmSearchCriteria& aCriteria,
            const TPosLmLocalSortPref& aSortPref,
            TInt aMaxNumOfMatches );

        /**
        * By default Symbian 2nd phase constructor is private.
        * @param[in] aCriteria The search criteria.
        * @param[in] aDisplayData The displayable data.
        */
        void ConstructL(
            const CPosLmSearchCriteria& aCriteria,
            CPosLmDisplayData* aDisplayData );

        // By default, prohibit copy constructor
        CPosLmLocalNearestSearch( const CPosLmLocalNearestSearch& );
        // Prohibit assigment operator
        CPosLmLocalNearestSearch& operator= ( const CPosLmLocalNearestSearch& );

    private:

        enum TNearestSearchStatus
            {
            ESearchNotStarted = 0,

            EPrepareNearestSearch,
            ENearestSearch,
            EReadDisplayData,
            
            EPrepareSorting,
            ESorting,
            
            ESearchCompleted
            };

        TNearestSearchStatus iSearchStatus;

        CPosLmNearestCriteria*  iCriteria;

        TBool                   iSortSearch;

        RArray<TPosLmNearestLandmark> iArray;

        TInt iCurrentResult;

        TBool iIncludeAreaCondition;
    };

#endif      // CPOSLMLOCALNEARESTSEARCH_H

// End of File
