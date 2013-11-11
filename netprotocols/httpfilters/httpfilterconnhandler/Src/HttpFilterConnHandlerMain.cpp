/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  ConnHandler filter main file.
*
*/

// INCLUDE FILES
#include <e32std.h>
#include <ecom/implementationproxy.h>
#include "HttpFilterConnHandler.h"

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES

// CONSTANTS
const TImplementationProxy KImplementationTable[] =
    {
        {{KConnHandlerFilterUidValue}, (TProxyNewLPtr) CHttpFilterConnHandler::InstallFilterL}
    };

// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// ============================= LOCAL FUNCTIONS ===============================


// ============================ MEMBER FUNCTIONS ===============================

// ========================== OTHER EXPORTED FUNCTIONS =========================

// -----------------------------------------------------------------------------
// ImplementationGroupConnHandler
// Returns the filters implemented in this DLL
// Returns: The filters implemented in this DLL
// -----------------------------------------------------------------------------
//
EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
{
    aTableCount = sizeof(KImplementationTable) / sizeof(TImplementationProxy);
    return KImplementationTable;
}

// -----------------------------------------------------------------------------
// PanicHttpFiltersConnHandler
// Panic
// Returns: void
// -----------------------------------------------------------------------------
//
void PanicHttpFiltersConnHandler(TInt aErr = 0)
{
    User::Panic(_L("HTTP FILTER CONNHANDLER"), aErr);
}

//  End of File
