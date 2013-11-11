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


#ifndef CPOSTP13_H
#define CPOSTP13_H

//  INCLUDES
#include "FT_CLandmarkTestProcedureBase.h"
#include <StifParser.h>



enum TStifDialogType
    {
    EStifDialogTypeYesNo,
    EStifDialogTypeOk,
    EStifDialogTypeOkCancel,
    EStifDialogTypeRetryCancel,
    EStifDialogTypeYesNoCancel
    };
// CLASS DECLARATION

/**
*  Test procedure 13
*/
class CPosTp13 :public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
        
        CPosTp13(TInt tpno, CStifLogger* aLog) : CLandmarkTestProcedureBase(tpno, aLog) {iErrorsFound = KErrNone;};
        /**
        * Destructor.
        */
        ~CPosTp13() {};

    public: // Functions from base classes
    
         
        /**
        * From CLandmarkTestProcedureBase
        * Runs the test in the module
        */
        void StartL(CStifItemParser&);

        
        void CloseTest();

    private:
        
        void ListCategoriesL( CPosLmCategoryManager& aCatMan );
        
        void FindLandmarksL( 
            CPosLandmarkDatabase& aDb, 
            TPosLmItemId aCategoryId,  
            RArray<TPosLmItemId>& aLmIds );

        CPosLandmark* FindLandmarkLC( CPosLandmarkDatabase& aDb, const TDesC& aName );

    private:
		TInt			iErrorsFound;
    };

#endif      // CPOSTP13_H
            
// End of File
