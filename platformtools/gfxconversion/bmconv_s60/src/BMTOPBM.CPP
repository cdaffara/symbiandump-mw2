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

#include "BMCONV.H"

const TRgbQuad KNokiaBrandBlue = {0xcc,0x33,0,0};
const TRgbQuad KNokiaBrandGreen = {0x33,0x99,0,0};
const int KNokiaBrandBlueIndex = 254;
const int KNokiaBrandGreenIndex = 253;

const unsigned char ColorRemapTable[256] = 
	{
	0xff, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
	0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
	0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f,
	0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f,
	0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f,
	0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f,
	0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f,
	0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f,
	0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f,
	0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f,
	0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf,
	0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf,
	0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf,
	0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0x00, 0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf,
	0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef,
	0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xd7
	};

BitmapLoader::BitmapLoader():
	iNumBmpColors(0),
	iBmpColors(NULL),
	iBmpBits(NULL),
	iAlphaBits(NULL)
	{}

BitmapLoader::~BitmapLoader()
	{
	delete [] iBmpColors;
	delete [] iBmpBits;
	delete [] iAlphaBits;
	}

int BitmapLoader::LoadBitmap(TSourceFile& aFileName,int aBpp,TBitmapColor aColor,SEpocBitmapHeader*& aPbm)
	{
	iSourceFile = &aFileName;
	char sig[2];
#if defined(__MSVCDOTNET__) || defined(__LINUX__) || defined(__TOOLS2__) || defined(__CW32__)
	fstream file(aFileName.FileName(), ios::in | ios::binary);
#else	
	fstream file(aFileName.FileName(), ios::in | ios::binary | ios::nocreate);	
#endif		

	if (file.is_open()==0)
		return Files;
	file.read(sig,2);
	file.close();
	if (file.gcount()!=2)
		return SourceFile ;
	if (sig[0]!='B'||sig[1]!='M')
		return SourceFile;

	int ret = DoLoad(aFileName.FileName());

    // Iff we have >8bit color BMP file and c8, change to c16
    if( (aColor==EColorBitmap) && (aBpp==8) && 
        (iBmpHeader.biBitCount>8) )
        {
        // cout << "Note: " << aFileName.FileName() << " changed c8 -> c16" << "\n";
        aBpp = 16;
        }
	if (!ret && aColor==EColorBitmapAlpha)
		{
		int fileNameLen = strlen(aFileName.FileName());
		char* alphaFileName = new char[fileNameLen + 7];// -alpha suffix is 6 chars, plus NUL termination
		if (alphaFileName == NULL)
			return NoMemory;
		int dotPos = -1;
		for (int i = 0; i < fileNameLen; ++i)
			if (aFileName.FileName()[i]=='.')
				dotPos=i;
		int prefixLen = (dotPos>=0?dotPos:fileNameLen);
		memcpy(alphaFileName,aFileName.FileName(),prefixLen);
		const char* suffix = "-alpha";
		memcpy(alphaFileName+prefixLen,suffix,6);
		if (dotPos>=0)
			memcpy(alphaFileName+prefixLen+6,aFileName.FileName()+dotPos,fileNameLen-dotPos);
		*(alphaFileName + fileNameLen + 6) = '\0';
		ret = DoLoadAlpha(alphaFileName); // overlay alpha data from separate file
		delete [] alphaFileName;
		}
	if (!ret)
		ret = DoConvert(aBpp,aColor,aPbm);
	return ret;
	}

