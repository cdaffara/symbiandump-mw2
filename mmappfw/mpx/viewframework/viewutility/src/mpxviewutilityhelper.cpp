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
* Description:  Implementation of view utility helper
*
*/



// INCLUDE FILES
#include "mpxviewutilityhelper.h"

// CONSTANTS
_LIT8( KMPXDelimiter8, ";" );
_LIT8( KMPXHexMarker8, "0x" );

_LIT( KMPXDelimiter, ";" );
_LIT( KMPXHexMarker, "0x" );

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Extracts number from data. ( 8 bit )
// ---------------------------------------------------------------------------
//
TInt CMPXViewUtilityHelper::ExtractNum(
    const TDesC8& aData,
    TInt& aNextPos,
    TInt& aResult )
    {
    if ( aNextPos < 0 || aNextPos > aData.Length() )
        {
        aNextPos = KErrNotFound;
        return KErrArgument;
        }

    TInt err( KErrNone );
    TPtrC8 string( aData.Mid( aNextPos ) );

    if ( string.Length() )
        {
        TInt delimiterPos = string.Find( KMPXDelimiter8 );
        if ( KErrNotFound != delimiterPos )
            {
            string.Set( aData.Mid( aNextPos, delimiterPos ) );
            aNextPos += delimiterPos + KMPXDelimiter8().Length();
            if ( aNextPos >= aData.Length() )
                {
                // Reach the end of string
                aNextPos = KErrNotFound;
                }
            }
        else
            {
            // Reach the end of string
            aNextPos = KErrNotFound;
            }

        // Convert string to integer
        TInt hexMarkerPos = string.Find( KMPXHexMarker8 );
        if ( KErrNotFound != hexMarkerPos )
            {
            TUint numHex( 0 );
            TLex8 lexHex( string.Mid( KMPXHexMarker8().Length() ) );
            err = lexHex.Val( numHex, EHex );
            aResult = numHex;
            }
        else
            {
            TLex8 lexDec( string );
            err = lexDec.Val( aResult );
            }
        }
    else
        {
        aNextPos = KErrNotFound;
        err = KErrNotFound;
        }

    return err;
    }

// ---------------------------------------------------------------------------
// Extracts number from data. ( generic )
// ---------------------------------------------------------------------------
//
TInt CMPXViewUtilityHelper::ExtractNum(
    const TDesC& aData,
    TInt& aNextPos,
    TInt& aResult )
    {
    if ( aNextPos < 0 || aNextPos > aData.Length() )
        {
        aNextPos = KErrNotFound;
        return KErrGeneral;
        }

    TInt err( KErrNone );
    TPtrC string( aData.Mid( aNextPos ) );

    if ( string.Length() )
        {
        TInt delimiterPos = string.Find( KMPXDelimiter );
        if ( KErrNotFound != delimiterPos )
            {
            string.Set( aData.Mid( aNextPos, delimiterPos ) );
            aNextPos += delimiterPos + KMPXDelimiter().Length();
            if ( aNextPos >= aData.Length() )
                {
                // Reach the end of string
                aNextPos = KErrNotFound;
                }
            }
        else
            {
            // Reach the end of string
            aNextPos = KErrNotFound;
            }

        // Convert string to integer
        TInt hexMarkerPos = string.Find( KMPXHexMarker );
        if ( KErrNotFound != hexMarkerPos )
            {
            TUint numHex( 0 );
            TLex lexHex( string.Mid( KMPXHexMarker().Length() ) );
            err = lexHex.Val( numHex, EHex );
            aResult = numHex;
            }
        else
            {
            TLex lexDec( string );
            err = lexDec.Val( aResult );
            }
        }
    else
        {
        aNextPos = KErrNotFound;
        err = KErrNotFound;
        }

    return err;
    }

//  End of File
