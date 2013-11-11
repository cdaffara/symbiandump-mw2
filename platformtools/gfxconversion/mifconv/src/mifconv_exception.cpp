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
* Description:  Mifconv exception class.
*
*/


#include "mifconv.h"
#include "mifconv_exception.h"

MifConvException::MifConvException()
:
iLine(0)
{}

MifConvException::MifConvException(const MifConvString& str, const MifConvString& file, int line)
:
iString(str),
iFile(file),
iLine(line)
{}

MifConvException::MifConvException(const MifConvException& rhs)
:
iString(rhs.iString),
iFile(rhs.iFile),
iLine(rhs.iLine)
{}

MifConvException::~MifConvException()
{}

void MifConvException::SetString(const MifConvString& str)
{
	iString = str;
}
const MifConvString& MifConvException::String() const
{
	return iString;
}

MifConvString& MifConvException::String()
{
	return iString;
}

void MifConvException::SetFile(const MifConvString& file)
{
	iFile = file;
}

const MifConvString& MifConvException::File() const
{
	return iFile;
}

void MifConvException::SetLine(int line)
{
	iLine = line;
}

int MifConvException::Line() const
{
	return iLine;
}
