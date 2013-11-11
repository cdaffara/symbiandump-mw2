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
* Description:  Mifconv argument manager class.
*
*/


#include "mifconv.h"
#include "mifconv_argumentmanager.h"
#include "mifconv_exception.h"
#include "mifconv_util.h"

// Static singleton initialization
MifConvArgumentManager* MifConvArgumentManager::iInstance = 0;

/**
 * Returns pointer to the singleton object
 */
MifConvArgumentManager* MifConvArgumentManager::Instance()
{
    if( iInstance == 0 )
    {
        iInstance = new MifConvArgumentManager();
    }
    return iInstance;
}

/**
 * Free the allocated memory
 */
void MifConvArgumentManager::Reset()
{
    delete iInstance;
    iInstance = 0;
}

/**
 *
 */
inline void MifConvArgumentManager::THROW_USAGE_EXCEPTION() const
{
	MifConvString usageStr;
	SetUsageString(usageStr);
	throw MifConvException(usageStr, MifConvString(__FILE__), __LINE__);
}

inline void MifConvArgumentManager::THROW_ERROR( const MifConvString& errorMsg, const MifConvString& file, int line ) const
{    
    throw MifConvException( MifConvString("ERROR: " + errorMsg + "\nType mifconv -? for help\n"), file, line);
}

/**
 *
 */
MifConvArgumentManager::MifConvArgumentManager()
:
iEpocRoot(DEFAULT_EPOCROOT)
{
    GetMifEnv();
}

/**
 *
 */
MifConvArgumentManager::~MifConvArgumentManager()
{}

/**
 * This function checks if the given argument is boolean type of argument.
 * Boolean type arguments are listed in a <code>MifConvBooleanArguments</code> table
 * and this function checks if the given string matches any of those. Returns the length
 * of the argument name if found, zero otherwise.
 */
size_t MifConvArgumentManager::IsBooleanArgument( const MifConvString& argName ) const
{
	if( IsArgument(argName) )
	{
		try {
			int tblSize = sizeof(MifConvBooleanArguments) / sizeof(MifConvString);
			for( int i = 0; i < tblSize; ++i )
			{
				size_t tmpLen = MifConvBooleanArguments[i].length();
				if( argName.length() > tmpLen )
				{
                    if( MifConvUtil::CompareIgnoreCase(MifConvString(argName.begin()+1, argName.begin() + 1 + tmpLen), MifConvBooleanArguments[i]) == 0 )
					{
						return tmpLen;
					}
				}
			}
		}
		catch(...)
		{
			THROW_USAGE_EXCEPTION();
		}
	}
	return 0;
}

/**
 * This function checks if the given argument is a help argument.
 * Help arguments are listed in a <code>MifConvHelpArguments</code> table
 * and this function checks if the given string matches any of those. Returns the length
 * of the argument name if found, zero otherwise.
 */
size_t MifConvArgumentManager::IsHelpArgument( const MifConvString& argName ) const
{
	if( IsArgument(argName) )
	{
		try {
			int tblSize = sizeof(MifConvHelpArguments) / sizeof(MifConvString);
			for( int i = 0; i < tblSize; ++i )
			{
				size_t tmpLen = MifConvHelpArguments[i].length();

                // Following check separates -H from -Hheadername.mbg parameter:
                if( argName.length() == tmpLen+1 )
				{
                    if( MifConvUtil::CompareIgnoreCase(MifConvString(argName.begin()+1, argName.begin() + 1 + tmpLen), MifConvHelpArguments[i]) == 0 )
					{
						return tmpLen;
					}
				}
			}
		}
		catch(...)
		{
			THROW_USAGE_EXCEPTION();
		}
	}
	return 0;
}

/**
 * This function checks if the given argument is string type of argument.
 * String type arguments are listed in a <code>MifConvStringArguments</code> table
 * and this function checks if the given string matches any of those. Returns the length
 * of the argument name if found, zero otherwise.
 */
size_t MifConvArgumentManager::IsStringArgument( const MifConvString& argName ) const
{    
	if( IsArgument(argName) )
	{
		try {
			int tblSize = sizeof(MifConvStringArguments) / sizeof(MifConvString);
			for( int i = 0; i < tblSize; ++i )
			{
				size_t tmpLen = MifConvStringArguments[i].length();
				if( argName.length() > tmpLen )
				{
                    if( MifConvUtil::CompareIgnoreCase(MifConvString(argName.begin()+1, argName.begin()+1+tmpLen), MifConvStringArguments[i]) == 0 )
					{
						return tmpLen;
					}
				}				
			}
		}
		catch(...)
		{
			THROW_USAGE_EXCEPTION();
		}
	}
	return 0;
}

