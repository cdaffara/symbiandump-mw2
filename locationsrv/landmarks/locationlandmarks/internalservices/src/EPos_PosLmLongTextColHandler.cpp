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
* Description: Provides functions to read and write from/to long text columns.
*
*
*/


#include <s32buf.h>
#include "EPos_PosLmLongTextColHandler.h"

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C void PosLmLongTextColHandler::ReadFromLongTextColumnL(
    TDes& aLongText,
    RDbRowSet& aView,
    TDbColNo aCol)
    {
    aLongText.Zero();

    RDbColReadStream readStream;
    readStream.OpenLC(aView, aCol);
    readStream.ReadL(aLongText, readStream.Source()->SizeL() / 2);
    // Size is in bytes and not characters. Assume 16-bit environment.
    readStream.Close(); // This close call cannot be removed.

    CleanupStack::PopAndDestroy(&readStream);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C void PosLmLongTextColHandler::WriteToLongTextColumnL(
    const TDesC& aLongText,
    RDbRowSet& aView,
    TDbColNo aCol)
    {
    RDbColWriteStream writeStream;
    writeStream.OpenLC(aView, aCol);
    writeStream.WriteL(aLongText);
    writeStream.Close(); // This close call cannot be removed.

    CleanupStack::PopAndDestroy(&writeStream);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C HBufC* PosLmLongTextColHandler::ReadFromLongTextColumnLC(
    RDbRowSet& aView,
    TDbColNo aCol )
    {
    TInt len = aView.ColLength( aCol );
    if ( len > 0 )
        {
        HBufC* text = HBufC::NewLC( len );
        TPtr ptr = text->Des();

        RDbColReadStream readStream;
        readStream.OpenL( aView, aCol );
        CleanupClosePushL( readStream );
        readStream.ReadL( ptr, len );
        CleanupStack::PopAndDestroy( &readStream );

        return text;
        }
    else
        {
        return KNullDesC().AllocLC();
        }
    }

