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

#ifndef TMS_INBAND_BODY_IMPL_H
#define TMS_INBAND_BODY_IMPL_H

#include <tms.h>
#include "tmsinbandtonebody.h"
#include "tmsclientserver.h"

namespace TMS {

// FORWARD DECLARATIONS
class TMSInbandToneObserver;
class TMSInbandTone;
class TMSProxy;

// TMSInbandBodyImpl class
class TMSInbandToneBodyImpl : public TMSInbandToneBody
    {
public:
    static gint Create(TMSInbandToneBody*& bodyimpl);
    virtual ~TMSInbandToneBodyImpl();

    // From TMSInbandToneBody begins
    virtual gint AddObserver(TMSInbandToneObserver& obsrvr, gpointer user_data);
    virtual gint RemoveObserver(TMSInbandToneObserver& obsrvr);
    virtual gint Start(TMSInbandToneType inbandtone);
    virtual gint Stop();

    void SetParent(TMSInbandTone*& parent);

private:
    TMSInbandToneBodyImpl();
    gint PostConstruct();

private:
    TMSInbandToneObserver* iObserver;
    gint iClientId;
    gpointer iUserData;
    TMSProxy* iProxy;
    TMSInbandTone* iParent;
    TMSInbandToneType iInbandType;
    };

} //namespace TMS

#endif // TMS_DTMF_BODY_IMPL_H

