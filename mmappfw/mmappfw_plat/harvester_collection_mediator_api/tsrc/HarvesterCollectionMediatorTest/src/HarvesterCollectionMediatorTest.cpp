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
* Description:  HarvesterCollectionMediatorTest.cpp
*
*/



// INCLUDE FILES
#include <Stiftestinterface.h>
#include "HarvesterCollectionMediatorTest.h"
#include <SettingServerClient.h>


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CHarvesterCollectionMediatorTest::CHarvesterCollectionMediatorTest
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CHarvesterCollectionMediatorTest::CHarvesterCollectionMediatorTest( 
    CTestModuleIf& aTestModuleIf ):
        CScriptBase( aTestModuleIf )
    {
    }

// -----------------------------------------------------------------------------
// CHarvesterCollectionMediatorTest::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CHarvesterCollectionMediatorTest::ConstructL()
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
          logFileName.Format(KHarvesterCollectionMediatorTestLogFileWithTitle, &title);
          }
      else
          {
          logFileName.Copy(KHarvesterCollectionMediatorTestLogFile);
          }

      iLog = CStifLogger::NewL( KHarvesterCollectionMediatorTestLogPath, 
                            logFileName,
                            CStifLogger::ETxt,
                            CStifLogger::EFile,
                            EFalse );
      
      SendTestClassVersion();
   	
    }

// -----------------------------------------------------------------------------
// CHarvesterCollectionMediatorTest::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CHarvesterCollectionMediatorTest* CHarvesterCollectionMediatorTest::NewL( 
    CTestModuleIf& aTestModuleIf )
    {
    CHarvesterCollectionMediatorTest* self = new (ELeave) CHarvesterCollectionMediatorTest( aTestModuleIf );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;

    }

// Destructor
CHarvesterCollectionMediatorTest::~CHarvesterCollectionMediatorTest()
    { 

    // Delete resources allocated from test methods
    Delete();

    // Delete logger
    delete iLog; 
   	
   	/*iCollectionUtility->Close();
   	iCollectionUtility = NULL;*/
    }


void CHarvesterCollectionMediatorTest::HandleMoveCompleteL( TInt /*aErr*/ )
    {
    iLog->Log( _L("HandleMoveCompleteL is called") );
    }


//-----------------------------------------------------------------------------
// CHarvesterCollectionMediatorTest::SendTestClassVersion
// Method used to send version of test class
//-----------------------------------------------------------------------------
//
void CHarvesterCollectionMediatorTest::SendTestClassVersion()
	{
	TVersion moduleVersion;
	moduleVersion.iMajor = TEST_CLASS_VERSION_MAJOR;
	moduleVersion.iMinor = TEST_CLASS_VERSION_MINOR;
	moduleVersion.iBuild = TEST_CLASS_VERSION_BUILD;
	
	TFileName moduleName;
	moduleName = _L("HarvesterCollectionMediatorTest.dll");

	TBool newVersionOfMethod = ETrue;
	TestModuleIf().SendTestModuleVersion(moduleVersion, moduleName, newVersionOfMethod);
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

    return ( CScriptBase* ) CHarvesterCollectionMediatorTest::NewL( aTestModuleIf );

    }


//  End of File
