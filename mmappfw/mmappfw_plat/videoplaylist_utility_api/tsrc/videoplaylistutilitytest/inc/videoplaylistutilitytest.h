/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  videoplaylistutility test class for STIF testing
*
*/

// Version : %version: 2 %


#ifndef VIDEOPLAYLISTUTILITY_TEST_H
#define VIDEOPLAYLISTUTILITY_TEST_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include <TestclassAssert.h>
#include "videoplaylistutilitytester.h"

// CONSTANTS


// MACROS
#define TEST_CLASS_VERSION_MAJOR 0
#define TEST_CLASS_VERSION_MINOR 0
#define TEST_CLASS_VERSION_BUILD 0

// Logging path
_LIT( Kvideoplaylistutility_testLogPath, "\\logs\\testframework\\videoplaylistutility_test\\" );

// Log file
_LIT( Kvideoplaylistutility_testLogFile, "videoplaylistutility_test.txt" );
_LIT( Kvideoplaylistutility_testLogFileWithTitle, "videoplaylistutility_test_[%S].txt" );

// FUNCTION PROTOTYPES


// FORWARD DECLARATIONS
class Cvideoplaylistutility_test;
class CVideoPlaylistUtility_Tester;

// DATA TYPES

// CLASS DECLARATION

/**
*  Cvideoplaylistutility_test test class for STIF Test Framework TestScripter.
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
NONSHARABLE_CLASS(Cvideoplaylistutility_test) : public CScriptBase
{
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static Cvideoplaylistutility_test* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~Cvideoplaylistutility_test();

    public: // Functions from base classes

        /**
        * From CScriptBase Runs a script line.
        * @since ?Series60_version
        * @param aItem Script line containing method name and parameters
        * @return Symbian OS error code
        */
        virtual TInt RunMethodL( CStifItemParser& aItem );

    private:

        /**
        * C++ default constructor.
        */
        Cvideoplaylistutility_test( CTestModuleIf& aTestModuleIf );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        /**
        * Frees all resources allocated from test methods.
        * @since ?Series60_version
        */
        void Delete();

        /**
        * Test methods are listed below.
        */

        virtual TInt CreatePlaylistUtilityL( CStifItemParser& aItem );
        virtual TInt GetFileInfoL( CStifItemParser& aItem );
        virtual TInt GetLinkL( CStifItemParser& aItem );
        virtual TInt GetPlaylistL( CStifItemParser& aItem );
        virtual TInt IsSeekableL( CStifItemParser& aItem );


        /**
         * Method used to log version of test class
         */
        void SendTestClassVersion();


    private:    // Friend classes

        CVideoPlaylistUtility_Tester* iPlaylistUtilityTester;
};

#endif      // VIDEOPLAYLISTUTILITY_TEST_H

// End of File
