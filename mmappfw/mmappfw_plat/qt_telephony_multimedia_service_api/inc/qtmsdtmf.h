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

#ifndef QTMS_DTMF_H
#define QTMS_DTMF_H

#include <qtms.h>
#include <QObject>
#include "qtmswrapperexport.h"

// FORWARD DECLARATIONS
namespace TMS
{
class TMSDTMF;
}

namespace QTMS
{

/**
 * QTMSDTMFclass
 *
 * This class provides DTMF playback capability to the QTMS clients. Each
 * DTMF player session is associated either with an uplink or downlink stream.
 *
 * @lib QTMSapi.lib
 *
 */
class QTMS_WRAPPER_DLL_EXPORT QTMSDTMF : public QObject
{
    Q_OBJECT
public:
    /**
     * Destructor
     */
    virtual ~QTMSDTMF();

    /**
     * Starts DTMF playback. In the case of downlink, the tone will be played
     * locally. In case of uplink, the network will play tones.
     *
     * Common for CS and VOIP (uplink and downlink)
     *
     * @return
     *      QTMS_RESULT_SUCCESS if the operation was successful.
     *
     */
    gint Start();

    /**
     * Stops DTMF playback.
     * Common for CS and VOIP (Uplink and Downlink).
     *
     * @return
     *      QTMS_RESULT_SUCCESS if the operation was successful.
     *
     */
    gint Stop();

    /**
     * Supply DTMF player with a string of tones to be played on local
     * playback device or sent via the uplink.
     *
     * @param  string
     *      String containing one or more DTMF digits.
     *
     * @return
     *      QTMS_RESULT_SUCCESS if the operation was successful.
     *
     */
    gint SetTone(GString* string);

    /**
     *  Allows a client to continue or cancel the sending of a DTMF string
     *  when it was stopped by the use of ‘w’ char in the string.
     *  The client will set sending param to true if it wishes to continue
     *  the DTMF string sending and will set sending param to false if it
     *  wishes to discard the rest of the DTMF string.
     *  Reference: Multimode ETel API Design Document
     *
     *  For CS calltype
     *
     *  @param  sending
     *      Boolean to continue sending DTMF string.
     *
     * @return
     *      QTMS_RESULT_SUCCESS if the operation was successful.
     *
     */
    gint ContinueDTMFStringSending(gboolean sending);

    Q_SIGNALS:
    void DTMFEvent(const QTMSDTMF& dtmf, QTMSSignalEvent event);

protected:

    /**
     * Constructor
     */
    QTMSDTMF();

protected:
    TMS::TMSDTMF* iDtmf;
};

} //namespace QTMS

#endif //__QTMS_DTMF_H__
// End of file
