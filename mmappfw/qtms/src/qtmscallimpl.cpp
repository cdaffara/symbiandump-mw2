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

#include <tmscall.h>
#include <qtms.h>
#include "qtmscallimpl.h"

using namespace QTMS;
using namespace TMS;

QTMSCallImpl::QTMSCallImpl()
{
}

QTMSCallImpl::~QTMSCallImpl()
{
}

gint QTMSCallImpl::Create(QTMSCall*& qtmscall, TMS::TMSCall*& tmscall)
{
    gint ret(QTMS_RESULT_INSUFFICIENT_MEMORY);
    QTMSCallImpl* self = new QTMSCallImpl();
    if (self) {
        ret = self->PostConstruct();
        if (ret != QTMS_RESULT_SUCCESS) {
            delete self;
            self = NULL;
        }
        self->iTmsCall = tmscall;
    }
    qtmscall = self;
    return ret;
}

gint QTMSCallImpl::PostConstruct()
    {
    gint ret(QTMS_RESULT_SUCCESS);
    return ret;
    }

// End of file
