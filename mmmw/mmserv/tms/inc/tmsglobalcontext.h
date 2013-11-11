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

#ifndef GLOBALCONTEXT_H
#define GLOBALCONTEXT_H

#include <tms.h>

namespace TMS {

// FORWARD DECLARATIONS
class TMSCallProxy;

// TMSGlobalContext class
class TMSGlobalContext
    {
public:
    TMSCallType CallType;
    TMSStreamType StreamType;
    gint StreamId;
    TMSCallProxy* CallProxy;
    };

} //namespace TMS

#endif // GLOBALCONTEXT_H

// End of file