/**
 * This function checks if the given argument is string list type of argument.
 * String list type arguments are listed in a <code>MifConvStringListArguments</code> table
 * and this function checks if the given string matches any of those. Returns the length
 * of the argument name if found, zero otherwise.
 */
size_t MifConvArgumentManager::IsStringListArgument( const MifConvString& argName ) const
{
	if( IsArgument(argName) )
	{
		try {
			int tblSize = sizeof(MifConvStringListArguments) / sizeof(MifConvString);
			for( int i = 0; i < tblSize; ++i )
			{
				size_t tmpLen = MifConvStringListArguments[i].length();
				if( argName.length() > tmpLen )
				{
                    if( MifConvUtil::CompareIgnoreCase(MifConvString(argName.begin()+1, argName.begin()+1+tmpLen), MifConvStringListArguments[i]) == 0 )
					{
						return tmpLen;
					}
				}				
			}
		}
		catch(...)
		{
			THROW_USAGE_EXCEPTION();
		}
	}
	return 0;
}

/**
 *
 */
bool MifConvArgumentManager::IsDepthArgument( const MifConvString& argName ) const
{
	if( IsArgument(argName) )
	{
		try {
			int tblSize = sizeof(MifConvDepthArguments) / sizeof(MifConvString);
			for( int i = 0; i < tblSize; ++i )
			{
				size_t tmpLen = MifConvDepthArguments[i].length();
                if( argName.length() > tmpLen )
				{
                    MifConvString trimmedArgument(argName.begin()+1, argName.begin() + 1 + tmpLen);                
                    if( MifConvUtil::CompareIgnoreCase(trimmedArgument, MifConvDepthArguments[i]) == 0 )
					{
						return true;
					}                    
				}
			}
		}
		catch(...)
		{
			THROW_USAGE_EXCEPTION();
		}
	}
	return false;
}

/**
 *
 */
bool MifConvArgumentManager::IsAnimatedFlag( const MifConvString& argName ) const
{
	return IsArgument( argName ) && argName.length() > MifConvAnimatedIconArg.length() && 
        MifConvUtil::CompareIgnoreCase(MifConvString(argName.begin()+1, argName.end() ), MifConvAnimatedIconArg ) == 0;
}

/**
 *
 */
void MifConvArgumentManager::SetTargetFile( const MifConvString& arg )
{
	iTargetFile = arg;
}

/**
 *
 */
const MifConvString& MifConvArgumentManager::TargetFile() const
{
	return iTargetFile;
}

/**
 *
 */
IconDisplayMode MifConvArgumentManager::ConvertToDisplayMode(IconDepth depth) const
{
    MifConvIconDisplayModeMap::const_iterator i = iDisplayModeMap.find(depth);
    if( i != iDisplayModeMap.end() )
        return i->second;

    return DisplayMode_None;    
}

/**
 *
 */
IconDisplayMode MifConvArgumentManager::ConvertToMaskDisplayMode(IconMaskDepth depth) const
{
    MifConvMaskIconDisplayModeMap::const_iterator i = iMaskDisplayModeMap.find(depth);
    if( i != iMaskDisplayModeMap.end() )
        return i->second;

    return DisplayMode_None;    
}

/**
 *
 */
IconDepth MifConvArgumentManager::ConvertToDepth( const MifConvString& depthStr ) const
{
    MifConvIconDepthMap::const_iterator i = iDepthMap.find(depthStr);
    if( i != iDepthMap.end() )
        return i->second;

    return IconDepth_Undefined;    
}
/**
 *
 */
IconMaskDepth MifConvArgumentManager::ConvertToMaskDepth( const MifConvString depthStr ) const
{
    MifConvIconMaskDepthMap::const_iterator i = iMaskDepthMap.find(depthStr);
    if( i != iMaskDepthMap.end() )
        return i->second;

    return IconMaskDepth_Undefined;    
}

/**
 *
 */
