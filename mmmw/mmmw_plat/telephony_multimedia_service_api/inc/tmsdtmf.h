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

#ifndef TMS_DTMF_H
#define TMS_DTMF_H

#include <glib.h>

namespace TMS {

// FORWARD DECLARATIONS
class TMSDTMFBody;
class TMSDTMFObserver;

/**
 * TMSDTMFclass
*
* This class provides DTMF playback capability to the TMS clients. Each
* DTMF player session is associated either with an uplink or downlink stream.
*
* @lib tmsapi.lib
*
*/
class TMSDTMF
    {
public:
    /**
     * Destructor
     */
    IMPORT_C virtual ~TMSDTMF();

    /**
     * Add client as an observer to TMSDTMF events.
     *
     * @param  obsrvr
     *      Observer object to TMSDTMF events.
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
    IMPORT_C gint AddObserver(TMSDTMFObserver& obsrvr, gpointer user_data);

    /**
     * Remove client as an observer to TMSDTMF events.
     *
     * @param  obsrvr
     *      Observer object to TMSDTMF events.
     *
     * @return
     *      TMS_RESULT_SUCCESS if the obsrvr is removed successfully from list.
     *      TMS_RESULT_DOES_NOT_EXIST if obsrvr is not already in the list.
     *
     */
    IMPORT_C gint RemoveObserver(TMSDTMFObserver& obsrvr);

    /**
     * Starts DTMF playback. In the case of downlink, the tone will be played
     * locally. In case of uplink, the network will play tones.
     *
     * Common for CS and VOIP (uplink and downlink)
     *
     * @return
     *      TMS_RESULT_SUCCESS if the operation was successful otherwise
     *      system error.
     *
     */
    IMPORT_C gint Start();

    /**
     * Stops DTMF playback.
     * Common for CS and VOIP (Uplink and Downlink).
     *
     * @return
     *      TMS_RESULT_SUCCESS if the operation was successful otherwise
     *      system error.
     *
     */
    IMPORT_C gint Stop();

    /**
     * Supply DTMF player with a string of tones to be played on local
     * playback device or sent via the uplink.
     *
     * @param  string
     *      String containing one or more DTMF digits.
     *
     * @return
     *      TMS_RESULT_SUCCESS if the operation was successful otherwise
     *      system error.
     *
     */
    IMPORT_C gint SetTone(GString* string);

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
     *      TMS_RESULT_SUCCESS if the operation was successful otherwise
     *      system error.
     *
     */
    IMPORT_C gint ContinueDTMFStringSending(gboolean sending);

protected:

    /**
     * Constructor
     */
    IMPORT_C TMSDTMF();

protected:
    TMSDTMFBody* iBody;
    };

} //namespace TMS

#endif //__TMS_DTMF_H__

// End of file
