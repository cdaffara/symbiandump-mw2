/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Description
*
*/

// Version : %version: 4 %




// INCLUDE FILES
#include <Stiftestinterface.h>
#include <SettingServerClient.h>
#include <mediarecognizer.h>
#include <streaminglinkmodel.h>
#include <devsoundif.h>

#include "playbackhelpertest.h"

// EXTERNAL DATA STRUCTURES
//extern  ?external_data;

// EXTERNAL FUNCTION PROTOTYPES  
//extern ?external_function( ?arg_type,?arg_type );

// CONSTANTS
//const ?type ?constant_var = ?constant;

// MACROS
//#define ?macro ?macro_def

// LOCAL CONSTANTS AND MACROS
//const ?type ?constant_var = ?constant;
//#define ?macro_name ?macro_def

// MODULE DATA STRUCTURES
//enum ?declaration
//typedef ?declaration

// LOCAL FUNCTION PROTOTYPES
//?type ?function_name( ?arg_type, ?arg_type );

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;

// ============================= LOCAL FUNCTIONS ===================================================

// -------------------------------------------------------------------------------------------------
// ?function_name ?description.
// ?description
// Returns: ?value_1: ?description
//          ?value_n: ?description_line1
//                    ?description_line2
// -------------------------------------------------------------------------------------------------
//
/*
?type ?function_name(
    ?arg_type arg,  // ?description
    ?arg_type arg)  // ?description
    {

    ?code  // ?comment

    // ?comment
    ?code
    }
*/

// ============================ MEMBER FUNCTIONS ===================================================

// -------------------------------------------------------------------------------------------------
// CPlaybackHelperTest::CPlaybackHelperTest
// C++ default constructor can NOT contain any code, that
// might leave.
// -------------------------------------------------------------------------------------------------
//
CPlaybackHelperTest::CPlaybackHelperTest( CTestModuleIf& aTestModuleIf )
: CScriptBase( aTestModuleIf )  
{
//      iFileMan = NULL;
}

// -------------------------------------------------------------------------------------------------
// CPlaybackHelperTest::ConstructL
// Symbian 2nd phase constructor can leave.
// -------------------------------------------------------------------------------------------------
//
void CPlaybackHelperTest::ConstructL()
{
    //Read logger settings to check whether test case name is to be
    //appended to log file name.
    RSettingServer settingServer;
    TInt ret = settingServer.Connect();
    if(ret != KErrNone)
    {
        User::Leave(ret);
    }
    // Struct to StifLogger settigs.
    TLoggerSettings loggerSettings; 
    // Parse StifLogger defaults from STIF initialization file.
    ret = settingServer.GetLoggerSettings(loggerSettings);
    if(ret != KErrNone)
    {
        User::Leave(ret);
    } 
    // Close Setting server session
    settingServer.Close();

    TFileName logFileName;
    
    if(loggerSettings.iAddTestCaseTitle)
    {
        TName title;
        TestModuleIf().GetTestCaseTitleL(title);
        logFileName.Format(KPlaybackHelperTestLogFileWithTitle, &title);
    }
    else
    {
        logFileName.Copy(KPlaybackHelperTestLogFile);
    }

    iLog = CStifLogger::NewL( KPlaybackHelperTestLogPath, 
                              logFileName,
                              CStifLogger::ETxt,
                              CStifLogger::EFile,
                              EFalse );

    iMediaRecognizer = CMediaRecognizer::NewL();
    iStreamingLinkModel = CStreamingLinkModel::NewL();
	iDevSoundIf = CDevSoundIf::NewL();

    SendTestClassVersion();
}

// -------------------------------------------------------------------------------------------------
// CPlaybackHelperTest::NewL
// Two-phased constructor.
// -------------------------------------------------------------------------------------------------
//
CPlaybackHelperTest* CPlaybackHelperTest::NewL( CTestModuleIf& aTestModuleIf )
{
    CPlaybackHelperTest* self = new (ELeave) CPlaybackHelperTest( aTestModuleIf );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;

}

// Destructor
CPlaybackHelperTest::~CPlaybackHelperTest()
{ 
    if ( iMediaRecognizer )
    {
        delete iMediaRecognizer;
        iMediaRecognizer = NULL;
    }

    if ( iStreamingLinkModel )
    {
        delete iStreamingLinkModel;
        iStreamingLinkModel = NULL;
    }

    if ( iDevSoundIf )
    {
        delete iDevSoundIf;
        iDevSoundIf = NULL;
    }

    REComSession::FinalClose();

    // Delete resources allocated from test methods
    Delete();

    // Delete logger
    if ( iLog )
    {
        delete iLog;
        iLog = NULL;
    }
}

//--------------------------------------------------------------------------------------------------
// CPlaybackHelperTest::SendTestClassVersion
// Method used to send version of test class
//--------------------------------------------------------------------------------------------------
//
void CPlaybackHelperTest::SendTestClassVersion()
{
	TVersion moduleVersion;
	moduleVersion.iMajor = TEST_CLASS_VERSION_MAJOR;
	moduleVersion.iMinor = TEST_CLASS_VERSION_MINOR;
	moduleVersion.iBuild = TEST_CLASS_VERSION_BUILD;
	
	TFileName moduleName;
	moduleName = _L("playbackhelpertest.dll");

	TBool newVersionOfMethod = ETrue;
	TestModuleIf().SendTestModuleVersion(moduleVersion, moduleName, newVersionOfMethod);
}

// ========================== OTHER EXPORTED FUNCTIONS =============================================

// -------------------------------------------------------------------------------------------------
// LibEntryL is a polymorphic Dll entry point.
// Returns: CScriptBase: New CScriptBase derived object
// -------------------------------------------------------------------------------------------------
//
EXPORT_C CScriptBase* LibEntryL( CTestModuleIf& aTestModuleIf )
{
    return ( CScriptBase* ) CPlaybackHelperTest::NewL( aTestModuleIf );
}

//  End of File
