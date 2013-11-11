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
* Description:  Mifconv bitmap to header converter class.
*
*/


#ifndef __MIFCONVBITMAPHEADERCONVERTER_H__
#define __MIFCONVBITMAPHEADERCONVERTER_H__

#include "mifconv_fileconverter.h"
#include <fstream>

/**
 * Converter class for converting source files to .mbg header files
 */
class MifConvBitmapHeaderConverter : public MifConvFileConverter
{
public:
    /**
     * Default constructor
     */
    MifConvBitmapHeaderConverter();

    /**
     * Destructor
     */
    virtual ~MifConvBitmapHeaderConverter();

    // From MifConvFileConverter
    virtual void AppendFile( const MifConvSourceFile& sourcefile );
    virtual void Convert();
    virtual void Cleanup(bool err = false);
    virtual void Init();
    virtual void CleanupTargetFiles();

protected:

    /**
     * Writes the beginning of the header
     */ 
    void WriteStart();

    /**
     * Writes source file information to the header file
     * @param source Source file object representing the file to be 
     * added to the header.
     */
    void WriteItemToHeader( const MifConvSourceFile& source );

    /**
     * Writes the end of the header.
     */
    void WriteEnd();

    MifConvString iHeaderFilename;
    fstream* iDestFile;
    int iCurrentSourceId;
};

#endif