void MifConvArgumentManager::Init( const MifConvStringList& argList )
{	    
    // Build maps for mapping depth, mask and displaymode constants:
    PopulateDepthAndMaskMaps();
    // Allocate search paths where to search source files:    
    MifConvString epocRoot(EpocRoot());
         
    // Global icons folder can contain only .svg files:
    iSearchRules.push_back(MifConvSourceSearchRule(MifConvString(epocRoot + S60_ICONS_PATH), vector<MifConvString>(1, SVG_FILE_EXTENSION)));
    // Global bitmaps folder can contain only .bmp files:
    iSearchRules.push_back(MifConvSourceSearchRule(MifConvString(epocRoot + S60_BITMAPS_PATH), vector<MifConvString>(1, BMP_FILE_EXTENSION)));
    // If not found from global icons folder, try subfolder nss:
    iSearchRules.push_back(MifConvSourceSearchRule(MifConvString(epocRoot + S60_ICONS_PATH + DIR_SEPARATOR + "nss"), vector<MifConvString>(1, SVG_FILE_EXTENSION)));
    // If not found from nss sub folder, try subfolder nokia:
    iSearchRules.push_back(MifConvSourceSearchRule(MifConvString(epocRoot + S60_ICONS_PATH + DIR_SEPARATOR + "nokia"), vector<MifConvString>(1, SVG_FILE_EXTENSION)));
    // If not found from nokia sub folder, try subfolder oem:
    iSearchRules.push_back(MifConvSourceSearchRule(MifConvString(epocRoot + S60_ICONS_PATH + DIR_SEPARATOR + "oem"), vector<MifConvString>(1, SVG_FILE_EXTENSION)));    
    // EPOCROOT, if given in environment variables:
    if( epocRoot.length() > 0 )
    {
        iSearchRules.push_back(MifConvSourceSearchRule(epocRoot+EPOC32_PATH, vector<MifConvString>(1, MIFCONV_WILDCARD)));
    }

    AddArguments(argList);

    // check if the parameter file is given:
    const MifConvString& paramFilename = StringValue(MifConvParameterFileArg);
    if( paramFilename.length() > 0 )
    {
        // Add arguments from the parameter file:
        MifConvStringList paramListFromFile;
        ReadParameterFile( paramFilename, paramListFromFile );
        if( paramListFromFile.size() > 0 )
        {
            AddArguments(paramListFromFile, true);
        }
    }
    // Resolve file type extensions using given flags and investigating the existing files:
	FinalizeArguments();
}

/**
 * Read string argument value:
 */
MifConvString MifConvArgumentManager::ReadStringArgument(const MifConvStringList& argList, MifConvStringList::const_iterator& i, unsigned int argNameLen)
{    
    // Take the actual argument value, for example /TmyTempDir --> myTempDir    
    MifConvString argValue((*i).begin() + argNameLen, (*i).end());
    if( argValue.length() > 0 )
    {
        // String arguments can have spaces when they are enclosed with " marks (For example directory names).
        if( argValue[0] == '\"' )
        {
            MifConvString quotedArgValue(argValue);
            // Check if the last char is also ":
            if( quotedArgValue[ quotedArgValue.length()-1 ] == '\"' )
            {                
                return quotedArgValue;
            }

            // See if the next string ends with \" mark, for example "My Folder" is presented with following argument list:
            // argList[0] = "My
            // argList[1] = Folder"
            while(++i != argList.end())
            {
                MifConvString nextString((*i).begin(), (*i).end());
                quotedArgValue += " " + nextString;
                if( nextString[ nextString.length()-1 ] == '\"' )
                {                    
                    return "\"" + quotedArgValue + "\"";
                }
            }
        }
    }    
    return argValue;
}

/**
 * Read string argument value:
 */
void MifConvArgumentManager::ReadStringListArgument(MifConvStringList::const_iterator& i, unsigned int argNameLen, MifConvStringList& StringValueList)
{    
    // Take the actual argument value, for example /imyInputDir;myTempDir --> myTempDir    
    MifConvString argValue((*i).begin() + argNameLen, (*i).end());    
    MifConvUtil::SplitString( argValue, STRING_LIST_ARGUMENT_SEPARATOR, StringValueList );
}

/**
 *
 */
