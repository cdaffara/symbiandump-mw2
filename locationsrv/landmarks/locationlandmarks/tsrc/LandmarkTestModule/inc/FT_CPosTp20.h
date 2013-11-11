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


#ifndef CPOSTP20_H
#define CPOSTP20_H

//  INCLUDES
#include "FT_CLandmarkTestProcedureBase.h"

// CLASS DECLARATION

/**
*  Test procedure 20
*/
class CPosTp20 : public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
        
        CPosTp20(TInt tpno, CStifLogger* aLog) : CLandmarkTestProcedureBase(tpno, aLog) {};
        
        /**
        * Destructor.
        */
        ~CPosTp20() {};

    public: // Functions from base classes

        /**
        * From CLandmarkTestProcedureBase
        * Runs the test in the module
        */
        void StartL();

        void CloseTest();

    private:

        CPosLandmarkDatabase* ReplaceGlobalCategoriesL();

        void SetupGlobalCategoriesL();

        //setup the default categories. english version
        //how to test russian??
        
        void VerifyGlobalCategoriesL(TBool aCheck = ETrue);

        void RenameAndDeleteGlobalL();
        
        void RemoveAllCategoriesL();
        
        
        // Added for BC testing 
        void UpdateGlobalCategoriesInDb();
        
        
        

    private: // Data

         CPosLmCategoryManager*          iCategoryManager;
         
         RArray<TUint>    				 iGlobalCategoryIds;
         

         RPointerArray<HBufC>            iGlobalCategoryNames;
         RArray<TPtrC>                   iGlobalCategoryTestNames;      
         
         TPosLmItemId			iLmId;
         TPosLmItemId			iCategoryId;
    };

#endif      // CPOSTP20_H
            
// End of File
