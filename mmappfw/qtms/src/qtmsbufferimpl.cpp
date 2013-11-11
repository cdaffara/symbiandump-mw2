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
 * Description: Telephony Multimedia Service
 *
 */

#include <qtms.h>
#include "tmsutility.h"
#include "qtmsmembuffer.h"
#include "qtmsbufferimpl.h"

using namespace QTMS;

gint QTMSBufferImpl::Create(QTMSBufferType buffertype, QTMSBuffer*& buffer,
    TMS::TMSBuffer*& tmsbuffer)
{
    gint ret(QTMS_RESULT_INSUFFICIENT_MEMORY);
    TRACE_PRN_FN_ENT;
    switch (buffertype) {
    case QTMS_BUFFER_MEMORY:
        ret = QTMSMemBuffer::Create(buffer, tmsbuffer);
        break;
    default:
        ret = QTMS_RESULT_BUFFER_TYPE_NOT_SUPPORTED;
        break;
    }

    TRACE_PRN_FN_EXT;
    return ret;
}

gint QTMSBufferImpl::Delete(QTMSBuffer*& buffer)
{
    gint ret(QTMS_RESULT_INVALID_ARGUMENT);
    TRACE_PRN_FN_ENT;
    QTMSBufferType buffertype;
    ret = buffer->GetType(buffertype);
    switch (buffertype) {
    case QTMS_BUFFER_MEMORY:
        delete (static_cast<QTMSMemBuffer*> (buffer));
        buffer = NULL;
        ret = QTMS_RESULT_SUCCESS;
        break;
    default:
        ret = QTMS_RESULT_BUFFER_TYPE_NOT_SUPPORTED;
        break;
    }

    TRACE_PRN_FN_EXT;
    return ret;
}

// End of file
