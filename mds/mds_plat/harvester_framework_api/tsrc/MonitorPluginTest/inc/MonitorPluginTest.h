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
* Description:  
*
* ============================================================================
*/


#ifndef MONITORPLUGINTEST_H
#define MONITORPLUGINTEST_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>

//#include <f32file.h>
#include <monitorplugin.h>
#include "mdeharvestersession.h"
#include <harvesterdata.h>

// CONSTANTS
// Logging path
_LIT( KMonitorPluginTestLogPath, "\\logs\\testframework\\MonitorPluginTest\\" ); 
// Log file
_LIT( KMonitorPluginTestLogFile, "MonitorPluginTest.txt" ); 
_LIT( KMonitorPluginTestLogFileWithTitle, "MonitorPluginTest_[%S].txt" );


// FORWARD DECLARATIONS
class CMonitorPluginTest;

// CLASS DECLARATION
NONSHARABLE_CLASS(CMonitorPluginTest) : public CScriptBase,
										public MMonitorPluginObserver,
										public MMdESessionObserver
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CMonitorPluginTest* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~CMonitorPluginTest();

    public: // Functions from base classes

        /**
        * From CScriptBase Runs a script line.
        * @since ?Series60_version
        * @param aItem Script line containing method name and parameters
        * @return Symbian OS error code
        */
        virtual TInt RunMethodL( CStifItemParser& aItem );
        
    private: // Functions from base classes        
        
        void MonitorEvent( CHarvesterData* aHarvesterData );
        void MonitorEvent( RPointerArray<CHarvesterData>& aHDArray );
        
    	void HandleSessionOpened(CMdESession& aClient, TInt aError);
    	void HandleSessionError(CMdESession& aClient, TInt aError);

    private:

        /**
        * C++ default constructor.
        */
        CMonitorPluginTest( CTestModuleIf& aTestModuleIf );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        /**
        * Frees all resources allocated from test methods.
        * @since ?Series60_version
        */
        void Delete();
        
        void ActiveWait2();

        /**
        * Test methods are listed below. 
        */

        virtual TInt ActiveWait( CStifItemParser& aItem );
        virtual TInt BeginMonitorSessionL( CStifItemParser& aItem );
        virtual TInt EndMonitorSession( CStifItemParser& aItem );
        virtual TInt LoadPluginsL( CStifItemParser& aItem );
        virtual TInt StartMonitoring( CStifItemParser& aItem );
        virtual TInt PauseMonitoring( CStifItemParser& aItem );
        virtual TInt ResumeMonitoring( CStifItemParser& aItem );
        virtual TInt StopMonitoring( CStifItemParser& aItem );
        virtual TInt CreateEventsL( CStifItemParser& aItem );
        virtual TInt CreateEventsDeleteL( CStifItemParser& aItem );

        //[TestMethods] - Do not remove

    private:    // Data
        
        RPointerArray<CMonitorPlugin> iPluginArray;
        CMdESession* iMdEClient;
    };

#endif      // MONITORPLUGINTEST_H

// End of File
