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

#ifndef QTMS_FORMAT_H
#define QTMS_FORMAT_H

#include <qtms.h>
#include "qtmswrapperexport.h"

namespace TMS
{
class TMSFormat;
}

namespace QTMS
{

/**
 * QTMSFormat class
 *
 * This is base class for codec format classes. Provides access methods to
 * configure and query common codec format settings.
 *
 * This format class can be used for both the encoder and decoder.
 *
 * Usage:
 * <code>
 *  QTMSFactory* iFactory;
 *  QTMSFormat* iFormat
 *  gint err;
 *  err = QTMSFactory::CreateFactory(iFactory);
 *  err = iFactory->Create(QTMS_FORMAT_XXX,iFormat);
 *  ...
 * </code>
 *
 * @lib QTMSapi.lib
 *
 */
class QTMS_WRAPPER_DLL_EXPORT QTMSFormat
{
public:
    /**
     * Destructor
     */
    virtual ~QTMSFormat();

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
     *      QTMS_RESULT_SUCCESS if the operation was successful.
     *      QTMS_RESULT_FEATURE_NOT_SUPPORTED if this method is not implemented
     *      by the adaptation.
     *
     */
    gint GetSupportedBitRates(BitRateVector& bitrates);

    /**
     * Set bitrate.
     *
     * This method is allowable at all times, while actively encoding or not.
     *
     * @param  bitrate
     *      Bitrate to be set for the encoder.
     *
     * @return
     *      QTMS_RESULT_SUCCESS if the operation was successful.
     *      QTMS_RESULT_INVALID_ARGUMENT if requested bitrate is invalid.
     *      QTMS_RESULT_FEATURE_NOT_SUPPORTED if this method is not implemented
     *      by the adaptation.
     *
     */
    gint SetBitRate(const guint bitrate);

    /**
     * Get current encoder's bitrate.
     *
     * This method is allowable at all times, while actively encoding or not.
     *
     * @param  bitrate
     *      Current bitrate.
     *
     * @return
     *      QTMS_RESULT_SUCCESS if the operation was successful.
     *      QTMS_RESULT_FEATURE_NOT_SUPPORTED if this method is not implemented
     *      by the adaptation.
     *
     */
    gint GetBitRate(guint& bitrate);

    /**
     * Return format type.
     *
     * @param  fmttype
     *      Type of the format object.
     *
     * @return
     *      QTMS_RESULT_SUCCESS if the operation was successful.
     *
     */
    virtual gint GetType(QTMSFormatType& fmttype);

protected:
    /**
     * Constructor
     */
    QTMSFormat();

protected:
    TMS::TMSFormat* iFormat;
};

} //namespace QTMS

#endif //QTMS_FORMAT_H
