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
* Description:  CMnrpDocumentBack class implementation
*
*/


#include "mnrpappuiback.h"
#include "mnrpengine.h"
#include "mnrpdocumentback.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnrpDocumentBack::CMnrpDocumentBack(CEikApplication& aApp) 
    : CAknDocument(aApp) 
    {
    }   

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnrpDocumentBack* CMnrpDocumentBack::NewL( CEikApplication& aApp )
    {
    CMnrpDocumentBack* self = new (ELeave) CMnrpDocumentBack( aApp );
    return self;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CEikAppUi* CMnrpDocumentBack::CreateAppUiL()
    {
    // Create the application user interface, and return a pointer to it,
    // the framework takes ownership of this object
    CEikAppUi* appUi = new (ELeave) CMnrpAppUiBack();
    return appUi;
    }


