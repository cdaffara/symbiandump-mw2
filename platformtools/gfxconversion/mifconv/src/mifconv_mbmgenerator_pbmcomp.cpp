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

#include "mifconv_mbmgenerator_pbmcomp.h"
#include "mifconv_mbmgenerator_bmtopbm.h"
#include "mifconv_util.h"
#include <stdlib.h>
#include <ctype.h>
#include <sys/stat.h>

extern TRgb* color256Palette;
extern char* color256InversePalette;

static inline bool CanCopy16bppData(const char* aDest, const char* aDestEnd, int aDataSize)
	{
	const char* aDestNew = 	aDest + (aDataSize / 128) * (256 + 1) + //(2 * 128) bytes data + 1 leading byte
		                            (aDataSize % 128 ? (aDataSize % 128) * 2 + 1 : 0);
	return aDestNew <= aDestEnd;
	}

static inline bool CanWrite16bppValue(const char* aDest, const char* aDestEnd, int aDataSize)
	{
	const char* aDestNew = 	aDest + (aDataSize / 128) * (2 + 1) + //2 bytes data + 1 leading byte
		                            (aDataSize % 128 ? 2 + 1 : 0);
	return aDestNew <= aDestEnd;
	}

static inline bool CanCopy8bppData(const char* aDest, const char* aDestEnd, int aDataSize)
	{
	const char* aDestNew = 	aDest + (aDataSize / 128) * (128 + 1) + //128 bytes data + 1 leading byte
		                            (aDataSize % 128 ? (aDataSize % 128) + 1 : 0);
	return aDestNew <= aDestEnd;
	}

static inline bool CanWrite8bppValue(const char* aDest, const char* aDestEnd, int aDataSize)
	{
	const char* aDestNew = 	aDest + (aDataSize / 128) * (1 + 1) + //1 byte data + 1 leading byte
		                            (aDataSize % 128 ? 1 + 1 : 0);
	return aDestNew <= aDestEnd;
	}

static inline bool CanCopy24bppData(const char* aDest, const char* aDestEnd, int aDataSize)
	{
	const char* aDestNew = 	aDest + (aDataSize / 128) * (384 + 1) + //(128 * 3) bytes data + 1 leading byte
		                            (aDataSize % 128 ? (aDataSize % 128) * 3 + 1 : 0);
	return aDestNew <= aDestEnd;
	}

static inline bool CanWrite24bppValue(const char* aDest, const char* aDestEnd, int aDataSize)
	{
	const char* aDestNew = 	aDest + (aDataSize / 128) * (3 + 1) + //3 bytes data + 1 leading byte
		                            (aDataSize % 128 ? 3 + 1 : 0);
	return aDestNew <= aDestEnd;
	}

static inline bool CanCopy32bppData(const char* aDest, const char* aDestEnd, int aDataSize)
	{
	const char* aDestNew = 	aDest + (aDataSize / 128) * (512 + 1) + //(128 * 4) bytes data + 1 leading byte
		                            (aDataSize % 128 ? (aDataSize % 128) * 4 + 1 : 0);
	return aDestNew <= aDestEnd;
	}

static inline bool CanWrite32bppValue(const char* aDest, const char* aDestEnd, int aDataSize)
	{
	const char* aDestNew = 	aDest + (aDataSize / 128) * (4 + 1) + //4 bytes data + 1 leading byte
		                            (aDataSize % 128 ? 4 + 1 : 0);
	return aDestNew <= aDestEnd;
	}

BitmapCompiler::BitmapCompiler(MifConvSourceFileList aSourcefilenames):
	iSourcefilenames(aSourcefilenames),
	iPbmSources(NULL),
	iNumSources(aSourcefilenames.size())
	{}

BitmapCompiler::~BitmapCompiler()
	{
	iDestFile.close();
	if(iPbmSources)
		for(int count=0;count<iNumSources;count++)
			delete iPbmSources[count];
	delete [] iPbmSources;
	iPbmSources = NULL;
	}

int BitmapCompiler::Compile(const MifConvString& aDestfilename, const MifConvString& aPaletteFilename)
	{    
        
	int ret = LoadPalette(aPaletteFilename.c_str());
	if (ret)
		return ret;
	
	iDestFile.open(aDestfilename.c_str(), ios::out | ios::binary);
	if(iDestFile.is_open()==0)
		return DestFile;
	if (iNumSources < 1)
		return SourceFile;

	// Read the usual list of [OPT]bmp_n source bitmaps.
	ret = LoadSourcefiles();
	
	if (ret)
		return ret;

	return FileImage();
	};

