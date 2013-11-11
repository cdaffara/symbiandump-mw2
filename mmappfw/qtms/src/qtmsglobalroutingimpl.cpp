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
#include <tmsglobalrouting.h>
#include "tmsutility.h"
#include "qtmsglobalroutingimpl.h"

using namespace QTMS;
using namespace TMS;

QTMSGlobalRoutingImpl::QTMSGlobalRoutingImpl()
{
}

QTMSGlobalRoutingImpl::~QTMSGlobalRoutingImpl()
{
    RemoveObserver(*this);
}

gint QTMSGlobalRoutingImpl::Create(QTMSGlobalRouting*& qrouting, TMS::TMSGlobalRouting*& tmsrouting)
{
    gint ret(QTMS_RESULT_INSUFFICIENT_MEMORY);
    QTMSGlobalRoutingImpl* self = new QTMSGlobalRoutingImpl();
    if (self) {
        ret = self->PostConstruct();
        if (ret != QTMS_RESULT_SUCCESS) {
            delete self;
            self = NULL;
        }
        self->iRouting = tmsrouting;
        ret = self->AddObserver(*self, NULL);
    }
    qrouting = self;
    return ret;
}

gint QTMSGlobalRoutingImpl::PostConstruct()
{
    gint ret(QTMS_RESULT_SUCCESS);
    return ret;
}

gint QTMSGlobalRoutingImpl::AddObserver(TMS::TMSGlobalRoutingObserver& obsrvr, gpointer user_data)
{
    gint ret(QTMS_RESULT_SUCCESS);

    if (iRouting) {
        ret = iRouting->AddObserver(obsrvr, user_data);
    }
    return ret;
}

gint QTMSGlobalRoutingImpl::RemoveObserver(TMS::TMSGlobalRoutingObserver& obsrvr)
{
    gint ret(QTMS_RESULT_SUCCESS);

    if (iRouting) {
        ret = iRouting->RemoveObserver(obsrvr);
    }
    return ret;
}

void QTMSGlobalRoutingImpl::GlobalRoutingEvent(const TMS::TMSGlobalRouting& /*routing*/,
    TMS::TMSSignalEvent event, TMS::TMSAudioOutput output)
{
    QTMSSignalEvent qevent;
    QTMSAudioOutput qoutput;

    qevent.type = event.type;
    qevent.reason = event.reason;
    qevent.curr_state = event.curr_state;
    qevent.prev_state = event.prev_state;
    qevent.event_data = event.event_data;
    qevent.user_data = event.user_data;
    qoutput = output;

    emit QTMS::QTMSGlobalRouting::GlobalRoutingEvent(static_cast<QTMSGlobalRouting&> (*this),
        qevent, qoutput);
}

// End of file
