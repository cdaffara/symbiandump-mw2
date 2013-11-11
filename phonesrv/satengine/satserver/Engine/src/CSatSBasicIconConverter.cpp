/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Converts basic icon to CFbsBitmap
*
*/


#include    <fbs.h>
#include    <gdi.h>
#include    "CSatSBasicIconConverter.h"
#include    "TSatBitOP.h"
#include    "SatLog.h"

// Black&white icon uses 1 bpp.
const TUint8 KBpp = 1;

// Black&white icon header size is 2.
const TUint8 KHeaderSize( 2 );

// Value of icon data, which means that pixel is lit.
const TInt KPixelIsLit = 1;
const TUint KBitCount( 8 );

// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// CSatSBasicIconConverter::CSatSBasicIconConverter
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSatSBasicIconConverter::CSatSBasicIconConverter() :
CSatSIconConverter()
    {
    LOG( SIMPLE, "SATENGINE: \
        CSatSBasicIconConverter::CSatSBasicIconConverter calling - exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSBasicIconConverter::ConvertL
// Converts the basic icon format (ETSI TS 131 102 V4.10.0) to CFbsBitmap
// The format of aData has to be following:
//  1st byte:           Image width
//  2nd byte:           Image height
//  3rd byte to end:    Image body.
// Image body has to be in following format:
//  Bit 8:  1st pixel
//  Bit 7:  2nd pixel
//  Bit 6:  3rd pixel
//  etc...
//
// Clut is not needed for converting basic icon.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CFbsBitmap* CSatSBasicIconConverter::ConvertL(
    const TDesC8& aData,
    const TDesC8& /*aClut*/ ) const
    {
    LOG( SIMPLE, "SATENGINE: CSatSBasicIconConverter::ConvertL calling" )

    CheckDataValidityL( aData );
    const TUint8 width( aData[0] );
    const TUint8 height( aData[1] );

    // Create black and white bitmap.
    CFbsBitmap* bitmap = CreateBitmapL( width, height, EGray2 );

    // Extract the header information away from icon data.
    const TPtrC8 iconData(
        aData.Mid( KHeaderSize, aData.Length() - KHeaderSize ) );

    TBitmapUtil bitmapUtil( bitmap );
    bitmapUtil.Begin( TPoint( 0, 0 ) );
    TInt x( 0 );
    TInt y( 0 );
    // Convert the icon data.
    for ( y = 0; y < height; y++ )
        {
        bitmapUtil.SetPos( TPoint( 0, y ) );
        for ( x = 0; x < width; x++ )
            {
            const TInt startingBit( y * width + x );
            const TUint8 color =
                TSatBitOP::ExtractBits( startingBit, KBpp, iconData );

            if ( KPixelIsLit == color )
                {
                // Black is lit pixel.
                bitmapUtil.SetPixel( KRgbBlack.Value() );
                }
            else
                {
                // White means no pixel.
                bitmapUtil.SetPixel( KRgbWhite.Value() );
                }

            bitmapUtil.IncXPos();
            }
        }

    bitmapUtil.End();
    LOG3( SIMPLE,"SATENGINE: CSatSBasicIconConverter::ConvertL, \
          x: %d, y: %d", x, y )
    LOG( SIMPLE, "SATENGINE: CSatSBasicIconConverter::ConvertL exiting" )
    return bitmap;
    }

// -----------------------------------------------------------------------------
// CSatSBasicIconConverter::CheckDataValidityL
// Checks that icon data has valid length.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatSBasicIconConverter::CheckDataValidityL( const TDesC8& aData ) const
    {
    LOG( SIMPLE,
        "SATENGINE: CSatSBasicIconConverter::CheckDataValidityL calling" )

    TInt err( KErrNone );

    // Is there at least width and height in aData.
    const TInt dataLength( aData.Length() );

    if ( dataLength > KHeaderSize )
        {
        LOG( SIMPLE, "SATENGINE: CSatSBasicIconConverter::CheckDataValidityL \
             dataLength bigger than KHeaderSize" )
        const TUint8 width( aData[0] );
        const TUint8 height( aData[1] );

        // Width and height does not belong to image body.
        const TInt imageBodyLength( dataLength - KHeaderSize );

        // Check that image body has at least as many
        // bits than width and height succest. There might
        // be more bits because the actual size of the image
        // body may occupie only couple bits from
        // last byte in image body.
        const TInt bitsInImageBody( imageBodyLength * KBitCount );
        const TInt bitsExpectedInImageBody( width * height );

        if ( bitsInImageBody < bitsExpectedInImageBody )
            {
            LOG( SIMPLE, "SATENGINE: CSatSBasicIconConverter::\
            CheckDataValidityL bitsInImageBody < bitsExpectedInImageBody" )
            err = KErrCorrupt;
            }
        }
    else
        {
        LOG( SIMPLE, "SATENGINE: CSatSBasicIconConverter::CheckDataValidityL \
             dataLength less than KHeaderSize" )
        err = KErrCorrupt;
        }

    User::LeaveIfError( err );
    LOG( SIMPLE,
        "SATENGINE: CSatSBasicIconConverter::CheckDataValidityL exiting" )
    }

//  End of File
