// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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
//

/**
 @file
 @publishedPartner
 @released
*/

#ifndef __MURILISTLOOKUP_H__
#define __MURILISTLOOKUP_H__

#include <e32std.h>

#include <ineturilist.h>
#include <ineturilistdef.h>

/**
The MUriListLookup class allows the caller to determine the list type, either blacklist 
or whitelist, of the supplied source URI.

@released
@publishedPartner
*/
class MUriListLookup
	{
public: // methods

/**
	Retrieves the list type of a given URI. 
	@param	aServerAddress	The source URI.
	@param	aListType		On return, contains the list type of the source URI.
	@return KErrUriNotFound The URI is not found in the list.
*/
	virtual TInt GetListType(const TDesC8& aServerAddress, InetUriList::TListType& aListType) = 0;
public:
	/** Destructor key */
	TUid iEcomDtorID;	
	};

#endif	// __MURILISTLOOKUP_H__

