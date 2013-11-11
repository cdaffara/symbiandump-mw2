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
#include <qtmseffect.h>
#include <tmsstream.h>
#include "tmsutility.h"
#include "qtmsstreamimpl.h"

using namespace QTMS;
using namespace TMS;

QTMSStreamImpl::QTMSStreamImpl()
{
}

QTMSStreamImpl::~QTMSStreamImpl()
{
    RemoveObserver(*this);
}

gint QTMSStreamImpl::Create(QTMSCallType callType, QTMSStreamType stype, QTMSStream*& qstrm,
    TMS::TMSStream*& tmsstrm)
{
    gint ret(QTMS_RESULT_INSUFFICIENT_MEMORY);
    QTMSStreamImpl* self = new QTMSStreamImpl();
    if (self) {
        ret = self->PostConstruct(callType, stype, *self);
        if (ret != QTMS_RESULT_SUCCESS) {
            delete self;
            self = NULL;
        }
        self->iStream = tmsstrm;
        self->AddObserver(*self, NULL);
    }
    qstrm = self;
    return ret;
}

gint QTMSStreamImpl::PostConstruct(QTMSCallType /*callType*/, QTMSStreamType /*stype*/, QTMSStream& /*parent*/)
{
    gint status(QTMS_RESULT_SUCCESS);
    return status;
}

gint QTMSStreamImpl::AddObserver(TMS::TMSStreamObserver& obsrvr, gpointer user_data)
{
    gint ret(QTMS_RESULT_SUCCESS);

    if (iStream) {
        iStream->AddObserver(obsrvr, user_data);
    }

    return ret;
}

gint QTMSStreamImpl::RemoveObserver(TMS::TMSStreamObserver& obsrvr)
{
    gint ret(QTMS_RESULT_SUCCESS);

    if (iStream) {
        ret = iStream->RemoveObserver(obsrvr);
    }

    return ret;
}

void QTMSStreamImpl::TMSStreamEvent(const TMS::TMSStream& /*stream*/, TMS::TMSSignalEvent event)
{
    QTMSSignalEvent qevent;

    qevent.type = event.type;
    qevent.reason = event.reason;
    qevent.curr_state = event.curr_state;
    qevent.prev_state = event.prev_state;
    qevent.event_data = event.event_data;
    qevent.user_data = event.user_data;

    emit QTMS::QTMSStream::TMSStreamEvent(static_cast<QTMSStream&> (*this), qevent);
}
