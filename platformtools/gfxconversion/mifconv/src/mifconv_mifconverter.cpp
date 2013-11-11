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
* Description:  Mifconv MIF converters class.
*
*/


#include "mifconv.h"
#include "mifconv_mifconverter.h"
#include "mifconv_util.h"
#include "mifconv_exception.h"
#include "mifconv_argumentmanager.h"

// File versions:
// V1 - first version.
// V2 - stores information if icon location (MIF/MBM) inside the MIF file (bitmap offsets array).
// This way, icon IDs in MBG header can be always the same regardless of which icons are in MIF
// file and which in MBM file.
static const int KFileVersion(2);
static const int KIconVersion(1);

static const int FileHeaderSizeInBytes = 4*4;
static const int IconHeaderSizeInBytes = 4*8;
static const int KUidAvkonMultiIconFile(0x034232342);
static const int KUidAvkonMultiIcon(0x034232343);

//static const int IconFormatType_BMP(0);
//static const int IconFormatType_SVG(1);

static const unsigned int svgbSignatureLow (0x03FA56CC); //66737868
static const unsigned int svgbSignatureHigh(0x03FA56CF); //66737871

static const unsigned int nvgSignatureLow (0x0067766E); //('n'+'v'+'g'+0)
static const unsigned int nvgSignatureHigh(0xFF67766E); //('n'+'v'+'g'+255)

/**
 *
 */
MifConvMifConverter::MifConvMifConverter()
{
    MifConvArgumentManager* argMgr = MifConvArgumentManager::Instance();
    iTargetFilename = argMgr->TargetFile();
}

/**
 *
 */
MifConvMifConverter::~MifConvMifConverter()
{
    // Delete file contents
    for( StringPtrVector::iterator i = iContentPointers.begin(); i != iContentPointers.end(); ++i )
    {
        delete[] *i;
    }
}

/**
 *
 */
void MifConvMifConverter::Init()
{
    CleanupTargetFiles();
}

/**
 *
 */
void MifConvMifConverter::CleanupTargetFiles()
{
	if( MifConvUtil::FileExists(iTargetFilename) )
	{
        // Try to remove file MIFCONV_MAX_REMOVE_TRIES times, no exception in case of failure:
	    MifConvUtil::RemoveFile(iTargetFilename, MIFCONV_MAX_REMOVE_TRIES, true);
	}
}

/**
 *
 */
void MifConvMifConverter::AppendFile( const MifConvSourceFile& sourcefile )
{     
	if( MifConvUtil::FileExtension( sourcefile.Filename() ) == BMP_FILE_EXTENSION ||
        MifConvUtil::FileExtension( sourcefile.Filename() ) == SVG_FILE_EXTENSION ||
        MifConvUtil::FileExtension( sourcefile.Filename() ) == SVGB_BINARY_FILE_EXTENSION )
	{        
		iSourceFiles.push_back( sourcefile );
	}
}

void MifConvMifConverter::Convert()
{
 	ReadFileContents();
    cout << "Writing mif: " << MifConvArgumentManager::Instance()->TargetFile() << endl;
	ConvertToMif();
}

void MifConvMifConverter::ReadFileContents()
{   
    for( MifConvSourceFileList::iterator i = iSourceFiles.begin(); i != iSourceFiles.end(); ++i )
    {        
        // Just read the contents of the .svgb files
        if( MifConvUtil::FileExtension( i->Filename() ) != BMP_FILE_EXTENSION )
        {            
            if( MifConvUtil::FileExists(i->Filename()) == false )
            {
                THROW_ERROR_COMMON("Unable to open file for reading! " + i->Filename(), MifConvString(__FILE__), __LINE__ );
            }            
            cout << "Loading file: " << i->Filename() << endl;
            MifConvFileData retVal = MifConvUtil::FileContents(i->Filename());
            iContentPointers.push_back(retVal.first); // Save pointer for deleting it later
            i->SetContent(retVal.first, retVal.second);
        }
    }
}


void MifConvMifConverter::Cleanup(bool err)
{
	CleanupTempFiles();
	if( err )
	{
	    CleanupTargetFiles();
	}
}

void MifConvMifConverter::ConvertToMif()
{  
    try {
        OpenTargetFile();
    }
    catch( MifConvException& e )
    {
        // If file creation/opening was not successful, give warning and return:
        MifConvString debugStr("WARNING: Target file " + iTargetFilename + " cannot be opened for writing.");
        cout <<  debugStr  << endl;
        MifConvUtil::DebugLog(debugStr);
        return;
    }
    WriteTargetHeader();
    WriteIconArray();
    WriteIcons();
}

