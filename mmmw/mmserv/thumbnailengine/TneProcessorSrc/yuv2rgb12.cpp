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
* Description:   Implementation of class CYuv2Rgb12.
*                YUV to EColor4K colorspace converter concrete classes 
*
*/


/*
-----------------------------------------------------------------------------

    DESCRIPTION

    

-----------------------------------------------------------------------------
*/


//  EXTERNAL RESOURCES  


//  Include Files  

#include <e32math.h>
#include "yuvconverter.h"
#include "yuv2rgb12.h"
#include "brightnesscontrast.h"


//  MEMBER FUNCTIONS


//=============================================================================

/*
-----------------------------------------------------------------------------

    CYuv2Rgb12

    CYuv2Rgb12()

    Standard C++ constructor

-----------------------------------------------------------------------------
*/

CYuv2Rgb12::CYuv2Rgb12()
{
    iRgbLookupTable = 0;
    iGamma = 65536;
    iBrightnessContrast = KMedBrightnessContrastIndex;
}



/*
-----------------------------------------------------------------------------

    CYuv2Rgb12

    ~CYuv2Rgb12()

    Standard C++ destructor

-----------------------------------------------------------------------------
*/

CYuv2Rgb12::~CYuv2Rgb12()
{
    User::Free(iRgbLookupTable);
}



/*
-----------------------------------------------------------------------------

    CYuv2Rgb12

    ConstructL()

    Standard Symbian OS second-phase constructor. Initializes the object.

-----------------------------------------------------------------------------
*/

void CYuv2Rgb12::ConstructL(TUint aWidth, TUint aHeight, TUint aMaxWidth, TUint aMaxHeight)
{
    // Remember the dimensions
//    __ASSERT_ALWAYS(((aWidth & 3) == 0) && ((aHeight & 3) == 0),
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

    CYuv2Rgb12

    SetGamma()

    Sets the conversion gamma value and recalculates the look-up table
    Please use the SetBrightnessContrast method for Series 60 display 

-----------------------------------------------------------------------------
*/
void CYuv2Rgb12::SetGamma(TInt aGamma)
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
        v = 298 * (i - ESaturateOffset - 16) / 256;
        // (see Convert())

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

        // 12bpp RGB has range [0..15] for all components, store to table:
        iRgbLookupTable[i] = (TUint8) (v >> 4);
    }
}


/*
-----------------------------------------------------------------------------

    CYuv2Rgb12

    SetBrightnessContrast()

    Sets the index to the predefined brightness&contrast lookup table 
    (KBrightnessContrastEnhParam) and recalculates the RGB look-up table
    The algorithm was developed by IMAAMI for Kenny display.

-----------------------------------------------------------------------------
*/
void CYuv2Rgb12::SetBrightnessContrast(TInt aBCIndex)
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

        // 12bpp RGB has range [0..15] for all components, store to table:
        iRgbLookupTable[i] = (TUint8) (v >> 4);
    }
}

/*
-----------------------------------------------------------------------------

    CYuv2Rgb12

    Convert()

    Converts a YUV frame to a ERgb12 frame

-----------------------------------------------------------------------------
*/

void CYuv2Rgb12::Convert(const TUint8 *aYBuf, const TUint8 *aUBuf,
                         const TUint8 *aVBuf,
                         TUint aBufWidth, TUint aBufHeight,
                         TUint8 *aTarget, TUint aTargetScanlineLength)
{
    TUint cols;
    TUint rows = iHeight;
    TUint8 *target;
    TUint8 *target2;
    const TUint8 *yb, *yb2;
    TInt y;
    TInt uval, vval;
    TUint8 val;
    TUint8 *rgbLookup = iRgbLookupTable + ESaturateOffset;
    TUint8 *rLookup, *gLookup, *bLookup;

    __ASSERT_ALWAYS((aBufWidth >= iWidth) && (aBufHeight >= iHeight),
                   User::Invariant());

    // cropping needed?
    if ( iCropWidth > 0 ) {
        // sets offset to buffers; from now on increments below will always result 
        // the same offset, since the increment is aBufWidth
        aYBuf += iCropWidth;
        aUBuf += iCropWidth/2;
        aVBuf += iCropWidth/2;
    }
    if ( iCropHeight > 0 ) {
        // skip lines on top
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
                // Traditional conversion:
                // R =  1.1643828125 * (Y-16)  +  1.59602734375 * (Cr-128)
                // G =  1.1643828125 * (Y-16)  +  -0.39178515625 * (Cb-128) + -0.81296875 * (Cr-128)
                // B =  1.1643828125 * (Y-16)  +  2.01723046875 * (Cb-128)

                // =>
                // R =  1.1643828125 * (Y - 16 +  1.370706718285 * (Cr-128))
                // G =  1.1643828125 * (Y - 16 +  -0.336474527143 * (Cb-128) + -0.6981971 * (Cr-128))
                // B =  1.1643828125 * (Y - 16 +  1.732446105434 * (Cb-128))

                // We'll create a lookup-table for 1.1643828125 * (x - 16). The
                // range needs go from -222 to 476 plus room for dithering.
                
                // Component lookups based on chrominance values for this 2x2
                // block
                vval = ((TInt) aVBuf[0]) - 128;
                //verified: shift to right is arithmetic in ARM-GCC => shifting of signed values is allowed
                rLookup = &rgbLookup[(351 * vval) >> 8]; 
                uval = ((TInt) aUBuf[0]) - 128;
                gLookup = &rgbLookup[(-86*uval - 179*vval) >> 8];
                bLookup = &rgbLookup[(444 * uval) >> 8];

                // Bitmap format: ggggbbbb xxxxrrrr
                
                // Upper left pixel
                y = yb[0];
                target[0] = (TUint8) ((gLookup[y] << 4) | bLookup[y]);
                target[1] = rLookup[y];
                
                // Upper right pixel
                y = yb[1] + 8;
                target[2] = (TUint8) ((gLookup[y] << 4) | bLookup[y]);
                target[3] = rLookup[y];
                    
                // Lower left:
                y = yb2[0] + 12;
                target2[0] = (TUint8) ((gLookup[y] << 4) | bLookup[y]);
                target2[1] = rLookup[y];

                // Lower right:
                y = yb2[1] + 4;
                target2[2] = (TUint8) ((gLookup[y] << 4) | bLookup[y]);
                target2[3] = rLookup[y];               
                
                // Next two pixels:
                target += 4;
                target2 += 4;
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
                // Upper left:
                val = rgbLookup[yb[0]];
                target[0] = (TUint8) ((val << 4) | val);
                target[1] = val;

                // Upper right:
                val = rgbLookup[yb[1] + 8];
                target[2] = (TUint8) ((val << 4) | val);
                target[3] = val;

                // Lower left:
                val = rgbLookup[yb[0] + 12];
                target2[0] = (TUint8) ((val << 4) | val);
                target2[1] = val;

                // Lower right:
                val = rgbLookup[yb[1] + 4];
                target2[2] = (TUint8) ((val << 4) | val);
                target2[3] = val;

                // Next two pixels:
                target += 4;
                target2 += 4;
                yb += 2;
                yb2 += 2;
                cols -= 2;
            }

            // Next row
            rows -= 2;
            aYBuf += aBufWidth;
            aTarget += aTargetScanlineLength;
        }
    }
}



// End of File
