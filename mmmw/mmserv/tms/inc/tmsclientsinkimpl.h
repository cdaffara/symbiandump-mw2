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

#ifndef TMS_CLIENT_SINK_IMPL_H
#define TMS_CLIENT_SINK_IMPL_H

#include <glib.h>
#include <tmsclientsink.h>

namespace TMS {

// FORWARD DECLARATIONS
class TMSGlobalContext;

// TMSClientSinkImpl class
class TMSClientSinkImpl : public TMSClientSink
    {
public:
    virtual ~TMSClientSinkImpl();
    static gint Create(TMSSink*& tmssink);
    gint SetProxy(TMSGlobalContext* context, gpointer queuehandler);

private:
    TMSClientSinkImpl();
    gint PostConstruct();
    };

} //namespace TMS

#endif // TMS_CLIENT_SINK_IMPL_H

// End of file
