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
* Description:  CMnrpMapControl class implementation
*
*/


#include <coemain.h>
#include <aknlists.h>
#include <barsread.h>
#include <aknnotewrappers.h>

#include <bitstd.h>
#include <bitdev.h>

#include <lbsposition.h>

#include "debug.h"

#include "mnrputils.h"
#include "mnrpmapview.h"
#include "mnrpmapcontrol.h"

const TInt KCodeKeyLeft = 0x34;
const TInt KCodeKeyRight = 0x36;
const TInt KCodeKeyUp = 0x32;
const TInt KCodeKeyDown = 0x38;
const TInt KCodeKeyCenter = 0x35;

const TInt KCodeKeySelect = EKeyDevice3;
const TInt KCodeKeySelect2 = 0x13;

const TInt KCodeKeyZoomIn = 42;
const TInt KCodeKeyZoomOut = 35;
const TInt KCodeKeyZoomIn2 = 0x33;
const TInt KCodeKeyZoomOut2 = 0x39;

const TRgb KCursorColor( 255, 255, 255 );
const TRgb KGridTextColor( 0, 0, 0 );
const TRgb KOriginMarkColor( 0, 0, 255 );

const TInt KTextFontHeightInTwips = 100;
const TInt KItemFontHeightInTwips = 120;
const TReal KBorderSizeFactor = 2;

const TInt KCursorMarginSize = 20; // pixels
const TInt KCursorArrowSize = 4; // pixels
const TInt KCursorCircleRadius = 4; // pixels

const TInt KCursorMove = 10; // pixels
const TInt KWindowMove = 20; // pixels
const TReal KJumpFactor = 5;
const TReal KCrawlFactor = 2.0 / KCursorMove;

