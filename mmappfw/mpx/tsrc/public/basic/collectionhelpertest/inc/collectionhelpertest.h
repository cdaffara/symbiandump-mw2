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
* Description:  Scripted Tests for MPXCollectionHelper
*
*/

#ifndef COLLECTIONHELPERTEST_H
#define COLLECTIONHELPERTEST_H


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
#include "basetest.h"



// Logging path
#ifdef __WINSCW__
_LIT( KCollectionHelperTestLogPath, "\\logs\\testframework\\CollectionHelperTest\\" ); 
#else
_LIT( KCollectionHelperTestLogPath, "e:\\logs\\testframework\\CollectionHelperTest\\" );
#endif

// Log file
_LIT( KCollectionHelperTestLogFile, "CollectionHelperTest.txt" ); 


/**
*  CCollectionHelperTest test class for STIF Test Framework TestScripter.
*
*/
NONSHARABLE_CLASS(CCollectionHelperTest) : public CBaseTest,
								public MMPXCollectionHelperObserver,
								public MMPXCHelperObserver,
								public MMPXCHelperEmbeddedOpenObserver,
                                public MMPXCollectionObserver,
           						public MMPXCollectionRemoveObserver,
           						public MMPXCollectionFindObserver
												
    {
	public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CCollectionHelperTest* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~CCollectionHelperTest();       

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
    	virtual void HandleEmbeddedOpenL( TInt aErr, 
                                          TMPXGeneralCategory aCategory );

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
	    
    private:

        /**
        * C++ default constructor.
        */
        CCollectionHelperTest( CTestModuleIf& aTestModuleIf );

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

        TInt OpenCollectionUtilityL(CStifItemParser& /*aItem*/);
		TInt NewUiHelperL(CStifItemParser& /*aItem*/);
        TInt NewHelperL(CStifItemParser& /*aItem*/);
        TInt NewCachedHelperL(CStifItemParser& /*aItem*/);
		TInt CloseUiHelperL(CStifItemParser& /*aItem*/);
		TInt CloseHelperL(CStifItemParser& /*aItem*/);
		TInt CloseCachedHelperL(CStifItemParser& /*aItem*/);      
        TInt AddFileL(CStifItemParser& /*aItem*/);
        TInt AddMediaL(CStifItemParser& /*aItem*/);
        TInt RemoveFileL(CStifItemParser& /*aItem*/);
        TInt RemoveAllL(CStifItemParser& /*aItem*/);
        TInt CleanupDeletedMediasL(CStifItemParser& /*aItem*/);
        TInt SetMediaL(CStifItemParser& /*aItem*/);
        TInt RenameUriL(CStifItemParser& /*aItem*/);
        TInt GetFileL(CStifItemParser& /*aItem*/);
        TInt GetFile2L(CStifItemParser& /*aItem*/);
        TInt FindAllL(CStifItemParser& /*aItem*/);

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
    	void HandleFindAllL(const CMPXMedia& aResults, TBool aComplete,
                            TInt aError);
    	        
        void CreateTestMediaL( CMPXMedia*& aNewProperty );
        
    private:    // Data
    
    	MMPXCollectionUtility*  	iCollectionUtility;

    	//NewUiHelperL()
        MMPXCollectionUiHelper* 	iUIHelper;  
             
        //NewHelperL()
		MMPXCollectionHelper* 		iHelper;
		
		//NewCachedHelperL()
		MMPXCollectionHelper* 		iCachedHelper;		
    };

#endif      // COLLECTIONHELPERTEST_H

// End of File
