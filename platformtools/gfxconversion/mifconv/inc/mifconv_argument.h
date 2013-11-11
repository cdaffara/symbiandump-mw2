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
* Description:  Mifconv tool argument definitions.
*
*/


#ifndef __MIFCONVARGUMENT_H__
#define __MIFCONVARGUMENT_H__

#include "mifconv.h"
#include "mifconv_sourcefile.h"
#include <vector>

// Argument constants
static const MifConvString MifConvUseExtensionArg("e");
static const MifConvString MifConvHeaderArg("h");
static const MifConvString MifConvHelpHArg("h"); // Duplicate name
static const MifConvString MifConvHelpQArg("?");
static const MifConvString MifConvHelpHelpArg("-help");
static const MifConvString MifConvPaletteFileArg("p");
static const MifConvString MifConvTempPathArg("t");
static const MifConvString MifConvBmconvPathArg("b");
static const MifConvString MifConvSvgencodePathArg("s");
static const MifConvString MifConvSvgtVersionArg("v");
static const MifConvString MifConvParameterFileArg("f");
static const MifConvString MifConvAnimatedIconArg("a"); // This is part of source file argument
static const MifConvString MifConvDisableSvgCompression("x");
static const MifConvString MifConvIconSourceDirectory("i");
static const MifConvString MifConvDepth_1("1");
static const MifConvString MifConvDepth_2("2");
static const MifConvString MifConvDepth_4("4");
static const MifConvString MifConvDepth_8("8");
static const MifConvString MifConvDepth_c4("c4");
static const MifConvString MifConvDepth_c8("c8");
static const MifConvString MifConvDepth_c12("c12");
static const MifConvString MifConvDepth_c16("c16");
static const MifConvString MifConvDepth_c24("c24");
static const MifConvString MifConvDepth_c32("c32");
static const MifConvString MifConvMaskDepth_1("1");
static const MifConvString MifConvMaskDepth_8("8");

// Array of help arguments:
const MifConvString MifConvHelpArguments[] = {
	MifConvHelpHArg,
    MifConvHelpQArg,
    MifConvHelpHelpArg
	};

// Array of boolean arguments:
const MifConvString MifConvBooleanArguments[] = {
	MifConvUseExtensionArg,
    MifConvDisableSvgCompression
	};

// Array of string arguments:
const MifConvString MifConvStringArguments[] = {	
	MifConvHeaderArg,
	MifConvPaletteFileArg,
	MifConvTempPathArg,
	MifConvBmconvPathArg,
	MifConvSvgencodePathArg,
	MifConvSvgtVersionArg,
	MifConvParameterFileArg    
};

// Array of string list arguments:
const MifConvString MifConvStringListArguments[] = {
    MifConvIconSourceDirectory
};

// Array of depth values:
const MifConvString MifConvDepthArguments[] = {
	MifConvDepth_1,
	MifConvDepth_2,
	MifConvDepth_4,
	MifConvDepth_8,
	MifConvDepth_c4,
	MifConvDepth_c8,
	MifConvDepth_c12,
	MifConvDepth_c16,
	MifConvDepth_c24,
	MifConvDepth_c32
};

// Argument class template
template <typename T>
class MifConvArgument
{
public:
    /**
     * Constructor
     * @param argName Argument name
     * @param argVal Argument value
     */
	MifConvArgument( const MifConvString& argName, const T& argVal );

    /**
     * Copy constructor
     * @param arg Argument object to be copied
     */
	MifConvArgument( const MifConvArgument& arg );

    /**
     * Destructor
     */
	virtual ~MifConvArgument();

    /**
     * Returns the argument name
     * @return Argument name
     */
	const MifConvString& Name() const;

    /** 
     * Returns the argument value
     * @return Argument value
     */
	const T& Value() const;

    /**
     * Sets the argument name
     * @param argName Argument name
     */
	void SetName( const MifConvString& argName );

    /**
     * Sets the argument value
     * @param argVal Argument value
     */
	void SetValue( const T& argVal );

    /**
     * Comparison argument for sorting
     */
	bool operator< (const MifConvArgument& arg );

protected:
	MifConvString iName;
	T iValue;
};

/**
 *
 */
template <typename T>
MifConvArgument<T>::MifConvArgument(const MifConvString& argName, const T& argVal )
: 
iName( argName ),
iValue( argVal )  
{
}

/**
 *
 */
template <typename T>
MifConvArgument<T>::MifConvArgument(const MifConvArgument& arg )
:
iName(arg.iName),
iValue(arg.iValue)
{}

/**
 *
 */
template <typename T>
MifConvArgument<T>::~MifConvArgument()
{}

/**
 *
 */
template <typename T>
const MifConvString& MifConvArgument<T>::Name() const
{
	return iName;
}

/**
 *
 */
template <typename T>
const T& MifConvArgument<T>::Value() const
{
	return iValue;
}

/**
 *
 */
template <typename T>
void MifConvArgument<T>::SetName( const MifConvString& argName )
{
	iName = argName;
}

/**
 *
 */
template <typename T>
void MifConvArgument<T>::SetValue( const T& argVal )
{
	iValue = argVal;
}

/**
 *
 */
template <typename T>
bool MifConvArgument<T>::operator< ( const MifConvArgument& arg )
{
	return iName < arg.iName;
}

/**
 * Type definitions
 */
typedef MifConvArgument<bool> MifConvBooleanArgument;
typedef MifConvArgument<MifConvString> MifConvStringArgument;
typedef MifConvArgument<MifConvStringList> MifConvStringListArgument;
typedef MifConvArgument<MifConvSourceFile> MifConvSourceFileArgument;

typedef std::vector<MifConvBooleanArgument> MifConvBooleanArgumentList;
typedef std::vector<MifConvStringArgument> MifConvStringArgumentList;
typedef std::vector<MifConvSourceFileArgument> MifConvSourceFileArgumentList;

#endif