void MifConvArgumentManager::AddArguments( const MifConvStringList& argList, bool paramsFromFile )
{
	MifConvStringList::const_iterator i = argList.begin();

	if( i == argList.end() )
	{		
        THROW_ERROR("No arguments", MifConvString(__FILE__), __LINE__);	
	}

    // Check if help is needed:
    while( i != argList.end() )
    {
        if( IsHelpArgument(*i) )
        {
            THROW_USAGE_EXCEPTION();
        }
        ++i;
    }

    i = argList.begin();

	while( i != argList.end() )
	{	        
		unsigned int argLen = 0;
		if( i == argList.begin() && paramsFromFile == false )
		{
			// First command line argument must be the target file.
            // If the given list (argList) is read from the file, then
            // the first one is not target file.
			if( !IsArgument(*i) )
			{           
			    MifConvString targetFile(ReadStringArgument( argList, i, 0 ));			    
			    // Make sure that the file extension is .mif:
			    targetFile = MifConvUtil::FilenameWithoutExtension(targetFile);
			    targetFile += MifConvString(FILE_EXTENSION_SEPARATOR) + MifConvString(MIF_FILE_EXTENSION);
				SetTargetFile(targetFile);                
			}
			else
			{
                THROW_ERROR( "Target file must be given as first argument.", MifConvString(__FILE__), __LINE__ );                
			}
			++i;
            if( i == argList.end() )
            {
                THROW_ERROR("Missing arguments", MifConvString(__FILE__), __LINE__);
            }
		}
		else if( IsBooleanArgument(*i) )
		{
			// Insert boolean type argument to the boolean arguments list:
			MifConvString argName((*i).begin()+1, (*i).end());
            MifConvUtil::ToLower(argName); // Lower the cases to make comparison easier later
            std::pair<BooleanArgMap::iterator, bool> res = iBooleanArguments.insert(std::make_pair<MifConvString, MifConvBooleanArgument>(
				argName, MifConvBooleanArgument( argName, true )));
            if( res.second == false )
            {
                // parameter already exists in the map, update the value:
                res.first->second = MifConvBooleanArgument( argName, true );
            }
			++i;
		}
		else if( (argLen = (unsigned int) IsStringArgument(*i) ) > 0 )
		{			
			MifConvString argName((*i).begin()+1, (*i).begin() + 1 + argLen);
            MifConvUtil::ToLower(argName); // Lower the cases to make comparison easier later			
            MifConvString argValue(ReadStringArgument( argList, i, argLen+1 ));            
			if( argValue.length() == 0 )
			{
				// Do not accept string arguments with zero length (E.g. "/H")
                THROW_ERROR( "Missing argument value for " + *i, MifConvString(__FILE__), __LINE__ );                
			}
			// Insert string type argument to the string arguments list:
			std::pair<StringArgMap::iterator, bool> res = iStringArguments.insert(std::make_pair<MifConvString, MifConvStringArgument>(
				argName, MifConvStringArgument( argName, argValue )));
            if( res.second == false )
            {
                // parameter already exists in the map, update the value:
                res.first->second = MifConvStringArgument( argName, argValue );
            }
			++i;
		}
        else if( (argLen = (unsigned int) IsStringListArgument(*i)) > 0 )
        {
            MifConvString argName((*i).begin()+1, (*i).begin() + 1 + argLen);
            MifConvUtil::ToLower(argName); // Lower the cases to make comparison easier later			
            MifConvStringList argValue;
            ReadStringListArgument( i, argLen+1, argValue );

            if( argValue.size() == 0 )
			{
				// Do not accept string arguments with zero length (E.g. "/H")
                THROW_ERROR( "Missing argument value for " + *i, MifConvString(__FILE__), __LINE__ );                
			}
			// Insert string list type argument to the string arguments list:
			std::pair<StringListArgMap::iterator, bool> res = iStringListArguments.insert(std::make_pair<MifConvString, MifConvStringListArgument>(
				argName, MifConvStringListArgument( argName, argValue )));
            if( res.second == false )
            {
                // parameter already exists in the map, update the value:
                res.first->second = MifConvStringListArgument( argName, argValue );
            }
			++i;
        }
		else if( IsDepthArgument(*i) )
		{
			// Let's build source file argument...
			// ... first is depth and mask:
			MifConvString depthAndMask(*i);
            MifConvUtil::ToLower(depthAndMask); // Lower the cases to make comparison easier later
			++i;
            // Check that there is still an argument:
            if( i == argList.end() )
            {
                THROW_ERROR( "Missing source file argument.", MifConvString(__FILE__), __LINE__ );                
            }

			// Then we check if animated flag is given next:
			bool isAnimated = IsAnimatedFlag(*i);

			if( isAnimated )
			{
				// This was an animated flag, so next must be filename:
				++i;
                // Check that there is still an argument:
                if( i == argList.end() )
                {
                    THROW_ERROR( "Missing source file argument.", MifConvString(__FILE__), __LINE__ );
                    //THROW_USAGE_EXCEPTION();
                }
			}
			
			// One more check... Check that the next string is not an argument (starting with '-' or '/')
			// It should be a filename for the source icon.
			if( IsArgument(*i) )
			{
                THROW_ERROR( "Missing source file argument.", MifConvString(__FILE__), __LINE__ );                
			}

            MifConvSourceFile srcFile;
            srcFile.SetDepthAndMask(depthAndMask);
            srcFile.SetDisplayMode(ConvertToDisplayMode(srcFile.Depth()));
            srcFile.SetMaskDisplayMode(ConvertToMaskDisplayMode(srcFile.MaskDepth()));
            srcFile.SetFilename(ReadStringArgument( argList, i, 0 ));
            srcFile.SetAnimated(isAnimated);
            iSourceFiles.push_back(srcFile);			
			++i;
		}
		else if( IsAnimatedFlag(*i) )
		{
			// Icon animated flag found
			// Let's see if the next is depth argument:
			++i;
			MifConvString depthAndMask;
			if( IsDepthArgument(*i) )
			{
				depthAndMask = *i;
                MifConvUtil::ToLower(depthAndMask);
				++i;
			}

			// One more check... Check that the next string is not an argument (starting with '-' or '/')
			if( IsArgument(*i) )
			{
                THROW_ERROR( "Missing source file argument.", MifConvString(__FILE__), __LINE__ );                
			}

            MifConvSourceFile srcFile;
            srcFile.SetDepthAndMask(depthAndMask);
            srcFile.SetDisplayMode(ConvertToDisplayMode(srcFile.Depth()));
            srcFile.SetMaskDisplayMode(ConvertToMaskDisplayMode(srcFile.MaskDepth()));
            srcFile.SetFilename(*i);
            srcFile.SetAnimated(true);
            iSourceFiles.push_back(srcFile);
			++i;
		}
		else
		{
            THROW_ERROR( "Invalid argument: " + *i, MifConvString(__FILE__), __LINE__ );            
        }
	}
}

