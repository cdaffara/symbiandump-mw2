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
* Description:  Mifconv bitmap converters class.
*
*/


#include "mifconv.h"
#include "mifconv_bitmapconverter.h"
#include "mifconv_util.h"
#include "mifconv_exception.h"
#include "mifconv_argumentmanager.h"
#include "mifconv_mbmgenerator.h"
#include "mifconv_mbmgenerator_pbmcomp.h"
#include <stdio.h>
#include <sys/stat.h>

using namespace std;

#ifdef __linux__
const char* DefaultPaletteFileSearchLocations[] =
{
    "/epoc32/include/mw/ThirdPartyBitmap.pal",
    "/epoc32/include/middleware/ThirdPartyBitmap.pal",
    "/epoc32/include/ThirdPartyBitmap.pal"
};
#else
const char* DefaultPaletteFileSearchLocations[] =
{
    "\\epoc32\\include\\mw\\ThirdPartyBitmap.pal",
    "\\epoc32\\include\\middleware\\ThirdPartyBitmap.pal",
    "\\epoc32\\include\\ThirdPartyBitmap.pal"
};
#endif

/**
 *
 */
MifConvBitmapConverter::MifConvBitmapConverter()
{
    MifConvArgumentManager* argMgr = MifConvArgumentManager::Instance();
    // Output file:
    iTargetFilename = MifConvUtil::FilenameWithoutExtension(argMgr->TargetFile()) + "." + MifConvString(MBM_FILE_EXTENSION);
}

/**
 *
 */
MifConvBitmapConverter::~MifConvBitmapConverter()
{
}

/**
 *
 */
void MifConvBitmapConverter::Init()
{
    CleanupTargetFiles();
}

/**
 *
 */
void MifConvBitmapConverter::CleanupTargetFiles()
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
void MifConvBitmapConverter::AppendFile( const MifConvSourceFile& aSourcefile )
{    
    if( MifConvUtil::FileExtension( aSourcefile.Filename() ) == BMP_FILE_EXTENSION )
    {
        iSourceFiles.push_back( aSourcefile );
    }
}

/**
 *
 */
void MifConvBitmapConverter::Convert()
{
    if( iSourceFiles.size() > 0 )
    {
        ConvertToMbm();
    }
}

/**
 *
 */
void MifConvBitmapConverter::Cleanup(bool err)
{
    CleanupTempFiles();
    if( err )
    {
        CleanupTargetFiles();
    }
}

/**
 *
 */
void MifConvBitmapConverter::ConvertToMbm()
{   
    MifConvArgumentManager* argMgr = MifConvArgumentManager::Instance();
    const MifConvString& bmconvPath = argMgr->StringValue(MifConvBmconvPathArg);
    
    // If bmconv.exe's location is given.
    if( bmconvPath.length() > 0 ) 
    {
        // Create and initialize the temp file.  
        InitTempFile();
    
        // Run external bmconv.
        RunExternalBmconv(bmconvPath);
    }
    else // Use internal logic.
    {
        // Create the path if not existing.
        MifConvUtil::EnsurePathExists(iTargetFilename, true);
        
        cout << "Writing mbm: " << iTargetFilename << endl; 
    
        // Variable for source files.
        MifConvSourceFileList sourceFiles;
        
        // Get palette file name from either arguments or default values.
        MifConvString palettefilename = GetPaletteFileName();
        
        // Add filenames to the temp file.
        // Sourcefiles and their mask files are considered as same.
        for( MifConvSourceFileList::iterator i = iSourceFiles.begin(); i != iSourceFiles.end(); ++i )
            {               
            cout << "Loading file: " << i->Filename() << endl;
            sourceFiles.push_back( *i );              
            // Prepare also for the case that mask is not used at all.
            if (i->BmpMaskFilename().length() > 0 )
                {
                cout << "Loading file: " << i->BmpMaskFilename() << endl;
                MifConvSourceFile maskFile;
                maskFile.SetFilename(i->BmpMaskFilename());
                maskFile.SetDepthString(i->MaskDepthString());
                sourceFiles.push_back( maskFile );
                }
            }
       
        // Create internal bmconv to handle compiling.
        BmConv bmConv = BmConv(sourceFiles);
        int ret = bmConv.Compile(iTargetFilename.c_str(), palettefilename); 
        if (ret) {
            THROW_ERROR_COMMON("BMP to MBM conversion failed", MifConvString(__FILE__), __LINE__);
        }

    }
}

