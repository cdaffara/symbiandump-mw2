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
* Description:  Scripted Tests for MPX Playback Plugin
*
*/

#ifndef PLAYBACKPLUGINTEST_H
#define PLAYBACKPLUGINTEST_H

#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include <TestclassAssert.h>
#include <mpxplaybackobserver.h>
#include "basetest.h"

// CONSTANTS
const TInt KTestPlayback1Seconds = 1000;

// MACROS
#define TEST_CLASS_VERSION_MAJOR 0
#define TEST_CLASS_VERSION_MINOR 0
#define TEST_CLASS_VERSION_BUILD 0

// Logging path
#ifdef __WINSCW__
_LIT( KPlaybackPluginTestLogPath, "\\logs\\testframework\\playbackplugintest\\" ); 
#else
_LIT( KPlaybackPluginTestLogPath, "e:\\logs\\testframework\\playbackplugintest\\" );
#endif

// Log file
_LIT( KPlaybackPluginTestLogFile, "playbackplugintest.txt" );
_LIT( KPlaybackPluginTestLogFileWithTitle, "playbackplugintest_[%S].txt" );

// Other strings
_LIT( KPlaybackPluginTest_digitalPath, "\\testing\\data\\" );
_LIT(KTextHandlePlaybackMessage2, "HandlePlaybackMessage(CMpxMessage)");


// FORWARD DECLARATIONS
class MMPXPlaybackUtility;

/**
*  Cplaybackplugintest test class for STIF Test Framework TestScripter.
*  
*/
NONSHARABLE_CLASS(CPlaybackPluginTest) : public CBaseTest,
                                public MMPXPlaybackObserver
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CPlaybackPluginTest* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~CPlaybackPluginTest();       

    public: // Functions from base classes

        /**
        * From CScriptBase Runs a script line.
        * @since ?Series60_version
        * @param aItem Script line containing method name and parameters
        * @return Symbian OS error code
        */
        virtual TInt RunMethodL( CStifItemParser& aItem );
        
        // From MMPXPlaybackObserver
        
        /**
        *  Handle playback message.
        *  Note: only one of HandlePlaybackMessage callback can be implemented.
        *  Note: change into pure virtual when all clients updated.
        *
        *  @since S60 3.2.3
        *  @param aMsg playback message, ownership not transferred. 
        *         Please check aMsg is not NULL before using it. If aErr is not 
        *         KErrNone, plugin might still call back with more info in the aMsg.
        *  @param aErr system error code.
        */
        void HandlePlaybackMessage(
            CMPXMessage* aMessage, TInt aError );      

    private:

        /**
        * C++ default constructor.
        */
        CPlaybackPluginTest( CTestModuleIf& aTestModuleIf );

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
        * Test methods are listed below. 
        */
        TInt MMPXPlaybackUtilityNewL(CStifItemParser& /*aItem*/);        
        TInt MMPXPlaybackUtilityNewLHelixVideoPlugin(CStifItemParser& /*aItem*/);
        TInt MMPXPlaybackUtilityNewLLocalAudioPlugin(CStifItemParser& /*aItem*/);
        TInt MMPXPlaybackUtilityUtilityL(CStifItemParser& /*aItem*/);
       
        //to test  virtual void InitStreamingL(const TDesC& aUri, 
        //                                     const TDesC8* aType,
        //                                     const TInt aAccessPoint)=0;
        TInt InitStreamingL( CStifItemParser& aItem );
        //to test virtual void Close()=0;
        TInt Close( CStifItemParser& aItem );
        //to test virtual void InitStreamingL(RFile& aShareableFile, const TInt aAccessPoint)=0;
        TInt InitStreaming2L( CStifItemParser& aItem );
        
        TInt InitRFileL( CStifItemParser& aItem );
        TInt InitURIL( CStifItemParser& aItem );
        
        /**
         * Method used to log version of test class
         */
        void SendTestClassVersion();
        
        //ADD NEW METHOD DEC HERE
        //[TestMethods] - Do not remove

    private:
        
        MMPXPlaybackUtility*      iPlaybackUtility;
    };

#endif      // PLAYBACKPLUGINTEST_H

// End of File
