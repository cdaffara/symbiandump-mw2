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
* Description:  Test module for mmmtpdataprovider plugin
*
*/


// INCLUDE FILES

#include <stiftestinterface.h>
#include <settingserverclient.h>
#include <mtp/mmtpdataprovider.h>
#include <mtp/cmtpdataproviderplugin.h>
#include "cmmmtpdataprovidertestmodule.h"
#include "logging.h"

// MACROS

#ifdef LOGGER
#undef LOGGER
#endif
#define LOGGER iLog

// MEMBER FUNCTIONS

CMmMtpDataProviderTestModule::CMmMtpDataProviderTestModule( 
        CTestModuleIf& aTestModuleIf )
        : CScriptBase( aTestModuleIf )
    {
    // no implementation required
    }

void CMmMtpDataProviderTestModule::ConstructL()
    {
    TFileName logFileName;  
    logFileName.Copy(KMtpDataProviderTestModuleLogFile);
    
    iLog = CStifLogger::NewL( KMtpDataProviderTestModuleLogPath, 
            logFileName,
            CStifLogger::ETxt,
            TRACE_TARGET,
            ETrue, ETrue, ETrue, EFalse, ETrue );
    PRINTF0( ">CMmMtpDataProviderTestModule::ConstructL" );
    
    SendTestClassVersion();
    
    if ( CActiveScheduler::Current() == NULL ) 
        {
        iActiveScheduler = new ( ELeave ) CActiveScheduler;
        CActiveScheduler::Install( iActiveScheduler );        
        }
    iTestObserver = CTestObserver::NewL( this );
    iTestConnChannel = new ( ELeave ) CTestConnectionChannel();
    iRequest = new ( ELeave ) TMTPTypeRequest();
    
    PRINTF0( "<CMmMtpDataProviderTestModule::ConstructL" );
    }

CMmMtpDataProviderTestModule* CMmMtpDataProviderTestModule::NewL( 
        CTestModuleIf& aTestModuleIf )
    {
    CMmMtpDataProviderTestModule* self = 
            new ( ELeave ) CMmMtpDataProviderTestModule( aTestModuleIf );
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    
    return self;
    }

CMmMtpDataProviderTestModule::~CMmMtpDataProviderTestModule()
    { 
    PRINTF0( ">CMmMtpDataProviderTestModule::~CMmMtpDataProviderTestModule" );
    
    // delete resources allocated from test methods
    Delete();
    
    if ( iTestObserver ) 
        {
        delete iTestObserver;
        }
     if ( iTestConnChannel )
        {
        delete iTestConnChannel;
        }
     if ( iActiveScheduler )
        {
        delete iActiveScheduler;
        }
     
     PRINTF0( "<CMmMtpDataProviderTestModule::~CMmMtpDataProviderTestModule" );
     if ( iLog )
         {
         delete iLog;
         }
    }

void CMmMtpDataProviderTestModule::SendTestClassVersion()
    {
    PRINTF0( ">CMmMtpDataProviderTestModule::SendTestClassVersion" );
    TVersion moduleVersion;
    moduleVersion.iMajor = TEST_CLASS_VERSION_MAJOR;
    moduleVersion.iMinor = TEST_CLASS_VERSION_MINOR;
    moduleVersion.iBuild = TEST_CLASS_VERSION_BUILD;
    
    _LIT( KModuleName, "mmmtpdataprovidertester.dll" );
    TFileName moduleName( KModuleName );
    
    TBool newVersionOfMethod = ETrue;
    TestModuleIf().SendTestModuleVersion( moduleVersion, moduleName,
            newVersionOfMethod );
    PRINTF0( "<CMmMtpDataProviderTestModule::SendTestClassVersion" );
    }

// EXPORTED FUNCTIONS

// polymorphic library entry point
EXPORT_C CScriptBase* LibEntryL( CTestModuleIf& aTestModuleIf )
    {
    return ( CScriptBase* ) CMmMtpDataProviderTestModule::NewL( aTestModuleIf );
    }
