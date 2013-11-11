// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef CIPADDRESS___
#define CIPADDRESS___

#include <in_sock.h>

/** Size of CIpAddress buffer. */
const TInt KIpAddressSize = 39; // large enough for full IPv6 address = 39 (8 * 4 + 7)

class CIpAddress : public CBase
/** Utility class to hold an IP address as a string on the heap.

This can be useful for parsers that have IP address fields. 
@publishedAll
@released
*/
	{
	public:
		IMPORT_C static CIpAddress*	NewL(const TDesC& aAddr);
		IMPORT_C static CIpAddress*	NewLC(const TDesC& aAddr);
		inline static CIpAddress*	NewL();
		inline static CIpAddress*	NewLC();
		IMPORT_C 					~CIpAddress();
		IMPORT_C void				SetAddrL(const TDesC& aAddr);
		IMPORT_C const TDesC&		Addr() const;
	private:
		CIpAddress();
	private:
		HBufC*				iAddr;
	};

inline CIpAddress* CIpAddress::NewL()
/** Allocates and constructs a new empty IP address object.

@return New IP address object */
	{
	return NewL(KNullDesC);
	}

inline CIpAddress* CIpAddress::NewLC()
/** Allocates and constructs a new empty IP address object, leaving the object 
on the cleanup stack.

@return New IP address object */
	{
	return NewLC(KNullDesC);
	}

#endif
