/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Mifconv definitions.
*
*/


#ifndef __MIFCONV_H__
#define __MIFCONV_H__

#if (defined( _MSC_VER ) && _MSC_VER <= 1200 ) //1200 == VC++ 6.0    
        #pragma warning(disable: 4100)
        #pragma warning(disable: 4511)
        #pragma warning(disable: 4512)
        #pragma warning(disable: 4530)
        #pragma warning(disable: 4663)
        #pragma warning(disable: 4710)
        #pragma warning(disable: 4786)
        #pragma warning(disable: 4800)
#endif

#include <iostream>
#include <fstream>
using namespace std;

#include "mifconv_types.h"
using namespace MifConvDefs;

// Mifconv version
static const MifConvString MifConvVersion("3.3.0");
static const MifConvString MifConvDate("9th August 2010");
static const MifConvString MifConvYears("2010");

// Mifconv return values:
#define MIFCONV_ERROR       1
#define MIFCONV_NO_ERROR    0

#define MIFCONV_WILDCARD "*"
#define BMP_FILE_EXTENSION "bmp"
#define SVG_FILE_EXTENSION "svg"
#define SVGB_BINARY_FILE_EXTENSION "svgb"
#define HEADER_FILE_EXTENSION "mbg"
#define MBM_FILE_EXTENSION "mbm"
#define MIF_FILE_EXTENSION "mif"

#define FILE_EXTENSION_SEPARATOR "."
#define STRING_LIST_ARGUMENT_SEPARATOR ";"

#ifdef MSVC7
    #define FILE_IN_BINARY_NOCREATE_FLAGS (ios::in | ios::binary | ios::nocreate)
#else
    #define FILE_IN_BINARY_NOCREATE_FLAGS (ios::in | ios::binary) 
#endif

#ifdef WIN32
    #define DIR_SEPARATOR "\\"
    #define DIR_SEPARATOR2 '\\'
    #define INCORRECT_DIR_SEPARATOR "/"
    #define INCORRECT_DIR_SEPARATOR2 '/'
    #define OPTION_PREFIX1_STR "/"
    #define OPTION_PREFIX1_CHAR '/'
    #define OPTION_PREFIX2_STR "-"
    #define OPTION_PREFIX2_CHAR '-'    
#else
    #define DIR_SEPARATOR "/"
    #define DIR_SEPARATOR2 '/'    
    #define INCORRECT_DIR_SEPARATOR "\\"
    #define INCORRECT_DIR_SEPARATOR2 '\\'
    #define OPTION_PREFIX1_STR "-"
    #define OPTION_PREFIX1_CHAR '-'
    #define OPTION_PREFIX2_STR "-"
    #define OPTION_PREFIX2_CHAR '-' 
#endif

// BMCONV defines
#define BMCONV_EXECUTABLE_NAME "bmconv"
#define BMCONV_TEMP_FILE_POSTFIX "_###_bmconv_tmp_cmd_file"

#if defined (__SERIES60_30__) || defined (__SERIES60_31__) || defined (__S60_32__)
#define BMCONV_OPTION_PREFIX "/"
#else
#define BMCONV_OPTION_PREFIX "-"
#endif

#define BMCONV_QUIET_PARAMETER "q"
#define BMCONV_PALETTE_PARAMETER "p"

// SVGTBINENCODE defines
#define SVGTBINENCODE_EXECUTABLE_NAME "svgtbinencode"
#define SVGTBINENCODE_OPTION_PREFIX "-"
#define SVGTBINENCODE_VERSION_PARAMETER "v"

// Bitmap searching directories
static const MifConvString S60_PATH = "epoc32" + MifConvString(DIR_SEPARATOR) + "s60";
static const MifConvString S60_ICONS_PATH   = S60_PATH + MifConvString(DIR_SEPARATOR) + "icons";
static const MifConvString S60_BITMAPS_PATH = S60_PATH + MifConvString(DIR_SEPARATOR) + "bitmaps";

// Environment variable names
static const MifConvString EPOCROOT_ENV = MifConvString("EPOCROOT");
static const MifConvString SBS_BUILD_DIR_ENV = MifConvString("SBS_BUILD_DIR");

// EPOC paths
static const MifConvString EPOC32_PATH = "epoc32";
static const MifConvString DEFAULT_EPOCROOT = MifConvString("");
static const MifConvString EPOC_TOOLS_PATH = "epoc32" + MifConvString(DIR_SEPARATOR) + "tools";

static const MifConvString EPOC_BUILD_PATH = "epoc32" + MifConvString(DIR_SEPARATOR) + "build";
static const MifConvString EPOC_ICON_BUILD_PATH_POSTFIX = "s60" + MifConvString(DIR_SEPARATOR) + "icons";


// Debug logging file environment variable
static const MifConvString MIFCONV_DEBUG_FILE_ENV("MIFCONV_DEBUG_FILE");

// Maximum amount of target file removal tries
#define MIFCONV_MAX_REMOVE_TRIES    10

#endif
