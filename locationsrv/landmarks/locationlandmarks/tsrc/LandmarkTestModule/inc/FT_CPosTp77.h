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


#ifndef CPOSTP77_H
#define CPOSTP77_H

//  INCLUDES
#include "FT_CLandmarkTestProcedureBase.h"
#include <ss_std.h>

// CLASS DECLARATION
class CPosLandmarkParser;

/**
*  Test procedure 77
*/
class CPosTp77 :public CLandmarkTestProcedureBase
    {
    public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
        CPosTp77() : CLandmarkTestProcedureBase(77) {};
        
        /**
        * Destructor.
        */
        ~CPosTp77() {};

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
        * Executes the bad client test thread
        */
        static void RunBadClientTestL(TAny* aData);

        /**
        * Executes the thread killing thread
        */
        static void RunThreadKillerL(TAny* aData);

    public:
        
        // Use this enum to define which test case to execute
        enum TTestCase 
            {
            ETestNotifyDbEvent, 
            ETestNotifyDbEvent2,
            ETestInitialize,
            ETestInitialize2,
            ETestWriteLock,
            ETestRegisterUri
            };

    private:

        void ResumeThreadsAndVerifyExitL();

        /**
        * Returns ETrue if any errors are encountered, EFalse otherwise.
        */
        TBool VerifyThreadExitResultsL(TInt aBadThreadExitReason);

        /**
        * Returns ETrue if any errors are encountered, EFalse otherwise.
        */
        TBool VerifyLmServerIsAliveL(CPosLandmarkDatabase* aDb);

    public:

        RThread     iBadThread;
        TTimeIntervalMicroSeconds32 iTimeToWait;

    private:

        TInt                iExpectedErrorCode;
        TTestCase           iTestCase;
        TExitType           iBadThreadExitType, iKillerThreadExitType;
        TExitCategoryName   iBadThreadExitCategory, iKillerThreadExitCategory;
        TInt                iBadThreadExitReason, iKillerThreadExitReason;    
        TTimeIntervalMicroSeconds32 iLastTimeToWait;
        TBuf<100>           iBuf;

    };

#endif      // CPOSTP77_H
            
// End of File
