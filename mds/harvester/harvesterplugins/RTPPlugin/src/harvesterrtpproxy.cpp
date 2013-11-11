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
* Description:  
*
*/


#include <e32std.h>
#include <implementationproxy.h>
#include "harvesterrtpplugin.h"

// --------------------------------------------------------------------------
// Provide a key pair value table.
// Used to identify the correct construction function
// for the requested interface.
// --------------------------------------------------------------------------
//
const TImplementationProxy ImplementationTable[] =
    {
    { { 0x2000B434 }, ( TProxyNewLPtr ) CHarvesterRtpPlugin::NewL }
    };

// --------------------------------------------------------------------------
// Return an instance of the proxy table.
// --------------------------------------------------------------------------
//
EXPORT_C const TImplementationProxy* ImplementationGroupProxy( TInt& aTableCount )
    {
    aTableCount = sizeof( ImplementationTable ) / sizeof( TImplementationProxy );
    return ImplementationTable;
    }
