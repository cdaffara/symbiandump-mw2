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
* Description:  Mifconv utilities.
*
*/


#include "mifconv.h"
#include "mifconv_util.h"
#include "mifconv_exception.h"
#include "mifconv_argumentmanager.h"
#include <sys/stat.h>   // for stat
#ifdef WIN32
    #include <direct.h>     // for _getcwd, _chdir, _mkdir
#else
    #include <unistd.h>
#endif
#include <stdlib.h>     // for _MAX_PATH
#include <stdio.h>
#include <cctype>

#ifndef _MAX_PATH
#define _MAX_PATH   (260)
#endif

MifConvUtil::MifConvDebugMode MifConvUtil::iDebugMode = DebugMode_Unknown;
MifConvString MifConvUtil::iDebugFile = "";
MifConvString MifConvUtil::iTempDirectory = "";

int MifConvUtil::ByteWidth(int aPixelWidth,int aBitsPerPixel)
    {
    int wordWidth = 0;

    switch (aBitsPerPixel)
        {
    case 1:
        wordWidth = (aPixelWidth + 31) / 32;
        break;
    case 2:
        wordWidth = (aPixelWidth + 15) / 16;
        break;
    case 4:
        wordWidth = (aPixelWidth + 7) / 8;
        break;
    case 8:
        wordWidth = (aPixelWidth + 3) / 4;
        break;
    case 12:
    case 16:
        wordWidth = (aPixelWidth + 1) / 2;
        break;
    case 24:
        wordWidth = (((aPixelWidth * 3) + 11) / 12) * 3;
        break;
    case 32:
        wordWidth = aPixelWidth;
        break;
    default:
        break;
        };

    return wordWidth * 4;
    }

/**
 *
 */
MifConvString MifConvUtil::FileExtension( const MifConvString& fileName )
{	
	size_t indexOfDot; // index of '.' character in the given string

	// Find last occurence of the '.' character
	if( ( indexOfDot = fileName.find_last_of('.') ) == MifConvString::npos )
	{
		// Not found, return empty string
		return MifConvString("");
	}

	// Return the substring starting after the '.' character
	return MifConvString( fileName.begin()+indexOfDot+1, fileName.end() );
}

/**
 *
 */
MifConvString MifConvUtil::FilenameWithoutExtension( const MifConvString& fileName )
{	
	size_t indexOfDot; // index of '.' character in the given string

	// Find last occurence of the '.' character
	if( ( indexOfDot = fileName.find_last_of('.') ) == MifConvString::npos )
	{
		// Not found, return the whole name
		return fileName;
	}

	// Return the substring preceding the last '.' character
	return MifConvString( fileName.begin(), fileName.begin() + indexOfDot );
}

/**
 *
 */
MifConvString MifConvUtil::FilenameWithoutPath( const MifConvString& fileName )
{	
	size_t indexOfDirSeparator; // index of directory separator in the given string

	// Find last occurence of the '.' character
	if( ( indexOfDirSeparator = fileName.find_last_of(DIR_SEPARATOR2) ) == MifConvString::npos )
	{
		// Not found, return the whole name
		return fileName;
	}

	// Return the substring beginnig after the last directory separator
	return MifConvString( fileName.begin()+indexOfDirSeparator+1, fileName.end() );
}

/**
 *
 */
bool MifConvUtil::FileExists( const MifConvString& fileName )
{
  struct stat fileInfo;   
  int retVal = 0; 

  // Try to get file attributes to see if the file exists or not:
  retVal = stat( fileName.c_str(), &fileInfo); 
  return retVal == 0;
}

/**
 *
 */
size_t MifConvUtil::FileSize( const MifConvString& fileName )
{
  struct stat fileInfo;   
  int retVal = 0; 

  // Try to get file attributes to see if the file exists or not:
  retVal = stat( fileName.c_str(), &fileInfo); 
  if( retVal != 0 )
  {
      THROW_ERROR_COMMON("File not found: " + fileName, MifConvString(__FILE__), __LINE__);
  }

  return fileInfo.st_size;
}

/**
 *
 */