int BitmapLoader::DoLoad(char* aFileName)
	{
#if defined (__MSVCDOTNET__)  || defined(__TOOLS2__) || defined(__CW32__)    	
	fstream file(aFileName, ios::in | ios::binary);
#else //!__MSVCDOTNET__
	fstream file(aFileName, ios::in | ios::binary | ios::nocreate);
#endif //__MSVCDOTNET__
	if (file.is_open()==0)
		return Files;
	TBitmapFileHeader bmpfile;
	long size=sizeof(TBitmapFileHeader);
	file.read((char *)&bmpfile,size);
	if (file.gcount()!=size)
		return SourceFile;
	size=sizeof(TBitmapInfoHeader);
	file.read((char *)&iBmpHeader,size);
	if (file.gcount()!=size)
		return SourceFile;
	if (iBmpHeader.biCompression != 0)
		return UnknownCompression;
	size=bmpfile.bfSize-sizeof(TBitmapInfoHeader)-sizeof(TBitmapFileHeader);
	long bitcount=iBmpHeader.biBitCount;
	long colors=iBmpHeader.biClrUsed;
	if (colors==0)
		{
		if (bitcount==24)
			iNumBmpColors=0;
		else if (bitcount==32)
			iNumBmpColors=0;//See MSDN - BITMAPFILEHEADER and BITMAPINFOHEADER structures.
                            //If biCompression is 0 - we don't have TRgbQuad array! 
		else
			iNumBmpColors=1<<bitcount;
		}
	else
		iNumBmpColors=colors;
	if (iNumBmpColors > 256)
		return SourceFile;
	if (iNumBmpColors>0)
		{
		iBmpColors = new TRgbQuad[iNumBmpColors];
		if (iBmpColors == NULL)
			return NoMemory;
		memset(iBmpColors,0,iNumBmpColors*sizeof(TRgbQuad));
		}
	size-=iNumBmpColors*sizeof(TRgbQuad);
	iBmpBits = new char[size];
	if (iBmpBits == NULL)
		return NoMemory;
	memset(iBmpBits,0xff,size);

	if(iBmpColors != NULL)
	    {
		file.read((char *)iBmpColors,iNumBmpColors*sizeof(TRgbQuad));
		if (file.gcount()!=(int)(iNumBmpColors*sizeof(TRgbQuad)))
			return SourceFile;
	    }
	file.read(iBmpBits,size);
	file.close();
	if (file.gcount()!=size)
		return SourceFile;
	return NoError;
	}

int BitmapLoader::DoLoadAlpha(char* aAlphaFileName)
	{
#if defined(__MSVCDOTNET__) || defined(__TOOLS2__)
	fstream file(aAlphaFileName, ios::in | ios::binary);
#else //!__MSVCDOTNET__
	fstream file(aAlphaFileName, ios::in | ios::binary | ios::nocreate);
#endif //__MSVCDOTNET__
	if (file.is_open()==0)
		return AlphaFiles;
	TBitmapFileHeader alphaBmpfile;
	long size=sizeof(TBitmapFileHeader);
	file.read((char *)&alphaBmpfile,size);
	if (file.gcount()!=size)
		return SourceFile;
	size=sizeof(TBitmapInfoHeader);
	TBitmapInfoHeader alphaBmpInfo;
	file.read((char *)&alphaBmpInfo,size);
	if (file.gcount()!=size)
		return SourceFile;
	if (alphaBmpInfo.biCompression != 0)
		return UnknownCompression;
	if (alphaBmpInfo.biWidth != iBmpHeader.biWidth || alphaBmpInfo.biHeight != iBmpHeader.biHeight)
		return AlphaDimensions;
	if (alphaBmpInfo.biBitCount != 8)
		return AlphaBpp;
	size=alphaBmpfile.bfSize-sizeof(TBitmapInfoHeader)-sizeof(TBitmapFileHeader);
	long numBmpColors=alphaBmpInfo.biClrUsed;
	if (numBmpColors == 0)
		numBmpColors = 256;
	if (numBmpColors != 256)
		return SourceFile;
	size-=numBmpColors*sizeof(TRgbQuad);
	iAlphaBits = new char[size];
	if (iAlphaBits == NULL)
		{
		return NoMemory;
		}
	memset(iAlphaBits,0xff,size);
	char* bmpColors = new char[numBmpColors*sizeof(TRgbQuad)];
	file.read((char *)bmpColors,numBmpColors*sizeof(TRgbQuad));
	delete [] bmpColors; // we aren't interested in the palette data for the 8bpp grayscale alpha bmp
	if (file.gcount()!=(int)(numBmpColors*sizeof(TRgbQuad)))
		return SourceFile;
	file.read(iAlphaBits,size);
	file.close();
	if (file.gcount()!=size)
		return SourceFile;
	return NoError;
	}

