/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Test class for SAT Refresh API
*
*/


#ifndef CSATREFRESHAPITEST_H
#define CSATREFRESHAPITEST_H

#include <stiflogger.h>
#include <testscripterinternal.h>
#include <stiftestmodule.h>
#include "satrefreshobserver.h"
// API under testing:
#include <rsatrefresh.h>
#include <msatrefreshobserver.h>
// This is needed for Refresh API
#include <rsatsession.h>

// Logging path
_LIT( KSatRefreshAPILogPath, "\\logs\\testframework\\SATRefreshAPITest\\" ); 
// Log file
_LIT( KSatRefreshAPILogFile, "SATRefreshAPITest.txt" ); 


/**
*  Test class for SAT Refresh API 
*
*  @lib satrefreshapitest.dll
*  @since S60 5.0
*/
class CSatRefreshAPI : public CScriptBase
    {
    public:  

        /**
        * Two-phased constructor.
        */
        static CSatRefreshAPI* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~CSatRefreshAPI();
        
         /**
         * From CScriptBase Runs a script line.
         *          
         * @since S60 5.0
         * @param aItem Script line containing method name and parameters
         * @return Symbian OS error code
         */
        TInt RunMethodL( CStifItemParser& aItem ); 

    private:    // Methods

        /**
        * C++ default constructor.
        */
        CSatRefreshAPI( CTestModuleIf& aTestModuleIf );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();
        
        /**
        * Calls OpenL method from RSatRefresh class
        *
        * @since S60 5.0
        * @param aItem, Script line containing method name and parameters
        * @return TInt, KErrNone if OK, other values if a failure happens
        */
        TInt OpenL( CStifItemParser& aItem );
        
        /**
        * Calls Close method from RSatRefresh class
        *
        * @since S60 5.0
        * @param aItem, Script line containing method name and parameters
        * @return TInt, KErrNone if OK, other values if a failure happens
        */
        TInt Close( CStifItemParser& aItem );
        
        /**
        * Calls Cancel method from RSatRefresh class
        *
        * @since S60 5.0
        * @param aItem, Script line containing method name and parameters
        * @return TInt, KErrNone if OK, other values if a failure happens
        */
        TInt Cancel(CStifItemParser& aItem );
        
        /**
        * Calls NotifyFileChangeL method from RSatRefresh class
        *
        * @since S60 5.0
        * @param aItem, Script line containing method name and parameters
        * @return TInt, KErrNone if OK, other values if a failure happens
        */
        TInt NotifyFileChangeL( CStifItemParser& aItem );
        
        /**
        * Calls RefreshEFRead method from RSatRefresh class
        *
        * @since S60 5.0
        * @param aItem, Script line containing method name and parameters
        * @return TInt, KErrNone if OK, other values if a failure happens
        */
        TInt RefreshEFRead( CStifItemParser& aItem );
        
    private:    // Data
   
        /**
        * Pointer to logger
        * Own.
        */
        CStifLogger* iLog;
        
        /**
        * Pointer to Observer
        * Own.
        */
        CSatRefreshObserver* iObserver;
        
        /**
        * Object of RSatSession
        */
        RSatSession iSatSession;
        
        /**
        * Pointer to RSatRefresh
        * Own
        */
        RSatRefresh* iSatRefresh;
     };

#endif      // CSATREFRESHAPITEST_H
