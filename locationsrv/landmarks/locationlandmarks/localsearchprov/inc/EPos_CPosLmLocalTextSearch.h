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
* Description: Class for handling text search.
*
*/


#ifndef CPOSLMLOCALTEXTSEARCH_H
#define CPOSLMLOCALTEXTSEARCH_H

#include "EPos_CPosLmLocalSearchHandler.h"

class CPosLmLocalDatabase;
class CPosLmTextCriteria;

/**
*  Class for handling text search.
*/
class CPosLmLocalTextSearch : public CPosLmLocalSearchHandler
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        *
        * @param[in] aDbAccess The access handle to the database to search.
        * @param[in] aCriteria The criteria for the searching.
        * @param[in] aSortPref A sort preference object.
        * @param[in] aDisplayData The displayable data.
        * @param[in] aMaxNumOfMatches The max number of items that will be
        *   searched for.
        */
        static CPosLmLocalTextSearch* NewL(
            CPosLmLocalDatabase& aDatabase,
            const CPosLmSearchCriteria& aCriteria,
            const TPosLmLocalSortPref& aSortPref,
            CPosLmDisplayData* aDisplayData,
            TInt aMaxNumOfMatches );

	   /**
        * Two-phased constructor.
        *
        * @param[in] aDbAccess The access handle to the database to search.
        * @param[in] aCriteria The array of the criterias, for the searching.
        * @param[in] aSortPref A sort preference object.
        * @param[in] aDisplayData The displayable data.
        * @param[in] aMaxNumOfMatches The max number of items that will be
        *   searched for.
        */
	   static CPosLmLocalTextSearch* NewL(
            CPosLmLocalDatabase& aDatabase,
            const RPointerArray<CPosLmSearchCriteria>& aCriteria,
            const TPosLmLocalSortPref& aSortPref,
            CPosLmDisplayData* aDisplayData,
            TInt aMaxNumOfMatches );
            
        /**
        * Destructor.
        */
        ~CPosLmLocalTextSearch();

    public: // New functions

        /**
        * Validates that the criteria is correct according to the search API.
        * Leaves if the criteria is missing a search text.
        *
        * @param[in] aCriteria The criteria to validate.
        */
        static void ValidateCriteriaL(
            const CPosLmTextCriteria& aCriteria
        );
        
        /**
        *set the composition type. it is used when more then one search pattern is provided while constructing
        @param[in] aType The composition type
        */
        
        void SetCompositionType(TInt atype);
    public: // From CPosLmLocalSearchHandler.

        /** @return ETrue. */
        TBool IsValidForLandmarkSearch() const;

		
        /**
        * @param[in][out] aProgress Will be set to the progress of the search when
        * the step has finished.
        * @return @p KPosLmOperationNotComplete if the step has completed but
        *   more steps are needed before the search has finished,
        *   @p KErrNone if the search has finished successfully.
        */
        TInt NextStepL( TReal32& aProgress );

    protected:

    private:

    	void PrepareSearchL();
    	void PrepareFieldSearchL( RDbView& aView );

        HBufC* CreatePosFieldQueryLC();
        HBufC* CreateAttributeQueryLC();

        void UnsortedSearchL();
        void SortedLimitedSearchL();
        void SortedUnlimitedSearchL();
        
        void InsertMatchIndexL( TUint aIndex, RDbRowSet* aDataRow = NULL );
        void AddMatchIndexL( TUint aIndex, RDbRowSet* aDataRow = NULL );
        
        /**
        * C++ default constructor.
        * @param[in] aDbAccess The access handle to the database to search.
        * @param[in] aCriteria The criteria for the searching.
        * @param[in] aSortPref A sort preference object.
        * @param[in] aMaxNumOfMatches The max number of items that will be
        *   searched for.
        */
        CPosLmLocalTextSearch(
            CPosLmLocalDatabase& aDatabase,
            const CPosLmSearchCriteria& aCriteria,
            const TPosLmLocalSortPref& aSortPref,
            TInt aMaxNumOfMatches );

        /**
        * By default Symbian 2nd phase constructor is private.
        * @param[in] aCriteria The criteria for the searching.
        * @param[in] aDisplayData The displayable data.
        */
        void ConstructL(
            const CPosLmSearchCriteria& aCriteria,
            CPosLmDisplayData* aDisplayData );
            
         /**
        * By default Symbian 2nd phase constructor is private.
        * @param[in] aCriteria The array of the criterias, for the searching.
        * @param[in] aDisplayData The displayable data.
        */
        void ConstructL(
            const RPointerArray<CPosLmSearchCriteria>& aCriteria,
            CPosLmDisplayData* aDisplayData );

        // By default, prohibit copy constructor
        CPosLmLocalTextSearch( const CPosLmLocalTextSearch& );
        // Prohibit assigment operator
        CPosLmLocalTextSearch& operator= ( const CPosLmLocalTextSearch& );
        
        TBool MatchText(TPtrC text); 
        

    private:

        enum TPosTextSearchStatus
            {
            ESearchNotStarted = 0,
            
            EPrepareUnsortedSearch,
            EUnsortedAttributeSearch,
            EPrepareUnsortedFieldSearch,
            EUnsortedFieldSearch,
            
            EPrepareSortedLimitedSearch,
            EPrepareSortedLimitedAttributeSearch,
            ESortedLimitedAttributeSearch,
            ESortedLimitedFieldSearch,
            
            EPrepareUnlimitedSearch,
            EPrepareUnlimitedFieldSearch,
            EUnlimitedAttributeSearch,
            EUnlimitedFieldSearch,

            ESearchCompleted
            };

        TPosTextSearchStatus iSearchStatus;

        TBool iPositionFieldSearch;

        TBool iAttributeSearch;

        TBool iSortSearch;

        CPosLandmark::TAttributes   iAttributes;
        RArray<TUint>               iFields;
        RPointerArray<HBufC>        iTextPattern;
        
        
        RArray<TUint>               iMatchIndexes;
        RArray<TUint>				iFieldMatches;
        
        TInt						iCompositionType;
    };

#endif      // CPOSLMLOCALTEXTSEARCH_H

