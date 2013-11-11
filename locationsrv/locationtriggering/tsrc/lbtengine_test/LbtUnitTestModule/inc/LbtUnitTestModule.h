/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: STIF testclass declaration
*
*/

#ifndef LBTUNITTESTMODULE_H
#define LBTUNITTESTMODULE_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include <TestclassAssert.h>
#include "lbtdiskspaceobserver.h"

// Logging path
_LIT( KLbtUnitTestModuleLogPath, "\\logs\\testframework\\LbtUnitTestModule\\" ); 
// Log file
_LIT( KLbtUnitTestModuleLogFile, "LbtUnitTestModule.txt" ); 
_LIT( KLbtUnitTestModuleLogFileWithTitle, "LbtUnitTestModule_[%S].txt" );

class CLbtUnitTestModule;
class MLbtDiskSpaceObserver;


/**
 *  CLbtUnitTestModule test class for STIF Test Framework TestScripter.
 */
NONSHARABLE_CLASS(CLbtUnitTestModule) : public CScriptBase,
                                        public MLbtDiskSpaceObserver
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CLbtUnitTestModule* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~CLbtUnitTestModule();

    public: // Functions from base classes

        /**
        * From CScriptBase Runs a script line.
        * @since ?Series60_version
        * @param aItem Script line containing method name and parameters
        * @return Symbian OS error code
        */
        virtual TInt RunMethodL( CStifItemParser& aItem );
    
    public:
        virtual void HandleDiskSpaceChange();

    private:

        /**
        * C++ default constructor.
        */
        CLbtUnitTestModule( CTestModuleIf& aTestModuleIf );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        /**
        * Frees all resources allocated from test methods.
        */
        void Delete();

        /**
        * Test methods are listed below. 
        */
        TInt TestCLbtGeoGsmCellNewL( CStifItemParser& aItem );
        
        TInt TestCLbtGeoGsmCellNewLC( CStifItemParser& aItem );
        
        TInt TestCLbtGeoGsmCellNew1L( CStifItemParser& aItem );
        
        TInt TestCLbtGeoGsmCellNew1LC( CStifItemParser& aItem );
        
        TInt TestSetAndGetBsicL( CStifItemParser& aItem );
        
        TInt TestGsmCGISpecifiedDuringCreationL( CStifItemParser& aItem );
        
        TInt TestGsmCellInternalizeExternalizeL( CStifItemParser& aItem );
        
        TInt TestCLbtGeoWcdmaCellL( CStifItemParser& aItem );
        
        TInt TestCLbtGeoWcdmaCellLC( CStifItemParser& aItem );
        
        TInt TestCLbtGeoWcdmaCell1L( CStifItemParser& aItem );
        
        TInt TestCLbtGeoWcdmaCell1LC( CStifItemParser& aItem );
        
        TInt TestSetAndGetWcdmaInfoL( CStifItemParser& aItem );
        
        TInt TestWcdmaCGISpecifiedDuringCreationL( CStifItemParser& aItem );
        
        TInt TestWcdmaCellInternalizeExternalizeL( CStifItemParser& aItem );
        
        TInt TestSetAndGetOptimisationLevelL( CStifItemParser& aItem );
        
        TInt TestSetAndGetModuleIdL( CStifItemParser& aItem );
        
        TInt TestSetAndGetNppModuleIdL( CStifItemParser& aItem );
        
        TInt TestSetAndGetMinUpdateIntOnGpsFailureL( CStifItemParser& aItem );
        
        TInt TestSetAndGetMaximumUserSpeedL( CStifItemParser& aItem  );
        
        TInt TestSetAndGetTrigMechStateL( CStifItemParser& aItem  );
        
        TInt TestTriggerConfigParamsL( CStifItemParser& aItem );
        
        TInt TestCLbtDiskSpaceListnerStartToListenL( CStifItemParser& aItem );
        
        TInt TestCLbtDiskSpaceListnerCancelL( CStifItemParser& aItem );
        
        TInt TestTriggerNodeMethodsL( CStifItemParser& aItem );
        
        TInt TestGsmCellInfoMethodsL( CStifItemParser& aItem );
        
        TInt TestWcdmaCellInfoMethodsL( CStifItemParser& aItem );
        
        TInt TestLbtOperationsMethodL( CStifItemParser& aItem );

private:
    CActiveSchedulerWait* iWait;
        
    };

#endif      // LBTUNITTESTMODULE_H

// End of File
