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


#ifndef CPOSTp126_H
#define CPOSTp126_H

//  INCLUDES
#include "FT_CLandmarkTestProcedureBase.h"
#include <EPos_CPosLMCategoryManager.h>
#include <EPos_CPosLmMultiDbSearch.h>
#include "EPos_LandmarksErrors.h"

enum TEnumStep
    {
    EStepZeroMaxMatches = 0,
    EStepNegativeMaxMatches,
    EStepUnlimitedMaxMatches,
    EStepOutOfBoundsDatabaseUriPtr,
    EStepOutOfBoundsGetSearchError,
    EStepTooBigValueMatchIteratorL,
    EStepTooBigValueNumOfMatches,
    EStepIllegalSortPrefLMa,
    EStepIllegalSortPrefLMb,
    EStepIllegalSortPrefLMc,
    EStepIllegalSortPrefLMd,
    EStepIllegalSortPrefCat,
    EStepNULLSearch1,
    EStepNULLSearch2,
    EStepNULLSearch3,
    EStepNULLSearch4
    };

// CLASS DECLARATION

/**
*  Test procedure 126
*/
class CPosTp126 : public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
        
        CPosTp126(TInt tpno, CStifLogger* aLog) : CLandmarkTestProcedureBase(tpno, aLog) {};
        /**
        * Destructor.
        */
        ~CPosTp126() {delete iLandmarksMultiDbSearch;
                      iThread.Close();};
        
        static void RunPanicTestL(TAny* aData);

    public: // Functions from base classes

        
        
        void CloseTest();

        /**
        * From CLandmarkTestProcedureBase
        * Runs the test in the module
        */
        void StartL();

    private:
        void CreateThreadL();
        void ResumeThreadAndVerifyExitL();
        void ResourceFilePanicTestL();
        
        // data members
    private:
    
        CPosLmMultiDbSearch*            iLandmarksMultiDbSearch;
        RThread                         iThread;
        TPositionLandmarksClientPanic   iExpectedPanicCode;
        
        TEnumStep                       iStep;
    };

#endif      // CPOSTp126_H
            
// End of File
