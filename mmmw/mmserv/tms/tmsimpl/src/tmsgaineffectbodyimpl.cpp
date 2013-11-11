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

#include <tms.h>
#include <tmseffectobsrvr.h>
#include "tmsutility.h"
#include "tmscallproxy.h"
#include "tmsqueuehandler.h"
#include "tmsgaineffectbodyimpl.h"

using namespace TMS;

TMSGainEffectBodyImpl::TMSGainEffectBodyImpl() :
    iObserver(NULL),
    iProxy(NULL),
    iParent(NULL)
    {
    }

TMSGainEffectBodyImpl::~TMSGainEffectBodyImpl()
    {
    TRACE_PRN_FN_ENT;
    TRACE_PRN_FN_EXT;
    }

gint TMSGainEffectBodyImpl::Create(TMSGainEffectBody*& bodyimpl)
    {
    gint ret(TMS_RESULT_INSUFFICIENT_MEMORY);
    TMSGainEffectBodyImpl* self = new TMSGainEffectBodyImpl();
    if (self)
        {
        ret = self->PostConstruct();
        if (ret != TMS_RESULT_SUCCESS)
            {
            delete self;
            self = NULL;
            }
        }
    bodyimpl = self;
    return ret;
    }

gint TMSGainEffectBodyImpl::PostConstruct()
    {
    gint ret(TMS_RESULT_SUCCESS);
    return ret;
    }

gint TMSGainEffectBodyImpl::AddObserver(TMSEffectObserver& obsrvr,
        gpointer user_data)
    {
    gint ret(TMS_RESULT_SUCCESS);
    if (!iObserver)
        {
        iObserver = &obsrvr;
        iUserData = user_data;
        }
    else
        {
        ret = TMS_RESULT_ALREADY_EXIST;
        }
    return ret;
    }

gint TMSGainEffectBodyImpl::RemoveObserver(TMSEffectObserver& obsrvr)
    {
    gint ret(TMS_RESULT_SUCCESS);
    if (&obsrvr == iObserver)
        {
        iObserver = NULL;
        }
    else
        {
        ret = TMS_RESULT_DOES_NOT_EXIST;
        }
    return ret;
    }

gint TMSGainEffectBodyImpl::GetLevel(guint& level)
    {
    gint ret(TMS_RESULT_SUCCESS);
    if (iProxy)
        {
        ret = iProxy->GetGain(level);
        }
    else
        {
        ret = TMS_RESULT_DOES_NOT_EXIST;
        }
    return ret;
    }

gint TMSGainEffectBodyImpl::SetLevel(const guint level)
    {
    gint ret(TMS_RESULT_SUCCESS);
    if (iProxy)
        {
        ret = iProxy->SetGain(level);
        }
    else
        {
        ret = TMS_RESULT_DOES_NOT_EXIST;
        }
    return ret;
    }

gint TMSGainEffectBodyImpl::GetMaxLevel(guint& level)
    {
    gint ret(TMS_RESULT_SUCCESS);
    if (iProxy)
        {
        ret = iProxy->GetMaxGain(level);
        }
    else
        {
        ret = TMS_RESULT_DOES_NOT_EXIST;
        }
    return ret;
    }

gint TMSGainEffectBodyImpl::GetType(TMSEffectType& effecttype)
    {
    gint ret(TMS_RESULT_SUCCESS);
    effecttype = TMS_EFFECT_GAIN;
    return ret;
    }

void TMSGainEffectBodyImpl::SetParent(TMSEffect*& parent)
    {
    iParent = parent;
    }

void TMSGainEffectBodyImpl::SetProxy(TMSCallProxy* aProxy,
        gpointer queuehandler)
    {
    iProxy = aProxy;
    if (queuehandler)
        {
        static_cast<TMSQueueHandler*>(queuehandler)->AddObserver(*this,
                TMS_EFFECT_GAIN);
        }
    }

void TMSGainEffectBodyImpl::QueueEvent(TInt aEventType, TInt aError,
        void* /*user_data*/)
    {
    TMSSignalEvent event = {}; //all elements initialized to zeros
    event.type = TMS_EVENT_EFFECT_GAIN_CHANGED;
    event.reason = aError;

    if (iObserver && iParent && aEventType == TMS_EVENT_EFFECT_GAIN_CHANGED)
        {
        iObserver->EffectsEvent(*iParent, event);
        }
    }

