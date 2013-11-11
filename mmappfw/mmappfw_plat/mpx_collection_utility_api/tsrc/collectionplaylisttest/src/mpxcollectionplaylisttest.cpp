/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  STIF testing for MPX Collection Playlist
*
*/


// INCLUDE FILES
#include <Stiftestinterface.h>
#include <SettingServerClient.h>
#include "mpxcollectionplaylisttest.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CMPXCollectionPlaylistTest::CMPXCollectionPlaylistTest
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
//
CMPXCollectionPlaylistTest::CMPXCollectionPlaylistTest(
    CTestModuleIf& aTestModuleIf ):
        CScriptBase( aTestModuleIf )
    {
    iCollectionPlaylist = NULL;
    }

// ---------------------------------------------------------------------------
// CMPXCollectionPlaylistTest::ConstructL
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CMPXCollectionPlaylistTest::ConstructL()
    {
    iLog = CStifLogger::NewL( KMPXCollectionPlaylistTestLogPath,
                              KMPXCollectionPlaylistTestLogFile,
                              CStifLogger::ETxt,
                              CStifLogger::EFile,
                              EFalse );
    // Print title of the test case
    TName title;
    TestModuleIf().GetTestCaseTitleL(title);
    iLog->Log(_L(" "));
    iLog->Log(_L("[Title] %S"), &title);
    }

// ---------------------------------------------------------------------------
// CMPXCollectionPlaylistTest::NewL
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CMPXCollectionPlaylistTest* CMPXCollectionPlaylistTest::NewL(
    CTestModuleIf& aTestModuleIf )
    {
    CMPXCollectionPlaylistTest* self = new (ELeave) CMPXCollectionPlaylistTest( aTestModuleIf );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// Destructor
CMPXCollectionPlaylistTest::~CMPXCollectionPlaylistTest()
    {
    // Delete resources allocated from test methods
    Delete();
    // Delete logger
    delete iLog;
    }

// ======== GLOBAL FUNCTIONS ========

// ---------------------------------------------------------------------------
// LibEntryL is a polymorphic Dll entry point.
// Returns: CScriptBase: New CScriptBase derived object
// ---------------------------------------------------------------------------
//
EXPORT_C CScriptBase* LibEntryL(
    CTestModuleIf& aTestModuleIf ) // Backpointer to STIF Test Framework
    {
    return ( CScriptBase* ) CMPXCollectionPlaylistTest::NewL( aTestModuleIf );
    }
