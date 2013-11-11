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

#ifndef QTMS_CLIENT_SOURCE_H
#define QTMS_CLIENT_SOURCE_H

#include <QObject>
#include <qtms.h>
#include <qtmsbuffer.h>
#include <qtmssource.h>
#include "qtmswrapperexport.h"

// FORWARD DECLARATIONS
namespace TMS
{
class TMSSource;
}

namespace QTMS
{

/**
 * QTMSClientSource class
 *
 * This class instantiates QTMS client source interface. It serves as a data
 * source to the QTMS, through which the client submits data buffers to the
 * QTMS call server for playback on the audio and/or video output device.
 *
 * @lib QTMSapi.lib
 *
 */
class QTMS_WRAPPER_DLL_EXPORT QTMSClientSource : public QObject,
                                                 public QTMSSource
{
    Q_OBJECT
public:
    /**
     * Destructor
     */
    virtual ~QTMSClientSource();

    /**
     * PULL MODE ONLY
     *
     * Tell the source that the buffer passed in has been filled in with data
     * by the client and it is ready to be sent do the QTMS for processing.
     *
     * This function must be called in response to the callback from the
     * source observer QTMSClientSourceObserver::FillBuffer.
     *
     * @param  buffer
     *      The buffer which has been processed by the client (filled in
     *      with data).
     *
     * @return
     *      Status of the operation.
     *
     */
    gint BufferFilled(QTMSBuffer& buffer);

    /**
     * In the PUSH MODE, tell the source that the supplied buffer filled in
     * with data should be queued before being sent for processing by the
     * server. All ProcessBuffer requests will be queued until client calls
     * Flush(). After all queued buffers are pushed to the server and
     * processed, the QTMS will respond by issuing
     * QTMSClientSourceObserver::BufferProcessed
     * callback event.
     *
     * @param  buffer
     *      The buffer with audio data supplied by the client for playback.
     *
     */
    gint ProcessBuffer(QTMSBuffer* buffer);

    /**
     * PUSH MODE ONLY
     *
     * Tell the QTMS framework to queue ProcessBuffer events. To request the
     * QTMS push mode operation, the SetEnqueueMode must be called with the
     * flag set TRUE. Buffer enqueuing continues until the client calls
     * Flush(). At this point all queued data (buffers) are sent over to the
     * QTMS for processing and the enqueuing mode is turned OFF. To turn it
     * ON again, the client has to call SetEnqueueMode with TRUE parameter.
     *
     * Has no effect in the PULL mode.
     *
     * @param  enable
     *      Toggles buffer enqueuing ON and OFF.
     *
     * @return
     *      QTMS_RESULT_SUCCESS if the operation was successful.
     *
     */
    gint SetEnqueueMode(const gboolean enable);

    /**
     * PUSH MODE ONLY
     *
     * Return current buffer enqueuing mode setting.
     * Has no effect in the PULL mode.
     *
     * @param  enable
     *      Current enqueuing mode.
     *
     */
    gint GetEnqueueMode(gboolean& enable);

    /**
     * PUSH MODE ONLY
     *
     * Push all queued buffers by ProcessBuffer to the server for processing.
     * After calling this function, the enqueuing mode is turned OFF. To set
     * it ON, the client has to call SetEnqueueMode again.
     *
     * Has no effect in the PULL mode.
     *
     * @return
     *      QTMS_RESULT_SUCCESS if the operation was successful.
     *
     */
    gint Flush();

    /**
     * Return the source type.
     *
     * This function can be called at any time.
     *
     * @param  QTMSSourceType&
     *      The type of the source object. The supported source types are
     *      as follows:
     *          QTMS_SOURCE_CLIENT
     *          QTMS_SOURCE_MODEM
     *          QTMS_SOURCE_MIC
     *
     * @return
     *      QTMS_RESULT_SUCCESS if the operation was successful.
     *
     */
    virtual gint GetType(QTMSSourceType& sourcetype);

    Q_SIGNALS:
    void FillBuffer(QTMSBuffer& buffer);
    void BufferProcessed(const QTMSBuffer* buffer, gint reason);

protected:
    /**
     * Constructor
     */
    QTMSClientSource();

protected:
    TMS::TMSSource* iSource;
};

} //namespace QTMS

#endif //QTMS_CLIENT_SOURCE_H
// End of file