int BitmapCompiler::AllocatePbmSourcesArray()
	{
	iPbmSources = new SEpocBitmapHeader*[iNumSources];
	if(iPbmSources == NULL)
		return NoMemory;

	memset(iPbmSources,0xff,sizeof(SEpocBitmapHeader*)*iNumSources);
	int count;
	for(count=0;count<iNumSources;count++)
		iPbmSources[count]=NULL;
	return NoError;
	}

int BitmapCompiler::LoadSourcefiles()
	{
    //Initialize iPbmSources.
	int ret = AllocatePbmSourcesArray();
	if (ret)
		return ret;

    int count = 0;	
    for( MifConvSourceFileList::iterator i = iSourcefilenames.begin(); i != iSourcefilenames.end(); ++i, ++count )
        {
        bool useAlpha = false;
        int bpp = 2;
        TBitmapColor color = EMonochromeBitmap;
        BitmapLoader bmpload;
        int loc = 0;
                    
        if(i->DepthString().length() > 0)
            {
            if(i->DepthString()[loc]=='c')
                {
                color = EColorBitmap;
                loc++;
                }
            bpp=i->DepthString()[loc]-'0';
            loc++;
            int next=i->DepthString()[loc]-'0';
            if(next==2 || next==6 || next==4)
                {
                bpp=bpp*10+next;
                loc++;
                }
            if(i->DepthString()[loc]=='a')
                {
                useAlpha = true;
                loc++;
                if (color!=EColorBitmap || bpp!=32)
                    return Bpp;
                }
            if (color == EColorBitmap && bpp!=4 && bpp!=8 && bpp!=12 && bpp!=16 && bpp!=24 && bpp!=32)
                return Bpp;
            if (color == EMonochromeBitmap && bpp!=1 && bpp!=2 && bpp!=4 && bpp!=8)
                return Bpp;
            }
                    
        if (color == EColorBitmap && useAlpha)
            color = EColorBitmapAlpha;
                   
        ret = bmpload.LoadBitmap(*i,bpp,color,iPbmSources[count]);
                        
        // If error.
        if (ret)
            return ret;
        }
    return NoError;
	}

int BitmapCompiler::FileImage()
	{    
	int count = 0;
	
	// Compress
    for(;count<iNumSources;count++)
        {
        int ret = CompressBitmap(iPbmSources[count]);
        if(ret)
            return ret;
        }
    
	int ret = WriteFileheader();
	if (ret)
		return ret;
	for (count=0;count<iNumSources;count++)
		{
		ret = WriteFilebitmap(iPbmSources[count]);
		if (ret)
			return ret;
		}
	ret = WriteHeadStream();
	return ret;
	}

int BitmapCompiler::WriteFileheader()
	{
	int zero=0;
	iDestFile.write((char*)&KWriteOnceFileStoreUid,4);
	iDestFile.write((char*)&KMultiBitmapFileImageUid,4);
	iDestFile.write((char*)&zero,4);
	iDestFile.write((char*)&KMultiBitmapFileImageChecksum,4);
	int byteswritten=16;
	for(int count=0;count<iNumSources;count++)
		{
		byteswritten+=iPbmSources[count]->iBitmapSize;
		}
	byteswritten+=4;
	iDestFile.write((char*)&byteswritten,4);
	return NoError;
	}

int BitmapCompiler::WriteHeadStream()
	{
	iDestFile.write((char*)&iNumSources,4);
	int byteswritten=20;
	for(int count=0;count<iNumSources;count++)
		{
		iDestFile.write((char*)&byteswritten,4);
		byteswritten+=iPbmSources[count]->iBitmapSize;
		}
	return NoError;
	}

int BitmapCompiler::WriteFilebitmap(SEpocBitmapHeader* aPbm)
	{
	if (aPbm->iPaletteEntries != 0)
		return PaletteSupportNotImplemented;

	int dataSize = aPbm->iBitmapSize - sizeof(SEpocBitmapHeader);

	iDestFile.write((char*)(aPbm),sizeof(SEpocBitmapHeader));
	iDestFile.write(((char*)(aPbm)) + sizeof(SEpocBitmapHeader),dataSize);

	return NoError;
	}