MifConvFileData MifConvUtil::FileContents( const MifConvString& fileName )
{
    unsigned int fileLen = (unsigned int) (MifConvUtil::FileSize(fileName)/sizeof(char));
    
    ifstream fs( fileName.c_str(), FILE_IN_BINARY_NOCREATE_FLAGS );
    if (!fs.is_open())
    {
        THROW_ERROR_COMMON("Unable to open file for reading! " + fileName, MifConvString(__FILE__), __LINE__);
    }
    char* buffer = new char[fileLen];
    fs.read(buffer, fileLen);
    return MifConvFileData(buffer, fileLen);
}

/**
 *
 */
void MifConvUtil::ReplaceChar( MifConvString& str, char replaceFrom, char replaceTo)
{
    if( str.length() > 0 )
    {        
        MifConvString::size_type index = 0;
        while( (index = str.find(replaceFrom, index)) != MifConvString::npos )
        {     
            str.replace(index,1,1,replaceTo);
        }
    }
}

/**
 *
 */
void MifConvUtil::ReplaceStr( MifConvString& str, 
                              MifConvString replaceFrom, MifConvString replaceTo)
{
    if( str.length() >= replaceFrom.length() )
    {        
        MifConvString::size_type index = 0;
        while( (index = str.find(replaceFrom, index)) != MifConvString::npos )
        {     
            MifConvString tmp_str = str.substr(0, index);
            tmp_str = tmp_str + replaceTo;
            str = tmp_str + str.substr(index+replaceFrom.length(),
                                           str.length()-index+replaceFrom.length()
                                           );
        }
    }
}

/**
 *
 */
void MifConvUtil::SplitPath( const MifConvString& sourcePath, MifConvString& drive, MifConvString& path)
    {    
        if( sourcePath.length() > 0 )
        {
            size_t driveSeparatorPos = sourcePath.find(':');
            if( driveSeparatorPos != MifConvString::npos )
            {
                drive = MifConvString(sourcePath.begin(), sourcePath.begin() + driveSeparatorPos );
                path = MifConvString(sourcePath.begin() + driveSeparatorPos + 1, sourcePath.end() );
            }
            else
            {
                path = sourcePath;
            }
        }
        else
        {
            drive = MifConvString("");
            path = MifConvString("");
        }
}

/**
 *
 */
MifConvString MifConvUtil::CurrentPath()
{
    char temp[_MAX_PATH];
#ifdef WIN32    
    _getcwd(temp, _MAX_PATH);    
#else
    getcwd(temp, _MAX_PATH);
#endif
    return MifConvString(temp);
}

/**
 *
 */
int MifConvUtil::CompareIgnoreCase( const MifConvString& lhs, const MifConvString& rhs )
{
    MifConvString lhsCopy(lhs);
    MifConvString rhsCopy(rhs);

    return ToLower(lhsCopy).compare(ToLower(rhsCopy));
}

/**
 *
 */
MifConvString& MifConvUtil::ToLower( MifConvString& str )
{
    MifConvString::iterator it(str.begin());
    for(; it != str.end(); ++it)
    {
        *it = (char) tolower((unsigned char)*it);
    }
    return str;
}

/**
 *
 */
void MifConvUtil::EnsurePathExists( const MifConvString& destFileName, bool ignoreLast )
    {
    MifConvString currentPath;
    MifConvString tmpDrive;    
    MifConvString tmpPath;

    // Parse a drive of a destination path; if any
    SplitPath( destFileName, tmpDrive, tmpPath );

    // Save current directory    
    currentPath = CurrentPath();

    // Change drive if needed:
    if( tmpDrive.length() > 0 )
    {        
        tmpDrive += ":";
        tmpDrive += DIR_SEPARATOR;
        ChangeDirectory(tmpDrive);        
    }

    // Split desination path to separate directories:
    MifConvStringList destDirList;

    // Check if the root is given first and add it to dir list:
    if( tmpPath.length() > 0 && tmpPath.at(0) == DIR_SEPARATOR2 )
    {
        destDirList.push_back(DIR_SEPARATOR);
    }

    // Add other directories to destination dir list:
    SplitString( tmpPath, DIR_SEPARATOR, destDirList );

    // Remove last component from the list if it should be ignored:
    if( ignoreLast )
    {
        destDirList.pop_back();
    }

    unsigned int i = 0;
    while( i < destDirList.size() )
    {        
        const MifConvString& dir = destDirList[i++];         
        if( !FileExists(dir) )
        {
            if( !CreateDirectory( dir ) )
            {
                // Change back to original directory:
                ChangeDirectory( currentPath );
                THROW_ERROR_COMMON("Directory " + tmpPath + " cannot be created", MifConvString(__FILE__), __LINE__);
            }
        }
        ChangeDirectory( dir );        
    }

    // Change back to original directory:
    ChangeDirectory( currentPath );
}

