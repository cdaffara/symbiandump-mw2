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

#ifndef TMS_DTMF_BODY_IMPL_H
#define TMS_DTMF_BODY_IMPL_H

#include <tms.h>
#include "tmsdtmfbody.h"
#include "tmsclientserver.h"

namespace TMS {

// FORWARD DECLARATIONS
class TMSDTMFObserver;
class TMSDTMF;
class TMSProxy;

// TMSDTMFBodyImpl class
class TMSDTMFBodyImpl : public TMSDTMFBody
    {
public:
    static gint Create(TMSStreamType streamtype, TMSDTMF& parent,
            TMSDTMFBody*& bodyimpl);
    virtual ~TMSDTMFBodyImpl();

    // From TMSDTMFBody
    virtual gint AddObserver(TMSDTMFObserver& obsrvr, gpointer user_data);
    virtual gint RemoveObserver(TMSDTMFObserver& obsrvr);
    virtual gint Start();
    virtual gint Stop();
    virtual gint SetTone(GString* string);
    virtual gint ContinueDTMFStringSending(gboolean sending);

private:
    TMSDTMFBodyImpl();
    gint PostConstruct(TMSStreamType stype, TMSDTMF& parent);

private:
    TMSDTMFObserver* iObserver;
    gint iClientId;
    gpointer iUserData;
    TMSProxy* iProxy;
    TMSDTMF* iParent;
    GString* iString;
    TMSStreamType iStreamType;
    };

} //namespace TMS

#endif // TMS_DTMF_BODY_IMPL_H

