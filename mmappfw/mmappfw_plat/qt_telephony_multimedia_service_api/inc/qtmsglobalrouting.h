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

#ifndef QTMS_GLOBAL_ROUTING_H
#define QTMS_GLOBAL_ROUTING_H

#include <qtms.h>
#include <QObject>
#include "qtmswrapperexport.h"

// FORWARD DECLARATIONS
namespace TMS
{
class TMSGlobalRouting;
}

namespace QTMS
{

/**
 * QTMSGlobalRouting class
 *
 * This class provides audio routing control in the global QTMS scope.
 * Audio routing changes done by any client are adjusted accross all calls
 * serviced by the QTMS server.
 *
 * @lib QTMSapi.lib
 *
 */
class QTMS_WRAPPER_DLL_EXPORT QTMSGlobalRouting : public QObject
{
    Q_OBJECT
public:
    /**
     * Destructor
     */
    virtual ~QTMSGlobalRouting();

    /**
     * Set global output device audio should be routed to.
     *
     * Settings are call-type dependent.
     *
     * Common:
     *  QTMS_AUDIO_OUTPUT_NONE
     * VoIP call type:
     *  QTMS_AUDIO_OUTPUT_PUBLIC
     *  QTMS_AUDIO_OUTPUT_PRIVATE
     * CS call type:
     *  QTMS_AUDIO_OUTPUT_HANDSET
     *  QTMS_AUDIO_OUTPUT_LOUDSPEAKER
     *  QTMS_AUDIO_OUTPUT_WIRED_ACCESSORY
     *  QTMS_AUDIO_OUTPUT_ACCESSORY
     *  QTMS_AUDIO_OUTPUT_ETTY
     *
     * @param  output
     *      Sets call-specific audio output device.
     *
     * @return
     *      QTMS_RESULT_INVALID_ARGUMENT if set to QTMS_AUDIO_OUTPUT_NONE.
     *      QTMS_RESULT_SUCCESS if the operation was successful.
     *
     */
    gint SetOutput(const QTMSAudioOutput output);

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
     *      QTMS_RESULT_SUCCESS if the operation was successful.
     *
     */
    gint GetOutput(QTMSAudioOutput& output);

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
     *      QTMS_RESULT_SUCCESS if the operation was successful.
     *
     */
    gint GetPreviousOutput(QTMSAudioOutput& output);

    /**
     * Return list of supported audio output devices.
     *
     * Settings are call-type dependent.
     * See SetOutput() (above)
     *
     * @param  outputs
     *      The typedef std::vector<guint> OutputVector; list of supported
     *      audio output devices (QTMSAudioOutput-type).
     *
     * @return
     *      QTMS_RESULT_SUCCESS if the operation was successful.
     *
     */
    gint GetAvailableOutputs(OutputVector& outputs);

    Q_SIGNALS:
    void GlobalRoutingEvent(const QTMSGlobalRouting& routing,
        QTMSSignalEvent event, QTMSAudioOutput output);

protected:
    /**
     * Constructor
     */
    QTMSGlobalRouting();

protected:
    TMS::TMSGlobalRouting* iRouting;
};

} //namespace QTMS

#endif //QTMS_GLOBAL_ROUTING_H
