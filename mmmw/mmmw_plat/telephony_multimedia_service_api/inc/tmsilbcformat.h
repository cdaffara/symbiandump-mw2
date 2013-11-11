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

#ifndef TMS_ILBC_FORMAT_H
#define TMS_ILBC_FORMAT_H

#include <tms.h>
#include <tmsformat.h>

namespace TMS {

// FORWARD DECLARATIONS
class TMSILBCFormatBody;

/**
 * TMSILBCFormat class
 *
 * This class provides access methods to configure and query iLBC codec
 * format settings.
 *
 * This format class can be used for both the encoder and decoder.
 *
 * @lib tmsapi.lib
 *
 */
class TMSILBCFormat : public TMSFormat
    {
public:
    /**
     * Destructor
     */
    IMPORT_C virtual ~TMSILBCFormat();

    /**
     * Set iLBC codec mode.
     *
     * This method is allowable when encoding is not active; i.e. anytime
     * before encoding is started or after it is stopped.
     *
     * Supported iLBC codec modes are:
     *  TMS_ILBC_CODEC_MODE_20MILLI_SEC_FRAME
     *  TMS_ILBC_CODEC_MODE_30MILLI_SEC_FRAME
     *
     * @param  mode
     *      Sets selected iLBC encoding or decoding mode.
     *
     * @return
     *      TMS_RESULT_SUCCESS if the operation was successful otherwise
     *      system error.
     *      TMS_RESULT_INVALID_STATE if this method is used during active
     *      encoding or decoding operation.
     *      TMS_RESULT_FEATURE_NOT_SUPPORTED if this method is not implemented
     *      by the adaptation.
     *      TMS_RESULT_DOES_NOT_EXIST if the custom interface handle does
     *      not exist.
     *
     */
    IMPORT_C gint SetMode(const TMSILBCCodecMode mode);

    /**
     * Return current iLBC codec mode.
     *
     * This method is allowable at all times, while actively encoding/decoding
     * or not.
     *
     * Supported iLBC codec modes are:
     *  TMS_ILBC_CODEC_MODE_20MILLI_SEC_FRAME
     *  TMS_ILBC_CODEC_MODE_30MILLI_SEC_FRAME
     *
     * @param  mode
     *      Returns current iLBC encoding or decoding mode.
     *
     * @return
     *      TMS_RESULT_SUCCESS if the operation was successful otherwise
     *      system error.
     *
     */
    IMPORT_C gint GetMode(TMSILBCCodecMode& mode);

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
     *      TMS_RESULT_SUCCESS if the operation was successful otherwise
     *      system error.
     *      TMS_RESULT_INVALID_STATE if this method is used during active
     *      decoding operation.
     *      TMS_RESULT_FEATURE_NOT_SUPPORTED if this method is not implemented
     *      by the adaptation.
     *      TMS_RESULT_DOES_NOT_EXIST if the custom interface handle does
     *      not exist.
     *
     */
    IMPORT_C gint SetCNG(const gboolean cng);

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
     *      TMS_RESULT_SUCCESS if the operation was successful otherwise
     *      system error.
     *
     */
    IMPORT_C gint GetCNG(gboolean& cng);

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
     *      TMS_RESULT_SUCCESS if the operation was successful otherwise
     *      system error.
     *      TMS_RESULT_FEATURE_NOT_SUPPORTED if this method is not implemented
     *      by the adaptation.
     *      TMS_RESULT_DOES_NOT_EXIST if the custom interface handle does
     *      not exist.
     *
     */
    IMPORT_C gint SetVADMode(const gboolean vad);

    /**
     * Returns current VAD setting of the iLBC encoder.
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
    IMPORT_C TMSILBCFormat();

protected:
    TMSILBCFormatBody *iBody;
    };

} //namespace TMS

#endif //TMS_ILBC_FORMAT_H
