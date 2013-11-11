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
* Description:  MnrpUtils class definition
*
*/


#ifndef MNRP_UTILS_H
#define MNRP_UTILS_H

const TInt KMaxCoordStrSize = 15;

class MnrpUtils
    {
    public:
    
        /** Converts given coordinate into a string 
         *  \param aCoord coordinate
         *  \param aStr target string of KMaxCoordStrSize length (at least)
         *  \param aFull if EFalse, will omit higher parts if they are 0,
         *               if ETrue, outputs degrees, minutes, seconds and milliseconds
         *  \param aMillis whether milliseconds should be included or not
         */
        IMPORT_C static void CoordinateToString( 
            TReal aCoord, 
            TDes& aStr, 
            TBool aFull = EFalse,
            TBool aMillis = EFalse );
            
        /** Converts given distance value to a string with units
         *  \param aDistance distance in meters 
         *  \param aStr target string
         */
        IMPORT_C static void DistanceToString( TReal aDistance, TDes& aStr );

        /** Converts given angle value to a string
         *  \param aAngle angle in degrees (angle form North Pole) 
         *  \param aStr target string
         */
        IMPORT_C static void AngleToString( TReal aAngle, TDes& aStr );

        /** Converts given speed value to a string with units
         *  \param aSpeed speed in meters per second
         *  \param aStr target string
         */
        IMPORT_C static void SpeedToString( TReal aSpeed, TDes& aStr );

        /** Replaces unprintable characters in a string with spaces
         *  \param aStr string to work with
         *  \return pointer to new string buffer with ownership
         */
        IMPORT_C static HBufC* PrintableStringLC( const TDesC& aStr );
    };

#endif // MNRP_UTILS_H

