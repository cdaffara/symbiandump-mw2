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
* Description:  CCollectionHelperTestClass heder for STIF Test Framework TestScripter.
*
*/



#ifndef COLLECTIONHELPERTESTCLASS_H
#define COLLECTIONHELPERTESTCLASS_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include <mpxcollectionutility.h>
#include <mpxcollectionpath.h>
#include <mpxcollectionplaylist.h>
#include <mpxcollectionmessage.h>
#include <mpxcollectionframeworkdefs.h>
#include "mpxcollectionhelper.h"
#include "mpxcollectionhelperfactory.h"
#include "mpxcollectionhelperobserver.h"
#include "mpxcollectionuihelper.h"
#include "mpxcollectionuihelperobserver.h"

#include "TimeoutController.h"


// CONSTANTS
const TInt KErrBadTestParameter= -1000;         // Error on configuration file

const TInt KErrEventPending =-2000;             // Timeout and not callback 
const TInt KErrCallbackErrorCode = -2001;       // Callback return error code
const TInt KErrUnexpectedValue =-2002;          // Unexpected value at setting    
const TInt KExpectedFrequencyChange = -2003;		// Is not finding more than one station
const TInt KErrTimeoutController = -2007;		// Unexpected notification

const TInt KSmallTimeout = 1000;
const TInt KMediumTimeout = 4000;
const TInt KLargeTimeout = 100000;


_LIT( KMsgBadTestParameters, "[Error] No valid test case parameters");


// MACROS
//#define ?macro ?macro_def
// Logging path
#ifdef __WINSCW__
_LIT( KCollectionHelperTestClassLogPath, "c:\\logs\\testframework\\CollectionHelperTestClass\\" ); 
#else
_LIT( KCollectionHelperTestClassLogPath, "e:\\logs\\testframework\\CollectionHelperTestClass\\" );
#endif

// Log file
_LIT( KCollectionHelperTestClassLogFile, "CollectionHelperTestClass.txt" ); 

	
_LIT( KTestFile, "c:\\Blackbird.aac");
_LIT( KTestFileMimeType, "audio/aac" );
_LIT( KRenameOldUri, "c:\\Blackbird.aac");
_LIT( KRenameNewUri, "c:\\data\\Blackbird.aac");

_LIT( KFileWmaSong, "c:\\data\\sounds\\digital\\1.mp3");

_LIT( KFile1, "e:\\data\\sounds\\digital\\1.mp3");    //ok
_LIT( KOldAbstractAlbumName, "1.mp3");    //ok
_LIT( KNewAbstractAlbumName, "The 99 Most Essential Beethoven MasterpiecesVarious Artists");    //ok
_LIT( KFile3, "e:\\data\\sounds\\digital\\(01) Amber - Yes.mp3");    //ok
_LIT( KFilet, "e:\\data\\sounds\\digital\\02 What A Wonderful World.mp3");    //ok


_LIT( KTestFile3, "e:\\data\\sounds\\digital\\Atomic Kitten - It's Ok.mp3");    //ok
_LIT( KSongtest, "e:\\data\\sounds\\digital\\02 What A Wonderful World.mp3"); //ok

_LIT( KAbstractalbum1, "e:\\data\\sounds\\digital\\1.alb");
_LIT( KAbstractalbum2, "e:\\data\\sounds\\digital\\2.alb");
_LIT( KAbstractalbum3, "e:\\data\\sounds\\digital\\3.alb");
_LIT( KStoreRoot, "e:\\");

_LIT( KAbstractAlbumName, "The 99 Most Essential Beethoven MasterpiecesVarious Artists");

_LIT( KAlbumArtistShort, "testAlbumArtist");



// FORWARD DECLARATIONS	
class CCollectionHelperTestClass;


