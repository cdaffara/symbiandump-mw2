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



#ifndef HARVESTERDATATEST_H
#define HARVESTERDATATEST_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>

#include <harvesterdata.h>
#include <mdesession.h>
#include <mdccommon.h>

// Logging path
_LIT( KHarvesterDataTestLogPath, "\\logs\\testframework\\HarvesterDataTest\\" ); 
// Log file
_LIT( KHarvesterDataTestLogFile, "HarvesterDataTest.txt" ); 
_LIT( KHarvesterDataTestLogFileWithTitle, "HarvesterDataTest_[%S].txt" );

// FORWARD DECLARATIONS
class CHarvesterDataTest;

// CLASS DECLARATION
NONSHARABLE_CLASS(CHarvesterDataTest) : public CScriptBase,
										public MMdESessionObserver,
										public MHarvesterPluginObserver,
										public MMonitorPluginObserver
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CHarvesterDataTest* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~CHarvesterDataTest();

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
    	void MonitorEvent( CHarvesterData* aHarvesterData );
    	void MonitorEvent( RPointerArray<CHarvesterData>& aHarvesterDataArray );

    private:

        /**
        * C++ default constructor.
        */
        CHarvesterDataTest( CTestModuleIf& aTestModuleIf );

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

        virtual TInt ActiveWait( CStifItemParser& aItem );
        virtual TInt BeginTestSessionL( CStifItemParser& aItem );
        virtual TInt EndTestSession( CStifItemParser& aItem );
        virtual TInt HarvestClientDataTestsL( CStifItemParser& aItem );
        virtual TInt PlaceholderDataTestsL( CStifItemParser& aItem );
        virtual TInt HarvesterDataTestsL( CStifItemParser& aItem );
        
        //[TestMethods] - Do not remove

    private:    // Data
        
        CMdESession* iMdEClient;
    	
    	RArray<TItemId> iAlbumIds;
    };

#endif      // HARVESTERDATATEST_H

// End of File
