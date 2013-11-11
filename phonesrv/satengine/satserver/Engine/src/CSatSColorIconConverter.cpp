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
* Description:  Converts color icon to CFbsBitmap
*
*/


#include    <gdi.h>
#include    <fbs.h>
#include    "CSatSColorIconConverter.h"
#include    "TSatBitOP.h"
#include    "SatLog.h"

const TInt KHeaderSize = 6;
const TInt KClutEntrySize = 3;
const TInt KRedShift = 16;
const TInt KGreenShift = 8;
const TInt KPixelSize = 8;

// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// CSatSColorIconConverter::CSatSColorIconConverter
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSatSColorIconConverter::CSatSColorIconConverter()
    {
    LOG( SIMPLE, "SATENGINE: \
        CSatSColorIconConverter::CSatSColorIconConverter calling - exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSColorIconConverter::CheckDataValidityL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatSColorIconConverter::CheckDataValidityL(
    const TDesC8& aData ) const
    {
    LOG( SIMPLE,
        "SATENGINE: CSatSColorIconConverter::CheckDataValidityL calling" )
    TInt err( KErrNone );
    const TInt dataLength( aData.Length() );

    if ( dataLength >= KHeaderSize )
        {
        LOG( SIMPLE, "SATENGINE: CSatSColorIconConverter::CheckDataValidityL \
             dataLength bigger than KHeaderSize" )
        const TInt width( aData[0] );
        const TInt height( aData[1] );
        const TInt bpp( aData[2] );

        if ( dataLength < ( width * height * bpp / KPixelSize + KHeaderSize ) )
            {
            LOG( SIMPLE, "SATENGINE: CSatSColorIconConverter::\
            CheckDataValidityL KErrCorrupt" )
            err = KErrCorrupt;
            }
        }

    else
        {
        LOG( SIMPLE, "SATENGINE: CSatSColorIconConverter::CheckDataValidityL \
             dataLength less than KHeaderSize" )
        err = KErrCorrupt;
        }

    User::LeaveIfError( err );
    LOG( SIMPLE,
        "SATENGINE: CSatSColorIconConverter::CheckDataValidityL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSColorIconConverter::CheckClutValidityL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatSColorIconConverter::CheckClutValidityL(
    const TDesC8& aData,
    const TDesC8& aClut ) const
    {
    LOG( SIMPLE,
        "SATENGINE: CSatSColorIconConverter::CheckClutValidityL calling" )
    const TInt clutEntries( aData[3] );
    const TInt expectedClutLength( clutEntries * KClutEntrySize );

    if ( aClut.Length() < expectedClutLength )
        {
        LOG( SIMPLE,
        "SATENGINE: CSatSColorIconConverter::CheckClutValidityL KErrCorrupt" )
        User::Leave( KErrCorrupt );
        }

    LOG( SIMPLE,
        "SATENGINE: CSatSColorIconConverter::CheckClutValidityL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSColorIconConverter::ConvertL
// Converts color icon to CFbsBitamp.
//
// aData has following format( ETSI TS 131 102 V4.10.0 ).
//  1 byte:         Icon width
//  2 byte:         Icon height
//  3 byte:         Bits per raster image point. (1 to 8)
//  7 to end byte:  Image body.
//
// The image body has following format.
//
//   Byte 1     Byte 2
//  1001 1100 1110 1100 ....
//
// If the Bits per pixel ratio is 3 then
// 1st raster point CLUT reference is 100
// 2nd ------------ "" -------------- 111
// 3rd ------------ "" -------------- 001
// 4th ------------ "" -------------- 110
// etc...
//
// One CLUT entry is 3 bytes and each byte defines the intensity of color (RGB).
// The CLUT is in following format:
// 1st CLUT entry
//      1 byte Red color intensity
//      2 byte Green color intensity
//      3 byte Blue color intensity
// 2nd CLUT entry
//      4 byte Red color intensity
//      5 byte Green color intensity
//      6 byte Blue color intensity
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CFbsBitmap* CSatSColorIconConverter::ConvertL(
    const TDesC8& aData,
    const TDesC8& aClut ) const
    {
    LOG( SIMPLE, "SATENGINE: CSatSColorIconConverter::ConvertL calling" )

    // Check that data is valid.
    CheckDataValidityL( aData );
    CheckClutValidityL( aData, aClut );

    const TUint8 width( aData[0] );
    const TUint8 height( aData[1] );

    // Create the bitmap
    CFbsBitmap* bitmap = CreateBitmapL( width, height, EColor16M );

    // Separate the header from image body.
    const TPtrC8 iconData(
        aData.Mid( KHeaderSize, aData.Length() - KHeaderSize ) );

    const TUint8 bpp( aData[2] );
    TBitmapUtil bitmapUtil( bitmap );
    bitmapUtil.Begin( TPoint( 0, 0 ) );
    TInt x(0);
    TInt y(0);
    // Go through the image body to convert the data.
    for ( y = 0; y < height; y++ )
        {
        bitmapUtil.SetPos( TPoint( 0, y ) );

        for ( x = 0; x < width; x++ )
            {
            // Calculate the starting bit index of current pixel
            // location.
            const TInt startBit( ( y * width + x ) * bpp );

            // Get the entry from icon data.
            const TUint8 clutReference(
                TSatBitOP::ExtractBits( startBit, bpp, iconData ) );

            // clut entry is 3 bytes long and therefore the start of
            // the clut entry is multiplied with 3.
            const TInt clutEntryPoint( clutReference * KClutEntrySize );

            // Take the color intensities from clut entry.
            const TUint red( aClut[clutEntryPoint] );
            const TUint green( aClut[clutEntryPoint + 1] );
            const TUint blue( aClut[clutEntryPoint + 2] );

            // Cannot use TRgb here, because of different interpration of
            // r,g,b order than bitmapUtil.
            bitmapUtil.SetPixel(
                red << KRedShift | green << KGreenShift | blue );

            bitmapUtil.IncXPos();
            }
        }
    LOG3( SIMPLE,"SATENGINE: CSatSColorIconConverter::ConvertL, \
          x: %d, y: %d", x, y )
    bitmapUtil.End();

    LOG( SIMPLE, "SATENGINE: CSatSColorIconConverter::ConvertL exiting" )
    return bitmap;
    }
