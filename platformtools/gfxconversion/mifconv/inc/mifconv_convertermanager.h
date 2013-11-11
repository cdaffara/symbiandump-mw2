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
* Description:  Mifconv converter manager class.
*
*/


#ifndef __MIFCONVCONVERTERMANAGER_H__
#define __MIFCONVCONVERTERMANAGER_H__

#include "mifconv_fileconverter.h"

typedef std::map<MifConvString, MifConvFileConverterList> FileConverterMap;

class MifConvConverterManager
{
public:
	
    static MifConvConverterManager* Instance();
    static void Reset();
	~MifConvConverterManager();

    /**
     * Initializes the converter manager. Note! Argument manager must be initialized
     * before calling this.
     */
    void Init();

	/**
	 * This method initializes the file converter list and processes the source 
	 * files given in arguments. It finds the correct file converter for each 
	 * source file and lets it do the file conversion.	 
	 */
	void ProcessIcons();

	/**
	 * This method runs the cleanup method of each file converter in the file converter list.
	 * @param error True if error situation
	 */
	void Cleanup(bool error = false);

    /**
     * Returns all the converters for the given type of file.
     * @return List of the matching converters.
     * @param source Source file object
     */
    MifConvFileConverterList& GetConverters(const MifConvSourceFile& source);

    /**
     * Returns all the converters for the given type of file.
     * @return List of the matching converters.
     * @param filename Source file name
     */
    MifConvFileConverterList& GetConverters(const MifConvString& filename);

private:

    MifConvConverterManager();
	// File converter map containing all the converters needed to process the source icons.
    // Map is ordered by file-extensions:    
    FileConverterMap iFileConverters;
    MifConvFileConverterList iAllConverters;
    MifConvFileConverterList iDummyConverterList;

    static MifConvConverterManager* iInstance;
};


#endif
