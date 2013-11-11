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


#ifndef CPOSTP132_H
#define CPOSTP132_H

//  INCLUDES
#include "EPos_LandmarksErrors.h"

#include "FT_CLandmarkTestProcedureBase.h"

// CLASS DECLARATION

/**
*  Test procedure 132
*/
class CPosTp132 : public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
       
        CPosTp132(TInt tpno, CStifLogger* aLog) : CLandmarkTestProcedureBase(tpno, aLog) {iErrorsFound=KErrNone;};
        /**
        * Destructor.
        */
        ~CPosTp132() {};

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
        
        /**
        * Used as a callback method to unset display data
        * from landmark search
        * @param self
        */
        TInt static UnsetTest(TAny* aSelf);
        
        /**
        * Used as a callback method to unset display data
        * from landmark multidb search
        * @param self
        */
        TInt static UnsetMultiSearch(TAny* aSelf);

    private:

      void CreateThreadL();

      void ResumeThreadAndVerifyExit();

      void RunLeaveTestL();

    public:
        // Use this enum to define if the thread should exit with panic or leave or return normally
        enum ExitType {ExitPanic, ExitLeave, ExitNormal};

    private:

        RThread     iThread;
        TInt        iExpectedErrorCode;
        ExitType    iExpectedExitType;
        TInt        iTestCase;
        TInt        iErrorsFound;
    };

#endif      // CPOSTP132_H
            
// End of File
