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

#include <tmsstream.h>
#include "tmsstreambody.h"

using namespace TMS;

EXPORT_C TMSStream::TMSStream(void) :
    iBody(NULL)
    {
    }

EXPORT_C TMSStream::~TMSStream()
    {
    delete iBody;
    }

EXPORT_C gint TMSStream::AddObserver(TMSStreamObserver& obsrvr,
        gpointer user_data)
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->AddObserver(obsrvr, user_data);
        }
    return status;
    }

EXPORT_C gint TMSStream::RemoveObserver(TMSStreamObserver& obsrvr)
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->RemoveObserver(obsrvr);
        }
    return status;
    }

EXPORT_C gint TMSStream::AddSource(TMSSource* source)
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->AddSource(source);
        }
    return status;
    }

EXPORT_C gint TMSStream::RemoveSource(TMSSource* source)
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->RemoveSource(source);
        }
    return status;
    }

EXPORT_C gint TMSStream::AddSink(TMSSink* sink)
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->AddSink(sink);
        }
    return status;
    }

EXPORT_C gint TMSStream::RemoveSink(TMSSink* sink)
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->RemoveSink(sink);
        }
    return status;
    }

EXPORT_C gint TMSStream::SetFormat(TMSFormat* format)
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->SetFormat(format);
        }
    return status;
    }

EXPORT_C gint TMSStream::ResetFormat(TMSFormat* format)
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->ResetFormat(format);
        }
    return status;
    }

EXPORT_C gint TMSStream::AddEffect(TMSEffect* effect)
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->AddEffect(effect);
        }
    return status;
    }

EXPORT_C gint TMSStream::RemoveEffect(TMSEffect* effect)
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->RemoveEffect(effect);
        }
    return status;
    }

EXPORT_C gint TMSStream::GetState()
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->GetState();
        }
    return status;
    }

EXPORT_C gint TMSStream::GetStreamType()
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->GetStreamType();
        }
    return status;
    }

EXPORT_C gint TMSStream::GetStreamId()
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->GetStreamId();
        }
    return status;
    }

EXPORT_C gint TMSStream::Init(gint retrytime)
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->Init(retrytime);
        }
    return status;
    }

EXPORT_C gint TMSStream::Pause()
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->Pause();
        }
    return status;
    }

EXPORT_C gint TMSStream::Start(gint retrytime)
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->Start(retrytime);
        }
    return status;
    }

EXPORT_C gint TMSStream::Stop()
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->Stop();
        }
    return status;
    }

EXPORT_C void TMSStream::Deinit()
    {
    if (iBody)
        {
        iBody->Deinit();
        }
    }

