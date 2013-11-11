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

#include <tmsformat.h>
#include "tmsutility.h"
#include "tmsformatbody.h"

using namespace TMS;

EXPORT_C TMSFormat::TMSFormat() :
    iFormatBody(NULL)
    {
    }

EXPORT_C TMSFormat::~TMSFormat()
    {
    delete iFormatBody;
    }

EXPORT_C gint TMSFormat::GetSupportedBitRates(BitRateVector& aVector)
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iFormatBody)
        {
        status = iFormatBody->GetSupportedBitRates(aVector);
        }
    return status;
    }

EXPORT_C gint TMSFormat::SetBitRate(const guint aBitrate)
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iFormatBody)
        {
        status = iFormatBody->SetBitRate(aBitrate);
        }
    return status;
    }

EXPORT_C gint TMSFormat::GetBitRate(guint& aBitrate)
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iFormatBody)
        {
        status = iFormatBody->GetBitRate(aBitrate);
        }
    return status;
    }

EXPORT_C gint TMSFormat::GetType(TMSFormatType& formatType)
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iFormatBody)
        {
        status = iFormatBody->GetType(formatType);
        }
    return status;
    }

// End of file
