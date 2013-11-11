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

#include <tmsglobalrouting.h>
#include <tmsglobalroutingobsrvr.h>
#include "tmsglobalroutingbody.h"

using namespace TMS;

EXPORT_C TMSGlobalRouting::TMSGlobalRouting() :
    iBody(NULL)
    {
    }

EXPORT_C TMSGlobalRouting::~TMSGlobalRouting()
    {
    delete iBody;
    }

EXPORT_C gint TMSGlobalRouting::AddObserver(TMSGlobalRoutingObserver& obsrvr,
        gpointer user_data)
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->AddObserver(obsrvr, user_data);
        }
    return status;
    }

EXPORT_C gint TMSGlobalRouting::RemoveObserver(
        TMSGlobalRoutingObserver& obsrvr)
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->RemoveObserver(obsrvr);
        }
    return status;
    }

EXPORT_C gint TMSGlobalRouting::SetOutput(const TMSAudioOutput output)
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->SetOutput(output);
        }
    return status;
    }

EXPORT_C gint TMSGlobalRouting::GetOutput(TMSAudioOutput& output)
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->GetOutput(output);
        }
    return status;
    }

EXPORT_C gint TMSGlobalRouting::GetPreviousOutput(TMSAudioOutput& output)
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->GetPreviousOutput(output);
        }
    return status;
    }

EXPORT_C gint TMSGlobalRouting::GetAvailableOutputs(OutputVector& outputs)
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->GetAvailableOutputs(outputs);
        }
    return status;
    }

// End of file
