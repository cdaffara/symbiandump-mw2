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


#ifndef CPOSTP9_H
#define CPOSTP9_H

//  INCLUDES
#include <EPos_CPosLandmark.h> 
#include "FT_CLandmarkTestProcedureBase.h"

// CLASS DECLARATION


/**
*  Test procedure 9
*/
class CPosTp9 :public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
       
       CPosTp9(TInt tpno, CStifLogger* aLog) : CLandmarkTestProcedureBase(tpno, aLog) {};
        
        /**
        * Destructor.
        */
        ~CPosTp9() {};

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

        void AddLandmarksL();
        
        void IterateLandmarksL();

        void IterateSetOfLandmarksL();

        void IterateSortedLandmarksL();

        CPosLandmark* InternalGet(TPosLmItemId id);
       
        void CheckIteratorArgL(const CPosLandmark::TAttributes& aAttr);
        
       

    private: // data
        
        RPointerArray<CPosLandmark> iLandmarks;
    };

#endif      // CPOSTP9_H
            
// End of File
