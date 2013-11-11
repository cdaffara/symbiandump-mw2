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

#ifndef TMS_MODEM_SOURCE_BODY_IMPL_H
#define TMS_MODEM_SOURCE_BODY_IMPL_H

#include <tms.h>
#include "tmsmodemsourcebody.h"

namespace TMS {

// FORWARD DECLARATIONS
class TMSCallProxy;

// TMSModemSourceBodyImpl class
class TMSModemSourceBodyImpl : public TMSModemSourceBody
    {
public:
    static gint Create(TMSModemSourceBody*& bodyimpl);
    virtual ~TMSModemSourceBodyImpl();

    // From TMSModemSourceBody begins
    virtual gint GetType(TMSSourceType& sourcetype);

    void SetProxy(TMSCallProxy* aProxy, gpointer queuehandler);

private:
    TMSModemSourceBodyImpl();
    gint PostConstruct();

private:
    TMSCallProxy* iProxy;
    };

} //namespace TMS

#endif // TMS_MODEM_SOURCE_BODY_IMPL_H

// End of file
