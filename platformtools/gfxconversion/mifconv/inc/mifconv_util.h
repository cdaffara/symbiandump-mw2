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


#ifndef __MIFCONVUTIL_H__
#define __MIFCONVUTIL_H__

#include "mifconv.h"
#include "mifconv_sourcefile.h"
#include <sstream>

class MifConvUtil
{
public:

    static int ByteWidth(int aPixelWidth,int aBitsPerPixel);
    
    /**
     * Returns file extension of the given filename
     * @return String representing the file extension
     * @param fileName String representing the file name.
     */
	static MifConvString FileExtension( const MifConvString& fileName );

    /**
     * Returns file name without the extension
     * @return String representing the file name without the extension
     * @param fileName String representing the file name.
     */
    static MifConvString FilenameWithoutExtension( const MifConvString& fileName );

    /**
     * Returns file name without the path
     * @return String representing the file name without the path
     * @param fileName String representing the file name.
     */
    static MifConvString FilenameWithoutPath( const MifConvString& fileName );

    /**
     * Tells whether the file with the given filename exists.
     * @return true if the given file exists, false otherwise.
     * @param fileName File name
     */
    static bool FileExists( const MifConvString& fileName ); 

    /**
     * Returns file size
     * @return File size
     * @param fileName File name
     */
    static size_t FileSize( const MifConvString& fileName );

    /**
     * Reads file contents and returns pointer to data buffer. Returns also
     * the data size in bytes.
     * @return std::pair object containing the pointer to the allocated data buffer and the 
     * data size in byte. The caller must take the ownership of the returned data pointer.
     * @param fileName Filename
     */
    static MifConvFileData FileContents( const MifConvString& fileName );

    /**
     * Converts the given object to string. Can be used for example to converting integers to string:
     * <code>
     * int i = 0;
     * MifConvString iAsString(MifConvUtil::ToString(i));
     * </code>
     *
     * @return String built from the given object.
     * @param s Reference to the object to be converted to string.
     */
    template <typename T>
    static MifConvString ToString(const T& s)
    {
        std::stringstream ss;
        ss << s;
        return MifConvString(ss.str());
    }

    /**
     * Replaces given characters to other one from the given string.
     * @param str String from which the characters will be replaced.
     * @param replaceFrom This character will be replaced by other one.
     * @param replaceTo Characters will be replaced by this.
     */
    static void ReplaceChar( MifConvString& str, char replaceFrom, char replaceTo);

    /**
     * Replaces given strings to other one from the given string.
     * @param str String from which the characters will be replaced.
     * @param replaceFrom This section of the str will be replaced by other one.
     * @param replaceTo Characters will be replaced by this.
     */
    static void ReplaceStr( MifConvString& str, 
                                  MifConvString replaceFrom, MifConvString replaceTo);
    
    /** 
     * Tells whether the given character is a whitespace character or not.
     * @param c Charater to be investigated.
     * @return true if the given character is whitespace character, false otherwise.
     */
    static bool IsWhiteSpace(char c);

    /**
     * Splits the given path to drive and directory path parts.
     * @param sourcePath Directory path to be splitted.
     * @param drive Drive part of the given path, if any.
     * @param path Directory path of the given path, if any.
     */
    static void SplitPath( const MifConvString& sourcePath, MifConvString& drive, MifConvString& path);

    /**
     * Compares two string. Ignores the case of the letters.
     * @return zero if the strings are equal
     * @param lhs Left hand side string
     * @param rhs Right hand side string
     */
    static int CompareIgnoreCase( const MifConvString& lhs, const MifConvString& rhs );

    /**
     * Replaces all the charactes by lower case characters.
     * @return String with lower case letters.
     * @param str Input string
     */
    static MifConvString& ToLower( MifConvString& str );

    /**
     * Splits the given string using the given separator.
     * @param str Input string
     * @param separator The input string is splitted using this sub-string as a separator
     * @param components List of the sub-strings splitted from the original input string
     */
    static void SplitString( const MifConvString& str, const MifConvString& separator, MifConvStringList& components );

    /**
     * Ensures that the given path exists. Tries to create it if it does not exist.
     * @param destFileName Filename that is to be checked.
     * @param ignoreLast If true, last part of the <code>destFileName</code> is stripped of 
     * Directory separator used as a separator.
     */
    static void EnsurePathExists( const MifConvString& destFileName, bool ignoreLast = false );

    /**
     * Removes duplicated directory separators from the given string.
     * @param str String from which the duplicated directory separators are to be searched and removed.
     */
    static void RemoveDuplicateDirSeparators( MifConvString& str );

    /**
     * Creates a directory with given path
     * @return True if the directory was successfully created, false otherwise.
     * @param path Directory path
     */
    static bool CreateDirectory( const MifConvString& path );

    /**
     * Trims the filename so that first letter is capitalized and others are lower case.
     * @return Trimmed string
     * @param filename Filename to be trimmed.
     */
    static MifConvString UnadornedFilename( const MifConvString& filename );

    /**
     * Finds the correct mask file for the bitmap file and sets it to the given object.
     * @param srcFile Source file object.
     */
    static void FindAndSetBitmapMaskFile( MifConvSourceFile& srcFile );

    /**
     * Returns a unique temporary file name.
     * @return Temporary file name string.
     */
    static MifConvString TemporaryFilename();

    /**
     * Copies a file 
     * @return True if the copying was successful, false otherwise.
     * @param from File which is to be copied from
     * @param to Destination file.
     */
    static bool CopyFile(const MifConvString& from, const MifConvString& to);

    /**
     * Returns current working directory path.
     * @return Current working directory path.
     */
    static MifConvString CurrentPath();

    /**
     * Changes the working directory
     * @param dirName Working directory name
     */
    static void ChangeDirectory( const MifConvString& dirName );

    /**
     * Removes a directory
     * @return Zero if the removal was successful, non-zero otherwise.
     * @param dirName Directory name
     */
    static int RemoveDirectory( const MifConvString& dirName );

    /**
     * Removes a file
     * @param fileName Filename
     * @param maxTries Maximum number of file deletion times in case of failure.
     * @param noException If true, no exception is thrown in case of failure. Warning is 
     * printed in debug log. If false, <code>MifConvException</code> is thrown in case of failure.
     */
    static void RemoveFile( const MifConvString& fileName, int maxTries = 1, bool noException = false );
            
    /**
     * Returns MifConv debug filename specified in MIFCONV_DEBUG_FILE
     * @return Debug file name
     */
    static MifConvString DebugFile();

    /**
     * Logs string to debug file
     * @param debugStr Debug string to be logged.
     */
    static void DebugLog(const MifConvString& debugStr);
    
    /**
     * Returns default temp directory for intermediate and temporary files
     * @return Temporary directory name
     */
    static const MifConvString& DefaultTempDirectory();

private:
    enum MifConvDebugMode
    {
        DebugMode_Unknown,
        DebugMode_NoDebug,
        DebugMode_Debug
    };

    static MifConvDebugMode iDebugMode;
    static MifConvString iDebugFile;
    static MifConvString iTempDirectory;
};


#endif
