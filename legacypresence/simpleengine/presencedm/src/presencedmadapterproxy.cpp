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

#include "presencedmadapter.h"
#include "presencedmlogger.h"

const TUint KPresDMAdapterUid = 0x10282416; 

// -----------------------------------------------------------------------------
// TImplementationProxy ImplementationTable[]
// Needed because of ECOM architecture
// -----------------------------------------------------------------------------
const TImplementationProxy ImplementationTable[] = 
    {
#ifdef __EABI__    
    IMPLEMENTATION_PROXY_ENTRY( KPresDMAdapterUid, CPresenceDMAdapter::NewL )
#else
    { { KPresDMAdapterUid }, CPresenceDMAdapter::NewL },
#endif
    };

// -----------------------------------------------------------------------------
// ImplementationGroupProxy()
// Needed because of ECOM architecture
// -----------------------------------------------------------------------------
EXPORT_C const TImplementationProxy* ImplementationGroupProxy( TInt& 
                                                               aTableCount )
    {
    aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
    PRES_DM_LOG(LOG_LIT8("CPresenceDMAdapter:ImplementationGroupProxy"));
    return ImplementationTable;
    }
