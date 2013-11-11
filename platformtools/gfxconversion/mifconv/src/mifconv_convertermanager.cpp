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


#include "mifconv.h"
#include "mifconv_convertermanager.h"
#include "mifconv_argumentmanager.h"
#include "mifconv_util.h"
#include "mifconv_bitmapconverter.h"
#include "mifconv_bitmapheaderconverter.h"
#include "mifconv_iconbinaryconverter.h"
#include "mifconv_mifconverter.h"

// Static singleton initialization
MifConvConverterManager* MifConvConverterManager::iInstance = 0;

/**
 * Returns pointer to the singleton object
 */
MifConvConverterManager* MifConvConverterManager::Instance()
{
    if( iInstance == 0 )
    {
        iInstance = new MifConvConverterManager();
    }
    return iInstance;
}

/**
 * Free the allocated memory
 */
void MifConvConverterManager::Reset()
{
    delete iInstance;
    iInstance = 0;
}

/**
 * Constructor
 */
MifConvConverterManager::MifConvConverterManager()
{
    
}

/**
 *
 */
void MifConvConverterManager::Init()
{
    // Add bmp --> mbm converter:
    MifConvBitmapConverter* bitmapConverter = new MifConvBitmapConverter();
    iAllConverters.push_back(bitmapConverter);

    // Add headerfile (mbg) converter:
    MifConvBitmapHeaderConverter* headerConverter = new MifConvBitmapHeaderConverter();    
    iAllConverters.push_back(headerConverter);

    // Add svg --> svgb converter:
    MifConvIconBinaryConverter* svgConverter = new MifConvIconBinaryConverter();
    iAllConverters.push_back(svgConverter);

    // Add mif converter:
    MifConvMifConverter* mifConverter = new MifConvMifConverter();
    iAllConverters.push_back(mifConverter);

    // Add file converters for .BMP files:
    MifConvFileConverterList bmpConverters;
    bmpConverters.push_back(bitmapConverter);
    bmpConverters.push_back(headerConverter);
    bmpConverters.push_back(mifConverter);
    iFileConverters.insert(std::make_pair(MifConvString(BMP_FILE_EXTENSION), bmpConverters));

    // Add file converters for .SVG files:
    MifConvFileConverterList svgConverters;
    svgConverters.push_back(headerConverter);
    svgConverters.push_back(svgConverter);
    if( MifConvArgumentManager::Instance()->BooleanValue(MifConvDisableSvgCompression) )
    {        
        // SVG files will be added to mif-file without compression (i.e. don't use svgtbinencode for them).
        svgConverters.push_back(mifConverter); 
    }
    iFileConverters.insert(std::make_pair(MifConvString(SVG_FILE_EXTENSION), svgConverters));

    // Add file converters for .SVGB files:
    MifConvFileConverterList svgbConverters;  
    svgbConverters.push_back(headerConverter);
    if( !MifConvArgumentManager::Instance()->BooleanValue(MifConvDisableSvgCompression) )
    {     
        // SVG files will be compressed first.
        svgbConverters.push_back(mifConverter);
    }
    iFileConverters.insert(std::make_pair(MifConvString(SVGB_BINARY_FILE_EXTENSION), svgbConverters));
    
    // Initialize converters
    for( MifConvFileConverterList::iterator i = iAllConverters.begin(); i != iAllConverters.end(); ++i )
    {
    	(*i)->Init();
    }
}

/**
 * Destructor
 */
MifConvConverterManager::~MifConvConverterManager()
{
    for( MifConvFileConverterList::iterator conv = iAllConverters.begin(); conv != iAllConverters.end(); ++conv )
    {
        delete *conv;    
    }
}

/**
 * Adds source files to converters and converts them
 */
void MifConvConverterManager::ProcessIcons()
{
    MifConvArgumentManager* argMgr = MifConvArgumentManager::Instance();
    // Loop all the source files and append them to correct converters:
    const MifConvSourceFileList& sourcefiles = argMgr->SourceFiles();
    for( MifConvSourceFileList::const_iterator i = sourcefiles.begin(); i != sourcefiles.end(); ++i )
	{        
        MifConvFileConverterList& converters = GetConverters(*i);
        for( MifConvFileConverterList::iterator converter = converters.begin(); converter != converters.end(); ++converter )
        {            
            (*converter)->AppendFile(*i);            
        }
    }

    // Run all the converters:
    for( MifConvFileConverterList::iterator conv = iAllConverters.begin(); conv != iAllConverters.end(); ++conv )
    {
        (*conv)->Convert();
    }
}

/**
 *
 */
void MifConvConverterManager::Cleanup(bool error)
{
    for( MifConvFileConverterList::iterator conv = iAllConverters.begin(); conv != iAllConverters.end(); ++conv )
    {
        try {
            (*conv)->Cleanup(error);
        }
        catch(...) {
        // Exception should not be thrown if cleaning after error situation
            if( !error )
                throw;
        }
    }           
}

/**
 *
 */
MifConvFileConverterList& MifConvConverterManager::GetConverters(const MifConvSourceFile& source)
{    
    return GetConverters(source.Filename());
}

/**
 *
 */
MifConvFileConverterList& MifConvConverterManager::GetConverters(const MifConvString& filename)
{
    MifConvString extension(MifConvUtil::FileExtension(filename));
    FileConverterMap::iterator i = iFileConverters.find(extension);
    if( i != iFileConverters.end() )
    {
        return i->second;
    }
    
    return iDummyConverterList;
}
