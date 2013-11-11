// Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
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

// System includes
//
#include <e32std.h>
#include <ecom/implementationproxy.h>

// Local includes
//
#include "CUnknownContentHandler.h"
#include "CMultiPartMixedContentHandler.h"
#include "CMultiPartRelAndAltContentHandler.h"

const TImplementationProxy ImplementationTable[] = 
	{
		IMPLEMENTATION_PROXY_ENTRY(0x101F3E60,	CUnknownContentHandler::NewL),
		IMPLEMENTATION_PROXY_ENTRY(0x101F3E64,	CMultiPartMixedContentHandler::NewL),
		IMPLEMENTATION_PROXY_ENTRY(0x101F3E65,	CMultiPartRelAndAltContentHandler::NewL)

	};

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
	{
	aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);

	return ImplementationTable;
	}
