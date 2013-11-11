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
*/

#ifndef __MIFCONVMBMGENERATORBMTOPBM_H__
#define __MIFCONVMBMGENERATORBMTOPBM_H__

#include "mifconv_mbmgenerator.h"
#include "mifconv.h"

struct TRgbQuad
    {
    unsigned char iBlue;
    unsigned char iGreen;
    unsigned char iRed;
    unsigned char iReserved;
    };

class BitmapLoader
    {
public:
    BitmapLoader();
    ~BitmapLoader();
    int LoadBitmap(MifConvSourceFile& aFileName,int aBpp,TBitmapColor aColor,SEpocBitmapHeader*& aPbm);
private:
    int DoLoad(const char* aFileName);
    int DoLoadAlpha(MifConvString& aAlphaFileName);
    int DoConvert(int aBpp,TBitmapColor aColor,SEpocBitmapHeader*& aPbm);
    TRgb GetBmpPixel(long aXCoord,long aYCoord);
    unsigned char GetAlphaPixel(long aXCoord,long aYCoord);
private:
    MifConvSourceFile* iSourceFile;
    TBitmapInfoHeader iBmpHeader;
    int iNumBmpColors;
    TRgbQuad* iBmpColors;
    char* iBmpBits;
    char* iAlphaBits;
    };

#endif
