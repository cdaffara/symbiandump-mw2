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
#include <tmsinbandtone.h>
#include "tmsutility.h"
#include "qtmsinbandtoneimpl.h"

using namespace QTMS;
using namespace TMS;

QTMSInbandToneImpl::QTMSInbandToneImpl()
{
}

QTMSInbandToneImpl::~QTMSInbandToneImpl()
{
    RemoveObserver(*this);
}

gint QTMSInbandToneImpl::Create(QTMSInbandTone*& qinband, TMS::TMSInbandTone*& tmsinband)
{
    gint ret(QTMS_RESULT_INSUFFICIENT_MEMORY);
    QTMSInbandToneImpl* self = new QTMSInbandToneImpl();
    if (self) {
        ret = self->PostConstruct();
        if (ret != QTMS_RESULT_SUCCESS) {
            delete self;
            self = NULL;
        }
        self->iInbandTone = tmsinband;
        self->AddObserver(*self, NULL);
    }
    qinband = self;
    return ret;
}

gint QTMSInbandToneImpl::PostConstruct()
{
    gint ret(QTMS_RESULT_SUCCESS);
    return ret;
}

gint QTMSInbandToneImpl::AddObserver(TMS::TMSInbandToneObserver& obsrvr, gpointer user_data)
{
    gint ret(QTMS_RESULT_SUCCESS);

    if (iInbandTone) {
        ret = iInbandTone->AddObserver(obsrvr, user_data);
    }
    return ret;
}

gint QTMSInbandToneImpl::RemoveObserver(TMS::TMSInbandToneObserver& obsrvr)
{
    gint ret(QTMS_RESULT_SUCCESS);

    if (iInbandTone) {
        ret = iInbandTone->RemoveObserver(obsrvr);
    }
    return ret;
}

void QTMSInbandToneImpl::InbandToneEvent(const TMS::TMSInbandTone& /*inbandtone*/,
    TMS::TMSSignalEvent event)
{
    QTMSSignalEvent qevent;
    qevent.type = event.type;
    qevent.reason = event.reason;
    qevent.curr_state = event.curr_state;
    qevent.prev_state = event.prev_state;
    qevent.event_data = event.event_data;
    qevent.user_data = event.user_data;

    emit QTMS::QTMSInbandTone::InbandToneEvent(static_cast<QTMSInbandTone&> (*this), qevent);
}

// End of file
