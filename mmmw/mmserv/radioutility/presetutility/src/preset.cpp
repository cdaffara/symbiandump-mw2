/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
*
*
*/


//  INCLUDES
#include <s32mem.h> // RWriteStream, RReadStream

#include <preset.h>
#include "trace.h"

/**
 * On/Off type flags
 *
 * Bit shifting in the value initialization is used to easily create
 * progressing bit patterns like so:
 *  1 << 0 equals 0001 equals 0x1
 *  1 << 1 equals 0010 equals 0x2
 *  1 << 2 equals 0100 equals 0x4
 *  1 << 3 equals 1000 equals 0x8
 */
enum TFlagValues
    {
     EFavorite       = 1 << 0
    ,ELocalStation   = 1 << 1
    ,ERenamedByUser  = 1 << 2
    };

// ============== LOCAL FUNCTIONS ===============

/**
 * Normalize return value from TBitFlags to ETrue or EFalse values
 * Two negations: 16 => 0 => 1
 */
static TBool NormalizeBool( TBool aBool )
    {
    return !( !aBool );
    }

const TUint8 KNbrOfMyExternalizedUint32Items = 3;

// ============= MEMBER FUNCTIONS ==============

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
EXPORT_C TPreset::TPreset()
    : iPresetFrequency( 0 )
    {
    FUNC_LOG;
    }

// ---------------------------------------------------------------------------
// Returns the channel frequency
// ---------------------------------------------------------------------------
//
EXPORT_C TUint TPreset::Frequency() const
    {
    FUNC_LOG;
    return iPresetFrequency;
    }

// ---------------------------------------------------------------------------
// Returns the channel name
// ---------------------------------------------------------------------------
//
EXPORT_C TPtrC TPreset::Name() const
    {
    FUNC_LOG;
    return TPtrC( iPresetName );
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
EXPORT_C TPtrC TPreset::Url() const
    {
    FUNC_LOG;
    return TPtrC( iUrl );
    }


// ---------------------------------------------------------------------------
// Sets the channel name
// ---------------------------------------------------------------------------
//
EXPORT_C void TPreset::SetName( const TPresetName& aStationName )
    {
    FUNC_LOG;
    iPresetName = aStationName;
    }

// ---------------------------------------------------------------------------
// Sets the channel frequency
// ---------------------------------------------------------------------------
//
EXPORT_C void TPreset::SetFrequency( const TUint aFrequency )
    {
    FUNC_LOG;
    iPresetFrequency = aFrequency;
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
EXPORT_C void TPreset::SetUrl( const TRadioUrl& aUrl )
    {
    FUNC_LOG;
    iUrl = aUrl;
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
EXPORT_C void TPreset::SetFavorite( TBool aFavorite )
    {
    FUNC_LOG;
    iFlags.Assign( EFavorite, aFavorite );
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
EXPORT_C TBool TPreset::Favorite() const
    {
    FUNC_LOG;
    // Normalize return value from TBitFlags to 1 or 0 just in case
    // Two negations: 16 => 0 => 1
    return NormalizeBool( iFlags.IsSet( EFavorite ) );
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
EXPORT_C void TPreset::SetLocalStation( TBool aLocalStation )
    {
    FUNC_LOG;
    iFlags.Assign( ELocalStation, aLocalStation );
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
EXPORT_C TBool TPreset::LocalStation() const
    {
    FUNC_LOG;
    // Normalize return value from TBitFlags to 1 or 0 just in case
    // Two negations: 16 => 0 => 1
    return NormalizeBool( iFlags.IsSet( ELocalStation ) );
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
EXPORT_C void TPreset::SetRenamedByUser( TBool aRenamed )
    {
    FUNC_LOG;
    iFlags.Assign( ERenamedByUser, aRenamed );
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
EXPORT_C TBool TPreset::RenamedByUser() const
    {
    FUNC_LOG;
    // Normalize return value from TBitFlags to 1 or 0 just in case
    // Two negations: 16 => 0 => 1
    return NormalizeBool( iFlags.IsSet( ERenamedByUser ) );
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
EXPORT_C void TPreset::SetGenre( TInt aGenre )
    {
    FUNC_LOG;
    iGenre = aGenre;
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
EXPORT_C TInt TPreset::Genre() const
    {
    FUNC_LOG;
    return iGenre;
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
EXPORT_C void TPreset::SetPiCode( TInt aPiCode )
    {
    FUNC_LOG;
    iPiCode = aPiCode;
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
EXPORT_C TInt TPreset::PiCode() const
    {
    FUNC_LOG;
    return iPiCode;
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
EXPORT_C TUint TPreset::MyExternalizedDataSize() const
    {
    FUNC_LOG;
    return ( KNbrOfMyExternalizedUint32Items * sizeof(TUint32) );
    }
// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
EXPORT_C void TPreset::ExternalizeL( RWriteStream& aStream ) const
    {
    FUNC_LOG;
    aStream.WriteUint32L( iGenre );
    aStream.WriteUint32L( iPiCode );
    aStream.WriteUint32L( iFlags.Value() );

    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
EXPORT_C void TPreset::InternalizeL( RReadStream& aStream )
    {
    FUNC_LOG;
    iGenre = aStream.ReadUint32L();
    iPiCode = aStream.ReadUint32L();
    iFlags = TBitFlags( aStream.ReadUint32L() );

    }
