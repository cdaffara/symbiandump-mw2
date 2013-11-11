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

#ifndef TMS_CLIENT_SOURCE_IMPL_H
#define TMS_CLIENT_SOURCE_IMPL_H

#include <glib.h>
#include <tmsclientsource.h>

namespace TMS {

// FORWARD DECLARATIONS
class TMSGlobalContext;
class TMSCallProxy;

// TMSClientSourceImpl class
class TMSClientSourceImpl : public TMSClientSource
    {
public:
    virtual ~TMSClientSourceImpl();
    static gint Create(TMSSource*& tmssource);
    gint SetProxy(TMSGlobalContext* context, gpointer queuehandler);

private:
    TMSClientSourceImpl();
    gint PostConstruct();
    };

} //namespace TMS

#endif // TMS_CLIENT_SOURCE_IMPL_H

// End of file
