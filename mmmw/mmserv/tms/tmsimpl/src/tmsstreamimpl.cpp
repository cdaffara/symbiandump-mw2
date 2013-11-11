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
#include "tmsstreambodyimpl.h"
#include "tmsstreamimpl.h"

using namespace TMS;

TMSStreamImpl::TMSStreamImpl()
    {
    TRACE_PRN_FN_ENT;
    TRACE_PRN_FN_EXT;
    }

TMSStreamImpl::~TMSStreamImpl()
    {
    TRACE_PRN_FN_ENT;
    TRACE_PRN_FN_EXT;
    }

gint TMSStreamImpl::PostConstruct(TMSCallType callType, TMSStreamType stype,
        TMSCallProxy* proxy)
    {
    TRACE_PRN_FN_ENT;
    gint ret(TMS_RESULT_INSUFFICIENT_MEMORY);
    TMSStreamBody* bodyimpl(NULL);
    ret = TMSStreamBodyImpl::Create(callType, stype, proxy, *this, bodyimpl);

    if (ret == TMS_RESULT_SUCCESS)
        {
        this->iBody = bodyimpl;
        }
    TRACE_PRN_FN_EXT;
    return ret;
    }

gint TMSStreamImpl::Create(TMSCallType callType, TMSStreamType stype,
        TMSCallProxy* proxy, TMSStream*& strm)
    {
    TRACE_PRN_FN_ENT;
    gint ret(TMS_RESULT_INSUFFICIENT_MEMORY);
    TMSStreamImpl *self = new TMSStreamImpl();

    if (self)
        {
        ret = self->PostConstruct(callType, stype, proxy);
        if (ret != TMS_RESULT_SUCCESS)
            {
            delete self;
            self = NULL;
            }
        }
    strm = self;
    TRACE_PRN_FN_EXT;
    return ret;
    }

gint TMSStreamImpl::Delete(TMSStream*& strm)
    {
    TRACE_PRN_FN_ENT;
    gint ret(TMS_RESULT_SUCCESS);
    delete (static_cast<TMSStreamImpl*> (strm));
    strm = NULL;
    TRACE_PRN_FN_EXT;
    return ret;
    }

