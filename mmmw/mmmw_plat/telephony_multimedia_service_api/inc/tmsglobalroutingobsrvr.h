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
 * Description: This class provides callback mechanism for TMSGlobalRouting
 * listeners.
 *
 */

#ifndef TMS_GLOBAL_ROUTING_OBSRVR_H
#define TMS_GLOBAL_ROUTING_OBSRVR_H

// Include files
#include <tms.h>

namespace TMS {

// FORWARD DECLARATIONS
class TMSGlobalRouting;

/**
 * TMSGlobalRoutingObserver class
 *
 * This class provides callback mechanism for TMSGlobalRouting listeners.
 *
 * Client class that is interested in observing global audio routing event
 * notifications must inherit from TMSGlobalRoutingObserver class and register
 * itself with the TMSGlobalRouting object.
 *
 * Functions declared in this class are called when global audio routing
 * events occur. GlobalRoutingEvent() will be called on every registered
 * client on any global audio routing event.
 *
 * The client must not do any blocking, or perform any CPU-intensive operations
 * in the callback methods. It is recommended that the client saves event data,
 * returns from this function immediately and if necessary, responds to
 * the event in asynchronous way.
 *
 * Usage:
 * <code>
 * // Register as an observer to TMSGlobalRouting
 * iClient->AddObserver(*this);
 * ...
 * // Unregister (optional)
 * iClient->RemoveObserver(*this);
 * ...
 * </code>
 *
 * @lib tmsapi.lib
 *
 */
class TMSGlobalRoutingObserver
    {
public:

    /**
     * Signal global audio routing event.
     *
     * This method is called when a global audio routing event occurs.
     *
     * Notified events:
     *  TMS_EVENT_ROUTING_AVAIL_OUTPUTS_CHANGED
     *  TMS_EVENT_ROUTING_OUTPUT_CHANGED
     *  TMS_EVENT_ROUTING_SET_OUTPUT_COMPLETE
     *
     * @param  routing
     *      Reference to the object that is being signaled.
     *
     * @param  event
     *      Event descriptor.
     *
     * @param  output
     *      Indicates the output device that audio will be routed to.
     *
     */
    virtual void GlobalRoutingEvent(const TMSGlobalRouting& routing,
            TMSSignalEvent event, TMSAudioOutput output) = 0;
    };

} //namespace TMS

#endif // TMS_GLOBAL_ROUTING_OBSRVR_H

// End of file
