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

#include <tmsclientsink.h>
#include <tmsclientsinkobsrvr.h>
#include "tmsclientsinkbody.h"

using namespace TMS;

EXPORT_C TMSClientSink::TMSClientSink() :
    iBody(NULL)
    {
    }

EXPORT_C TMSClientSink::~TMSClientSink()
    {
    delete iBody;
    }

EXPORT_C gint TMSClientSink::AddObserver(TMSClientSinkObserver& obsrvr,
        gpointer user_data)
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->AddObserver(obsrvr, user_data);
        }
    return status;
    }

EXPORT_C gint TMSClientSink::RemoveObserver(TMSClientSinkObserver& obsrvr)
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->RemoveObserver(obsrvr);
        }
    return status;
    }

// Push mode
EXPORT_C gint TMSClientSink::BufferProcessed(TMSBuffer* buffer)
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->BufferProcessed(buffer);
        }
    return status;
    }

EXPORT_C gint TMSClientSink::GetType(TMSSinkType& sinktype)
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->GetType(sinktype);
        }
    return status;
    }

// End of file
