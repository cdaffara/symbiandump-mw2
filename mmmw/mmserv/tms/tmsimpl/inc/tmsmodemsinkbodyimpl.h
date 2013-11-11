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

#ifndef TMS_MODEM_SINK_BODY_IMPL_H
#define TMS_MODEM_SINK_BODY_IMPL_H

#include <tms.h>
#include "tmsmodemsinkbody.h"

namespace TMS {

// FORWARD DECLARATIONS
class TMSCallProxy;

// TMSModemSinkBodyImpl class
class TMSModemSinkBodyImpl : public TMSModemSinkBody
    {
public:
    static gint Create(TMSModemSinkBody*& bodyimpl);
    virtual ~TMSModemSinkBodyImpl();

    // From TMSModemSinkBody
    virtual gint GetType(TMSSinkType& sinktype);

    void SetProxy(TMSCallProxy* aProxy, gpointer queuehandler);

private:
    TMSModemSinkBodyImpl();
    gint PostConstruct();

private:
    TMSCallProxy* iProxy;
    };

} //namespace TMS

#endif // TMS_MODEM_SINK_BODY_IMPL_H

// End of file