int BitmapCompiler::LoadPalette(const char* aPaletteFilename)
	{
	if (!aPaletteFilename)
		{
		iDefaultPalette = 1;
		return NoError;
		}

	iDefaultPalette = 0;
	color256Palette = iPalette;
	color256InversePalette = iInversePalette;

	struct stat fileinfo;
	if (stat(aPaletteFilename,&fileinfo)==-1)
		return PaletteFile;

	int filesize = fileinfo.st_size;
	if (filesize == 0)
		return PaletteFile;

	fstream paletteFile(aPaletteFilename, FILE_IN_BINARY_NOCREATE_FLAGS);
	
	if(!paletteFile.is_open())
		return PaletteFile;

	char* paletteData = new char[filesize];
	if (!paletteData)
		return NoMemory;

	memset(paletteData,0,filesize);
	paletteFile.read(paletteData,filesize);
	paletteFile.close();

	char* dataPtr = (char*)paletteData;
	char* dataPtrLimit = dataPtr + filesize;

	for (int index = 0; index < 256; index++)
		{
		char hexBuf[10];
		int ret = ReadHexString(hexBuf,dataPtr,dataPtrLimit);
		if (ret)
			{
			delete paletteData;
			return ret;
			}

		int red = HexToInt(hexBuf[8],hexBuf[9]);
		int green = HexToInt(hexBuf[6],hexBuf[7]);
		int blue = HexToInt(hexBuf[4],hexBuf[5]);

		iPalette[index] = TRgb(red,green,blue);
		}

	delete paletteData;
	CalculateInversePalette();
	return NoError;
	}

void BitmapCompiler::CalculateInversePalette()
	{
	for (int index = 0; index < 4096; index++)
		{
		TRgb color = TRgb((index & 0x00f) * 17,((index & 0x0f0) >> 4) * 17,((index & 0xf00) >> 8) * 17);

		int nearest = 0;
		int distance = iPalette[0].Difference(color);

		for (int paletteIndex = 0; paletteIndex < 256; paletteIndex++)
			{
			TRgb paletteColor = iPalette[paletteIndex];

			if (paletteColor == color)
				{
				nearest = paletteIndex;
				break;
				}

			int paletteDistance = paletteColor.Difference(color);
			if (paletteDistance < distance)
				{
				nearest = paletteIndex;
				distance = paletteDistance;
				}
			}

		iInversePalette[index] = (char)nearest;
		TRgb palColor = iPalette[nearest];
		color = palColor;
		}
	}

int BitmapCompiler::CompressBitmap(SEpocBitmapHeader*& aPbm)
	{
       
	int originalsize = aPbm->iBitmapSize-sizeof(SEpocBitmapHeader);
	char* newbits=new char[originalsize+sizeof(SEpocBitmapHeader)];
	if(!newbits)
		return NoMemory;

	memset(newbits,0xff,sizeof(SEpocBitmapHeader)+originalsize);
	memcpy(newbits,aPbm,sizeof(SEpocBitmapHeader));
	char* newbitsptr=newbits+sizeof(SEpocBitmapHeader);
	char* oldbits=((char*)(aPbm))+sizeof(SEpocBitmapHeader);

	TBitmapfileCompression compression = ENoBitmapCompression;
	int ret = NoCompression;

	if (aPbm->iBitsPerPixel <= 8)
		{
		compression = EByteRLECompression;
		ret = CompressByteData(newbitsptr,oldbits,originalsize);
		}
	else if (aPbm->iBitsPerPixel == 12)
		{
		compression = ETwelveBitRLECompression;
		ret = CompressTwelveBitData(newbitsptr,oldbits,originalsize);
		}
	else if (aPbm->iBitsPerPixel == 16)
		{
		compression = ESixteenBitRLECompression;
		ret = CompressSixteenBitData(newbitsptr,oldbits,originalsize);
		}
	else if (aPbm->iBitsPerPixel == 24)
		{
		compression = ETwentyFourBitRLECompression;
		ret = CompressTwentyFourBitData(newbitsptr,oldbits,originalsize);
		}
	else if (aPbm->iBitsPerPixel == 32 && (aPbm->iColor == EColorBitmap))
		{
		compression = EThirtyTwoUBitRLECompression;
		ret = CompressThirtyTwoUBitData(newbitsptr,oldbits,originalsize);
		}
	else if (aPbm->iBitsPerPixel == 32 && (aPbm->iColor == EColorBitmapAlpha))
		{
		compression = EThirtyTwoABitRLECompression;
		ret = CompressThirtyTwoABitData(newbitsptr,oldbits,originalsize);
		}

	if(ret)
		{
		delete [] newbits;
		if(ret>0)
			return ret;
		return NoError;
		}

	delete aPbm;
	aPbm = (SEpocBitmapHeader*)newbits;
	aPbm->iBitmapSize = newbitsptr-(newbits+sizeof(SEpocBitmapHeader))+sizeof(SEpocBitmapHeader);
	aPbm->iCompression = compression;
	return NoError;
	}