/**
 * Resolves correct type for the source file. Sets also mask filenames for bmp-files:
 */
void MifConvArgumentManager::ResolveSourceFileTypes()
{
    bool extensionFlag = BooleanValue(MifConvUseExtensionArg);

    // check if the input directory is given:
    const MifConvStringList& inputDirList = StringListValue(MifConvIconSourceDirectory);

    // Add user-defined input directory to search directory list, put MIFCONV_WILDCARD
    // as filetype rule, because user defined directory can contain all supported filetypes:
    int indexcounter = 0;
    for( MifConvStringList::const_iterator iDir = inputDirList.begin(); iDir != inputDirList.end(); ++iDir )
    {
        MifConvSourceSearchRule customRule(*iDir, MifConvStringList(1, MIFCONV_WILDCARD));
        MifConvUtil::ReplaceChar(customRule.SearchPath(), INCORRECT_DIR_SEPARATOR2, DIR_SEPARATOR2);
        MifConvUtil::RemoveDuplicateDirSeparators(customRule.SearchPath());
        iSearchRules.insert(iSearchRules.begin()+indexcounter, customRule);
        
        ++indexcounter;
    }
    
    for( MifConvSourceFileList::iterator src = iSourceFiles.begin(); src != iSourceFiles.end(); ++ src )
    {
        if( extensionFlag )
        {
            MifConvString extension = MifConvUtil::FileExtension(src->Filename());
            if( !FindAndSetPathAndType( *src, extension ) )
            {                 
                THROW_ERROR_COMMON("File not found " + src->Filename(), MifConvString(__FILE__), __LINE__ );
            }
        }
        else
        {
            // "Use extension" -flag not given, so resolve extensions for source files
            if( !FindAndSetPathAndType( *src, SVGB_BINARY_FILE_EXTENSION ) )
            { 
                if( !FindAndSetPathAndType( *src, SVG_FILE_EXTENSION ) )
                {                
                    if( !FindAndSetPathAndType( *src, BMP_FILE_EXTENSION ) )
                    {                
                        THROW_ERROR_COMMON("File not found " + src->Filename(), MifConvString(__FILE__), __LINE__ );
                    }
                }
            }
        }
    }
}

/**
 *
 */
void MifConvArgumentManager::GetMifEnv()
{    
    // Read EPOCROOT environment variable
    char* tmpPtr = 0;
    tmpPtr = getenv(EPOCROOT_ENV.c_str());
    if( tmpPtr )
    {        
        iEpocRoot = MifConvString(tmpPtr);
        MifConvUtil::ReplaceChar(iEpocRoot, INCORRECT_DIR_SEPARATOR2, DIR_SEPARATOR2);
        // Make sure that the last char is directory separator
        if( iEpocRoot.length() > 0 && iEpocRoot.at( iEpocRoot.length()-1) != DIR_SEPARATOR2 )
        {
            iEpocRoot += DIR_SEPARATOR;
        }
    }
}

/**
 *
 */
const MifConvString& MifConvArgumentManager::EpocRoot() const
{
    return iEpocRoot;
}

/**
 *
 */
