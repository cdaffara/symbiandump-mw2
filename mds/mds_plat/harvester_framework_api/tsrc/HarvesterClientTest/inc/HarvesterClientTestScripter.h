/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  ?Description
*
*/


#ifndef HARVESTERCLIENTTESTSCRIPTER_H
#define HARVESTERCLIENTTESTSCRIPTER_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include "harvesterclient.h"
#include "mdccommon.h"
#include "mdesession.h"
#include "mdequery.h"
#include "harvestermediaidutil.h"
#include "blacklistclient.h"

// CONSTANTS

// MACROS
// Logging path
_LIT( KHarvesterClientTestScripterLogPath, "\\logs\\testframework\\HarvesterClientTestScripter\\" ); 
// Log file
_LIT( KHarvesterClientTestScripterLogFile, "HarvesterClientTestScripter.txt" ); 
_LIT( KHarvesterClientTestScripterLogFileWithTitle, "HarvesterClientTestScripter_[%S].txt" );

// FORWARD DECLARATIONS
class CMdEHarvesterSession;
class CHarvesterClientTestScripter;

// CLASS DECLARATION

/**
*  CHarvesterClientTestScripter test class for STIF Test Framework TestScripter.
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
NONSHARABLE_CLASS(CHarvesterClientTestScripter) : public CScriptBase,
                                                  public MMdESessionObserver,
                                                  public MHarvestObserver,
                                                  public MMdEQueryObserver,
                                                  public MMdEObjectObserver,
                                                  public MHarvesterEventObserver
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CHarvesterClientTestScripter* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~CHarvesterClientTestScripter();

    public: // Functions from base classes

        /**
        * From CScriptBase Runs a script line.
        * @since ?Series60_version
        * @param aItem Script line containing method name and parameters
        * @return Symbian OS error code
        */
        virtual TInt RunMethodL( CStifItemParser& aItem );

    private:  // Functions from base classes
		void HandleSessionOpened(CMdESession& aSession, TInt aError);
		void HandleSessionError(CMdESession& aSession, TInt aError);
		void HandleObjectNotification(CMdESession& aSession, 
	                                  TObserverNotificationType aType,
	                                  const RArray<TItemId>& aObjectIdArray);
	    void HandleQueryNewResults(CMdEQuery& aQuery,
	                               TInt aFirstNewItemIndex,
	                               TInt aNewItemCount);
	    void HandleQueryCompleted(CMdEQuery& aQuery, TInt aError);
		
        void HarvestingComplete( TDesC& aURI, TInt aError );
        void HarvestingUpdated( HarvesterEventObserverType aHEObserverType, 
        				HarvesterEventState aHarvesterEventState,
        				TInt aItemsLeft );

    private:

        /**
        * C++ default constructor.
        */
        CHarvesterClientTestScripter( CTestModuleIf& aTestModuleIf );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        /**
        * Frees all resources allocated from test methods.
        * @since ?Series60_version
        */
        void Delete();

		void CheckComplete();
        /**
        * Test helper methods are listed below. 
        */
        virtual TInt AddObjectObserverL( CStifItemParser& aItem );
        virtual TInt RemoveObjectObserverL( CStifItemParser& aItem );
        virtual TInt BeginHarvesterSessionL( CStifItemParser& aItem );
        virtual TInt EndHarvesterSessionL( CStifItemParser& aItem );

        /**
        * Test methods are listed below. 
        */
		virtual TInt SetUp( CStifItemParser& aItem );
		virtual TInt TearDown( CStifItemParser& aItem );
		virtual TInt HarvesterClientConnectL( CStifItemParser& aItem );
		virtual TInt HarvesterClientCloseL( CStifItemParser& aItem );
		virtual TInt HarvesterClientPauseL( CStifItemParser& aItem );
		virtual TInt HarvesterClientResumeL( CStifItemParser& aItem );
		virtual TInt SetHarvesterObserverL( CStifItemParser& aItem );
		virtual TInt RemoveHarvesterObserverL( CStifItemParser& aItem );
		virtual TInt SetLocationInfoL( CStifItemParser& aItem );
        virtual TInt HarvestFileL( CStifItemParser& aItem );
        virtual TInt HarvestFileWithUIDL( CStifItemParser& aItem );
        virtual TInt QueryImageObjectByIdIndexL( CStifItemParser& aItem );
        virtual TInt QueryAudioObjectByIdIndexL( CStifItemParser& aItem );
        virtual TInt RemoveObjectL( CStifItemParser& aItem );
        virtual TInt Results( CStifItemParser& aItem );
        virtual TInt AddHarvesterEventObserverL( CStifItemParser& aItem );
        virtual TInt RemoveHarvesterEventObserverL( CStifItemParser& aItem );
        virtual TInt SetUpBlacklistL( CStifItemParser& aItem );
        virtual TInt TearDownBlacklistL( CStifItemParser& aItem );
        virtual TInt AddFileToBlacklistL( CStifItemParser& aItem );
        virtual TInt CheckBlacklistL( CStifItemParser& aItem );
        virtual TInt RemoveFileFromBlacklistL( CStifItemParser& aItem );
        
    private:    // Data
        // Status booleans
		TBool iHarvestingComplete;
		TBool iPlaceholderComplete;
		TBool iNewObjectComplete;
		TBool iQuerySuccess;
		TBool iObjectNotification;
        TBool iSetLocation;

        // Test helper classes
		CMdESession* iMdeSession;
		CMdEHarvesterSession* iHarvesterSession;
		CMdEQuery* iQuery;

        RHarvesterClient iHc;
        RArray<TItemId> iAlbumIds;
        RArray<TItemId> iObjectIdArray;
        
        TFileName iUri;

        RBlacklistClient iBlacklistClient;
        CHarvesterMediaIdUtil* iMediaIdUtil;
        RFs iFs;
    };

#endif      // HARVESTERCLIENTTESTSCRIPTER_H

// End of File