int BitmapCompiler::CompressByteData(char*& aDest,char* aSrce,int aSize)
	{
	const char* destEnd = aDest + aSize;
	char* bytepointer=aSrce;
	char* limitpointer=bytepointer+aSize;
	int margin = (aSize>>6);
	char* limitmargin = limitpointer - ((margin > 4) ? margin : 4);
	char* cutoffpointer=aDest+(aSize>>1)+(aSize>>2);
	int ret=NoError;
	char* oldbytepointer=NULL;
	while(bytepointer<limitmargin)
		{
		char value=*bytepointer;
		if(*(bytepointer+1)==value && *(bytepointer+2)==value)
			{
			oldbytepointer=bytepointer;
			bytepointer+=3;
			while ( ( bytepointer < limitmargin ) && ( *bytepointer == value ) )
				bytepointer++;
			ret = WriteCompressedByteValues(aDest,value,bytepointer-oldbytepointer, destEnd);
			if(ret) return ret;
			}
		else
			{
			oldbytepointer=bytepointer;
			while((bytepointer<limitmargin) && (*(bytepointer+1)!=value || *(bytepointer+2)!=value))
				{
				bytepointer++;
				value=*bytepointer;
				}
			ret = WriteCompressedByteData(aDest,oldbytepointer,bytepointer-oldbytepointer, destEnd);
			if(ret) return ret;
			}
		if(aDest>cutoffpointer)
			return NoCompression;
		}

	int remaining = limitpointer-bytepointer;
	if(remaining > 0)
		{
		if (aDest + remaining > cutoffpointer)
			return NoCompression;
		ret=WriteCompressedByteData(aDest,bytepointer,remaining, destEnd);
		if(ret) return ret;
		}
	return NoError;
	}

int BitmapCompiler::WriteCompressedByteData(char*& aDest,char* aData,int aLength, const char* aDestEnd)
	{
	if(!CanCopy8bppData(aDest, aDestEnd, aLength))
		return NoCompression;
	while(aLength>128)
		{
		*aDest++=-128;
		for(int count=0;count<128;count++)
			*aDest++=*aData++;
		aLength-=128;
		}
	if(aLength>128 || aLength<1) return CompressionError;
	*aDest++=char(-aLength);
	for(int count=0;count<aLength;count++)
		*aDest++=*aData++;
	return NoError;
	}


int BitmapCompiler::WriteCompressedByteValues(char*& aDest,char aValue,int aLength, const char* aDestEnd)
	{
	if (aLength < 1)
		return CompressionError;

	if(!CanWrite8bppValue(aDest, aDestEnd, aLength))
		return NoCompression;

	while (aLength > 128)
		{
		*aDest++ = 127;
		*aDest++ = aValue;
		aLength -= 128;
		}

	*aDest++ = char(aLength-1);
	*aDest++ = aValue;

	return NoError;
	}

int BitmapCompiler::CompressTwelveBitData(char*& aDest,char* aSrce,int aSizeInBytes)
	{
	unsigned short* srcePtr = (unsigned short*)aSrce;
	unsigned short* srceLimitPtr = srcePtr + (aSizeInBytes / 2);
	unsigned short* dest = (unsigned short*)aDest;

	while (srcePtr < srceLimitPtr)
		{
		unsigned short* runStartPtr = srcePtr;
		unsigned short value = *srcePtr;
		do
			srcePtr++;
		while (srcePtr < srceLimitPtr && *srcePtr == value);
		WriteCompressedTwelveBitData(dest,*runStartPtr,srcePtr - runStartPtr); // Ignore error return as 12bpp compression never fails
		}

	aDest = (char*)dest;
	return NoError;
	}