bool MifConvArgumentManager::FindAndSetPathAndType( MifConvSourceFile& srcFile, const MifConvString& extension )
{   
    // Search the filename first "as is":
    MifConvString tmp( MifConvUtil::FilenameWithoutExtension( srcFile.Filename() ) + MifConvString(FILE_EXTENSION_SEPARATOR) + extension );     
    if( MifConvUtil::FileExists(tmp) )
    {
        srcFile.SetFilename(tmp);
        MifConvUtil::FindAndSetBitmapMaskFile(srcFile);
        return true;
    }

    // If the absolute path was given, return false, because the file was not found with given path and filename. 
    // Otherwise continue searching.
    if( //(srcFile.Filename().length() > 0 && srcFile.Filename().at(0) == DIR_SEPARATOR2) ||
        (srcFile.Filename().length() > 1 && srcFile.Filename().at(1) == ':') )
    {        
        return false;
    }

    // Search from the pre-defined locations:        
    for( SearchRules::iterator i = iSearchRules.begin(); i != iSearchRules.end(); ++i )
    {
        bool validPath = false;
        const MifConvStringList& allowedTypes = i->AllowedFileTypes();

        // See if the file with given extension is allowed to locate in search path.
        // For example, epoc32\s60\icons folder can contain only .svg files and epoc32\s60\bitmaps 
        // can contain only .bmp files:
        for( MifConvStringList::const_iterator typeIter = allowedTypes.begin(); typeIter != allowedTypes.end(); ++typeIter )
        {
            if( *typeIter == MIFCONV_WILDCARD || *typeIter == extension )
            {        
                validPath = true;
                break;
            }
        }

        if( validPath )
        {            
            MifConvString searchPath(i->SearchPath());
                                  
            // Make sure that the last char is directory separator
            if( searchPath.length() > 0 && searchPath.at( searchPath.length()-1) != DIR_SEPARATOR2 )
            {
                searchPath += DIR_SEPARATOR;
            }

            searchPath += MifConvUtil::FilenameWithoutExtension( srcFile.Filename() ) + MifConvString(FILE_EXTENSION_SEPARATOR) + extension;

            MifConvUtil::RemoveDuplicateDirSeparators(searchPath);
           
            if( MifConvUtil::FileExists( searchPath ) )
            {
                srcFile.SetFilename(searchPath);
                MifConvUtil::FindAndSetBitmapMaskFile(srcFile);
                                
                return true; 
            }  
        }            
    }
    return false;
}

/**
 *
 */
void MifConvArgumentManager::ProcessArgumentPaths()
{
	// Fix directory separators first:
    for( StringArgMap::iterator i = iStringArguments.begin(); i != iStringArguments.end(); ++i )
    {
        MifConvString tmp = i->second.Value();
        MifConvUtil::ReplaceChar(tmp, INCORRECT_DIR_SEPARATOR2, DIR_SEPARATOR2);
        MifConvUtil::RemoveDuplicateDirSeparators(tmp);
        i->second.SetValue(tmp);
    }

    // Fix directory separators in source filenames also:
    for( MifConvSourceFileList::iterator j = iSourceFiles.begin(); j != iSourceFiles.end(); ++j )
    {
        MifConvString tmp = j->Filename();
        MifConvUtil::ReplaceChar(tmp, INCORRECT_DIR_SEPARATOR2, DIR_SEPARATOR2);
        MifConvUtil::RemoveDuplicateDirSeparators(tmp);
        j->SetFilename(tmp);
    }
    
    // Fix directory separators in search rule directories also:
    for( SearchRules::iterator k = iSearchRules.begin(); k != iSearchRules.end(); ++k )
    {
        MifConvString& tmp = k->SearchPath();
        MifConvUtil::ReplaceChar(tmp, INCORRECT_DIR_SEPARATOR2, DIR_SEPARATOR2);
        MifConvUtil::RemoveDuplicateDirSeparators(tmp);
    }

    // Fix target file also:
    MifConvUtil::ReplaceChar(iTargetFile, INCORRECT_DIR_SEPARATOR2, DIR_SEPARATOR2);
    MifConvUtil::RemoveDuplicateDirSeparators(iTargetFile);
}

/**
 *
 */
void MifConvArgumentManager::FinalizeArguments()
{
	ProcessArgumentPaths();
	ResolveSourceFileTypes();
}

/**
 *
 */
