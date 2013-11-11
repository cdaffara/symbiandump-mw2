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
* Description: test class header file for STIF Test Framework TestScripter
*
*/



#ifndef HARVESTERMETADATAEXTRACTORTEST_H
#define HARVESTERMETADATAEXTRACTORTEST_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include <TestclassAssert.h>
#include <mpxmedia.h>
#include <APGCLI.H>
#include <mpxcollectiontype.h>
#include <mpxmetadataextractor.h>
#include <MetaDataUtility.h>
#include <mpxharvesterutility.h>
#include <mpxcollectionobserver.h>
#include <mpxharvesterutilityobserver.h>


//#include <mpxmetadataextractor.h>
// CONSTANTS
//const ?type ?constant_var = ?constant;


// Logging path
#ifdef __WINSCW__
_LIT( KharvestermetadataextractortestLogPath, "C:\\logs\\testframework\\harvestermetadataextractortest\\" ); 
// Log file
_LIT( KharvestermetadataextractortestLogFile, "harvestermetadataextractortest.txt" ); 
_LIT( KharvestermetadataextractortestLogFileWithTitle, "harvestermetadataextractortest_[%S].txt" );
_LIT( KmpxMetadataExtractor_digitalPath, "C:\\testing\\data\\" ); 
_LIT( KTestFile, "C:\\Catwalk.aac");
_LIT( KTestFileMimeType, ".aac" );
#else
// Log file
_LIT( KharvestermetadataextractortestLogPath, "\\logs\\testframework\\harvestermetadataextractortest\\" ); 
// Log file
_LIT( KharvestermetadataextractortestLogFile, "harvestermetadataextractortest.txt" ); 
_LIT( KharvestermetadataextractortestLogFileWithTitle, "harvestermetadataextractortest_[%S].txt" );
_LIT( KmpxMetadataExtractor_digitalPath, "e:\\testing\\data\\" ); 
_LIT( KTestFile, "\\Catwalk.aac");
_LIT( KTestFileMimeType, ".aac" );

#endif


// FUNCTION PROTOTYPES
//?type ?function_name(?arg_list);

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;
class Charvestermetadataextractortest;
class MMPXHarvesterUtility;
class MMPXCollectionUtility;
class CMPXMetadataScanner;
// DATA TYPES
//enum ?declaration
//typedef ?declaration
//extern ?data_type;

// CLASS DECLARATION

/**
*  Charvestermetadataextractortest test class for STIF Test Framework TestScripter.
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
NONSHARABLE_CLASS(Charvestermetadataextractortest) : public CScriptBase,
													 public MMPXCollectionObserver,
													 public MMPXHarvesterUtilityObserver
    {
    private:
    	enum TExpectedEvents
    		{
    		EEventHandlePlaylistExportCompletedL,
    		EEventHandlePlaylistImportCompletedL,
    		EEventHandleFileAddCompletedL,
    		EEventHandleDeleteCompletedL,
    		EEventHandleFileImportCompletedL,
    		EEventHandleFileGetMediaCompletedL
    		};
    
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static Charvestermetadataextractortest* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~Charvestermetadataextractortest();
        
       

  

    public: // Functions from base classes

        /**
        * From CScriptBase Runs a script line.
        * @since ?Series60_version
        * @param aItem Script line containing method name and parameters
        * @return Symbian OS error code
        */
        virtual TInt RunMethodL( CStifItemParser& aItem );
        /**
    	* From MMPXHarvesterUtilityObserver.
    	* @since s60 5.0
    	*/
    	virtual void HandlePlaylistExportCompletedL( CMPXMedia* aMedia, TInt aErr );
    	virtual void HandlePlaylistImportCompletedL( CMPXMedia* aMedia, TInt aErr );
    	virtual void HandleFileAddCompletedL( CMPXMedia* aMedia, TInt aErr );
    	virtual void HandleDeleteCompletedL( TInt aErr );
    	virtual void HandleFileImportCompletedL( CMPXMedia* aMedia, TInt aErr );
        virtual void HandleFileGetMediaCompletedL( CMPXMedia* aMedia, TInt aErr );
        
        /**
    	* From MMPXCollectionObserver.
    	* @since s60 5.0
    	*/
    	virtual void HandleCollectionMediaL( const CMPXMedia& aMedia, TInt aError );
    	virtual void HandleCollectionMessage( CMPXMessage* aMsg, TInt aError );
    	virtual void HandleOpenL( const CMPXMedia& aEntries, TInt aIndex, TBool aComplete, TInt aError );
    	virtual void HandleOpenL( const CMPXCollectionPlaylist& aPlaylist, TInt aError );
        
       
   

    private:

        /**
        * C++ default constructor.
        */
        Charvestermetadataextractortest( CTestModuleIf& aTestModuleIf );

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
    	* Remove event from the expected event list
    	* @since S60 5.0
    	*/
    	TBool RemoveExpectedEvent( TExpectedEvents aEvent );

        /**
        * Test methods are listed below. 
        */
        
        virtual TInt ExampleL( CStifItemParser& aItem );
        TInt CMPXMetadataExtractorCreateInstance( CStifItemParser& aItem );
        TInt CMPXMetadataExtractorCreateMediaL( CStifItemParser& aItem );
        TInt CMPXMetadataExtractorExtractAlbumArtL( CStifItemParser& aItem );
            /**
         * Method used to log version of test class
         */
      
        //ADD NEW METHOD DEC HERE
        //[TestMethods] - Do not remove

   
    private:    // Friend classes
        //?friend_class_declaration;
    	CMPXMetadataExtractor* iMetadataExtractor; 
    	MMPXHarvesterUtility* iHarvester;
    	MMPXCollectionUtility* iCollection;
		RArray<TExpectedEvents> iExpectedEvents;
		TInt iResult;
		TDataType iMimeType;
    };

#endif      // HARVESTERMETADATAEXTRACTORTEST_H

// End of File
