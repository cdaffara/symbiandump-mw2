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
* Description:  Bit manipulator.
*
*/


#include    "TSatBitOP.h"
#include    "SatLog.h"

const TUint8 KBitsInByte( 8 );
const TUint8 KBitsInWord( 16 );
const TUint8 KHighBitInByte( 7 );
const TUint8 KHighBitInWord( 15 );

// -----------------------------------------------------------------------------
// Extracts bits from bit sequence. First bit is 0 index.
// The lsb of extracted bits is set to 0 bit.
// -----------------------------------------------------------------------------
//
TUint8 TSatBitOP::ExtractBits(
    TInt aBitIndex,
    TUint8 aBitCount,
    const TDesC8& aBytes )
    {
    LOG( DETAILED, "SATENGINE: TSatBitOP::ExtractBits calling" )

    // Calculate the byte where aBitIndex is pointing.
    const TInt byteIndex( static_cast<TInt>( aBitIndex / KBitsInByte ) );
    if ( byteIndex > 0 ) // aBitIndex is pointing other than first byte.
        {
        LOG( DETAILED, "SATENGINE: TSatBitOP::ExtractBits byteIndex > 0" )
        // Change the aBitIndex to point the bits 0 to 7, because
        // we know which byte it is indexing.
        aBitIndex -= byteIndex * KBitsInByte;
        }

    TUint8 result( 0 );
    // Check if the bits to be extracted is in two bytes.
    // ie. Part of the bits is in one byte and another
    // part is in the following byte.
    const TUint8 lastBit( static_cast<TUint8>( aBitIndex + aBitCount ) );
    LOG2( DETAILED, "SATENGINE: TSatBitOP::ExtractBits lastBit: %i", lastBit )
    if ( lastBit > KBitsInByte )
        {
        // The bits to be extracted reside in two bytes.

        // Take the bytes.
        const TUint16 word( static_cast<TUint16>(
            ( aBytes[byteIndex] << KBitsInByte ) + aBytes[byteIndex + 1] ) );

        // Create the mask, which extracts the bits.
        const TUint16 mask(
            CreateMask16Bit( static_cast<TUint8>( aBitIndex ), aBitCount ) );

        // Extract the bits from the word.
        // Result will be shifted so that the lsb bit of the extracted bits
        // will be in bit 0.
        result =
            static_cast<TUint8>( ( word & mask ) >> ( KBitsInWord - lastBit ) );
        }
    else
        {
        // Bits can be extracted from single byte.

        // Create the mask.
        const TUint8 mask(
            CreateMask8Bit( static_cast<TUint8>( aBitIndex ), aBitCount ) );

        const TUint8 byte( aBytes[byteIndex] );

        // Mask the bits and move the lsb of the extracted bits to 0 bit.
        // Result will be shifted so that the lsb bit of the extracted bits
        // will be in bit 0.
        result =
            static_cast<TUint8>( ( byte & mask ) >> ( KBitsInByte - lastBit ) );
        }

    LOG( DETAILED, "SATENGINE: TSatBitOP::ExtractBits exiting" )
    return result;
    }

// -----------------------------------------------------------------------------
// Creates a 8-bit mask of ones.
// -----------------------------------------------------------------------------
//
TUint8 TSatBitOP::CreateMask8Bit( TUint8 aBitIndex, TUint8 aCount )
    {
    LOG( DETAILED, "SATENGINE: TSatBitOP::CreateMask8Bit calling" )

    // Change aBitIndex to use normal indexing of bits, because
    // 0 in aBitIndex means MSB.
    aBitIndex = static_cast<TUint8>( KHighBitInByte - aBitIndex );

    TUint8 result( 0 );
    TInt i ( 0 );
    for ( i = 0; i < aCount; i++ )
        {
        result |= static_cast<TUint8>( 1 << aBitIndex );
        aBitIndex--;
        }
    LOG2( DETAILED, "SATENGINE: TSatBitOP::CreateMask8Bit i: %i", i )
    LOG2( DETAILED, "SATENGINE: TSatBitOP::CreateMask8Bit exiting, \
          result: %i", result )
    return result;
    }

// -----------------------------------------------------------------------------
// Creates a 16-bit mask of ones.
// -----------------------------------------------------------------------------
//
TUint16 TSatBitOP::CreateMask16Bit( TUint8 aBitIndex, TUint8 aCount )
    {
    LOG( DETAILED, "SATENGINE: TSatBitOP::CreateMask16Bit calling" )

    // Change aBitIndex to use normal indexing of bits, because
    // 0 in aBitIndex means MSB.
    aBitIndex = static_cast<TUint8>( KHighBitInWord - aBitIndex );

    TUint16 result( 0 );
    TInt i ( 0 );
    for ( i = 0;  i < aCount; i++ )
        {
        result |= static_cast<TUint16>( 1 << aBitIndex );
        aBitIndex--;
        }
    LOG2( DETAILED, "SATENGINE: TSatBitOP::CreateMask16Bit i: %i", i )
    LOG2( DETAILED, "SATENGINE: TSatBitOP::CreateMask16Bit exiting, \
          result: %i", result )
    return result;
    }
