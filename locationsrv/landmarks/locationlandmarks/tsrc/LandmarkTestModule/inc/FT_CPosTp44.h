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


#ifndef CPOSTp44_H
#define CPOSTp44_H

//  INCLUDES
#include "EPos_LandmarksErrors.h"

#include "FT_CLandmarkTestProcedureBase.h"

// CLASS DECLARATION

/**
*  Test procedure 44
*/
class CPosTp44 : public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
       
        CPosTp44(TInt tpno, CStifLogger* aLog) : CLandmarkTestProcedureBase(tpno, aLog) {};
        /**
        * Destructor.
        */
        ~CPosTp44() {};

    public:
       
        /**
        * Includes both Panic testing and testing of correct leave codes
        */
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
        
        static TInt DeleteEncoderCallback(TAny* aSelf);

    private:

      void CreateThreadL();

      void ResumeThreadAndVerifyExit();

      void RunLeaveTestL();

    public:
        enum ExitType {ExitPanic, ExitLeave};

    private:

        RThread     iThread;
        TInt        iExpectedErrorCode;
        ExitType    iExpectedExitType;
        TInt        iTestCase;
    };

#endif      // CPOSTp44_H
            
// End of File