/**
 *
 */
void MifConvUtil::RemoveDuplicateDirSeparators( MifConvString& str )
{    
    MifConvString searchString(DIR_SEPARATOR);
    searchString += DIR_SEPARATOR;
    size_t pos = str.find(searchString);

    while(pos != MifConvString::npos)
    {
        str.erase(pos, 1);
        pos = str.find(searchString); 
    }
}

/**
 *
 */
bool MifConvUtil::CreateDirectory( const MifConvString& path )
{    
#ifdef WIN32
    return _mkdir( path.c_str() ) == 0;
#else
    return mkdir( path.c_str(), 0777 ) == 0;
#endif
}

/**
 *
 */
void MifConvUtil::SplitString( const MifConvString& str, const MifConvString& separator, MifConvStringList& components )
{
    size_t beginPos = 0;
    size_t endPos = 0;

    while( (endPos = str.find(separator, beginPos)) != MifConvString::npos )
    {
        if( endPos - beginPos > 0 )
        {
            components.push_back( MifConvString( str.begin()+beginPos, str.begin()+endPos ) );            
        }
        beginPos = endPos+1;
    }
    if( str.begin()+beginPos != str.end() )
    {
        components.push_back( MifConvString(str.begin()+beginPos, str.end()) );
    }
}

/**
 *
 */
MifConvString MifConvUtil::UnadornedFilename( const MifConvString& filename )
{    
    MifConvStringList splitted;
    SplitString(filename, DIR_SEPARATOR, splitted);

    MifConvString tmp(splitted.back());
    splitted.clear();
    SplitString(tmp, INCORRECT_DIR_SEPARATOR, splitted);

    tmp = splitted.back();
    splitted.clear();
    SplitString(tmp, FILE_EXTENSION_SEPARATOR, splitted);

    MifConvString fixedname;
    
    if( splitted.size() > 0 )
    {
        fixedname = ToLower(splitted[0]);
        if( fixedname.length() > 0 )
        {
            fixedname[0] = (char) toupper( fixedname[0] );
        }
        
        // Replace spaces (if any) with "_" in source filename.
        size_t found;
        while ((found = fixedname.rfind(" ")) != string::npos) {
            fixedname.replace (found,1,"_");
        }  
    }
    return fixedname;
}

/**
 *
 */
void MifConvUtil::FindAndSetBitmapMaskFile( MifConvSourceFile& srcFile )
{
    if( srcFile.MaskDepth() == IconMaskDepth_Undefined )
    {
        return; // No mask file
    }
    MifConvString fileExtension(FileExtension( srcFile.Filename() ));

    if( CompareIgnoreCase(fileExtension, BMP_FILE_EXTENSION) != 0 )
    {
        return;
    }

    MifConvString maskFilename(FilenameWithoutExtension(srcFile.Filename()));

    if( srcFile.MaskDepth() == IconMaskDepth_1 )
    {
        maskFilename += "_mask.bmp";
    }
    else if( srcFile.MaskDepth() == IconMaskDepth_8 )
    {
        maskFilename += "_mask_soft.bmp";
    }
    else
    {
        THROW_ERROR_COMMON("Invalid mask depth\n", MifConvString(__FILE__), __LINE__);
    }

    if( !FileExists(maskFilename) )
    {
        cerr << endl << "*** WARNING! Missing bitmap: " << maskFilename << endl;
    }

    srcFile.SetBmpMaskFilename( maskFilename );
}

/**
 *
 */
MifConvString MifConvUtil::TemporaryFilename()
{
    MifConvString tmp(tmpnam(NULL));
    MifConvString::iterator i = tmp.begin();

    if( *i == '.' )
    {
        ++i;
    }

    if( *i == DIR_SEPARATOR2 || *i == INCORRECT_DIR_SEPARATOR2 )
    {
        ++i;
    }

    return MifConvString(i, tmp.end());
}

