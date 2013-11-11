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

#ifndef TMSRINGTONEBODYIMPL_H
#define TMSRINGTONEBODYIMPL_H

#include <w32std.h>
#include <tms.h>
#include "tmsringtonebody.h"

namespace TMS {

// FORWARD DECLARATIONS
class TMSRingToneObserver;
class TMSProxy;
class TMSRingTone;

// TMSRingToneBodyImpl class
class TMSRingToneBodyImpl : public TMSRingToneBody
    {
public:
    static gint Create(TMSRingToneBody*& bodyimpl);
    static gint Create(TMSRingToneBody*& bodyimpl, RWindow& window, gint scrid);
    virtual ~TMSRingToneBodyImpl();

    // From TMSRingToneBody
    virtual gint AddObserver(TMSRingToneObserver& obsrvr, gpointer user_data);
    virtual gint RemoveObserver(TMSRingToneObserver& obsrvr);
    virtual gint Init(const TMSRingToneType type, GString* str, GString* tts);
    virtual gint Deinit();
    virtual gint Play();
    virtual gint Stop();
    virtual gint Pause();
    virtual gint Mute();

    void SetParent(TMSRingTone*& parent);

private:
    TMSRingToneBodyImpl();
    gint PostConstruct();

private:
    TMSRingToneObserver* iObserver;
    gpointer iUserData;
    TMSProxy* iProxy;
    TMSRingTone* iParent;
    guint iClientId;
    gint iScreenID;
    };

} //namespace TMS

#endif //TMSRINGTONEBODYIMPL_H

// End of file

