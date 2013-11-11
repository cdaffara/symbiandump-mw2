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
* Description:  Mifconv converter base class.
*
*/


#ifndef __MIFCONVFILECONVERTER_H__
#define __MIFCONVFILECONVERTER_H__

#include <vector>
#include "mifconv.h"
#include "mifconv_sourcefile.h"

/**
 * Virtual base class for file converters
 */
class MifConvFileConverter
{
public:

    /**
     * Destructor
     */
    virtual ~MifConvFileConverter(){};

    /**
     * Appends source file to the internal file list.
     * @param Source file object
     */
	virtual void AppendFile( const MifConvSourceFile& sourcefile ) = 0;

    /**
     * Converts all the files in the internal file list.     
     */
	virtual void Convert() = 0;

    /**
     * Cleans up the temporary files etc.
     * @param error True in error situations
     */
	virtual void Cleanup(bool error = false) = 0;
	
	virtual void Init() = 0;
	
	virtual void CleanupTargetFiles() = 0;

protected:
    // Internal file list
	MifConvSourceFileList iSourceFiles;
};

typedef std::vector<MifConvFileConverter*> MifConvFileConverterList;

#endif
