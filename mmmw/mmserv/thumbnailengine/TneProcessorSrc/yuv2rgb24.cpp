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
* Description:   Implementation of class CYuv2Rgb24.
*                YUV to EColor16M colorspace converter concrete classes 
*
*/





//  EXTERNAL RESOURCES  


//  Include Files  

#include <e32math.h>
#include "yuvconverter.h"
#include "yuv2rgb24.h"
#include "brightnesscontrast.h"


//  MEMBER FUNCTIONS


//=============================================================================

/*
-----------------------------------------------------------------------------

    CYuv2Rgb24

    CYuv2Rgb24()

    Standard C++ constructor

-----------------------------------------------------------------------------
*/

CYuv2Rgb24::CYuv2Rgb24()
{
    iRgbLookupTable = 0;
    iGamma = 65536;
    iBrightnessContrast = KMedBrightnessContrastIndex;

}



/*
-----------------------------------------------------------------------------

    CYuv2Rgb24

    ~CYuv2Rgb24()

    Standard C++ destructor

-----------------------------------------------------------------------------
*/

CYuv2Rgb24::~CYuv2Rgb24()
{
    User::Free(iRgbLookupTable);
}



/*
-----------------------------------------------------------------------------

    CYuv2Rgb24

    ConstructL()

    Standard Symbian OS second-phase constructor. Initializes the object.

-----------------------------------------------------------------------------
*/

void CYuv2Rgb24::ConstructL(TUint aWidth, TUint aHeight, TUint aMaxWidth, TUint aMaxHeight)
{
    // Remember the dimensions
//    __ASSERT_ALWAYS(((aWidth & 1) == 0) && ((aHeight & 1) == 0),
//                    User::Leave(KErrArgument));
    iWidth = aWidth;
    iHeight = aHeight;
    if ( iWidth > aMaxWidth ) {
        iCropWidth = (iWidth-aMaxWidth)/2;
        iWidth = aMaxWidth;
    }
    else {
        iCropWidth = 0;
    }
    if ( iHeight > aMaxHeight ) {
        iCropHeight = (iHeight-aMaxHeight)/2;
        iHeight = aMaxHeight;
    }
    else {
        iCropHeight = 0;
    }

    // Allocate the RGB saturate/gamma lookup table    
    iRgbLookupTable = (TUint8*) User::AllocL(ESaturateLength);

    // Initialize brightness & contrast value, this will calculate the conversion table
    // Since this uses the median index, it makes no difference if the preferred 
    // enhancement is this or gamma. Furthermore, changes to the value will be done using
    // the appropriate method.
    SetBrightnessContrast(KMaxBCInputIndex/2);
}



/*
-----------------------------------------------------------------------------

    CYuv2Rgb24

    SetGamma()

    Sets the conversion gamma value and recalculates the look-up table

-----------------------------------------------------------------------------
*/

void CYuv2Rgb24::SetGamma(TInt aGamma)
{
    TInt i, v;
    TReal vNorm;
    
    // Remember gamma and convert it to floating point
    iGamma = aGamma;
    TReal fGamma = TReal(iGamma) / TReal(65536);

    // Calculate table entries for all possible RGB values:
    for ( i = 0; i < ESaturateLength; i++ )
    {
        // Actual RGB value for this table index
        v = i - ESaturateOffset;

        // Saturate if <0 or >255, otherwise calculate gamma       
        if ( v < 0 )
            v = 0;
        else if ( v > 255 )
            v = 255;
        else
        {
            // Normalize v:
            vNorm = TReal(v) / TReal(255);

            // Gamma-correct: v = v ^ gamma
            Math::Pow(vNorm, vNorm, fGamma);

            // Scale back to [0..255] and clamp:
            vNorm = (TReal(255) * vNorm) + 0.5;
            v = (TInt) vNorm;
            if ( v < 0 ) v = 0;
            if ( v > 255 ) v = 255;
        }

        // 24bpp RGB has range [0..255] for all components, store to table:
        iRgbLookupTable[i] = (TUint8) v;
    }
}

