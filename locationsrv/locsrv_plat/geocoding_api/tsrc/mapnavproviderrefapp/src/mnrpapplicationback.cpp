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
* Description:  CMnrpApplicationGeoGeo class implementation
*
*/


#include "debug.h"

#include "mnrpdocumentback.h"
#include "mnrpappserverback.h"
#include "mnrpapplicationback.h"

// UID for the application, this should match the uid defined in the mmp file.
static const TUid KUidMnRefProviderBackApp = {0x03132014};

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CApaDocument* CMnrpApplicationBack::CreateDocumentL()
    {  
    LOG( "MnRefProviderBack: CreateDocumentL" );
    CApaDocument* document = CMnrpDocumentBack::NewL( *this );
    return document;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TUid CMnrpApplicationBack::AppDllUid() const
    {
    // Return the UID for the MnRefProvider application
    return KUidMnRefProviderBackApp;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpApplicationBack::NewAppServerL( CApaAppServer*& aAppServer )
    {
    LOG( "MnRefProviderBack: NewAppServerL" );
    aAppServer = CMnrpAppServerBack::NewL();
    }
