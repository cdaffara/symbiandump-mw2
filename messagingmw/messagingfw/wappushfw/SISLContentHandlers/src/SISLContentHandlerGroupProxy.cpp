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

// System includes
//
#include <e32std.h>
#include <ecom/implementationproxy.h>

// Local includes
//
#include "CSLContentHandler.h"
#include "CSIContentHandler.h"

const TImplementationProxy ImplementationTable[] = 
	{
		IMPLEMENTATION_PROXY_ENTRY(0x101F3E61,	CSIContentHandler::NewL),
		IMPLEMENTATION_PROXY_ENTRY(0x101F3E63,	CSLContentHandler::NewL)
	};

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
	{
	aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);

	return ImplementationTable;
	}

#ifndef EKA2
// NOTE - had to add this here as was missing for this DLL. It appeared to be 
// using one of the E32Dll entry points that was export (?!) from one of the
// other WAP Push libraries that this links to. Doh!
GLDEF_C TInt E32Dll(TDllReason/* aReason*/)	
	{
	return (KErrNone);
	}
#endif
