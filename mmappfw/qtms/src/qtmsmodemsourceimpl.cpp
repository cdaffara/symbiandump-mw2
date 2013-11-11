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
#include <tmsmodemsource.h>
#include "qtmsmodemsourceimpl.h"

using namespace QTMS;
using namespace TMS;

QTMSModemSourceImpl::QTMSModemSourceImpl()
{
}

QTMSModemSourceImpl::~QTMSModemSourceImpl()
{
}

gint QTMSModemSourceImpl::Create(QTMSSource*& qsource, TMS::TMSSource*& tmssource)
{
    gint ret(QTMS_RESULT_INSUFFICIENT_MEMORY);
    QTMSModemSourceImpl* self = new QTMSModemSourceImpl();
    if (self) {
        ret = self->PostConstruct();
        if (ret != QTMS_RESULT_SUCCESS) {
            delete self;
            self = NULL;
        }
        self->iSource = tmssource;
    }
    qsource = self;
    return ret;
}

gint QTMSModemSourceImpl::PostConstruct()
{
    gint ret(QTMS_RESULT_SUCCESS);
    return ret;
}

gint QTMSModemSourceImpl::GetSource(TMS::TMSSource*& tmssource)
{
    gint ret(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iSource) {
        tmssource = iSource;
        ret = QTMS_RESULT_SUCCESS;
    }
    return ret;
}

// End of file
