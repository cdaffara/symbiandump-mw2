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

#include <tmsgaineffect.h>
#include <tmseffectobsrvr.h>
#include "tmsgaineffectbody.h"

using namespace TMS;

EXPORT_C TMSGainEffect::TMSGainEffect() :
    iBody(NULL)
    {
    }

EXPORT_C TMSGainEffect::~TMSGainEffect()
    {
    delete iBody;
    }

EXPORT_C gint TMSGainEffect::AddObserver(TMSEffectObserver& obsrvr,
        gpointer user_data)
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->AddObserver(obsrvr, user_data);
        }
    return status;
    }

EXPORT_C gint TMSGainEffect::RemoveObserver(TMSEffectObserver& obsrvr)
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->RemoveObserver(obsrvr);
        }
    return status;
    }

EXPORT_C gint TMSGainEffect::GetLevel(guint& level)
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->GetLevel(level);
        }
    return status;
    }

EXPORT_C gint TMSGainEffect::SetLevel(const guint level)
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->SetLevel(level);
        }
    return status;
    }

EXPORT_C gint TMSGainEffect::GetMaxLevel(guint& level)
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->GetMaxLevel(level);
        }
    return status;
    }

EXPORT_C gint TMSGainEffect::GetType(TMSSourceType& sourcetype)
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->GetType(sourcetype);
        }
    return status;
    }

// End of file
