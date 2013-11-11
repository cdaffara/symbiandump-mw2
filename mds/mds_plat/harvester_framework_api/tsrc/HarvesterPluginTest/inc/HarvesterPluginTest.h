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


#ifndef HARVESTERPLUGINTEST_H
#define HARVESTERPLUGINTEST_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>

#include "contextengine.h"
#include <mdccommon.h>
#include <harvesterdata.h>
#include "mdeharvestersession.h"
#include <harvesterplugin.h>
#include "harvesterplugininfo.h"

// CONSTANTS

_LIT( KBlacklistDatabaseName, "C:[200009f5]blacklistdb.sq" );

// Logging path
_LIT( KHarvesterPluginTestLogPath, "\\logs\\testframework\\HarvesterPluginTest\\" ); 
// Log file
_LIT( KHarvesterPluginTestLogFile, "HarvesterPluginTest.txt" ); 
_LIT( KHarvesterPluginTestLogFileWithTitle, "HarvesterPluginTest_[%S].txt" );

// FORWARD DECLARATIONS
class CHarvesterPluginTest;
class CHarvesterBlacklist;
class CMdeObjectHandler;

// CLASS DECLARATION
NONSHARABLE_CLASS(CHarvesterPluginTest) : public CScriptBase,
										  MMdESessionObserver,
										  MHarvesterPluginObserver
									
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CHarvesterPluginTest* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~CHarvesterPluginTest();

    public: // Functions from base classes

        /**
        * From CScriptBase Runs a script line.
        * @since ?Series60_version
        * @param aItem Script line containing method name and parameters
        * @return Symbian OS error code
        */
        virtual TInt RunMethodL( CStifItemParser& aItem );

    private: // Functions from base classes
        	
        void HandleSessionOpened(CMdESession& aSession, TInt aError);
        void HandleSessionError(CMdESession& aSession, TInt aError);      
        void HarvestingCompleted( CHarvesterData* aHarvesterData );

    private:

        /**
        * C++ default constructor.
        */
        CHarvesterPluginTest( CTestModuleIf& aTestModuleIf );

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

        virtual TInt BeginTestSessionL( CStifItemParser& aItem );
        virtual TInt EndTestSession( CStifItemParser& aItem );
        virtual TInt LoadPluginsL( CStifItemParser& aItem );
        virtual TInt TestFunctionsL( CStifItemParser& aItem );

        // helper methods
        TBool GetObjectDef(const TDesC& aUri, TDes& aObjectDef, TBool aCheck);
        TInt HarvestL( CHarvesterData* aHD );
        void AddNewPluginL( const TDesC8& aType, const TDesC8& aOpaque, TUid aPluginUid );
        CHarvesterPluginInfo* GetHPIFromFileNameL(const TDesC& aFileName);
        CHarvesterPluginInfo* GetFromNormalFileL( TDesC& aExt );
        
    public:     // Data
        // ?one_line_short_description_of_data
        //?data_declaration;

    protected:  // Data
        // ?one_line_short_description_of_data
        //?data_declaration;

    private:    // Data
        
    	CMdESession* iMdeSession;
    	CMdEHarvesterSession* iSession;
    	
    	CContextEngine* iCtxEngine;
    	CHarvesterBlacklist* iBlacklist;
    	CMdeObjectHandler* iMdeObjectHandler;
    	
    	CHarvesterPlugin* iPlugin;
    	RPointerArray<CHarvesterPluginInfo> iPluginInfoArray;

    };

#endif      // HARVESTERPLUGINTEST_H

// End of File