/**
 *
 */
bool MifConvUtil::CopyFile(const MifConvString& from, const MifConvString& to)
{
    bool retval = false;
    ifstream in(from.c_str(), ios::binary);    
    if( in )
    {
        ofstream out(to.c_str(), ios::binary);        
        if( out )
        {
            out << in.rdbuf();
            retval = out.good();
            out.close();
        }
        in.close();
    }
    return retval;
} 

/**
 *
 */
bool MifConvUtil::IsWhiteSpace(char c)
{
    return c == ' ' || c == '\n' || c == '\r' || c == '\t';
}

/**
 *
 */
void MifConvUtil::ChangeDirectory( const MifConvString& dirName )
{
#ifdef WIN32
    _chdir( dirName.c_str() );
#else
    chdir( dirName.c_str() );
#endif
}

/**
 *
 */
int MifConvUtil::RemoveDirectory( const MifConvString& dirName )
{
#ifdef WIN32
    return _rmdir( dirName.c_str() );
#else
    return rmdir( dirName.c_str() );
#endif
}

/**
 *
 */
void MifConvUtil::RemoveFile( const MifConvString& fileName, int maxTries, bool noException )
{
    for( int i = 0; i < maxTries; ++i )
    {    
#ifdef WIN32
        int ret = _unlink(fileName.c_str());
#else
        int ret = unlink(fileName.c_str());
#endif
        
        if( ret == 0 )
        {        
            // Delete was successful
            return;
        }
        
        // Delete was not successful
        if( i >= maxTries-1 )
        {
            // Max amount of tries exceeded -> print warning or throw an exception
            if( noException )
            {
                MifConvString debugStr("WARNING: Cannot remove file " + fileName);
                DebugLog(debugStr);
            }
            else
            {
                THROW_ERROR_COMMON("Cannot remove file " + fileName, MifConvString(__FILE__), __LINE__);
            }
        }       
    }
}

/**
 *
 */
MifConvString MifConvUtil::DebugFile()
{
    char* tmpPtr = getenv(MIFCONV_DEBUG_FILE_ENV.c_str());
    if( tmpPtr )
    {
        return MifConvString(tmpPtr);        
    }
    return MifConvString();
}

/**
 *
 */
void MifConvUtil::DebugLog(const MifConvString& debugStr)
{
    if( iDebugMode == DebugMode_Unknown )
    {      
        iDebugFile = MifConvUtil::DebugFile();
        if( iDebugFile.length() > 0 )
        {
            iDebugMode = DebugMode_Debug;            
        }
        else
        {
            iDebugMode = DebugMode_NoDebug;
        }
    }

    if( iDebugMode == DebugMode_Debug )
    {
        ios_base::openmode file_flags;
        if( MifConvUtil::FileExists(iDebugFile) )
        {                
            file_flags = ios::out|ios::app;
        }
        else
        {             
            file_flags = ios::out|ios::trunc;
        }
        fstream debugLog(iDebugFile.c_str(), file_flags);
        if(debugLog.is_open())
        { 
            debugLog << debugStr << endl;
            debugLog.close();
        }
    }      
}

/**
 *
 */
const MifConvString& MifConvUtil::DefaultTempDirectory()
    {
    if( iTempDirectory.length() == 0 )
        {
        char* tmpPtr = getenv(SBS_BUILD_DIR_ENV.c_str());
        if( tmpPtr && MifConvString(tmpPtr).length() > 0 )
            {
            iTempDirectory = MifConvString(tmpPtr);
            
            if( iTempDirectory.at(iTempDirectory.length()-1) != DIR_SEPARATOR2 )
                {                
                iTempDirectory += DIR_SEPARATOR;
                }
            
            iTempDirectory += EPOC_ICON_BUILD_PATH_POSTFIX + 
                MifConvString(DIR_SEPARATOR) + 
                MifConvString("temp");
            }
        else
            {
            iTempDirectory = MifConvArgumentManager::Instance()->EpocRoot() + 
                EPOC_BUILD_PATH + 
                MifConvString(DIR_SEPARATOR) + 
                EPOC_ICON_BUILD_PATH_POSTFIX + 
                MifConvString(DIR_SEPARATOR) + 
                MifConvString("temp");
            }
        }
    return iTempDirectory;
    }