const MifConvString MifConvBitmapConverter::GetPaletteFileName() 
    {
    
    // Get palette argument.
    MifConvArgumentManager* argMgr = MifConvArgumentManager::Instance();
    const MifConvString& paletteArg = argMgr->StringValue(MifConvPaletteFileArg);
    
    MifConvString palettefilename;       
    MifConvString paletteBuf;
    
    // If palette argument has been given.
    if( paletteArg.length() == 0 )
        {
        // Default palette file.
        const MifConvString& defaultPalettefile = DefaultPaletteFileName(paletteBuf);
        
        // Palette file not given in arguments. If the default palette file
        // was found, use it, otherwise throw error.
        if( defaultPalettefile.length() > 0 )
            {
            palettefilename = defaultPalettefile;           
            }
        else
            {
            // Palettefile not given in arguments nor found from default locations:
            THROW_ERROR_COMMON("Unable to open palette file from default locations! ", MifConvString(__FILE__), __LINE__ );
            }
        } else {
        palettefilename = paletteArg;           
        }   
    
    // Check that palette file exists.
    if( !MifConvUtil::FileExists(palettefilename) )
    {
        THROW_ERROR_COMMON("Unable to open palette file! " + paletteArg, MifConvString(__FILE__), __LINE__ );
    }
    
    return palettefilename;
    }

const MifConvString& MifConvBitmapConverter::DefaultPaletteFileName(MifConvString& aBuf)
    {
    int numOfSearchLocations = sizeof(DefaultPaletteFileSearchLocations)/sizeof(char*);
    
    // Get epocroot.
    MifConvString epocRoot(MifConvArgumentManager::Instance()->EpocRoot());
    
    for( int i = 0; i < numOfSearchLocations; ++i )
        {
        aBuf = epocRoot;
        aBuf += DefaultPaletteFileSearchLocations[i];
        
        // Remove possible double-\.
        if ((aBuf.at(1) == '\\') && (aBuf.at(0) == '\\')) aBuf = aBuf.substr(1, aBuf.length()-1);
        if( MifConvUtil::FileExists(aBuf))
            {
            return aBuf;
            }
        }
    aBuf = "";
    return aBuf;
    }

/**
 *
 */
void MifConvBitmapConverter::InitTempFile()
{
    // Take an instance from the argument manager.
    MifConvArgumentManager* argMgr = MifConvArgumentManager::Instance();

    // Construct temp file name.
    iTempDir = MifConvUtil::DefaultTempDirectory();
    
    const MifConvString& tempDirArg = argMgr->StringValue(MifConvTempPathArg);
    if( tempDirArg.length() > 0 )
    {
        iTempDir = tempDirArg;
    }

    if( iTempDir.length() > 0 && iTempDir.at(iTempDir.length()-1) != DIR_SEPARATOR2 )
    {
        iTempDir.append(DIR_SEPARATOR);
    }

    // Generate new temp-filename:
    iTempDir.append(MifConvUtil::TemporaryFilename());

    // append tmp at as postfix
    // this is needed because the generated name can contain a single period '.'
    // character as the last character which is eaten away when the directory created.
    iTempDir.append(MifConvString("tmp"));

    MifConvUtil::EnsurePathExists(iTempDir);

    iTempDir.append(DIR_SEPARATOR);

    iTempFilename = iTempDir + MifConvUtil::FilenameWithoutExtension(MifConvUtil::FilenameWithoutPath(argMgr->TargetFile()));
    iTempFilename += BMCONV_TEMP_FILE_POSTFIX;
    
    // Create temp file
    fstream tempFile(iTempFilename.c_str(), ios::out|ios::binary|ios::trunc);
    if (!tempFile.is_open())
    {        
        throw MifConvException(MifConvString("Unable to create tmp file! ") + iTempFilename);    
    }

    try {
        // quiet mode
        // ex. "-q"
        tempFile << BMCONV_OPTION_PREFIX << BMCONV_QUIET_PARAMETER << " ";
        
        // Palette argument
        const MifConvString& paletteArg = argMgr->StringValue(MifConvPaletteFileArg);
        if( paletteArg.length() > 0 )
        {
            // ex. "-p"
            tempFile << BMCONV_OPTION_PREFIX << BMCONV_PALETTE_PARAMETER;            
            tempFile << MifConvString(paletteArg + " ");
        }

        tempFile << iTargetFilename << " ";
        
        // Add filenames to the temp file
        for( MifConvSourceFileList::iterator i = iSourceFiles.begin(); i != iSourceFiles.end(); ++i )
        {
            AppendBmpToTempFile(tempFile, *i);
        }
    }
    catch(...) {
        tempFile.close();
        throw;
    }    
    tempFile.close();
}

