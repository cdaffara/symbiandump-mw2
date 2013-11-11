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
#include <tmsglobalgaineffect.h>
#include "tmsutility.h"
#include "qtmsglobalgaineffectimpl.h"

using namespace QTMS;
using namespace TMS;

QTMSGlobalGainEffectImpl::QTMSGlobalGainEffectImpl()
{
}

QTMSGlobalGainEffectImpl::~QTMSGlobalGainEffectImpl()
{
    RemoveObserver(*this);
}

gint QTMSGlobalGainEffectImpl::Create(QTMSEffect*& qglobalgain, TMS::TMSEffect*& tmsglobalgain)
{
    gint ret(QTMS_RESULT_INSUFFICIENT_MEMORY);
    QTMSGlobalGainEffectImpl* self = new QTMSGlobalGainEffectImpl();
    if (self) {
        ret = self->PostConstruct();
        if (ret != QTMS_RESULT_SUCCESS) {
            delete self;
            self = NULL;
        }
        self->iEffect = tmsglobalgain;
        ret = self->AddObserver(*self, NULL);
    }
    qglobalgain = self;
    return ret;
}

gint QTMSGlobalGainEffectImpl::PostConstruct()
{
    gint ret(QTMS_RESULT_SUCCESS);
    return ret;
}

gint QTMSGlobalGainEffectImpl::AddObserver(TMS::TMSEffectObserver& obsrvr, gpointer user_data)
{
    gint ret(QTMS_RESULT_SUCCESS);

    if (iEffect) {
        ret = static_cast<TMSGlobalGainEffect*> (iEffect)->AddObserver(obsrvr, user_data);
    }
    return ret;
}

gint QTMSGlobalGainEffectImpl::RemoveObserver(TMS::TMSEffectObserver& obsrvr)
{
    gint ret(QTMS_RESULT_SUCCESS);

    if (iEffect) {
        ret = static_cast<TMSGlobalGainEffect*> (iEffect)->RemoveObserver(obsrvr);
    }
    return ret;
}

void QTMSGlobalGainEffectImpl::EffectsEvent(const TMS::TMSEffect& /*tmseffect*/,
    TMS::TMSSignalEvent event)
{
    QTMSSignalEvent qevent;

    qevent.type = event.type;
    qevent.reason = event.reason;
    qevent.curr_state = event.curr_state;
    qevent.prev_state = event.prev_state;
    qevent.event_data = event.event_data;
    qevent.user_data = event.user_data;

    emit QTMS::QTMSGlobalGainEffect::EffectsEvent(static_cast<QTMSEffect&> (*this), qevent);
}

// End of file
