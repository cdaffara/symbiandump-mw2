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
 * Description: QT Bindings for TMS
 *
 */

#ifndef QTMS_RINGTONE_H
#define QTMS_RINGTONE_H

#include <qtms.h>
#include <QObject>
#include "qtmswrapperexport.h"

// FORWARD DECLARATIONS
namespace TMS
{
class TMSRingTone;
}

namespace QTMS
{

/**
 * QTMSRingTone class
 *
 * This class provides ring tone playback capability to the QTMS clients.
 *
 * @lib QTMSapi.lib
 *
 */
class QTMS_WRAPPER_DLL_EXPORT QTMSRingTone : public QObject
{
    Q_OBJECT
public:
    /**
     * Destructor
     */
    virtual ~QTMSRingTone();

    /**
     * Init ring tone player to play supplied content based on its type.
     *
     * @param  type
     *      Type of the content descriptor. Allowed types:
     *          - QTMS_RINGTONE_DEFAULT - use RT from current Profile
     *          - QTMS_RINGTONE_FILE  - A/V File
     *          - QTMS_RINGTONE_SEQUENCE - RNG sequence in the descriptor format
     *          - QTMS_RINGTONE_BEEP_ONCE - play a beep
     *          - QTMS_RINGTONE_SILENT - play silent tone
     *          - QTMS_RINGTONE_UNSECURE_VOIP - play tone for uncecure VoIP call
     *
     * @param  str
     *      Content descriptor to play. String containing one of the following:
     *          - File with folder name
     *          - Nokia ring tone sequence in a descriptor format
     *          - NULL - Will use current Profile.
     *
     * @param  tts
     *      Text to be played. When added it will be played along with the
     *      tone. TTS can only be played with QTMS_RINGTONE_DEFAULT and
     *      TMS_RINGTONE_FILE. Parameter will be ignored for all other types.
     *          - NULL - Will use current Profile TTS setting - caller's name
     *            will be played from the phone book (if available).
     *
     * @return
     *      QTMS_RESULT_SUCCESS if the operation was successful.
     *
     */
    gint Init(const QTMSRingToneType type = QTMS_RINGTONE_DEFAULT,
        GString* str = NULL, GString* tts = NULL);

    /**
     * Deinit ring tone player.
     *
     * @return
     *      QTMS_RESULT_SUCCESS if the operation was successful.
     *
     */
    gint Deinit();

    /**
     * Start ring tone playback.
     *
     * @return
     *      QTMS_RESULT_SUCCESS if the operation was successful.
     *
     */
    gint Play();

    /**
     * Stop ring tone playback.
     *
     * @return
     *      QTMS_RESULT_SUCCESS if the operation was successful.
     *
     */
    gint Stop();

    /**
     * Pause video ring tone playback.
     *
     * @return
     *      QTMS_RESULT_SUCCESS if the operation was successful.
     *
     */
    gint Pause();

    /**
     * Mute ring tone playback.
     *
     * @return
     *      QTMS_RESULT_SUCCESS if the operation was successful.
     *
     */
    gint Mute();

    Q_SIGNALS:
    void RingtoneEvent(const QTMSRingTone& rt, QTMSSignalEvent event);

protected:
    /**
     * Constructor
     */
    QTMSRingTone();

protected:
    TMS::TMSRingTone *iRingTone;
};

} //namespace QTMS

#endif //QTMS_RINGTONE_H
// End of file