/**
*  CCollectionHelperTestClass test class for STIF Test Framework TestScripter.
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
NONSHARABLE_CLASS(CCollectionHelperTestClass) : public CScriptBase, 
												public MTimeoutObserver, 
												public MMPXCollectionHelperObserver,
												public MMPXCHelperObserver,
												public MMPXCHelperEmbeddedOpenObserver,                        public MMPXCollectionObserver,
                         						public MMPXCollectionRemoveObserver,
                         						public MMPXCollectionFindObserver
												
    {
	public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CCollectionHelperTestClass* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~CCollectionHelperTestClass();
        
		enum TCollectionHelperExpectedEvent
		{
			EHandleCollectionMessage,
			EHandleOpen,
			EHandleAddFileComplete,	
			EHandleRemove,
			EHandleFindAllComplete
		};

    public: // New functions

		/**
    	* Callback function to signal that adding a file is complete
    	* @param aErr, error of the operation
	    */
    	virtual void HandleAddFileCompleteL( TInt aErr ); 

		/**
		* Generic Observer class for the ui helper
		* A single callback function, but usable for all collection helper callbacks
		*/   
		virtual void HandleOperationCompleteL( TCHelperOperation aOperation,
        	                                   TInt aErr,
            	                               void* aArgument );
                                           
    	/**
    	* Callback to the observer for embedded OpenL() result
    	* @param aErr, error for the operation, KErrNone if successful
    	* @param aCategory, category of the file opened
    	*/
    	virtual void HandleEmbeddedOpenL( TInt aErr, TMPXGeneralCategory aCategory );

        /**
        *  From MMPXCollectionObserver
        *  Handle extended media properties
        *  @param aMedia media 
        *  @param aError error code    
        */
        void HandleCollectionMediaL(
                        const CMPXMedia& aMedia, 
                        TInt aError);
            
        /**
        *  From MMPXCollectionObserver
        *  Handle collection message
        *  @param aMessage collection message
        */
        void HandleCollectionMessageL(const CMPXMessage& aMsg);

        /**
        *  From MMPXCollectionObserver
        *  Handles the collection entries being opened. Typically called
        *  when client has Open()'d a folder
        *
        *  @param aEntries collection entries opened
        *  @param aIndex focused entry
        *  @param aComplete ETrue no more entries. EFalse more entries
        *                   expected
        *  @param aError error code   
        */
        void HandleOpenL(const CMPXMedia& aEntries,
                                 TInt aIndex,TBool aComplete,TInt aError);
        
        /**
        *  From MMPXCollectionObserver
        *  Handles the item being opened. Typically called
        *  when client has Open()'d an item. Client typically responds by
        *  'playing' the item via the playlist
        *
        *  @param aPlaylist collection playlist, owner ship is transfered    
        *  @param aError error code   
        */                         
        void HandleOpenL(const CMPXCollectionPlaylist& aPlaylist,TInt aError);
        
                		    
        void HandleCommandComplete(CMPXCommand* aCommandResult, TInt aError);
                                                     
    public: // Functions from base classes

        /**
        * From CScriptBase Runs a script line.
        * @since ?Series60_version
        * @param aItem Script line containing method name and parameters
        * @return Symbian OS error code
        */
        virtual TInt RunMethodL( CStifItemParser& aItem );


	    // From MTimeoutObserver

	    /**
	    * @since 3.2
	    * @param none
	    * Review if all the expected events have ocurred once the time is over
	    */
	    void HandleTimeout(TInt error);
	    
    private:

        /**
        * C++ default constructor.
        */
        CCollectionHelperTestClass( CTestModuleIf& aTestModuleIf );

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
        * @since S60 3.2
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */

	    /**
	    * Set an event as expected and set default timeout
	    * @since S60 3.2
	    */
	    void AddExpectedEvent(TCollectionHelperExpectedEvent event, TInt ms);


	    /**
	    * Unset an event as expected
	    * @since S60 3.2
	    */
	    TBool RemoveExpectedEvent(TCollectionHelperExpectedEvent event);


	    /**
	    * @since S60 3.2
	    * @param none
	    * Removes all expected events
	    */
	    void RemoveAllExpectedEvents();

	    /**
	    * Verify that the event was expected, removes it from the list
	    * Signal the TestScripter with the returned error code
	    * @since S60 3.2
	    */
	    void ProcessEvent(TCollectionHelperExpectedEvent aEvent, TInt aError);

	    /**
	    * Maps a event with a descriptor with its name
	    * @since S60 3.2
	    */
	    TPtrC EventName( TInt aKey );

	    /*
	    * Test methods are listed below.
	    */

	    /**
	    * Sets a timeout different since the default
	    * @since S60 3.2
	    * @param aItem Script line containing parameters.
	    * @return Symbian OS error code.
	    */
	    TInt SetTimeout( CStifItemParser& aItem );

	    /**
	    *
	    * @since S60 3.2
	    * @param aItem Script line containing parameters.
	    * @return Symbian OS error code.
	    */
	    TInt SetExpectedEvents( CStifItemParser& aItem );

	    /**
	    *
	    * @since S60 3.2
	    * @param aItem Script line containing parameters.
	    * @return Symbian OS error code.
	    */
	    TInt SetAllowedPanic( CStifItemParser& aItem );
	    
	    void CreateTestMediaL( CMPXMedia*& aNewProperty );
	    /**
	    * Connect to the default client to the server
	    * @since S60 3.2
	    * @param aItem Script line containing parameters.
	    * @return Symbian OS error code.
	    */

        virtual TInt ExampleL( CStifItemParser& aItem );
        virtual TInt OpenCollectionUtilityL(CStifItemParser& aItem);
		virtual TInt NewUiHelperL(CStifItemParser& aItem);
        virtual TInt NewHelperL(CStifItemParser& aItem);
        virtual TInt NewCachedHelperL(CStifItemParser& aItem);
		virtual TInt CloseUiHelperL(CStifItemParser& aItem);
		virtual TInt CloseHelperL(CStifItemParser& aItem);
		virtual TInt CloseCachedHelperL(CStifItemParser& aItem);      

        virtual TInt AddSongL(CStifItemParser& aItem);
        TInt FindMediaL(const TDesC& aPath, TMPXGeneralCategory aCategory);
		virtual TInt RemoveSongL(CStifItemParser& aItem );
	    virtual TInt AddAbstractAlbumL(CStifItemParser& aItem);
	    virtual TInt RemoveAbstractAlbumL(CStifItemParser& aItem);
	    virtual TInt SetAbstractAlbumL(CStifItemParser& aItem);
	    virtual TInt SetSongAlbumArtistL(CStifItemParser& aItem);
	    virtual TInt GetSongAlbumArtistL(CStifItemParser& aItem);
	    virtual TInt SetAbstractAlbumArtistL(CStifItemParser& aItem);
	    virtual TInt GetAbstractAlbumArtistL(CStifItemParser& aItem);
	    virtual TInt UpdateAbstractAlbumNameL(CStifItemParser& aItem);
	    virtual TInt GetAbstractAlbumAndSongsL(CStifItemParser& aItem);
	    virtual TInt RenameAbstractAlbumL(CStifItemParser& aItem);
        void AttachSongsL(CMPXMediaArray* aAbstractalbumArray, const TDesC& aPath);
    	/**
    	*  From MMPXCollectionRemoveObserver
    	*  Handles removing a collection path
    	*  calls back with the list of file paths that were deleted
    	*  This callback is only applicable for local file storage plugins.
    	*
    	*  @param aUriArray, list of files that were deleted
    	*/
    	void HandleRemoveL(MDesCArray& aUriArray, TInt aError);

		//***********************************************************************

	    /**
	    *  From MMPXCollectionFindObserver
    	*  Handle callback for "find" operation
    	*  @param aEntries, CMPXMedia to be returned
    	*  @param aComplete ETrue no more entries. EFalse more entries
    	*                   expected
    	*  @param aError error code   
    	*/
    	void HandleFindAllL(const CMPXMedia& aResults, TBool aComplete,TInt aError);
    
	
    public:     // Data

    protected:  // Data

    private:    // Data
              
        // reference to TestModuleIf
    	CTestModuleIf& iTestModuleIf;
    
    	MMPXCollectionUtility*  	iCollectionUtility;

    	//NewUiHelperL()
        MMPXCollectionUiHelper* 	iUIHelper;  
             
        //NewHelperL()
		MMPXCollectionHelper* 		iHelper;
		
		//NewCachedHelperL()
		MMPXCollectionHelper* 		iCachedHelper;
		
    	// Active object with a timer to timeout the test case
    	CSimpleTimeout * iTimeoutController;

    	// Indicates if the test case use a normal exit reason
    	TBool iNormalExitReason;

    	// List of expected events
    	RArray<TCollectionHelperExpectedEvent> iExpectedEvents;

    	// List of notification event (by the callback)
    	RArray<TCollectionHelperExpectedEvent> iOcurredEvents;

    	public:     // Friend classes
        	//?friend_class_declaration;
    	protected:  // Friend classes
        	//?friend_class_declaration;
    	private:    // Friend classes
        	//?friend_class_declaration;
		
    };

#endif      // COLLECTIONHELPERTESTCLASS_H

// End of File