int BitmapCompiler::WriteCompressedTwelveBitData(unsigned short*& aDest,unsigned short aData,int aLength)
	{
	// The run length l is stored as l-1 in the top 4 bits of each 16-bit word (between 1 and 16)
	aData &= 0x0fff;
	unsigned short maxLengthData = (unsigned short)(aData | 0xf000);

	while(aLength>16)
		{
		*aDest++ = maxLengthData;
		aLength -= 16;
		}

	if (aLength > 0)
		*aDest++ = (unsigned short)(aData | ((aLength - 1) << 12));

	return NoError;
	}

int BitmapCompiler::CompressSixteenBitData(char*& aDest,char* aSrce,int aSizeInBytes)
	{
	char* destEnd = aDest + aSizeInBytes;
	unsigned short* srcePtr = (unsigned short*)aSrce;
	unsigned short* srceLimitPtr = srcePtr + (aSizeInBytes / 2);
	unsigned short* srceLimitPtrMinusOne = srceLimitPtr - 1;
	char* destCompressedLimitPtr = aDest + (aSizeInBytes * 7 / 8);
	int ret = NoError;

	while (srcePtr < srceLimitPtrMinusOne)
		{
		unsigned short value = *srcePtr;
		unsigned short* runStartPtr = srcePtr++;

		if(*srcePtr == value)
			{
			do
				srcePtr++;
			while ( ( srcePtr < srceLimitPtr ) && ( *srcePtr == value ) );

			ret = WriteCompressedSixteenBitValues(aDest,value,srcePtr-runStartPtr, destEnd);
			if (ret)
				return ret;
			}
		else
			{
			value = *srcePtr;
			while (srcePtr < srceLimitPtrMinusOne && *(srcePtr + 1) != value)
				{
				srcePtr++;
				value = *srcePtr;
				}

			ret = WriteCompressedSixteenBitData(aDest,runStartPtr,srcePtr - runStartPtr, destEnd);
			if (ret)
				return ret;
			}
		if (aDest > destCompressedLimitPtr)
			return NoCompression;
		}

	if (srcePtr < srceLimitPtr)
		{
		ret = WriteCompressedSixteenBitData(aDest,srcePtr,srceLimitPtr - srcePtr, destEnd);
		if (ret)
			return ret;
		}

	if (aDest > destCompressedLimitPtr)
		return NoCompression;
	return NoError;
	}

int BitmapCompiler::WriteCompressedSixteenBitData(char*& aDest,unsigned short* aData,
												  int aLength, const char* aDestEnd)
	{
	if (aLength < 1)
		return CompressionError;

	if(!CanCopy16bppData(aDest, aDestEnd, aLength))
		return NoCompression;

	char* srcePtr = (char*)aData;

	while (aLength > 128)
		{
		*aDest++ = -128;
		memcpy(aDest,srcePtr,256);
		aDest += 256;
		srcePtr += 256;
		aLength -= 128;
		}

	*aDest++ = char(-aLength);

	int remainingBytes = aLength * 2;
	memcpy(aDest,srcePtr,remainingBytes);
	aDest += remainingBytes;

	return NoError;
	}

int BitmapCompiler::WriteCompressedSixteenBitValues(char*& aDest,unsigned short aValue,
													int aLength, const char* aDestEnd)
	{
	if (aLength < 1)
		return CompressionError;

	if(!CanWrite16bppValue(aDest, aDestEnd, aLength))
		return NoCompression;

	char lowByte = char(aValue);
	char highByte = char(aValue >> 8);

	while (aLength > 128)
		{
		*aDest++ = 127;
		*aDest++ = lowByte;
		*aDest++ = highByte;
		aLength -= 128;
		}

	*aDest++ = char(aLength-1);
	*aDest++ = lowByte;
	*aDest++ = highByte;

	return NoError;
	}