TRgb BitmapLoader::GetBmpPixel(long aXCoord,long aYCoord)
	{
	TRgb darkgray(128,128,128);
	TRgb darkgrayex(127,127,127);
	TRgb lightgray(192,192,192);
	TRgb lightgrayex(187,187,187);
	unsigned char col;
	TRgb color;

	switch(iBmpHeader.biBitCount)
		{
		case 1:
			col=iBmpBits[(iBmpHeader.biHeight-aYCoord-1)*(((iBmpHeader.biWidth+31)>>5)<<2)+(aXCoord>>3)];
			col&=(0x80>>(aXCoord%8));
			if (iBmpColors)
				{
				TRgbQuad rgbq;
				if (col)
					rgbq = iBmpColors[1];
				else
					rgbq = iBmpColors[0];
				color = TRgb(rgbq.iRed,rgbq.iGreen,rgbq.iBlue);
				}
			else
				{
				if (col)
					color = TRgb(0x00ffffff);
				else
					color = TRgb(0);
				}
			break;
		case 4:
			col=iBmpBits[(iBmpHeader.biHeight-aYCoord-1)*(((iBmpHeader.biWidth+7)>>3)<<2)+(aXCoord>>1)];
			if (aXCoord%2==0)
				col=(unsigned char)(col>>4);
			col&=0x0f;
			if (iBmpColors)
				{
				TRgbQuad rgbq = iBmpColors[col];
				color = TRgb(rgbq.iRed,rgbq.iGreen,rgbq.iBlue);
				}
			else
				{
				col *= 17;
				color = TRgb(col,col,col);
				}
			break;
		case 8:
			col=iBmpBits[(iBmpHeader.biHeight-aYCoord-1)*((iBmpHeader.biWidth+3)&~3)+aXCoord];
			if (iBmpColors)
				{
				TRgbQuad rgbq = iBmpColors[col];
				color = TRgb(rgbq.iRed,rgbq.iGreen,rgbq.iBlue);
				}
			else
				color = TRgb(col,col,col);
			break;
		case 16:
			{
			unsigned short int* wordptr = (unsigned short int*)&iBmpBits[(iBmpHeader.biHeight-aYCoord-1)*(((iBmpHeader.biWidth+1)&~1)<<1)+(aXCoord<<1)];
			color = TRgb((*wordptr&0x7c)>>10,(*wordptr&0x3e)>>5,(*wordptr&0x1f));
			}
			break;
		case 24:
			{
			TRgbTriple rgbcol = *((TRgbTriple *)&(iBmpBits[(iBmpHeader.biHeight-aYCoord-1)*((3*iBmpHeader.biWidth+3)&~3)+aXCoord+(aXCoord<<1)]));
			color = TRgb(rgbcol.rgbtRed,rgbcol.rgbtGreen,rgbcol.rgbtBlue);
			}
			break;
		case 32:
			{
			unsigned long int* dwordptr = (unsigned long int*)&iBmpBits[(iBmpHeader.biHeight-aYCoord-1)*((iBmpHeader.biWidth)<<2)+(aXCoord<<2)];
			color = TRgb((*dwordptr&0xff0000)>>16,(*dwordptr&0xff00)>>8,*dwordptr&0xff);
			}
			break;
		default:
			break;
		}
	if (color == darkgray)
		color = darkgrayex;
	else if (color == lightgray)
		color = lightgrayex;
	return color;
	}

unsigned char BitmapLoader::GetAlphaPixel(long aXCoord,long aYCoord)
	{
	return iAlphaBits[(iBmpHeader.biHeight-aYCoord-1)*((iBmpHeader.biWidth+3)&~3)+aXCoord];
	}

