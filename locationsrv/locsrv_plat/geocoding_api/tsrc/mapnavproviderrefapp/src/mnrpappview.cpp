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
* Description:  CMnrpAppView class implementation
*
*/


#include "mnrp.hrh"
#include "mnrpappui.h"
#include "mnrpappcontrol.h"
#include "mnrpappview.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnrpAppView::CMnrpAppView()
    {
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnrpAppView::~CMnrpAppView()
    {
    delete iControl;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnrpAppView* CMnrpAppView::NewL()
    {
    CMnrpAppView* self = new ( ELeave ) CMnrpAppView();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpAppView::ConstructL()
    {
    BaseConstructL();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TUid CMnrpAppView::Id() const
    {
    return TUid::Uid( EMnrpAppViewId );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpAppView::DoActivateL(
    const TVwsViewId& /*aPrevViewId*/,
    TUid /* aCustomMessageId */,
    const TDesC8& /* aCustomMessage */)
    {
    if ( !iControl )
        {
        iControl = CMnrpAppControl::NewL( *this, ClientRect() );
        iControl->SetMopParent( this );
        }

    // Enable receiving of keyboard events.
    CMnrpAppUi* ui = ( CMnrpAppUi* ) AppUi();
    ui->AddToStackL( *this, iControl );

    _LIT( KEmpty, "" );
    ui->SetNaviPaneTitleL( KEmpty );

    // Make view visible.
    iControl->MakeVisible( ETrue );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpAppView::DoDeactivate()
    {
    if ( iControl )
        {
        // Hide view.
        iControl->MakeVisible( EFalse );

        // Disable receiving keyboard events.
        AppUi()->RemoveFromStack( iControl );
        }
    }
