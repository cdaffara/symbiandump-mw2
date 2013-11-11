/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Factory method for creating SatServer
*
*/



#ifndef SATSERVERFACTORY_H
#define SATSERVERFACTORY_H

//  INCLUDES
#include <e32std.h>

// CONSTANTS

_LIT( KSatServerPanic, "SatServer" );

// Full server file path and name.
// Server name.
_LIT( KSatServerName, "SatServer" );

// Init error assertion string.
_LIT( KSatSInitError, "Error initalizing SatServer" );

// FORWARD DECLARATIONS
class CSatSServer;

/**
* Creates an instance of CSatSServer
*/
//lint -e{1717} void declaration not possible
IMPORT_C CSatSServer* CreateSatServerL();

#endif      // SATSERVERFACTORY_H

// End of File
