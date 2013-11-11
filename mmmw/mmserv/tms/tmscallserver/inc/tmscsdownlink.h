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

#ifndef TMSCSDOWNLINK_H
#define TMSCSDOWNLINK_H

#include <glib.h>
#include "tmscsdevsound.h"

namespace TMS {

/**
 *  Provides Downlink stream functionality.
 *
 */
NONSHARABLE_CLASS(TMSCSDownlink) : public TMSCSDevSound
    {
public:

    static TMSCSDownlink* NewL(TMSCSDevSoundObserver& observer,
            const gint retrytime);

    virtual ~TMSCSDownlink();

    /**
     * Sets volume.
     * @param volume Volume.
     */
    void SetVolume(gint volume);

    /**
     * Gets volume.
     * @return Current volume.
     */
    gint Volume();

    /**
     * Gets max volume.
     * @return Current volume.
     */
    gint MaxVolume();

    // from base class MDevSoundObserver

    /**
     * From MDevSoundObserver.
     * Indication from the devsound that Downlink has been activated
     * successfully.
     */
    void BufferToBeFilled(CMMFBuffer* /*aBuffer*/);

    /**
     * From MDevSoundObserver.
     * Indication from devsound that Downlink activation has failed.
     */
    void PlayError(TInt aErrorCode);

private:

    /**
     * From TMSCSDevSound.
     * Tries to activate the mic stream.
     */
    void DoActivateL();

protected:

    TMSCSDownlink(TMSCSDevSoundObserver& observer);
    void ConstructL(const gint retrytime);
    };

} //namespace TMS

#endif // TMSCSDOWNLINK_H
