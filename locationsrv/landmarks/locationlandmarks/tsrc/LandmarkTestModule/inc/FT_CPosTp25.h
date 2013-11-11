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


#ifndef CPOSTp25_H
#define CPOSTp25_H

//  INCLUDES
#include "FT_CLandmarkTestProcedureBase.h"
#include <EPos_CPosLMCategoryManager.h>
// CLASS DECLARATION

/**
*  Test procedure 23
*/
class CPosTp25 : public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
        
        CPosTp25(TInt tpno, CStifLogger* aLog) : CLandmarkTestProcedureBase(tpno, aLog) {};
        /**
        * Destructor.
        */
        ~CPosTp25() {};

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
        
        void TestSearchPatternLengthL();

        /**
        * Include the global categories when searching
        */
        void SearchGlobalL(const RPointerArray<CCategorySearchResult>& aSearchResults, const TBool& aSync=EFalse);

        /**
        * Verify error report "ESLI-62DBXR Landmarks - Sort order is not correctly implemented"
        */
        void VerifySortOrderL();

    };

#endif      // CPOSTp25_H
            
// End of File
