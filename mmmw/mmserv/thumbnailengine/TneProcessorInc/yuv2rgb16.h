/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   YUV to EColor64K colorspace converter concrete classes 
*
*/



#ifndef     __YUV2RGB16_H__
#define     __YUV2RGB16_H__

//  INCLUDES

#ifndef __YUVCONVERTER_H__
#include "yuvconverter.h"
#endif

//  CLASS DEFINITIONS

class CYuv2Rgb16 : public CYuvConverter
{
public: // CYuvConverter methods
    // Constructors & destructor
    CYuv2Rgb16();
    ~CYuv2Rgb16();
    void ConstructL(TUint aWidth, TUint aHeight, TUint aMaxWidth, TUint aMaxHeight);
    
    void Convert(const TUint8 *aYBuf, const TUint8 *aUBuf,
                 const TUint8 *aVBuf,
                 TUint aBufWidth, TUint aBufHeight,
                 TUint8 *aTarget, TUint aTargetScanlineLength);

    void SetGamma(TInt /*aGamma*/) {};

protected: // Data
    
    TUint iWidth, iHeight;
    
};


// The fixed point constants for YUV -> RGB conversion
const TInt KYMult = 296;
const TInt KCrToR = 408;
const TInt KCbToG =  96;
const TInt KCrToG = 204;
const TInt KCbToB = 520;

// -----------------------------------------------------------------------------
// UvToRDiff
// Calculate the red color difference from Cr by using fixed point values.
// -----------------------------------------------------------------------------
//
inline TInt UvToRDiff(
    TInt aCr,
    TInt /*aCb*/)
    {
    return (KCrToR * aCr) / 256;
    }

// -----------------------------------------------------------------------------
// UvToGDiff
// Calculate the green color difference from Cr and Cb components
// by using fixed point values.
// -----------------------------------------------------------------------------
//
inline TInt UvToGDiff(
    TInt aCr,
    TInt aCb)
    {
    return (KCbToG * aCb + KCrToG * aCr) / 256;
    }

// -----------------------------------------------------------------------------
// UvToBDiff
// Calculate the blue color difference from Cb by using fixed point values.
// -----------------------------------------------------------------------------
//
inline TInt UvToBDiff(
    TInt /*aCr*/,
    TInt aCb)
    {
    return (KCbToB * aCb) / 256;
    }


#endif
