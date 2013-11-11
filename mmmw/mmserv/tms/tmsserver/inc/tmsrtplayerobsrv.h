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
 * Description:  Observer interface for ringing tone playing.
 *
 */

#ifndef TMSRTPLAYEROBSRV_H
#define TMSRTPLAYEROBSRV_H

// INCLUDES
#include <e32base.h>

namespace TMS {

// FORWARD DECLARATIONS
class TMSRtPlayer;

// CLASS DECLARATION

/**
 *  Observer interface for TMSRingTonePlayer
 */
class TMSRtPlayerObsrv
    {
public:
    /**
     * This method is called when TMSRingTone event occurs.
     *
     * Notified events:
     *  ECmdRingToneOpenComplete
     *  ECmdRingTonePlayComplete
     *
     * @param  aEventType
     *      Event descriptor.
     *
     * @param  aError
     *      Operation status.
     *
     */
    virtual void RtPlayerEvent(TInt aEventType, TInt aError) = 0;
    };

} //namespace TMS

#endif // TMSRTPLAYEROBSRV_H

// End of File
