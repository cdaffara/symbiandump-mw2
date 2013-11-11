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


#ifndef CPOSTP79_H
#define CPOSTP79_H

// INCLUDES
#include "FT_CLandmarkTestProcedureBase.h"
#include <ss_std.h>
#include <e32std.h>

// CLASS DECLARATION
class CPosLandmarkParser;

/**
*  Test procedure 79
*/
class CPosTp79 :public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
        CPosTp79() : CLandmarkTestProcedureBase(79) {};
        
        /**
        * Destructor.
        */
        ~CPosTp79() {};

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
        * From CUtfwTestBase
        * Always called before StartL()
        */
        void InitTestL();

        /**
        * From CUtfwTestBase
        * Always called, even if test leaves
        */
        void CloseTest();

        /**
        * From CLandmarkTestProcedureBase
        * Runs the test in the module
        */
        void StartL();

    public: // New functions
       
        /**
        * Creates and initializes the default landmark database
        */
        void InitDbL();

        /**
        * Executes the search test thread
        */
        static void RunSearchTestL(TDes* aMessage);

        /**
        * Executes the add remove categories test thread
        */
        static void RunAddRemoveCatsTestL(TDes* aMessage);

        /**
        * Executes the read and update test thread
        */
        static void RunReadAndUpdateTestL(TDes* aMessage);

    private:

        void ResumeThreadsAndVerifyExitL();

    private:

        TBuf<100>               iBuf;

    };

#endif      // CPOSTP79_H
            
// End of File
