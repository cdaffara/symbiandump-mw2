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
* Description:  
*           Miscellaneous SVG-T related utility methods.
*
*/



// ========== INCLUDE FILES ================================

#include "MsgSvgUtils.h"

#include <SVGEngineInterfaceImpl.h>

// ========== LOCAL CONSTANTS AND MACROS ===================


// ========== MEMBER FUNCTIONS =============================


// ---------------------------------------------------------
// CMsgSvgUtils::CMsgSvgUtils
// 
// C++ constructor.
// ---------------------------------------------------------
//
EXPORT_C CMsgSvgUtils::CMsgSvgUtils()
    {
    }

// ---------------------------------------------------------
// CMsgSvgUtils::~CMsgSvgUtils
// 
// Destructor.
// ---------------------------------------------------------
//
CMsgSvgUtils::~CMsgSvgUtils()
    {
    }

// ---------------------------------------------------------
// CMsgSvgUtils::GenerateThumbnailL
// 
// Creates SVG-T engine instance and opens the given file handle
// to it. Calculates the correctly "fitted" thumbnail size from
// the given maximum size and either from SVG-T engine scaled size
// or if this is not set then from original SVG-T file dimensions.
// Finally renders the thumbnail frame to the bitmap & mask.
// ---------------------------------------------------------
//

#ifdef RD_SVGT_IN_MESSAGING

EXPORT_C void CMsgSvgUtils::GenerateThumbnailL( RFile& aFile, 
                                                CFbsBitmap* aBitmap,
                                                CFbsBitmap* aMask,
                                                TBool aAutomaticDrmControl,
                                                TSize& aContentSize )
    {
    TSize maxSize( aContentSize );
    
    TFontSpec spec;
    CSvgEngineInterfaceImpl* svgEngine = CSvgEngineInterfaceImpl::NewL( aBitmap, this, spec );
    CleanupStack::PushL( svgEngine );
    
    svgEngine->SetBackgroundColor( KRgbWhite.Value() );
    svgEngine->SetDRMRights( aAutomaticDrmControl );
    
    TInt domHandle( KNullHandle );
    MSvgError* result = svgEngine->PrepareDom( aFile, domHandle );
    
    HandleSVGEngineErrorL( result );
        
    result = svgEngine->UseDom( domHandle, aBitmap, aMask );
    
    HandleSVGEngineErrorL( result );
        
    aContentSize = svgEngine->Size();
    
    if ( aContentSize.iHeight <= 0 &&
         aContentSize.iWidth <= 0 )
        {
        // SVG engine might fail to set proper size so in that case
        // we use the original size.
        aContentSize = svgEngine->GetUnscaledContentSize( domHandle );
        }
    
    if ( aContentSize.iHeight > 0 &&
         aContentSize.iWidth > 0 )
        { 
        TReal widthRatio = static_cast<TReal>( maxSize.iWidth ) / 
                           static_cast<TReal>( aContentSize.iWidth );
        TReal heightRatio = static_cast<TReal>( maxSize.iHeight ) / 
                            static_cast<TReal>( aContentSize.iHeight );
        
        // Calculate "meet" ratio                    
        TReal scaleFactor = ( widthRatio < heightRatio ) ? widthRatio : heightRatio;
        
        aContentSize.iHeight *= scaleFactor;
        aContentSize.iWidth *= scaleFactor;
        
        svgEngine->SetSvgDimensionToFrameBuffer( aContentSize.iWidth, aContentSize.iHeight );
        
        // Chooses view box for SVG files where it has not bee set.
        svgEngine->ChooseViewBoxIfNotSet( domHandle );    
        svgEngine->InitializeEngine();
        
        // Render the content on consumer provided bitmap
        svgEngine->RenderFrame( NULL, 0 );
        }
    
    CleanupStack::PopAndDestroy( svgEngine );
    }

#else

EXPORT_C void CMsgSvgUtils::GenerateThumbnailL( RFile& /*aFile*/, 
                                                CFbsBitmap* /*aBitmap*/,
                                                CFbsBitmap* /*aMask*/,
                                                TBool /*aAutomaticDrmControl*/,
                                                TSize& /*aContentSize*/ )
    {
    User::Leave( KErrNotSupported );
    }
    
#endif

// -----------------------------------------------------------------------------
// CMsgSvgUtils::UpdateScreen
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CMsgSvgUtils::UpdateScreen()
    {    
    }

// -----------------------------------------------------------------------------
// CMsgSvgUtils::ScriptCall
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CMsgSvgUtils::ScriptCall( const TDesC& /*aScript*/, CSvgElementImpl* /*aCallerElement*/ )
    {
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CMsgSvgUtils::FetchImage
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CMsgSvgUtils::FetchImage( const TDesC& /*aUri*/, RFs& /*aSession*/, RFile& /*aFileHandle*/ )
    {
    return KErrNotFound;
    }

// -----------------------------------------------------------------------------
// CMsgSvgUtils::FetchFont
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CMsgSvgUtils::FetchFont( const TDesC& /*aUri*/, RFs& /*aSession*/, RFile& /*aFileHandle*/ )
    {
    return KErrNotFound;
    }

// -----------------------------------------------------------------------------
// CMsgSvgUtils::UpdatePresentation
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CMsgSvgUtils::UpdatePresentation( const TInt32&  /*aNoOfAnimation*/ )
    {
    }

// ---------------------------------------------------------
// CMsgSvgUtils::HandleSVGEngineErrorL
// 
// Performs SVG-T engine error resolving.
// ---------------------------------------------------------
//
void CMsgSvgUtils::HandleSVGEngineErrorL( MSvgError* aError ) const
    {
    if ( !aError )
        {
        User::Leave( KErrNoMemory ); 
        }
    else if ( aError->HasError() && !aError->IsWarning() )
        {
        User::Leave( aError->SystemErrorCode() ); 
        }
    }

//  End of File
