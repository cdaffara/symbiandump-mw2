/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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


#ifndef CPOSTP19_H
#define CPOSTP19_H

//  INCLUDES
#include "FT_CLandmarkTestProcedureBase.h"

// CLASS DECLARATION

/**
*  Test procedure 19
*/
class CPosTp19 : public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
       
        CPosTp19(TInt tpno, CStifLogger* aLog) : CLandmarkTestProcedureBase(tpno, aLog) {};
        /**
        * Destructor.
        */
        ~CPosTp19() {};

    public: // Functions from base classes
        /**
        * From CLandmarkTestProcedureBase
        * Runs the test in the module
        */
        void StartL();

        void CloseTest();

    private:

        void TestGetByNameL();

        void ReadLandmarksDbFileL();

        void ParseCategoryL(const TDesC& aDes);
     
        void SetupGlobalCategoriesL();

        void TestGetGlobalL();
        
        void AddCategoriesL( TInt aNumofCats );
        
        void VerifyAddedCategoriesL();
    
    private: // Data

        enum TValueIndex
            {
            ECategoryId           =0,	
            ECategoryName         =1	
            };

        CPosLmCategoryManager*          iCategoryManager;        
        RArray<TPosLmItemId>            iCategoryIds;        
        RArray<TUint> iGlobalCategoryIds;

        RPointerArray<HBufC>            iCategoryNames;        
        RPointerArray<HBufC>            iGlobalCategoryNames;       
        RPointerArray<CPosLandmarkCategory> iCategories;
    };

#endif      // CPOSTP19_H
            
// End of File
