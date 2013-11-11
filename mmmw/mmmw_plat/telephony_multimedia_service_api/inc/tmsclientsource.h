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

#ifndef TMS_CLIENT_SOURCE_H
#define TMS_CLIENT_SOURCE_H

#include <tms.h>
#include <tmssource.h>

namespace TMS {

// FORWARD DECLARATIONS
class TMSClientSourceBody;
class TMSClientSourceObserver;
class TMSBuffer;

/**
 * TMSClientSource class
 *
 * This class instantiates TMS client source interface. It serves as a data
 * source to the TMS, through which the client submits data buffers to the
 * TMS call server for playback on the audio and/or video output device.
 *
 * @lib tmsapi.lib
 *
 */
class TMSClientSource : public TMSSource
    {
public:
    /**
     * Destructor
     */
    IMPORT_C virtual ~TMSClientSource();

    /**
     * Add client as an observer for this source object.
     *
     * This function can be called at any time. It is recommended adding the
     * observer before calling any functions on the source. Otherwise, the
     * observer may miss a callback.
     *
     * A reference to the obsrvr is stored in the source. The QueueEvent
     * function will be called when a source event occurs. When the source
     * object is deleted, all TMSClientSourceObserver references are
     * automatically released. The ownership of the obsrvr is still maintained
     * by the client. Should the client need to delete the obsrvr before
     * deleting the source, it should remove all the obsrvr reference from the
     * source by calling RemoveObserver() function.
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
    IMPORT_C gint AddObserver(TMSClientSourceObserver& obsrvr,
            gpointer user_data);

    /**
     * Remove a client observer from this Source.
     *
     * This function can be called at any time.
     *
     * @param  obsrvr
     *      The listener to remove.
     *
     * @return
     *      TMS_RESULT_SUCCESS if the obsrvr is removed successfully from list.
     *      TMS_RESULT_DOES_NOT_EXIST if obsrvr is not already in the list.
     *
     */
    IMPORT_C gint RemoveObserver(TMSClientSourceObserver& obsrvr);

    /**
     * PULL MODE ONLY
     *
     * Tell the source that the buffer passed in has been filled in with data
     * by the client and it is ready to be sent do the TMS for processing.
     *
     * This function must be called in response to the callback from the
     * source observer TMSClientSourceObserver::FillBuffer.
     *
     * @param  buffer
     *      The buffer which has been processed by the client (filled in
     *      with data).
     *
     * @return
     *      Status of the operation.
     *
     */
    IMPORT_C gint BufferFilled(TMSBuffer& buffer);

    /**
     * In the PUSH MODE, tell the source that the supplied buffer filled in
     * with data should be queued before being sent for processing by the
     * server. All ProcessBuffer requests will be queued until client calls
     * Flush(). After all queued buffers are pushed to the server and
     * processed, the TMS will respond by issuing
     * TMSClientSourceObserver::BufferProcessed
     * callback event.
     *
     * @param  buffer
     *      The buffer with audio data supplied by the client for playback.
     *
     */
    IMPORT_C gint ProcessBuffer(TMSBuffer* buffer);

    /**
     * PUSH MODE ONLY
     *
     * Tell the TMS framework to queue ProcessBuffer events. To request the
     * TMS push mode operation, the SetEnqueueMode must be called with the
     * flag set TRUE. Buffer enqueuing continues until the client calls
     * Flush(). At this point all queued data (buffers) are sent over to the
     * TMS for processing and the enqueuing mode is turned OFF. To turn it
     * ON again, the client has to call SetEnqueueMode with TRUE parameter.
     *
     * Has no effect in the PULL mode.
     *
     * @param  enable
     *      Toggles buffer enqueuing ON and OFF.
     *
     * @return
     *      TMS_RESULT_SUCCESS if the operation was successful otherwise
     *      system error.
     *
     */
    IMPORT_C gint SetEnqueueMode(const gboolean enable);

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
    IMPORT_C gint GetEnqueueMode(gboolean& enable);

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
     *      TMS_RESULT_SUCCESS if the operation was successful otherwise
     *      system error.
     *
     */
    IMPORT_C gint Flush();

    /**
     * Return the source type.
     *
     * This function can be called at any time.
     *
     * @param  TMSSourceType&
     *      The type of the source object. The supported source types are
     *      as follows:
     *          TMS_SOURCE_CLIENT
     *          TMS_SOURCE_MODEM
     *          TMS_STREAM_MIC
     *
     * @return
     *      TMS_RESULT_SUCCESS if the operation was successful otherwise
     *      system error.
     *
     */
    IMPORT_C virtual gint GetType(TMSSourceType& sourcetype);

protected:
    /**
     * Constructor
     */
    IMPORT_C TMSClientSource();

protected:
    TMSClientSourceBody *iBody;
    };

} //namespace TMS

#endif //TMS_CLIENT_SOURCE_H

// End of file

