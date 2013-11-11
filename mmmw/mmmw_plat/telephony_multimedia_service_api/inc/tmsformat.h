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

#ifndef TMS_FORMAT_H
#define TMS_FORMAT_H

#include <tms.h>

namespace TMS {

// FORWARD DECLARATIONS
class TMSFormatBody;

/**
 * TMSFormat class
 *
 * This is base class for codec format classes. Provides access methods to
 * configure and query common codec format settings.
 *
 * This format class can be used for both the encoder and decoder.
 *
 * Usage:
 * <code>
 *  TMSFactory* iFactory;
 *  TMSFormat* iFormat
 *  gint err;
 *  err = TMSFactory::CreateFactory(iFactory);
 *  err = iFactory->Create(TMS_FORMAT_XXX,iFormat);
 *  ...
 * </code>
 *
 * @lib tmsapi.lib
 *
 */
class TMSFormat
    {
public:
    /**
     * Destructor
     */
    IMPORT_C virtual ~TMSFormat();

    /**
     * Get supported bitrates. Returns list of supported bitrates for the
     * selected encoder
     *
     * This method is allowable at all times, while actively encoding or not.
     *
     * @param  bitrates
     *      The typedef std::vector<guint> BitRateVector; list of codecs.
     *
     * @return
     *      TMS_RESULT_SUCCESS if the operation was successful otherwise
     *      system error.
     *      TMS_RESULT_FEATURE_NOT_SUPPORTED if this method is not implemented
     *      by the adaptation.
     *
     */
    IMPORT_C gint GetSupportedBitRates(BitRateVector& bitrates);

    /**
     * Set bitrate.
     *
     * This method is allowable at all times, while actively encoding or not.
     *
     * @param  bitrate
     *      Bitrate to be set for the encoder.
     *
     * @return
     *      TMS_RESULT_SUCCESS if the operation was successful otherwise
     *      system error.
     *      TMS_RESULT_INVALID_ARGUMENT if requested bitrate is invalid.
     *      TMS_RESULT_FEATURE_NOT_SUPPORTED if this method is not implemented
     *      by the adaptation.
     *
     */
    IMPORT_C gint SetBitRate(const guint bitrate);

    /**
     * Get current encoder's bitrate.
     *
     * This method is allowable at all times, while actively encoding or not.
     *
     * @param  bitrate
     *      Current bitrate.
     *
     * @return
     *      TMS_RESULT_SUCCESS if the operation was successful otherwise
     *      system error.
     *      TMS_RESULT_FEATURE_NOT_SUPPORTED if this method is not implemented
     *      by the adaptation.
     *
     */
    IMPORT_C gint GetBitRate(guint& bitrate);

    /**
     * Return format type.
     *
     * @param  fmttype
     *      Type of the format object.
     *
     * @return
     *      TMS_RESULT_SUCCESS if the operation was successful otherwise
     *      system error.
     *
     */
    IMPORT_C virtual gint GetType(TMSFormatType& fmttype);

protected:
    /**
     * Constructor
     */
    IMPORT_C TMSFormat();

protected:
    TMSFormatBody *iFormatBody;
    };

} //namespace TMS

#endif //TMS_FORMAT_H
