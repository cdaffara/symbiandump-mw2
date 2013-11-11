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
* Description:  Mifconv bitmap converters class.
*
*/


#ifndef __MIFCONVBITMAPCONVERTER_H__
#define __MIFCONVBITMAPCONVERTER_H__

#include "mifconv_fileconverter.h"
#include <fstream>

/**
 * Converter class for converting .bmp files to .mbm file.
 */

class MifConvBitmapConverter : public MifConvFileConverter
{
public:

    /**
     * Default constructor
     */ 
    MifConvBitmapConverter();

    /**
     * Destructor
     */
    virtual ~MifConvBitmapConverter();

    // From MifConvFileConverter
	virtual void AppendFile( const MifConvSourceFile& sourcefile );
	virtual void Convert();
	virtual void Cleanup(bool err = false);
	virtual void Init();
	virtual void CleanupTargetFiles();

protected:

    /**
     * Converts files to .mbm file     
     */
	virtual void ConvertToMbm();

    /**
     * Removes temporary files from the file system.
     */
	virtual void CleanupTempFiles();

    /**
     * Runs external bmconv application, which builds the .mbm file.
     * @param bmconvPath location of bmconv
     */
    void RunExternalBmconv( const MifConvString& aBmconvPath );
       
    /**
     * Check first from argument manager if palette name has been given.
     * If not, use default palette file name.
     */
    const MifConvString GetPaletteFileName();
    
    /**
     * Returns defaulta palette file name.
     * @param aBuf used to return the reference constant.
     */
    const MifConvString& DefaultPaletteFileName(MifConvString& aBuf);
    
    /**
     * Creates and initializes the temporary file.     
     */
    void InitTempFile();

    /**
     * Add bitmap file to the temporary file
     * @param stream file stream for temporary file
     * @param bmpFile Source file object representing the bitmap file.
     */
    void AppendBmpToTempFile(fstream& aStream, const MifConvSourceFile& aBmpFile);

private:
        
    MifConvString iTempDir;
    MifConvString iTempFilename;
    MifConvString iTargetFilename;
};


#endif
