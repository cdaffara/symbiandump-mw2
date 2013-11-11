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
#include <tmsglobalvoleffect.h>
#include "tmsutility.h"
#include "qtmsglobalvoleffectimpl.h"

using namespace QTMS;
using namespace TMS;

QTMSGlobalVolEffectImpl::QTMSGlobalVolEffectImpl()
{
}

QTMSGlobalVolEffectImpl::~QTMSGlobalVolEffectImpl()
{
    RemoveObserver(*this);
}

gint QTMSGlobalVolEffectImpl::Create(QTMSEffect*& qglobalvol, TMS::TMSEffect*& tmsglobalvol)
{
    gint ret(QTMS_RESULT_INSUFFICIENT_MEMORY);
    QTMSGlobalVolEffectImpl* self = new QTMSGlobalVolEffectImpl();
    if (self) {
        ret = self->PostConstruct();
        if (ret != QTMS_RESULT_SUCCESS) {
            delete self;
            self = NULL;
        }
        self->iEffect = tmsglobalvol;
        ret = self->AddObserver(*self, NULL);
    }
    qglobalvol = self;
    return ret;
}

gint QTMSGlobalVolEffectImpl::PostConstruct()
{
    gint ret(QTMS_RESULT_SUCCESS);
    return ret;
}

gint QTMSGlobalVolEffectImpl::AddObserver(TMS::TMSEffectObserver& obsrvr, gpointer user_data)
{
    gint ret(QTMS_RESULT_SUCCESS);

    if (iEffect) {
        ret = static_cast<TMSGlobalVolEffect*> (iEffect)->AddObserver(obsrvr, user_data);
    }
    return ret;
}

gint QTMSGlobalVolEffectImpl::RemoveObserver(TMS::TMSEffectObserver& obsrvr)
{
    gint ret(QTMS_RESULT_SUCCESS);

    if (iEffect) {
        ret = static_cast<TMSGlobalVolEffect*> (iEffect)->RemoveObserver(obsrvr);
    }
    return ret;
}

void QTMSGlobalVolEffectImpl::EffectsEvent(const TMS::TMSEffect& /*tmseffect*/,
    TMS::TMSSignalEvent event)
{
    QTMSSignalEvent qevent;

    qevent.type = event.type;
    qevent.reason = event.reason;
    qevent.curr_state = event.curr_state;
    qevent.prev_state = event.prev_state;
    qevent.event_data = event.event_data;
    qevent.user_data = event.user_data;

    emit QTMS::QTMSGlobalVolEffect::EffectsEvent(static_cast<QTMSEffect&> (*this), qevent);
}

// End of file
