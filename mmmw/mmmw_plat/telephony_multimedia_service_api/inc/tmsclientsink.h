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

#ifndef TMS_CLIENT_SINK_H
#define TMS_CLIENT_SINK_H

#include <tms.h>
#include <tmssink.h>

namespace TMS {

// FORWARD DECLARATIONS
class TMSClientSinkBody;
class TMSClientSinkObserver;
class TMSBuffer;

/**
 * TMSClientSink class
 *
 * This class provides TMS client sink interface. It serves as a data sink
 * to the TMS, which supplies the client with buffers recorded by the a/v
 * recording device. The constant flow of data between the a/v recorder
 * device and the client is accomplished by sending BufferProcessed() events
 * in response to TMSClientSinkObserver::ProcessBuffer sink observer callbacks.
 * This indicates to the TMS that the supplied buffer has been consumed by the
 * client and it is ready to receive more recorded data.
 *
 * @lib tmsapi.lib
 *
 */
class TMSClientSink : public TMSSink
    {
public:
    /**
     * Destructor
     */
    IMPORT_C virtual ~TMSClientSink();

    /**
     * Add client as an observer for this sink object.
     *
     * This function can be called at any time. It is recommended adding the
     * observer before calling any functions on the sink. Otherwise, the
     * observer may miss a callback.
     *
     * A reference to the obsrvr is stored in the sink. The QueueEvent function
     * will be called when a sink event occurs. When the sink object is deleted,
     * all TMSClientSinkObserver references are automatically released. The
     * ownership of the obsrvr is still maintained by the client. Should the
     * client need to delete the obsrvr before deleting the sink, it should
     * remove all the obsrvr reference from the sink by calling RemoveObserver()
     * function.
     *
     * @param  obsrvr
     *      Listener to be added.
     *
     * @param  user_data
     *      Any user data passed to the function.
     *
     * @return
     *      TMS_RESULT_SUCCESS if the operation was successful otherwise
     *      system error.
     *      TMS_RESULT_ALREADY_EXIST if the obsrvr is already in the list.
     *
     */
    IMPORT_C gint AddObserver(TMSClientSinkObserver& obsrvr,
            gpointer user_data);

    /**
     * Remove client observer from this sink object.
     *
     * This function can be called at any time.
     *
     * @param  obsrvr
     *      The listener to be removed.
     *
     * @return
     *      TMS_RESULT_SUCCESS if the operation was successful otherwise
     *      system error.
     *      TMS_RESULT_DOES_NOT_EXIST if obsrvr is not already in the list.
     *
     */
    IMPORT_C gint RemoveObserver(TMSClientSinkObserver& obsrvr);

    /**
     * Tell the sink that the buffer passed in has been processed by
     * the client and is ready to be reused by the Sink.
     *
     * This function must be called in response to the callback from the
     * sink observer TMSClientSinkObserver::ProcessBuffer().
     *
     * @param  buffer
     *      The buffer which has been processed by the client (filled in
     *      with data).
     *
     * @return
     *      Status of the operation.
     *
     */
    IMPORT_C gint BufferProcessed(TMSBuffer* buffer);

    /**
     * Return sink type.
     *
     * This function can be called at any time.
     *
     * @param  sinktype
     *      The type of sink object (TMS_SINK_CLIENT).
     *
     * @return
     *      TMS_RESULT_SUCCESS if the operation was successful otherwise
     *      system error.
     *
     */
    IMPORT_C virtual gint GetType(TMSSinkType& sinktype);

protected:
    /**
     * Constructor
     */
    IMPORT_C TMSClientSink();

protected:
    TMSClientSinkBody *iBody;
    };

} //namespace TMS

#endif //TMS_CLIENT_SINK_H

// End of file

