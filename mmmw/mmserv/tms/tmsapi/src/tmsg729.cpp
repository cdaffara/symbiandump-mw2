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

#include <tmsg729format.h>
#include "tmsg729formatbody.h"
#include "tmsutility.h"

using namespace TMS;

EXPORT_C TMSG729Format::TMSG729Format() :
    iBody(NULL)
    {
    }

EXPORT_C TMSG729Format::~TMSG729Format()
    {
    // iBody is deleted with TMSFormat::iFormatBody
    }

EXPORT_C gint TMSG729Format::SetVADMode(const gboolean aVad)
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->SetVADMode(aVad);
        }
    return status;
    }

EXPORT_C gint TMSG729Format::GetVADMode(gboolean& aVad)
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->GetVADMode(aVad);
        }
    return status;
    }

// End of file
