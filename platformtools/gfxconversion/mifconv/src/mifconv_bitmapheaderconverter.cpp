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
* Description:  Mifconv bitmap header converter class.
*
*/


#include "mifconv.h"
#include "mifconv_bitmapheaderconverter.h"
#include "mifconv_argumentmanager.h"
#include "mifconv_util.h"
#include "mifconv_exception.h"

const int MIF_ID_FIRST = 16384;

/**
 *
 */
MifConvBitmapHeaderConverter::MifConvBitmapHeaderConverter()
:
iCurrentSourceId(MIF_ID_FIRST)
{
	MifConvArgumentManager* argMgr = MifConvArgumentManager::Instance();
    iHeaderFilename = argMgr->StringValue(MifConvHeaderArg);
}

/**
 *
 */
MifConvBitmapHeaderConverter::~MifConvBitmapHeaderConverter()
{
}

/**
 *
 */
void MifConvBitmapHeaderConverter::Init()
{
    CleanupTargetFiles();
}

/**
 *
 */
void MifConvBitmapHeaderConverter::CleanupTargetFiles()
{
	if( MifConvUtil::FileExists(iHeaderFilename) )
	{
        // Try to remove file MIFCONV_MAX_REMOVE_TRIES times, no exception in case of failure:
        MifConvUtil::RemoveFile(iHeaderFilename, MIFCONV_MAX_REMOVE_TRIES, true);
	}
}

/**
 *
 */
void MifConvBitmapHeaderConverter::AppendFile( const MifConvSourceFile& sourcefile )
{    
	iSourceFiles.push_back( sourcefile );	
}

/**
 *
 */
void MifConvBitmapHeaderConverter::Convert()
{
    // Check if the headerfile name is given. Return if not:    
    if( iHeaderFilename.length() > 0 )
    {
        cout << "Writing mbg: " << iHeaderFilename << endl;
        MifConvUtil::EnsurePathExists(iHeaderFilename, true);
    }
    else
    {
        return;
    }

    // Create / open the header file:
    fstream headerFile(iHeaderFilename.c_str(), ios::out|ios::trunc);
    
    // If file creation/opening was not successful, give warning and return:
    if( !headerFile.is_open() )
    {
        MifConvString debugStr("WARNING: Headerfile " + iHeaderFilename + " cannot be opened for writing.");
        cout <<  debugStr  << endl;
        MifConvUtil::DebugLog(debugStr);
        return;
    }
    iDestFile = &headerFile; // Save the pointer to the file stream    
    (*iDestFile) << endl;
    // Write the beginning of the header file:
    WriteStart();

    // Add source file items to enumerator:
    for( MifConvSourceFileList::iterator i = iSourceFiles.begin(); i != iSourceFiles.end(); ++i )
    {
        WriteItemToHeader(*i);
    }

    // Write the end of the file...
    WriteEnd();

    // ...and close the file:
    iDestFile->close();
}

/**
 *
 */
void MifConvBitmapHeaderConverter::WriteStart()
{
    (*iDestFile) << "/* This file has been generated, DO NOT MODIFY. */" << endl;
    (*iDestFile) << "enum TMif" << MifConvUtil::UnadornedFilename(iHeaderFilename) << "\n\t{" << endl;
}

/**
 *
 */
void MifConvBitmapHeaderConverter::WriteItemToHeader( const MifConvSourceFile& source )
{
    MifConvString unadornedHeaderFile(MifConvUtil::UnadornedFilename(iHeaderFilename));
    
    (*iDestFile) << "\tEMbm" << unadornedHeaderFile << MifConvUtil::UnadornedFilename(source.Filename()) <<  " = " << iCurrentSourceId++ << "," << endl;

    // Write mask also, if any:
    if( source.MaskDepth() != IconMaskDepth_Undefined )
    {
        (*iDestFile) << "\tEMbm" << unadornedHeaderFile << MifConvUtil::UnadornedFilename(source.Filename()) <<  "_mask = " << iCurrentSourceId++ << "," << endl;
    }
    else
    {
        // Skip mask ID if mask not present so that adding a mask later does not change IDs.
        iCurrentSourceId++;
    }
}

/**
 *
 */
void MifConvBitmapHeaderConverter::WriteEnd()
{
    (*iDestFile) << "\tEMbm" << MifConvUtil::UnadornedFilename(iHeaderFilename) << "LastElement" << endl;
    (*iDestFile) << "\t};" << endl;
}

/**
 *
 */
void MifConvBitmapHeaderConverter::Cleanup(bool err)
{
    if( err )
	{
	    CleanupTargetFiles();
	}
}
