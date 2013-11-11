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


#ifndef CPOSTP23_H
#define CPOSTP23_H

//  INCLUDES
#include "FT_CLandmarkTestProcedureBase.h"

// CLASS DECLARATION

/**
*  Test procedure 23
*/
class CPosTp23 : public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
      
        CPosTp23(TInt tpno, CStifLogger* aLog) : CLandmarkTestProcedureBase(tpno, aLog) {};
        /**
        * Destructor.
        */
        ~CPosTp23() {};

    public: // Functions from base classes

       
        /**
        * From CLandmarkTestProcedureBase
        * Runs the test in the module
        */
        void StartL();

        void CloseTest();
    private:

        void SearchL(
            const RPointerArray<CSearchResult>& aSearchResults, 
            TExecutionMode aExecutionMode);

        void SearchWithSortOrderL(const RPointerArray<CSearchResult>& aSearchResults);

        /**
        * Verify error report "ESLI-62DBXR Landmarks - Sort order is not correctly implemented"
        */
        void VerifySortOrderL();

        void SearchAndDeleteL(const RPointerArray<CSearchResult>& aSearchResults);

        void SearchAndRenameL(const RPointerArray<CSearchResult>& aSearchResults);

        void SearchAndCancelL(const TBool& aInCallback=EFalse);
        
        void TestMaxLengthL();
        
        void TestInvalidPositionFieldsL();
    };

#endif      // CPOSTP23_H
            
// End of File
