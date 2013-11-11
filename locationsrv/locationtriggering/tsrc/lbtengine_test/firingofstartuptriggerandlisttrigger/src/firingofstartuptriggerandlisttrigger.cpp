/*
* Copyright (c) 2002 - 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Test module to implement Firing of startup trigger and Listing of trigger related test cases
*
*/





// INCLUDE FILES
#include <Stiftestinterface.h>
#include <MProEngEngine.h>
#include "firingofstartuptriggerandlisttrigger.h"
#include <SettingServerClient.h>


// Log file
_LIT( KAdvancedTriggerSupervisionLogFile, "Firing.txt" ); 
_LIT( KAdvancedTriggerSupervisionLogFileWithTitle, "Firing[%S].txt" );


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

// ============================= LOCAL FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// ?function_name ?description.
// ?description
// Returns: ?value_1: ?description
//          ?value_n: ?description_line1
//                    ?description_line2
// -----------------------------------------------------------------------------
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

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CFiringofStartupTriggerAndListTrigger::CFiringofStartupTriggerAndListTrigger
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CFiringofStartupTriggerAndListTrigger::CFiringofStartupTriggerAndListTrigger( 
    CTestModuleIf& aTestModuleIf ):
        CScriptBase( aTestModuleIf ),
                iProEngine( NULL )
    {
    }

// -----------------------------------------------------------------------------
// CFiringofStartupTriggerAndListTrigger::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CFiringofStartupTriggerAndListTrigger::ConstructL()
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
        logFileName.Format(KAdvancedTriggerSupervisionLogFileWithTitle, &title);
        }
    else
        {
        logFileName.Copy(KAdvancedTriggerSupervisionLogFile);
        }

    iLog = CStifLogger::NewL( Kt_lbtltapiLogPath, 
                          Kt_lbtltapiLogFile,
                          CStifLogger::ETxt,
                          CStifLogger::EFile,
                          EFalse );
    
    }

// -----------------------------------------------------------------------------
// CFiringofStartupTriggerAndListTrigger::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CFiringofStartupTriggerAndListTrigger* CFiringofStartupTriggerAndListTrigger::NewL( 
    CTestModuleIf& aTestModuleIf )
    {
    CFiringofStartupTriggerAndListTrigger* self = new (ELeave) CFiringofStartupTriggerAndListTrigger( aTestModuleIf );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// Destructor
CFiringofStartupTriggerAndListTrigger::~CFiringofStartupTriggerAndListTrigger()
    { 

    // Delete resources allocated from test methods
    Delete();

    // Delete logger
    delete iLog; 
    delete iProEngine;
    iProEngine = NULL;
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================

// -----------------------------------------------------------------------------
// LibEntryL is a polymorphic Dll entry point.
// Returns: CScriptBase: New CScriptBase derived object
// -----------------------------------------------------------------------------
//
EXPORT_C CScriptBase* LibEntryL( 
    CTestModuleIf& aTestModuleIf ) // Backpointer to STIF Test Framework
    {

    return ( CScriptBase* ) CFiringofStartupTriggerAndListTrigger::NewL( aTestModuleIf );

    }

// -----------------------------------------------------------------------------
// E32Dll is a DLL entry point function.
// Returns: KErrNone
// -----------------------------------------------------------------------------
//
#ifndef EKA2 // Hide Dll entry point to EKA2
GLDEF_C TInt E32Dll(
    TDllReason /*aReason*/) 
    {
    return(KErrNone);

    }
#endif // EKA2

//  End of File
