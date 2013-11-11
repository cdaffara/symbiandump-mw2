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
* Description:  CMnrpNaviView class implementation
*
*/


#include <e32base.h>
#include <lbsposition.h>
#include <epos_cposlandmark.h>

#include <mnrefprovider.rsg>
#include "mnrp.hrh"

#include "debug.h"
#include "mnrputils.h"
#include "mnrpengine.h"

#include "mnrpappui.h"
#include "mnrpnavicontrol.h"

#include "mnrpnaviview.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnrpNaviView::CMnrpNaviView( CMnrpEngine& aEngine )
 : iEngine( aEngine )
    {
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnrpNaviView::~CMnrpNaviView()
    {
    delete iDestination;
    delete iControl;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnrpNaviView* CMnrpNaviView::NewL( TBool aChained, CMnrpEngine& aEngine )
    {
    CMnrpNaviView* self = new (ELeave) CMnrpNaviView( aEngine );
    CleanupStack::PushL( self );
    self->ConstructL( aChained );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpNaviView::ConstructL( TBool aChained )
    {
    if ( aChained )
        {
        BaseConstructL( R_MNREFPROVIDER_NAVI_VIEW_CHAINED );
        }
    else
        {
        BaseConstructL( R_MNREFPROVIDER_NAVI_VIEW );
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpNaviView::SetDestinationL( const CPosLandmark& aNewDestination )
    {
    delete iDestination;
    iDestination = NULL;
    iDestination = CPosLandmark::NewL( aNewDestination );
    if ( iControl )
        {
        iControl->UpdateModelL( *iDestination );
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TUid CMnrpNaviView::Id() const
    {
    return TUid::Uid( EMnrpNavigationViewId );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpNaviView::HandleCommandL( TInt aCommand )
    {
    switch ( aCommand )
        {
        default:
            AppUi()->HandleCommandL( aCommand );
            break;
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpNaviView::DoActivateL(
    const TVwsViewId& /*aPrevViewId*/,
    TUid /* aCustomMessageId */,
    const TDesC8& /* aCustomMessage */)
    {
    if ( !iControl )
        {
        iControl = CMnrpNaviControl::NewL( *this, ClientRect(), iEngine );
        iControl->SetMopParent( this );
        }

    if ( iDestination )
        {
        iControl->UpdateModelL( *iDestination );
        }

    // Enable receiving of keyboard events.
    CMnrpAppUi* ui = ( CMnrpAppUi* ) AppUi();
    ui->AddToStackL( *this, iControl );

    _LIT( KNavigation, "Navigation" );
    ui->SetNaviPaneTitleL( KNavigation );

    // Make view visible.
    iControl->MakeVisible( ETrue );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpNaviView::DoDeactivate()
    {
    if ( iControl )
        {
        // Hide view.
        iControl->MakeVisible( EFalse );

        // Disable receiving keyboard events.
        AppUi()->RemoveFromStack( iControl );
        }
    }

