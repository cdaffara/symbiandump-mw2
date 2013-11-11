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

#ifndef TMS_MODEM_SINK_IMPL_H
#define TMS_MODEM_SINK_IMPL_H

#include <glib.h>
#include <tmsmodemsink.h>

namespace TMS {

// FORWARD DECLARATIONS
class TMSCallProxy;

// TMSModemSinkImpl class
class TMSModemSinkImpl : public TMSModemSink
    {
public:
    virtual ~TMSModemSinkImpl();
    static gint Create(TMSSink*& tmssink);
    gint SetProxy(TMSCallProxy* aProxy, gpointer queuehandler);

private:
    TMSModemSinkImpl();
    gint PostConstruct();
    };

} //namespace TMS

#endif // TMS_Modem_SINK_IMPL_H

// End of file
