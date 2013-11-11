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


#ifndef CPOSTp38_H
#define CPOSTp38_H

//  INCLUDES
#include "FT_CLandmarkTestProcedureBase.h"

// CLASS DECLARATION
class CPosLandmarkDatabase;
class CPosLmCompositeCriteria;
/**
*  Test procedure 1
*/
class CPosTp38 :public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
        
        CPosTp38(TInt tpno, CStifLogger* aLog) : CLandmarkTestProcedureBase(tpno, aLog) {};
        /**
        * Destructor.
        */
        ~CPosTp38() {};

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

    private:

        /*
        * Use the file lmDbCompositeSearchResult.txt and parse
        * all test combinations, perform all tests and check for correct result
        */ 
        void StartLandmarksCompositeTestL();

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
        * Perform a test with two CPosLmCategoryCriteria objects
        */
        void TestTwoLmCategoryCriteriaL();

        /**
        * Perform a test with two different text arguments combined in a composite landmark search
        * with AND composition
        */
        void TestSeveralTextArgumentsL();
        /**
        * Perform a test with two different text arguments combined in a composite landmark search
        * with OR composition
        */
        void TestSeveralTextArgumentsWithORL();
        /**
        * Test to check sort order when using OR composite criteria
        */
        
        void TestSortOrderWithORL();

        /**
        * Test to add and remove search arguments to the CPosLmCompositeCriteria object
        */
        void TestAddAndRemoveL();

        /**
        * Test that no result is returned if setting the  aSearchOnlyPreviousMatches to ETrue
        * and there is no previous result
        */
        void TestWithNoPreviousResultL(TInt aTestNr);

        /**
        * Test cancel of a composite search
        */
        void SearchAndCancelL(const TBool& aInCallback);

		/**
		* Test with different sort order scenarios, test sortpref for landmark and category
		* also test with nearest
		*/
    	void TestWithSortOrderLandmarksL();
    
    	void TestWithSortOrderCategoriesL();
    
    	void TestWithSortOrderNearestL();

    };

#endif      // CPOSTp38_H
            
// End of File
