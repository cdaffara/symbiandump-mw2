/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Test class for SAT Service API 
*
*/


#ifndef CSATSERVICEAPITEST_H
#define CSATSERVICEAPITEST_H

#include <testscripterinternal.h>
#include <rsatsession.h>
#include <rsatservice.h>

// Logging path
_LIT( KSatServiceAPILogPath, "\\logs\\testframework\\SATServiceAPITest\\" ); 
// Log file
_LIT( KSatServiceAPILogFile, "SATServiceAPITest.txt" ); 

class CSatServiceAPIMonitor;
class CStifLogger;
class CTestModuleIf;

/**
 *  Test class for SAT Service API / RSatService.h
 *
 *  @lib satserviceapitest.dll
 *  @since S60 5.0
 */
class CSATServiceAPI : public CScriptBase
    {
    public:  

        /**
         * Two-phased constructor.
         */
        static CSATServiceAPI* NewL( CTestModuleIf& aTestModuleIf );

        /**
         * Destructor.
         */
        virtual ~CSATServiceAPI();
        
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
         *
         * @param aTestModuleIf a test module ref.
         */
        CSATServiceAPI( CTestModuleIf& aTestModuleIf );

        /**
         * By default Symbian 2nd phase constructor is private.
         */
        void ConstructL();
        
        /**
         * Calls OpenL method from RSatService class
         *
         * @since S60 5.0
         * @param aItem, Script line containing method name and parameters
         * @return TInt, KErrNone if OK, other values if a failure happens
         */
        TInt OpenL( CStifItemParser& aItem );
        
        /**
         * Calls Close method from RSatService class
         *
         * @since S60 5.0
         * @param aItem, Script line containing method name and parameters
         * @return TInt, KErrNone if OK, other values if a failure happens
         */
        TInt Close( CStifItemParser& aItem );
        
        /**
         * Calls GetServiceInfoL method from RSatService class
         *
         * @since S60 5.0
         * @param aItem, Script line containing method name and parameters
         * @return TInt, KErrNone if OK, other values if a failure happens
         */
        TInt GetIconInfoL( CStifItemParser& aItem );
        
        /**
         * Calls GetServiceL method from RSatService class
         *
         * @since S60 5.0
         * @param aItem, Script line containing method name and parameters
         * @return TInt, KErrNone if OK, other values if a failure happens
         */
        TInt GetIconL( CStifItemParser& aItem );
        
        /**
         * Calls SetIdleModeTextResponse method from RSatService class
         *
         * @since S60 5.0
         * @param aItem, Script line containing method name and parameters
         * @return TInt, KErrNone if OK, other values if a failure happens
         */
        TInt SetIdleModeTextResponse( CStifItemParser& aItem );
      
        /**
         * Calls NotifySetupIdleModeTextChange method from RSatService class
         *
         * @since S60 5.1
         * @param aItem, Script line containing method name and parameters
         * @return TInt, KErrNone if OK, other values if a failure happens
         */
        TInt NotifySetupIdleModeTextChange( CStifItemParser& aItem );
                                
        /**
         * Calls NotifySetupIdleModeTextChangeCancel from RSatService class
         *
         * @since S60 5.1
         * @param aItem, Script line containing method name and parameters
         * @return TInt, KErrNone if OK, other values if a failure happens
         */
        TInt NotifySetupIdleModeTextChangeCancel( CStifItemParser& aItem );
        
        /**
         * Calls GetSetupIdleModeTextL method from RSatService class
         *
         * @since S60 5.1
         * @param aItem, Script line containing method name and parameters
         * @return TInt, KErrNone if OK, other values if a failure happens
         */
        TInt GetSetupIdleModeTextL( CStifItemParser& aItem );    
        
        /**
         * Publish setupidlemode text and icon to P&S keys.
         *
         * @since S60 5.1
         * @param aItem, Script line containing method name and parameters
         * @return TInt, KErrNone if OK, other values if a failure happens
         */
        TInt SetSetupIdleModeText( CStifItemParser& aItem );    
        
        /**
         * Check setupidlemode text and icon.
         *
         * @since S60 5.1
         * @param aItem, Script line containing method name and parameters
         * @return TInt, KErrNone if OK, other values if a failure happens
         */
        TInt CheckSetupIdleModeTextL( CStifItemParser& aItem );
        
    private:    // Data
   
        /**
         * Pointer to logger
         * Own.
         */
        CStifLogger* iLog;
        
        /**
         * Pointer to RSatService
         * Own.
         */
        RSatService* iSatService;
        
        /**
         * Object of RSatSession
         */
        RSatSession iSatSession;
        
        /**
         * Object of RSatSession
         * Own.
         */        
        CSatServiceAPIMonitor* iMonitor;
       
        /**
         * Setupidlemode text
         */        
        TPtrC iText;
        
        /**
         * Setupidlemode icon record id
         */        
        TUint8 iRecordId;
        
        /**
         * Setupidlemode icon qulifier
         */        
        RSatService::TSatIconQualifier iIconQualifier;
        
     };

#endif      // CSATSERVICEAPITEST_H
