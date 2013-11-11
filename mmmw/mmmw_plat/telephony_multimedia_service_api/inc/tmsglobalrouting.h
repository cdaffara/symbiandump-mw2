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
 * Description:TMSGlobalRouting class
 *
 */

#ifndef TMS_GLOBAL_ROUTING_H
#define TMS_GLOBAL_ROUTING_H

#include <tms.h>

namespace TMS {

// FORWARD DECLARATIONS
class TMSGlobalRoutingBody;
class TMSGlobalRoutingObserver;

/**
 * TMSGlobalRouting class
 *
 * This class provides audio routing control in the global TMS scope.
 * Audio routing changes done by any client are adjusted accross all calls
 * serviced by the TMS server.
 *
 * @lib tmsapi.lib
 *
 */
class TMSGlobalRouting
    {
public:
    /**
     * Destructor
     */
    IMPORT_C virtual ~TMSGlobalRouting();

    /**
     * Add a client as an observer for global audio routing.
     *
     * This function can be called at any time. It is recommended to add
     * observer before calling any functions on TMSGlobalRouting. Otherwise,
     * the observer may miss a callback.
     *
     * A reference to the obsrvr is stored in the TMSGlobalRouting and
     * RoutingEvent callback will be called when global routing event occurs.
     * When TMSGlobalRouting is deleted, all TMSGlobalRoutingObserver references
     * are automatically released. The ownership of the obsrvr is still
     * maintained by the client. Should the client need to delete the obsrvr
     * before deleting the global routing object, it should remove the obsrvr
     * reference from the global routing by calling RemoveObserver() function.
     *
     * @param  obsrvr
     *      Listener to be added.
     *
     * @param  user_data
     *      Any user data passed to the function.
     *
     * @return
     *      TMS_RESULT_SUCCESS if the obsrvr is successfully added to the list.
     *      TMS_RESULT_ALREADY_EXIST if the obsrvr is already in the list.
     *
     */
    IMPORT_C gint AddObserver(TMSGlobalRoutingObserver& obsrvr,
            gpointer user_data);

    /**
     * Remove a client observer from this global routing object.
     *
     * This function can be called at any time.
     *
     * @param  obsrvr
     *      Listener to be removed.
     *
     * @return
     *      TMS_RESULT_SUCCESS if the obsrvr is successfully removed.
     *      TMS_RESULT_DOES_NOT_EXIST if trying to remove the obsrvr that has
     *      not been added to the list.
     *
     */
    IMPORT_C gint RemoveObserver(TMSGlobalRoutingObserver& obsrvr);

    /**
     * Set global output device audio should be routed to.
     *
     * Settings are call-type dependent.
     *
     * Common:
     *  TMS_AUDIO_OUTPUT_NONE
     * VoIP call type:
     *  TMS_AUDIO_OUTPUT_PUBLIC
     *  TMS_AUDIO_OUTPUT_PRIVATE
     * CS call type:
     *  TMS_AUDIO_OUTPUT_HANDSET
     *  TMS_AUDIO_OUTPUT_LOUDSPEAKER
     *  TMS_AUDIO_OUTPUT_WIRED_ACCESSORY
     *  TMS_AUDIO_OUTPUT_ACCESSORY
     *  TMS_AUDIO_OUTPUT_ETTY
     *
     * @param  output
     *      Sets call-specific audio output device.
     *
     * @return
     *      TMS_RESULT_INVALID_ARGUMENT if set to TMS_AUDIO_OUTPUT_NONE.
     *      TMS_RESULT_SUCCESS if the operation was successful otherwise
     *      system error.
     *
     */
    IMPORT_C gint SetOutput(const TMSAudioOutput output);

    /**
     * Return current global audio output device settings.
     *
     * Settings are call-type dependent.
     * See SetOutput() (above)
     *
     * @param  output
     *      Current audio output device.
     *
     * @return
     *      TMS_RESULT_SUCCESS if the operation was successful otherwise
     *      system error.
     *
     */
    IMPORT_C gint GetOutput(TMSAudioOutput& output);

    /**
     * Return previous audio output device.
     *
     * Settings are call-type dependent.
     * See SetOutput() (above)
     *
     * @param  output
     *      Previous audio output device.
     *
     * @return
     *      TMS_RESULT_SUCCESS if the operation was successful otherwise
     *      system error.
     *
     */
    IMPORT_C gint GetPreviousOutput(TMSAudioOutput& output);

    /**
     * Return list of supported audio output devices.
     *
     * Settings are call-type dependent.
     * See SetOutput() (above)
     *
     * @param  outputs
     *      The typedef std::vector<guint> OutputVector; list of supported
     *      audio output devices (TMSAudioOutput-type).
     *
     * @return
     *      TMS_RESULT_SUCCESS if the operation was successful otherwise
     *      system error.
     *
     */
    IMPORT_C gint GetAvailableOutputs(OutputVector& outputs);

protected:
    /**
     * Constructor
     */
    IMPORT_C TMSGlobalRouting();

protected:
    TMSGlobalRoutingBody* iBody;
    };

} //namespace TMS

#endif //TMS_GLOBAL_ROUTING_H
