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
 * Description: This class provides callback mechanism for TMSRingTone
 * listeners.
 *
 */

#ifndef TMS_RINGTONE_OBSRVR_H
#define TMS_RINGTONE_OBSRVR_H

// Include files
#include <tms.h>

namespace TMS {

// FORWARD DECLARATIONS
class TMSRingTone;

/**
 * TMSRingToneObserver class
 *
 * This class provides callback mechanism for TMSRingTone listeners.
 *
 * @lib tmsapi.lib
 *
 */
class TMSRingToneObserver
    {
public:
    /**
     * This method is called when TMSRingTone event occurs.
     *
     * Notified events:
     *  TMS_EVENT_RINGTONE_OPEN_COMPLETE
     *  TMS_EVENT_RINGTONE_PLAY_COMPLETE
     *
     * @param  rt
     *      Reference to the object that is being signaled.
     *
     * @param  event
     *      Event descriptor.
     *
     */
    virtual void RingtoneEvent(const TMSRingTone& rt,
            TMSSignalEvent event) = 0;
    };

} //namespace TMS

#endif //TMS_RINGTONE_OBSRVR_H

// End of file
