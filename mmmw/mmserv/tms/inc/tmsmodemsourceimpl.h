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

#ifndef TMS_MODEM_SOURCE_IMPL_H
#define TMS_MODEM_SOURCE_IMPL_H

#include <glib.h>
#include <tmsmodemsource.h>

namespace TMS {

// FORWARD DECLARATIONS
class TMSCallProxy;

// TMSModemSourceImpl class
class TMSModemSourceImpl : public TMSModemSource
    {
public:
    virtual ~TMSModemSourceImpl();
    static gint Create(TMSSource*& tmssource);
    gint SetProxy(TMSCallProxy* aProxy, gpointer queuehandler);

private:
    TMSModemSourceImpl();
    gint PostConstruct();
    };

} //namespace TMS

#endif // TMS_Modem_SOURCE_IMPL_H

// End of file
