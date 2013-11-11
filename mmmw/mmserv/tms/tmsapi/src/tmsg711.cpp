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

#include <tmsg711format.h>
#include "tmsg711formatbody.h"
#include "tmsutility.h"

using namespace TMS;

EXPORT_C TMSG711Format::TMSG711Format() :
    iBody(NULL)
    {
    }

EXPORT_C TMSG711Format::~TMSG711Format()
    {
    // iBody is deleted with TMSFormat::iFormatBody
    }

EXPORT_C gint TMSG711Format::SetMode(const TMSG711CodecMode aMode)
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->SetMode(aMode);
        }
    return status;
    }

EXPORT_C gint TMSG711Format::GetMode(TMSG711CodecMode& aMode)
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->GetMode(aMode);
        }
    return status;
    }

EXPORT_C gint TMSG711Format::SetCNG(const gboolean aCng)
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->SetCNG(aCng);
        }
    return status;
    }

EXPORT_C gint TMSG711Format::GetCNG(gboolean& aCng)
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->GetCNG(aCng);
        }
    return status;
    }

EXPORT_C gint TMSG711Format::SetPlc(const gboolean aPlc)
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->SetPlc(aPlc);
        }
    return status;
    }

EXPORT_C gint TMSG711Format::GetPlc(gboolean& aPlc)
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->GetPlc(aPlc);
        }
    return status;
    }

EXPORT_C gint TMSG711Format::SetVADMode(const gboolean aVad)
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->SetVADMode(aVad);
        }
    return status;
    }

EXPORT_C gint TMSG711Format::GetVADMode(gboolean& aVad)
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->GetVADMode(aVad);
        }
    return status;
    }

// End of file