int BitmapCompiler::CompressTwentyFourBitData(char*& aDest,char* aSrce,int aSizeInBytes)
	{
	const char* destEnd = aDest + aSizeInBytes; 
	char* srceLimitPtr = aSrce + aSizeInBytes;
	char* srceLimitPtrMinusThree = srceLimitPtr - 3; // three bytes == one pixel
	char* destCompressedLimitPtr = aDest + (aSizeInBytes * 7 / 8);
	int ret = NoError;

	while (aSrce < srceLimitPtrMinusThree)
		{
		char* runStartPtr = aSrce;
		char component1 = *aSrce++;
		char component2 = *aSrce++;
		char component3 = *aSrce++;

		if (TrueColorPointerCompare(aSrce,component1,component2,component3))
			{
			do
				aSrce += 3;
			while (aSrce < srceLimitPtr && TrueColorPointerCompare(aSrce,component1,component2,component3));

			ret = WriteCompressedTwentyFourBitValues(aDest,component1,component2,component3,(aSrce - runStartPtr) / 3, destEnd);
			if (ret)
				return ret;
			}
		else
			{
			bool more  = true;
			bool eqRun = false;
			do
				{
				component1 = *aSrce++;
				component2 = *aSrce++;
				component3 = *aSrce++;
				more = (aSrce < srceLimitPtr);
				eqRun = more && TrueColorPointerCompare(aSrce,component1,component2,component3);
				}
			while (more && !eqRun);
			if (eqRun)
				aSrce -= 3;
			int pixelLength = (aSrce - runStartPtr) / 3;
			ret = WriteCompressedTwentyFourBitData(aDest,runStartPtr,pixelLength,destEnd);
			if (ret)
				return ret;
			}
		if (aDest > destCompressedLimitPtr)
			return NoCompression;
		}

	if (aSrce < srceLimitPtr)
		{
		ret = WriteCompressedTwentyFourBitData(aDest,aSrce,(srceLimitPtr - aSrce) / 3, destEnd);
		if (ret)
			return ret;
		}

	if (aDest > destCompressedLimitPtr)
		return NoCompression;
	return NoError;
	}

int BitmapCompiler::WriteCompressedTwentyFourBitData(char*& aDest,char* aData,
													 int aLength, const char* aDestEnd)
	{
	if (aLength < 1)
		return CompressionError;

	if(!CanCopy24bppData(aDest, aDestEnd, aLength))
		return NoCompression;

	while (aLength > 128)
		{
		*aDest++ = -128;
		memcpy(aDest,aData,384);
		aDest += 384;
		aData += 384;
		aLength -= 128;
		}

	*aDest++ = char(-aLength);

	int remainingBytes = aLength * 3;
	memcpy(aDest,aData,remainingBytes);
	aDest += remainingBytes;

	return NoError;
	}

int BitmapCompiler::WriteCompressedTwentyFourBitValues(char*& aDest,char aComponent1,
													   char aComponent2,char aComponent3,
													   int aLength, const char* aDestEnd)
	{
	if (aLength < 1)
		return CompressionError;

	if(!CanWrite24bppValue(aDest, aDestEnd, aLength))
		return NoCompression;

	while (aLength > 128)
		{
		*aDest++ = 127;
		*aDest++ = aComponent1;
		*aDest++ = aComponent2;
		*aDest++ = aComponent3;
		aLength -= 128;
		}

	*aDest++ = char(aLength-1);
	*aDest++ = aComponent1;
	*aDest++ = aComponent2;
	*aDest++ = aComponent3;

	return NoError;
	}

/** The function encodes 32-bit buffer into 24-bit stream by using RLE compression algorithm*/
int BitmapCompiler::CompressThirtyTwoUBitData(char*& aDest,char* aSrce,int aSizeInBytes)
	{
	const char* destEnd = aDest + aSizeInBytes; 
	char* srceLimitPtr = aSrce + aSizeInBytes;
	char* srceLimitPtrMinusFour = srceLimitPtr - 4; // four bytes == one pixel
	char* destCompressedLimitPtr = aDest + (aSizeInBytes * 7 / 8);
	int ret = NoError;

	while (aSrce < srceLimitPtrMinusFour)
		{
		char* runStartPtr = aSrce;
		char component1 = *aSrce++;
		char component2 = *aSrce++;
		char component3 = *aSrce++;
		aSrce++;

		if (TrueColorPointerCompare(aSrce,component1,component2,component3))
			{
			do
				aSrce += 4;
			while (aSrce < srceLimitPtr && TrueColorPointerCompare(aSrce,component1,component2,component3));

			ret = WriteCompressedThirtyTwoUBitValues(aDest,component1,component2,component3,(aSrce - runStartPtr) / 4, destEnd);
			if (ret)
				return ret;
			}
		else //non repeating pixel buffer
			{
			bool more  = true;
			bool eqRun = false;
			do {
				component1 = *aSrce++;
				component2 = *aSrce++;
				component3 = *aSrce++;
				aSrce++; //last byte is unused (no alpha component)
				more = (aSrce < srceLimitPtr);
				eqRun = more && TrueColorPointerCompare(aSrce,component1,component2,component3);
				} while (more && !eqRun);
			if (eqRun)
				aSrce -= 4;
			int pixelLength = (aSrce - runStartPtr) / 4;
			ret = WriteCompressedThirtyTwoUBitData(aDest,runStartPtr,pixelLength,destEnd);
 			if (ret)
				return ret;
			}
		if (aDest > destCompressedLimitPtr)
			return NoCompression;
		}

	if (aSrce < srceLimitPtr)
		{
		ret = WriteCompressedThirtyTwoUBitData(aDest,aSrce,(srceLimitPtr - aSrce) / 4, destEnd);
		if (ret)
			return ret;
		}

	if (aDest > destCompressedLimitPtr)
		return NoCompression;
	return NoError;
	}

