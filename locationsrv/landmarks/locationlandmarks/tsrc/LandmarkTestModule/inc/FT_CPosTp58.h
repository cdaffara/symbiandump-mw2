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


#ifndef CPOSTP58_H
#define CPOSTP58_H

//  INCLUDES
#include "FT_CLandmarkTestProcedureBase.h"
#include <ss_std.h>

// CLASS DECLARATION
class CPosLandmarkParser;

/**
*  Test procedure 58
*/
class CPosTp58 :public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
        CPosTp58() : CLandmarkTestProcedureBase(58) {};
        
        /**
        * Destructor.
        */
        ~CPosTp58() {};

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
        * Creates, initializes and opens the default landmark database
        */
        CPosLandmarkDatabase* InitDbLC();

        /**
        * Executes the import test thread
        */
        static void RunImportTestL(TAny* aData);

        /**
        * Executes the thread killing thread
        */
        static void RunThreadKillerL(RThread* aThread);

        /**
        * Does the import of landmarks
        */
        void ImportLandmarksL(const RArray<TUint>* aLandmarkSubSet);

    public:
        
        // Use this enum to define which test case to execute
        enum TTestCase 
            {
            ETestImportAllLandmarks, 
            ETestImportSubsetOfLandmarks
            };

    private:

        void ResumeThreadsAndVerifyExitL();

        /**
        * Returns ETrue if any errors are encountered, EFalse otherwise.
        */
        TBool VerifyThreadExitResults(
            TExitType aKillerThreadExitType,
            TExitType aImportThreadExitType,
            const TExitCategoryName& aImportThreadExitCategory,
            TInt aImportThreadExitReason);

    private:

        TInt                        iExpectedErrorCode;
        TTestCase                   iTestCase;

    };

#endif      // CPOSTp58_H
            
// End of File
