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
#include <tmsilbcformat.h>
#include "qtmsilbcimpl.h"

using namespace QTMS;
using namespace TMS;

QTMSILBCFormatImpl::QTMSILBCFormatImpl()
{
}

QTMSILBCFormatImpl::~QTMSILBCFormatImpl()
{
}

gint QTMSILBCFormatImpl::Create(QTMSFormat*& qformat, TMS::TMSFormat*& tmsformat)
{
    gint ret(QTMS_RESULT_INSUFFICIENT_MEMORY);
    QTMSILBCFormatImpl* self = new QTMSILBCFormatImpl();
    if (self) {
        ret = self->PostConstruct();
        if (ret != QTMS_RESULT_SUCCESS) {
            delete self;
            self = NULL;
        }
        self->iFormat = tmsformat;
    }
    qformat = self;
    return ret;
}

gint QTMSILBCFormatImpl::PostConstruct()
{
    gint ret(QTMS_RESULT_SUCCESS);
    return ret;
}

gint QTMSILBCFormatImpl::GetFormat(TMS::TMSFormat*& fmt)
{
    gint ret(QTMS_RESULT_UNINITIALIZED_OBJECT);

    if (iFormat) {
        fmt = iFormat;
        ret = QTMS_RESULT_SUCCESS;
    }
    return ret;
}

gint QTMSILBCFormatImpl::GetType(QTMSFormatType& fmttype)
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iFormat) {
        status = iFormat->GetType(fmttype);
    }
    return status;
}

// End of file
