/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Playback settings
*
*/



// INCLUDE FILES
#include <centralrepository.h>
#include <mpxprivatecrkeys.h>

#include "mpxplaybacksettings.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMPXPlaybackSettings::CMPXPlaybackSettings()
    {
    // Do nothing
    }

// -----------------------------------------------------------------------------
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CMPXPlaybackSettings::ConstructL()
    {
    // Get Pre-Init setting
    }

// -----------------------------------------------------------------------------
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CMPXPlaybackSettings* CMPXPlaybackSettings::NewL()
    {
    CMPXPlaybackSettings* self = new(ELeave) CMPXPlaybackSettings;
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
//
CMPXPlaybackSettings::~CMPXPlaybackSettings()
    {
    }

// -----------------------------------------------------------------------------
// Returns PreInit enabled
// -----------------------------------------------------------------------------
//

//  End of File
