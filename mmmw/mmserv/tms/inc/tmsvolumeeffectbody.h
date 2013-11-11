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

#ifndef TMS_VOLUME_EFFECT_BODY_H
#define TMS_VOLUME_EFFECT_BODY_H

#include <tms.h>

namespace TMS {

// FORWARD DECLARATIONS
class TMSEffectObserver;

// TMSVolumeEffectBody class
class TMSVolumeEffectBody
    {
public:
    virtual ~TMSVolumeEffectBody() {}

    virtual gint AddObserver(TMSEffectObserver& obsrvr, gpointer user_data) = 0;
    virtual gint RemoveObserver(TMSEffectObserver& obsrvr) = 0;
    virtual gint SetLevel(const guint level) = 0;
    virtual gint GetLevel(guint& level) = 0;
    virtual gint GetMaxLevel(guint& level) = 0;
    virtual gint GetType(TMSEffectType& effecttype) = 0;
    };

} //namespace TMS

#endif //TMS_VOLUME_EFFECT_BODY_H
