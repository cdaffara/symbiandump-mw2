/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Container class for refresh related implementation..
*
*/



// INCLUDE FILES
#include    <csssettingsrefreshcontainer.h> 
#include    "csssettingsrefreshhandler.h" 


// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CSSSettingsRefreshContainer::CSSSettingsRefreshContainer
// -----------------------------------------------------------------------------
//
CSSSettingsRefreshContainer::CSSSettingsRefreshContainer()
    {
    }

// -----------------------------------------------------------------------------
// CSSSettingsRefreshContainer::ConstructL
// -----------------------------------------------------------------------------
//
void CSSSettingsRefreshContainer::ConstructL()
    {
    iRefreshHandler = CSSSettingsRefreshHandler::NewL(); 
    }

// -----------------------------------------------------------------------------
// CSSSettingsRefreshContainer::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CSSSettingsRefreshContainer* CSSSettingsRefreshContainer::NewL()
    {
    CSSSettingsRefreshContainer* self = 
        new( ELeave ) CSSSettingsRefreshContainer;
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;
    }

// -----------------------------------------------------------------------------
// CSSSettingsRefreshContainer::~CSSSettingsRefreshContainer
// -----------------------------------------------------------------------------
//
EXPORT_C CSSSettingsRefreshContainer::~CSSSettingsRefreshContainer()
    {
    delete iRefreshHandler;
    iRefreshHandler = NULL;
    }

// -----------------------------------------------------------------------------
// CSSSettingsRefreshContainer::RefreshHandler
// -----------------------------------------------------------------------------
//
EXPORT_C MSSSettingsRefreshHandler& 
    CSSSettingsRefreshContainer::RefreshHandler()
    {
    return *iRefreshHandler;
    }


//  End of File  
