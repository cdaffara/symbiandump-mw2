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

#ifndef TMS_INBAND_TONE_H
#define TMS_INBAND_TONE_H

#include <tms.h>

namespace TMS {

// FORWARD DECLARATIONS
class TMSInbandToneBody;
class TMSInbandToneObserver;

/**
 * TMSInbandTone class
*
* This class provides Inband tone playback capability to the TMS clients.
*
* @lib tmsapi.lib
*
*/
class TMSInbandTone
    {
public:
    /**
     * Destructor
     */
    IMPORT_C virtual ~TMSInbandTone();

    /**
     * Add client as an observer to TMSInbandTone events.
     *
     * @param  obsrvr
     *      Observer object to TMSInbandTone events.
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
    IMPORT_C gint AddObserver(TMSInbandToneObserver& obsrvr, gpointer user_data);

    /**
     * Remove client as an observer to TMSInbandTone events.
     *
     * @param  obsrvr
     *      Observer object to TMSInbandTone events.
     *
     * @return
     *      TMS_RESULT_SUCCESS if the obsrvr is removed successfully from list.
     *      TMS_RESULT_DOES_NOT_EXIST if obsrvr is not already in the list.
     *
     */
    IMPORT_C gint RemoveObserver(TMSInbandToneObserver& obsrvr);

    /**
     * Starts the inband tone playback. The inband volume will correspond to
     * the current downlink volume level.
     *
     * For CS calltype.
     *
     * @return
     *      TMS_RESULT_SUCCESS if the operation was successful otherwise
     *      system error.
     *
     */
    IMPORT_C gint Start(TMSInbandToneType inbandtonetype);

    /**
     * Stops the inband tone playback.
     *
     * @return
     *      TMS_RESULT_SUCCESS if the operation was successful otherwise
     *      system error.
     *
     */
    IMPORT_C gint Stop();


protected:

    /**
     * Constructor
     */
    IMPORT_C TMSInbandTone();

protected:
    TMSInbandToneBody* iBody;
    };

} //namespace TMS

#endif //__TMS_INBAND_TONE_H__

// End of file
