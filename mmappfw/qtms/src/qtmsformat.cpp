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

#include <tmsformat.h>
#include <qtmsformat.h>

using namespace QTMS;

QTMSFormat::QTMSFormat() :
    iFormat(NULL)
{
}

QTMSFormat::~QTMSFormat()
{
    delete iFormat;
}

gint QTMSFormat::GetSupportedBitRates(BitRateVector& aVector)
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iFormat) {
        status = iFormat->GetSupportedBitRates(aVector);
    }
    return status;
}

gint QTMSFormat::SetBitRate(const guint aBitrate)
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iFormat) {
        status = iFormat->SetBitRate(aBitrate);
    }
    return status;
}

gint QTMSFormat::GetBitRate(guint& aBitrate)
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iFormat) {
        status = iFormat->GetBitRate(aBitrate);
    }
    return status;
}

gint QTMSFormat::GetType(QTMSFormatType& fmttype)
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iFormat) {
        status = iFormat->GetType(fmttype);
    }
    return status;
}

// End of file
