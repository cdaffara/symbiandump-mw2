/*
* Copyright (c) 2002 - 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Contains the GSM specific implementation of NetworkHandlingProxy.h 
*
*/


// INCLUDE FILES
#include <networkhandlingproxy.h>
#include "CNWGsmSessionImplementation.h"
#include "NWLogger.h"


// ========================== OTHER EXPORTED FUNCTIONS =========================

// -----------------------------------------------------------------------------
// CreateL implements NetworkHandlingProxy.h
// 
// Returns: pointer to the GSM specific CNWSession object 
// -----------------------------------------------------------------------------
//
EXPORT_C CNWSession* CreateL(
        MNWMessageObserver& aMessage,
        TNWInfo& aTNWInfo)
    {
    NWLOGSTRING( KNWOBJECT,
        "NW: CNWSession::CreateL() Called" );
    
    return CNWGsmSessionImplementation::NewL( aMessage, aTNWInfo );
    }

//  End of File 

