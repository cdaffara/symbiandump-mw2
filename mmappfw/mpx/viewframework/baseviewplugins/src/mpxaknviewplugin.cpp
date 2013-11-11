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
* Description:  MPX Avkon view plugin definition.
*
*/



// INCLUDE FILES
#include <aknViewAppUi.h>
#include <aknview.h>
#include <mpxcustomcommandobserver.h>
#include <mpxviewutilitycommanddef.h>

#include "mpxaknviewplugin.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
EXPORT_C CMPXAknViewPlugin::~CMPXAknViewPlugin()
    {
    // do nothing
    }

// ---------------------------------------------------------------------------
// From CMPXViewPlugin
// Interface method to create view.
// ---------------------------------------------------------------------------
//
EXPORT_C void CMPXAknViewPlugin::CreateViewL()
    {
    // no need to create again if view already exist
    if ( !iView )
        {
        CAknView* view = ConstructViewLC();
        iAvkonViewAppUi->AddViewL( view );
        CleanupStack::Pop( view );  // ownership transferred
        iView = view;
        }    
    }

// ---------------------------------------------------------------------------
// From CMPXViewPlugin
// Interface method to destroy view.
// ---------------------------------------------------------------------------
//
EXPORT_C void CMPXAknViewPlugin::DestroyView()
    {
    ASSERT( iView );
    }

// ---------------------------------------------------------------------------
// From CMPXViewPlugin
// Interface method to activate view.
// ---------------------------------------------------------------------------
//
EXPORT_C void CMPXAknViewPlugin::ActivateViewL( 
    const TDesC* aParam )
    {
    if ( !iView )
        {
        CreateViewL();
        }

    if ( !aParam )
        {
        iAvkonViewAppUi->ActivateLocalViewL( iView->Id() );
        }
    else
        {
        // Create a narrow heap descriptor from a unicode descriptor
        HBufC8* des8 = HBufC8::NewLC( aParam->Length() );
        des8->Des().Copy( *aParam );

        iAvkonViewAppUi->ActivateLocalViewL( iView->Id(), KNullUid, *des8 );

        CleanupStack::PopAndDestroy( des8 );
        }
    }

// ---------------------------------------------------------------------------
// From CMPXViewPlugin
// Interface method to activate as default view, will not bring to foreground
// ---------------------------------------------------------------------------
//
EXPORT_C void CMPXAknViewPlugin::SetAsDefaultViewL()
    {
    if ( !iView )
        {
        CreateViewL();
        }
    iAvkonViewAppUi->SetDefaultViewL( *iView );
    }

// ---------------------------------------------------------------------------
// From CMPXViewPlugin
// Interface method to deactivate view.
// ---------------------------------------------------------------------------
//
EXPORT_C void CMPXAknViewPlugin::DeactivateView()
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
