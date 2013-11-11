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
* Description:  CMnrpApplication class implementation
*
*/


#include "debug.h"

#include "mnrpdocument.h"
#include "mnrpappserver.h"
#include "mnrpapplication.h"

// UID for the application, this should match the uid defined in the mmp file.
static const TUid KUidMnRefProviderApp = {0x03132013};

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CApaDocument* CMnrpApplication::CreateDocumentL()
    {  
    LOG( "MnRefProvider: CreateDocumentL" );
    CApaDocument* document = CMnrpDocument::NewL( *this );
    return document;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TUid CMnrpApplication::AppDllUid() const
    {
    // Return the UID for the MnRefProvider application
    return KUidMnRefProviderApp;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpApplication::NewAppServerL( CApaAppServer*& aAppServer )
    {
    LOG( "MnRefProvider: NewAppServerL" );
    aAppServer = new (ELeave) CMnrpAppServer;
    }
