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
#include "tmsvolumeeffectbodyimpl.h"
#include "tmsvolumeeffectimpl.h"

using namespace TMS;

TMSVolumeEffectImpl::TMSVolumeEffectImpl()
    {
    TRACE_PRN_FN_ENT;
    TRACE_PRN_FN_EXT;
    }

TMSVolumeEffectImpl::~TMSVolumeEffectImpl()
    {
    TRACE_PRN_FN_ENT;
    TRACE_PRN_FN_EXT;
    }

gint TMSVolumeEffectImpl::PostConstruct()
    {
    gint ret(TMS_RESULT_INSUFFICIENT_MEMORY);
    TMSVolumeEffectBody* bodyimpl(NULL);
    TRACE_PRN_FN_ENT;
    ret = TMSVolumeEffectBodyImpl::Create(bodyimpl);

    if (ret == TMS_RESULT_SUCCESS)
        {
        this->iBody = bodyimpl;
        }
    TRACE_PRN_FN_EXT;
    return ret;
    }

gint TMSVolumeEffectImpl::Create(TMSEffect*& tmseffect)
    {
    gint ret(TMS_RESULT_INSUFFICIENT_MEMORY);
    TMSVolumeEffectImpl *self = new TMSVolumeEffectImpl();

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
        tmseffect = self;
        ret = self->SetParent(tmseffect);
        }
    TRACE_PRN_FN_EXT;
    return ret;
    }

gint TMSVolumeEffectImpl::SetProxy(TMSCallProxy* aProxy, gpointer queuehandler)
    {
    gint ret(TMS_RESULT_SUCCESS);
    if (this->iBody)
        {
        static_cast<TMSVolumeEffectBodyImpl*>(this->iBody)->SetProxy(aProxy,
                queuehandler);
        }
    else
        {
        ret = TMS_RESULT_UNINITIALIZED_OBJECT;
        }
    return ret;
    }

gint TMSVolumeEffectImpl::SetParent(TMSEffect*& parent)
    {
    gint ret(TMS_RESULT_SUCCESS);
    if (this->iBody)
        {
        static_cast<TMSVolumeEffectBodyImpl*>(this->iBody)->SetParent(parent);
        }
    else
        {
        ret = TMS_RESULT_UNINITIALIZED_OBJECT;
        }
    return ret;
    }

