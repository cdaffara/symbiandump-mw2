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
* Description:  Base class for converters.
*
*/


#include    <fbs.h>
#include    "CSatSIconConverter.h"
#include    "CSatSBasicIconConverter.h"
#include    "CSatSColorIconConverter.h"
#include    "SatLog.h"

// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// CSatSIconConverter::CSatSIconConverter
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSatSIconConverter::CSatSIconConverter()
    {
    LOG( SIMPLE,
        "SATENGINE: CSatSIconConverter::CSatSIconConverter calling - exiting" )
    }

// -----------------------------------------------------------------------------
// CSatSIconConverter::CreateConverterL
// Factory method for creating converter.
// -----------------------------------------------------------------------------
//
CSatSIconConverter* CSatSIconConverter::CreateConverterL(
    const RSat::TImageCoding& aCoding )
    {
    LOG( SIMPLE, "SATENGINE: CSatSIconConverter::CreateConverterL calling" )

    CSatSIconConverter* converter = NULL;
    LOG2( SIMPLE, "SATENGINE: CSatSIconConverter::CreateConverterL \
          aCoding: %d", aCoding )
    switch ( aCoding )
        {
        case RSat::KBasic:
            {
            converter = new( ELeave )CSatSBasicIconConverter();
            break;
            }

        case RSat::KColour:
            {
            converter = new( ELeave )CSatSColorIconConverter();
            break;
            }

        default:
            {
            User::Leave( KErrNotSupported );
            }
        }

    LOG( SIMPLE, "SATENGINE: CSatSIconConverter::CreateConverterL exiting" )
    return converter;
    }

// -----------------------------------------------------------------------------
// CSatSIconConverter::CreateBitmapL
// Creates the bitmap.
// -----------------------------------------------------------------------------
//
CFbsBitmap* CSatSIconConverter::CreateBitmapL(
    TUint8 aWidth,
    TUint8 aHeight,
    TDisplayMode aDisplayMode ) const
    {
    LOG( SIMPLE, "SATENGINE: CSatSIconConverter::CreateBitmapL calling" )

    CFbsBitmap* bitmap = new( ELeave )CFbsBitmap();
    CleanupStack::PushL( bitmap );
    User::LeaveIfError(
        bitmap->Create( TSize( aWidth, aHeight ), aDisplayMode ) );
    CleanupStack::Pop( bitmap );

    LOG( SIMPLE, "SATENGINE: CSatSIconConverter::CreateBitmapL exiting" )
    return bitmap;
    }
