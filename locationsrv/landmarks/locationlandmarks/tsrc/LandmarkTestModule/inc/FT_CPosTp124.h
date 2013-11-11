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


#ifndef CPOSTp124_H
#define CPOSTp124_H

//  INCLUDES
#include "FT_CLandmarkTestProcedureBase.h"
#include <LbsPosition.h>
// CLASS DECLARATION
class CPosLandmarkDatabase;
class CPosLmCompositeCriteria;
class CPosLmMultiDbSearch;
class CCompositeSearchResult;
//class TCoordinate;

/**
*  Test procedure 124
*/
class CPosTp124 :public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
       
        CPosTp124(TInt tpno, CStifLogger* aLog) : CLandmarkTestProcedureBase(tpno, aLog) {iErrorsFound = KErrNone;};
        /**
        * Destructor.
        */
        ~CPosTp124() {};

    public: // Functions from base classes

        

        /**
        *  Always called even if test leaves, 
        * after test completion
        */
        void CloseTest();


        /**
        * From CLandmarkTestProcedureBase
        * Runs the test in the module
        */
        void StartL();
        
       
        void InitTestL();
        
    private:
    
       	enum TTp124SortOrder
            {
            ESortNone,
            ESortAscending,
            ESortDescending,
            ESortDistance
            };

        /*
        * Use the file lmDbCompositeSearchResult.txt and parse
        * all test combinations, perform all tests and check for correct result
        */ 
        void StartLandmarksCompositeTestL(TExecutionMode aExecutionMode, TTp124SortOrder aSortOrder);

        /**
        * Prints debug information about the composite search object
        */
        void PrintCompositeInfo(CPosLmCompositeCriteria* aComposite);

        /**
        * Test that it is possible to search for categories using the
        * CPosLmCatNameCriteria class in a StartCategorySearchL search
        */
        void TestCategoryCompositeSearchL();

        /**
        * Perform a test with a composite object used in a composite landmark search 
        */
        void TestCompositeInCompositeL();

        /**
        * Perform a test with two different text arguments combined in a composite landmark search
        */
        void TestSeveralTextArgumentsL();

        /**
        * Test to add and remove search arguments to the CPosLmCompositeCriteria object
        */
        void TestAddAndRemoveL();

        /**
        * Test that no result is returned if setting the  aSearchOnlyPreviousMatches to ETrue
        * and there is no previous result
        */
        void TestWithNoPreviousResultL();

        /**
        * Test cancel of a composite search
        */
        void CancelTestL(const TBool& aCancelInCallback);
        
        void LandmarksSortL(RArray<TPosLmItemId>& aArray, TTp124SortOrder aSortOrder);
        
        void SearchL(
            CPosLmCompositeCriteria* aCompositeSearchCriteria,
            TExecutionMode aExecutionMode,
            TBool aRefinedSearch = EFalse,
            TTp124SortOrder aSortOrder = ESortNone);

		/**
		* Test with different sort order scenarios, test sortpref for landmark and category
		* also test with nearest
		*/
    	void TestWithSortOrderLandmarksL(TExecutionMode aExecutionMode,
    										 TTp124SortOrder aSortOrder);
        
    	void TestWithSortOrderNearestL(TExecutionMode aExecutionMode, TTp124SortOrder aSortOrder);
    	
    	void PrepareDatabasesL();
    	
    	void SetupLimits();
    	
    	void PrintLm(CPosLandmarkDatabase* db);
    	
    	void ValidateIdL(TPosLmItemId aId, TInt aDbIndex);
    	
    	void ValidateSearchResultsL(
            RIdArray& aExpectedIds,
            TTp124SortOrder aSortOrder,
            TInt aNrOfExpectedErrors = 0);
            
        TInt NrOfSearchErrors(CPosLmMultiDbSearch* aMultiDbSearcher);
        
        TBool CheckCompositeForIdListL(CPosLmCompositeCriteria* aComposite);
        
        void TestWithIdListAndCompositeL();
        
        void TestOnTheFlySearchL();
        
        void TestLessDbsToSearchL();
        
        void CompositeStressTestL(TInt aStartNr, TInt aTotalNr);
    	
    	private:
            
	    	TUint iIdDb1Min;
	        TUint iIdDb1Max;
	        TUint iIdDb2Min;
	        TUint iIdDb2Max;
	        TUint iIdDb3Min;
	        TUint iIdDb3Max;
	        TUint iIdDb4Min;
	        TUint iIdDb4Max;
	        TUint iIdDb5Min;
	        TUint iIdDb5Max;
	        
	        TInt 		iErrorsFound;
	        
	        CDesCArray*             iDatabases;
	        CPosLmMultiDbSearch*    iMultiDbSearch;
	        TInt                    iNrOfDatabases;
	        /**
	         * contains inital db count afte removal of all db
	         */
	        TInt iDbCount;
	        CPosLmDisplayData* 		iDisplayData;
	        TCoordinate 			iCoord;

    };

#endif      // CPOSTp124_H
            
// End of File
