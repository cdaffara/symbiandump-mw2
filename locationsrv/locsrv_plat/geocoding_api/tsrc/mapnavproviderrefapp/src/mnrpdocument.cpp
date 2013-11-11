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
* Description:  CMnrpDocument class implementation
*
*/


#include "mnrpappui.h"
#include "mnrpengine.h"
#include "mnrpdocument.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnrpDocument::CMnrpDocument(CEikApplication& aApp) 
    : CAknDocument(aApp) 
    {
    }   

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpDocument::ConstructL()
    {
    iEngine = CMnrpEngine::NewL();
    }    

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnrpDocument* CMnrpDocument::NewL( CEikApplication& aApp )
    {
    CMnrpDocument* self = new (ELeave) CMnrpDocument(aApp);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnrpDocument::~CMnrpDocument()
    {
    delete iEngine;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnrpEngine& CMnrpDocument::Engine()
    { 
    return *iEngine; 
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CEikAppUi* CMnrpDocument::CreateAppUiL()
    {
    // Create the application user interface, and return a pointer to it,
    // the framework takes ownership of this object
    CEikAppUi* appUi = new (ELeave) CMnrpAppUi( *iEngine );
    return appUi;
    }


