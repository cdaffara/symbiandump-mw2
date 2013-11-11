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
#include <tmsmodemsink.h>
#include "qtmsmodemsinkimpl.h"

using namespace QTMS;
using namespace TMS;

QTMSModemSinkImpl::QTMSModemSinkImpl()
{
}

QTMSModemSinkImpl::~QTMSModemSinkImpl()
{
}

gint QTMSModemSinkImpl::Create(QTMSSink*& qsink, TMS::TMSSink*& tmssink)
{
    gint ret(QTMS_RESULT_INSUFFICIENT_MEMORY);
    QTMSModemSinkImpl* self = new QTMSModemSinkImpl();
    if (self) {
        ret = self->PostConstruct();
        if (ret != QTMS_RESULT_SUCCESS) {
            delete self;
            self = NULL;
        }
        self->iSink = tmssink;
    }
    qsink = self;
    return ret;
}

gint QTMSModemSinkImpl::PostConstruct()
{
    gint ret(QTMS_RESULT_SUCCESS);
    return ret;
}

gint QTMSModemSinkImpl::GetSink(TMS::TMSSink*& tmssink)
{
    gint ret(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iSink) {
        tmssink = iSink;
        ret = QTMS_RESULT_SUCCESS;
    }
    return ret;
}

// End of file
