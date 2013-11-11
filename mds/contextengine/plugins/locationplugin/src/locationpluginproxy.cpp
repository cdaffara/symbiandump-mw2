/*
* Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  A Context Engine Plug-in to get location context information.
*
*/


#include <e32std.h>
#include <implementationproxy.h>
#include "locationcontextplugin.h"

// Map the interface UIDs to implementation factory functions
const TImplementationProxy KImplementationTable[] = 
    {
        {{0x200071D0},	(TProxyNewLPtr)CLocationContextPlugin::NewL}
    };

// Exported proxy for instantiation method resolution
EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
    {
    aTableCount = sizeof(KImplementationTable) / sizeof(TImplementationProxy);
    return KImplementationTable;
    }
