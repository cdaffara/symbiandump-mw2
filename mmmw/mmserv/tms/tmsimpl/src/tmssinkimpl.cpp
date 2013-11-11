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

#include "tmsutility.h"
#include "tmsclientsinkimpl.h"
#include "tmsmodemsinkimpl.h"
#include "tmsspeakersinkimpl.h"
#include "tmssinkimpl.h"

using namespace TMS;

EXPORT_C gint TMSSinkImpl::Create(TMSSinkType sinktype, TMSSink*& tmssink)
    {
    gint ret(TMS_RESULT_INSUFFICIENT_MEMORY);
    TRACE_PRN_FN_ENT;
    switch (sinktype)
        {
        case TMS_SINK_CLIENT:
            ret = TMSClientSinkImpl::Create(tmssink);
            break;
        case TMS_SINK_MODEM:
            ret = TMSModemSinkImpl::Create(tmssink);
            break;
        case TMS_SINK_SPEAKER:
            ret = TMSSpeakerSinkImpl::Create(tmssink);
            break;
        default:
            ret = TMS_RESULT_SINK_TYPE_NOT_SUPPORTED;
            break;
        }

    TRACE_PRN_FN_EXT;
    return ret;
    }

EXPORT_C gint TMSSinkImpl::Delete(TMSSink*& tmssink)
    {
    gint ret(TMS_RESULT_INVALID_ARGUMENT);
    TRACE_PRN_FN_ENT;
    TMSSinkType sinktype;
    ret = tmssink->GetType(sinktype);
    switch (sinktype)
        {
        case TMS_SINK_CLIENT:
            {
            delete (static_cast<TMSClientSinkImpl*>(tmssink));
            tmssink = NULL;
            ret = TMS_RESULT_SUCCESS;
            }
            break;
        case TMS_SINK_MODEM:
            {
            delete (static_cast<TMSModemSinkImpl*>(tmssink));
            tmssink = NULL;
            ret = TMS_RESULT_SUCCESS;
            }
            break;
        case TMS_SINK_SPEAKER:
            {
            delete (static_cast<TMSSpeakerSinkImpl*>(tmssink));
            tmssink = NULL;
            ret = TMS_RESULT_SUCCESS;
            }
            break;
        default:
            ret = TMS_RESULT_SINK_TYPE_NOT_SUPPORTED;
            break;
        }

    TRACE_PRN_FN_EXT;
    return ret;
    }

// End of file
