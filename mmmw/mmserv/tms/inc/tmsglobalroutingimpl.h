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

#ifndef TMS_GLOBAL_ROUTING_IMPL_H
#define TMS_GLOBAL_ROUTING_IMPL_H

#include <glib.h>
#include <tmsglobalrouting.h>

namespace TMS {

// TMSGlobalRoutingImpl class
class TMSGlobalRoutingImpl : public TMSGlobalRouting
    {
public:
    virtual ~TMSGlobalRoutingImpl();
    IMPORT_C static gint Create(TMSGlobalRouting*& globrouting);
    IMPORT_C static gint Delete(TMSGlobalRouting*& globrouting);

private:
    TMSGlobalRoutingImpl();
    gint PostConstruct();
    gint SetParent(TMSGlobalRouting*& parent);
    };

} //namespace TMS

#endif // TMS_GLOBAL_ROUTING_IMPL_H

