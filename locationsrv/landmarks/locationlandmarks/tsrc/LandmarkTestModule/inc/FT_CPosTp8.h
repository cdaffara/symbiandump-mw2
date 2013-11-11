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


#ifndef CPOSTP8_H
#define CPOSTP8_H

//  INCLUDES
#include "FT_CLandmarkTestProcedureBase.h"
#include <e32cmn.h>

// CLASS DECLARATION

/**
*  Test procedure 8
*/
class CPosTp8 : public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
        
        CPosTp8(TInt tpno, CStifLogger* aLog) : CLandmarkTestProcedureBase(tpno, aLog) {};
        /**
        * Destructor.
        */
        ~CPosTp8() {};

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
        void AddLandmarksL(CPosLandmarkDatabase* aDatabase);
        
        void UpdateLandmarksL(CPosLandmarkDatabase* aDatabase);    
        
        void ReadLandmarksL(CPosLandmarkDatabase* aDatabase);

        void RemoveLandmarksL(CPosLandmarkDatabase* aDatabase);

        void RemoveLandmarksAsyncL(CPosLandmarkDatabase* aDatabase);

        void RemoveLandmarksAsyncWithWaitForReqL(CPosLandmarkDatabase* aDatabase);

        void CheckRemovedL(const TInt startIndex, const TInt stopIndex);

        void RemoveSetOfLandmarksL(CPosLandmarkDatabase* aDatabase);

        void ReadLandmarkL(TPosLmItemId aCategoryId);

        void RemoveLandmarksAsyncAndCancelL();
        
        void CheckLandmarkNameMaxLengthL();
        
        CPosLandmarkDatabase* BackupTp8DbFileL();
        
        CPosLandmarkDatabase* RestoreTp8DbFileL();
        
    private: //data
        
        RPointerArray<CPosLandmark> iLandmarks;
          
    };

#endif      // CPOSTP8_H
            
// End of File
