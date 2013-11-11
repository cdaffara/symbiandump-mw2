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


#ifndef __MIFCONVMBMGENERATORPBMCOMP_H__
#define __MIFCONVMBMGENERATORPBMCOMP_H__

#include "mifconv_mbmgenerator.h"
#include "mifconv_util.h"

class BitmapCompiler
/**
Historical note - Pbm is an abbreviation for "Symbian Bitmap"
*/
    {
public:
    BitmapCompiler(MifConvSourceFileList aSourcefilenames);
    ~BitmapCompiler();
    int Compile(const MifConvString& aDestfilename, const MifConvString& aPaletteFilename);
private:
    int AllocatePbmSourcesArray();
    int LoadSourcefiles();
    int FileImage();
    int WriteFileheader();
    int WriteFilebitmap(SEpocBitmapHeader* aPbm);
    int WriteHeadStream();
    int LoadPalette(const char* aPaletteFilename);
    void CalculateInversePalette();
    int CompressBitmap(SEpocBitmapHeader*& aPbm);
    int CompressByteData(char*& aDest,char* aSrce,int aSize);
    int WriteCompressedByteData(char*& aDest,char* aData,int aLength, const char* aDestEnd);
    int WriteCompressedByteValues(char*& aDest,char aValue,int aLength, const char* aDestEnd);
    int CompressTwelveBitData(char*& aDest,char* aSrce,int aSizeInBytes);
    int WriteCompressedTwelveBitData(unsigned short*& aDest,unsigned short aData,int aLength);
    int CompressSixteenBitData(char*& aDest,char* aSrce,int aSize);
    int WriteCompressedSixteenBitData(char*& aDest,unsigned short* aData,int aLength, const char* aDestEnd);
    int WriteCompressedSixteenBitValues(char*& aDest,unsigned short aValue,int aLength, const char* aDestEnd);
    int CompressTwentyFourBitData(char*& aDest,char* aSrce,int aSizeInBytes);
    int WriteCompressedTwentyFourBitData(char*& aDest,char* aData,int aLength, const char* aDestEnd);
    int WriteCompressedTwentyFourBitValues(char*& aDest,char aComponent1,char aComponent2,char aComponent3,int aLength, const char* aDestEnd);
    int CompressThirtyTwoUBitData(char*& aDest,char* aSrce,int aSizeInBytes);
    int WriteCompressedThirtyTwoUBitData(char*& aDest,char* aData, int aLength, const char* aDestEnd);
    int WriteCompressedThirtyTwoUBitValues(char*& aDest,char aComponent1, char aComponent2,char aComponent3, int aLength, const char* aDestEnd);
    int TrueColorPointerCompare(char* aColorPointer,char aComponent1,char aComponent2,char aComponent3);
    int CompressThirtyTwoABitData(char*& aDest,char* aSrce,int aSizeInBytes);
    int WriteCompressedThirtyTwoABitData(char*& aDest,char* aData,int aLength,const char* aDestEnd);
    int WriteCompressedThirtyTwoABitValues(char*& aDest,char aComponent1,char aComponent2,char aComponent3,char aComponent4,int aLength,const char* aDestEnd);
    int ColorAlphaPointerCompare(char* aColorPointer,char aComponent1,char aComponent2,char aComponent3,char aComponent4);
    int ReadHexString(char aHexBuf[10],char*& aDataPtr,char* aDataPtrLimit);
    int HexToInt(char aHighNibble,char aLowNibble);
    int HexToInt(char aNibble);
private:
    fstream iDestFile;
    MifConvSourceFileList iSourcefilenames;
    SEpocBitmapHeader** iPbmSources;
    int iNumSources;
    int iDefaultPalette;
    TRgb iPalette[256];
    char iInversePalette[4096];
    };

#endif
