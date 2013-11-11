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
 * Description: This class provides callback mechanism for TMSStream listeners.
 *
 */

#ifndef TMS_STREAM_OBSRVR_H
#define TMS_STREAM_OBSRVR_H

// Include files
#include <tms.h>

namespace TMS {

// Forward declarations
class TMSStream;

/**
 * TMSStreamObserver class
 *
 * This class provides callback mechanism for TMSStream listeners.
 *
 * Client class that is interested in observing stream notifications has to
 * inherit from TMSStreamObserver class and register itself with the TMSStream.
 * Functions declared in this class are called when events occur in TMSStream.
 *
 * The client must not do any blocking, or perform any CPU-intensive operations
 * in the callback methods. It is recommended that the client saves event data,
 * returns from this function immediately, and if necessary, responds to
 * the event in asynchronous way.
 *
 * Usage:
 * <code>
 * // Register as an observer to the uplink stream
 * iUplinkStream->AddObserver(*this, NULL);
 * ...
 * // Unregister (optional)
 * iUplinkStream->RemoveObserver(*this);
 * ...
 * </code>
 *
 * @lib tmsapi.lib
 *
 */
class TMSStreamObserver
    {
public:
    /**
     * Signal stream event.
     *
     * This function is called when a stream event occurs.
     *      (1) TMS_EVENT_TYPE_STATE_CHANGED.
     *
     *
     * @param  stream
     *      Reference to the object that is being signaled.
     *
     * @param  event
     *      Event descriptor.
     *
     */
    virtual void TMSStreamEvent(const TMSStream& stream,
            TMSSignalEvent event) = 0;
    };

} //namespace TMS

#endif // TMS_STREAM_OBSRVR_H

// End of file
