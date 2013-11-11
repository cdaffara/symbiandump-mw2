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

#ifndef __CURILISTLOOKUP_H__
#define __CURILISTLOOKUP_H__

#include <e32base.h>
#include <push/murilistlookup.h>

/**
The CUriListLookup class allows the caller to determine the list type, either blacklist 
or whitelist, of the supplied source URI.

@internalComponent
@released
*/
class CUriListLookup: public CBase, public MUriListLookup
	{
	private:
		CUriListLookup();
	
	public:
		~CUriListLookup();
		static MUriListLookup* NewL();
		TInt GetListType(const TDesC8& aServerAddress, InetUriList::TListType& aListType);
	};

#endif	// __CURILISTLOOKUP_H__


