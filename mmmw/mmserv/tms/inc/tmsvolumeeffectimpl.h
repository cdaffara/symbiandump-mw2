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

#ifndef TMS_VOLUME_EFFECT_IMPL_H
#define TMS_VOLUME_EFFECT_IMPL_H

#include <glib.h>
#include <tmsvolumeeffect.h>

namespace TMS {

// FORWARD DECLARATIONS
class TMSCallProxy;

// TMSVolumeEffectImpl class
class TMSVolumeEffectImpl : public TMSVolumeEffect
    {
public:
    virtual ~TMSVolumeEffectImpl();
    static gint Create(TMSEffect*& tmseffect);
    gint SetProxy(TMSCallProxy* aProxy, gpointer queuehandler);

private:
    TMSVolumeEffectImpl();
    gint PostConstruct();
    gint SetParent(TMSEffect*& parent);
    };

} //namespace TMS

#endif // TMS_VOLUME_EFFECT_IMPL_H

