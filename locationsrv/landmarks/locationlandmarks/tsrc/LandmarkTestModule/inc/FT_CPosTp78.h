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


#ifndef CPOSTP78_H
#define CPOSTP78_H

// INCLUDES
#include "FT_CLandmarkTestProcedureBase.h"
#include <ss_std.h>
#include <e32std.h>

// CLASS DECLARATION
class CPosLandmarkParser;
class CPosTp78;

// Data types
struct TThreadParam
    {
    void (*iTestFunction)(TDes*);
    TDes* iMessage;
    };

class CThreadMonitor : public CActive
    {
    public:

        static CThreadMonitor* NewL(RThread* aThread);

        ~CThreadMonitor();

    protected:

        void DoCancel();

        void RunL();

    private:

        CThreadMonitor(RThread* aThread);

        void ConstructL();

    private:

        RThread*    iThread;

    };

/**
*  Test procedure 78
*/
class CPosTp78 :public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
        CPosTp78() : CLandmarkTestProcedureBase(78) {};
        
        /**
        * Destructor.
        */
        ~CPosTp78() {};

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
        * Executes the open database test thread
        */
        static void RunOpenDbTestL(TDes* aMessage);

        /**
        * Executes the add remove LM test thread
        */
        static void RunAddRemoveLmTestL(TDes* aMessage);

        /**
        * Executes the import export LM test thread
        */
        static void RunImportExportLmTestL(TDes* aMessage);

        /**
        * Executes the database event test thread
        */
        static void RunDbEventTestL(TDes* aMessage);

        /**
        * Executes the read and update test thread
        */
        static void RunReadAndUpdateTestL(TDes* aMessage);
        
        /**
        * Function called from RunReadAndUpdateTestL
        */
        static void UpdateLandmarksL(CPosLandmarkDatabase* aDb, TPosLmItemId aId, TDes* aMessage);

    private:

        void ResumeThreadsAndVerifyExitL();

    private:

        TBuf<100>               iBuf;

    };

#endif      // CPOSTP78_H
            
// End of File
