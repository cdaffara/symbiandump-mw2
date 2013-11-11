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


#ifndef __MIFCONVARGUMENTMANAGER_H__
#define __MIFCONVARGUMENTMANAGER_H__

#include <map>
#include "mifconv.h"
#include "mifconv_sourcefile.h"
#include "mifconv_argument.h"


/**
 * This helper class is used to specify rules that will be used when
 * mifconv is searching source icons from the predefined folders.
 * Currently it supports rules that specify what kind of files can be
 * located in the folders.
 */
class MifConvSourceSearchRule
{
public:
    /**
     * Constructor
     * @param path Path to the folder, which the rule affects on.
     * @param types Allowed file types that can be located in the given 
     * <code>path</code>
     */
    MifConvSourceSearchRule(const MifConvString& path, const MifConvStringList& types);

    /**
     * Destructor
     */
    virtual ~MifConvSourceSearchRule();

    /**
     * Returns the path to the folder
     * @return Path to the folder from which the files are searched.
     */
    const MifConvString& SearchPath() const;
    MifConvString& SearchPath();

    /**
     * Returns a list of allowed file types (extensions). 
     * <code>MIFCONV_WILDCARD</code> can be used as a wildcard, meaning that
     * there are no restrictions for the filetype in the specific folder.
     * @return List of allowed file extensions.
     */
    const MifConvStringList& AllowedFileTypes() const;
private:
    MifConvString iSearchPath;
    MifConvStringList iAllowedFileTypes;
};

class MifConvArgumentManager
{
public:

    /**
     * Returns a pointer to the singleton instance.
     * @return Pointer to the singleton instance.
     */
    static MifConvArgumentManager* Instance();
        
    /** 
     * Destructor
     */
	virtual ~MifConvArgumentManager();

	/** 
	 * Initializes the argument list.
	 * @param argList List of strings from which the arguments are parsed.
	 */
	void Init( const MifConvStringList& argList );

    /**
     * Deallocates the allocated memory.
     */
    static void Reset();

	/**
	 * Set target filename (i.e. mif-filename)
	 * @param arg target filename
	 */
	void SetTargetFile( const MifConvString& arg );

	/**
	 * Returns target filename
	 * @return target filename
	 */
	const MifConvString& TargetFile() const;

	/**
	 * Returns argument value
	 * @param argName Argument name
	 * @return Argument value
	 */
	const MifConvString& StringValue(const MifConvString& argName) const;

    /**
     * Returns argument value list
     * @param argName Argument name
     * @return List of argument values
     */
    const MifConvStringList& StringListValue(const MifConvString& argName) const;

	/**
	 * Returns value of an boolean argument. Returns false, if argument is not 
	 * found from the boolean argument list.
	 * @param argName Argument name
	 * @return Argument value
	 */
	bool BooleanValue( const MifConvString& argName ) const;

	/**
	 * Returns source file list given in command line parameters
	 * @return List of source files
	 */
	const MifConvSourceFileList& SourceFiles() const;

	/**
	 * Adds argument value to the argument map
	 * @param argName Name of the argument
	 * @param argValue Value of the argument
	 */
	void AddArgumentValue( const MifConvString& argName, const MifConvString& argValue );

	/**
	 * Add source file to the source file list
	 * @param sourceFile Source file object to be added to the list
	 */
	void AddSourceFile( const MifConvSourceFile& sourceFile );
    
    /**
     * Returns value of EPOCROOT environment variable. If the EPOCROOT 
     * environment variable is defined (i.e. length > 0), directory separator
     * is added to returned string if it is missing from the EPOCROOT.
     * @return EPOCROOT environment variable value
     */
    const MifConvString& EpocRoot() const;

    /**
     * Converts depth value to display mode value
     * @return IconDisplayMode enumeration value 
     * @param depth IconDepth enumeration value
     */
    IconDisplayMode ConvertToDisplayMode(IconDepth depth) const;

    /**
     * Converts mask depth value to display mode value
     * @return IconDisplayMode enumeration value 
     * @param mask IconMaskDepth enumeration value
     */
    IconDisplayMode ConvertToMaskDisplayMode(IconMaskDepth mask) const;

    /**
     * Converts depth string value to depth enumeration value
     * @return IconDepth enumeration value
     * @param depthStr depth string      
     */
    IconDepth ConvertToDepth( const MifConvString& depthStr ) const;

    /**
     * Converts mask depth string value to mask depth enumeration value
     * @return IconMaskDepth enumeration value 
     * @param depthStr mask depth string
     */
    IconMaskDepth ConvertToMaskDepth( const MifConvString depthStr ) const;

protected:

    /**
     * Tells whether the given string is a valid argument name
     * @return True if the given string is valid argument
     * @param argName String representing the argument name
     */
	bool IsArgument( const MifConvString& argName ) const;

    /**
     * Tells whether the given argument is of string type and returns the 
     * length of the argument name. For example: If given string 
     * "/HMyHeader.mbg" this function returns 1, which is the length of the
     * argument name "H".
     * @return Length of the string if the given argument is string type, zero 
     * otherwise
     * @param argName Argument name
     */
	size_t IsStringArgument( const MifConvString& argName ) const;

    /**
     * Tells whether the given argument is of string list type and returns the 
     * length of the argument name. For example: If given string 
     * "/iInputDir;TempDir" this function returns 1, which is the length of the
     * argument name "i".
     * @return Length of the string if the given argument is string list type, zero 
     * otherwise
     * @param argName Argument name
     */
	size_t IsStringListArgument( const MifConvString& argName ) const;

