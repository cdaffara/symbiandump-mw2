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

#ifndef QTMS_GAIN_EFFECT_H
#define QTMS_GAIN_EFFECT_H

#include <qtms.h>
#include <qtmseffect.h>
#include "qtmswrapperexport.h"

// FORWARD DECLARATIONS
namespace TMS
{
class TMSEffect;
}

namespace QTMS
{

/**
 * QTMSGainEffect class
 *
 * This class provides microphone gain level control in the stream scope.
 *
 * @lib QTMSapi.lib
 *
 */
class QTMS_WRAPPER_DLL_EXPORT QTMSGainEffect : public QObject,
public QTMSEffect
{
    Q_OBJECT
public:
    /**
     * Destructor
     */
    virtual ~QTMSGainEffect();

    /**
     * Set mic gain level. The gain level should not be higher than
     * the value returned by GetMaxLevel().
     *
     * @param  level
     *      Gain level to be set.
     *
     * @return
     *      QTMS_RESULT_SUCCESS if the operation was successful.
     *
     */
    gint SetLevel(const guint level);

    /**
     * Return mic gain level.
     *
     * @param  level
     *      Current mic gain level.
     *
     * @return
     *      QTMS_RESULT_SUCCESS if the operation was successful.
     *
     */
    gint GetLevel(guint& level);

    /**
     * Return max microphone gain level supported by the device.
     *
     * @param  level
     *      Max gain level.
     *
     * @return
     *      QTMS_RESULT_SUCCESS if the operation was successful.
     *
     */
    gint GetMaxLevel(guint& level);

    /**
     * Returns type of the effect.
     *
     * This function can be called at any time.
     *
     * @param  effecttype
     *      Variable that contains the effect type on return.
     *
     * @return
     *      QTMS_RESULT_SUCCESS if the operation was successful.
     *
     */
    virtual gint GetType(QTMSEffectType& effecttype);

    Q_SIGNALS:
    void EffectsEvent(const QTMSEffect& tmseffect, QTMSSignalEvent event);

protected:
    /**
     * Constructor
     */
    QTMSGainEffect();

protected:
    TMS::TMSEffect *iEffect;
};

} //namespace QTMS

#endif //QTMS_GAIN_EFFECT_H
