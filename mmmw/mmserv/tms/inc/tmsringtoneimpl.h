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

#ifndef TMS_RINGTONEIMPL_H_
#define TMS_RINGTONEIMPL_H_

#include <w32std.h>
#include <tms.h>
#include <tmsringtone.h>

namespace TMS {

// TMSRingToneImpl class
class TMSRingToneImpl : public TMSRingTone
    {
public:
    virtual ~TMSRingToneImpl();

    IMPORT_C static gint Create(TMSRingTone*& tmsrt);
    IMPORT_C static gint Create(TMSRingTone*& tmsrt, RWindow& window,
            gint scrid);
    IMPORT_C static gint Delete(TMSRingTone*& tmsrt);

private:
    TMSRingToneImpl();
    gint PostConstruct();
    gint PostConstruct(RWindow& window, gint scrid);
    gint SetParent(TMSRingTone*& parent);
    };

} //namespace TMS

#endif //TMS_RINGTONEIMPL_H_
