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


#ifndef CPOSTP11_H
#define CPOSTP11_H

//  INCLUDES

#include <EPos_CPosLandmarkCategory.h>
#include <EPos_CPosLMCategoryManager.h>
#include "FT_CLandmarkTestProcedureBase.h"

// CLASS DECLARATION

/**
*  Test procedure 11
*/
class CPosTp11 :public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
       // CPosTp11() : CLandmarkTestProcedureBase(11) {};
        CPosTp11(TInt tpno, CStifLogger* aLog) : CLandmarkTestProcedureBase(tpno, aLog) {};
        /**
        * Destructor.
        */
        ~CPosTp11() {};

    public: // Functions from base classes

        /**
        * From CUtfwTestBase
        * Retrieves the test module name
        * @param aName The name of the test module.
        */
       /* void GetName(
            TDes&   aName
            ) const;
			*/
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

        void AddLandmarksL();

        void IterateCategoriesL();

        void IterateSetOfCategoriesL();

        void CheckIteratorItemsL(CPosLmItemIterator& iter);
        
        CPosLandmarkCategory* InternalGet(TPosLmItemId id);

        void IterateSortedCategoriesL();

        void IterateReferencedCategoriesL();
                                     
        void IterateReferencedSortedCategoriesL();       
        
        // Added for BC testing 
        void MakeEmptyLmDatabaseL( CPosLandmarkDatabase& aLdb,
                                   CPosLmCategoryManager& aCatMgr ); 
        

    private: // data

        CPosLmCategoryManager*                 iCategoryManager;
        RPointerArray<CPosLandmarkCategory>    iCategories;
        RArray<CPosLandmarkCategory>           iReferencedCategories;
        RPointerArray<CPosLandmark>            iLandmarks;       
    };

#endif      // CPOSTP11_H
            
// End of File
