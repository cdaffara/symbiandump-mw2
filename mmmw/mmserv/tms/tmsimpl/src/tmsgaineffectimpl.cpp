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
#include "tmsgaineffectbodyimpl.h"
#include "tmsgaineffectimpl.h"

using namespace TMS;

TMSGainEffectImpl::TMSGainEffectImpl()
    {
    TRACE_PRN_FN_ENT;
    TRACE_PRN_FN_EXT;
    }

TMSGainEffectImpl::~TMSGainEffectImpl()
    {
    TRACE_PRN_FN_ENT;
    TRACE_PRN_FN_EXT;
    }

gint TMSGainEffectImpl::PostConstruct()
    {
    gint ret(TMS_RESULT_INSUFFICIENT_MEMORY);
    TMSGainEffectBody* bodyimpl(NULL);
    TRACE_PRN_FN_ENT;
    ret = TMSGainEffectBodyImpl::Create(bodyimpl);

    if (ret == TMS_RESULT_SUCCESS)
        {
        this->iBody = bodyimpl;
        }
    TRACE_PRN_FN_EXT;
    return ret;
    }

gint TMSGainEffectImpl::Create(TMSEffect*& tmseffect)
    {
    gint ret(TMS_RESULT_INSUFFICIENT_MEMORY);
    TMSGainEffectImpl *self = new TMSGainEffectImpl();

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

gint TMSGainEffectImpl::SetProxy(TMSCallProxy* aProxy, gpointer queuehandler)
    {
    gint ret(TMS_RESULT_SUCCESS);
    if (this->iBody)
        {
        static_cast<TMSGainEffectBodyImpl*>(this->iBody)->SetProxy(aProxy,
                queuehandler);
        }
    else
        {
        ret = TMS_RESULT_UNINITIALIZED_OBJECT;
        }
    return ret;
    }

gint TMSGainEffectImpl::SetParent(TMSEffect*& parent)
    {
    gint ret(TMS_RESULT_SUCCESS);
    if (this->iBody)
        {
        static_cast<TMSGainEffectBodyImpl*>(this->iBody)->SetParent(parent);
        }
    else
        {
        ret = TMS_RESULT_UNINITIALIZED_OBJECT;
        }
    return ret;
    }