/** The function copies non repeating 32-bit buffer to 24-bit compressed buffer*/
int BitmapCompiler::WriteCompressedThirtyTwoUBitData(char*& aDest,char* aData,
													 int aLength, const char* aDestEnd)
	{
	if (aLength < 1)
		return CompressionError;

	if(!CanCopy24bppData(aDest, aDestEnd, aLength))
		return NoCompression;

	while (aLength > 128)
		{
		*aDest++ = -128;
		for(int ii = 0; ii < 128; ii++)
			{
			memcpy(aDest,aData, 3);
			aDest += 3;
			aData += 4;
			}
		aLength -= 128;
		}

	*aDest++ = char(-aLength);

	while(aLength--)
		{
		memcpy(aDest,aData, 3);
		aDest += 3;
		aData += 4;
		}

	return NoError;
	}

/** The function copies repeating colour to the 24-bit compressed buffer */
int BitmapCompiler::WriteCompressedThirtyTwoUBitValues(char*& aDest,char aComponent1,
													   char aComponent2,char aComponent3,
													   int aLength, const char* aDestEnd)
	{
	if (aLength < 1)
		return CompressionError;

	if(!CanWrite24bppValue(aDest, aDestEnd, aLength))
		return NoCompression;
	while (aLength > 128)
		{
		*aDest++ = 127;
		*aDest++ = aComponent1;
		*aDest++ = aComponent2;
		*aDest++ = aComponent3;
		aLength -= 128;
		}

	*aDest++ = char(aLength-1);
	*aDest++ = aComponent1;
	*aDest++ = aComponent2;
	*aDest++ = aComponent3;

	return NoError;
	}

int BitmapCompiler::TrueColorPointerCompare(char* aColorPointer,char aComponent1,char aComponent2,char aComponent3)
	{
	return (*aColorPointer == aComponent1 && *(aColorPointer + 1) == aComponent2 && *(aColorPointer + 2) == aComponent3);
	}

/** The function encodes 32-bit buffer into 32-bit stream by using RLE compression algorithm*/
int BitmapCompiler::CompressThirtyTwoABitData(char*& aDest,char* aSrce,int aSizeInBytes)
	{
	const char* destEnd = aDest + aSizeInBytes; 
	char* srceLimitPtr = aSrce + aSizeInBytes;
	char* srceLimitPtrMinusFour = srceLimitPtr - 4; // four bytes == one pixel
	char* destCompressedLimitPtr = aDest + (aSizeInBytes * 7 / 8);
	int ret = NoError;

	while (aSrce < srceLimitPtrMinusFour)
		{
		char* runStartPtr = aSrce;
		char component1 = *aSrce++;
		char component2 = *aSrce++;
		char component3 = *aSrce++;
		char component4 = *aSrce++;

		if (ColorAlphaPointerCompare(aSrce,component1,component2,component3,component4))
			{
			do
				aSrce += 4;
			while (aSrce < srceLimitPtr && ColorAlphaPointerCompare(aSrce,component1,component2,component3,component4));
			int eqPixelLength = (aSrce - runStartPtr) / 4;
			ret = WriteCompressedThirtyTwoABitValues(aDest,component1,component2,component3,component4,eqPixelLength,destEnd);
			if (ret)
				return ret;
			}
		else //non repeating pixel buffer
			{
			bool more  = true;
			bool eqRun = false;
			do {
				component1 = *aSrce++;
				component2 = *aSrce++;
				component3 = *aSrce++;
				component4 = *aSrce++;
				more = (aSrce < srceLimitPtr);
				eqRun = more && ColorAlphaPointerCompare(aSrce,component1,component2,component3,component4);
				} while (more && !eqRun);
			if (eqRun)
				aSrce -= 4;
			int nePixelLength = (aSrce - runStartPtr) / 4;
			ret = WriteCompressedThirtyTwoABitData(aDest,runStartPtr,nePixelLength,destEnd);
			if (ret)
				return ret;
			}
		if (aDest > destCompressedLimitPtr)
			return NoCompression;
		}

	if (aSrce < srceLimitPtr)
		{
		ret = WriteCompressedThirtyTwoABitData(aDest,aSrce,(srceLimitPtr - aSrce) / 4, destEnd);
		if (ret)
			return ret;
		}

	if (aDest > destCompressedLimitPtr)
		return NoCompression;
	return NoError;
	}

