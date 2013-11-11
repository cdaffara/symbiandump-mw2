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

#ifndef TMS_GAIN_EFFECT_BODY_IMPL_H
#define TMS_GAIN_EFFECT_BODY_IMPL_H

#include <tms.h>
#include "tmsgaineffectbody.h"
#include "tmsqueuehandler.h"

namespace TMS {

// FORWARD DECLARATIONS
class TMSEffectObserver;
class TMSCallProxy;
class TMSEffect;

// TMSGainEffectBodyImpl class
class TMSGainEffectBodyImpl : public TMSGainEffectBody,
                              public MQueueHandlerObserver
    {
public:
    static gint Create(TMSGainEffectBody*& bodyimpl);
    virtual ~TMSGainEffectBodyImpl();

    // From TMSGainEffectBody
    virtual gint AddObserver(TMSEffectObserver& obsrvr, gpointer user_data);
    virtual gint RemoveObserver(TMSEffectObserver& obsrvr);
    virtual gint SetLevel(const guint level);
    virtual gint GetLevel(guint& level);
    virtual gint GetMaxLevel(guint& level);
    virtual gint GetType(TMSEffectType& effecttype);

    // From MQueueHandlerObserver
    virtual void QueueEvent(TInt aEventType, TInt aError, void* user_data);

    void SetParent(TMSEffect*& parent);
    void SetProxy(TMSCallProxy* aProxy, gpointer queuehandler);

private:
    TMSGainEffectBodyImpl();
    gint PostConstruct();

private:
    TMSEffectObserver* iObserver;
    gpointer iUserData;
    TMSCallProxy* iProxy;
    TMSEffect* iParent;
    };

} //namespace TMS

#endif // TMS_GAIN_EFFECT_BODY_IMPL_H

