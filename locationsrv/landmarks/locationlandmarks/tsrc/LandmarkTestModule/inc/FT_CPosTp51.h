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


#ifndef CPOSTp51_H
#define CPOSTp51_H

//  INCLUDES
#include "EPos_LandmarksErrors.h"

#include "FT_CLandmarkTestProcedureBase.h"

// CLASS DECLARATION

/**
*  Test procedure 51
*/
class CPosTp51 : public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
       
        CPosTp51(TInt tpno, CStifLogger* aLog) : CLandmarkTestProcedureBase(tpno, aLog) {};
        /**
        * Destructor.
        */
        ~CPosTp51() {};

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

    private:

      void CreateThreadL();

      void ResumeThreadAndVerifyExitL();

      void RunLeaveTestL();
      
      void ListAllServers();

    public:
        // Use this enum to define if the thread should exit with panic or leave
        enum ExitType {ExitPanic, ExitLeave};

    private:

        RThread     iThread;
        TInt        iExpectedErrorCode;
        ExitType    iExpectedExitType;
        TInt        iTestCase;
        
        TPtrC8      iMimeType;
        TInt        iThreadCounter;
        TInt        iErrorsFound;
    };

#endif      // CPOSTp51_H
            
// End of File
