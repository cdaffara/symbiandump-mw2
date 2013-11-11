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
#include "tmsutility.h"
#include "qtmsg729impl.h"

using namespace QTMS;
using namespace TMS;

QTMSG729FormatImpl::QTMSG729FormatImpl()
{
}

QTMSG729FormatImpl::~QTMSG729FormatImpl()
{
    TRACE_PRN_FN_ENT;
    TRACE_PRN_FN_EXT;
}

gint QTMSG729FormatImpl::Create(QTMSFormat*& qformat, TMS::TMSFormat*& tmsformat)
{
    gint ret(QTMS_RESULT_INSUFFICIENT_MEMORY);
    QTMSG729FormatImpl* self = new QTMSG729FormatImpl();
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

gint QTMSG729FormatImpl::PostConstruct()
{
    gint ret(QTMS_RESULT_SUCCESS);
    return ret;
}

gint QTMSG729FormatImpl::GetFormat(TMS::TMSFormat*& fmt)
{
    gint ret(QTMS_RESULT_UNINITIALIZED_OBJECT);

    if (iFormat) {
        fmt = iFormat;
        ret = QTMS_RESULT_SUCCESS;
    }
    return ret;
}

gint QTMSG729FormatImpl::GetType(QTMSFormatType& fmttype)
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iFormat) {
        status = iFormat->GetType(fmttype);
    }
    return status;
}

// End of file
