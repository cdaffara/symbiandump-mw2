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

#include "tmsclientsourceimpl.h"
#include "tmsutility.h"
#include "tmssourceimpl.h"
#include "tmsmodemsourceimpl.h"
#include "tmsmicsourceimpl.h"

using namespace TMS;

EXPORT_C gint TMSSourceImpl::Create(TMSSourceType sourcetype,
        TMSSource*& tmssource)
    {
    gint ret(TMS_RESULT_INSUFFICIENT_MEMORY);
    TRACE_PRN_FN_ENT;
    switch (sourcetype)
        {
        case TMS_SOURCE_CLIENT:
            ret = TMSClientSourceImpl::Create(tmssource);
            break;
        case TMS_SOURCE_MODEM:
            ret = TMSModemSourceImpl::Create(tmssource);
            break;
        case TMS_SOURCE_MIC:
            ret = TMSMicSourceImpl::Create(tmssource);
            break;
        default:
            ret = TMS_RESULT_SOURCE_TYPE_NOT_SUPPORTED;
            break;
        }

    TRACE_PRN_FN_EXT;
    return ret;
    }

EXPORT_C gint TMSSourceImpl::Delete(TMSSource*& tmssource)
    {
    gint ret(TMS_RESULT_INVALID_ARGUMENT);
    TRACE_PRN_FN_ENT;
    TMSSourceType sourcetype;
    ret = tmssource->GetType(sourcetype);
    switch (sourcetype)
        {
        case TMS_SOURCE_CLIENT:
            delete (static_cast<TMSClientSourceImpl*>(tmssource));
            tmssource = NULL;
            ret = TMS_RESULT_SUCCESS;
            break;
        case TMS_SOURCE_MODEM:
            {
            delete (static_cast<TMSModemSourceImpl*>(tmssource));
            tmssource = NULL;
            ret = TMS_RESULT_SUCCESS;
            }
            break;
        case TMS_SOURCE_MIC:
            {
            delete (static_cast<TMSMicSourceImpl*>(tmssource));
            tmssource = NULL;
            ret = TMS_RESULT_SUCCESS;
            }
            break;
        default:
            ret = TMS_RESULT_SOURCE_TYPE_NOT_SUPPORTED;
            break;
        }

    TRACE_PRN_FN_EXT;
    return ret;
    }

// End of file
