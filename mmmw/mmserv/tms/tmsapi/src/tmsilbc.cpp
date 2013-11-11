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

#include <tmsilbcformat.h>
#include "tmsilbcformatbody.h"
#include "tmsutility.h"

using namespace TMS;

EXPORT_C TMSILBCFormat::TMSILBCFormat() :
    iBody(NULL)
    {
    }

EXPORT_C TMSILBCFormat::~TMSILBCFormat()
    {
    // iBody is deleted with TMSFormat::iFormatBody
    }

EXPORT_C gint TMSILBCFormat::SetMode(const TMSG711CodecMode aMode)
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->SetMode(aMode);
        }
    return status;
    }

EXPORT_C gint TMSILBCFormat::GetMode(TMSG711CodecMode& aMode)
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->GetMode(aMode);
        }
    return status;
    }

EXPORT_C gint TMSILBCFormat::SetCNG(const gboolean aCng)
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->SetCNG(aCng);
        }
    return status;
    }

EXPORT_C gint TMSILBCFormat::GetCNG(gboolean& aCng)
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->GetCNG(aCng);
        }
    return status;
    }

EXPORT_C gint TMSILBCFormat::SetVADMode(const gboolean aVad)
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->SetVADMode(aVad);
        }
    return status;
    }

EXPORT_C gint TMSILBCFormat::GetVADMode(gboolean& aVad)
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->GetVADMode(aVad);
        }
    return status;
    }

// End of file
