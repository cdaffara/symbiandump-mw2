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
* Description:  Mifconv type definitions.
*
*/


#ifndef __MIFCONVTYPES_H__
#define __MIFCONVTYPES_H__

#include <vector>
#include <string>
#include <map>

namespace MifConvDefs
{
	typedef std::string MifConvString;
	typedef std::vector<MifConvString> MifConvStringList;
    
	enum IconDepth
	{
		IconDepth_Undefined = 0,
		IconDepth_1,
		IconDepth_2,
		IconDepth_4,
		IconDepth_8,
		IconDepth_c4,
		IconDepth_c8,
		IconDepth_c12,
		IconDepth_c16,
		IconDepth_c24,
		IconDepth_c32
	};

	enum IconMaskDepth
	{
		IconMaskDepth_Undefined = 0,
		IconMaskDepth_1,
		IconMaskDepth_8
	};
    
    enum IconDisplayMode
	{
	/** No display mode */
	DisplayMode_None,
	/** Monochrome display mode (1 bpp) */
	DisplayMode_Gray2,
	/** Four grayscales display mode (2 bpp) */
	DisplayMode_Gray4,
	/** 16 grayscales display mode (4 bpp) */
	DisplayMode_Gray16,
	/** 256 grayscales display mode (8 bpp) */
	DisplayMode_Gray256,
	/** Low colour EGA 16 colour display mode (4 bpp) */
	DisplayMode_Color16,
	/** 256 colour display mode (8 bpp) */
	DisplayMode_Color256,
	/** 64,000 colour display mode (16 bpp) */
	DisplayMode_Color64K,
	/** True colour display mode (24 bpp) */
	DisplayMode_Color16M,
	/** (Not an actual display mode used for moving buffers containing bitmaps) */
	DisplayMode_Rgb,
	/** 4096 colour display (12 bpp). */
	DisplayMode_Color4K,
	DisplayMode_Color16MU,
	//Any new display mode should be insterted here!
	//There might be up to 255 display modes, so value of the last
	//inserted EColorXXX enum item should be less than 256 -
	//BC reasons!
	DisplayMode_ColorLast
	};
    
    enum IconFormatType
    {
        IconFormatType_BMP,
        IconFormatType_SVG,
        IconFormatType_NVG
    };

    typedef std::map<MifConvString, IconDepth> MifConvIconDepthMap;
    typedef std::map<MifConvString, IconMaskDepth> MifConvIconMaskDepthMap;
    typedef std::map<IconDepth, IconDisplayMode> MifConvIconDisplayModeMap;
    typedef std::map<IconMaskDepth, IconDisplayMode> MifConvMaskIconDisplayModeMap;

    typedef std::pair<char*, unsigned int> MifConvFileData;
}

#endif
