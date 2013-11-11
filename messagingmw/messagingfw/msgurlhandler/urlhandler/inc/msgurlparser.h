// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
// This file contains the API definition for the classes 
// TDelimitedAddressParser, TDelimitedBodyParser, TMailtoUrlParser,
// TDelimitedSmsParser, TParseUtil,
// These classes are used by the scheme handler classes to parse the 
// messaging urls.
// 
//

/**
 @file
*/

#ifndef __MSGURLPARSER_H__
#define __MSGURLPARSER_H__


// System Includes
//
#include <e32base.h>
#include <delimitedparser16.h>
#include <delimitedquery16.h>


/**
	@class
	Comments :	For parsing address component of mailto urls delimited by a ','
				as defined in RFC2368 and below.
				This class is used by CMailtoSchemeHandler class.

	Define format of mailto url:
			mailto:<address segment>?<body segment>

	@since 7.0
*/
class TDelimitedAddressParser : public TDelimitedParserBase16
/**
@internalComponent
@released
*/
	{
public:

/**
	@fn				TDelimitedAddressParser()
	Intended Usage	:	Constructor.
	@since			7.0
	@post			Delimeter is set
 */
	TDelimitedAddressParser();

/**
	@fn				Parse(const TDesC& aAddress)
	Intended Usage	:	This parses the descriptor into address segments from
						left to right.
	@since			7.0
	@param			aAddress	A descriptor containing the address segment.
	@pre 			The delimiter must have been set.
	@post			The current segment is the leftmost segment and the direction
					of parsing is set from left to right.
 */
	void Parse(const TDesC& aAddress);
	};


/**
	@typedef TDelimitedBodyParser
	Comments :	For parsing body segment of mailto urls delimited by a '&'
				as defined in RFC2368 and below.
				This class is used by CMailtoSchemeHandler class.

	Define format of mailto url:
			mailto:<address segment>?<body segment>

	@since 7.0

@internalComponent
@released
*/
typedef TDelimitedQueryParser16 TDelimitedBodyParser; 


/**
	@class
	Comments :	Parses the address segment and body segment as defined 
				in RFC2368 and below.
				This class is used by CMailtoSchemeHandler class.

	Define format of mailto url:
			mailto:<address segment>?<body segment>

	@since 7.0
*/
class TMailtoUrlParser
/**
@internalComponent
@released
*/
	{
public:

/**
	@fn				TMailtoUrlParser()
	Intended Usage	:	Constructor.
	@since			7.0
 */
	TMailtoUrlParser();

/**
	@fn				Parse(const TDesC& aData)
	Intended Usage	:	Parses the address segment and body segment of mailto url.
	@since			7.0
	@param			aData		A descriptor containing the data to be parsed. 
	@pre 			None
	@post			The address segment and body segment is parsed.
					The current segment is the leftmost segment and the direction
					of parsing is set from left to right.
 */
	void Parse(const TDesC& aData);

/**
	@fn				ExtractAddresses()
	Intended Usage	:	Returns TDelimitedAddressParser
	@since			7.0
	@return			TDelimitedAddressParser& 
	@pre 			The address segment has been parsed.
 */
	const TDelimitedAddressParser& ExtractAddresses() const;

/**
	@fn				const TDelimitedBodyParser& ExtractBody()
	Intended Usage	:	Returns TDelimitedBodyParser
	@since			7.0
	@return			const TDelimitedBodyParser& 
	@pre 			The body segment has been parsed
 */
	const TDelimitedBodyParser& ExtractBody() const;

private:
	/** Contains the parsed address segment */
	TDelimitedAddressParser iAddressParser;
	/** Contains the parsed body segment */
	TDelimitedBodyParser iBodyParser;
	};


/**
	@typedef TDelimitedSmsParser
	Comments :	For parsing sms urls delimited by a ','.
				This class is used by CSmsSchemeHandler class.

	@since 7.0

@internalComponent
@released
*/
typedef TDelimitedAddressParser TDelimitedSmsParser;


/**
	@class
	Comments :	This class contains parsing utility functions used by the
				scheme handler classes.
	@since 7.0
*/
class ParseUtil
/**
@internalComponent
@released
*/
	{
public:

/**
	@fn				Subset(const TDesC& aDes, const TDesC& aSubset, TPtrC& aData)
	Intended Usage	:	Sets the leftmost part of aDes up to the first non-allowable 
						character.
	@since			7.0
	@param			aDes		A descriptor to be parsed. 
	@param			aSubset		A descriptor containing the allowed characters.
	@param			aData		This is an output argument that is set to the 
								parsed descriptor. 
 */
	static void Subset(const TDesC& aDes, const TDesC& aSubset, TPtrC& aData);
	};

#endif
