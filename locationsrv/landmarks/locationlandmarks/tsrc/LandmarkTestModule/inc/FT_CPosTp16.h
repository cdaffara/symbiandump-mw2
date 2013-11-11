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


#ifndef CPOSTP16_H
#define CPOSTP16_H

//  INCLUDES
#include "FT_CLandmarkTestProcedureBase.h"

// CLASS DECLARATION

/**
*  Test procedure 16
*/
class CPosTp16 : public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
        CPosTp16() : CLandmarkTestProcedureBase(16) {};
        
        /**
        * Destructor.
        */
        ~CPosTp16() {};

    public: // Functions from base classes

        /**
        * From CUtfwTestBase
        * Retrieves the test module name
        * @param aName The name of the test module.
        */
        void GetName(
            TDes&   aName
            ) const;

        /**
        * From CLandmarkTestProcedureBase
        * Runs the test in the module
        */
        void StartL();

        void CloseTest();
    
    private:
        CPosLandmarkDatabase* UseCorruptDbL();
        CPosLandmarkDatabase* UseTextDbL();
        CPosLandmarkDatabase* UseContactDbL();

        // check that global categories are automatically inserter into lm db 
        // when recovering database
        void CheckGlobalCategoriesL();

        void SetupGlobalCategoriesL();

    private:
        RArray<TPosLmGlobalCategory>    iGlobalCategoryIds;
        RArray<TPtrC>                   iGlobalCategoryNames;
    };

#endif      // CPOSTP16_H
            
// End of File
