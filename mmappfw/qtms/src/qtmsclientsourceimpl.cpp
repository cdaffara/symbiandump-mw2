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
#include <qtmsbuffer.h>
#include <tmsbuffer.h>
#include <tmsclientsource.h>
#include "qtmsclientsourceimpl.h"

using namespace QTMS;
using namespace TMS;

QTMSClientSourceImpl::QTMSClientSourceImpl()
{
}

QTMSClientSourceImpl::~QTMSClientSourceImpl()
{
    RemoveObserver(*this);
}

gint QTMSClientSourceImpl::Create(QTMSSource*& qsource, TMSSource*& tmssource)
{
    gint ret(QTMS_RESULT_INSUFFICIENT_MEMORY);
    QTMSClientSourceImpl* self = new QTMSClientSourceImpl();
    if (self) {
        ret = self->PostConstruct();
        if (ret != QTMS_RESULT_SUCCESS) {
            delete self;
            self = NULL;
        }
        self->iSource = tmssource;
        self->AddObserver(*self, NULL);
    }
    qsource = self;
    return ret;
}

gint QTMSClientSourceImpl::PostConstruct()
{
    gint ret(QTMS_RESULT_SUCCESS);
    return ret;
}

gint QTMSClientSourceImpl::AddObserver(TMS::TMSClientSourceObserver& obsrvr, gpointer user_data)
{
    gint ret(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iSource) {
        ret = static_cast<TMSClientSource*> (iSource)->AddObserver(obsrvr, user_data);
    }
    return ret;
}

gint QTMSClientSourceImpl::RemoveObserver(TMS::TMSClientSourceObserver& obsrvr)
{
    gint ret(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iSource) {
        ret = static_cast<TMSClientSource*> (iSource)->RemoveObserver(obsrvr);
    }
    return ret;
}

gint QTMSClientSourceImpl::GetSource(TMS::TMSSource*& tmssource)
{
    gint ret(QTMS_RESULT_UNINITIALIZED_OBJECT);

    if (iSource) {
        tmssource = iSource;
        ret = QTMS_RESULT_SUCCESS;
    }
    return ret;
}

void QTMSClientSourceImpl::FillBuffer(TMS::TMSBuffer& buffer)
{
    emit QTMSClientSource::FillBuffer(reinterpret_cast<QTMSBuffer&> (buffer));
}

void QTMSClientSourceImpl::BufferProcessed(const TMS::TMSBuffer* buffer, gint reason)
{
    TMSBuffer* buf(const_cast<TMSBuffer*> (buffer));
    emit QTMSClientSource::BufferProcessed(reinterpret_cast<QTMSBuffer*> (buf), reason);
}

// End of file
