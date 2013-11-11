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
* Description:  Test class for SAT Client API / rsatsession.h
*
*/

#ifndef CSATCLIENTAPITEST_H
#define CSATCLIENTAPITEST_H

#include <stiflogger.h>
#include <testscripterinternal.h>
#include <stiftestmodule.h>
// API under testing:
#include <rsatsession.h>

// Logging path
_LIT( KSatClientAPILogPath, "\\logs\\testframework\\SATClientAPITest\\" ); 
// Log file
_LIT( KSatClientAPILogFile, "SATClientAPITest.txt" ); 


/**
*  Test class for SAT Client API / rsatsession.h
*
*  @lib satclientapitest.dll
*  @since S60 5.0
*/
class CSATClientAPI : public CScriptBase
    {
    public:  

        /**
        * Two-phased constructor.
        */
        static CSATClientAPI* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~CSATClientAPI();
        
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
        CSATClientAPI( CTestModuleIf& aTestModuleIf );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();
        
        /**
        * Calls ConnectL method from RSatSession class
        *
        * @since S60 5.0
        * @param aItem, Script line containing method name and parameters
        * @return TInt, KErrNone if OK, other values if a failure happens
        */
        TInt Connect( CStifItemParser& aItem );
        
        /**
        * Calls Close method from RSatSession class
        *
        * @since S60 5.0
        * @param aItem, Script line containing method name and parameters
        * @return TInt, KErrNone if OK, other values if a failure happens
        */
        TInt Close( CStifItemParser& aItem );
        
        /**
        * Calls Version method from RSatSession class
        *
        * @since S60 5.0
        * @param aItem, Script line containing method name and parameters
        * @return TInt, KErrNone if OK, other values if a failure happens
        */
        TInt Version(CStifItemParser& aItem );

    private:    // Data
   
        /**
        * Pointer to logger
        * Own.
        */
        CStifLogger * iLog;
        
        /**
        * Object of RSatSession
        */
        RSatSession iSatSession;
     };

#endif      // CSATCLIENTAPITEST_H
