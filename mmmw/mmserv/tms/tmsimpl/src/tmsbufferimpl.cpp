/*
 * Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "tmsutility.h"
#include "tmsmembuffer.h"
#include "tmsbufferimpl.h"

using namespace TMS;

EXPORT_C gint TMSBufferImpl::Create(TMSBufferType buffertype, guint size,
        TMSBuffer*& tmsbuffer)
    {
    gint ret(TMS_RESULT_INSUFFICIENT_MEMORY);
    TRACE_PRN_FN_ENT;
    switch (buffertype)
        {
        case TMS_BUFFER_MEMORY:
            ret = TMSMemBuffer::Create(size, tmsbuffer);
            break;
        default:
            ret = TMS_RESULT_BUFFER_TYPE_NOT_SUPPORTED;
            break;
        }

    TRACE_PRN_FN_EXT;
    return ret;
    }

EXPORT_C gint TMSBufferImpl::Delete(TMSBuffer*& tmsbuffer)
    {
    gint ret(TMS_RESULT_INVALID_ARGUMENT);
    TRACE_PRN_FN_ENT;
    TMSBufferType buffertype;
    ret = tmsbuffer->GetType(buffertype);
    switch (buffertype)
        {
        case TMS_BUFFER_MEMORY:
            delete (static_cast<TMSMemBuffer*>(tmsbuffer));
            tmsbuffer = NULL;
            ret = TMS_RESULT_SUCCESS;
            break;
        default:
            ret = TMS_RESULT_BUFFER_TYPE_NOT_SUPPORTED;
            break;
        }

    TRACE_PRN_FN_EXT;
    return ret;
    }

// End of file
