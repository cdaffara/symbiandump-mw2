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

#include <tmsdtmf.h>
#include "tmsutility.h"
#include "qtmsdtmfimpl.h"

using namespace QTMS;
using namespace TMS;

QTMSDTMFImpl::QTMSDTMFImpl()
{
}

QTMSDTMFImpl::~QTMSDTMFImpl()
{
    RemoveObserver(*this);
}

gint QTMSDTMFImpl::Create(QTMSDTMF*& qdtmf, TMS::TMSDTMF*& tmsdtmf)
{
    gint ret(TMS_RESULT_INSUFFICIENT_MEMORY);
    QTMSDTMFImpl* self = new QTMSDTMFImpl();
    if (self) {
        ret = self->PostConstruct();
        if (ret != QTMS_RESULT_SUCCESS) {
            delete self;
            self = NULL;
        }
        self->iDtmf = tmsdtmf;
        ret = self->AddObserver(*self, NULL);
    }
    qdtmf = self;
    return ret;
}

gint QTMSDTMFImpl::PostConstruct()
{
    gint ret(QTMS_RESULT_SUCCESS);
    return ret;
}

gint QTMSDTMFImpl::AddObserver(TMS::TMSDTMFObserver& obsrvr, gpointer user_data)
{
    gint ret(QTMS_RESULT_UNINITIALIZED_OBJECT);

    if (iDtmf) {
        ret = iDtmf->AddObserver(obsrvr, user_data);
    }
    return ret;
}

gint QTMSDTMFImpl::RemoveObserver(TMS::TMSDTMFObserver& obsrvr)
{
    gint ret(QTMS_RESULT_UNINITIALIZED_OBJECT);

    if (iDtmf) {
        ret = iDtmf->RemoveObserver(obsrvr);
    }
    return ret;
}

void QTMSDTMFImpl::DTMFEvent(const TMS::TMSDTMF& /*dtmf*/, TMS::TMSSignalEvent event)
{
    QTMSSignalEvent qevent;

    qevent.type = event.type;
    qevent.reason = event.reason;
    qevent.curr_state = event.curr_state;
    qevent.prev_state = event.prev_state;
    qevent.event_data = event.event_data;
    qevent.user_data = event.user_data;

    emit QTMS::QTMSDTMF::DTMFEvent(static_cast<QTMSDTMF&> (*this), qevent);
}

// End of file
