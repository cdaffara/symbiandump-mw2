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

#ifndef QTMS_VOLUME_EFFECT_H
#define QTMS_VOLUME_EFFECT_H

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
 * QTMSVolumeEffect class
 *
 * This class provides volume level control in the stream scope.
 *
 * @lib QTMSapi.lib
 *
 */
class QTMS_WRAPPER_DLL_EXPORT QTMSVolumeEffect : public QObject,
                                                 public QTMSEffect
{
    Q_OBJECT
public:
    /**
     * Destructor
     */
    virtual ~QTMSVolumeEffect();

    /**
     * Set audio volume level. The volume level should not be higher than
     * the value returned by GetMaxLevel().
     *
     * @param  level
     *      Volume level to be set.
     *
     * @return
     *      QTMS_RESULT_SUCCESS if the operation was successful.
     *
     */
    gint SetLevel(const guint level);

    /**
     * Return current audio volume level settings.
     *
     * @param  level
     *      Volume level.
     *
     * @return
     *      QTMS_RESULT_SUCCESS if the operation was successful.
     *
     */
    gint GetLevel(guint& level);

    /**
     * Return max volume level supported by the device.
     *
     * @param  level
     *      Max volume level.
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
    QTMSVolumeEffect();

protected:
    TMS::TMSEffect *iEffect;
};

} //namespace QTMS

#endif //QTMS_VOLUME_EFFECT_H
