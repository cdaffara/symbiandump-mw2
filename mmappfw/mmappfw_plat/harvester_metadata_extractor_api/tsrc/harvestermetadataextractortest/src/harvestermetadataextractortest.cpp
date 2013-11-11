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
* Description:  harvestermetadataextractortest implemetantion for STIF Test Framework TestScripter
*
*/



// INCLUDE FILES
#include <Stiftestinterface.h>
#include "harvestermetadataextractortest.h"
#include <SettingServerClient.h>


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// Charvestermetadataextractortest::Charvestermetadataextractortest
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
Charvestermetadataextractortest::Charvestermetadataextractortest( 
    CTestModuleIf& aTestModuleIf ):
        CScriptBase( aTestModuleIf )
    {
    iHarvester = NULL;
    iCollection = NULL;
    }

// -----------------------------------------------------------------------------
// Charvestermetadataextractortest::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void Charvestermetadataextractortest::ConstructL()
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
        logFileName.Format(KharvestermetadataextractortestLogFileWithTitle, &title);
        }
    else
        {
        logFileName.Copy(KharvestermetadataextractortestLogFile);
        }

    iLog = CStifLogger::NewL( KharvestermetadataextractortestLogPath, 
                          logFileName,
                          CStifLogger::ETxt,
                          CStifLogger::EFile,
                          EFalse );
    
    }

// -----------------------------------------------------------------------------
// Charvestermetadataextractortest::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
Charvestermetadataextractortest* Charvestermetadataextractortest::NewL( 
    CTestModuleIf& aTestModuleIf )
    {
    Charvestermetadataextractortest* self = new (ELeave) Charvestermetadataextractortest( aTestModuleIf );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;

    }

// Destructor
Charvestermetadataextractortest::~Charvestermetadataextractortest()
    { 

    // Delete resources allocated from test methods
    Delete();

    // Delete logger
    delete iLog; 
    if(NULL != iMetadataExtractor)
       	{
   	    delete iMetadataExtractor;
   	    iMetadataExtractor = NULL;
   	    }

   
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

    return ( CScriptBase* ) Charvestermetadataextractortest::NewL( aTestModuleIf );

    }


//  End of File