int BitmapLoader::DoConvert(int aBpp,TBitmapColor aColor,SEpocBitmapHeader*& aPbm)
	{
	bool useAlpha = (aColor==EColorBitmapAlpha);
	long desttwipswidth = 0;
	long desttwipsheight = 0;

	long bytewidth = BitmapUtils::ByteWidth(iBmpHeader.biWidth,aBpp);
	long destlength = iBmpHeader.biHeight * bytewidth;

	if (iBmpHeader.biXPelsPerMeter>0)
		desttwipswidth = iBmpHeader.biWidth*1440000/254/iBmpHeader.biXPelsPerMeter;
	if (iBmpHeader.biYPelsPerMeter>0)
		desttwipsheight = iBmpHeader.biHeight*1440000/254/iBmpHeader.biYPelsPerMeter;

	aPbm = (SEpocBitmapHeader*)new char[sizeof(SEpocBitmapHeader) + destlength];
	if (aPbm == NULL)
		return NoMemory;
	memset(aPbm,0,sizeof(SEpocBitmapHeader));

	// aBitmap->iByteWidth = bytewidth;
	// aBitmap->iDataOffset = sizeof(Bitmap);

	aPbm->iBitmapSize = sizeof(SEpocBitmapHeader) + destlength;
	aPbm->iStructSize = sizeof(SEpocBitmapHeader);
	aPbm->iWidthInPixels = iBmpHeader.biWidth;
	aPbm->iHeightInPixels = iBmpHeader.biHeight;
	aPbm->iWidthInTwips = desttwipswidth;
	aPbm->iHeightInTwips = desttwipsheight;
	aPbm->iBitsPerPixel = aBpp;
	aPbm->iColor = aColor;
	aPbm->iPaletteEntries = 0;
	aPbm->iCompression = ENoBitmapCompression;

	char* pbmBits = ((char*)aPbm) + sizeof(SEpocBitmapHeader);
	memset(pbmBits,0xff,destlength);

	long col = 0;
	char* pixadd = 0;
	bool indicesOnly = false;
	if (aColor == EColorBitmap && aBpp == 8)
		{
		indicesOnly =
			(iNumBmpColors==256 &&
			 iBmpColors[KNokiaBrandGreenIndex].iRed == KNokiaBrandGreen.iRed &&
			 iBmpColors[KNokiaBrandGreenIndex].iBlue == KNokiaBrandGreen.iBlue &&
			 iBmpColors[KNokiaBrandGreenIndex].iGreen == KNokiaBrandGreen.iGreen &&
			 iBmpColors[KNokiaBrandBlueIndex].iRed == KNokiaBrandBlue.iRed &&
			 iBmpColors[KNokiaBrandBlueIndex].iBlue == KNokiaBrandBlue.iBlue &&
			 iBmpColors[KNokiaBrandBlueIndex].iGreen == KNokiaBrandBlue.iGreen);
		}

	switch(aBpp)
		{
	case 1:
		{
		for(long ycrd=0;ycrd<iBmpHeader.biHeight;ycrd++)
			for(long xcrd=0;xcrd<iBmpHeader.biWidth;xcrd++)
				{
				TRgb color=GetBmpPixel(xcrd,ycrd);
				col=color.Gray2();
				pixadd=&(pbmBits[ycrd*bytewidth+(xcrd>>3)]);
				(*pixadd)&=~(1<<((xcrd&7)));
				(*pixadd)|=(unsigned char)(col<<(xcrd&7));
				}
		}
		break;
	case 2:
		{
		for(long ycrd=0;ycrd<iBmpHeader.biHeight;ycrd++)
			for(long xcrd=0;xcrd<iBmpHeader.biWidth;xcrd++)
				{
				TRgb color=GetBmpPixel(xcrd,ycrd);
				col=color.Gray4();
				pixadd=&(pbmBits[ycrd*bytewidth+(xcrd>>2)]);
				(*pixadd)&=~(0x3<<(2*(xcrd%4)));
				(*pixadd)|=(unsigned char)(col<<(2*(xcrd%4)));
				}
		}
		break;
	case 4:
		{
		for(long ycrd=0;ycrd<iBmpHeader.biHeight;ycrd++)
			for(long xcrd=0;xcrd<iBmpHeader.biWidth;xcrd++)
				{
				TRgb color=GetBmpPixel(xcrd,ycrd);
				if (aColor == EMonochromeBitmap)
					col = color.Gray16();
				else
					col = color.Color16();
				pixadd=&(pbmBits[ycrd*bytewidth+(xcrd>>1)]);
				if (xcrd%2!=0)
					*pixadd=(unsigned char)((unsigned char)((col<<4)|(*pixadd&0x0f)));
				else
					*pixadd=(unsigned char)((unsigned char)(col|(*pixadd&0xf0)));
				}
		}
		break;
	case 8:
		{
		for(long ycrd=0;ycrd<iBmpHeader.biHeight;ycrd++)
			for(long xcrd=0;xcrd<iBmpHeader.biWidth;xcrd++)
				{
				if (indicesOnly)
					{
					unsigned char pixel = iBmpBits[(iBmpHeader.biHeight-ycrd-1)*((iBmpHeader.biWidth+3)&~3)+xcrd];
					col = ColorRemapTable[pixel];
					iSourceFile->SetCompileInfo(TSourceFile::ENokiaBitmap);
					}
				else
					{
					TRgb color=GetBmpPixel(xcrd,ycrd);
					if (aColor == EMonochromeBitmap)
						col = color.Gray256();
					else
						col = color.Color256();
					iSourceFile->SetCompileInfo(TSourceFile::EThirdPartyBitmap);
					}
				pixadd=&(pbmBits[ycrd*((iBmpHeader.biWidth+3)&~3)+xcrd]);
				*pixadd=(unsigned char)col;
				}
		}
		break;
	case 12:
		{
		for(long ycrd=0;ycrd<iBmpHeader.biHeight;ycrd++)
			for(long xcrd=0;xcrd<iBmpHeader.biWidth;xcrd++)
				{
				TRgb color=GetBmpPixel(xcrd,ycrd);
				col=color.Color4K();
				pixadd=&(pbmBits[ycrd*bytewidth+(xcrd<<1)]);
				unsigned short* wordadd=(unsigned short*)pixadd;
				*wordadd=(unsigned short)col;
				}
		}
		break;
	case 16:
		{
		for(long ycrd=0;ycrd<iBmpHeader.biHeight;ycrd++)
			for(long xcrd=0;xcrd<iBmpHeader.biWidth;xcrd++)
				{
				TRgb color=GetBmpPixel(xcrd,ycrd);
				col=color.Color64K();
				pixadd=&(pbmBits[ycrd*bytewidth+(xcrd<<1)]);
				unsigned short* wordadd=(unsigned short*)pixadd;
				*wordadd=(unsigned short)col;
				}
		}
		break;
	case 24:
		{
		for(long ycrd=0;ycrd<iBmpHeader.biHeight;ycrd++)
			{
			unsigned char* bytePtr = (unsigned char*)&pbmBits[ycrd*bytewidth];
			for(long xcrd=0;xcrd<iBmpHeader.biWidth;xcrd++)
				{
				TRgb col = GetBmpPixel(xcrd,ycrd);
				*bytePtr++ = (unsigned char) col.iBlue;
				*bytePtr++ = (unsigned char) col.iGreen;
				*bytePtr++ = (unsigned char) col.iRed;
				}
			}
		}
		break;
	case 32:
		{
		for(long ycrd=0;ycrd<iBmpHeader.biHeight;ycrd++)
			{
			unsigned char* bytePtr = (unsigned char*)&pbmBits[ycrd*bytewidth];
			for(long xcrd=0;xcrd<iBmpHeader.biWidth;xcrd++)
				{
				TRgb col = GetBmpPixel(xcrd,ycrd);
				unsigned char alpha = useAlpha?GetAlphaPixel(xcrd,ycrd):(unsigned char)0;
				*bytePtr++ = (unsigned char) col.iBlue;
				*bytePtr++ = (unsigned char) col.iGreen;
				*bytePtr++ = (unsigned char) col.iRed;
				*bytePtr++ = alpha;	
				}
			}
		}
		break;
		};

	return NoError;
	}

