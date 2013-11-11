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
* Description:  CMnrpAppControlclass implementation
*
*/


#include <coemain.h>
#include <akndef.h>
#include "mnrpappview.h"
#include "mnrpappcontrol.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnrpAppControl::CMnrpAppControl( CMnrpAppView& aView )
: iView( aView )
    {
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnrpAppControl::~CMnrpAppControl()
    {
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnrpAppControl* CMnrpAppControl::NewL( CMnrpAppView& aView, const TRect& aRect)
    {
    CMnrpAppControl* self = new (ELeave) CMnrpAppControl( aView );
    CleanupStack::PushL( self );
    self->ConstructL( aRect );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TInt CMnrpAppControl::CountComponentControls() const 
    { 
    return 0; 
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CCoeControl* CMnrpAppControl::ComponentControl( TInt /*aIndex*/ ) const
    {
    return NULL;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpAppControl::ConstructL( const TRect& aRect )
    {
    // Create a window for this application view
    CreateWindowL();

    // Set the windows size
    SetRect( aRect );

    // Activate the window, which makes it ready to be drawn
    ActivateL();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpAppControl::Draw( const TRect& /*aRect*/ ) const
    {
    // Get the standard graphics context
    CWindowGc& gc = SystemGc();

    // Gets the control's extent
    TRect rect = Rect();

    // Clears the screen
    gc.Clear( rect );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpAppControl::HandleResourceChange( TInt aType )
    {
    CCoeControl::HandleResourceChange( aType );
    if ( aType == KEikDynamicLayoutVariantSwitch )
        {
        SetRect( iView.ClientRect() );
        }
    }
