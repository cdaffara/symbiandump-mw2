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

#ifndef TMS_CLIENT_SINK_BODY_H
#define TMS_CLIENT_SINK_BODY_H

#include <tms.h>

namespace TMS {

// FORWARD DECLARATIONS
class TMSClientSinkObserver;
class TMSBuffer;

// CLASS DECLARATION
class TMSClientSinkBody
    {
public:
    virtual ~TMSClientSinkBody() {}

    virtual gint AddObserver(TMSClientSinkObserver& obsrvr,
            gpointer user_data) = 0;
    virtual gint RemoveObserver(TMSClientSinkObserver& obsrvr) = 0;

    // Push mode
    virtual gint BufferProcessed(TMSBuffer* buffer) = 0;

    virtual gint GetType(TMSSinkType& Sinktype) = 0;
    };

} //namespace TMS

#endif //TMS_CLIENT_SINK_BODY_H
