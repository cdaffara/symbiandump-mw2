/*
* Copyright (c) 2002-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Capsulates the icon info from the SIM.
*
*/


// INCLUDE FILES
#include    <etelsat.h>
#include    <tsaticoninfo.h>

#include    "SatLog.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// TSatIconInfo::TSatIconInfo
// C++ default constructor can NOT contain any code, that
// might leave.
// The aIconDescription has to be in following format: (ETSI TS 131 102 V4.10.0)
//      1 byte: Icon width
//      2 byte: Icon height
//      3 byte: Image coding scheme
//      4 byte: High byte of file identifier
//      5 byte: Low byte of file identifier.
//      6 byte: High byte of offset of the icon in EF.
//      7 byte: Low byte of offset of the icon in EF.
//      8 byte: High byte of icon length.
//      9 byte: Low byte of icon lenght.
// -----------------------------------------------------------------------------
//

TSatIconInfo::TSatIconInfo() :
    iSize(),
    iCodingScheme( static_cast<TCodingScheme>( 0 ) ),
    iFileIdentifier( 0 ),
    iOffset( 0 ),
    iLength( 0 )
    {
    LOG( SIMPLE, "SATCLIENT: TSatIconInfo::TSatIconInfo calling-exits" )
    }

// -----------------------------------------------------------------------------
// TSatIconInfo::IconSize
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TSize TSatIconInfo::IconSize() const
    {
    LOG( SIMPLE, "SATCLIENT: TSatIconInfo::IconSize calling-exits" )    
    return iSize;
    }

// -----------------------------------------------------------------------------
// TSatIconInfo::CodingScheme
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TSatIconInfo::TCodingScheme TSatIconInfo::CodingScheme() const
    {
    LOG( SIMPLE, "SATCLIENT: TSatIconInfo::CodingScheme calling-exits" )
    return iCodingScheme;
    }

// -----------------------------------------------------------------------------
// TSatIconInfo::FileIdentifier
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TUint16 TSatIconInfo::FileIdentifier() const
    {
    LOG2( SIMPLE, 
    "SATCLIENT: TSatIconInfo::FileIdentifier calling-exits, iFileIdentifier: %i", 
    iFileIdentifier )
    return iFileIdentifier;
    }

// -----------------------------------------------------------------------------
// TSatIconInfo::Offset
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TUint16 TSatIconInfo::Offset() const
    {
    LOG2( SIMPLE, 
    "SATCLIENT: TSatIconInfo::Offset calling-exits, iOffset: %i", 
    iOffset )
    return iOffset;
    }

// -----------------------------------------------------------------------------
// TSatIconInfo::DataLength
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TUint16 TSatIconInfo::DataLength() const
    {
    LOG2( SIMPLE, 
    "SATCLIENT: TSatIconInfo::DataLength calling-exits, iLength: %i", 
    iLength )
    return iLength;
    }

// -----------------------------------------------------------------------------
// TSatIconInfo::SetIconSize
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TSatIconInfo::SetIconSize( const TSize aIconSize )
    {
    LOG3( SIMPLE, 
    "SATCLIENT: TSatIconInfo::SetIconSize calling-exits aIconSize: x=%i, y=%i", 
    aIconSize.iWidth, aIconSize.iHeight )
    iSize = aIconSize;
    }

// -----------------------------------------------------------------------------
// TSatIconInfo::SetCodingScheme
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TSatIconInfo::SetCodingScheme( const TCodingScheme aCodingScheme )
    {
    LOG2( SIMPLE, 
    "SATCLIENT: TSatIconInfo::SetCodingScheme calling-exits, aCodingScheme: %i",
    aCodingScheme )
    iCodingScheme = aCodingScheme;
    }

// -----------------------------------------------------------------------------
// TSatIconInfo::SetFileIdentifier
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TSatIconInfo::SetFileIdentifier( const TUint16 aFileIdentifier )
    {
    LOG2( SIMPLE, 
    "SATCLIENT: TSatIconInfo::SetFileIdentifier calling-exits, aFileIdentifier: %i", 
    aFileIdentifier )
    iFileIdentifier = aFileIdentifier;
    }

// -----------------------------------------------------------------------------
// TSatIconInfo::SetOffset
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TSatIconInfo::SetOffset( const TUint16 aOffset)
    {
    LOG2( SIMPLE, 
    "SATCLIENT: TSatIconInfo::SetOffset calling-exiting, aOffset: %i", 
    aOffset )
    iOffset = aOffset;
    }

// -----------------------------------------------------------------------------
// TSatIconInfo::SetDataLength
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void TSatIconInfo::SetDataLength( const TUint16 aDataLength )
    {
    LOG2( SIMPLE, 
    "SATCLIENT: TSatIconInfo::SetDataLength calling-exiting aDataLength: %i", 
    aDataLength )
    iLength = aDataLength;
    }

//  End of File
