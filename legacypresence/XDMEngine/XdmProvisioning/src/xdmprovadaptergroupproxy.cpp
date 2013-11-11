/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Table of plugins for ECom
*
*/





// INCLUDE FILES
#include <implementationproxy.h>

#include "xdmprovadapter.h"
#include "xdmprovuid.h"


// CONSTANTS
const TImplementationProxy KImplementationTable[] = 
    {
#ifdef __EABI__    
    IMPLEMENTATION_PROXY_ENTRY( KXdmProvAdapterImplementation, CXdmProvAdapter::NewL )
#else
    { { KXdmProvAdapterImplementation }, CXdmProvAdapter::NewL },
#endif
    };

// ========================== OTHER EXPORTED FUNCTIONS =========================

// -----------------------------------------------------------------------------
// ImplementationGroupProxy
// -----------------------------------------------------------------------------
//
EXPORT_C const TImplementationProxy* ImplementationGroupProxy( TInt& aTableCount )
    {
    aTableCount = sizeof( KImplementationTable ) / sizeof( TImplementationProxy );
    return KImplementationTable;
    }


