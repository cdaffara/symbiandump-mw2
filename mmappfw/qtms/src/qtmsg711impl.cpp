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
#include <tmsg711format.h>
#include "qtmsg711impl.h"

using namespace QTMS;
using namespace TMS;

QTMSG711FormatImpl::QTMSG711FormatImpl()
{
}

QTMSG711FormatImpl::~QTMSG711FormatImpl()
{
}

gint QTMSG711FormatImpl::Create(QTMSFormat*& qformat, TMS::TMSFormat*& tmsformat)
{
    gint ret(QTMS_RESULT_INSUFFICIENT_MEMORY);
    QTMSG711FormatImpl* self = new QTMSG711FormatImpl();
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

gint QTMSG711FormatImpl::PostConstruct()
{
    gint ret(QTMS_RESULT_SUCCESS);
    return ret;
}

gint QTMSG711FormatImpl::GetFormat(TMS::TMSFormat*& fmt)
{
    gint ret(QTMS_RESULT_UNINITIALIZED_OBJECT);

    if (iFormat) {
        fmt = iFormat;
        ret = QTMS_RESULT_SUCCESS;
    }
    return ret;
}

gint QTMSG711FormatImpl::GetType(QTMSFormatType& fmttype)
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iFormat) {
        status = iFormat->GetType(fmttype);
    }
    return status;
}

// End of file
