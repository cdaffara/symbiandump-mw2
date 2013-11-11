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

#include <stdlib.h>
#include "tmsutility.h"
#include "tmsmembuffer.h"

using namespace TMS;

TMSMemBuffer::TMSMemBuffer() :
    iOwnsBuffer(FALSE)
    {
    }

TMSMemBuffer::~TMSMemBuffer()
    {
    if (iOwnsBuffer)
        {
        free(iDataPtr);
        }
    }

gint TMSMemBuffer::Create(guint size, TMSBuffer*& bodyimpl)
    {
    gint ret(TMS_RESULT_INSUFFICIENT_MEMORY);
    TRACE_PRN_FN_ENT;
    TMSMemBuffer* self = new TMSMemBuffer();
    if (self)
        {
        ret = self->PostConstruct(size);
        if (ret != TMS_RESULT_SUCCESS)
            {
            delete self;
            self = NULL;
            }
        }
    bodyimpl = self;
    TRACE_PRN_FN_EXT;
    return ret;
    }

gint TMSMemBuffer::Create(guint size, guint8* dataptr, TMSBuffer*& tmsbuffer)
    {
    gint ret(TMS_RESULT_INSUFFICIENT_MEMORY);
    TRACE_PRN_FN_ENT;
    TMSMemBuffer* self = new TMSMemBuffer();
    if (self)
        {
        self->iDataPtr = dataptr;
        self->iOwnsBuffer = FALSE;
        self->iBufferSize = size;
        ret = TMS_RESULT_SUCCESS;
        }
    tmsbuffer = self;
    TRACE_PRN_FN_EXT;
    return ret;
    }

gint TMSMemBuffer::PostConstruct(guint size)
    {
    gint ret(TMS_RESULT_SUCCESS);
    TRACE_PRN_FN_ENT;
    iDataPtr = static_cast<guint8*>(malloc(size));
    if (!iDataPtr)
        {
        ret = TMS_RESULT_INSUFFICIENT_MEMORY;
        }
    iOwnsBuffer = TRUE;
    iBufferSize = size;
    TRACE_PRN_FN_EXT;
    return ret;
    }

gint TMSMemBuffer::GetType(TMSBufferType& buffertype)
    {
    gint ret(TMS_RESULT_SUCCESS);
    buffertype = TMS_BUFFER_MEMORY;
    return ret;
    }

/**
 * Gets the timestamp on the Buffer so that the framework can
 * determine the time at which this buffer has to be rendered
 * by the output device sink.
 *
 * @param ts timestamp in microseconds
 *
 */
gint TMSMemBuffer::GetTimeStamp(guint64& ts)
    {
    gint ret(TMS_RESULT_SUCCESS);
    ts = iTimeStamp;
    return ret;
    }

/**
 * Sets the timestamp on the Buffer so that the framework can
 * determine the time at which this buffer has to be rendered
 * by the output device sink.
 *
 * @param ts timestamp in milliseconds
 *
 */
gint TMSMemBuffer::SetTimeStamp(const guint64 ts)
    {
    gint ret(TMS_RESULT_SUCCESS);
    iTimeStamp = ts;
    return ret;
    }

/**
 * Gets the size of data in the buffer specified by the client.
 *
 * @param size size of data in bytes
 *
 */
gint TMSMemBuffer::GetDataSize(guint& size)
    {
    gint ret(TMS_RESULT_SUCCESS);
    size = iBufferSize;
    return ret;
    }

/**
 * Sets the size of data in the buffer after the client fill it.
 *
 * @param size size of data in bytes
 *
 */
gint TMSMemBuffer::SetDataSize(const guint size)
    {
    gint ret(TMS_RESULT_SUCCESS);
    iBufferSize = size; //TODO: should realloc when new size > old size (?)
    return ret;
    }

/**
 * Gets the pointer to the memory location associated with this
 * buffer where the data is stored.
 *
 * @param bufptr ptr to the data stored in the buffer.
 *
 */
gint TMSMemBuffer::GetDataPtr(guint8*& bufptr)
    {
    gint ret(TMS_RESULT_SUCCESS);
    bufptr = iDataPtr;
    return ret;
    }

