/*
* Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: Implementation of CVmSettingsUiIconFileProvider class.
*
*/

#include "vmbxlogger.h"
#include "vmsettingsuiiconfileprovider.h"

// ================= MEMBER FUNCTIONS ==========================================

// -----------------------------------------------------------------------------
// CVmSettingsUiIconFileProvider::CVmSettingsUiIconFileProvider
//
// -----------------------------------------------------------------------------
//
CVmSettingsUiIconFileProvider::CVmSettingsUiIconFileProvider()
    {
    VMBLOGSTRING( "VMBX: CVmSettingsUiIconFileProvider::CVmSettingsUiIconFileProvider <=>" );
    }

// -----------------------------------------------------------------------------
// CVmSettingsUiIconFileProvider::~CVmSettingsUiIconFileProvider
//
// -----------------------------------------------------------------------------
//
CVmSettingsUiIconFileProvider::~CVmSettingsUiIconFileProvider()
    {
    VMBLOGSTRING( "VMBX: CVmSettingsUiIconFileProvider::~CVmSettingsUiIconFileProvider <=" );
    iFile.Close();
    VMBLOGSTRING( "VMBX: CVmSettingsUiIconFileProvider::~CVmSettingsUiIconFileProvider =>" );
    }

// -----------------------------------------------------------------------------
// CVmSettingsUiIconFileProvider::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CVmSettingsUiIconFileProvider::ConstructL( RFile& aFile )
    {
    VMBLOGSTRING( "VMBX: CVmSettingsUiIconFileProvider::ConstructL <=" );
    User::LeaveIfError( iFile.Duplicate( aFile ) );
    VMBLOGSTRING( "VMBX: CVmSettingsUiIconFileProvider::ConstructL =>" );
    }

// ----------------------------------------------------------------------------
// CVmSettingsUiIconFileProvider::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CVmSettingsUiIconFileProvider* CVmSettingsUiIconFileProvider::NewL(
    RFile& aFile )
    {
    VMBLOGSTRING( "VMBX: CVmSettingsUiIconFileProvider::NewL <=" );
    CVmSettingsUiIconFileProvider* self =
        new ( ELeave ) CVmSettingsUiIconFileProvider();
    CleanupStack::PushL( self );
    self->ConstructL( aFile );
    CleanupStack::Pop( self );
    VMBLOGSTRING( "VMBX: CVmSettingsUiIconFileProvider::NewL =>" );
    return self;
    }

// ---------------------------------------------------------------------------
// From MAknIconFileProvider
//
// ---------------------------------------------------------------------------
//
void CVmSettingsUiIconFileProvider::RetrieveIconFileHandleL(
    RFile& aFile, const TIconFileType /*aType*/ )
    {
    VMBLOGSTRING( "VMBX: CVmSettingsUiIconFileProvider::RetrieveIconFileHandleL <=" );
    // duplicate the file's handle
    User::LeaveIfError( aFile.Duplicate( iFile ) );
    VMBLOGSTRING( "VMBX: CVmSettingsUiIconFileProvider::RetrieveIconFileHandleL =>" );
    }

// ---------------------------------------------------------------------------
// From MAknIconFileProvider
//
// ---------------------------------------------------------------------------
//
void CVmSettingsUiIconFileProvider::Finished()
    {
    VMBLOGSTRING( "VMBX: CVmSettingsUiIconFileProvider::Finished <=" );
    delete this;
    VMBLOGSTRING( "VMBX: CVmSettingsUiIconFileProvider::Finished =>" );
    }

//  End of File
