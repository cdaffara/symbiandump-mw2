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


#include "mifconv.h"
#include "mifconv_sourcefile.h"
#include "mifconv_util.h"
#include "mifconv_exception.h"
#include "mifconv_argument.h"
#include "mifconv_argumentmanager.h"

MifConvSourceFile::MifConvSourceFile()
:
iDepth(IconDepth_Undefined),
iMaskDepth(IconMaskDepth_Undefined),
iDisplayMode(DisplayMode_None),
iMaskDisplayMode(DisplayMode_None),
iAnimated(false),
iContent(0),
iContentLen(0)
{
}

MifConvSourceFile::MifConvSourceFile( const MifConvSourceFile& src )
:
iFilename( src.Filename() ),
iDepthString( src.DepthString() ),
iMaskDepthString( src.MaskDepthString() ),
iDepth(src.iDepth),
iMaskDepth(src.iMaskDepth),
iDisplayMode(src.iDisplayMode),
iMaskDisplayMode(src.iMaskDisplayMode),
iAnimated( src.IsAnimated() ),
iMaskFilename( src.iMaskFilename ),
iContent(src.iContent),
iContentLen(src.iContentLen),
iInfo(EFormatNotImportant)
{
}

MifConvSourceFile::~MifConvSourceFile()
{}

char* MifConvSourceFile::Content() const
{
    return iContent;
}

int MifConvSourceFile::ContentLength() const
{
    return iContentLen;
}

void MifConvSourceFile::SetContent(char* content, unsigned int len)
{
    iContent = content;
    iContentLen = len;
}

const MifConvString& MifConvSourceFile::Filename() const
{
	return iFilename;
}

void MifConvSourceFile::SetAnimated( bool isAnimated )
{
    iAnimated = isAnimated;
}

void MifConvSourceFile::SetFilename( const MifConvString& filename )
{
	iFilename = filename;
}

void MifConvSourceFile::SetDisplayMode( IconDisplayMode mode)
{
    iDisplayMode = mode;
}

IconDisplayMode MifConvSourceFile::DisplayMode() const
{
    return iDisplayMode;
}

void MifConvSourceFile::SetMaskDisplayMode( IconDisplayMode mode)
{
    iMaskDisplayMode = mode;
}

IconDisplayMode MifConvSourceFile::MaskDisplayMode() const
{
    return iMaskDisplayMode;
}

IconDepth MifConvSourceFile::Depth() const
{
	return iDepth;
}

void MifConvSourceFile::SetDepth(IconDepth depth)
{
	iDepth = depth;
}

const MifConvString& MifConvSourceFile::DepthString() const
{
    return iDepthString;
}

IconMaskDepth MifConvSourceFile::MaskDepth() const
{	
    return iMaskDepth;
}

void MifConvSourceFile::SetMaskDepth(IconMaskDepth depth)
{	
    iMaskDepth = depth;
}

const MifConvString& MifConvSourceFile::MaskDepthString() const
{
   return iMaskDepthString;
}

bool MifConvSourceFile::IsAnimated() const 
{
    return iAnimated;
}

const MifConvString& MifConvSourceFile::BmpMaskFilename() const
{
    return iMaskFilename;
}

void MifConvSourceFile::SetBmpMaskFilename( const MifConvString& maskFilename )
{
	iMaskFilename = maskFilename;
}

void MifConvSourceFile::SetDepthString( const MifConvString& depth )
{
	iDepthString = depth;
}

void MifConvSourceFile::SetMaskDepthString( const MifConvString& mask )
{
	iMaskDepthString = mask;
}

void MifConvSourceFile::SetDepthAndMask( const MifConvString& depthAndMask )
{   
    size_t separatorIndex = depthAndMask.find(",");
    
    if( separatorIndex != MifConvString::npos )
    {
        iDepthString = MifConvString(depthAndMask.begin()+1, depthAndMask.begin()+separatorIndex);
        iMaskDepthString = MifConvString(depthAndMask.begin()+separatorIndex+1, depthAndMask.end());        
        iMaskDepth = MifConvArgumentManager::Instance()->ConvertToMaskDepth(iMaskDepthString);
    }
    else
    {
        iDepthString = MifConvString(depthAndMask.begin()+1, depthAndMask.end());
    }
    iDepth = MifConvArgumentManager::Instance()->ConvertToDepth(iDepthString); 
}

void MifConvSourceFile::SetCompileInfo(TCompileInfo aInfo)
    {
    iInfo = aInfo;
    }

void MifConvSourceFile::WriteCompileInfo(ostream& aOut)
    {
    aOut << iFilename;
    if (iInfo == ENokiaBitmap)
        aOut << " (Nokia format)";
    else if (iInfo == EThirdPartyBitmap)
        aOut << " (Third party format)";
    }
