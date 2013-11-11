/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  EdwinTestControl.cpp
*
*/

// Version : %version: 3.1.1 %




#include "EdwinTestControl.h"
#include <eikenv.h>
#include <aknsutils.h> 
#include <aknsdrawutils.h> 
#include <aknutils.h> 
#include <aknsbasicbackgroundcontrolcontext.h>


CEdwinTestControl::~CEdwinTestControl()
	{
	delete iEditWin;
	iEditWin = NULL;
	delete iBgContext;
	iBgContext = NULL;
	}   
 
CEdwinTestControl* CEdwinTestControl::NewL(void)
    {
    CEdwinTestControl* self = new(ELeave)CEdwinTestControl();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }
 
void CEdwinTestControl::ConstructL()
    {
    CreateWindowL();
 
    iEditWin = new (ELeave) CEikEdwin();
    iEditWin->ConstructL( 0, 100, 100, 1 );
    iEditWin->SetContainerWindowL( *this );
    iEditWin->CreateTextViewL();
//    iEditWin->SetInputCapabilitiesL( TCoeInputCapabilities::EAllText );
    
	// make first with no size at all
	iBgContext = CAknsBasicBackgroundControlContext::NewL(KAknsIIDQsnBgScreen,TRect(0,0,1,1), ETrue );
	// Setting rect will cause SizeChanged to be called
	// and iBgContext size & position is updated accordingly.
	SetRect( CEikonEnv::Static()->EikAppUi()->ClientRect() );	
	
	ActivateL();
	DrawNow();
    }
 
void CEdwinTestControl::SizeChanged()
	{
	if ( iBgContext )
		{
		iBgContext->SetRect(Rect());
 
		if ( &Window() )
			{
			iBgContext->SetParentPos( PositionRelativeToScreen() );
			}
		}
	if ( iEditWin )
		{
		iEditWin->SetRect(Rect());	
		}
	}
 
 
void CEdwinTestControl::HandleResourceChange( TInt aType )
	{
	TRect rect;
 
    if ( aType == KEikDynamicLayoutVariantSwitch )
    	{    
        AknLayoutUtils::LayoutMetricsRect(AknLayoutUtils::EMainPane, rect);
        SetRect(rect);
    	}
 
	CCoeControl::HandleResourceChange(aType);
	}
 
 
TTypeUid::Ptr CEdwinTestControl::MopSupplyObject(TTypeUid aId)
	{
	if ( iBgContext )
		{
		return MAknsControlContext::SupplyMopObject( aId, iBgContext );
		}	

	return CCoeControl::MopSupplyObject( aId );
	}
 
 
void CEdwinTestControl::Draw(const TRect& aRect) const
	{
	CWindowGc& gc = SystemGc();
    
    // draw background skin first.
  	MAknsSkinInstance* skin = AknsUtils::SkinInstance();
	AknsDrawUtils::Background( skin, iBgContext, this, gc, aRect );
	}

TInt CEdwinTestControl::CountComponentControls() const
{
    return 1;
}
 
CCoeControl* CEdwinTestControl::ComponentControl(TInt aIndex) const
{
    switch (aIndex)
    	{
        case 0:
        	return iEditWin;
        default:
            return 0;
    	}
}

void CEdwinTestControl::HandleControlEventL( CCoeControl* /*aControl*/, TCoeEvent /*aEventType*/)
	{
    // TODO: Add your control event handler code here
	}

TKeyResponse CEdwinTestControl::OfferKeyEventL( const TKeyEvent& aKeyEvent, TEventCode aType )
	{
	if ( iEditWin )
		{
		return iEditWin->OfferKeyEventL( aKeyEvent, aType );
		}
	else
		{
		return EKeyWasNotConsumed;
		}
	}
