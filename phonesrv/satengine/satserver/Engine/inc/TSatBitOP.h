/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Extracts a bits from byte row.
*
*/



#ifndef TSATBITOP_H
#define TSATBITOP_H

//  INCLUDES
#include <e32std.h>

// CLASS DECLARATION

/**
*  Bit manipulations
*
*  @lib SatEngine.lib
*  @since 2.6
*/
class TSatBitOP
    {
    public: // New functions

        /**
        * Extracts bits from byte row.
        * Following assumptions is made about parameters:
        *  aBitCount is 1 to 8.
        * @param aBitIndex Index to bit, where extraction starts. Index of
        * MSB from aBytes[0] is 0.
        * @param aBitCount Number of extracted bits.
        * @param aBytes Array of bytes where bits are extracted.
        * @return Value of extracted bits.
        */
        static TUint8 ExtractBits(
            TInt aBitIndex,
            TUint8 aBitCount,
            const TDesC8& aBytes );

        /**
        * Creates a mask of ones, starting from aBitIndex
        * to aBitIndex + aCount.
        * if aMSB is 5 and aCount is 2 then the mask would be
        * 0011 0000.
        * @param aBitIndex. 0 means MSB and 7 is LSB.
        * @param aCount 1 to 8.
        * @return Mask.
        */
        static TUint8 CreateMask8Bit(
            TUint8 aBitIndex,
            TUint8 aCount );

        /**
        * Creates a mask of ones, starting from aBitIndex to aBitIndex +
        * aCount.
        * @param aBitIndex 0 means MSB and 15 means LSB.
        * @param aCount 1 to 16.
        * @return Mask of ones.
        */
        static TUint16 CreateMask16Bit(
            TUint8 aBitIndex,
            TUint8 aCount );

    };

#endif      // TSATBITOP_H

// End of File
