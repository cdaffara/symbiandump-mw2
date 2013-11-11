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
* Description:  Mifconv source file class.
*
*/


#ifndef __MIFCONVSOURCEFILE_H__
#define __MIFCONVSOURCEFILE_H__

#include "mifconv.h"

class MifConvSourceFile
{
public:

    enum TCompileInfo
        {
        EFormatNotImportant,
        ENokiaBitmap,
        EThirdPartyBitmap
        };
    
	/**
	 * Default constructor
	 */
	MifConvSourceFile();

    /**
     * Copy constructor
     */
    MifConvSourceFile( const MifConvSourceFile& src );
	
	/**
	 * Destructor
	 */
	virtual ~MifConvSourceFile();

	/**
	 * Sets filename for the icon file
	 * @param filename Source icon filename 
	 */
	void SetFilename( const MifConvString& filename );
	
	/**
	 * Sets icon depth for the source file
	 * @param depth Icon depth string.
	 */
	void SetDepthString( const MifConvString& depth);

	/**
	 * Sets icon mask depth for the source file
	 * @param depth Icon mask depth string.
	 */
	void SetMaskDepthString( const MifConvString& depth);

	/**
	 * Specifies the animated flag for icon
	 * @param isAnimated, true if the icon is animated, false otherwise
	 */
	void SetAnimated( bool isAnimated );

	/**
	 * Sets icon depth and mask depth parameters for the source file. Sets both string and enumeration values of
     * depth and mask depth parameters.
	 * @param depthAndMask String containing the depth and mask as user has given them (e.g "/c8,8")
	 */
	void SetDepthAndMask( const MifConvString& depthAndMask );

	/**
	 * Returns source icon filename
	 * @return Filename of the icon file
	 */
	const MifConvString& Filename() const;

	/**
	 * Returns icon depth of the source file
	 * @return Icon depth enumeration value.
	 */
	IconDepth Depth() const;

    /**
	 * Sets icon depth of the source file
	 * @param depth Icon depth enumeration value.
	 */
	void SetDepth(IconDepth depth);

    /**
     * Returns icon depth of the source file
     * @return Icon depth string
     */
    const MifConvString& DepthString() const;

	/**
	 * Returns icon depth of the mask file
	 * @return Icon mask depth enumeration value.
	 */
	IconMaskDepth MaskDepth() const;

    /**
	 * Sets icon depth of the mask file
	 * @param depth Icon mask depth enumeration value.
	 */
	void SetMaskDepth(IconMaskDepth depth);

    /**
     * Returns icon depth of the mask file
     * @return Icon mask depth string
     */
    const MifConvString& MaskDepthString() const;

    /**
     * Returns true if the icon is animated, otherwise false
     * @return <code>boolean</code> value specifying if the icon is animated.
     */
    bool IsAnimated() const;

    /**
     * Returns the mask filename for the bmp file
     * @return Mask file name for the bitmap file.
     */
    const MifConvString& BmpMaskFilename() const;

    /**
     * Sets mask filename.
     * @param maskFilename Mask filename
     */
    void SetBmpMaskFilename( const MifConvString& maskFilename );

    /**
     * Sets file content.
     * @param content Pointer to the data content array.
     * @param len Data content lenght
     */
    void SetContent( char* content, unsigned int len );

    /**
     * Returns data content of the file
     * @return Pointer to the data content array. See also ContentLength() -method.
     */
    char* Content() const;

    /**
     * Returns data content length.
     * @return Length of the data content.
     */
    int ContentLength() const;

    /**
     * Sets display mode for the source icon.
     * @param mode Display mode enumeration value.
     */
    void SetDisplayMode( IconDisplayMode mode);

    /**
     * Returns display mode of the icon.
     * @return Display mode enumeration value.
     */
    IconDisplayMode DisplayMode() const;

    /**
     * Sets display mode for the mask file.
     * @param mode Display mode enumeration value for the mask file.
     */
    void SetMaskDisplayMode( IconDisplayMode mode);

    /**
     * Returns display mode of the mask file.
     * @return Display mode enumeration value of the mask file.
     */
    IconDisplayMode MaskDisplayMode() const;

    void SetCompileInfo(TCompileInfo aInfo);
    void WriteCompileInfo(ostream& aOut);
    
protected:
 
    /**
     * Parses icon depth parameter from the user given argument string. If, for example, user
     * has given a string "/c8,8" as a depth and mask for the file, this function returns <code>IconDepth_c8</code>
     * @param depthAndMask String containing the depth and mask parameters for the source file.
     * @return Icon depth enumeration value
     */
    IconDepth ParseIconDepth( const MifConvString& depthAndMask );

    /**
     * Parses icon mask parameter from the user given argument string. If, for example, user
     * has given a string "/c8,8" as a depth and mask for the file, this function returns <code>IconMask_8</code>
     * @param depthAndMask String containing the depth and mask parameters for the source file.
     * @return Icon mask enumeration value
     */
    IconMaskDepth ParseIconMaskDepth( const MifConvString& depthAndMask );

    MifConvString   iFilename;
    MifConvString   iDepthString;
    MifConvString   iMaskDepthString;
    IconDepth       iDepth;
    IconMaskDepth   iMaskDepth;
    IconDisplayMode iDisplayMode;
    IconDisplayMode iMaskDisplayMode;
    bool iAnimated;    
    MifConvString iMaskFilename;
    char* iContent;
    unsigned int iContentLen;
    TCompileInfo iInfo;
};

typedef std::vector<MifConvSourceFile> MifConvSourceFileList;

#endif
