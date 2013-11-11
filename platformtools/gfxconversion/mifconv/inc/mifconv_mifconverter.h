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
* Description:  Mifconv MIF converters class.
*
*/


#ifndef __MIFCONVMIFCONVERTER_H__
#define __MIFCONVMIFCONVERTER_H__

#include "mifconv_fileconverter.h"
#include <fstream>
#include <vector>

/**
 * Converter class for converting source files to .mif file.
 */

class MifConvMifConverter : public MifConvFileConverter
{
public:

    /**
     * Default constructor
     */ 
    MifConvMifConverter();

    /**
     * Destructor
     */
    virtual ~MifConvMifConverter();

    // From MifConvFileConverter
	virtual void AppendFile( const MifConvSourceFile& sourcefile );
	virtual void Convert();
	virtual void Cleanup(bool err = false);
	virtual void Init();
	virtual void CleanupTargetFiles();

protected:

    /**
     * Converts files to .mif file
     * @param argMgr Reference to argument manager object. 
     */
	virtual void ConvertToMif();

    /**
     * Removes temporary files from the file system.
     */
	virtual void CleanupTempFiles();
    
    /**
     * Creates and initializes the temporary file.
     * @param argMgr Reference to argument manager object.
     */
    void InitTempFile();

    /**
     * Add source file to the temporary file     
     * @param srcFile Source file object
     */
    void AppendSourceToTempFile(const MifConvSourceFile& srcFile);

    void ReadFileContents();
    void OpenTargetFile();
    void WriteTargetHeader();
    void WriteIconArray();
    void WriteIcons();
    void WriteIconHeader(const MifConvSourceFile& src);
    void WriteIconData(const MifConvSourceFile& src);
    MifConvDefs::IconFormatType ReadIconBinaryType(const MifConvSourceFile& src);

    
private:
    
    MifConvString iTargetFilename;
    fstream iTargetFile;
    typedef std::vector<char*> StringPtrVector;
    StringPtrVector iContentPointers;
};

#endif
