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
#include <tmsamrformat.h>
#include "tmsutility.h"
#include "qtmsamrimpl.h"

using namespace QTMS;
using namespace TMS;

QTMSAMRFormatImpl::QTMSAMRFormatImpl()
{
}

QTMSAMRFormatImpl::~QTMSAMRFormatImpl()
{
    TRACE_PRN_FN_ENT;
    TRACE_PRN_FN_EXT;
}

gint QTMSAMRFormatImpl::Create(QTMSFormat*& qformat, TMS::TMSFormat*& tmsformat)
{
    gint ret(TMS_RESULT_INSUFFICIENT_MEMORY);
    QTMSAMRFormatImpl* self = new QTMSAMRFormatImpl();
    if (self) {
        ret = self->PostConstruct();
        if (ret != TMS_RESULT_SUCCESS) {
            delete self;
            self = NULL;
        }
        self->iFormat = tmsformat;
    }
    qformat = self;
    return ret;
}

gint QTMSAMRFormatImpl::PostConstruct()
{
    gint ret(TMS_RESULT_SUCCESS);
    return ret;
}

gint QTMSAMRFormatImpl::GetFormat(TMS::TMSFormat*& fmt)
{
    gint ret(QTMS_RESULT_UNINITIALIZED_OBJECT);

    if (iFormat) {
        fmt = iFormat;
        ret = QTMS_RESULT_SUCCESS;
    }
    return ret;
}

gint QTMSAMRFormatImpl::GetType(QTMSFormatType& fmttype)
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iFormat) {
        status = iFormat->GetType(fmttype);
    }
    return status;
}

// End of file
