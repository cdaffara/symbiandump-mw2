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

#ifndef TMS_GLOBAL_GAIN_EFFECT_BODY_IMPL_H
#define TMS_GLOBAL_GAIN_EFFECT_BODY_IMPL_H

#include <tms.h>
#include "tmsglobalgaineffectbody.h"

namespace TMS {

// FORWARD DECLARATIONS
class TMSEffectObserver;
class TMSProxy;
class TMSEffect;

// TMSGlobalGainEffectBodyImpl class
class TMSGlobalGainEffectBodyImpl : public TMSGlobalGainEffectBody
    {
public:
    static gint Create(TMSGlobalGainEffectBody*& bodyimpl);
    virtual ~TMSGlobalGainEffectBodyImpl();

    // From TMSGainEffectBody
    virtual gint AddObserver(TMSEffectObserver& obsrvr, gpointer user_data);
    virtual gint RemoveObserver(TMSEffectObserver& obsrvr);
    virtual gint SetLevel(const guint level);
    virtual gint GetLevel(guint& level);
    virtual gint GetMaxLevel(guint& level);
    virtual gint GetType(TMSEffectType& effecttype);

    void SetParent(TMSEffect*& parent);

private:
    TMSGlobalGainEffectBodyImpl();
    gint PostConstruct();

private:
    TMSEffectObserver* iObserver;
    gint iClientId;
    gpointer iUserData;
    TMSProxy* iProxy;
    TMSEffect* iParent;
    };

} //namespace TMS

#endif // TMS_GLOBAL_GAIN_EFFECT_BODY_IMPL_H

