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

#include "tmsinbandtonebody.h"
#include "tmsinbandtone.h"
#include "tmsinbandtoneobsrvr.h"

using namespace TMS;

EXPORT_C TMSInbandTone::TMSInbandTone(void) :
    iBody(NULL)
    {
    }

EXPORT_C TMSInbandTone::~TMSInbandTone(void)
    {
    delete iBody;
    }

EXPORT_C gint TMSInbandTone::AddObserver(TMSInbandToneObserver& obsrvr,
        gpointer user_data)
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->AddObserver(obsrvr, user_data);
        }
    return status;
    }

EXPORT_C gint TMSInbandTone::RemoveObserver(TMSInbandToneObserver& obsrvr)
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->RemoveObserver(obsrvr);
        }
    return status;
    }

EXPORT_C gint TMSInbandTone::Start(TMSInbandToneType inbandtonetype)
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->Start(inbandtonetype);
        }
    return status;
    }

EXPORT_C gint TMSInbandTone::Stop()
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->Stop();
        }
    return status;
    }