void MifConvArgumentManager::SetUsageString( MifConvString& usageStr ) const
{
	usageStr = "";

    usageStr += "Copyright (c) " + MifConvYears + " Nokia Corporation and/or its subsidiary(-ies). All rights reserved.\n";
    usageStr += "\n";
    usageStr += "Usage: mifconv <MIFFILE> [-F<file>] <options> <sources>]\n";
    usageStr += "\n";
    usageStr += "Where:\n";
    usageStr += "  MIFFILE            Specifies the target MIF file to be created\n";
    usageStr += "  -F<file>           Specifies a parameter file, which can contain any of the options\n";
    usageStr += "                        and sources separated by spaces or newlines\n";
    usageStr += "\n";
    usageStr += "Options:\n";
    usageStr += "  -H<file>           Specifies a name of the MIF header file (default extension MBG)\n";
    usageStr += "  -I<dir;dir;...>    Specifies a set of custom source directories where source files\n";
    usageStr += "                        will be searched. As a fallback, global source directories are\n";
    usageStr += "                        used\n";
    usageStr += "  -E                 Specifies that source icons are only loaded with given file\n";
    usageStr += "                        extensions. By default, Mifconv prefers source icons with\n";
    usageStr += "                        extension .SVG over .BMP, regardless of which is given as\n";
    usageStr += "                        a parameter\n";
    usageStr += "  -X                 Disables SVG compression. If this flag is set, SVG icons are\n";
    usageStr += "                        added to MIF file without compressing them first\n";
    usageStr += "  -P<file>           Specifies a path to custom palette file for bitmap files\n";
    usageStr += "  -T<dir>            Specifies a path where temporary files are created\n";
    usageStr += "  -B<file>           Specifies a path for non-default BMConv utility\n";
    usageStr += "  -S<file>           Specifies a path for non-default SVGTBinenCode utility\n";
    usageStr += "  -V<string>         Specifies a non-platform default format version of SVGT binary\n";
    usageStr += "                        conversion. It can be any of the following value:\n";
    usageStr += "                             1      BGR / float encoding\n";
    usageStr += "                             2      BGR / fixed point encoding\n";
    usageStr += "                             3      RGB / fixed point encoding\n";
    usageStr += "                             4      RGB / float encoding\n";
    usageStr += "Sources:\n";
    usageStr += "  [-A] <DEPTH[,MASK]> <FILE> [ [-A] <DEPTH[,MASK]> <FILE> ... ]\n";
    usageStr += "        [-A]         Specifies animated flag for the icon\n";
    usageStr += "        [DEPTH]      Specifies icon depth, it can be any of these values\n";
    usageStr += "                        -1,-2,-4,-8,-c4,-c8,-c12,-c16,-c24,-c32\n";
    usageStr += "        [MASK]       Specifies icon mask depth, it can be any of these values\n";
    usageStr += "                        1,8\n";
    usageStr += "        [FILE]       Specifies path to the input file, supported file extensions are\n";
    usageStr += "                        SVG, SVGB, BMP\n";
    usageStr += "\n";
    usageStr += "Other info:\n";
#ifdef WIN32
    usageStr += "  * '-' or '/' can be used as parameter switch prefix\n";
#endif    
    usageStr += "  * Value of icon mask and depth is meaningful only for bitmap files, but the mask\n";
    usageStr += "    value defines if mask entry will be available or not in the header file\n";
    usageStr += "  * If mask parameter is defined for a BMP file, Mifconv automatically pics\n";
    usageStr += "    a file ending _mask_soft for value 8 and _mask for value 1 of mask\n";
    usageStr += "\n";
    usageStr += "Examples:\n";
    usageStr += "  mifconv mybuttons.mif -Hmybuttons.mbg -c8,8 button1 -c8,8 button2\n";
}

/**
 *
 */
const MifConvString& MifConvArgumentManager::StringValue( const MifConvString& argName ) const
{
	StringArgMap::const_iterator i = iStringArguments.find(argName);
	if( i != iStringArguments.end() )
	{
		return i->second.Value();
	}
	return iDummyString;
}

/**
 *
 */
const MifConvStringList& MifConvArgumentManager::StringListValue( const MifConvString& argName ) const
{
	StringListArgMap::const_iterator i = iStringListArguments.find(argName);
	if( i != iStringListArguments.end() )
	{
		return i->second.Value();
	}
	
	return iDummyStringList;
}

/**
 *
 */
bool MifConvArgumentManager::BooleanValue( const MifConvString& argName ) const
{
	BooleanArgMap::const_iterator i = iBooleanArguments.find(argName);
	if( i != iBooleanArguments.end() )
	{
		return i->second.Value();
	}
	
	return false;
}

/**
 *
 */
const MifConvSourceFileList& MifConvArgumentManager::SourceFiles() const
{
	return iSourceFiles;
}

/**
 *
 */
bool MifConvArgumentManager::IsArgument( const MifConvString& str ) const
{
	try {
		return str.at(0) == OPTION_PREFIX1_CHAR || str.at(0) == OPTION_PREFIX2_CHAR;
	}
	catch(...)
	{
		THROW_ERROR("Zero or corrupted string in MifConvArgumentManager::IsArgument()\n", MifConvString(__FILE__), __LINE__);
	}
	return false;
}

