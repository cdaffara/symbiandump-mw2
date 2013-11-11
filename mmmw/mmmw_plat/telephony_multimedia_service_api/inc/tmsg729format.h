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

#ifndef TMS_G729_FORMAT_H
#define TMS_G729_FORMAT_H

#include <tms.h>
#include <tmsformat.h>

namespace TMS {

// FORWARD DECLARATIONS
class TMSG729FormatBody;

/**
 * TMSG729Format class
 *
 * This class provides access methods to configure and query G.729 codec
 * format settings.
 *
 * This format class can be used for both the encoder and decoder.
 *
 * @lib tmsapi.lib
 *
 */
class TMSG729Format : public TMSFormat
    {
public:
    /**
     * Destructor
     */
    IMPORT_C virtual ~TMSG729Format();

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
     * Returns current VAD setting of the G.729 encoder.
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
    IMPORT_C TMSG729Format();

protected:
    TMSG729FormatBody *iBody;
    };

} //namespace TMS

#endif //TMS_G729_FORMAT_H