const TReal KZoomOutRate = 1.5;
const TReal KZoomInRate =  1 / KZoomOutRate;

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnrpMapControl::CMnrpMapControl( CMnrpMapView& aView )
 :  iView( aView )
    {
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnrpMapControl::~CMnrpMapControl()
    {
    delete iModel;
    delete iOffScreenBitmap;
    DestroyFonts();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnrpMapControl* CMnrpMapControl::NewL( 
    CMnrpMapView& aView,
    const TRect& aRect,
    CMnrpEngine& aEngine )
    {
    CMnrpMapControl* self = new ( ELeave ) CMnrpMapControl( aView );
    CleanupStack::PushL( self );
    self->ConstructL( aRect, aEngine );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpMapControl::UpdateModelL( CMnrpMapViewService& aService )
    {
    iModel->UpdateModelL( aService );
    }
    
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpMapControl::SetSelectionModeL( TBool aSelectionMode ) 
    {
    iSelectionMode = aSelectionMode;
    iAbsoluteCursor = iModel->Center();
    iCursor = iModel->AbsoluteToScreen( iAbsoluteCursor );
    DrawDeferred();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TCoordinate CMnrpMapControl::CurrentLocation()
    {
    return iAbsoluteCursor;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpMapControl::ConstructL( 
    const TRect& aRect,
    CMnrpEngine& aEngine )
    {
    // Create a window for this application view
    CreateWindowL();

    iModel = CMnrpMapModel::NewL( *this, aEngine );

    // Set the windows size
    SetRect( aRect );
    
    iAbsoluteCursor = TCoordinate( 0, 0 );
    iCursor = iModel->AbsoluteToScreen( iAbsoluteCursor );
    
    CreateFonts();
    CreateScreenBufferL( aRect.Size() );

    // Activate the window, which makes it ready to be drawn
    ActivateL();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TKeyResponse CMnrpMapControl::OfferKeyEventL(
    const TKeyEvent& aKeyEvent, 
    TEventCode /*aType*/ )
    {
    TKeyResponse response = EKeyWasConsumed;
    
    TInt step = iSelectionMode ? KCursorMove : KWindowMove;
    TBool isShift = aKeyEvent.iModifiers & EModifierShift;
    TReal crawlFactor = isShift ? KCrawlFactor : 1;

    TInt xmove = 0;
    TInt ymove = 0;
    
    TBool redraw = ETrue;

    switch ( aKeyEvent.iCode )
        {
        case EKeyLeftArrow:
            xmove = -step * crawlFactor;
            break;

        case KCodeKeyLeft:
            xmove = -step * KJumpFactor;
            break;

        case EKeyRightArrow:
            xmove = step * crawlFactor;
            break;

        case KCodeKeyRight:
            xmove = step * KJumpFactor;
            break;

        case EKeyUpArrow:
            ymove = -step * crawlFactor;
            break;

        case KCodeKeyUp:
        case EKeyPageUp:
            ymove = -step * KJumpFactor;
            break;

        case EKeyDownArrow:
            ymove = step * crawlFactor;
            break;
            
        case KCodeKeyDown:
        case EKeyPageDown:
            ymove = step * KJumpFactor;
            break;

        case KCodeKeyZoomIn:
        case KCodeKeyZoomIn2:
            Zoom( KZoomInRate );
            break;

        case KCodeKeyZoomOut:
        case KCodeKeyZoomOut2:
            Zoom( KZoomOutRate );
            break;

        case KCodeKeyCenter:
            iModel->GoCenter();
            iCursor = iModel->AbsoluteToScreen( TCoordinate( 0, 0 ) );
            break;

        case KCodeKeySelect:
        case KCodeKeySelect2:
            if ( iSelectionMode )
                {
                iView.HandleUserSelectionL( KErrNone );
                redraw = EFalse;
                }
            break;

        default:
            response = EKeyWasNotConsumed;
            redraw = EFalse;
            break;
        }

    if ( xmove || ymove )
        {
        if ( iSelectionMode )
            {
            MoveCursor( xmove, ymove );
            }
        else
            {
            TCoordinate center = iModel->Center();
            TCoordinate moveTo( 
                center.Latitude() - iModel->ScreenToAbsolute( ymove ),
                center.Longitude() + iModel->ScreenToAbsolute( xmove ) );
            iModel->MoveTo( moveTo );
            }
        }

    if ( redraw )
        {
        RenderL();
        DrawDeferred();
        }

    return response;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpMapControl::HandleModelUpdateL()
    {
    RenderL();
    DrawDeferred();
    
    TInt ignored = iModel->NumIgnoredLandmarks();
    if ( ignored )
    	{
        CAknInformationNote* note = new (ELeave) CAknInformationNote( ETrue );
        _LIT( KMessageIgnoredLm, "One landmark\nhas no coordinates\nand can't be shown." );
        _LIT( KMessageIgnoredLms, "%d landmarks\nhave no coordinates\nand can't be shown." );
        if ( ignored == 1 )
        	{
        	note->ExecuteLD( KMessageIgnoredLm() );
        	}
        else
        	{
            TBuf<64> message;
        	message.Format( KMessageIgnoredLms, ignored );
        	note->ExecuteLD( message );
        	}
    	}
    }
    
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpMapControl::CreateScreenBufferL( const TSize& aSize )
	{
	CGraphicsDevice* screenDevice = iEikonEnv->ScreenDevice();

	// create a bitmap to be used off-screen
	CFbsBitmap* offScreenBitmap = new (ELeave) CFbsBitmap();
	User::LeaveIfError( offScreenBitmap->Create( aSize, screenDevice->DisplayMode() ) );
	CleanupStack::PushL( offScreenBitmap );

	if ( iOffScreenBitmap )
		{
		delete iOffScreenBitmap;
		}
	iOffScreenBitmap = offScreenBitmap;

	CleanupStack::Pop( offScreenBitmap );
	}


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpMapControl::CreateFonts()
	{
    CGraphicsDevice* screenDevice = iEikonEnv->ScreenDevice();

    const CFont* denseFont = iEikonEnv->DenseFont();
    TFontSpec fs = denseFont->FontSpecInTwips();

    fs.iHeight = KTextFontHeightInTwips;
    screenDevice->GetNearestFontInTwips( iTextFont, fs );
    
    fs.iHeight = KItemFontHeightInTwips;
    screenDevice->GetNearestFontInTwips( iItemFont, fs );
	}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpMapControl::DestroyFonts()
	{
	CGraphicsDevice* screenDevice = iEikonEnv->ScreenDevice();
	
    screenDevice->ReleaseFont( iItemFont );
    iItemFont = NULL;
    screenDevice->ReleaseFont( iTextFont );
    iTextFont = NULL;
	}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpMapControl::Draw( const TRect& /*aRect*/ ) const
    {
    // Get the standard graphics context
    CWindowGc& gc = SystemGc();

    TRect rect = Rect();
    TRect border( rect );
    border.Shrink( BorderSize() );
	
    gc.Clear( TRect( rect.iTl.iX, rect.iTl.iY, rect.iBr.iX, border.iBr.iY ) );
    gc.Clear( TRect( rect.iTl.iX, border.iBr.iY, rect.iBr.iX, rect.iBr.iY ) );
    gc.Clear( TRect( rect.iTl.iX, border.iTl.iY, border.iTl.iX, border.iBr.iY ) );
    gc.Clear( TRect( border.iBr.iX, border.iTl.iY, rect.iBr.iX, border.iBr.iY ) );

    CMnrpMapModel::TGrid grid;
    iModel->GetGrid( grid );
    DrawGridText( gc, border, rect, iTextFont, grid );

	if ( iOffScreenBitmap )
		{
		gc.BitBlt( border.iTl, iOffScreenBitmap );
		}
	else
		{
		gc.Clear();	
		}
			
    if ( iSelectionMode )
        {
        gc.SetOrigin( border.iTl );
        DrawCursor( gc );
        }

    // draw border
    gc.SetPenColor( KRgbGray );
    gc.SetPenSize( TSize( 1, 1 ) );
    gc.SetOrigin( rect.iTl );
    gc.DrawRect( border );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpMapControl::DrawGridText( 
    CWindowGc& aGc, 
    const TRect& aInnerBox,
    const TRect& aOuterBox,
    const CFont* aFont,
    CMnrpMapModel::TGrid& aGrid ) const
    {
    aGc.UseFont( aFont );
    
    TBuf<KMaxCoordStrSize> latTopStr, latBottomStr, lonLeftStr, lonRightStr;
    TBuf<KMaxCoordStrSize> gridMajorStr, latStr, lonStr;
    
    MnrpUtils::CoordinateToString( iModel->Left(), lonLeftStr, ETrue );
    MnrpUtils::CoordinateToString( iModel->Top(), latTopStr, ETrue );
    MnrpUtils::CoordinateToString( iModel->Right(), lonRightStr, ETrue );
    MnrpUtils::CoordinateToString( iModel->Bottom(), latBottomStr, ETrue );

    if ( !iSelectionMode )
        {
        MnrpUtils::CoordinateToString( ( iModel->Right() + iModel->Left() ) / 2, lonStr, ETrue );
        MnrpUtils::CoordinateToString( ( iModel->Top() + iModel->Bottom() ) / 2, latStr, ETrue );
        }
    else
        {
        MnrpUtils::CoordinateToString( iAbsoluteCursor.Longitude(), lonStr, ETrue );
        MnrpUtils::CoordinateToString( iAbsoluteCursor.Latitude(), latStr, ETrue );
        }

    MnrpUtils::CoordinateToString( aGrid.iAbsoluteGridStep, gridMajorStr );

    TRect lonBox( aInnerBox.iTl.iX, 0, aInnerBox.iBr.iX, aInnerBox.iBr.iY );
    TRect latBox( 0, aInnerBox.iTl.iY, aInnerBox.iTl.iX, aInnerBox.iBr.iY );
    TRect centerBox( aInnerBox.iTl.iX, aInnerBox.iBr.iY, aInnerBox.iBr.iX, aOuterBox.iBr.iY );

    TInt textBaseLine = aFont->AscentInPixels() + KBorderSizeFactor / 2;
    TInt vertTextBaseLine = aFont->AscentInPixels() + KBorderSizeFactor / 2;

    aGc.SetPenColor( KGridTextColor );

    aGc.DrawText( lonLeftStr, lonBox, textBaseLine, CGraphicsContext::ELeft );
    aGc.DrawText( lonRightStr, lonBox, textBaseLine, CGraphicsContext::ERight );
    aGc.DrawTextVertical( latTopStr, latBox, vertTextBaseLine, EFalse, CGraphicsContext::ELeft );
    aGc.DrawTextVertical( latBottomStr, latBox, vertTextBaseLine, EFalse, CGraphicsContext::ERight );

    aGc.DrawText( gridMajorStr, lonBox, textBaseLine, CGraphicsContext::ECenter );

    aGc.SetPenColor( KOriginMarkColor );
    aGc.DrawText( latStr, centerBox, textBaseLine, CGraphicsContext::ELeft );
    aGc.DrawText( lonStr, centerBox, textBaseLine, CGraphicsContext::ERight );
    
    aGc.DiscardFont();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpMapControl::DrawCursor( CWindowGc& aGc ) const
    {
    aGc.SetPenColor( KCursorColor );
    aGc.SetPenSize( TSize( 1, 1 ) );
    aGc.SetDrawMode( CGraphicsContext::EDrawModeNOTSCREEN );
    
    TRect cursorCircleRect( iCursor, TSize( 0, 0 ) );
    cursorCircleRect.Grow( KCursorCircleRadius, KCursorCircleRadius );
    aGc.DrawEllipse( cursorCircleRect );
    
    // upper vertical line
    aGc.DrawLine( TPoint( iCursor.iX, cursorCircleRect.iTl.iY ), 
                  TPoint( iCursor.iX, cursorCircleRect.iTl.iY - KCursorArrowSize ) );
    // lower vertical line
    aGc.DrawLine( TPoint( iCursor.iX, cursorCircleRect.iBr.iY ), 
                  TPoint( iCursor.iX, cursorCircleRect.iBr.iY + KCursorArrowSize ) );

    // left horiz line
    aGc.DrawLine( TPoint( cursorCircleRect.iTl.iX, iCursor.iY ), 
                  TPoint( cursorCircleRect.iTl.iX - KCursorArrowSize, iCursor.iY ) );
    // right horiz line
    aGc.DrawLine( TPoint( cursorCircleRect.iBr.iX, iCursor.iY ), 
                  TPoint( cursorCircleRect.iBr.iX + KCursorArrowSize, iCursor.iY ) );

    aGc.SetDrawMode( CGraphicsContext::EDrawModePEN );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpMapControl::SizeChanged()
    {
    TRect rect( Rect() );
    
    DestroyFonts();
    CreateFonts();
    
    rect.Shrink( BorderSize() );
    iViewSize = rect.Size();
    iModel->SetScreenSize( iViewSize );
    TRAP_IGNORE( CreateScreenBufferL( rect.Size() ) );
    TRAP_IGNORE( RenderL() );
    DrawDeferred();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpMapControl::HandleResourceChange( TInt aType )
    {
    CCoeControl::HandleResourceChange( aType );
    if ( aType == KEikDynamicLayoutVariantSwitch )
        {
        SetFullScreen( iIsFullScreen );
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpMapControl::ToggleFullScreen()
    {
    SetFullScreen( !iIsFullScreen );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpMapControl::SetFullScreen( TBool aFullScreen )
    {
    iIsFullScreen = aFullScreen;
    if ( iIsFullScreen)
        {
        SetExtentToWholeScreen();
        }
    else
        {
        SetRect( iView.ClientRect() );
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpMapControl::MoveCursor( TInt aXmove, TInt aYmove )
    {
    iCursor += TPoint( aXmove, aYmove );

    iCursor.iX = Max( 0, iCursor.iX );
    iCursor.iX = Min( iViewSize.iWidth, iCursor.iX );

    iCursor.iY = Max( 0, iCursor.iY );
    iCursor.iY = Min( iViewSize.iHeight, iCursor.iY );

    // verify cursor margin
    TRect cursorMargin = CursorMargin();
    
    iAbsoluteCursor = iModel->ScreenToAbsolute( iCursor );
    
    TReal xcorrection = 0;
    TReal ycorrection = 0;

    if ( aXmove > 0 ) // moving right
        {
        TInt toMargin = iCursor.iX - cursorMargin.iBr.iX;
        if ( toMargin > 0 ) // cursor went beyond cursor margin, move view
            {
            xcorrection = iModel->ScreenToAbsolute( toMargin );
            }
        }
    else if ( aXmove < 0 ) // moving left
        {
        TInt toMargin = iCursor.iX - cursorMargin.iTl.iX;
        if ( toMargin < 0 )
            {
            xcorrection = iModel->ScreenToAbsolute( toMargin );
            }
        }

    if ( aYmove > 0 ) // moving down
        {
        TInt toMargin = iCursor.iY - cursorMargin.iBr.iY;
        if ( toMargin > 0 )
            {
            ycorrection = -iModel->ScreenToAbsolute( toMargin );
            }
        }
    else if ( aYmove < 0 ) // moving up
        {
        TInt toMargin = iCursor.iY - cursorMargin.iTl.iY;
        if ( toMargin < 0 )
            {
            ycorrection = -iModel->ScreenToAbsolute( toMargin );
            }
        }
    
    TCoordinate center = iModel->Center();
    TCoordinate moveTo( 
        center.Latitude() + ycorrection,
        center.Longitude() + xcorrection );
    iModel->MoveTo( moveTo );

    iCursor = iModel->AbsoluteToScreen( iAbsoluteCursor );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TRect CMnrpMapControl::CursorMargin()
    {
    TRect rect( iViewSize );
    rect.Shrink( KCursorMarginSize, KCursorMarginSize );
    return rect;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpMapControl::Zoom( TReal aRate )
    {
    if ( iSelectionMode )
        {
        iModel->MoveTo( iAbsoluteCursor );
        iModel->Zoom( aRate );
        iCursor = iModel->AbsoluteToScreen( iAbsoluteCursor );
        }
    else
        {
        iModel->Zoom( aRate );
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TSize CMnrpMapControl::BorderSize() const 
    {
	TSize size( 0, 0 );
	if ( iTextFont )
		{
	    TInt fontHeight = iTextFont->HeightInPixels();
	    
	    size.iWidth = size.iHeight = TInt( fontHeight + KBorderSizeFactor );
		}
    return size;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpMapControl::RenderL()
    {
    TRect rect = Rect();
    TRect border( rect );
    border.Shrink( BorderSize() );

    if ( iOffScreenBitmap && iTextFont && iItemFont )
        {
        iModel->RenderL( *iOffScreenBitmap, TRect( border.Size() ), iTextFont, iItemFont );
        }
    }
