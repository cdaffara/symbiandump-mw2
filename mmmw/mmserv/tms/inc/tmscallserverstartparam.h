/*
 * Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description: Telephony Multimedia Service
 *
 */

#ifndef TMSCALLSERVERSTARTPARAM_H
#define TMSCALLSERVERSTARTPARAM_H

#include <glib.h>
#include <e32std.h>

namespace TMS {

const gint KTMSCallServerStackSize = 0x5000; //  20 KB
const gint KTMSCallServerInitHeapSize = 0x500000; // 5.2 MB
const gint KTMSCallServerMaxHeapSize = 0x1800000; //  25 MB

// FORWARD DECLARATIONS
class TMSServer;

// TMSCallServerStartParam
class TMSCallServerStartParam
    {
public:
    TMSCallServerStartParam(TMSServer* aTMSServer, RServer2& aHandle);

public:
    TMSServer* iTMSServer;
    RServer2& iTMSCallServerHandle;
    };

} //namespace TMS

#include "tmscallserverstartparam.inl"

#endif // TMSCALLSERVERSTARTPARAM_H

// End of file
