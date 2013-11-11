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

#include <tmsringtone.h>
#include <qtmseffect.h>
#include "tmsutility.h"
#include "qtmsringtoneimpl.h"

using namespace QTMS;
using namespace TMS;

QTMSRingToneImpl::QTMSRingToneImpl()
{
}

QTMSRingToneImpl::~QTMSRingToneImpl()
{
    RemoveObserver(*this);
}

gint QTMSRingToneImpl::Create(QTMSRingTone*& qrt, TMS::TMSRingTone*& tmsrt)
{
    gint ret(QTMS_RESULT_INSUFFICIENT_MEMORY);
    QTMSRingToneImpl* self = new QTMSRingToneImpl();
    if (self) {
        ret = self->PostConstruct();
        if (ret != QTMS_RESULT_SUCCESS) {
            delete self;
            self = NULL;
        }
        self->iRingTone = tmsrt;
        ret = self->AddObserver(*self, NULL);
    }
    qrt = self;
    return ret;
}

gint QTMSRingToneImpl::PostConstruct()
{
    gint ret(QTMS_RESULT_SUCCESS);
    return ret;
}

gint QTMSRingToneImpl::AddObserver(TMS::TMSRingToneObserver& obsrvr, gpointer user_data)
{
    gint ret(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iRingTone) {
        ret = iRingTone->AddObserver(obsrvr, user_data);
    }
    return ret;
}

gint QTMSRingToneImpl::RemoveObserver(TMS::TMSRingToneObserver& obsrvr)
{
    gint ret(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iRingTone) {
        ret = iRingTone->RemoveObserver(obsrvr);
    }
    return ret;
}

void QTMSRingToneImpl::RingtoneEvent(const TMS::TMSRingTone& /*rt*/, TMS::TMSSignalEvent event)
{
    QTMSSignalEvent qevent;
    qevent.type = event.type;
    qevent.reason = event.reason;
    qevent.curr_state = event.curr_state;
    qevent.prev_state = event.prev_state;
    qevent.event_data = event.event_data;
    qevent.user_data = event.user_data;

    emit QTMS::QTMSRingTone::RingtoneEvent(static_cast<QTMSRingTone&> (*this), qevent);
}

// End of file
