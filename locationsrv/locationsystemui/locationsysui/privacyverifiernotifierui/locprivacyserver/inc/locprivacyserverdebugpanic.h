/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Panic code definitions for server.
*
*/

#ifndef LOCPRIVACYSERVERDEBUGPANIC_H
#define LOCPRIVACYSERVERDEBUGPANIC_H

#include <e32def.h>

_LIT(KLocPrivSrvDebugPanicCategory, "Loc Priv debug");

enum TLocPrivSrvDebugPanic
    {
    // Unknown request recieved from client.
    ELocPrivSrvPanicUnknownActivity = 0
    };

#endif      // LOCPRIVACYSERVERDEBUGPANIC_H

// End of File