/**
 *
 */
void MifConvArgumentManager::PopulateDepthAndMaskMaps()
{
    // Insert value-string pairs for the icon depths:
    iDepthMap.insert(std::make_pair(MifConvDepth_1,    IconDepth_1));
    iDepthMap.insert(std::make_pair(MifConvDepth_2,    IconDepth_2));
    iDepthMap.insert(std::make_pair(MifConvDepth_4,    IconDepth_4));
    iDepthMap.insert(std::make_pair(MifConvDepth_8,    IconDepth_8));
    iDepthMap.insert(std::make_pair(MifConvDepth_c4,   IconDepth_c4));
    iDepthMap.insert(std::make_pair(MifConvDepth_c8,   IconDepth_c8));
    iDepthMap.insert(std::make_pair(MifConvDepth_c12,  IconDepth_c12));
    iDepthMap.insert(std::make_pair(MifConvDepth_c16,  IconDepth_c16));
    iDepthMap.insert(std::make_pair(MifConvDepth_c24,  IconDepth_c24));
    iDepthMap.insert(std::make_pair(MifConvDepth_c32,  IconDepth_c32));
    
    // Insert value-string pairs for the icon masks:
    iMaskDepthMap.insert(std::make_pair(MifConvMaskDepth_1, IconMaskDepth_1));
    iMaskDepthMap.insert(std::make_pair(MifConvMaskDepth_8, IconMaskDepth_8));

    // Insert value-pairs for display modes:
    iDisplayModeMap.insert(std::make_pair(IconDepth_1,      DisplayMode_Gray2));
    iDisplayModeMap.insert(std::make_pair(IconDepth_2,      DisplayMode_Gray4));
    iDisplayModeMap.insert(std::make_pair(IconDepth_4,      DisplayMode_Gray16));
    iDisplayModeMap.insert(std::make_pair(IconDepth_8,      DisplayMode_Gray256));
    iDisplayModeMap.insert(std::make_pair(IconDepth_c4,     DisplayMode_Color16));
    iDisplayModeMap.insert(std::make_pair(IconDepth_c8,     DisplayMode_Color256));
    iDisplayModeMap.insert(std::make_pair(IconDepth_c12,    DisplayMode_Color4K));
    iDisplayModeMap.insert(std::make_pair(IconDepth_c16,    DisplayMode_Color64K));
    iDisplayModeMap.insert(std::make_pair(IconDepth_c24,    DisplayMode_Color16M));
    iDisplayModeMap.insert(std::make_pair(IconDepth_c32,    DisplayMode_Color16MU));

    iMaskDisplayModeMap.insert(std::make_pair(IconMaskDepth_1,  DisplayMode_Gray2));
    iMaskDisplayModeMap.insert(std::make_pair(IconMaskDepth_8,  DisplayMode_Gray256));
}

/**
 *
 */
void MifConvArgumentManager::ReadParameterFile(const MifConvString& paramFilename, MifConvStringList& paramList)
{
    // Check if the file exists:
    if( MifConvUtil::FileExists(paramFilename) == false )
    {
        THROW_ERROR_COMMON("Unable to open file for reading! " + paramFilename, MifConvString(__FILE__), __LINE__ );
    }

    MifConvFileData paramFileData = MifConvUtil::FileContents(paramFilename);    

    MifConvString tmpString;
    for(size_t i = 0; i < paramFileData.second; ++i )
    {
        if( MifConvUtil::IsWhiteSpace(paramFileData.first[i]) == false )
        {
            tmpString += paramFileData.first[i];
        }
        else if( tmpString.length() > 0 )
        {
            paramList.push_back( tmpString );            
            tmpString = MifConvString();       
        }
    }

    if( tmpString.length() > 0 )
    {
        paramList.push_back( tmpString );
        tmpString = MifConvString();
    }
    delete[] paramFileData.first;
}

/**
 * Helper class for source search rules
 */

MifConvSourceSearchRule::MifConvSourceSearchRule(const MifConvString& path, const MifConvStringList& types)
:
iSearchPath(path),
iAllowedFileTypes(types)
{}

MifConvSourceSearchRule::~MifConvSourceSearchRule()
{}

const MifConvString& MifConvSourceSearchRule::SearchPath() const 
{ 
    return iSearchPath; 
}

MifConvString& MifConvSourceSearchRule::SearchPath() 
{ 
    return iSearchPath; 
}

const MifConvStringList& MifConvSourceSearchRule::AllowedFileTypes() const
{
    return iAllowedFileTypes;
}
