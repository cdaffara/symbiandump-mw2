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

#ifndef TMS_BUFFER_H
#define TMS_BUFFER_H

#include <tms.h>

namespace TMS {

/**
 * TMSBuffer class
 *
 * This interface provides data buffers to TMS clients. This class allows
 * clients to allocate data by themselves or request a specific size of the
 * buffer from the factory.
 *
 * @lib tmsapi.lib
 *
 */
class TMSBuffer
    {
public:
    /*
     * Destructor
     */
    virtual ~TMSBuffer() {}

    /**
     * Return buffer type.
     *
     * @param  buftype
     *      Type of the buffer object (TMS_BUFFER_MEMORY).
     *
     * @return
     *      TMS_RESULT_SUCCESS if the operation was successful otherwise
     *      system error.
     *
     */
    virtual gint GetType(TMSBufferType& buftype) = 0;

    /**
     * Returns time stamp of the buffer so that the framework can determine
     * the time at which this buffer has to be rendered by the output device
     * sink.
     *
     * @param  ts
     *      Time stamp in microseconds.
     *
     * @return
     *      TMS_RESULT_SUCCESS if the operation was successful otherwise
     *      system error.
     *
     */
    virtual gint GetTimeStamp(guint64& ts) = 0;

    /**
     * Sets the time stamp on the Buffer so that the framework can determine
     * the time at which this buffer has to be rendered by the output device
     * sink.
     *
     * @param  ts
     *      Time stamp in microseconds.
     *
     * @return
     *      TMS_RESULT_SUCCESS if the operation was successful otherwise
     *      system error.
     *
     */
    virtual gint SetTimeStamp(const guint64 ts) = 0;

    /**
     * Returns size of data in the buffer.
     *
     * @param  size
     *      Size of data in the buffer.
     *
     * @return
     *      TMS_RESULT_SUCCESS if the operation was successful otherwise
     *      system error.
     *
     */
    virtual gint GetDataSize(guint& size) = 0;

    /**
     * Set size of data in the buffer after filling in by the client.
     *
     * @param  size
     *      Size of data in the buffer.
     *
     * @return
     *      TMS_RESULT_SUCCESS if the operation was successful otherwise
     *      system error.
     *
     */
    virtual gint SetDataSize(const guint size) = 0;

    /**
     * Return pointer to the memory location of the data associated with this
     * buffer.
     *
     * @param  bufptr
     *      Pointer to the data stored in the buffer.
     *
     * @return
     *      TMS_RESULT_SUCCESS if the operation was successful otherwise
     *      system error.
     *
     */
    virtual gint GetDataPtr(guint8*& bufptr) = 0;
    };

} //namespace TMS

#endif //TMS_BUFFER_H
