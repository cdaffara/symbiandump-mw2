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
* Description:  Mifconv's bitmap converters class.
*
*/


#ifndef __MIFCONVMBMGENERATOR_H__
#define __MIFCONVMBMGENERATOR_H__

#include "mifconv.h"
#include "mifconv_util.h"

#ifdef __linux__
    #define GNUPACK __attribute__((packed)) 
    #define OPTCHAR '-'
    /* on Linux the '/' optchar cannot be supported as filenames can start with '/'. */
    #define ALTERNATE_OPTCHAR '-'
#else
    #define GNUPACK
    #define OPTCHAR '-' 
    #define ALTERNATE_OPTCHAR '/'
#endif

const long int KWriteOnceFileStoreUid=268435511;
const long int KCBitwiseBitmapUid=268435520;
const long int KMultiBitmapRomImageUid=268435521;
const long int KMultiBitmapFileImageUid=268435522;
const long int KMultiBitmapFileImageChecksum=1194943545;

enum TBitmapColor
    {
    EMonochromeBitmap,
    EColorBitmap,
    EColorBitmapAlpha,  
    EColorBitmapUndefined   
    };

struct TBitmapInfoHeader
    {
    unsigned long biSize;
    long biWidth;
    long biHeight;
    unsigned short biPlanes;
    unsigned short biBitCount;
    unsigned long biCompression;
    unsigned long biSizeImage;
    long biXPelsPerMeter;
    long biYPelsPerMeter;
    unsigned long biClrUsed;
    unsigned long biClrImportant;
    };

#ifndef __linux__ 
#include <pshpack2.h>
#endif 

enum Errors
    {
    NoCompression=-1,
    NoError=0,
    NoMemory,
    Files,
    SourceFile,
    DestFile,
    OutOfRange,
    TooManyArgs,
    UnknownCompression,
    CompressionError,
    DecompressionError,
    Bpp,
    PaletteFile,
    PaletteSupportNotImplemented,
    AlphaFiles,
    AlphaDimensions,
    AlphaBpp
    };

/** 
Defines the types of file compression. 
*/
enum TBitmapfileCompression
    {
    /** Bitmap file is not compressed. */
    ENoBitmapCompression=0,
    /** File is compressed using run-length encoding compression. */
    EByteRLECompression,
    /** File is compressed using twelve bit run-length encoding compression. */
    ETwelveBitRLECompression,
    /** File is compressed using sixteen bit run-length encoding compression. */
    ESixteenBitRLECompression,
    /** File is compressed using twenty four bit run-length encoding compression. */
    ETwentyFourBitRLECompression,
    /** File is compressed using unsigned thirty two bit run-length encoding compression. */
    EThirtyTwoUBitRLECompression,
    /** File is compressed using unsigned thirty two bit run-length encoding compression and includes an alpha channel. */
    EThirtyTwoABitRLECompression,
    /** Insert new compression types here */
    ERLECompressionLast = 255
    };

struct SEpocBitmapHeader
/** Contains information about the bitmap when streaming bitmaps to stores. */
    {
    /** The size of the bitmap data, in bytes. */
    long iBitmapSize;
    /** The size of the structure in which the bitmap data is stored. */
    long iStructSize;
    long iWidthInPixels;
    long iHeightInPixels;
    long iWidthInTwips;
    long iHeightInTwips;
    /** The bitmaps number of bits per pixel */
    long iBitsPerPixel;
    /** Whether or not the bitmap is colour. A true value means that the bitmap 
    is colour. */
    TBitmapColor iColor;
    /** The number of entries in the bitmap's palette. */
    long iPaletteEntries;   // always 0 in current implementations
    /** The type of compression used to store the bitmap. */
    TBitmapfileCompression iCompression;
    };

#ifndef __linux__ 
#include <pshpack2.h>
#endif 

struct TBitmapFileHeader
    {
    unsigned short bfType;
    unsigned long bfSize;
    unsigned short bfReserved1;
    unsigned short bfReserved2;
    unsigned long bfOffBits;
    } GNUPACK;
#ifndef __linux__ 
#include <poppack.h>
#endif 
    
struct TRgbTriple
    {
    unsigned char rgbtBlue;
    unsigned char rgbtGreen;
    unsigned char rgbtRed;
    } GNUPACK;
#ifndef __linux__ 
#include <poppack.h>
#endif 

class TRgb
/** 
24-bit RGB colour value with 8 bits each for red, green and blue.

All Graphics drawing functions are specified in terms of a 32-bit TRgb colour 
containing the three colour values plus 8 unused bits. For hardware which 
does not support 24-bit colour, a mapping from TRgb to display colours is 
performed. 

The supported display modes are enumerated in the TDisplayMode type. In each 
display mode a unique index can represent each physical colours supported, 
and which can be mapped onto a full RGB value. The mappings are as follows: 

16-colour displays use the EGA colour set: black, white, and then both light 
and dark versions of grey, red, green, blue, cyan, magenta and yellow

256-colour displays support 216 colours made up of 6x6x6 RGB values, each 
containing all possible multiples of 51 for R,G,B values. Additionally, all 
remaining 10 shades of pure red, green, blue and grey are represented, by 
adding all remaining multiples of 17. This use of 256 colours is sometimes 
known as the Netscape colour cube.

4096-colour displays effectively support RGB values with 4 bits per primary 
colour

64k-colour displays effectively support RGB values with 5 bits allocated to 
red, 6 to green and 5 to blue

16 million-colour displays support true colour with 8 bits allocated to each 
primary colour

@see TDisplayMode
@see DynamicPalette 
*/
    {
public:
    TRgb();
    TRgb(long unsigned int val);
    TRgb(int aRed,int aGreen,int aBlue);
    TRgb &operator=(const TRgb& aColor);
    int operator==(const TRgb& aColor);
    int Difference(const TRgb& col) const;
    int Gray2() const;
    int Gray4() const;
    int Gray16() const;
    int Gray256() const;
    int Color16() const;
    int Color256() const;
    int Color4K() const;
    int Color64K() const;
    long int Color16M() const;
    static TRgb Gray2(int aGray2);
    static TRgb Gray4(int aGray4);
    static TRgb Gray16(int aGray16);
    static TRgb Gray256(int aGray256);
    static TRgb Color16(int aColor16);
    static TRgb Color256(int aColor256);
    static TRgb Color4K(int aColor64K);
    static TRgb Color64K(int aColor64K);
    static TRgb Color16M(long int aColor16M);
public:
    unsigned char iRed;
    unsigned char iGreen;
    unsigned char iBlue;
    unsigned char iSpare;
    };

/**
 * Converter class for converting .bmp files to .mbm file.
 */
class BmConv
{
public:

    /**
     * Default constructor
     * @param aSourcefilenames Source file names to be compiled.
     */ 
    BmConv(MifConvSourceFileList aSourcefilenames);

    /**
     * Destructor
     */
    ~BmConv();
   
    /**
     * Compile source files given in constructor to destination file using a given palette.
     * @param aDestfilename Location of destination file.
     * @param aPaletteFilename Location of palette file.
     */
    int Compile(const MifConvString& aDestfilename, const MifConvString& aPaletteFilename);

protected:

    MifConvString ErrorMessage(int aErrorNumber, MifConvString aDestfile, bool aDestCreated);
    void Report(int aError);
    
private:
    
    /**
     * List os source files.
     */
    MifConvSourceFileList iSourcefilenames;
};

#endif
