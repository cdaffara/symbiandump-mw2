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


#ifndef CPOSTp26_H
#define CPOSTp26_H

//  INCLUDES
#include "FT_CLandmarkTestProcedureBase.h"

// CLASS DECLARATION

/**
*  Test procedure 26
*/
class CPosTp26 : public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
        
        CPosTp26(TInt tpno, CStifLogger* aLog) : CLandmarkTestProcedureBase(tpno, aLog) {iGlobalErr = KErrGeneral;iNrSearchRounds=0;};
        /**
        * Destructor.
        */
        ~CPosTp26() {};

    public: // Functions from base classes

        
        void CloseTest();

        /**
        * From CPosTestProcedureBase
        * Runs the test in the module
        */
        void StartL();

    private:    // New functions

        /**
        * Test to do a iterate landmark search using the CPosLmTextCriteria criteria
        */
        void DoLandmarkSearchL(const RPointerArray<CSearchResult>& aSearchResults);

        /**
        * Test to do a iterate category landmark search using the CPosLmCatNameCriteria criteria
        */
        void DoLandmarkCategorySearchL(const RPointerArray<CCategorySearchResult>& aSearchResults);

    private:    // Data

        
        TInt    iGlobalErr;
        TInt    iNrSearchRounds;
    };

#endif      // CPOSTp26_H
            
// End of File
