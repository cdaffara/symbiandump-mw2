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
 * Description:TMSGlobalGainEffect class
 *
 */

#ifndef TMS_GLOBAL_GAIN_EFFECT_H
#define TMS_GLOBAL_GAIN_EFFECT_H

#include <tms.h>
#include <tmseffect.h>

namespace TMS {

// FORWARD DECLARATIONS
class TMSGlobalGainEffectBody;
class TMSEffectObserver;

/**
 * TMSGlobalGainEffect class
 *
 * This class provides microphone gain level control in the global TMS scope.
 * Mic gain level changes done by any client are adjusted accross all calls
 * serviced by the TMS server.
 *
 * @lib tmsapi.lib
 *
 */
class TMSGlobalGainEffect : public TMSEffect
    {
public:
    /**
     * Destructor
     */
    IMPORT_C virtual ~TMSGlobalGainEffect();

    /**
     * Add a client as an observer for this Global Gain Effect.
     *
     * This function can be called at any time. It is recommended adding the
     * observer before calling any functions on the Effect. Otherwise, the
     * observer may miss a callback.
     *
     * A reference to the obsrvr is stored in the effect. EffectsEvent callback
     * will be called when an effect event occurs. When effect is deleted, all
     * TMSEffectObserver references are automatically released. The ownership of
     * the obsrvr is still maintained by the client. Should the client need to
     * delete the obsrvr before deleting the effect, it should remove the obsrvr
     * reference from the effect by calling RemoveObserver() function.
     *
     * @param  obsrvr
     *      Listener to be added to the global gain effect object.
     *
     * @param  user_data
     *      Any user data passed to the function.
     *
     * @return
     *      TMS_RESULT_SUCCESS if the obsrvr is successfully added to the list.
     *      TMS_RESULT_ALREADY_EXIST if the obsrvr is already in the list.
     *
     */
    IMPORT_C gint AddObserver(TMSEffectObserver& obsrvr, gpointer user_data);

    /**
     * Remove client observer from the global gain effect.
     *
     * This function can be called at any time.
     *
     * @param  obsrvr
     *      Listener to be removed from the global gain effect object.
     *
     * @return
     *      TMS_RESULT_SUCCESS if the obsrvr is successfully removed.
     *      TMS_RESULT_DOES_NOT_EXIST if trying to remove the obsrvr that has
     *      not been added to the list.
     *
     */
    IMPORT_C gint RemoveObserver(TMSEffectObserver& obsrvr);

    /**
     * Set global microphone gain level. The global gain level should be
     * between 0 and the max value returned by GetMaxLevel().
     *
     * @param  level
     *      Gain level to be set.
     *
     * @return
     *      TMS_RESULT_SUCCESS if the operation was successful otherwise
     *      system error.
     *
     */
    IMPORT_C gint SetLevel(const guint level);

    /**
     * Return current global microphone gain level.
     *
     * @param  level
     *      Current mic gain level.
     *
     * @return
     *      TMS_RESULT_SUCCESS if the operation was successful otherwise
     *      system error.
     *
     */
    IMPORT_C gint GetLevel(guint& level);

    /**
     * Return max microphone gain level supported by the device.
     *
     * @param  level
     *      Max mic gain level.
     *
     * @return
     *      TMS_RESULT_SUCCESS if the operation was successful otherwise
     *      system error.
     *
     */
    IMPORT_C gint GetMaxLevel(guint& level);

    /**
     * Return effect type.
     *
     * @param  effecttype
     *      Type of the effect object (TMS_EFFECT_GLOBAL_GAIN).
     *
     * @return
     *      TMS_RESULT_SUCCESS if the operation was successful otherwise
     *      system error.
     *
     */
    IMPORT_C virtual gint GetType(TMSEffectType& effecttype);

protected:
    /**
     * Constructor
     */
    IMPORT_C TMSGlobalGainEffect();

protected:
    TMSGlobalGainEffectBody *iBody;
    };

} //namespace TMS

#endif //TMS_GLOBAL_GAIN_EFFECT_H
