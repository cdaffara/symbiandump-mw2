/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  MnrpUtils class implementation
*
*/


#include <e32base.h>
#include <e32math.h>
#include <lbsposition.h>

#include "mnrputils.h"

_LIT( KNan, "<n/a>" );
_LIT( KNeg, "<neg>" );
_LIT( KOutOfBounds, "<inv>" );

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C void MnrpUtils::CoordinateToString(
    TReal aCoord,
    TDes& aStr,
    TBool aFull,
    TBool aMillis )
    {
    const TChar KDegreeMark = 0xB0;
    const TChar KMinuteMark = 0x27;
    const TChar KSecondMark = 0x22;
    const TChar KMilliSecondMark = 0x2E;
    const TChar KMinus = '-';
    const TChar KPlus = '+';

    _LIT( KCoordFormatFull, "%c%d%c%02d%c%02d%c" );
    _LIT( KCoordFormatPartMseconds, "%c%03d" );

    if ( Math::IsNaN( aCoord ) )
        {
        aStr.Copy( KNan );
        }
    else if ( Abs( aCoord ) > 360 )
        {
        aStr.Copy( KOutOfBounds );
        }
    else
        {
        TBool negative = aCoord < 0;
        aCoord = Abs( aCoord );

        TInt degrees = TInt( aCoord );
        aCoord = ( aCoord - degrees ) * 60;
        TInt minutes = TInt( aCoord );
        aCoord = ( aCoord - minutes ) * 60;
        TInt seconds = TInt( aCoord );
        aCoord = ( aCoord - seconds ) * 100;
        TInt mseconds = TInt( aCoord );

        TChar sign = negative ? KMinus : KPlus;

        if ( aFull )
            {
            aStr.Format( KCoordFormatFull,
                            TUint( sign ), degrees, TUint( KDegreeMark ),
                            minutes, TUint( KMinuteMark ),
                            seconds, TUint( KSecondMark ) );
            if ( aMillis )
                {
                aStr.AppendFormat( KCoordFormatPartMseconds, TUint( KMilliSecondMark ), mseconds );
                }
            }
        else
            {
            aStr.Zero();
            if ( degrees )
                {
                aStr.AppendNum( degrees );
                aStr.Append( KDegreeMark );
                }
            if ( minutes )
                {
                aStr.AppendNum( minutes );
                aStr.Append( KMinuteMark );
                }
            if ( seconds )
                {
                aStr.AppendNum( seconds );
                aStr.Append( KSecondMark );
                }
            if ( mseconds && aMillis )
                {
                if ( !seconds )
                    {
                    aStr.AppendNum( 0 );
                    aStr.Append( KSecondMark );
                    }
                aStr.AppendFormat( KCoordFormatPartMseconds, TUint( KMilliSecondMark ), mseconds );
                }

            if ( !aStr.Length() )
                {
                aStr.AppendNum( degrees );
                aStr.Append( KDegreeMark );
                }
            }
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C void MnrpUtils::DistanceToString( TReal aDistance, TDes& aStr )
    {
    _LIT( KSmall, "< 1 m" );
    _LIT( KMeters, "%d m" );
    _LIT( KKiloMeters, "%.1f Km" );
    _LIT( KThousandsKiloMeters, "%.1f tKm" );

    const TInt KMeter = 1;
    const TInt KKilometer = KMeter * 1000;
    const TInt KThousandKilometer = KKilometer * 1000;

    if ( Math::IsNaN( aDistance ) )
        {
        aStr.Copy( KNan );
        }
    else if ( aDistance < 0 )
        {
        aStr.Copy( KNeg );
        }
    else if ( aDistance < KMeter )
        {
        aStr.Copy( KSmall );
        }
    else if ( aDistance < KKilometer )
        {
        aStr.Format( KMeters, TInt( aDistance ) );
        }
    else if ( aDistance < KThousandKilometer )
        {
        aStr.Format( KKiloMeters, aDistance / KKilometer );
        }
    else
        {
        aStr.Format( KThousandsKiloMeters, aDistance / KThousandKilometer );
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C void MnrpUtils::SpeedToString( TReal aSpeed, TDes& aStr )
    {
    _LIT( KPerSecond, "/s" );

    if ( Math::IsNaN( aSpeed ) )
        {
        aStr.Copy( KNan );
        }
    else if ( aSpeed < 0 )
        {
        aStr.Copy( KNeg );
        }
    else
        {
        DistanceToString( aSpeed, aStr );
        aStr.Append( KPerSecond );
        }
    }
    
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C void MnrpUtils::AngleToString( TReal aAngle, TDes& aStr )
    {
    const TChar KMinus = '-';
    const TChar KPlus = '+';
    _LIT( KAngleFormat, "%S%c%.1f\xB0 /  %.1f\xB0" );
    
    _LIT( KNorth, "N" );
    _LIT( KSouth, "S" );
    _LIT( KWest, "W" );
    _LIT( KEast, "E" );

    TPtrC side;
    TReal angle;

    if ( Math::IsNaN( aAngle ) )
        {
        aStr.Copy( KNan );
        }
    else if ( aAngle > 360 )
        {
        aStr.Copy( KOutOfBounds );
        }
    else
        {
        if ( aAngle > 315 )
            {
            side.Set( KNorth );
            angle = aAngle - 360;
            }
        else if ( aAngle > 225 )
            {
            side.Set( KWest );
            angle = aAngle - 270;
            }
        else if ( aAngle > 135 )
            {
            side.Set( KSouth );
            angle = aAngle - 180;
            }
        else if ( aAngle > 45 )
            {
            side.Set( KEast );
            angle = aAngle - 90;
            }
        else // aAngle <= 45
            {
            side.Set( KNorth );
            angle = aAngle;
            }

        TChar sign = ( angle >= 0 ) ? KPlus : KMinus;
        aStr.Format( KAngleFormat, &side, TUint( sign ), Abs( angle ), aAngle );
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C HBufC* MnrpUtils::PrintableStringLC( const TDesC& aStr )
    {
    HBufC* buf = aStr.AllocLC();
    TPtr string( buf->Des() );
    for ( TInt i = 0; i < string.Length(); i++ )
        {
        if ( !TChar( string[i] ).IsPrint() )
            {
            string[i] = ' ';
            }
        }
    return buf;
    }
