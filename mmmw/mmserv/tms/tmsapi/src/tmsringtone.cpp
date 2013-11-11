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

#include <tmsringtoneobsrvr.h>
#include <tmsringtone.h>
#include "tmsringtonebody.h"

using namespace TMS;

EXPORT_C TMSRingTone::TMSRingTone() :
    iBody(NULL)
    {
    }

EXPORT_C TMSRingTone::~TMSRingTone()
    {
    delete iBody;
    }

EXPORT_C gint TMSRingTone::AddObserver(TMSRingToneObserver& obsrvr,
        gpointer user_data)
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->AddObserver(obsrvr, user_data);
        }
    return status;
    }

EXPORT_C gint TMSRingTone::RemoveObserver(TMSRingToneObserver& obsrvr)
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->RemoveObserver(obsrvr);
        }
    return status;
    }

EXPORT_C gint TMSRingTone::Init(const TMSRingToneType type, GString* str,
        GString* tts)
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->Init(type, str, tts);
        }
    return status;
    }

EXPORT_C gint TMSRingTone::Deinit()
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->Deinit();
        }
    return status;
    }

EXPORT_C gint TMSRingTone::Play()
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->Play();
        }
    return status;
    }

EXPORT_C gint TMSRingTone::Stop()
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->Stop();
        }
    return status;
    }

EXPORT_C gint TMSRingTone::Pause()
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->Pause();
        }
    return status;
    }

EXPORT_C gint TMSRingTone::Mute()
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->Mute();
        }
    return status;
    }

// End of file