void MifConvMifConverter::OpenTargetFile()
{
    iTargetFile.open( iTargetFilename.c_str(), ios::out|ios::binary );

    if (!iTargetFile.is_open())
    {
        // Create path if it does not exist.
        MifConvUtil::EnsurePathExists(iTargetFilename, true);
        iTargetFile.clear();
        iTargetFile.open( iTargetFilename.c_str(), ios::out|ios::binary );
    }
    if (!iTargetFile.is_open())
    {
        THROW_ERROR_COMMON("Cannot write to file! " + iTargetFilename, MifConvString(__FILE__), __LINE__);
    }
}

void MifConvMifConverter::WriteTargetHeader()
    {
    size_t arraySize = iSourceFiles.size() * 2;
    int offset = FileHeaderSizeInBytes;
    iTargetFile.write( (char*) &KUidAvkonMultiIconFile, 4 );
    iTargetFile.write( (char*) &KFileVersion, 4 );
    iTargetFile.write( (char*) &offset, 4 );
    iTargetFile.write( (char*) &arraySize, 4 );
    }

void MifConvMifConverter::CleanupTempFiles()
{
}

void MifConvMifConverter::WriteIcons()
{    
    for( MifConvSourceFileList::iterator i = iSourceFiles.begin(); i != iSourceFiles.end(); ++i )
    {
        if( i->ContentLength() > 0 )
        {
            WriteIconHeader(*i);
            WriteIconData(*i);
        }
    }
}

MifConvDefs::IconFormatType MifConvMifConverter::ReadIconBinaryType(const MifConvSourceFile& src)
{
    MifConvDefs::IconFormatType ret = MifConvDefs::IconFormatType_SVG;
    
    if( src.ContentLength() >= 4 )
    {
        const char* iconData = src.Content();
        
        if( iconData[0] == 'n' &&
            iconData[1] == 'v' &&
            iconData[2] == 'g' )
        {
            ret = MifConvDefs::IconFormatType_NVG;
        }
    }
    
    return ret;
}

void MifConvMifConverter::WriteIconHeader(const MifConvSourceFile& src)
{
    int type = 0;
    if( MifConvUtil::FileExtension(src.Filename()) == BMP_FILE_EXTENSION )
        type = MifConvDefs::IconFormatType_BMP;
    else
        type = ReadIconBinaryType(src);
        //type = IconFormatType_SVG;

    int animated = (int) src.IsAnimated();
    int dataLen = src.ContentLength();
    int depth = src.DisplayMode();
    int mask = src.MaskDisplayMode();
    int dataoffset = IconHeaderSizeInBytes; // 8 = number of writes in this method.

    iTargetFile.write((char*)&KUidAvkonMultiIcon, 4); // 1
    iTargetFile.write((char*)&KIconVersion, 4);       // 2
    iTargetFile.write((char*)&dataoffset, 4);         // 3
    iTargetFile.write((char*)&dataLen, 4);        // 4
    iTargetFile.write((char*)&type, 4);          // 5
    iTargetFile.write((char*)&depth, 4);         // 6
    iTargetFile.write((char*)&animated, 4);      // 7
    iTargetFile.write((char*)&mask, 4);     // 8
}

void MifConvMifConverter::WriteIconData(const MifConvSourceFile& src)
{
    if( src.Content() )
    {        
        iTargetFile.write(src.Content(), src.ContentLength());
    }
}

/**
* 
*/
void MifConvMifConverter::WriteIconArray()
{
    int offset = (int) FileHeaderSizeInBytes +  (int)iSourceFiles.size()*4*2*2;
    MifConvSourceFileList::iterator i = iSourceFiles.begin();

    int mbmIndex = 0;
    int zero = 0;

    for( ; i != iSourceFiles.end(); ++i )
    {
        // MIF icon
        if( MifConvUtil::FileExtension(i->Filename()) != BMP_FILE_EXTENSION )
        {            
            int length = i->ContentLength() + IconHeaderSizeInBytes;
            iTargetFile.write( (char*) &offset, 4 );
            iTargetFile.write( (char*) &length, 4 );
            // same information for the mask...
            iTargetFile.write( (char*) &offset, 4 );
            iTargetFile.write( (char*) &length, 4 );

            offset += length;            
        }
        // MBM icon
        else
        {
            iTargetFile.write( (char*) &mbmIndex, 4 );
            iTargetFile.write( (char*) &zero, 4 );

            // Masked MBM icon -> There is own MBM index for the mask.
            if(i->MaskDepth() != IconMaskDepth_Undefined )
            {
                mbmIndex--;
            }

            iTargetFile.write( (char*) &mbmIndex, 4 );
            iTargetFile.write( (char*) &zero, 4 );

            // MBM incides are coded as negative in the 'offset' field,
            // so that they can be easily separated from the actual MIF offsets.
            mbmIndex--;
        }
    }
}
