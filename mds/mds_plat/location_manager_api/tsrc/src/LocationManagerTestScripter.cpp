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
* Description:  ?Description
*
*/


// INCLUDE FILES
#include <StifTestInterface.h>
#include "LocationManagerTestScripter.h"
#include <SettingServerClient.h>


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CLocationManagerTestScripter::CLocationManagerTestScripter
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CLocationManagerTestScripter::CLocationManagerTestScripter( 
    CTestModuleIf& aTestModuleIf ):
        CScriptBase( aTestModuleIf )
    {
    }

// -----------------------------------------------------------------------------
// CLocationManagerTestScripter::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CLocationManagerTestScripter::ConstructL()
    {
    	
#ifdef LOC_REVERSEGEOCODE
     iLocGeoTaggerConnected = EFalse;
#endif //LOC_REVERSEGEOCODE
	 
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
        logFileName.Format(KLocationManagerTestScripterLogFileWithTitle, &title);
        }
    else
        {
        logFileName.Copy(KLocationManagerTestScripterLogFile);
        }

    iLog = CStifLogger::NewL( KLocationManagerTestScripterLogPath, 
                          logFileName,
                          CStifLogger::ETxt,
                          CStifLogger::EFile,
                          EFalse );

    }

// -----------------------------------------------------------------------------
// CLocationManagerTestScripter::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CLocationManagerTestScripter* CLocationManagerTestScripter::NewL( 
    CTestModuleIf& aTestModuleIf )
    {
    CLocationManagerTestScripter* self = new (ELeave) CLocationManagerTestScripter( aTestModuleIf );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;

    }

// Destructor
CLocationManagerTestScripter::~CLocationManagerTestScripter()
    { 

    // Delete resources allocated from test methods
    Delete();

    // Delete logger
    delete iLog; 

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

    return ( CScriptBase* ) CLocationManagerTestScripter::NewL( aTestModuleIf );

    }


//  End of File
