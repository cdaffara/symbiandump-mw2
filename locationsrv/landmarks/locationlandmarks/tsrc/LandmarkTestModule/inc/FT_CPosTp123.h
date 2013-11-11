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


#ifndef CPOSTP123_H
#define CPOSTP123_H

//  INCLUDES
#include "FT_CLandmarkTestProcedureBase.h"

// FORWARD DECLARATIONS
class CPosLmDatabaseManager;
class CPosLmMultiDbSearch;

// CLASS DECLARATION

/**
*  Test procedure 123
*/
class CPosTp123 :public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
       
        CPosTp123(TInt tpno, CStifLogger* aLog) : CLandmarkTestProcedureBase(tpno, aLog) {};
        /**
        * Destructor.
        */
        ~CPosTp123() {};

    public: // Functions from base classes


        void CloseTest();

        void InitTestL();

        /**
        * From CLandmarkTestProcedureBase
        * Runs the test in the module
        */
        void StartL();

    private:
    
        enum TSearchCriterion
            {
            ENoCriterion,
            ELocalCategoryId,
            EGlobalCategoryId,
            ECategoryName
            };
            
        void TestSearchL(
            TBool aExecuteSync, 
            TSearchCriterion aCriterion, 
            TInt aNrOfHits, 
            TBool aSorted);

        void TestSearchCategoriesL(TBool aExecuteSync);
        
        void RemoveCategoriesFromLandmarksL(TInt aNrOfLandmarks);

        void SearchForUncategorizedLMsL(TInt aNrOfHits, TBool aSorted = EFalse);

        void RemoveCategoryFromDbL(TPosLmItemId aCategoryId, const TDesC& aDbUri);
        
        void VerifySearchResultL(TInt aNrOfHits, TBool aSorted);
        
        void VerifySortOrderL(CPosLmItemIterator* aIter, CPosLandmarkDatabase* aDb);
        
    private:
    
        RArray<RIdArray>        iIdArrays;
        TBuf<200>               iBuf;
        CDesCArray*             iDatabases;
        TInt                    iNrOfDatabases;
        CPosLmMultiDbSearch*    iLandmarkSearch;

    };

#endif      // CPOSTP123_H
            
// End of File
