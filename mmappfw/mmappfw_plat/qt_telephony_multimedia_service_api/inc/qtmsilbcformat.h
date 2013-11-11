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

#ifndef QTMS_ILBC_FORMAT_H
#define QTMS_ILBC_FORMAT_H

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
 * QTMSILBCFormat class
 *
 * This class provides access methods to configure and query iLBC codec
 * format settings.
 *
 * This format class can be used for both the encoder and decoder.
 *
 * @lib QTMSapi.lib
 *
 */
class QTMS_WRAPPER_DLL_EXPORT QTMSILBCFormat: public QObject, public QTMSFormat
{
Q_OBJECT
public:
    /**
     * Destructor
     */
    virtual ~QTMSILBCFormat();

    /**
     * Set iLBC codec mode.
     *
     * This method is allowable when encoding is not active; i.e. anytime
     * before encoding is started or after it is stopped.
     *
     * Supported iLBC codec modes are:
     *  QTMS_ILBC_CODEC_MODE_20MILLI_SEC_FRAME
     *  QTMS_ILBC_CODEC_MODE_30MILLI_SEC_FRAME
     *
     * @param  mode
     *      Sets selected iLBC encoding or decoding mode.
     *
     * @return
     *      QTMS_RESULT_SUCCESS if the operation was successful.
     *      QTMS_RESULT_INVALID_STATE if this method is used during active
     *      encoding or decoding operation.
     *      QTMS_RESULT_FEATURE_NOT_SUPPORTED if this method is not implemented
     *      by the adaptation.
     *      QTMS_RESULT_DOES_NOT_EXIST if the custom interface handle does
     *      not exist.
     *
     */
    gint SetMode(const QTMSILBCCodecMode mode);

    /**
     * Return current iLBC codec mode.
     *
     * This method is allowable at all times, while actively encoding/decoding
     * or not.
     *
     * Supported iLBC codec modes are:
     *  QTMS_ILBC_CODEC_MODE_20MILLI_SEC_FRAME
     *  QTMS_ILBC_CODEC_MODE_30MILLI_SEC_FRAME
     *
     * @param  mode
     *      Returns current iLBC encoding or decoding mode.
     *
     * @return
     *      QTMS_RESULT_SUCCESS if the operation was successful.
     *
     */
    gint GetMode(QTMSILBCCodecMode& mode);

    /**
     * Toggles on and off Comfort Noise Generator (CNG) for iLBC decoder.
     * Not supported by the encoder.
     *
     * This method is allowable when decoding is not active; i.e. anytime
     * before decoding is started or after it is stopped.
     *
     * @param  cng
     *      Sets CNG feature on or off.
     *
     * @return
     *      QTMS_RESULT_SUCCESS if the operation was successful.
     *      QTMS_RESULT_INVALID_STATE if this method is used during active
     *      decoding operation.
     *      QTMS_RESULT_FEATURE_NOT_SUPPORTED if this method is not implemented
     *      by the adaptation.
     *      QTMS_RESULT_DOES_NOT_EXIST if the custom interface handle does
     *      not exist.
     *
     */
    gint SetCNG(const gboolean cng);

    /**
     * Returns current CNG setting of the iLBC decoder.
     *
     * This method is allowable when decoding is not active; i.e. anytime
     * before decoding is started or after it is stopped.
     *
     * @param  cng
     *      Current CNG setting of the decoder.
     *
     * @return
     *      QTMS_RESULT_SUCCESS if the operation was successful.
     *
     */
    gint GetCNG(gboolean& cng);

    /**
     * Toggles on and off Voice Activation Detection (VAD) for iLBC encoder.
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
     * Returns current VAD setting of the iLBC encoder.
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
    QTMSILBCFormat();
};

} //namespace QTMS

#endif //QTMS_ILBC_FORMAT_H
