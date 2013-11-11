/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Description
*
*/

// Version : %version: 6 %




#ifndef PLAYBACKHELPERTEST_H
#define PLAYBACKHELPERTEST_H

//  INCLUDES
#include <StifLogger.h>
#include <StifTestModule.h>
#include <TestScripterInternal.h>

// MACROS
//#define ?macro ?macro_def
#define TEST_CLASS_VERSION_MAJOR 0
#define TEST_CLASS_VERSION_MINOR 0
#define TEST_CLASS_VERSION_BUILD 0

// Logging path
_LIT( KPlaybackHelperTestLogPath, "C:\\logs\\testframework\\PlaybackHelperTest\\" ); 
// Log file
_LIT( KPlaybackHelperTestLogFile, "PlaybackHelperTest.txt" ); 
_LIT( KPlaybackHelperTestLogFileWithTitle, "PlaybackHelperTest_[%S].txt" );


#ifdef __WINSCW__
_LIT( KPlaybackHelper_testPath, "\\logs\\testframework\\PlaybackHelperTest\\" );
#else
_LIT( KPlaybackHelper_testPath, "e:\\logs\\testframework\\PlaybackHelperTest\\" );
#endif


// FORWARD DECLARATIONS
class CMediaRecognizer;
class CStreamingLinkModel;
class CDevSoundIf;

// DATA TYPES
//enum ?declaration
//typedef ?declaration
//extern ?data_type;

// CLASS DECLARATION

/**
*  CPlaybackHelperTest test class for STIF Test Framework TestScripter.
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
NONSHARABLE_CLASS(CPlaybackHelperTest) : public CScriptBase
{
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CPlaybackHelperTest* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~CPlaybackHelperTest();

    public: // New functions

        /**
        * ?member_description.
        * @since ?Series60_version
        * @param ?arg1 ?description
        * @return ?description
        */
        //?type ?member_function( ?type ?arg1 );
        
    public: // Functions from base classes
        
        /**
        * From CScriptBase Runs a script line.
        * @since ?Series60_version
        * @param aItem Script line containing method name and parameters
        * @return Symbian OS error code
        */
    	virtual TInt RunMethodL( CStifItemParser& aItem );
          
    protected:  // New functions
           
        /**
        * ?member_description.
        * @since ?Series60_version
        * @param ?arg1 ?description
        * @return ?description
        */
        //?type ?member_function( ?type ?arg1 );
        
    protected:  // Functions from base classes

        /**
        * From ?base_class ?member_description
        */
        //?type ?member_function();

    private:

        /**
        * C++ default constructor.
        */
        CPlaybackHelperTest( CTestModuleIf& aTestModuleIf );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        // Prohibit copy constructor if not deriving from CBase.
        // ?classname( const ?classname& );
        // Prohibit assigment operator if not deriving from CBase.
        // ?classname& operator=( const ?classname& );

        /**
        * Frees all resources allocated from test methods.
        * @since ?Series60_version
        */
        void Delete();
        
        /**
         * Method used to log version of test class
         */
        void SendTestClassVersion();

        /**
        * Test methods are listed below. 
        */

        /**
        * Example test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        TInt IdentifyMediaTypeViaFileName( CStifItemParser& aItem );
        TInt IdentifyMediaTypeViaFileHandle( CStifItemParser& aItem );
        TInt MimeTypeViaFileName( CStifItemParser& aItem );
        TInt MimeTypeViaFileHandle( CStifItemParser& aItem );
        TInt IsValidStreamingPrefix( CStifItemParser& aItem );
        TInt IsValidUrlPrefix1( CStifItemParser& aItem );
        TInt IsValidUrlPrefix2( CStifItemParser& aItem );
        TInt FreeFilehandleForRecognizer();

        TInt OpenLinkFile1( CStifItemParser& aItem );
        TInt OpenLinkFile2( CStifItemParser& aItem );
        TInt GetNextLink( CStifItemParser& aItem );
        TInt MaxLinkLength( CStifItemParser& aItem );
        TInt CreateNewLinkFile( CStifItemParser& aItem );
        TInt MultiLinksCount( CStifItemParser& aItem );
        TInt ResetLinkCount();
        TInt FreeFilehandleForLink();
		TInt OpenAsxLinkViaFileName( CStifItemParser& aItem );
		TInt OpenAsxLinkViaFileHandle( CStifItemParser& aItem );
		TInt GetNumberOfVolumeSteps();
		TInt IsSeekable( CStifItemParser& aItem );
		
    private:    // Data
        CMediaRecognizer*    iMediaRecognizer;
        CStreamingLinkModel* iStreamingLinkModel;
		CDevSoundIf*         iDevSoundIf;
};

#endif      // PLAYBACKHELPERTEST_H

// End of File
