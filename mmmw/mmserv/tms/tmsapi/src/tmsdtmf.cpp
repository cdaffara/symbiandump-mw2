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

#include "tmsdtmfbody.h"
#include "tmsdtmf.h"
#include "tmsdtmfobsrvr.h"

using namespace TMS;


EXPORT_C TMSDTMF::TMSDTMF(void) :
    iBody(NULL)
    {
    }

EXPORT_C TMSDTMF::~TMSDTMF(void)
    {
    delete iBody;
    }

EXPORT_C gint TMSDTMF::AddObserver(TMSDTMFObserver& obsrvr, gpointer user_data)
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->AddObserver(obsrvr, user_data);
        }
    return status;
    }

EXPORT_C gint TMSDTMF::RemoveObserver(TMSDTMFObserver& obsrvr)
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->RemoveObserver(obsrvr);
        }
    return status;
    }

EXPORT_C gint TMSDTMF::Start()
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->Start();
        }
    return status;
    }

EXPORT_C gint TMSDTMF::Stop()
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->Stop();
        }
    return status;
    }

EXPORT_C gint TMSDTMF::SetTone(GString* string)
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->SetTone(string);
        }
    return status;
    }

EXPORT_C gint TMSDTMF::ContinueDTMFStringSending(gboolean aContinue)
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->ContinueDTMFStringSending(aContinue);
        }
    return status;
    }
