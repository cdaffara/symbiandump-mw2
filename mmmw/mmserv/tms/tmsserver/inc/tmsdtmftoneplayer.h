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

#ifndef TMSAUDIODTMFTONEPLAYER_H
#define TMSAUDIODTMFTONEPLAYER_H

// INCLUDES
#include <glib.h>
#include <sounddevice.h>
#include "tmsdtmftoneplayerobserver.h"

// FORWARD DECLARATIONS
class CMMFDevSound;

// CLASS DECLARATION

/**
 *  Plays DTMF tones using media server
 */

namespace TMS {

class TMSAudioDtmfTonePlayer : public MDevSoundObserver
    {
public:
    // Constructors and destructor

    /**
     * Two-phased constructor.
     */
    static TMSAudioDtmfTonePlayer* NewL(TMSDTMFTonePlayerObserver& obsrvr,
            guint audioPreference, guint audioPriority);

    /**
     * Destructor.
     */
    virtual ~TMSAudioDtmfTonePlayer();

public:

    /**
     * Sets volume.
     * @param aVolume volume value or KDtmfSilent.
     * @return None.
     */
    void SetVolume(gint volume);

    /**
     * Starts playing given dtmf tone infinitely.
     * @param aTone Tone to be played.
     * @return None.
     */
    void PlayDtmfTone(TDes& tone);

    /**
     * Cancels playing.
     * @param None.
     * @return None.
     */
    void Cancel();

public:
    //From MDevSoundObserver
    /**
     * See MDevSoundObserver
     */
    void InitializeComplete(TInt aError);

    /**
     * See MDevSoundObserver
     */
    void ToneFinished(TInt aError);

    /**
     * See MDevSoundObserver
     */
    void BufferToBeFilled(CMMFBuffer* /*aBuffer*/) {}

    /**
     * See MDevSoundObserver
     */
    void PlayError(TInt /*aError*/) {}

    /**
     * See MDevSoundObserver
     */
    void BufferToBeEmptied(CMMFBuffer* /*aBuffer*/) {}

    /**
     * See MDevSoundObserver
     */
    void RecordError(TInt /*aError*/) {}

    /**
     * See MDevSoundObserver
     */
    void ConvertError(TInt /*aError*/) {}
    /**
     * See MDevSoundObserver
     */
    void DeviceMessage(TUid /*aMessageType*/, const TDesC8& /*aMsg*/) {}

protected:

    /**
     * C++ default constructor.
     */
    TMSAudioDtmfTonePlayer(TMSDTMFTonePlayerObserver& obsrvr,
            guint audioPreference, guint audioPriority);

    void ConstructL();

    /**
     * Normalize tone.
     * @param aTone dtmf tone.
     * @return None.
     */
    void Normalize(TChar& tone);

private:
    //new functions

    /**
     * Converts the given value to the one that matches tha range used by
     * CMMFDevSound. The CMMFDevSound uses valuess that can from 0 to the
     * value returned by a call to CMMFDevSound::MaxVolume().
     */
    gint ConvertVolumeLevel(gint volume);

protected:
    // Data

    // Current volume setting.
    gint iVolume;

    // DTMF player instance
    CMMFDevSound* iDTMFPlayer;

    TMSDTMFTonePlayerObserver& iObserver;
    guint iPref;
    guint iPrior;
    };

} //namespace TMS

#endif // TMSAUDIODTMFTONEPLAYER_H

// End of File
