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

#ifndef TMS_CLIENT_SOURCE_BODY_H
#define TMS_CLIENT_SOURCE_BODY_H

#include <tms.h>

namespace TMS {

// FORWARD DECLARATIONS
class TMSClientSourceObserver;
class TMSBuffer;

// TMSClientSourceBody class
class TMSClientSourceBody
    {
public:
    virtual ~TMSClientSourceBody() {}

    virtual gint AddObserver(TMSClientSourceObserver& obsrvr,
            gpointer user_data) = 0;

    virtual gint RemoveObserver(TMSClientSourceObserver& obsrvr) = 0;

    // In pull mode, client calls this.??? How to identify last buffer.
    // Option 1 is to move setlast buffer to TMSBuffer interface.
    // Option 2 is to have overloaded function with another parameter.
    virtual gint BufferFilled(TMSBuffer& buffer) = 0;
    // Push mode
    virtual gint ProcessBuffer(TMSBuffer* buffer) = 0;
    // Indicates framework to queue ProcessBuffer. default is off
    // unsupported in pull mode??? (atleast initially)
    virtual gint SetEnqueueMode(const gboolean enable) = 0;
    virtual gint GetEnqueueMode(gboolean& enable) = 0;
    // Sends batch data to framework and clears queue mode.
    // Valid only when queue mode is set, otherwise no-op
    virtual gint Flush() = 0;

    virtual gint GetType(TMSSourceType& sourcetype) = 0;
    };

} //namespace TMS

#endif //TMS_CLIENT_SOURCE_BODY_H
