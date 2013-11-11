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
* Description:  videoplaylistutility test class for STIF testing
*
*/

// Version : %version: 2 %


// INCLUDE FILES
#include <Stiftestinterface.h>
#include <ecom.h>
#include "videoplaylistutilitytest.h"
#include <SettingServerClient.h>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// Cvideoplaylistutility_test::Cvideoplaylistutility_test
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
Cvideoplaylistutility_test::Cvideoplaylistutility_test( CTestModuleIf& aTestModuleIf )
    : CScriptBase( aTestModuleIf )
{
}

// -----------------------------------------------------------------------------
// Cvideoplaylistutility_test::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void Cvideoplaylistutility_test::ConstructL()
{
    //Read logger settings to check whether test case name is to be
    //appended to log file name.
    RSettingServer settingServer;

    TInt ret = settingServer.Connect();

    if ( ret != KErrNone )
    {
        User::Leave(ret);
    }

    // Struct to StifLogger settigs.
    TLoggerSettings loggerSettings;

    // Parse StifLogger defaults from STIF initialization file.
    ret = settingServer.GetLoggerSettings(loggerSettings);

    if ( ret != KErrNone )
    {
        User::Leave(ret);
    }

    // Close Setting server session
    settingServer.Close();

    TFileName logFileName;

    if ( loggerSettings.iAddTestCaseTitle )
    {
        TName title;
        TestModuleIf().GetTestCaseTitleL(title);
        logFileName.Format(Kvideoplaylistutility_testLogFileWithTitle, &title);
    }
    else
    {
        logFileName.Copy(Kvideoplaylistutility_testLogFile);
    }

    iLog = CStifLogger::NewL( Kvideoplaylistutility_testLogPath,
                              logFileName,
                              CStifLogger::ETxt,
                              CStifLogger::EFile,
                              EFalse );

    SendTestClassVersion();
}

// -----------------------------------------------------------------------------
// Cvideoplaylistutility_test::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
Cvideoplaylistutility_test* Cvideoplaylistutility_test::NewL( CTestModuleIf& aTestModuleIf )
{
    Cvideoplaylistutility_test* self = new (ELeave) Cvideoplaylistutility_test( aTestModuleIf );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;
}

// Destructor
Cvideoplaylistutility_test::~Cvideoplaylistutility_test()
{
    // Delete resources allocated from test methods
    Delete();

    // Delete logger
    delete iLog;

    REComSession::FinalClose();
}

//-----------------------------------------------------------------------------
// Cvideoplaylistutility_test::SendTestClassVersion
// Method used to send version of test class
//-----------------------------------------------------------------------------
//
void Cvideoplaylistutility_test::SendTestClassVersion()
{
    TVersion moduleVersion;
    moduleVersion.iMajor = TEST_CLASS_VERSION_MAJOR;
    moduleVersion.iMinor = TEST_CLASS_VERSION_MINOR;
    moduleVersion.iBuild = TEST_CLASS_VERSION_BUILD;

    TFileName moduleName;
    moduleName = _L("videoplaylistutility_test.dll");

    TBool newVersionOfMethod = ETrue;
    TestModuleIf().SendTestModuleVersion(moduleVersion, moduleName, newVersionOfMethod);
}

// ========================== OTHER EXPORTED FUNCTIONS =========================

// -----------------------------------------------------------------------------
// LibEntryL is a polymorphic Dll entry point.
// Returns: CScriptBase: New CScriptBase derived object
// -----------------------------------------------------------------------------
//
EXPORT_C CScriptBase* LibEntryL( CTestModuleIf& aTestModuleIf )
{
    return ( CScriptBase* ) Cvideoplaylistutility_test::NewL( aTestModuleIf );
}

//  End of File
