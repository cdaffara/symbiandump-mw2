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
* Description:   Server application for launching SendUi with low 
*                capability requirements.
*
*/


#include <eikappui.h>

#include "senduilauncherappui.h"
#include "senduilauncherdoc.h"


// ======== LOCAL FUNCTIONS ========
// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CSendUiLauncherDocument::CSendUiLauncherDocument(CEikApplication& aApp) : CAknDocument(aApp) 
    {
    // no implementation required
    }

// ---------------------------------------------------------------------------
// NewL
// ---------------------------------------------------------------------------
//
CSendUiLauncherDocument* CSendUiLauncherDocument::NewL(CEikApplication& aApp)
    {
    CSendUiLauncherDocument* self = NewLC(aApp);
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// NewLC
// ---------------------------------------------------------------------------
//
CSendUiLauncherDocument* CSendUiLauncherDocument::NewLC(CEikApplication& aApp)
    {
    CSendUiLauncherDocument* self = new (ELeave) CSendUiLauncherDocument(aApp);
    CleanupStack::PushL(self);
    return self;
    }


// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CSendUiLauncherDocument::~CSendUiLauncherDocument()
    {
    // no implementation required
    }

// ---------------------------------------------------------------------------
// CreateAppUiL
// ---------------------------------------------------------------------------
//
CEikAppUi* CSendUiLauncherDocument::CreateAppUiL()
    {
    // Create the application user interface, and return a pointer to it,
    // the framework takes ownership of this object
    CEikAppUi* appUi = new(ELeave)CSendUiLauncherAppUi;
    return appUi;
    }
