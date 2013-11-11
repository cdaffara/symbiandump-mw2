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
* Description:   Implementation of class CYuv2Rgb16.
*                YUV to EColor64K colorspace converter concrete classes 
*
*/




#include "yuv2rgb16.h"

// ---------------------------------------------------------
// CYuv2Rgb16::CYuv2Rgb16()
// Standard C++ constructor.
// ---------------------------------------------------------
//
CYuv2Rgb16::CYuv2Rgb16()
{
    iWidth = iHeight = 0;
}

// ---------------------------------------------------------
// CYuv2Rgb16::~CYuv2Rgb16()
// Destructor
// ---------------------------------------------------------
//
CYuv2Rgb16::~CYuv2Rgb16()
{
}


// ---------------------------------------------------------
// CYuv2Rgb16::ConstructL()
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------
//
void CYuv2Rgb16::ConstructL(TUint aWidth, TUint aHeight, TUint /*aMaxWidth*/, TUint /*aMaxHeight*/)
{
    iWidth = aWidth;
    iHeight = aHeight;
}

// ---------------------------------------------------------
// CYuv2Rgb16::Convert()
// Converts a YUV frame to a EColor64K frame
// ---------------------------------------------------------
//
void CYuv2Rgb16::Convert(const TUint8 *aYBuf, const TUint8 *aUBuf,
                         const TUint8 *aVBuf,
                         TUint aBufWidth, TUint aBufHeight,
                         TUint8 *aTarget, TUint aTargetScanlineLength)
{


    TUint cols;
    TUint rows = iHeight;       
    
    __ASSERT_ALWAYS((aBufWidth >= iWidth) && (aBufHeight >= iHeight),
        User::Invariant());
        
    // Convert all rows, two at a time
    while ( rows )  
    {
        // Convert all pixels in this row, two at a time
        cols = iWidth;
        
        TUint8* target1 = aTarget;
        TUint8* target2 = aTarget + aTargetScanlineLength;
        const TUint8* YPtr1 = aYBuf;  
        const TUint8* YPtr2 = aYBuf + aBufWidth;        
        const TUint8* UPtr = aUBuf;
        const TUint8* VPtr = aVBuf;

        TUint    y, u, v;
        TInt rDiff, gDiff, bDiff;
        TInt     r, g, b;
       
        while ( cols )
        {
            y = *(YPtr1)++;
            u = *(UPtr)++ - 128;
            v = *(VPtr)++ - 128;

            rDiff = UvToRDiff(v, u);
            gDiff = UvToGDiff(v, u);
            bDiff = UvToBDiff(v, u);

            // Convert upper left pixel            
            r = (TInt)y + rDiff;
            r = r<0 ? 0 : r>255 ? 0xF800 : (r<<8)&0xF800;
            g = (TInt)y - gDiff;
            g = g<0 ? 0 : g>255 ? 0x07E0 : (g<<3)&0x07E0;
            b = (TInt)y + bDiff;
            b = b<0 ? 0 : b>255 ? 0x001F : (b>>3)&0x001F;

            // Write the pixel in RGB format
            *(TUint16*)target1 = (TUint16)(r | g | b);
            target1 += 2;

            // Convert upper right pixel
            y = *(YPtr1)++;

            r = (TInt)y + rDiff;
            r = r<0 ? 0 : r>255 ? 0xF800 : (r<<8)&0xF800;
            g = (TInt)y - gDiff;
            g = g<0 ? 0 : g>255 ? 0x07E0 : (g<<3)&0x07E0;
            b = (TInt)y + bDiff;
            b = b<0 ? 0 : b>255 ? 0x001F : (b>>3)&0x001F;

            *(TUint16*)target1 = (TUint16)(r | g | b);
            target1 += 2;

            // Convert lower left pixel
            y = *(YPtr2)++;

            r = (TInt)y + rDiff;
            r = r<0 ? 0 : r>255 ? 0xF800 : (r<<8)&0xF800;
            g = (TInt)y - gDiff;
            g = g<0 ? 0 : g>255 ? 0x07E0 : (g<<3)&0x07E0;
            b = (TInt)y + bDiff;
            b = b<0 ? 0 : b>255 ? 0x001F : (b>>3)&0x001F;

            *(TUint16*)target2 = (TUint16)(r | g | b);
            target2 += 2;
                
            // Convert lower right pixel
            y = *(YPtr2)++;

            r = (TInt)y + rDiff;
            r = r<0 ? 0 : r>255 ? 0xF800 : (r<<8)&0xF800;
            g = (TInt)y - gDiff;
            g = g<0 ? 0 : g>255 ? 0x07E0 : (g<<3)&0x07E0;
            b = (TInt)y + bDiff;
            b = b<0 ? 0 : b>255 ? 0x001F : (b>>3)&0x001F;

            *(TUint16*)target2 = (TUint16)(r | g | b);
            target2 += 2;

            // Next two pixels                                   
            cols -= 2;
        }
        
        // Next rows
        rows -= 2;
        aYBuf += 2*aBufWidth;
        aUBuf += aBufWidth/2;
        aVBuf += aBufWidth/2;
        aTarget += 2*aTargetScanlineLength;
    }
}