/** The function copies non repeating 32-bit buffer to 32-bit compressed buffer*/
int BitmapCompiler::WriteCompressedThirtyTwoABitData(char*& aDest,char* aData,
													 int aLength, const char* aDestEnd)
	{
	if (aLength < 1)
		return CompressionError;

	if(!CanCopy32bppData(aDest, aDestEnd, aLength))
		return NoCompression;

	while (aLength > 128)
		{
		*aDest++ = -128;
		memcpy(aDest,aData,512);
		aDest += 512;
		aData += 512;
		aLength -= 128;
		}

	*aDest++ = char(-aLength);
	memcpy(aDest,aData, aLength*4);
	aDest += aLength*4;
	return NoError;
	}

/** The function copies repeating pixels including alpha to a 32-bit compressed buffer */
int BitmapCompiler::WriteCompressedThirtyTwoABitValues(char*& aDest,char aComponent1,
													   char aComponent2,char aComponent3,
													   char aComponent4,int aLength,
													   const char* aDestEnd)

	{
	if (aLength < 1)
		return CompressionError;

	if(!CanWrite32bppValue(aDest, aDestEnd, aLength))
		return NoCompression;
	while (aLength > 128)
		{
		*aDest++ = 127;
		*aDest++ = aComponent1;
		*aDest++ = aComponent2;
		*aDest++ = aComponent3;
		*aDest++ = aComponent4;
		aLength -= 128;
		}

	*aDest++ = char(aLength-1);
	*aDest++ = aComponent1;
	*aDest++ = aComponent2;
	*aDest++ = aComponent3;
	*aDest++ = aComponent4;

	return NoError;
	}

int BitmapCompiler::ColorAlphaPointerCompare(char* aColorPointer,char aComponent1,char aComponent2,char aComponent3,char aComponent4)
	{
	return (*aColorPointer == aComponent1 && *(aColorPointer + 1) == aComponent2 && *(aColorPointer + 2) == aComponent3 && *(aColorPointer + 3) == aComponent4);
	}

int BitmapCompiler::ReadHexString(char aHexBuf[10],char*& aDataPtr,char* aDataPtrLimit)
	{
	while (aDataPtr < aDataPtrLimit)
		{
		if (aDataPtr[0] == '0')
			{
			if (aDataPtr[1] == 'x' || aDataPtr[1] == 'X')
				{
				memcpy(aHexBuf,aDataPtr,10);
				aDataPtr += 10;
				return NoError;
				}
			}
		aDataPtr++;
		}

	return PaletteFile;
	}

int BitmapCompiler::HexToInt(char aHighNibble,char aLowNibble)
	{
	return (HexToInt(aHighNibble) << 4) + HexToInt(aLowNibble);
	}

int BitmapCompiler::HexToInt(char aNibble)
	{
	int value = 0;

	if (aNibble >= '0' && aNibble <= '9')
		value = aNibble - '0';
	else if (aNibble >= 'a' && aNibble <= 'f')
		value = aNibble - 'a' + 10;
	else if (aNibble >= 'A' && aNibble <= 'F')
		value = aNibble - 'A' + 10;

	return value;
	}
