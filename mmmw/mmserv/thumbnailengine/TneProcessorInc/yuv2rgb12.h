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
* Description:   YUV to EColor4K colorspace converter concrete classes 
*
*/


    

#ifndef     __YUV2RGB12_H__
#define     __YUV2RGB12_H__


//  INCLUDES

#ifndef __YUVCONVERTER_H__
#include "yuvconverter.h"
#endif


//  CLASS DEFINITIONS

// Normal YUV to Rgb12 with dither
class CYuv2Rgb12 : public CYuvConverter
{
public: // CYuvConverter methods
    // Constructors & destructor
    CYuv2Rgb12();
    ~CYuv2Rgb12();
    void ConstructL(TUint aWidth, TUint aHeight, TUint aMaxWidth, TUint aMaxHeight);
    void SetGamma(TInt aGamma);
    void SetBrightnessContrast(TInt aBCIndex);
    void Convert(const TUint8 *aYBuf, const TUint8 *aUBuf,
                 const TUint8 *aVBuf,
                 TUint aBufWidth, TUint aBufHeight,
                 TUint8 *aTarget, TUint aTargetScanlineLength);

protected: // Data
    enum
    {
        ESaturateOffset = 222,    //saturation table offset; min value (when U == 0, Y == 0) is -222 
        ESaturateLength = 476+12+ESaturateOffset  //table length; max value (when U == 255, Y == 255) is 476+12(dither)
    };
    TUint8 *iRgbLookupTable; // RGB clamp/gamma lookup table
    TUint iWidth, iHeight, iCropWidth, iCropHeight;
    TInt iGamma;
    TInt iBrightnessContrast;
};




#endif      //  __YUV2RGB12_H__
            
// End of File
