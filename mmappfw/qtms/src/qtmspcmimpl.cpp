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
#include <tmspcmformat.h>
#include "tmsutility.h"
#include "qtmspcmimpl.h"

using namespace QTMS;
using namespace TMS;

QTMSPCMFormatImpl::QTMSPCMFormatImpl()
{
}

QTMSPCMFormatImpl::~QTMSPCMFormatImpl()
{
    TRACE_PRN_FN_ENT;
    TRACE_PRN_FN_EXT;
}

gint QTMSPCMFormatImpl::Create(QTMSFormat*& qformat, TMS::TMSFormat*& tmsformat)
{
    gint ret(QTMS_RESULT_INSUFFICIENT_MEMORY);
    QTMSPCMFormatImpl* self = new QTMSPCMFormatImpl();
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

gint QTMSPCMFormatImpl::PostConstruct()
{
    gint ret(QTMS_RESULT_SUCCESS);
    return ret;
}

gint QTMSPCMFormatImpl::GetFormat(TMS::TMSFormat*& fmt)
{
    gint ret(QTMS_RESULT_UNINITIALIZED_OBJECT);

    if (iFormat) {
        fmt = iFormat;
        ret = QTMS_RESULT_SUCCESS;
    }
    return ret;
}

gint QTMSPCMFormatImpl::GetType(QTMSFormatType& fmttype)
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iFormat) {
        status = iFormat->GetType(fmttype);
    }
    return status;
}

// End of file