/*
-----------------------------------------------------------------------------

    CYuv2Rgb24

    SetBrightnessContrast()

    Sets the index to the predefined brightness&contrast lookup table 
    (KBrightnessContrastEnhParam) and recalculates the RGB look-up table
    The algorithm was developed by IMAAMI for Kenny display.

-----------------------------------------------------------------------------
*/
void CYuv2Rgb24::SetBrightnessContrast(TInt aBCIndex)
{
    TInt i, v;
    TReal vNorm;
    
    // Convert & remember brightness-contrast index. aBCIndex == 0 to KMaxBCInputIndex.
    iBrightnessContrast = (aBCIndex*KMaxBrightnessContrastIndex)/KMaxBCInputIndex;

    // Calculate table entries for all possible RGB values:
    for ( i = 0; i < ESaturateLength; i++ )
    {
        // Actual RGB value for this table index
        v = 298 * (i - ESaturateOffset - 16) / 256;
        // (see Convert())

        // Saturate if <0 or >255, otherwise calculate value
        if ( v < 0 )
            v = 0;
        else if ( v > 255 )
            v = 255;
        else
        {

            // Normalize v:
            vNorm = TReal(v) / TReal(255);

            vNorm = KBrightnessContrastEnhParam[iBrightnessContrast].a * vNorm + KBrightnessContrastEnhParam[iBrightnessContrast].b;
            if ( vNorm < 0 ) 
                vNorm = 0;
            else if ( vNorm > 1 )
                vNorm = 1;
            Math::Pow( vNorm, vNorm, KBrightnessContrastEnhParam[iBrightnessContrast].g );

            // Scale back to [0..255] and clamp:
            vNorm = (TReal(255) * vNorm) + 0.5;
            v = (TInt) vNorm;
            if ( v < 0 ) v = 0;
            if ( v > 255 ) v = 255;
        }

        // 24bpp RGB has range [0..255] for all components, store to table:
        iRgbLookupTable[i] = (TUint8) v;
    }
}


/*
-----------------------------------------------------------------------------

    CYuv2Rgb24

    Convert()

    Converts a YUV frame to a EColor16M frame

-----------------------------------------------------------------------------
*/

