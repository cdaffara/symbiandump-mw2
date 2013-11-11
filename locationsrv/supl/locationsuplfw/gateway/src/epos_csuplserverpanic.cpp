/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   Server Panic handler
*
*/



// INCLUDE FILES
#include <e32base.h>
#include "epos_csuplserverpanic.h"
#include "epos_csuplglobal.h"

// CONSTANTS
_LIT(KSuplPanicCategory, "EPosSuplGateway");

// CONSTANTS
//#ifdef _DEBUG
_LIT(KTraceFileName, "SUPL_GW::epos_csuplserverpanic.cpp");
//#endif

// IMPLEMENTATION

// ---------------------------------------------------------
// DebugPanic
// ---------------------------------------------------------
GLDEF_C void DebugPanic(TSuplServerDebugPanic aPanic)
    {
    DEBUG_TRACE("EPosSuplGateway panicked with error code: ", aPanic);
    User::Panic(KSuplPanicCategory, aPanic);
    }

// End of file
