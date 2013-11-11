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


#ifndef CPOSTp36_H
#define CPOSTp36_H

//  INCLUDES
#include "FT_CLandmarkTestProcedureBase.h"

// CLASS DECLARATION

/**
*  Test procedure 1
*/
class CPosTp36 :public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
        
        CPosTp36(TInt tpno, CStifLogger* aLog) : CLandmarkTestProcedureBase(tpno, aLog) {};
        /**
        * Destructor.
        */
        ~CPosTp36() {};

    public: // Functions from base classes

       
        /**
        * Always called even if test leaves, 
        * after test completion
        */
        void CloseTest();


        /**
        * From CLandmarkTestProcedureBase
        * Runs the test in the module
        */
        void StartL();

    private:
        /**
        *
        */
        void DoTestL(TBool aSync, TInt aTestNr, TInt aNrOfHits);

        /**
        *
        */
        void DoSearchForCategoryL(TBool aSync);
        
        /**
        *
        */
        void RemoveCategoriesFromLandmarksL(TInt aNrOfLandmarks);

        /**
        *
        */
        TInt GetNrOfLandmarksL();

    private:
        RArray<TPosLmItemId> iArray;

    };

#endif      // CPOSTp36_H
            
// End of File
