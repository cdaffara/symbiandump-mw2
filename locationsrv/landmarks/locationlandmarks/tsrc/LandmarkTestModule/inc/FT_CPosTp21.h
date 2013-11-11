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


#ifndef CPOSTp21_H
#define CPOSTp21_H

//  INCLUDES
#include "FT_CLandmarkTestProcedureBase.h"

// CLASS DECLARATION

/**
*  Test procedure 21
*/
class CPosTp21 : public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
        //CPosTp21() : CLandmarkTestProcedureBase(21) {};
        CPosTp21(TInt tpno, CStifLogger* aLog) : CLandmarkTestProcedureBase(tpno, aLog) {};
        /**
        * Destructor.
        */
        ~CPosTp21() {};

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
        * From CUtfwTestBase
        */
        void CloseTest();

        /**
        * From CLandmarkTestProcedureBase
        * Runs the test in the module
        */
        void StartL();

    private:

        void SearchL(
            const RPointerArray<CCategoryLandmarkSearchResult>& aSearchResults, 
            TBool aNameOrId=EFalse, 
            TExecutionMode aExecutionMode = ESynchronous);

        void SearchNonExistingL(TExecutionMode aExecutionMode);

        void SearchForLandmarksInGlobalCategoriesL(TExecutionMode aExecutionMode);

        void AddLandmarksToGlobalCategoriesL();       

        void SearchWithSortOrderL();

        void SearchAndCancelL(const TBool& aInCallback=EFalse);
        
        void TestSearchPatternLengthL();
        
        // Added for BC testing 
                
        void AddCategoriesL();

        void AddLandmarksL();
        
         // Added for testing (Should be made common metod in BaseClass ??)
        void MakeEmptyLmDatabaseL( CPosLandmarkDatabase& aLdb,
                                   CPosLmCategoryManager& aCatMgr );  

    private:      
        TUint iRestaurantId1;
        TUint iRestaurantId2;
        TUint iGasStationId1;
        TUint iGasStationId2;
        TUint iGarageId1;
        
        // Added for BC testing 
        CPosLmCategoryManager*                 iCategoryManager;
        RPointerArray<CPosLandmarkCategory>    iCategories;        
        //RArray<CPosLandmarkCategory>         iReferencedCategories;        
        RPointerArray<CPosLandmarkCategory>    iReferencedCategories;        
        RPointerArray<CPosLandmark>            iLandmarks;

    };

#endif      // CPOSTp21_H
            
// End of File
