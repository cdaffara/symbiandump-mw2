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
#include <tmsbuffer.h>
#include <tmsclientsink.h>
#include <qtmsbuffer.h>
#include "qtmsclientsinkimpl.h"

using namespace QTMS;
using namespace TMS;

QTMSClientSinkImpl::QTMSClientSinkImpl()
{
}

QTMSClientSinkImpl::~QTMSClientSinkImpl()
{
    RemoveObserver(*this);
}

gint QTMSClientSinkImpl::Create(QTMSSink*& qsink, TMS::TMSSink*& tmssink)
{
    gint ret(QTMS_RESULT_INSUFFICIENT_MEMORY);
    QTMSClientSinkImpl* self = new QTMSClientSinkImpl();
    if (self) {
        ret = self->PostConstruct();
        if (ret != QTMS_RESULT_SUCCESS) {
            delete self;
            self = NULL;
        }
        self->iSink = tmssink;
        self->AddObserver(*self, NULL);
    }
    qsink = self;
    return ret;
}

gint QTMSClientSinkImpl::PostConstruct()
{
    gint ret(QTMS_RESULT_SUCCESS);
    return ret;
}

gint QTMSClientSinkImpl::AddObserver(TMS::TMSClientSinkObserver& obsrvr, gpointer user_data)
{
    gint ret(QTMS_RESULT_SUCCESS);
    if (iSink) {
        ret = static_cast<TMSClientSink*> (iSink)->AddObserver(obsrvr, user_data);
    }
    return ret;
}

gint QTMSClientSinkImpl::RemoveObserver(TMS::TMSClientSinkObserver& obsrvr)
{
    gint ret(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iSink) {
        ret = static_cast<TMSClientSink*> (iSink)->RemoveObserver(obsrvr);
    }
    return ret;
}

gint QTMSClientSinkImpl::GetSink(TMS::TMSSink*& tmssink)
{
    gint ret(QTMS_RESULT_UNINITIALIZED_OBJECT);

    if (iSink) {
        tmssink = iSink;
        ret = QTMS_RESULT_SUCCESS;
    }
    return ret;
}

void QTMSClientSinkImpl::ProcessBuffer(const TMS::TMSBuffer* buffer)
{
    TMSBuffer* buf(const_cast<TMSBuffer*> (buffer));
    QTMSBuffer* qtbuf(reinterpret_cast<QTMSBuffer*> (buf));
    emit QTMSClientSink::ProcessBuffer(qtbuf);
}

// End of file
