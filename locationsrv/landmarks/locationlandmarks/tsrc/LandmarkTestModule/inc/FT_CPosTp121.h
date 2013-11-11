/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: 
*   ?description_line
*
*/


#ifndef CPOSTP121_H
#define CPOSTP121_H

//  INCLUDES
#include "FT_CLandmarkTestProcedureBase.h"

// FORWARD DECLARATIONS
class CPosLmMultiDbSearch;
class CPosLmCategoryCriteria;

// CLASS DECLARATION

/**
*  Test procedure 121
*/
class CPosTp121 : public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
        
        CPosTp121(TInt tpno, CStifLogger* aLog) : CLandmarkTestProcedureBase(tpno, aLog) {};
        /**
        * Destructor.
        */
        ~CPosTp121() {};

    public: // Functions from base classes

        void CloseTest();

       
        void InitTestL();

        /**
        * From CLandmarkTestProcedureBase
        * Runs the test in the module
        */
        void StartL();

    private:

        enum TTp121SortOrder
            {
            ESortNone,
            ESortAscending,
            ESortDescending
            };
            
        void TestSearchL(
            const RPointerArray<CCategoryLandmarkSearchResult>& aSearchResults, 
            TExecutionMode aExecutionMode = ESynchronous,
            TTp121SortOrder aSortOrder = ESortNone);

        void SearchNonExistingL(TExecutionMode aExecutionMode);

        void SearchForLandmarksInGlobalCategoriesL(TExecutionMode aExecutionMode);

        void AddLandmarksToGlobalCategoriesL();

        void TestMaximumNrOfMatchesL(
            const RPointerArray<CCategoryLandmarkSearchResult>& aSearchResults, 
            TExecutionMode aExecutionMode);

        void TestLeaveCodesL();

        void SearchAndCancelL();
        
        void ValidateIdL(TPosLmItemId aId, TInt aDbIndex);
        
        void ValidateTotalNrOfMatchesL(
            TUint aRetrievedNrOfMatches,
            TUint aExpectedNrOfMatches, 
            const TDesC& aCategoryName);
            
        void ValidateSearchResultsL(
            CCategoryLandmarkSearchResult* aExpectedSearchResult,
            CPosLmMultiDbSearch* aMultiDbSearcher,
            TTp121SortOrder aSortOrder = ESortNone);

        void ValidateSearchResultsL(
            const RIdArray& aExpectedIds,
            CPosLmMultiDbSearch* aMultiDbSearcher,
            TInt aNrOfExpectedErrors = 0);

        void SearchL(
            CPosLmCategoryCriteria* aCatSearchCriteria,
            TExecutionMode aExecutionMode,
            TBool aRefinedSearch = EFalse,
            TTp121SortOrder aSortOrder = ESortNone);
            
        void TestResetSearchAttributesL();
        
        void TestCaseInsensitiveL();
        
        void TestWildCardsL();
        
        void TestRefinedSearchL(TExecutionMode aExecutionMode);

        void TestDatabaseListL(const RIdArray& aExpectedMatches);
        
        void TestOnTheFlySearchL(const RIdArray& aExpectedMatches);
        
        TInt NrOfSearchErrors(CPosLmMultiDbSearch* aMultiDbSearcher);
        
    private:

        TUint                   iRestaurantId1;
        TUint                   iRestaurantId2;
        TUint                   iGasStationId1;
        TUint                   iGasStationId2;
        TUint                   iGarageId1;
        CDesCArray*             iDatabases;
        CPosLmMultiDbSearch*    iMultiDbSearch;
        TInt                    iNrOfDatabases;

    };

#endif      // CPOSTP121_H
            
// End of File
