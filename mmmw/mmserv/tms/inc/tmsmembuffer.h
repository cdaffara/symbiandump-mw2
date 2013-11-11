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

#ifndef TMS_MEM_BUFFER_H
#define TMS_MEM_BUFFER_H

#include <tmsbuffer.h>

namespace TMS {

// TMSMemBuffer class
class TMSMemBuffer : public TMSBuffer
    {
public:
    virtual ~TMSMemBuffer();

    /**
     * Factory methods
     */
    static gint Create(guint size, TMSBuffer*& tmsbuffer);
    static gint Create(guint size, guint8* dataptr, TMSBuffer*& tmsbuffer);

    /*
     * Returns buffer type.
     *
     * @param buftype buffer type
     *
     */
    virtual gint GetType(TMSBufferType& buftype);

    /**
     * Gets the timestamp on the Buffer so that the framework can
     * determine the time at which this buffer has to be rendered
     * by the output device sink.
     *
     * @param ts buffer timestamp in microseconds
     *
     */
    virtual gint GetTimeStamp(guint64& ts);

    /**
     * Sets the timestamp on the Buffer so that the framework can
     * determine the time at which this buffer has to be rendered
     * by the output device sink.
     *
     * @param ts timestamp in milliseconds
     *
     */
    virtual gint SetTimeStamp(const guint64 ts);

    /**
     * Gets the size of data in the buffer specified by the client.
     *
     * @param size size of data in bytes
     *
     */
    virtual gint GetDataSize(guint& size);

    /**
     * Sets the size of data in the buffer after the client fill it.
     *
     * @param size size of data in bytes
     *
     */
    virtual gint SetDataSize(const guint size);

    /**
     * Gets the pointer to the memory location associated with this
     * buffer where the data is stored.
     *
     * @param bufptr ptr to the data stored in the buffer.
     *
     */
    virtual gint GetDataPtr(guint8*& bufptr);

private:
    TMSMemBuffer();
    gint PostConstruct(guint size);

private:
    guint iBufferSize;
    guint64 iTimeStamp;
    guint8* iDataPtr;
    gboolean iOwnsBuffer;
    };

} //namespace TMS

#endif //TMS_MEM_BUFFER_H

