/*
* Copyright (c) 2002 - 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  STIF for mpx_albumart_utility_api
*
*/



#ifndef MPXALBUMARTUTILITYTEST_H
#define MPXALBUMARTUTILITYTEST_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include <TestclassAssert.h>

#include <mpxalbumartutility.h>
#include <mpxmedia.h>
#include <mpxmediageneraldefs.h>
#include <mpxmediamusicdefs.h>
#include <gulicon.h>
// CONSTANTS
//const ?type ?constant_var = ?constant;

// MACROS
//#define ?macro ?macro_def
#define TEST_CLASS_VERSION_MAJOR 0
#define TEST_CLASS_VERSION_MINOR 0
#define TEST_CLASS_VERSION_BUILD 0

// Logging path
#ifdef __WINSCW__
_LIT( KmpxalbumartutilitytestLogPath, "C:\\logs\\testframework\\mpxalbumartutilitytest\\" ); 
// Log file
_LIT( KmpxalbumartutilitytestLogFile, "mpxalbumartutilitytest.txt" ); 
_LIT( KmpxalbumartutilitytestLogFileWithTitle, "mpxalbumartutilitytest_[%S].txt" );
_LIT( Kmpxalbumartutilitytest_testPath, "C:\\testing\\data\\" );
_LIT( KTestFileMimeType, "audio/aac" );
_LIT( KDestJPG, "C:\\testing\\data\\test.jpg" );
 
#else
_LIT( KmpxalbumartutilitytestLogPath, "E:\\logs\\testframework\\mpxalbumartutilitytest\\" ); 
// Log file
_LIT( KmpxalbumartutilitytestLogFile, "mpxalbumartutilitytest.txt" ); 
_LIT( KmpxalbumartutilitytestLogFileWithTitle, "mpxalbumartutilitytest_[%S].txt" );
_LIT( Kmpxalbumartutilitytest_testPath, "E:\\testing\\data\\" );
_LIT( KTestFileMimeType, "audio/aac" );
_LIT( KDestJPG, "E:\\testing\\data\\test.jpg" );
#endif

// FUNCTION PROTOTYPES
//?type ?function_name(?arg_list);

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;
class Cmpxalbumartutilitytest;
class CMPXAlbumArtUtility;
class CMPXMedia;

/**
*  Cmpxalbumartutilitytest test class for STIF Test Framework TestScripter.
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
NONSHARABLE_CLASS(Cmpxalbumartutilitytest) : public CScriptBase,
											 public	MMPXAlbumArtUtilityObserver
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static Cmpxalbumartutilitytest* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~Cmpxalbumartutilitytest();

    

    public: // Functions from base classes

        /**
        * From CScriptBase Runs a script line.
        * @since ?Series60_version
        * @param aItem Script line containing method name and parameters
        * @return Symbian OS error code
        */
        virtual TInt RunMethodL( CStifItemParser& aItem );

        /**
         *  Notify that extraction of album art started.
         *
         *  @since S60 3.2.3
         */
        virtual void ExtractAlbumArtStarted();

        /**
         *  Notify that the extraction of album art has completed.
         *
         *  @since S60 3.2.3
         *  @param aBitmap a converted image.
         *  @param aErr error code
         */
        virtual void ExtractAlbumArtCompleted( CFbsBitmap* aBitmap, TInt aErr ) ;

   

    private:

        /**
        * C++ default constructor.
        */
        Cmpxalbumartutilitytest( CTestModuleIf& aTestModuleIf );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();


        /**
        * Frees all resources allocated from test methods.
        * @since ?Series60_version
        */
        void Delete();
        
        /* void ExtractAlbumArtStarted();
         void ExtractAlbumArtCompleted( CFbsBitmap* aBitmap, TInt aErr ) ;
*/
        /**
        * Test methods are listed below. 
        */

        /**
        * Example test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt ExampleL( CStifItemParser& aItem );
        TInt CMPXAlbumArtUtilityNewL( CStifItemParser& aItem );//Pass
        TInt CMPXAlbumArtUtilityDestructor( CStifItemParser& aItem );//Pass
        TInt CMPXAlbumArtUtilityExtractAlbumArtL( CStifItemParser& aItem );
        TInt CMPXAlbumArtUtilityDecode( CStifItemParser& aItem );
        TInt CMPXAlbumArtUtilityDecodeASourceJPG( CStifItemParser& aItem );
        TInt CMPXAlbumArtUtilityEncode( CStifItemParser& aItem );
        TInt CMPXAlbumArtUtilityScale( CStifItemParser& aItem );
        TInt CMPXAlbumArtUtilityDecodeL( CStifItemParser& aItem );//Pass
        TInt CMPXAlbumArtUtilityDecodeLASourceJPG( CStifItemParser& aItem );//Pass
        TInt CMPXAlbumArtUtilityEncodeL( CStifItemParser& aItem );//Pass
        TInt CMPXAlbumArtUtilityScaleL( CStifItemParser& aItem );
        TInt CMPXAlbumArtUtilityExtractL( CStifItemParser& aItem );//Pass
        TInt CMPXAlbumArtUtilityBitmapL( CStifItemParser& aItem );
        TInt CMPXAlbumArtUtilityBitmapDataL( CStifItemParser& aItem );
        TInt CMPXAlbumArtUtilityCancelRequest( CStifItemParser& aItem );//Pass
        /**
         * Method used to log version of test class
         */
        void SendTestClassVersion();

        
    private:    // Friend classes
        //?friend_class_declaration;
    	CMPXAlbumArtUtility* iAlbumArtUtility;
    	TBool iAlbumArtConverting;
    	CFbsBitmap*   iAlbumArt;
    	CFbsBitmap* iBackgroundBitmap;
    	TBool         iArtExist;
    	CGulIcon* iDefaultAAImage;
    };

#endif      // MPXALBUMARTUTILITYTEST_H

// End of File
