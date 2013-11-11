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


#ifndef CPOSTp125_H
#define CPOSTp125_H

//  INCLUDES
#include "FT_CLandmarkTestProcedureBase.h"
#include <EPos_CPosLMCategoryManager.h>
#include <EPos_CPosLmMultiDbSearch.h>
//#include <EPos_CPosLmMultiDbSearchOperation.h>

// CLASS DECLARATION

/**
*  Test procedure 125
*/
class CPosTp125 : public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
        
        CPosTp125(TInt tpno, CStifLogger* aLog) : CLandmarkTestProcedureBase(tpno, aLog) {};
        /**
        * Destructor.
        */
        ~CPosTp125() {delete iLandmarksMultiDbSearch;
                      delete iDbArray;};

    public: // Functions from base classes

        void CloseTest();

        /**
        * From CLandmarkTestProcedureBase
        * Runs the test in the module
        */
        void StartL();

    private:

        void SearchL(
            const RPointerArray<CCategorySearchResult>& aSearchResults, 
            TExecutionMode aExecutionMode);

        void SearchWithMaximumL(
            const RPointerArray<CCategorySearchResult>& aSearchResults, 
            TExecutionMode aExecutionMode);

        void SearchWithSortOrderL(
            const RPointerArray<CCategorySearchResult>& aSearchResults, 
            TExecutionMode aExecutionMode);

        void SearchAfterAddedCategoryL();

        void SearchAndCancelL(const TBool& aInCallback=EFalse);

        /**
        * Include the global categories when searching
        */
        void SearchGlobalL(const RPointerArray<CCategorySearchResult>& aSearchResults, const TBool& aSync=EFalse);
        
        /**
        * All categories are included in all databases. Remove so there are no duplicates.
        */
        void RemoveExtraCategoriesL();
        
        void RemoveCategoryL(CPosLmCategoryManager* aCatMan,
                                TPosLmItemId aCatId);
        
                                       
        void AssertThatIdIsInCorrectDbL(TInt aDbIndex, TInt aId);
        
        void SetupDbArrayL();
        
        void SortResultsL(RArray<TPosLmItemId>& aArray,
                          CDesCArrayFlat*& aResultArray);
                 
        void InvertArrayL(CDesCArrayFlat*& aArray);
        
        void TestDuplicateGlobalCategoriesL(const RPointerArray<CCategorySearchResult>& aSearchResults, const TBool& aSync=EFalse);
        
        // data members
    private:
        
        CDesCArrayFlat*         iDbArray;
        CPosLmMultiDbSearch*    iLandmarksMultiDbSearch;
        
    };

#endif      // CPOSTp125_H
            
// End of File
