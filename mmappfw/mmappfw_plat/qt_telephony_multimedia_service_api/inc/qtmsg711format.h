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

#ifndef QTMS_G711_FORMAT_H
#define QTMS_G711_FORMAT_H

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
 * QTMSG711Format class
 *
 * This class provides access methods to configure and query G.711 codec
 * format settings.
 *
 * This format class can be used for both the encoder and decoder.
 *
 * @lib QTMSapi.lib
 *
 */
class QTMS_WRAPPER_DLL_EXPORT QTMSG711Format: public QObject, public QTMSFormat
{
Q_OBJECT
public:
    /**
     * Destructor
     */
    virtual ~QTMSG711Format();

    /**
     * Set G.711 codec mode.
     *
     * This method is allowable when encoding is not active; i.e. anytime
     * before encoding is started or after it is stopped.
     *
     * Supported G.711 codec modes are:
     *  QTMS_G711_CODEC_MODE_ALAW
     *  QTMS_G711_CODEC_MODE_MULAW
     *
     * @param  mode
     *      Sets selected G.711 encoding or decoding mode.

     * @return
     *      QTMS_RESULT_SUCCESS if the operation was successful.
     *      QTMS_RESULT_INVALID_STATE if this method is used during active
     *      encoding or decoding operation.
     *      QTMS_RESULT_FEATURE_NOT_SUPPORTED if this method is not implemented
     *      by the adaptation.
     *
     */
    gint SetMode(const QTMSG711CodecMode mode);

    /**
     * Return current G.711 codec mode.
     *
     * This method is allowable at all times, while actively encoding/decoding
     * or not.
     *
     * Supported G.711 codec modes are:
     *  QTMS_G711_CODEC_MODE_ALAW
     *  QTMS_G711_CODEC_MODE_MULAW
     *
     * @param  mode
     *      Returns current G.711 encoding or decoding mode.

     * @return
     *      QTMS_RESULT_SUCCESS if the operation was successful.
     *
     */
    gint GetMode(QTMSG711CodecMode& mode);

    /**
     * Toggles on and off Comfort Noise Generator (CNG) for G.711 decoder.
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
     *
     */
    gint SetCNG(const gboolean cng);

    /**
     * Returns current CNG setting of the G.711 decoder.
     *
     * This method is allowable when decoding is not active; i.e. anytime
     * before decoding is started or after it is stopped.
     *
     * @param  cng
     *      Current CNG setting of the decoder.
     *
     * @return gint
     *      QTMS_RESULT_SUCCESS if the operation was successful.
     *
     */
    gint GetCNG(gboolean& cng);

    /**
     * Toggles on and off Packet Loss Concealment (PLC) for G.711 decoder.
     * Not supported by the encoder.
     *
     * This method is allowable when decoding is not active; i.e. anytime
     * before decoding is started or after it is stopped.
     *
     * @param  plc
     *      Sets PLC feature on or off.
     *
     * @return
     *      QTMS_RESULT_SUCCESS if the operation was successful.
     *      QTMS_RESULT_INVALID_STATE if this method is used during active
     *      decoding operation.
     *      QTMS_RESULT_FEATURE_NOT_SUPPORTED if this method is not implemented
     *      by the adaptation.
     *
     */
    gint SetPlc(const gboolean plc);

    /**
     * Returns current PLC setting of the G.711 decoder.
     *
     * This method is allowable at all times, while actively decoding or not.
     *
     * @param  plc
     *      Current CNG setting of the decoder.
     *
     * @return
     *      QTMS_RESULT_SUCCESS if the operation was successful.
     *
     */
    gint GetPlc(gboolean& plc);

    /**
     * Toggles on and off Voice Activation Detection (VAD) for G.711 encoder.
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
     *
     */
    gint SetVADMode(const gboolean vad);

    /**
     * Returns current VAD setting of the G.711 encoder.
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
    QTMSG711Format();
};

} //namespace QTMS

#endif //QTMS_G711_FORMAT_H