void CYuv2Rgb24::Convert(const TUint8 *aYBuf, const TUint8 *aUBuf,
                         const TUint8 *aVBuf,
                         TUint aBufWidth, TUint aBufHeight,
                         TUint8 *aTarget, TUint aTargetScanlineLength)
{
    TUint cols;
    TUint rows = iHeight;
    TUint8 *target;
    TUint8 *target2;
    const TUint8 *yb, *yb2;
    TInt rc, gc, bc;
    TInt y;
    TInt uval, vval;
    TUint8 val;


    __ASSERT_ALWAYS((aBufWidth >= iWidth) && (aBufHeight >= iHeight),
                    User::Invariant());

    // Cropping needed? 
    if ( iCropWidth > 0 ) {
        //sets offset to buffers; from now on increments below will always result the same offset, since the increment is aBufWidth
        aYBuf += iCropWidth;
        aUBuf += iCropWidth/2;
        aVBuf += iCropWidth/2;
    }
    if ( iCropHeight > 0 ) {
        //skip lines on top
        aYBuf += iCropHeight*aBufWidth;
        aUBuf += (iCropHeight/2)*aBufWidth/2;
        aVBuf += (iCropHeight/2)*aBufWidth/2;
    }
    // We don't interpolate the chrominance values at all, since that way we
    // can save a lot of multiplications. This actually doesn't affect the
    // subjective picture quality much, if at all, with natural images.

    // Conversion is done 2x2 pixels at a time

    // Luminance-only conversion?
    if ( (aUBuf != NULL) && (aVBuf != NULL) )
    {
        // Full conversion
        
        // Convert all rows, two at a time
        while ( rows )
        {
            // Convert all pixels in this row, two at a time
            cols = iWidth;
            target = aTarget;
            target2 = aTarget + aTargetScanlineLength;
            yb = aYBuf;
            yb2 = aYBuf + aBufWidth;
        
            while ( cols )
            {
                // Charles Poynton: Color FAQ
                // (http://www.inforamp.net/~poynton/ColorFAQ.html)
                // 30. How do I encode Y'CBCR components from computer R'G'B' ?

                // [normalized]
                // R =  1.1643828125 * (Y-16)  +  1.59602734375 * (Cr-128)
                // G =  1.1643828125 * (Y-16)  +  -0.39178515625 * (Cb-128) + -0.81296875 * (Cr-128)
                // B =  1.1643828125 * (Y-16)  +  2.01723046875 * (Cb-128)

                // We'll use fixed-point with 16 bits of fractional part for
                // accuracy. Besides, 24bpp RGB is not likely to be used in
                // low-CPU devices in the near future...

                // Red chrominance part for this 2x2 block:
                vval = ((TInt) aVBuf[0]) - 128;
                rc =  104597 * vval;

                // Green chrominance:
                uval = ((TInt) aUBuf[0]) - 128;
                gc = -25676*uval - 53279*vval;

                // Blue chrominance:
                bc = 132201 * uval;

                // Upper left pixel y part for all components:
                y = 76309 * (((TInt) yb[0]) - 16) + 32768; // round up

                // Calculate components and store:
                // Bitmap format: bbbbbbbb gggggggg rrrrrrrr
                target[0] = iRgbLookupTable[((y+bc) >> 16) + ESaturateOffset];
                target[1] = iRgbLookupTable[((y+gc) >> 16) + ESaturateOffset];
                target[2] = iRgbLookupTable[((y+rc) >> 16) + ESaturateOffset];

                // Upper right pixel:
                y = 76309 * (((TInt) yb[1]) - 16) + 32768;
                target[3] = iRgbLookupTable[((y+bc) >> 16) + ESaturateOffset];
                target[4] = iRgbLookupTable[((y+gc) >> 16) + ESaturateOffset];
                target[5] = iRgbLookupTable[((y+rc) >> 16) + ESaturateOffset];

                // Lower left:
                y = 76309 * (((TInt) yb2[0]) - 16) + 32768;
                target2[0] = iRgbLookupTable[((y+bc) >> 16) + ESaturateOffset];
                target2[1] = iRgbLookupTable[((y+gc) >> 16) + ESaturateOffset];
                target2[2] = iRgbLookupTable[((y+rc) >> 16) + ESaturateOffset];

                // Lower right:
                y = 76309 * (((TInt) yb2[1]) - 16) + 32768;
                target2[3] = iRgbLookupTable[((y+bc) >> 16) + ESaturateOffset];
                target2[4] = iRgbLookupTable[((y+gc) >> 16) + ESaturateOffset];
                target2[5] = iRgbLookupTable[((y+rc) >> 16) + ESaturateOffset];

                // Next two pixels:
                target += 6;
                target2 += 6;
                yb += 2;
                yb2 += 2;
                aUBuf++;
                aVBuf++;
                cols -= 2;
            }

            // Next rows
            rows -= 2;
            aYBuf += 2*aBufWidth;
            aUBuf += (aBufWidth - iWidth)/2;
            aVBuf += (aBufWidth - iWidth)/2;
            aTarget += 2*aTargetScanlineLength;
        }
    }
    else
    {
        // No chrominance given, do a luminance-only conversion
        
        // Convert all rows
        while ( rows )
        {
            // Convert all pixels in this row, two at a time
            cols = iWidth;
            target = aTarget;
        
            while ( cols )
            {
                // Do a pixel:
                y = 76309 * (((TInt) aYBuf[0]) - 16) + 32768;
                val = iRgbLookupTable[(y >> 16) + ESaturateOffset];
                target[0] = val;
                target[1] = val;
                target[2] = val;

                // And another one:
                y = 76309 * (((TInt) aYBuf[1]) - 16) + 32768;
                val = iRgbLookupTable[(y >> 16) + ESaturateOffset];
                target[3] = val;
                target[4] = val;
                target[5] = val;

                // Next two pixels:
                target += 6;
                aYBuf += 2;
                cols -= 2;
            }

            // Next row
            rows--;
            aYBuf += aBufWidth - iWidth;
            aTarget += aTargetScanlineLength;
        }
    }
}





//  End of File  
