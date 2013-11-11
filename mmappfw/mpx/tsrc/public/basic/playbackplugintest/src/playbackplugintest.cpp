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
* Description:  Scripted Tests for MPX Playback Plugin
*
*/

// INCLUDE FILES
#include <Stiftestinterface.h>
#include <mpxplaybackutility.h>
#include "playbackplugintest.h"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPlaybackPluginTest::Cplaybackplugintest
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPlaybackPluginTest::CPlaybackPluginTest( CTestModuleIf& aTestModuleIf ):
        CBaseTest( aTestModuleIf )
    {   
    }   

// Destructor
CPlaybackPluginTest::~CPlaybackPluginTest()
    {        
    // Delete resources allocated from test methods
    Delete();
    
    iPlaybackUtility->Close();
    iPlaybackUtility = NULL;
    }
    
// -----------------------------------------------------------------------------
// CPlaybackPluginTest::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPlaybackPluginTest* CPlaybackPluginTest::NewL( CTestModuleIf& aTestModuleIf )
    {
    CPlaybackPluginTest* self = 
        new (ELeave) CPlaybackPluginTest( aTestModuleIf );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;
    }

// -----------------------------------------------------------------------------
// CPlaybackPluginTest::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPlaybackPluginTest::ConstructL()
    {
    CBaseTest::ConstructL();
    
    iLog = CStifLogger::NewL( KPlaybackPluginTestLogPath, 
                      KPlaybackPluginTestLogFile,
                      CStifLogger::ETxt,
                      CStifLogger::EFile,
                      EFalse );
    }

//-----------------------------------------------------------------------------
// CPlaybackPluginTest::SendTestClassVersion
// Method used to send version of test class
//-----------------------------------------------------------------------------
//
void CPlaybackPluginTest::SendTestClassVersion()
    {
    TVersion moduleVersion;
    moduleVersion.iMajor = TEST_CLASS_VERSION_MAJOR;
    moduleVersion.iMinor = TEST_CLASS_VERSION_MINOR;
    moduleVersion.iBuild = TEST_CLASS_VERSION_BUILD;
    
    TFileName moduleName;
    moduleName = _L("playbackplugintest.dll");

    TBool newVersionOfMethod = ETrue;
    TestModuleIf().SendTestModuleVersion(moduleVersion, moduleName, 
                                        newVersionOfMethod);
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

    return ( CScriptBase* ) CPlaybackPluginTest::NewL( aTestModuleIf );

    }


//  End of File
