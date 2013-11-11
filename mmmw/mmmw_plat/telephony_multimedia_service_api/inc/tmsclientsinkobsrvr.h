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
 * Description: This class provides callback mechanism for TMSClientSink
 * listeners.
 *
 */

#ifndef TMS_CLIENT_SINK_OBSRVR_H
#define TMS_CLIENT_SINK_OBSRVR_H

// Include files

namespace TMS {

// FORWARD DECLARATIONS
class TMSBuffer;

/**
 * TMSClientSinkObserver class
 *
 * This class provides callback mechanism for TMSClientSink listeners.
 *
 * Client class that wants to receive data buffer events from TMS must inherit
 * from TMSClientSinkObserver class and register itself with the TMSBuffer
 * object.
 *
 * Functions declared in this class are called when data is available at the
 * sink and pushed to the application for further processing.
 *
 * The client must not do any blocking, or perform any CPU-intensive operations
 * in the callback methods. It is recommended that the client saves event data,
 * returns from this function immediately and if necessary, responds to
 * the event in asynchronous way.
 *
 * @lib tmsapi.lib
 *
 */
class TMSClientSinkObserver
    {
public:
    /**
     * This method is called when the TMS pushes data buffers to the Client.
     *
     * Only push mode is supported at this moment.
     *
     * @param  buffer
     *      Buffer with recorded data to be emptied by the client.
     *
     */
    virtual void ProcessBuffer(const TMSBuffer* buffer) = 0;
    };

} //namespace TMS

#endif // TMS_CLIENT_SINK_OBSRVR_H

// End of file
