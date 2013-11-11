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

#ifndef TMSAUDIOTONEUTILITYIMPL_H
#define TMSAUDIOTONEUTILITYIMPL_H

#include <e32base.h>
#include <mdaaudiotoneplayer.h>
#include <glib.h>
#include "tmsaudiotoneutility.h"

namespace TMS {

/**
 *  Wraps CMdaAudioToneUtility.
 *  To ease module testing.
 *
 *  @lib AudioHandling
 */
class TMSAudioToneUtilityImpl : public TMSAudioToneUtility
    {
public:

    static TMSAudioToneUtilityImpl* NewL(
            MMdaAudioToneObserver& toneObserver);

    /**
     * Destructor.
     */
    virtual ~TMSAudioToneUtilityImpl();

    // from base class TMSAudioToneUtilityImpl

    /**
     * From TMSAudioToneUtilityImpl
     * @see CMdaAudioToneUtility
     */
    void PrepareToPlayDesSequence(const TDesC8& sequence);

    /**
     * From TMSAudioToneUtilityImpl
     * @see CMdaAudioToneUtility
     */
    TMdaAudioToneUtilityState State();

    /**
     * From TMSAudioToneUtilityImpl
     * @see CMdaAudioToneUtility
     */
    void CancelPlay();

    /**
     * From TMSAudioToneUtilityImpl
     * @see CMdaAudioToneUtility
     */
    gint MaxVolume();

    /**
     * From TMSAudioToneUtilityImpl
     * @see CMdaAudioToneUtility
     */
    void SetVolume(gint volume);

    /**
     * From TMSAudioToneUtilityImpl
     * @see CMdaAudioToneUtility
     */
    void SetRepeats(gint repeatNumberOfTimes,
            const TTimeIntervalMicroSeconds& trailingSilence);

    /**
     * From TMSAudioToneUtilityImpl
     * @see CMdaAudioToneUtility
     */
    void SetPriority(gint priority, TMdaPriorityPreference pref);

    /**
     * From TMSAudioToneUtilityImpl
     * @see CMdaAudioToneUtility
     */
    void SetVolumeRamp(const TTimeIntervalMicroSeconds& rampDuration);

    /**
     * From TMSAudioToneUtilityImpl
     * @see CMdaAudioToneUtility
     */
    void Play();

    /**
     * From TMSAudioToneUtilityImpl
     * @see CMdaAudioToneUtility
     */
    void CancelPrepare();

private:

    TMSAudioToneUtilityImpl();

    void ConstructL(MMdaAudioToneObserver& toneObserver);

private:
    // data

    /**
     * Own.
     * For playing tones.
     */
    CMdaAudioToneUtility* iToneUtility;
    };
} //namespace TMS

#endif // TMSAUDIOTONEUTILITYIMPL_H
