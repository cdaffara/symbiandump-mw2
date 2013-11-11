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

#ifndef TMS_CLIENT_SOURCE_BODY_IMPL_H
#define TMS_CLIENT_SOURCE_BODY_IMPL_H

#include <tms.h>
#include "tmsclientsourcebody.h"
#include "tmsqueuehandler.h"

namespace TMS {

// FORWARD DECLARATIONS
class TMSClientSourceObserver;
class TMSBuffer;
class TMSGlobalContext;
class TMSCallProxy;

// TMSClientSourceBodyImpl class
class TMSClientSourceBodyImpl : public TMSClientSourceBody,
                                public MQueueHandlerObserver
    {
public:
    static gint Create(TMSClientSourceBody*& bodyimpl);

    virtual ~TMSClientSourceBodyImpl();

    virtual gint AddObserver(TMSClientSourceObserver& obsrvr,
            gpointer user_data);
    virtual gint RemoveObserver(TMSClientSourceObserver& obsrvr);

    // In pull mode, client calls this.??? How to identify last buffer.
    // Option 1 is to move setlast buffer to TMSBuffer interface.
    // Option 2 is to have overloaded function with another parameter.
    virtual gint BufferFilled(TMSBuffer& buffer);
    // Push mode
    virtual gint ProcessBuffer(TMSBuffer* buffer);
    // Indicates framework to queue ProcessBuffer. default is off
    // unsupported in pull mode??? (atleast initially)
    virtual gint SetEnqueueMode(const gboolean enable);
    virtual gint GetEnqueueMode(gboolean& enable);
    // Sends batch data to framework and clears queue mode.
    // Valid only when queue mode is set, otherwise no-op
    virtual gint Flush();

    virtual gint GetType(TMSSourceType& sourcetype);

    // From MQueueHandlerObserver
    virtual void QueueEvent(TInt aEventType, TInt aError, void* user_data);

    void SetProxy(TMSGlobalContext* context, gpointer queuehandler);

private:
    TMSClientSourceBodyImpl();
    gint PostConstruct();

private:
    TMSClientSourceObserver* iObserver;
    gpointer iUserData;
    TMSGlobalContext* iContext;
    TMSCallProxy* iProxy;
    gboolean iQueueMode;
    };

} //namespace TMS

#endif // TMS_CLIENT_SOURCE_BODY_IMPL_H

// End of file
