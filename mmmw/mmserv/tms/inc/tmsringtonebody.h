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

#ifndef TMS_RINGTONE_BODY_H
#define TMS_RINGTONE_BODY_H

#include <tms.h>

namespace TMS {

// FORWARD DECLARATIONS
class TMSRingToneObserver;

// TMSRingToneBody class
class TMSRingToneBody
    {
public:
    virtual ~TMSRingToneBody() {}

    virtual gint AddObserver(TMSRingToneObserver& obsrvr,
            gpointer user_data) = 0;
    virtual gint RemoveObserver(TMSRingToneObserver& obsrvr) = 0;
    virtual gint Init(const TMSRingToneType type, GString* str,
            GString* tts) = 0;
    virtual gint Deinit() = 0;
    virtual gint Play() = 0;
    virtual gint Stop() = 0;
    virtual gint Pause() = 0;
    virtual gint Mute() = 0;
    };

} //namespace TMS

#endif //TMS_RINGTONE_BODY_H
