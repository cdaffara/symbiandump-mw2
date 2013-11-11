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
* Description: playlistenginetest header for STIF Test Framework TestScripter.
*
*/



#ifndef PLAYLISTENGINETEST_H
#define PLAYLISTENGINETEST_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include <TestclassAssert.h>

#include "mpxplaylistengine.h"
#include "mpxmediaarray.h"
#include "mpxmedia.h"
#include "mpxmediageneraldefs.h"
#include "mpxmediacontainerdefs.h"

#include "mpxplaylistpluginhandler.h"
#include "charconv.h"

#include "mpxplaylistplugin.h"
#include <ecom/ecom.h>
// CONSTANTS
//const ?type ?constant_var = ?constant;
// MACROS
//#define ?macro ?macro_def
#define TEST_CLASS_VERSION_MAJOR 0
#define TEST_CLASS_VERSION_MINOR 0
#define TEST_CLASS_VERSION_BUILD 0



#ifdef __WINSCW__
_LIT( KplaylistenginetestLogPath, "\\logs\\testframework\\playlistenginetest\\" ); 
_LIT( Kplaylistenginetest_testPath, "c:\\testing\\data\\" );
_LIT( KlistExportPath,"c:\\testing\\listExport\\");
_LIT( KTestItemPath2, "c:\\testing\\data\\mpxtestplaylist.m3u");
#else
_LIT( KplaylistenginetestLogPath, "e:\\testing\\log\\" );
_LIT( Kplaylistenginetest_testPath, "e:\\testing\\data\\" );
_LIT( KlistExportPath,"e:\\testing\\listExport\\");
_LIT( KTestItemPath2, "e:\\testing\\data\\mpxtestplaylist.m3u");
#endif
// Log file
_LIT( KplaylistenginetestLogFile, "playlistenginetest.txt" ); 
_LIT( KplaylistenginetestLogFileWithTitle, "playlistenginetest_[%S].txt" );


class Cplaylistenginetest;
class CMPXPlaylistEngine ;
class CMPXMediaArray;
class CMPXMedia;
class MMPXPlaylistPluginObserver;
class MMPXPluginHandlerObserver;
class MMPXPlaylistEngineObserver;
class CMPXPlaylistPlugin;

// CLASS DECLARATION

/**
*  Cplaylistenginetest test class for STIF Test Framework TestScripter.
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
NONSHARABLE_CLASS(Cplaylistenginetest) : public CScriptBase,
										 public MMPXPlaylistPluginObserver,
										 public MMPXPluginHandlerObserver
										 
    {
    public:  // Constructors and destructor
    	enum TMediaProperties
    	        {
    	        ETitle = 0x01,
    	        EUri = 0x02,
    	        EDuration = 0x04,
    	        EType = 0x08,
    	        ECategory = 0x10,
    	        EArrayContents = 0x20,
    	        EArrayCount = 0x40,
    	        EOthers = 0x80,
    	        EAll = 0xffffffff
    	        };
        /**
        * Two-phased constructor.
        */
        static Cplaylistenginetest* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~Cplaylistenginetest();

    public: // New functions

    	 void HandlePlaylistL(CMPXMedia* aPlaylist,
    	                                     const TInt aError,
    	                                     const TBool aCompleted) ;
    	
    	 void HandlePlaylistL(const TDesC& aPlaylistUri,
    	                                     const TInt aError) ;   
    	
    	 void HandlePluginHandlerEvent(TPluginHandlerEvents aEvent, const TUid& aPluginUid,
    	        TBool aLoaded, TInt aData) ;
    	 
    	 // Create a playlist that contains the specified attributes
	     CMPXMedia* CreatePlaylistL(const TUint& aAttributes);

    public: // Functions from base classes

        /**
        * From CScriptBase Runs a script line.
        * @since ?Series60_version
        * @param aItem Script line containing method name and parameters
        * @return Symbian OS error code
        */
        virtual TInt RunMethodL( CStifItemParser& aItem );  
        
        //test functions for mpxplaylistengine.h
        TInt CMPXPlaylistEngineCreate( CStifItemParser& aItem );
        TInt CMPXPlaylistEngineDestructor( CStifItemParser& aItem );
        TInt CMPXPlaylistEnginePlaylistPluginHandler( CStifItemParser& aItem );
        TInt CMPXPlaylistEngineIsPlaylistL( CStifItemParser& aItem );
        TInt CMPXPlaylistEngineInternalizePlaylistL( CStifItemParser& aItem );
        TInt CMPXPlaylistEngineInternalizePlaylistLRFile( CStifItemParser& aItem );
        TInt CMPXPlaylistEngineExternalizePlaylistL( CStifItemParser& aItem );         
        TInt CMPXPlaylistEngineCancelRequests( CStifItemParser& aItem );
        
        //test functions for mpxplaylistpluginhandler.h
        TInt CMPXPlaylistPluginHandlerCreate( CStifItemParser& aItem );
        TInt CMPXPlaylistPluginHandlerDestructor( CStifItemParser& aItem );
      
        TInt SelectPlaylistPluginLByUri( CStifItemParser& aItem );
        TInt SelectPlaylistPluginLByFile( CStifItemParser& aItem );
        TInt SelectPlaylistPluginLByType( CStifItemParser& aItem );
        TInt SelectPlaylistPluginLByName( CStifItemParser& aItem );
        TInt SelectPlaylistPluginLByUid( CStifItemParser& aItem );

        
        
       

        
    private:

        /**
        * C++ default constructor.
        */
        Cplaylistenginetest( CTestModuleIf& aTestModuleIf );

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

        /**
        * Example test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt ExampleL( CStifItemParser& aItem );
        
        /**
         * Method used to log version of test class
         */
        void SendTestClassVersion();

        //ADD NEW METHOD DEC HERE
        //[TestMethods] - Do not remove

  
    private:    // Friend classes
        //?friend_class_declaration;
    	CMPXPlaylistEngine* iPlaylistEngine;
    	CMPXPlaylistPluginHandler* iPlaylistPluginHandler;
    	CMPXPlaylistPlugin* iMPXPlaylistPlugin;
    };

#endif      // PLAYLISTENGINETEST_H

// End of File
