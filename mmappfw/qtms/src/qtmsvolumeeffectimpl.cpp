/*
 * Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description: QT Bindings for TMS
 *
 */

#include <qtms.h>
#include <tmsvolumeeffect.h>
#include "qtmsvolumeeffectimpl.h"

using namespace QTMS;
using namespace TMS;

QTMSVolumeEffectImpl::QTMSVolumeEffectImpl()
{
}

QTMSVolumeEffectImpl::~QTMSVolumeEffectImpl()
{
    RemoveObserver(*this);
}

gint QTMSVolumeEffectImpl::Create(QTMSEffect*& qvol, TMS::TMSEffect*& tmsvol)
{
    gint ret(QTMS_RESULT_INSUFFICIENT_MEMORY);
    QTMSVolumeEffectImpl* self = new QTMSVolumeEffectImpl();
    if (self) {
        ret = self->PostConstruct();
        if (ret != QTMS_RESULT_SUCCESS) {
            delete self;
            self = NULL;
        }
        self->iEffect = tmsvol;
        ret = self->AddObserver(*self, NULL);
    }
    qvol = self;
    return ret;
}

gint QTMSVolumeEffectImpl::PostConstruct()
{
    gint ret(QTMS_RESULT_SUCCESS);
    return ret;
}

gint QTMSVolumeEffectImpl::AddObserver(TMS::TMSEffectObserver& obsrvr, gpointer user_data)
{
    gint ret(QTMS_RESULT_SUCCESS);

    if (iEffect) {
        ret = static_cast<TMSVolumeEffect*> (iEffect)->AddObserver(obsrvr, user_data);
    }
    return ret;
}

gint QTMSVolumeEffectImpl::RemoveObserver(TMS::TMSEffectObserver& obsrvr)
{
    gint ret(QTMS_RESULT_SUCCESS);

    if (iEffect) {
        ret = static_cast<TMSVolumeEffect*> (iEffect)->RemoveObserver(obsrvr);
    }
    return ret;
}

gint QTMSVolumeEffectImpl::GetEffect(TMS::TMSEffect*& tmseffect)
{
    gint ret(QTMS_RESULT_UNINITIALIZED_OBJECT);

    if (iEffect) {
        tmseffect = iEffect;
        ret = QTMS_RESULT_SUCCESS;
    }
    return ret;
}

void QTMSVolumeEffectImpl::EffectsEvent(const TMS::TMSEffect& /*tmseffect*/,
    TMS::TMSSignalEvent event)
{
    QTMSSignalEvent qevent;

    qevent.type = event.type;
    qevent.reason = event.reason;
    qevent.curr_state = event.curr_state;
    qevent.prev_state = event.prev_state;
    qevent.event_data = event.event_data;
    qevent.user_data = event.user_data;

    emit QTMS::QTMSVolumeEffect::EffectsEvent(static_cast<QTMSEffect&> (*this), qevent);
}

// End of file
