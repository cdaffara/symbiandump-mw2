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
* Description:  CMnrpMapView class implementation
*
*/


#include <e32base.h>
#include <lbsposition.h>

#include <eikmenup.h>
#include <eikbtgpc.h>
#include <avkon.rsg>

#include <mnrefprovider.rsg>
#include "mnrp.hrh"

#include "debug.h"

#include "mnrputils.h"
#include "mnrpengine.h"

#include "mnrpappui.h"
#include "mnrpmapview.h"
#include "mnrpmapcontrol.h"
#include "mnrpmapviewservice.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnrpMapView::CMnrpMapView( CMnrpEngine& aEngine ) 
: iEngine( aEngine )
    {
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnrpMapView::~CMnrpMapView()
    {
    TRAP_IGNORE( HandleUserSelectionL( KErrCancel ) );
    delete iControl;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnrpMapView* CMnrpMapView::NewL( TBool aChained, CMnrpEngine& aEngine )
    {
    CMnrpMapView* self = new (ELeave) CMnrpMapView( aEngine );
    CleanupStack::PushL( self );
    self->ConstructL( aChained );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpMapView::ConstructL( TBool aChained )
    {
    iChained = aChained;
    if ( aChained )
        {
        BaseConstructL( R_MNREFPROVIDER_MAP_VIEW_CHAINED );
        }
    else
        {
        BaseConstructL( R_MNREFPROVIDER_MAP_VIEW );
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpMapView::SetMapModeL( CMnrpMapViewService& aService )
    {
    iSelectionCallback = NULL;
    iMapViewService = &aService;
/*
    if ( iChained )
        {
        Cba()->SetCommandSetL( R_AVKON_SOFTKEYS_OPTIONS_BACK );
        }
    else
        {
        Cba()->SetCommandSetL( R_AVKON_SOFTKEYS_OPTIONS_EXIT );
        }
*/        
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpMapView::SetSelectionModeL( 
    CMnrpMapViewService& aService,  
    MMapSelectionCallback& aCallback )
    {
    iMapViewService = &aService;
    iSelectionCallback = &aCallback;
    Cba()->SetCommandSetL( R_AVKON_SOFTKEYS_OPTIONS_CANCEL );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpMapView::StopSelection()
    {
    iSelectionCallback = NULL;
    iMapViewService = NULL;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpMapView::HandleUserSelectionL( TInt aError )
    {
    if ( iSelectionCallback && iControl )
        {
        if ( !aError )
            {
            iSelectionCallback->HandleSelectionL( iControl->CurrentLocation() );
            }
        else
            {
            iSelectionCallback->HandleSelectionErrorL( aError );
            }
        iSelectionCallback = NULL;            
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TUid CMnrpMapView::Id() const
    {
    return TUid::Uid( EMnrpMapViewId );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpMapView::HandleCommandL( TInt aCommand )
    {
    switch ( aCommand )
        {
        case EMnrpCommandFullScreen:
            if ( iControl ) 
                {
                iControl->ToggleFullScreen();
                }
            break;

        case EMnrpCommandSelect:
            HandleUserSelectionL( KErrNone );
            break;

        case EEikCmdExit:
        case EAknSoftkeyBack:
        case EAknSoftkeyExit:
            HandleUserSelectionL( KErrCancel );
            AppUi()->HandleCommandL( aCommand );
            break;

        default:
            AppUi()->HandleCommandL( aCommand );
            break;
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpMapView::DynInitMenuPaneL( TInt aResourceId, CEikMenuPane *aMenuPane )
    {
    if ( aResourceId == R_MNREFPROVIDER_MAP_VIEW_MENU )
        {
        TBool isSelectionMode = iSelectionCallback != NULL;
        aMenuPane->SetItemDimmed( EMnrpCommandSelect, !isSelectionMode );
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpMapView::DoActivateL(
    const TVwsViewId& /*aPrevViewId*/,
    TUid /* aCustomMessageId */,
    const TDesC8& /* aCustomMessage */)
    {
    if ( !iControl )
        {
        iControl = CMnrpMapControl::NewL( *this, ClientRect(), iEngine );
        iControl->SetMopParent( this );
        }

    iControl->SetRect( ClientRect() );
    if ( iMapViewService )
        {
        iControl->UpdateModelL( *iMapViewService );
        }
    iControl->SetSelectionModeL( ( iSelectionCallback == NULL ) ? EFalse : ETrue );

    // Enable receiving of keyboard events.
    CMnrpAppUi* ui = ( CMnrpAppUi* ) AppUi();
    ui->AddToStackL( *this, iControl );

    if ( iSelectionCallback )
        {
        ui->SetNaviPaneTitleL( iMapViewService->SelectionRequestText() );
        }
    else
        {
        _LIT( KMapView, "MapView" );
	    ui->SetNaviPaneTitleL( KMapView );
        }

    // Make view visible.
    iControl->MakeVisible( ETrue );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpMapView::DoDeactivate()
    {
    if ( iControl )
        {
        // Hide view.
        iControl->MakeVisible( EFalse );

        // Disable receiving keyboard events.
        AppUi()->RemoveFromStack( iControl );
        }
    }

