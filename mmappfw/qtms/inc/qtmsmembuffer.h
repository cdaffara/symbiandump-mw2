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

#ifndef QTMS_MEM_BUFFER_H
#define QTMS_MEM_BUFFER_H

#include <qtmsbuffer.h>

// FORWARD DECLARATION
namespace TMS
{
class TMSBuffer;
}

namespace QTMS
{

// QTMSMemBuffer class
class QTMSMemBuffer: public QTMSBuffer
{
public:
    virtual ~QTMSMemBuffer();

    /**
     * Factory methods
     */
    static gint Create(QTMSBuffer*& buffer, TMS::TMSBuffer*& tmsbuffer);

    /**
     * Return buffer type.
     *
     * @param  buftype
     *      Type of the buffer object (QTMS_BUFFER_MEMORY).
     *
     * @return
     *      QTMS_RESULT_SUCCESS if the operation was successful.
     *
     */
    virtual gint GetType(QTMSBufferType& buftype);

    /**
     Gets the timestamp on the Buffer so that the framework can
     determine the time at which this buffer has to be rendered
     by the output device sink.

     @param ts
     timestamp in microseconds
     */
    virtual gint GetTimeStamp(guint64& ts);

    /**
     Sets the timestamp on the Buffer so that the framework can
     determine the time at which this buffer has to be rendered
     by the output device sink.

     @param ts
     timestamp in milliseconds
     */
    virtual gint SetTimeStamp(const guint64 ts);

    /**
     Gets the size of data in the buffer specified by the client.

     @param size
     size of data in bytes
     */
    virtual gint GetDataSize(guint& size);

    /**
     Sets the size of data in the buffer after the client
     fill it.

     @param size
     size of data in bytes
     */
    virtual gint SetDataSize(const guint size);

    /**
     Gets the pointer to the memory location associated with this
     buffer where the data is stored.

     @param bufptr
     ptr to the data stored in the buffer.
     */
    virtual gint GetDataPtr(guint8*& bufptr);

private:
    QTMSMemBuffer();
    gint PostConstruct(guint size, TMS::TMSBuffer*& tmsbuffer);

protected:
    TMS::TMSBuffer* iTmsBuffer;

private:
    guint iBufferSize;
    guint64 iTimeStamp;
    guint8* iDataPtr;
    gboolean iOwnsBuffer;
};

} //namespace QTMS

#endif //QTMS_MEM_BUFFER_H
