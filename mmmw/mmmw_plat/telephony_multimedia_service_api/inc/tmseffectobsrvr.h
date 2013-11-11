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
 * Description: This class provides callback mechanism for TMSEffect listeners.
 *
 */

#ifndef TMS_EFFECT_OBSRVR_H
#define TMS_EFFECT_OBSRVR_H

// Include files
#include <tms.h>

namespace TMS {

// FORWARD DECLARATIONS
class TMSEffect;

/**
 * TMSEffectObserver class
 *
 * This class provides callback mechanism for TMSEffect listeners.
 *
 * Client class that wants to receive events from TMSEffect concrete class
 * objects must inherit from TMSEffectObserver class and register itself with
 * the TMSEffect object.
 *
 * Functions declared in this class are called when effect events occur.
 *
 * The client must not do any blocking, or perform any CPU-intensive operations
 * in the callback methods. It is recommended that the client saves event data,
 * returns from this function immediately and if necessary, responds to
 * the event in asynchronous way.
 *
 * Usage:
 * <code>
 * // Register as observer to TMSVolumeEffect
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
class TMSEffectObserver
    {
public:
    /**
     * Signal Effects event.
     *
     * This method is called when an Effects event occurs.
     *
     * Effect types:
     *  TMS_EFFECT_GLOBAL_VOL
     *  TMS_EFFECT_GLOBAL_GAIN
     *  TMS_EFFECT_VOLUME
     *  TMS_EFFECT_GAIN
     *
     * Notified events:
     *  TMS_EVENT_EFFECT_VOL_CHANGED
     *  TMS_EVENT_EFFECT_GAIN_CHANGED
     *
     * @param  tmseffect
     *      Reference to the object that is being signaled.
     *
     * @param  event
     *      Event descriptor.
     *
     */
    virtual void EffectsEvent(const TMSEffect& tmseffect,
            TMSSignalEvent event) = 0;
    };

} //namespace TMS

#endif // TMS_EFFECT_OBSRVR_H

// End of file
