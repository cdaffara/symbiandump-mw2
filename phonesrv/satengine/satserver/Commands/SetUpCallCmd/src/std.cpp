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
* Description:  General functions
*
*/



// INCLUDE FILES
#include    <e32base.h>
#include    <implementationproxy.h>
#include    "CSetUpCallHandler.h"

// CONSTANTS

const TImplementationProxy implementationTable[] =
    {
    //lint -e{611,1924} Warning "Suspicious cast" can not be avoided.
    IMPLEMENTATION_PROXY_ENTRY( 0x1000f006, CSetUpCallHandler::NewL )
    };

// ========================== OTHER EXPORTED FUNCTIONS =========================

// -----------------------------------------------------------------------------
// ImplementationGroupProxy ECOM framework factory function
// Calculates the number of implementations.
// Returns: Implementation proxy.
// -----------------------------------------------------------------------------
//
EXPORT_C const TImplementationProxy* ImplementationGroupProxy(
    TInt& aTableCount ) // Number of implementations
    {
    aTableCount =
        sizeof( implementationTable ) /
        sizeof( TImplementationProxy );
    return implementationTable;
    }

//  End of File
