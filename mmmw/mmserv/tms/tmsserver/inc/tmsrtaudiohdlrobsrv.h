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
 * Description:  Observer interface for audio player.
 *
 */

#ifndef TMSRTAUDIOHDLROBSRV_H
#define TMSRTAUDIOHDLROBSRV_H

// INCLUDES

namespace TMS {

// CLASS DECLARATION

/**
 *  Observer interface for audio player.
 */
class TMSRtAudioHdlrObsrv
    {
public:
    /**
     * This method is called when TMSRingTone event occurs.
     *
     * Notified events:
     *  ECmdRingToneOpenComplete
     *  ECmdRingTonePlayComplete
     *
     * @param  aEvent
     *      Event descriptor.
     * @param  aError
     *      Operation status.
     * @param  aPlayer
     *      Indicates audio player that generated the event.
     */
    virtual void RtAudioHdlrEvent(TInt aEvent, TInt aError, TInt aPlayer) = 0;
    };

} // namespace TMS

#endif // TMSRTAUDIOHDLROBSRV_H

// End of File
