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

#ifndef TMS_RINGTONE_H
#define TMS_RINGTONE_H

#include <tms.h>

namespace TMS {

// FORWARD DECLARATIONS
class TMSRingToneImpl;
class TMSRingToneBody;
class TMSRingToneObserver;

/**
 * TMSRingTone class
 *
 * This class provides ring tone playback capability to the TMS clients.
 *
 * @lib tmsapi.lib
 *
 */
class TMSRingTone
    {
public:
    /**
     * Destructor
     */
    IMPORT_C virtual ~TMSRingTone();

    /**
     * Add client as an observer to TMSRingTone events.
     *
     * @param  obsrvr
     *      Observer object to TMSRingTone events.
     *
     * @param  user_data
     *      Any user data passed to the function.
     *
     * @return
     *      TMS_RESULT_SUCCESS if the operation was successful otherwise
     *      system error.
     *      TMS_RESULT_ALREADY_EXIST if the obsrvr is already in the list.
     *
     */
    IMPORT_C gint AddObserver(TMSRingToneObserver& obsrvr, gpointer user_data);

    /**
     * Remove client as an observer to TMSRingTone events.
     *
     * @param  obsrvr
     *      Observer object to TMSRingTone events.
     *
     * @return
     *      TMS_RESULT_SUCCESS if the obsrvr is removed successfully from list.
     *      TMS_RESULT_DOES_NOT_EXIST if obsrvr is not already in the list.
     *
     */
    IMPORT_C gint RemoveObserver(TMSRingToneObserver& obsrvr);

    /**
     * Init ring tone player to play supplied content based on its type.
     *
     * @param  type
     *      Type of the content descriptor. Allowed types:
     *          - TMS_RINGTONE_DEFAULT - use RT from current Profile
     *          - TMS_RINGTONE_FILE  - A/V File
     *          - TMS_RINGTONE_SEQUENCE - RNG sequence in the descriptor format
     *          - TMS_RINGTONE_BEEP_ONCE - play a beep
     *          - TMS_RINGTONE_SILENT - play silent tone
     *          - TMS_RINGTONE_UNSECURE_VOIP - play tone for uncecure VoIP call
     *
     * @param  str
     *      Content descriptor to play. String containing one of the following:
     *          - File with folder name
     *          - Nokia ring tone sequence in a descriptor format
     *          - NULL - Will use current Profile.
     *
     * @param  tts
     *      Text to be played. When added it will be played along with the
     *      tone. TTS can only be played with TMS_RINGTONE_DEFAULT and
     *      TMS_RINGTONE_FILE. Parameter will be ignored for all other types.
     *          - NULL - Will use current Profile TTS setting - caller's name
     *            will be played from the phone book (if available).
     *
     * @return
     *      TMS_RESULT_SUCCESS if the operation was successful otherwise
     *      system error.
     *
     */
    IMPORT_C gint Init(const TMSRingToneType type = TMS_RINGTONE_DEFAULT,
            GString* str = NULL, GString* tts = NULL);

    /**
     * Deinit ring tone player.
     *
     * @return
     *      TMS_RESULT_SUCCESS if the operation was successful otherwise
     *      system error.
     *
     */
    IMPORT_C gint Deinit();

    /**
     * Start ring tone playback.
     *
     * @return
     *      TMS_RESULT_SUCCESS if the operation was successful otherwise
     *      system error.
     *
     */
    IMPORT_C gint Play();

    /**
     * Stop ring tone playback.
     *
     * @return
     *      TMS_RESULT_SUCCESS if the operation was successful otherwise
     *      system error.
     *
     */
    IMPORT_C gint Stop();

    /**
     * Pause video ring tone playback.
     *
     * @return
     *      TMS_RESULT_SUCCESS if the operation was successful otherwise
     *      system error.
     *
     */
    IMPORT_C gint Pause();

    /**
     * Mute ring tone playback.
     *
     * @return
     *      TMS_RESULT_SUCCESS if the operation was successful otherwise
     *      system error.
     *
     */
    IMPORT_C gint Mute();

protected:
    /**
     * Constructor
     */
    IMPORT_C TMSRingTone();

protected:
    TMSRingToneBody *iBody;
    };

} //namespace TMS

#endif //TMS_RINGTONE_H

// End of file

