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
* Description:   Document class for SendUi service resolver 
*
*/



#include <centralrepository.h>
#include <apgwgnam.h>           //CApaWindowGroupName

#include "senduisrappui.h"
#include "senduisrdoc.h"


// ======== LOCAL FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CSendUiSrDocument::CSendUiSrDocument(CEikApplication& aApp) : CAknDocument(aApp) 
    {
    // no implementation required
    }


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// ConstructL
// ---------------------------------------------------------------------------
//
void CSendUiSrDocument::ConstructL()
    {
    // no implementation required
    } 


// ---------------------------------------------------------------------------
// NewL
// ---------------------------------------------------------------------------
//
CSendUiSrDocument* CSendUiSrDocument::NewL(CEikApplication& aApp)
    {
    CSendUiSrDocument* self = NewLC(aApp);
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// NewLC
// ---------------------------------------------------------------------------
//
CSendUiSrDocument* CSendUiSrDocument::NewLC(CEikApplication& aApp)
    {
    CSendUiSrDocument* self = new (ELeave) CSendUiSrDocument(aApp);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CSendUiSrDocument::~CSendUiSrDocument()
    {
    // no implementation required
    }

// ---------------------------------------------------------------------------
//  From CAknDocument.
// ---------------------------------------------------------------------------
//
CEikAppUi* CSendUiSrDocument::CreateAppUiL()
    {
    // Create the application user interface, and return a pointer to it,
    // the framework takes ownership of this object
  
    CEikAppUi* appUi = new(ELeave)CSendUiSrAppUi;
    return appUi;
    }

// ---------------------------------------------------------------------------
//  From CAknDocument.
//  Hide application from the fast swap list.
// ---------------------------------------------------------------------------
//
void CSendUiSrDocument::UpdateTaskNameL(CApaWindowGroupName* aWgName)
    {
    CAknDocument::UpdateTaskNameL(aWgName);
    aWgName->SetHidden(ETrue); 
    }
    
