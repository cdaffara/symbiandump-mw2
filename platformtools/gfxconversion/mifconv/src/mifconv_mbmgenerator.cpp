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

#include "mifconv_mbmgenerator.h"
#include "mifconv_mbmgenerator_pbmcomp.h"
#include "mifconv_util.h"

#include <stdio.h>
#include <sys/stat.h>

BmConv::BmConv(MifConvSourceFileList aSourcefilenames):
    iSourcefilenames(aSourcefilenames)
    {}

BmConv::~BmConv() { 
}

int BmConv::Compile(const MifConvString& aDestfilename, const MifConvString& aPaletteFilename)
    {     
    int ret = OutOfRange;
           
    BitmapCompiler bmcomp(iSourcefilenames);
    ret = bmcomp.Compile(aDestfilename, aPaletteFilename);
    
    Report(ret);
    return ret;
    }

/**
Returns an informative error message, the result of the program actions performed.
@return Informative error string
@param aErrorNumber The error returned from the actions performed
@param aDestfile The multiple bitmap store file name
@param aDestCreated True if the multiple bitmap store has been created/modified
*/
MifConvString BmConv::ErrorMessage(int aErrorNumber, MifConvString aDestfile="", bool aDestCreated=false)
    {
    // Remove the multiple bitmap store if it has been created/modified during an fstream session and there has been an error
    if((aDestfile.length() > 0) && (aErrorNumber != NoError) && (aDestCreated == true))
        {
        remove(aDestfile.c_str());
        }

    switch(aErrorNumber)
        {
        case NoError:
            return "Success.";
        case NoMemory:
            return "Out of memory.";
        case Files:
            return "File does not exist";
        case SourceFile:
            return "Bad source file(s).";
        case DestFile:
            return "Bad destination file(s).";
        case OutOfRange:
            return "Number of sources/targets mismatch.";
        case UnknownCompression:
            return "Unknown source compression type.";
        case CompressionError:
            return "Compression error.";
        case Bpp:
            return "Invalid bitmap mode specified.";
        case PaletteFile:
            return "Bad palette file.";
        case PaletteSupportNotImplemented:
            return "Palettes not supported";
        case AlphaFiles:
            return "Alpha bitmap file does not exist";
        case AlphaDimensions:
            return "Alpha channel bitmap's dimensions don't match pixel bitmap's dimensions.";
        case AlphaBpp:
            return "Alpha channel bitmap must be 8bpp.";
        default:
            return "Unknown error!";
        };
    }

void BmConv::Report(int aError)
    {
    if (aError > 0) 
        {
        cout << "BMP to MBM conversion error: ";
        cout << ErrorMessage(aError) << "\n";
        }
    }
