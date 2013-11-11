/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Proxy implementation table.
*
*/



// INCLUDE FILES
#include <e32std.h>
#include <implementationproxy.h>
#include <cconvergedcallprovider.h>
#include "ccprefconvergedcallprovider.h"
#include "ccptestplugindefs.h"

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
const TImplementationProxy KImplementationTable[] = 
	{
	// Implementation UID of the plugin (see ccptestplugindefs.h)
	IMPLEMENTATION_PROXY_ENTRY( KPluginCsImplementationUid, CCPRefConvergedCallProvider::CsNewL ),
	IMPLEMENTATION_PROXY_ENTRY( KPluginVccImplementationUid, CCPRefConvergedCallProvider::VccNewL ),
	IMPLEMENTATION_PROXY_ENTRY( KPluginPsImplementationUid, CCPRefConvergedCallProvider::PsNewL )
	};


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
	{
	aTableCount = sizeof(KImplementationTable) / sizeof(TImplementationProxy);

	return KImplementationTable;
	}

//  End of File  
