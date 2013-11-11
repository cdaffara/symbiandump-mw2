/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Main application methods
*
*/


#include <e32base.h>
#include <ecom/ecom.h>
#include <ecom/implementationproxy.h>

#include "debug.h"
#include "mnrpcoordconverter.h"

// ========================== GLOBAL FUNCTIONS ===============================

// ---------------------------------------------------------------------------
// Table of implementations
// ---------------------------------------------------------------------------
//
const TImplementationProxy ImplementationTable[] =
    {
    // Note! UID below is implementation UID, not DLL UID.
    IMPLEMENTATION_PROXY_ENTRY( 0x03133038, CMnrpCoordConverter::NewL ),
    };

// ---------------------------------------------------------------------------
// Note this function must be exported at ordinal 1
// ---------------------------------------------------------------------------
//
EXPORT_C const TImplementationProxy* ImplementationGroupProxy(
    TInt& aTableCount)
    {
    aTableCount = 1;
    return ImplementationTable;
    }
