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
* Description:  CMnrpNaviControl class implementation
*
*/


#include <e32base.h>
#include <coemain.h>
#include <aknlists.h>
#include <barsread.h>

#include <aknlists.h>

#include "mnrpnavimodel.h"
#include "mnrpnaviview.h"
#include "mnrpnavicontrol.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnrpNaviControl::CMnrpNaviControl( CMnrpNaviView& aView )
 :  iView( aView )
    { 
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnrpNaviControl::~CMnrpNaviControl()
    {
    delete iListBox;
    delete iModel;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnrpNaviControl* CMnrpNaviControl::NewL( 
    CMnrpNaviView& aView, 
    const TRect& aRect,
    CMnrpEngine& aEngine )
    {
    CMnrpNaviControl* self = new (ELeave) CMnrpNaviControl( aView );
    CleanupStack::PushL( self );
    self->ConstructL( aRect, aEngine );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpNaviControl::ConstructL( const TRect& aRect, CMnrpEngine& aEngine )
    {
    // Create a window for this application view
    CreateWindowL();
    CreateListBoxL();
    
    // Set the windows size
    SetRect( aRect );
    
    iModel = CMnrpNaviModel::NewL( *this, aEngine );

    // Activate the window, which makes it ready to be drawn
    ActivateL();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpNaviControl::UpdateModelL( CPosLandmark& aDestination )
    {
    iModel->SetDestinationL( aDestination );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpNaviControl::HandleModelUpdateL()
    {
    TArray< CMnrpNaviModel::CListItem* > items = iModel->ListItemsL();
    
    const TInt KGranularity = 8;
    CDesCArraySeg* array = new (ELeave) CDesCArraySeg( KGranularity );
    CleanupStack::PushL( array );
    
    _LIT( KItemFormat, "%S\t%S\t\t" );
    for ( TInt i = 0; i < items.Count(); i++ )
        {
        CMnrpNaviModel::CListItem* item = items[i];

        const TInt KNumTabsAndOther = 10;
        HBufC* text = HBufC::NewLC(
            item->Header().Length() + item->Value().Length() + KNumTabsAndOther);

        text->Des().Format( KItemFormat, &(item->Header()), &(item->Value()) );
        
        array->AppendL( *text );
        CleanupStack::PopAndDestroy( text );
        }
        
    CleanupStack::Pop( array );

    CTextListBoxModel* model = iListBox->Model();
    model->SetItemTextArray( array );
    model->SetOwnershipType( ELbmOwnsItemArray );
    iListBox->HandleItemAdditionL();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TInt CMnrpNaviControl::CountComponentControls() const 
    { 
    return 1; 
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CCoeControl* CMnrpNaviControl::ComponentControl(TInt aIndex) const
    {
    return (aIndex == 0) ? iListBox : NULL;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TKeyResponse CMnrpNaviControl::OfferKeyEventL(
    const TKeyEvent& aKeyEvent, TEventCode aType)
    {
    TKeyResponse response = EKeyWasNotConsumed;

    switch (aKeyEvent.iCode)
        {
        case EKeyUpArrow:
        case EKeyDownArrow:
            // Scrolling the listbox. Forward to iListBox.
            response = iListBox->OfferKeyEventL(aKeyEvent, aType);
            break;

        case EKeyLeftArrow:
        case EKeyRightArrow:
            // Eat these to disable switching views
            response = EKeyWasConsumed;
            break;

        case EKeyDevice3:
            response = EKeyWasConsumed;
            break;

        default:
            // menu keys should be handled
            break;
        }

    return response;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpNaviControl::CreateListBoxL()
    {
    // Create a control to display a list of descriptors
    iListBox = new (ELeave) CAknSingleHeadingStyleListBox();
    iListBox->SetContainerWindowL( *this );
    iListBox->ConstructL(this, CEikListBox::EViewerFlag );

    // Create scrollbars
    CEikScrollBarFrame* scrollBar = iListBox->CreateScrollBarFrameL();
    scrollBar->SetScrollBarVisibilityL(
                CEikScrollBarFrame::EOff, CEikScrollBarFrame::EAuto);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpNaviControl::SizeChanged()
    {
    iListBox->SetRect( Rect() );
    iListBox->DrawDeferred();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpNaviControl::HandleResourceChange( TInt aType )
    {
    CCoeControl::HandleResourceChange( aType );
    if ( aType == KEikDynamicLayoutVariantSwitch )
        {
        SetRect( iView.ClientRect() );
        }
    }