    /**
     * Tells whether the given argument is of boolean type and returns the 
     * length of the argument name. For example: If given string "/E" this 
     * function returns 1, which is the length of the argument name "E".
     * @return Length of the argument name if the given argument is boolean 
     * type, zero otherwise
     * @param argName Argument name
     */
	size_t IsBooleanArgument( const MifConvString& argName ) const;

    /**
     * Tells whether the given argument is a help argument and returns the 
     * length of the argument name. For example: If given string "--help" this
     * function returns 5, which is the length of the argument name "-help".
     * @return Length of the argument name if the given argument is a help 
     * argument, zero otherwise
     * @param argName Argument name
     */
    size_t IsHelpArgument( const MifConvString& argName ) const;

    /**
     * Tells whether the given argument is a valid depth value
     * @return True if the given argument is a valid depth value, false 
     * otherwise
     * @param argName Argument string
     */
	bool IsDepthArgument( const MifConvString& argName ) const;

    /**
     * Tells whether the given argument is an "animated" -flag
     * @return True if the given argument is an "animated" -flag, false 
     * otherwise
     * @param argName Argument string
     */
	bool IsAnimatedFlag( const MifConvString& argName ) const;

    /**
     * Finalizes the command line arguments for internal use.
     */
	void FinalizeArguments();

    /**
     * Processes and fix the directory paths given in command line arguments.
     */
	void ProcessArgumentPaths();

    /**
     * Resolves the actual source file types by searching them in user defined
     * or pre-defined locations.
     */
	void ResolveSourceFileTypes();

    /**
     * Sets usage instructions in the given string
     * @param usageStr Usage instuctions are copied in this string object.
     */
	void SetUsageString( MifConvString& usageStr ) const;

    /**
     * Gets needed environment variables.
     */
    void GetMifEnv();

    /**
     * Completes the given path to full directory path without the drive letter.
     * @param sourcePath Directory path to be completed to full path.
     * @param targetPath Full directory path is copied here.
     */
    void MifFullPath( const MifConvString& sourcePath, MifConvString& targetPath);

    /**
     * Completes the given drive and path to full absolute directory path.
     * @param sourceDrive Drive of the absolute path. If empty, epocroot 
     * environment variable is used.
     * @param sourcePath Directory path to be completed to absolute path.
     * @param targetPath Absolute directory path is copied here.
     */     
    void MifAbsolutePath( const MifConvString& sourceDrive, const MifConvString& sourcePath, MifConvString& targetPath);

    /**
     * Checks wheter the given file-extension is the correct one for the source 
     * file. Sets also the source filename to complete one.
     * @return true if the given source file with given extension is found and 
     * to be used.
     * @param srcFile Source file object
     * @param extension Extension of the source file
     */
    bool FindAndSetPathAndType( MifConvSourceFile& srcFile, const MifConvString& extension );

    /**
     * Fills up the depth and mask depth maps used to map different enumeration 
     * values internally.
     */
    void PopulateDepthAndMaskMaps();

    /**
     * Reads the parameter file containing commandline parameters and source 
     * icon files
     * @param paramFilename Filename for the parameter file.
     * @param paramList Parameters are inserted to this list.
     */
    void ReadParameterFile(const MifConvString& paramFilename, MifConvStringList& paramList);

    /**
     * Adds arguments to internal data structures
     * @param argList List containing arguments
     * @param paramsFromFile Tells if the parameters are read from the 
     * parameter file or not.
     * Applied usage rules depend sligthly on this.
     */
    void AddArguments( const MifConvStringList& argList, bool paramsFromFile = false );

    /**
     * Reads string argument from the list to internal data structure
     */
    MifConvString ReadStringArgument(const MifConvStringList& argList, MifConvStringList::const_iterator& i, unsigned int argNameLen);

    /**
     * Reads string list argument from the list to internal data structure
     */
    void ReadStringListArgument(MifConvStringList::const_iterator& i, unsigned int argNameLen, MifConvStringList& StringValueList);

    // Internal data structure for boolean type arguments
	typedef std::map<MifConvString, MifConvBooleanArgument> BooleanArgMap;
    BooleanArgMap iBooleanArguments;

    // Internal data structure for string type arguments
	typedef std::map<MifConvString, MifConvStringArgument> StringArgMap;
    StringArgMap iStringArguments;

    // Internal data structure for string list type arguments
    typedef std::map<MifConvString, MifConvStringListArgument> StringListArgMap;
    StringListArgMap iStringListArguments;

	inline void THROW_USAGE_EXCEPTION() const;
    inline void THROW_ERROR( const MifConvString& errorMsg, const MifConvString& file = MifConvString(), int line = 0) const;
	
	MifConvSourceFileList iSourceFiles;
	MifConvString iTargetFile;
	MifConvString iDummyString;
    MifConvStringList iDummyStringList;
    //MifConvStringList iSearchPaths;    
    MifConvString iEpocRoot;

    // Internal data structure storing the rules used when finding source icons 
    // from the folders
    typedef vector<MifConvSourceSearchRule> SearchRules;
    SearchRules iSearchRules;

    private:

    /**
     * Default constructor
     */
	MifConvArgumentManager();
    static MifConvArgumentManager* iInstance;

    MifConvIconDepthMap iDepthMap;
    MifConvIconMaskDepthMap iMaskDepthMap;
    MifConvIconDisplayModeMap iDisplayModeMap;
    MifConvMaskIconDisplayModeMap iMaskDisplayModeMap;
};


#endif
