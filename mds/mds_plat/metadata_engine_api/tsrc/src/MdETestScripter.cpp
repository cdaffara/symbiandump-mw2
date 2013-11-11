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
* Description:  
*
*/

// INCLUDE FILES
#include <StifTestInterface.h>
#include <SettingServerClient.h>
#include <pathinfo.h>
#include <mdesession.h>
#include "MdETestScripter.h"
#include "MdETestScripterAO.h"

// ============================= LOCAL FUNCTIONS ===============================

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMdETestScripter::CMdETestScripter
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMdETestScripter::CMdETestScripter( CTestModuleIf& aTestModuleIf ):
        CScriptBase( aTestModuleIf )
    {
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CMdETestScripter::ConstructL()
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
        logFileName.Format(KMdETestScripterLogFileWithTitle, &title);
        }
    else
        {
        logFileName.Copy(KMdETestScripterLogFile);
        }

#if ( defined (__WINS__) || defined (__WINSCW__) )
    iFilePath.Copy( PathInfo::PhoneMemoryRootPath().Left( KPathPrefixLength ));
#else
    iFilePath.Copy( PathInfo::MemoryCardRootPath ().Left( KPathPrefixLength ));
#endif
    iFilePath.Append( KMdETestDataPath );
	iFilePath.ZeroTerminate();

    iLog = CStifLogger::NewL( KMdETestScripterLogPath, 
                          logFileName,
                          CStifLogger::ETxt,
                          CStifLogger::EFile,
                          EFalse );
    iObjectDefNameArray = new (ELeave )CDesCArrayFlat( 8 );
    iRelationDefNameArray = new (ELeave )CDesCArrayFlat( 8 );
	iEventDefNameArray = new (ELeave )CDesCArrayFlat( 8 );
	iPropertyDefNameArray = new (ELeave )CDesCArrayFlat( 8 );
	iNamespaceDefNameArray = new (ELeave )CDesCArrayFlat( 8 );
	
	iObjectNameArray = new (ELeave )CDesCArrayFlat( 8 );
    iRelationNameArray = new (ELeave )CDesCArrayFlat( 8 );
    iEventNameArray = new (ELeave )CDesCArrayFlat( 8 );
    
    iPropertyNameArray = new (ELeave )CDesCArrayFlat( 8 );
    
	iRelationConditionNameArray = new (ELeave )CDesCArrayFlat( 8 );
	iObjectConditionNameArray = new (ELeave )CDesCArrayFlat( 8 );
	iEventConditionNameArray = new (ELeave )CDesCArrayFlat( 8 );
			
	iPropertyConditionNameArray = new (ELeave )CDesCArrayFlat( 8 );
	iLogicConditionNameArray = new (ELeave )CDesCArrayFlat( 8 );

	iMdeSession = CMdESession::NewL( *this );
	iAsyncHandler = CMdETestScripterAO::NewL();
    }

// -----------------------------------------------------------------------------
// CMdETestScripter::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CMdETestScripter* CMdETestScripter::NewL( 
    CTestModuleIf& aTestModuleIf )
    {
    CMdETestScripter* self = new (ELeave) CMdETestScripter( aTestModuleIf );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;
    }

// -----------------------------------------------------------------------------
// ~CMdETestScripter()
// Destructor.
// -----------------------------------------------------------------------------
//
CMdETestScripter::~CMdETestScripter()
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
    return ( CScriptBase* ) CMdETestScripter::NewL( aTestModuleIf );
    }

//  End of File
