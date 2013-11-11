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


#ifndef CPOSTP10_H
#define CPOSTP10_H

//  INCLUDES
#include <EPos_CPosLMCategoryManager.h>
#include "FT_CLandmarkTestProcedureBase.h"



/**
*  Test procedure 10
*/
class CPosTp10 :public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
        
        CPosTp10(TInt tpno, CStifLogger* aLog) : CLandmarkTestProcedureBase(tpno, aLog) {};
        
        /**
        * Destructor.
        */
        ~CPosTp10() {};

    public: // Functions from base classes

       
        /**
        * From CLandmarkTestProcedureBase
        * Runs the test in the module
        */
        void StartL();
        /**
        * From CLandmarkTestProcedureBase
        * Cleans up after a test.
        * Always called, even if the test leaves. 
        */
        void CloseTest();

    private:
        void AddCategoriesL();

        void ReadCategoriesL();

        void UpdateCategoriesL();

        void AddLandmarksL();

        void AddCategoriesToLandmarksL();

        void AddCategoriesToLandmarksAsyncL();

        void AddCategoriesToLandmarksUsingWaitForReqL();

        void AddCategoriesToLandmarksAsyncAndCancelL(const TBool& aInCallback=EFalse);

        void CheckLandmarksL();

        void CheckRemovedL(const TInt startIndex, const TInt stopIndex);

        void RemoveCategoryL();

        void RemoveCategoryAsyncL();

        void RemoveCategoryUsingWaitForReqL();

        void RemoveCategoryAsyncAndCancelL(const TBool& aInCallback=EFalse);

        void RemoveSetOfCategoriesL();
        
        void RemoveSetOfCategoriesAsyncL();

        void RemoveSetOfCategoriesUsingWaitForReqL();

        void RemoveSetOfCategoriesAsyncAndCancelL(const TBool& aInCallback=EFalse);

        void RemoveCategoryFromSetOfLandmarksL();
        
        void RemoveCategoryFromSetOfLandmarksAsyncL();

        void RemoveCategoryFromSetOfLandmarksUsingWaitForReqL();

        void RemoveCategoryFromSetOfLandmarksAsyncAndCancelL(const TBool& aInCallback=EFalse);

        void RemovedFromLandmarksInternalL(TPosLmItemId aCategoryId);
    
        void ReadCategoryL(TPosLmItemId aCategoryId);

        void AddCategoriesToLandmarksAsyncErrCheckL();

        /**
        * Test LMREQ121
        */
        void TestMissingCategories1L();

        /**
        * Test LMREQ121
        */
        void TestMissingCategories2L();
        
        void CompareTwoCategoriesL(TInt aIndex);
        
        CPosLandmarkDatabase* BackupTp10DbFileL();
        
        CPosLandmarkDatabase* RestoreTp10DbFileL();


    private: // data

        CPosLmCategoryManager*              iCategoryManager;
        RPointerArray<CPosLandmarkCategory> iCategories;
        RPointerArray<CPosLandmark>         iLandmarks;
    
    };

#endif      // CPOSTP1_H
            
// End of File
