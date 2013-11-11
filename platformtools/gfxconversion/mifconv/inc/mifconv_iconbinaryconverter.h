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
* Description:  Mifconv icon binary converters class.
*
*/


#ifndef __MIFCONVICONBINARYCONVERTER_H__
#define __MIFCONVICONBINARYCONVERTER_H__

#include "mifconv_fileconverter.h"
#include <fstream>

/**
 * Converter class for .svg to .svgb conversion:
 */

class MifConvIconBinaryConverter : public MifConvFileConverter
{
public:

    /**
     * Default constructor
     */
    MifConvIconBinaryConverter();

    /**
     * Destructor
     */
    virtual ~MifConvIconBinaryConverter();

    // From MifConvFileConverter:
	virtual void AppendFile( const MifConvSourceFile& sourcefile );
	virtual void Convert();
	virtual void Cleanup(bool err = false);
	virtual void Init();
	virtual void CleanupTargetFiles();

protected:

    /**
     * Converts files to svgb files.     
     */
	virtual void ConvertToSvgb();

    /**
     * Removes temporary files from the file system.
     */
	virtual void CleanupTempFiles();

    /**
     * Runs the external converter.     
     */
    void RunExtConverter();

    /**
     * Creates and initializes the temporary file     
     */
    void InitTempFile();
   
    /**
     * Returns default path to external converter.
     * @return Default path to external converter.
     */
    const MifConvString& GetDefaultExtConverterPath();

    void ConvertToBinaryFilename( MifConvString& input );
private:
    
    MifConvString iTempDir;
    MifConvStringList iTempFilenames;
    MifConvString iDefaultExtConverterPath;    
};


#endif
