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
#include "tmsdtmfbodyimpl.h"
#include "tmsdtmfimpl.h"

using namespace TMS;

TMSDTMFImpl::TMSDTMFImpl()
    {
    TRACE_PRN_FN_ENT;
    TRACE_PRN_FN_EXT;
    }

TMSDTMFImpl::~TMSDTMFImpl()
    {
    TRACE_PRN_FN_ENT;
    TRACE_PRN_FN_EXT;
    }

gint TMSDTMFImpl::PostConstruct(TMSStreamType streamtype)
    {
    TRACE_PRN_FN_ENT;
    gint ret(TMS_RESULT_INSUFFICIENT_MEMORY);
    TMSDTMFBody* bodyimpl(NULL);
    ret = TMSDTMFBodyImpl::Create(streamtype, *this, bodyimpl);

    if (ret == TMS_RESULT_SUCCESS)
        {
        this->iBody = bodyimpl;
        }
    TRACE_PRN_FN_EXT;
    return ret;
    }

EXPORT_C gint TMSDTMFImpl::Create(TMSStreamType streamtype, TMSDTMF*& dtmf)
    {
    TRACE_PRN_FN_ENT;
    gint ret(TMS_RESULT_INSUFFICIENT_MEMORY);
    TMSDTMFImpl *self = new TMSDTMFImpl();

    if (self)
        {
        ret = self->PostConstruct(streamtype);
        if (ret != TMS_RESULT_SUCCESS)
            {
            delete self;
            self = NULL;
            }
        }

    dtmf = self;
    TRACE_PRN_FN_EXT;
    return ret;
    }

EXPORT_C gint TMSDTMFImpl::Delete(TMSDTMF*& dtmf)
    {
    TRACE_PRN_FN_ENT;
    gint ret(TMS_RESULT_SUCCESS);
    delete (static_cast<TMSDTMFImpl*> (dtmf));
    dtmf = NULL;
    TRACE_PRN_FN_EXT;
    return ret;
    }

