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

#ifndef QTMS_G729_FORMAT_H
#define QTMS_G729_FORMAT_H

#include <qtms.h>
#include <qtmsformat.h>
#include <QObject>
#include "qtmswrapperexport.h"

// FORWARD DECLARATIONS
namespace TMS
{
class TMSFormat;
}

namespace QTMS
{

/**
 * QTMSG729Format class
 *
 * This class provides access methods to configure and query G.729 codec
 * format settings.
 *
 * This format class can be used for both the encoder and decoder.
 *
 * @lib QTMSapi.lib
 *
 */
class QTMS_WRAPPER_DLL_EXPORT QTMSG729Format: public QObject, public QTMSFormat
{
Q_OBJECT
public:
    /**
     * Destructor
     */
    virtual ~QTMSG729Format();

    /**
     * Toggles on and off Voice Activation Detection (VAD) for G.729 encoder.
     * Not supported by the decoder.
     *
     * This method is allowable at all times, while actively encoding or not.
     *
     * @param  vad
     *      Sets VAD feature on or off.
     *
     * @return
     *      QTMS_RESULT_SUCCESS if the operation was successful.
     *      QTMS_RESULT_FEATURE_NOT_SUPPORTED if this method is not implemented
     *      by the adaptation.
     *      QTMS_RESULT_DOES_NOT_EXIST if the custom interface handle does
     *      not exist.
     *
     */
    gint SetVADMode(const gboolean vad);

    /**
     * Returns current VAD setting of the G.729 encoder.
     *
     * This method is allowable at all times, while actively encoding or not.
     *
     * @param  vad
     *      Current VAD setting of the encoder.
     *
     * @return
     *      QTMS_RESULT_SUCCESS if the operation was successful.
     *
     */
    gint GetVADMode(gboolean& vad);

protected:
    /**
     * Constructor
     */
    QTMSG729Format();
};

} //namespace QTMS

#endif //QTMS_G729_FORMAT_H
