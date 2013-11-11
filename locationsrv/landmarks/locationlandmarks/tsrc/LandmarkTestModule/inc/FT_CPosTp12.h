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


#ifndef CPOSTP12_H
#define CPOSTP12_H

//  INCLUDES
#include "EPos_LandmarksErrors.h"

#include "FT_CLandmarkTestProcedureBase.h"

// CLASS DECLARATION

/**
*  Test procedure 12
*/
class CPosTp12 : public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
       
        CPosTp12(TInt tpno, CStifLogger* aLog) : CLandmarkTestProcedureBase(tpno, aLog) {};
        /**
        * Destructor.
        */
        ~CPosTp12() {};

    public:
       
        static void RunPanicTestL(TAny* aData);
       
    public: // Functions from base classes

        
        /**
        * From CLandmarkTestProcedureBase
        * Runs the test in the module
        */
        void StartL();

        /**
        * From CLandmarkTestProcedureBase
        * Always called, even if the test leaves
        */
        void CloseTest();

    private:

      void CreateThreadL();

      void ResumeThreadAndVerifyExit();

      void ResourceFilePanicTestL();

      CPosLandmarkDatabase* UseInvalidResourceFileL();
    
      void RestoreResourceFileL();

      

    private:

        RThread                         iThread;
        TPositionLandmarksClientPanic   iExpectedPanicCode;
        TInt                            iTestCase;
        TInt                            iTreadCounter;
    };

#endif      // CPOSTP12_H
            
// End of File
