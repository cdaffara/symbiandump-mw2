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
#include "tmsringtonebodyimpl.h"
#include "tmsringtoneimpl.h"

using namespace TMS;

TMSRingToneImpl::TMSRingToneImpl()
    {
    TRACE_PRN_FN_ENT;
    TRACE_PRN_FN_EXT;
    }

TMSRingToneImpl::~TMSRingToneImpl()
    {
    TRACE_PRN_FN_ENT;
    TRACE_PRN_FN_EXT;
    }

gint TMSRingToneImpl::PostConstruct()
    {
    TRACE_PRN_FN_ENT;
    gint ret(TMS_RESULT_INSUFFICIENT_MEMORY);
    TMSRingToneBody* bodyimpl(NULL);
    ret = TMSRingToneBodyImpl::Create(bodyimpl);

    if (ret == TMS_RESULT_SUCCESS)
        {
        this->iBody = bodyimpl;
        }
    TRACE_PRN_FN_EXT;
    return ret;
    }

gint TMSRingToneImpl::PostConstruct(RWindow& window, gint scrid)
    {
    TRACE_PRN_FN_ENT;
    gint ret(TMS_RESULT_INSUFFICIENT_MEMORY);
    TMSRingToneBody* bodyimpl(NULL);
    ret = TMSRingToneBodyImpl::Create(bodyimpl, window, scrid);

    if (ret == TMS_RESULT_SUCCESS)
        {
        this->iBody = bodyimpl;
        }
    TRACE_PRN_FN_EXT;
    return ret;
    }

EXPORT_C gint TMSRingToneImpl::Create(TMSRingTone*& rt)
    {
    TRACE_PRN_FN_ENT;
    gint ret(TMS_RESULT_INSUFFICIENT_MEMORY);
    TMSRingToneImpl *self = new TMSRingToneImpl();

    TRACE_PRN_FN_ENT;
    if (self)
        {
        ret = self->PostConstruct();
        if (ret != TMS_RESULT_SUCCESS)
            {
            delete self;
            self = NULL;
            }
        }
    if (self && ret == TMS_RESULT_SUCCESS)
        {
        rt = self;
        self->SetParent(rt);
        }
    TRACE_PRN_FN_EXT;
    return ret;
    }

EXPORT_C gint TMSRingToneImpl::Create(TMSRingTone*& rt, RWindow& window,
        gint scrid)
    {
    TRACE_PRN_FN_ENT;
    gint ret(TMS_RESULT_INSUFFICIENT_MEMORY);
    TMSRingToneImpl *self = new TMSRingToneImpl();
    if (self)
        {
        ret = self->PostConstruct(window, scrid);
        if (ret != TMS_RESULT_SUCCESS)
            {
            delete self;
            self = NULL;
            }
        }
    if (self && ret == TMS_RESULT_SUCCESS)
        {
        rt = self;
        self->SetParent(rt);
        }
    TRACE_PRN_FN_EXT;
    return ret;
    }

EXPORT_C gint TMSRingToneImpl::Delete(TMSRingTone*& rt)
    {
    TRACE_PRN_FN_ENT;
    gint ret(TMS_RESULT_SUCCESS);
    delete (static_cast<TMSRingToneImpl*>(rt));
    rt = NULL;
    TRACE_PRN_FN_EXT;
    return ret;
    }

gint TMSRingToneImpl::SetParent(TMSRingTone*& parent)
    {
    gint ret(TMS_RESULT_SUCCESS);
    if (this->iBody)
        {
        static_cast<TMSRingToneBodyImpl*>(this->iBody)->SetParent(parent);
        }
    else
        {
        ret = TMS_RESULT_UNINITIALIZED_OBJECT;
        }
    return ret;
    }