/**
 * Run external bmconv from given path.
 */
void MifConvBitmapConverter::RunExternalBmconv( const MifConvString& aBmconvPath )
{    
    // Build bmconv command    
    MifConvString bmconvCommand("\""); // Open " mark
    
    bmconvCommand += aBmconvPath; // If the path is given, use it.
        
    // Ensure that the last char of the path is dir-separator:
    if( bmconvCommand.length() > 1 && bmconvCommand.at(bmconvCommand.length()-1) != DIR_SEPARATOR2 )
        bmconvCommand += DIR_SEPARATOR;

    // Then add bmconv executable call and close the " mark
    bmconvCommand += BMCONV_EXECUTABLE_NAME + MifConvString("\" ");  
    bmconvCommand += "\"" + iTempFilename + "\"";
            
    MifConvUtil::EnsurePathExists(iTargetFilename, true);
        
    cout << "Writing mbm: " << iTargetFilename << endl;           
    int err = 0;
      
#ifdef __linux__
    if ((err = system (MifConvString(bmconvCommand).c_str())) != 0)   // Returns 0 if success
#else
    if ((err = system (MifConvString("\""+bmconvCommand+"\"").c_str())) != 0)   // Returns 0 if success
#endif
    {
            THROW_ERROR_COMMON("Executing BMCONV failed", MifConvString(__FILE__), __LINE__);
    }
}

/**
 *
 */
void MifConvBitmapConverter::CleanupTempFiles()
{
    if( iTempFilename.length() > 0 && remove( iTempFilename.c_str() ) != 0 )
    {
        perror( "Error deleting temporary file (bitmap conversion)" );
    }
    
    if( iTempDir.length() > 0 && MifConvUtil::RemoveDirectory( iTempDir ) != 0 )
    {
        perror( "Error deleting temporary directory (bitmap conversion)" );
    }
}

/**
 *
 */
void MifConvBitmapConverter::AppendBmpToTempFile(fstream& aStream, const MifConvSourceFile& aBmpFile)
    {
    cout << "Loading file: " << aBmpFile.Filename() << endl;
    
    // ex. "-8..\..\bitmaps\mifconv_test_bitmap_01.bmp"
    aStream << BMCONV_OPTION_PREFIX;
    aStream << aBmpFile.DepthString();
    aStream << aBmpFile.Filename();
    aStream << " ";
        
    // Prepare also for the case that mask is not used at all.
    const MifConvString& maskName = aBmpFile.BmpMaskFilename();
    if (maskName.length() > 0 )
    {
        cout << "Loading file: " << maskName << endl;
        
        // ex. -8..\..\bitmaps\mifconv_test_bitmap_01_mask_soft.bmp 
        aStream << BMCONV_OPTION_PREFIX;
        aStream << aBmpFile.MaskDepthString();
        aStream << maskName;      
    }
    aStream << " ";    
    }
