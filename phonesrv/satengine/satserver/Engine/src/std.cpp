/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  General functions
*
*/



// INCLUDE FILES
#include    <e32std.h>
#include    "SatServerFactory.h"
#include    "CSatSServer.h"
#include    "SatLog.h"

// ========================== OTHER EXPORTED FUNCTIONS =========================

// -----------------------------------------------------------------------------
// Factory method for CSatSServer
// -----------------------------------------------------------------------------
EXPORT_C CSatSServer* CreateSatServerL()
    {
    LOG( SIMPLE, "SATENGINE: CreateSatServerL calling-exiting" )
    return CSatSServer::NewL();
    }

//  End of File
