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

#ifndef TMSAUDIOINBANDTONEPLAYER_H
#define TMSAUDIOINBANDTONEPLAYER_H

// INCLUDES
#include <AudioPreference.h>
#include <mdaaudiotoneplayer.h>
#include <barsc.h>
#include <barsread.h>
#include <ccpdefs.h>
#include <tms.h>

namespace TMS {

// CONSTANTS
const gint KPhoneInbandToneZero = 0;

// FORWARD DECLARATIONS
class TMSAudioToneUtility;

// CLASS DECLARATION

/**
 *  This class is used to play inband tones using media server.
 *
 */
class TMSAudioInbandTonePlayer : public MMdaAudioToneObserver
    {
public:
    // Constructors and destructor

    /**
     * Two-phased constructor.
     */
    static TMSAudioInbandTonePlayer* NewL();

    /**
     * Destructor.
     */
    virtual ~TMSAudioInbandTonePlayer();

    /**
     * Plays given tone. Originates from CCP plugin.
     * @param aTone inband tone needed to play
     */
    void PlayInbandTone(TMSInbandToneType tone);

    /**
     * Cancels playing.
     */
    void Cancel();

    /**
     * Sets volume.
     * @param    aVolume     Volume in 0...10 scale.
     */
    void SetVolume(gint volume);

private:

    /**
     * C++ default constructor.
     */
    TMSAudioInbandTonePlayer();

    /**
     * By default Symbian 2nd phase constructor is private.
     */
    void ConstructL();

    /**
     * Create inband tone sequences from resources.
     */
    void CreateToneSequencesL();

    /**
     * Playes current tone.
     * @param None.
     * @return None.
     */
    void PlayCurrentTone();

    /**
     * Configure tone player utility for current sequence playback.
     */
    void SetToneAttributes(const guint pref,
            const guint priority = KAudioPriorityNetMsg,
            const gint repeatTimes = KMdaAudioToneRepeatForever,
            const gint trailSilence = KPhoneInbandToneZero);

    /**
     * Allocate tone sequence from resource.
     */
    void AllocSeqFromResourceL(const gint resource);

    //From MMdaAudioToneObserver

    /**
     * Handles completion of preparation.
     * @param aError Isn't used.
     * @return None.
     */
    void MatoPrepareComplete(TInt aError);

    //From MMdaAudioToneObserver
    /**
     * Handles completion of playing.
     * @param aError Isn't used.
     * @return None.
     */
    void MatoPlayComplete(TInt aError);

    /**
     * Calculates media server volume level.
     * @param   aVolume     Volume level in 0...10 scale.
     * @return  Volume level in media server scale.
     */
    gint CalculateMediaServerVolume(gint volume) const;

    /**
     * Updates tone player's volume to the current audio volume level.
     */
    void UpdateTonePlayerVolume();

private:
    // Data

    // Currently playing, NULL or tone sequence.
    TMSAudioToneUtility* iPlayer;
    // Member variable for inband tone
    TMSInbandToneType iToneName;
    // Ringing type
    // TProfileRingingType iRingingType;
    // Handle to a file server session
    RFs iFsSession;
	// For reading sequence tones from resource file
    TFileName iFileName;
    RResourceFile iResourceFile;
    TResourceReader iReader;
    // Array of tone pointers
    RPointerArray<HBufC8> iTones;
    };

} // namespace TMS

#endif // TMSAUDIOINBANDTONEPLAYER_H

// End of File
