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
* Description:    This file contains testclass implementation.
* This file defines the test class for testing the encoder functionality.
*
*/




// INCLUDE FILES
#include <Stiftestinterface.h>
#include <SettingServerClient.h>
#include "CUTLandmarkURL.h"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CCUTLandmarkURL::CCUTLandmarkURL
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CCUTLandmarkURL::CCUTLandmarkURL( 
    CTestModuleIf& aTestModuleIf ):
        CScriptBase( aTestModuleIf ),
        iCount(0),
        iVerticalAccuracy(_L("")),
        iHorizontalAccuracy(_L("")),
        iName(_L("")),
        iDescription(_L("")),
        iLatitude(_L("")),
        iLongitude(_L("")),
        iAltitude(_L("")),
        iCategory(_L("")),
        iMatchCategory(_L("")),
        iMatchCategory2(_L("")),
        iTS(_L("")),
        iPID(_L("")),
        iYear(_L("")),
        iMonth(_L("")),
        iDay(_L("")),
        iHour(_L("")),
        iMinutes(_L("")),
        iSeconds(_L("")),
        iMSeconds(_L("")),
        iSpeed(_L("")),
        iTelephone(_L("")),
        iWeb(_L("")),
        iHeading(_L("")),
        iCountry(_L("")),
        iState(_L("")),
        iCity(_L("")),
        iStreet(_L("")),
        iHouseNumber(_L("")),
        iPostalCode(_L(""))
    {
    
    }

// -----------------------------------------------------------------------------
// CCUTLandmarkURL::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCUTLandmarkURL::ConstructL()
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
        logFileName.Format(KCUTLandmarkURLLogFileWithTitle, &title);
        }
    else
        {
        logFileName.Copy(KCUTLandmarkURLLogFile);
        }

    iLog = CStifLogger::NewL( KCUTLandmarkURLLogPath, 
                          logFileName,
                          CStifLogger::ETxt,
                          CStifLogger::EFile,
                          EFalse );
    
    SendTestClassVersion();
    //Connects the file session
    User::LeaveIfError(iFileSession.Connect());
    }

// -----------------------------------------------------------------------------
// CCUTLandmarkURL::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCUTLandmarkURL* CCUTLandmarkURL::NewL( 
    CTestModuleIf& aTestModuleIf )
    {
    CCUTLandmarkURL* self = new (ELeave) CCUTLandmarkURL( aTestModuleIf );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;

    }

// Destructor
CCUTLandmarkURL::~CCUTLandmarkURL()
    { 

    // Delete resources allocated from test methods
    Delete();

    // Delete logger
    delete iLog; 

    }

//-----------------------------------------------------------------------------
// CCUTLandmarkURL::SendTestClassVersion
// Method used to send version of test class
//-----------------------------------------------------------------------------
//
void CCUTLandmarkURL::SendTestClassVersion()
	{
	TVersion moduleVersion;
	moduleVersion.iMajor = TEST_CLASS_VERSION_MAJOR;
	moduleVersion.iMinor = TEST_CLASS_VERSION_MINOR;
	moduleVersion.iBuild = TEST_CLASS_VERSION_BUILD;
	
	TFileName moduleName;
	moduleName = _L("CUTLandmarkURL.dll");

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

    return ( CScriptBase* ) CCUTLandmarkURL::NewL( aTestModuleIf );

    }


//  End of File
