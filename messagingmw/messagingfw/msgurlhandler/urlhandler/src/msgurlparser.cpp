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
// This file contains the implementation for the classes defined in
// MsgUrlParser.h
// 
//

/**
 @file
 @see	MsgUrlParser.h
*/

// Local includes
//
#include "msgurlparser.h"

TDelimitedAddressParser::TDelimitedAddressParser()
: TDelimitedParserBase16()
	{
	SetDelimiter(TChar(','));
	}

void TDelimitedAddressParser::Parse(const TDesC& aAddress)
	{
	// Call base class function as it is protected
	TDelimitedParserBase16::Parse(aAddress);
	}

TMailtoUrlParser::TMailtoUrlParser()
	{
	}

void TMailtoUrlParser::Parse(const TDesC& aData)
	{
	// Contains the address segment
	TPtrC addressData;

	// Contains the body segment
	TPtrC bodyData;

	// Look for segment separator
	TInt pos = aData.Locate('?');
	if (pos != KErrNotFound)
		{
		addressData.Set(aData.Left(pos));

		// Sets everything after the segment separator - excludes '?' delimiter
		bodyData.Set(aData.Mid(pos+1));
		}
	else 
		{
		addressData.Set(aData);
		bodyData.Set(KNullDesC);
		}

	// Parse the address segment
	iAddressParser.Parse(addressData);

	// Parse the body segment
	iBodyParser.Parse(bodyData);
	}

const TDelimitedAddressParser& TMailtoUrlParser::ExtractAddresses() const
	{
	return iAddressParser;
	}

const TDelimitedBodyParser& TMailtoUrlParser::ExtractBody() const
	{
	return iBodyParser;
	}

void ParseUtil::Subset(const TDesC& aDes, const TDesC& aSubset, TPtrC& aData)
	{
	TInt pos=0;

	while (pos<aDes.Length() && aSubset.Locate(aDes[pos]) != KErrNotFound)
		{
		++pos;
		}

	aData.Set(aDes.Left(pos));
	}
