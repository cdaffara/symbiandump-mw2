/*
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: 
*
*/


#ifndef CONTEXTENGINEPLUGINTEST_H
#define CONTEXTENGINEPLUGINTEST_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>

#include <mdesession.h>
#include <contextengine.h>
#include <contextplugin.h>

// Logging path
_LIT( KContextEnginePluginTestLogPath, "\\logs\\testframework\\ContextEnginePluginTest\\" ); 
// Log file
_LIT( KContextEnginePluginTestLogFile, "ContextEnginePluginTest.txt" ); 
_LIT( KContextEnginePluginTestLogFileWithTitle, "ContextEnginePluginTest_[%S].txt" );

// FORWARD DECLARATIONS
class CContextEnginePluginTest;

// CLASS DECLARATION
NONSHARABLE_CLASS(CContextEnginePluginTest) : public CScriptBase,
											  public MMdESessionObserver,
				                              public MContextInitializationObserver,
				                              public MContextSnapshotObserver,
				                              public MContextPluginObserver
				                              
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CContextEnginePluginTest* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~CContextEnginePluginTest();

    public: // Functions from base classes

        virtual TInt RunMethodL( CStifItemParser& aItem );

    private: // Functions from base classes
    	
    	void HandleSessionOpened(CMdESession& aSession, TInt aError);
    	void HandleSessionError(CMdESession& aSession, TInt aError);
        void ContextInitializationStatus( TInt aError );
        void ContextSnapshotStatus( CHarvesterData* aHD );        
        void PluginInitializationStatus( TInt aError );
        void PluginSnapshotStatus( CHarvesterData* aHD );

    private:

        /**
        * C++ default constructor.
        */
        CContextEnginePluginTest( CTestModuleIf& aTestModuleIf );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        /**
        * Frees all resources allocated from test methods.
        */
        void Delete();

        void ActiveWait( TInt aTimeout );
        
        /**
        * Test methods are listed below. 
        */

        // engine
        virtual TInt BeginTestSessionL( CStifItemParser& aItem );
        virtual TInt EndTestSession( CStifItemParser& aItem );
        virtual TInt CreateObjectsL( CStifItemParser& aItem );
        virtual TInt DeleteObjectsL( CStifItemParser& aItem );
        virtual TInt CreateContextEngine1L( CStifItemParser& aItem );
        virtual TInt CreateContextEngine2L( CStifItemParser& aItem );
        virtual TInt ReleaseInstanceL( CStifItemParser& aItem );
        virtual TInt SetMdeSessionL( CStifItemParser& aItem );
        virtual TInt ContextSnapshotsL( CStifItemParser& aItem );
        virtual TInt MultipleSnapshotsL( CStifItemParser& aItem );
        virtual TInt PluginCount( CStifItemParser& aItem );
        virtual TInt Destroy( CStifItemParser& aItem );
        virtual TInt ContextEngineStaticData( CStifItemParser& aItem );
        
        // plugin
        virtual TInt LoadPluginL( CStifItemParser& aItem );

    private:    // Data
        
        CMdESession* iMdEClient;
        CContextEngine* iContextEngine;
        CHarvesterData* iHD;
        
        RPointerArray<CHarvesterData> iObjectArray;
        RArray<TInt> iIds;
        
        CContextPlugin* iPlugin;
        CContextPlugin* iLocationPlugin;
        CContextPlugin* iCalendarPlugin;

        TInt iErrorCode;
        
        TInt iInitCount;
        TInt iSnapshotCount;
        TInt iInitTarget;
        TInt iSnapshotTarget;
        
        TBool iPluginSet;

    };

#endif      // CONTEXTENGINEPLUGINTEST_H

// End of File
