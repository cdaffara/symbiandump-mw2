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
#include "tmsipcallbodyimpl.h"
#include "tmscscallbodyimpl.h"
#include "tmscallimpl.h"

using namespace TMS;

TMSCallImpl::TMSCallImpl()
    {
    TRACE_PRN_FN_ENT;
    TRACE_PRN_FN_EXT;
    }

TMSCallImpl::~TMSCallImpl()
    {
    TRACE_PRN_FN_ENT;
    TRACE_PRN_FN_EXT;
    }

gint TMSCallImpl::PostConstruct(TMSCallType ctype, guint /*ctxid*/)
    {
    TRACE_PRN_FN_ENT;
    gint ret(TMS_RESULT_INSUFFICIENT_MEMORY);
    TMSCallBody* tmscallimplbody(NULL);

    switch (ctype)
        {
        case TMS_CALL_IP:
            ret = TMSIPCallBodyImpl::Create(tmscallimplbody);
            break;
        case TMS_CALL_CS:
            ret = TMSCSCallBodyImpl::Create(tmscallimplbody);
            break;
        default:
            ret = TMS_RESULT_CALL_TYPE_NOT_SUPPORTED;
            break;
        };

    if (ret == TMS_RESULT_SUCCESS)
        {
        this->iBody = tmscallimplbody;
        }
    TRACE_PRN_FN_EXT;
    return ret;
    }

EXPORT_C gint TMSCallImpl::Create(TMSCallType ctype, TMSCall*& tmscall,
        guint ctxid)
    {
    TRACE_PRN_FN_ENT;
    gint ret(TMS_RESULT_INSUFFICIENT_MEMORY);
    TMSCallImpl *self = new TMSCallImpl();

    if (self)
        {
        ret = self->PostConstruct(ctype, ctxid);
        if (ret != TMS_RESULT_SUCCESS)
            {
            delete self;
            self = NULL;
            }
        }
    tmscall = self;
    TRACE_PRN_FN_EXT;
    return ret;
    }

EXPORT_C gint TMSCallImpl::Delete(TMSCall*& tmscall)
    {
    TRACE_PRN_FN_ENT;
    gint ret(TMS_RESULT_INSUFFICIENT_MEMORY);
    delete (static_cast<TMSCallImpl*>(tmscall));
    tmscall = NULL;
    TRACE_PRN_FN_EXT;
    return ret;
    }

