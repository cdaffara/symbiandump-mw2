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
* Description:  MPX Avkon dialog plugin definition.
*
*/



// INCLUDE FILES
#include <AknDialog.h>
#include <mpxcustomcommandobserver.h>
#include <mpxviewutilitycommanddef.h>

#include "mpxakndialogplugin.h"


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
EXPORT_C CMPXAknDialogPlugin::~CMPXAknDialogPlugin()
    {
    delete iDialog;
    }

// ---------------------------------------------------------------------------
// From CMPXViewPlugin
// Interface method to create view.
// ---------------------------------------------------------------------------
//
EXPORT_C void CMPXAknDialogPlugin::CreateViewL()
    {
    // no need to create again if view already exist
    if ( !iDialog )
        {
        iDialog = ConstructDialogL();
        }    
    }

// ---------------------------------------------------------------------------
// From CMPXViewPlugin
// Interface method to destroy view.
// ---------------------------------------------------------------------------
//
EXPORT_C void CMPXAknDialogPlugin::DestroyView()
    {
    delete iDialog;
    iDialog = NULL;
    }

// ---------------------------------------------------------------------------
// From CMPXViewPlugin
// Interface method to activate view.
// ---------------------------------------------------------------------------
//
EXPORT_C void CMPXAknDialogPlugin::ActivateViewL( 
    const TDesC* /* aParam */ )
    {
    if ( !iDialog )
        {
        CreateViewL();
        }    
    iDialog->ExecuteLD( ResourceId() );
    if ( iCommandObserver )
        {
        TPckgC<TInt> selfPtr( reinterpret_cast<TInt>( this ) );    
        iCommandObserver->HandleCustomCommand( 
            TUid::Uid( KMPXViewUtilityCustomCommand ), 
            EMPXViewUtilCmdViewDeActivated,
            selfPtr );
        }    
    iDialog = NULL;
    }

// ---------------------------------------------------------------------------
// From CMPXViewPlugin
// Interface method to activate as default view, will not bring to foreground
// ---------------------------------------------------------------------------
//
EXPORT_C void CMPXAknDialogPlugin::SetAsDefaultViewL()
    {
    User::Leave( KErrNotSupported );
    }

// ---------------------------------------------------------------------------
// From CMPXViewPlugin
// Interface method to deactivate view.
// ---------------------------------------------------------------------------
//
EXPORT_C void CMPXAknDialogPlugin::DeactivateView()
    {
    if ( iCommandObserver )
        {
        TPckgC<TInt> selfPtr( reinterpret_cast<TInt>( this ) );    
        iCommandObserver->HandleCustomCommand( 
            TUid::Uid( KMPXViewUtilityCustomCommand ), 
            EMPXViewUtilCmdViewDeActivated,
            selfPtr );
        }
    }

// End of File
