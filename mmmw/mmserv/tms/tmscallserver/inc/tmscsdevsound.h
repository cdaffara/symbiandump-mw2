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

#ifndef TMSCSDEVSOUND_H
#define TMSCSDEVSOUND_H

#include <sounddevice.h>
#include "tmstimer.h"

namespace TMS {

// FORWARD DECLARATIONS
class TMSTimer;
class TMSCSDevSoundObserver;

/**
 *  Wrapper for CMMFDevSound
 *
 */
NONSHARABLE_CLASS(TMSCSDevSound) : public CBase,
                                   public MDevSoundObserver,
                                   public TMSTimerObserver
    {
public:

    /**
     * Destructor.
     */
    virtual ~TMSCSDevSound();

    /**
     * Activates the DevSound stream.
     * Stream activation status is indicated by TMSCSDevSoundObserver callback
     * methods. If the stream is already active or being activated, call to
     * this will result in no action.
     */
    virtual void Activate(const gint retrytime);

    /**
     * Deactivates an active DevSound stream.
     */
    virtual void Deactivate(gboolean reset = TRUE);

    /*
     * Returns DevSound instance associated with the stream.
     */
    CMMFDevSound& DevSound();

    /**
     * From MDevSoundObserver
     * Indicates DevSound initialization status.
     */
    void InitializeComplete(TInt aError);

    /**
     * From MDevSoundObserver
     * Empty implementation
     */
    void BufferToBeFilled(CMMFBuffer* /*aBuffer*/) {}

    /**
     * From MDevSoundObserver
     * Empty implementation
     */
    void PlayError(TInt /*aError*/) {}

    /**
     * From MDevSoundObserver
     * Empty implementation
     */
    void ToneFinished(TInt /*aError*/) {}

    /**
     * From MDevSoundObserver
     * Empty implementation
     */
    void BufferToBeEmptied(CMMFBuffer* /*aBuffer*/) {}

    /**
     * From MDevSoundObserver
     * Empty implementation
     */
    void RecordError(TInt /*aError*/) {}

    /**
     * From MDevSoundObserver
     * Empty implementation
     */
    void ConvertError(TInt /*aError*/) {}

    /**
     * From MDevSoundObserver
     * Empty implementation
     */
    void DeviceMessage(TUid /*aMessageType*/, const TDesC8& /*aMsg*/) {}

    /*
     * From TMSTimerObserver.
     * Called upon timer timeout event.
     */
    void TimerEvent();

protected:

    TMSCSDevSound(TMSCSDevSoundObserver& observer);
    void ConstructL(const TMSStreamType strmtype, const gint retrytime);
    void InitializeL();
    void NotifyEvent(gint error);
    void StartTimer();
    void CancelTimer();

private:

    /**
     * Derived class has to activate the stream here.
     */
    virtual void DoActivateL() = 0;

protected:

    /**
     * Indication of DevSound activated state. TRUE == stream activated.
     * Derived class has to update this.
     */
    gboolean iActive;

    /**
     * Indication of an ongoing DevSound activation.
     * Derived class has to update this.
     */
    gboolean iActivationOngoing;

    /**
     * Devsound instance associated with the stream.
     */
    CMMFDevSound* iDevSound;

    /**
     * An observer subscribing to TMSCSDevSound notifications.
     */
    TMSCSDevSoundObserver& iObserver;

    TMSStreamType iStreamType;
    gint iPreference;
    gint iPriority;
    TMMFState iMode;

    /*
     * For retry timer
     */
    gint iInitRetryTime;
    gint iStartRetryTime;
    TMSTimer* iTimer;
    gint iPeriodic;
    gint iElapsedTime;
    };

} //namespace TMS

#endif //TMSCSDEVSOUND_H
