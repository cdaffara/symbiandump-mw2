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

#include <tmsclientsource.h>
#include <tmsclientsourceobsrvr.h>
#include "tmsclientsourcebody.h"

using namespace TMS;

EXPORT_C TMSClientSource::TMSClientSource() :
    iBody(NULL)
    {
    }

EXPORT_C TMSClientSource::~TMSClientSource()
    {
    delete iBody;
    }

EXPORT_C gint TMSClientSource::AddObserver(TMSClientSourceObserver& obsrvr,
        gpointer user_data)
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->AddObserver(obsrvr, user_data);
        }
    return status;
    }

EXPORT_C gint TMSClientSource::RemoveObserver(TMSClientSourceObserver& obsrvr)
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->RemoveObserver(obsrvr);
        }
    return status;
    }

// In pull mode, client calls this.??? How to identify last buffer.
// Option 1 is to move setlast buffer to TMSBuffer interface.
// Option 2 is to have overloaded function with another parameter.
EXPORT_C gint TMSClientSource::BufferFilled(TMSBuffer& buffer)
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->BufferFilled(buffer);
        }
    return status;
    }

// Push mode
EXPORT_C gint TMSClientSource::ProcessBuffer(TMSBuffer* buffer)
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->ProcessBuffer(buffer);
        }
    return status;
    }

// Indicates framework to queue ProcessBuffer. default is off
// unsupported in pull mode??? (atleast initially)
EXPORT_C gint TMSClientSource::SetEnqueueMode(const gboolean enable)
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->SetEnqueueMode(enable);
        }
    return status;
    }

EXPORT_C gint TMSClientSource::GetEnqueueMode(gboolean& enable)
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->GetEnqueueMode(enable);
        }
    return status;
    }

// Sends batch data to framework and clears queue mode.
// Valid only when queue mode is set, otherwise no-op
EXPORT_C gint TMSClientSource::Flush()
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->Flush();
        }
    return status;
    }

EXPORT_C gint TMSClientSource::GetType(TMSSourceType& sourcetype)
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->GetType(sourcetype);
        }
    return status;
    }

// End of file
