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

#ifndef TMS_G711_FORMAT_H
#define TMS_G711_FORMAT_H

#include <tms.h>
#include <tmsformat.h>

namespace TMS {

// FORWARD DECLARATIONS
class TMSG711FormatBody;

/**
 * TMSG711Format class
 *
 * This class provides access methods to configure and query G.711 codec
 * format settings.
 *
 * This format class can be used for both the encoder and decoder.
 *
 * @lib tmsapi.lib
 *
 */
class TMSG711Format : public TMSFormat
    {
public:
    /**
     * Destructor
     */
    IMPORT_C virtual ~TMSG711Format();

    /**
     * Set G.711 codec mode.
     *
     * This method is allowable when encoding is not active; i.e. anytime
     * before encoding is started or after it is stopped.
     *
     * Supported G.711 codec modes are:
     *  TMS_G711_CODEC_MODE_ALAW
     *  TMS_G711_CODEC_MODE_MULAW
     *
     * @param  mode
     *      Sets selected G.711 encoding or decoding mode.

     * @return
     *      TMS_RESULT_SUCCESS if the operation was successful otherwise
     *      system error.
     *      TMS_RESULT_INVALID_STATE if this method is used during active
     *      encoding or decoding operation.
     *      TMS_RESULT_FEATURE_NOT_SUPPORTED if this method is not implemented
     *      by the adaptation.
     *
     */
    IMPORT_C gint SetMode(const TMSG711CodecMode mode);

    /**
     * Return current G.711 codec mode.
     *
     * This method is allowable at all times, while actively encoding/decoding
     * or not.
     *
     * Supported G.711 codec modes are:
     *  TMS_G711_CODEC_MODE_ALAW
     *  TMS_G711_CODEC_MODE_MULAW
     *
     * @param  mode
     *      Returns current G.711 encoding or decoding mode.

     * @return
     *      TMS_RESULT_SUCCESS if the operation was successful otherwise
     *      system error.
     *
     */
    IMPORT_C gint GetMode(TMSG711CodecMode& mode);

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
     *      TMS_RESULT_SUCCESS if the operation was successful otherwise
     *      system error.
     *      TMS_RESULT_INVALID_STATE if this method is used during active
     *      decoding operation.
     *      TMS_RESULT_FEATURE_NOT_SUPPORTED if this method is not implemented
     *      by the adaptation.
     *
     */
    IMPORT_C gint SetCNG(const gboolean cng);

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
     *      TMS_RESULT_SUCCESS if the operation was successful otherwise system error..
     *
     */
    IMPORT_C gint GetCNG(gboolean& cng);

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
     *      TMS_RESULT_SUCCESS if the operation was successful otherwise system error..
     *      TMS_RESULT_INVALID_STATE if this method is used during active
     *      decoding operation.
     *      TMS_RESULT_FEATURE_NOT_SUPPORTED if this method is not implemented
     *      by the adaptation.
     *
     */
    IMPORT_C gint SetPlc(const gboolean plc);

    /**
     * Returns current PLC setting of the G.711 decoder.
     *
     * This method is allowable at all times, while actively decoding or not.
     *
     * @param  plc
     *      Current CNG setting of the decoder.
     *
     * @return
     *      TMS_RESULT_SUCCESS if the operation was successful otherwise
     *      system error.
     *
     */
    IMPORT_C gint GetPlc(gboolean& plc);

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
     *      TMS_RESULT_SUCCESS if the operation was successful otherwise
     *      system error.
     *      TMS_RESULT_FEATURE_NOT_SUPPORTED if this method is not implemented
     *      by the adaptation.
     *
     */
    IMPORT_C gint SetVADMode(const gboolean vad);

    /**
     * Returns current VAD setting of the G.711 encoder.
     *
     * This method is allowable at all times, while actively encoding or not.
     *
     * @param  vad
     *      Current VAD setting of the encoder.
     *
     * @return
     *      TMS_RESULT_SUCCESS if the operation was successful otherwise
     *      system error.
     *
     */
    IMPORT_C gint GetVADMode(gboolean& vad);

protected:
    /**
     * Constructor
     */
    IMPORT_C TMSG711Format();

protected:
    TMSG711FormatBody *iBody;
    };

} //namespace TMS

#endif //TMS_G711_FORMAT_H
