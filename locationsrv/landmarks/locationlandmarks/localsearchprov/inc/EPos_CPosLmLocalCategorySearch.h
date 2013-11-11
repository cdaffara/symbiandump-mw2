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
* Description:  PRovides search handling related to category
*
*/


#ifndef CPOSLMLOCALCATEGORYSEARCH_H
#define CPOSLMLOCALCATEGORYSEARCH_H

#include <e32base.h>
#include "EPos_CPosLmLocalSearchHandler.h"

class CPosLmCategoryCriteria;

/**
*  Class for handling category id, category name,
*  and global category search.
*/
class CPosLmLocalCategorySearch : public CPosLmLocalSearchHandler
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
        static CPosLmLocalCategorySearch* NewL(
            CPosLmLocalDatabase& aDatabase,
            const CPosLmSearchCriteria& aCriteria,
            const TPosLmLocalSortPref& aSortPref,
            CPosLmDisplayData* aDisplayData,
            TInt aMaxNumOfMatches );

        /**
        * Destructor.
        */
        ~CPosLmLocalCategorySearch();

    public: // From CPosLmLocalSearchHandler.

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

        void CancelSearch();

    protected:

    private:

        void PrepareSearchL();
        void UnsortedSearchL();
        void SortedSearchL();

        TInt PrepareQueryL();
        void PrepareSortQueryIfNeededL();

        TPosLmItemId FindCategoryIdL();
        HBufC* CreateCategoryItemIdQueryLC( const TPosLmItemId aItemId );

        void PrepareCategorizedSearchL( TPosLmItemId aCategoryId );
        void PrepareUncategorizedSearchL();

        /**
        * @param[in] aDbAccess The access handle to the database to search in.
        * @param[in] aCriteria The search criteria.
        * @param[in] aSortPref A sort preference object.
        * @param[in] aMaxNumOfMatches The max number of items that will be
        *   searched for.
        */
        CPosLmLocalCategorySearch(
            CPosLmLocalDatabase& aDatabase,
            const CPosLmSearchCriteria& aCriteria,
            const TPosLmLocalSortPref& aSortPref,
            TInt aMaxNumOfMatches );

        /**
        * @param[in] aCriteria The search criteria.
        * @param[in] aDisplayData The displayable data.
        */
        void ConstructL(
            const CPosLmSearchCriteria& aCriteria,
            CPosLmDisplayData* aDisplayData );

        // By default, prohibit copy constructor
        CPosLmLocalCategorySearch(
            const CPosLmLocalCategorySearch& );
        // Prohibit assigment operator
        CPosLmLocalCategorySearch& operator= (
            const CPosLmLocalCategorySearch& );

    private:

        enum TSearchStatus
            {
            ESearchNotStarted = 0,
            
            EPrepareUnsortedSearch,
            EPrepareUnsortedCategorySearch,
            EUnsortedCategorySearch,
            
            EPrepareSortedSearch,
            EPrepareSortedCategorySearch,
            EPrepareSortedLandmarkSearch,
            ESortedCategorySearch,
            ESortedLandmarkSearch,
            
            ESearchCompleted
            };

        TSearchStatus iSearchStatus;

        enum TSearchType
            {
            EUnknown = 0,
            ECategoryItemIdSearch,
            ECategoryNameSearch,
            EGlobalCategorySearch,
            EUncategorizedSearch,
            };

        TSearchType iSearchType;

        TBool iSortSearch;

        CPosLmCategoryCriteria* iCriteria;

        RArray<TUint> iLmids;
    };

#endif      // CPOSLMLOCALCATEGORYSEARCH_H

// End of File
