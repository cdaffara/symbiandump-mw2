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
* Description:  MPX view plugin definition.
*
*/



// INCLUDE FILES
#include <mpxcustomcommandobserver.h>
#include "mpxviewplugin.h"


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
EXPORT_C CMPXViewPlugin* CMPXViewPlugin::NewL( 
    const TUid& aUid, 
    MMPXCustomCommandObserver* aCommandObserver )
    {
    TAny* plugin = REComSession::CreateImplementationL( 
        aUid, 
        _FOFF( CMPXViewPlugin, iDtorKey ) );
    CMPXViewPlugin* self = reinterpret_cast<CMPXViewPlugin*>( plugin );
    self->SetObserver( aCommandObserver );
    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
EXPORT_C CMPXViewPlugin::~CMPXViewPlugin()
    {
    REComSession::DestroyedImplementation( iDtorKey );
    }

// ---------------------------------------------------------------------------
// Set command observer
// ---------------------------------------------------------------------------
//
EXPORT_C void CMPXViewPlugin::SetObserver( 
    MMPXCustomCommandObserver* aCommandObserver )
    {
    iCommandObserver = aCommandObserver;
    }

// End of File
