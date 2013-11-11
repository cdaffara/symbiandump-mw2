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

#ifndef QTMS_INBAND_TONE_H
#define QTMS_INBAND_TONE_H

#include <glib.h>
#include <qtms.h>
#include <QObject>
#include "qtmswrapperexport.h"

// FORWARD DECLARATIONS
namespace TMS
{
class TMSInbandTone;
}

namespace QTMS
{

/**
 * QTMSInbandTone class
 *
 * This class provides Inband tone playback capability to the QTMS clients.
 *
 * @lib QTMSapi.lib
 *
 */
class QTMS_WRAPPER_DLL_EXPORT QTMSInbandTone : public QObject
{
    Q_OBJECT
public:
    /**
     * Destructor
     */
    virtual ~QTMSInbandTone();

    /**
     * Starts the inband tone playback. The inband volume will correspond to
     * the current downlink volume level.
     *
     * For CS calltype.
     *
     * @return
     *      QTMS_RESULT_SUCCESS if the operation was successful.
     *
     */
    gint Start(QTMSInbandToneType inbandtonetype);

    /**
     * Stops the inband tone playback.
     *
     * @return
     *      QTMS_RESULT_SUCCESS if the operation was successful.
     *
     */
    gint Stop();

    Q_SIGNALS:
    void InbandToneEvent(const QTMSInbandTone& inbandtone,
        QTMSSignalEvent event);

protected:

    /**
     * Constructor
     */
    QTMSInbandTone();

protected:
    TMS::TMSInbandTone* iInbandTone;
};

} //namespace QTMS

#endif //__QTMS_INBAND_TONE_H__
// End of file
