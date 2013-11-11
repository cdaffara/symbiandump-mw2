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

#ifndef QTMS_CLIENT_SINK_H
#define QTMS_CLIENT_SINK_H

#include <QObject>
#include <qtms.h>
#include <qtmsbuffer.h>
#include <qtmssink.h>
#include <qglobal.h>
#include "qtmswrapperexport.h"

// FORWARD DECLARATIONS
namespace TMS
{
class TMSSink;
}

namespace QTMS
{

/**
 * QTMSClientSink class
 *
 * This class provides QTMS client sink interface. It serves as a data sink
 * to the QTMS, which supplies the client with buffers recorded by the a/v
 * recording device. The constant flow of data between the a/v recorder
 * device and the client is accomplished by sending BufferProcessed() events
 * in response to TMSClientSinkObserver::ProcessBuffer sink observer callbacks.
 * This indicates to the QTMS that the supplied buffer has been consumed by the
 * client and it is ready to receive more recorded data.
 *
 * @lib QTMSapi.lib
 *
 */
class QTMS_WRAPPER_DLL_EXPORT QTMSClientSink : public QObject,
                                               public QTMSSink
{
    Q_OBJECT
public:
    /**
     * Destructor
     */
    virtual ~QTMSClientSink();

    /**
     * Tell the sink that the buffer passed in has been processed by
     * the client and is ready to be reused by the Sink.
     *
     * This function must be called in response to the callback from the
     * sink observer QTMSClientSinkObserver::ProcessBuffer().
     *
     * @param  buffer
     *      The buffer which has been processed by the client (filled in
     *      with data).
     *
     * @return
     *      Status of the operation.
     *
     */
    gint BufferProcessed(QTMSBuffer* buffer);

    /**
     * Return sink type.
     *
     * This function can be called at any time.
     *
     * @param  sinktype
     *      The type of sink object (QTMS_SINK_CLIENT).
     *
     * @return
     *      QTMS_RESULT_SUCCESS if the operation was successful.
     *
     */
    virtual gint GetType(QTMSSinkType& sinktype);

    Q_SIGNALS:
    void ProcessBuffer(const QTMSBuffer* buffer);

protected:
    /**
     * Constructor
     */
    QTMSClientSink();

protected:
    TMS::TMSSink* iSink;
};

} //namespace QTMS

#endif //QTMS_CLIENT_SINK_H
// End of file

