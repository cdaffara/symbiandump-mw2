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

#ifndef TMSCSUPLINK_H
#define TMSCSUPLINK_H

#include <glib.h>
#include "tmscsdevsound.h"

namespace TMS {

/**
 *  Provides Uplink stream functionality.
 *
 */
NONSHARABLE_CLASS(TMSCSUplink) : public TMSCSDevSound
    {
public:

    static TMSCSUplink* NewL(TMSCSDevSoundObserver& observer,
            const gint retrytime);

    virtual ~TMSCSUplink();

    /**
     * Set mic gain.
     */
    void SetGain(gint gain);

    /**
     * Get mic muted.
     */
    gint Gain();

    /**
     * Get max gain.
     */
    gint MaxGain();

    // from base class MDevSoundObserver

    /**
     * From MDevSoundObserver.
     * Indication from the devsound that Uplink has been activated
     * successfully.
     */
    void BufferToBeEmptied(CMMFBuffer* aBuffer);

    /**
     * From MDevSoundObserver
     * Indication from devsound that Uplink activation has failed.
     */
    void RecordError(TInt aError);

private:

    /**
     * From TMSCSDevSound.
     * Tries to activate the mic stream.
     */
    void DoActivateL();

protected:

    TMSCSUplink(TMSCSDevSoundObserver& aObserver);
    void ConstructL(const gint retrytime);
    };

} //namespace TMS

#endif // TMSCSUPLINK_H
