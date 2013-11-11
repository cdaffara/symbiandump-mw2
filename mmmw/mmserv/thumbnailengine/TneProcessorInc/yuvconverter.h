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
* Description:   The YUV-to-something converter base class CYuvConverter definition.
*
*/


    

#ifndef     __YUVCONVERTER_H__
#define     __YUVCONVERTER_H__


//  INCLUDES

#ifndef __E32BASE_H__
#include <e32base.h>
#endif


//  CLASS DEFINITIONS 

class CYuvConverter : public CBase
{
public:
    // CBase provides a virtual destructor

    // Second-phase constructor
    virtual void ConstructL(TUint aWidth, TUint aHeight, TUint aMaxWidth, TUint aMaxHeight) = 0;

    // Set conversion gamma
    virtual void SetGamma(TInt aGamma) = 0;

    // Set Brightness & Contrast setting. All yuv2rgb classes may not support.
    // This might suit better than SetGamma for displays typically used in Series60 devices.
    virtual void SetBrightnessContrast(TInt ) { };

    // Do the conversion
    virtual void Convert(const TUint8 *aYBuf, const TUint8 *aUBuf,
                         const TUint8 *aVBuf,
                         TUint aBufWidth, TUint aBufHeight,
                         TUint8 *aTarget, TUint aTargetScanlineLength) = 0;
};

#endif      //  __YUVCONVERTER_H__
            
// End of File
