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
*/


#ifndef CPOSLMLOCALAREASEARCH_H
#define CPOSLMLOCALAREASEARCH_H

#include "EPos_CPosLmLocalSearchHandler.h"

class CPosLmAreaCriteria;

/**
*  Class for handling area searching for landmarks.
*/
class CPosLmLocalAreaSearch : public CPosLmLocalSearchHandler
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
        static CPosLmLocalAreaSearch* NewL(
            CPosLmLocalDatabase& aDatabase,
            const CPosLmSearchCriteria& aCriteria,
            const TPosLmLocalSortPref& aSortPref,
            CPosLmDisplayData* aDisplayData,
            TInt aMaxNumOfMatches );

        /**
        * Destructor.
        */
        ~CPosLmLocalAreaSearch();

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

        /** Returns ETrue if current row in landmark table 
         *  matches the criteria, EFalse - otherwise */
        TBool CurrentRowMatch( RDbRowSet& aView );
        
    private:

        void PrepareSearchL();
        void PrepareQueryL( TBool aFullData );
        void UnsortedSearchL();
        void SortedSearchL();

        /**
        * @param[in] aDbAccess The access handle to the database to search in.
        * @param[in] aCriteria The search criteria.
        * @param[in] aSortPref A sort preference object.
        * @param[in] aMaxNumOfMatches The max number of items that will be
        *   searched for.
        */
        CPosLmLocalAreaSearch(
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
        CPosLmLocalAreaSearch( const CPosLmLocalAreaSearch& );
        // Prohibit assigment operator
        CPosLmLocalAreaSearch& operator= ( const CPosLmLocalAreaSearch& );

    private:

        enum TSearchStatus
            {
            ESearchNotStarted = 0,
            
            EPrepareUnsortedSearch,
            EUnsortedSearch,
            
            EPrepareSortedSearch,
            ESortedSearch,
            
            ESearchCompleted
            };

        TSearchStatus iSearchStatus;

        TReal64 iSouth;
        TReal64 iNorth;
        TReal64 iWest;
        TReal64 iEast;
    };

#endif      // CPOSLMLOCALAREASEARCH_H

