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
#include "tmsvolumeeffectbodyimpl.h"

using namespace TMS;

TMSVolumeEffectBodyImpl::TMSVolumeEffectBodyImpl() :
    iObserver(NULL),
    iProxy(NULL),
    iParent(NULL)
    {
    }

TMSVolumeEffectBodyImpl::~TMSVolumeEffectBodyImpl()
    {
    TRACE_PRN_FN_ENT;
    TRACE_PRN_FN_EXT;
    }

gint TMSVolumeEffectBodyImpl::Create(TMSVolumeEffectBody*& bodyimpl)
    {
    gint ret(TMS_RESULT_INSUFFICIENT_MEMORY);
    TMSVolumeEffectBodyImpl* self = new TMSVolumeEffectBodyImpl();
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

gint TMSVolumeEffectBodyImpl::PostConstruct()
    {
    gint ret(TMS_RESULT_SUCCESS);
    return ret;
    }

gint TMSVolumeEffectBodyImpl::AddObserver(TMSEffectObserver& obsrvr,
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

gint TMSVolumeEffectBodyImpl::RemoveObserver(TMSEffectObserver& obsrvr)
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

gint TMSVolumeEffectBodyImpl::GetLevel(guint& level)
    {
    gint ret(TMS_RESULT_SUCCESS);
    if (iProxy)
        {
        ret = iProxy->GetVolume(level);
        }
    else
        {
        ret = TMS_RESULT_DOES_NOT_EXIST;
        }
    return ret;
    }

gint TMSVolumeEffectBodyImpl::SetLevel(const guint level)
    {
    gint ret(TMS_RESULT_SUCCESS);
    if (iProxy)
        {
        ret = iProxy->SetVolume(level);
        }
    else
        {
        ret = TMS_RESULT_DOES_NOT_EXIST;
        }
    return ret;
    }

gint TMSVolumeEffectBodyImpl::GetMaxLevel(guint& level)
    {
    gint ret(TMS_RESULT_SUCCESS);
    if (iProxy)
        {
        ret = iProxy->GetMaxVolume(level);
        }
    else
        {
        ret = TMS_RESULT_DOES_NOT_EXIST;
        }
    return ret;
    }

gint TMSVolumeEffectBodyImpl::GetType(TMSEffectType& effecttype)
    {
    gint ret(TMS_RESULT_SUCCESS);
    effecttype = TMS_EFFECT_VOLUME;
    return ret;
    }

void TMSVolumeEffectBodyImpl::SetParent(TMSEffect*& parent)
    {
    iParent = parent;
    }

void TMSVolumeEffectBodyImpl::SetProxy(TMSCallProxy* aProxy,
        gpointer queuehandler)
    {
    iProxy = aProxy;
    if (queuehandler)
        {
        static_cast<TMSQueueHandler*>(queuehandler)->AddObserver(*this,
                TMS_EFFECT_VOLUME);
        }
    }

void TMSVolumeEffectBodyImpl::QueueEvent(TInt aEventType, TInt aError,
        void* event_data)
    {
    TMSSignalEvent event = {}; //all elements initialized to zeros
    event.user_data = NULL; //use only to return data passed in by the user
    event.type = TMS_EVENT_EFFECT_VOL_CHANGED;
    event.reason = aError;

    if (event_data)
        {
        event.event_data = static_cast<gpointer>(event_data);
        }

    if (iObserver && iParent && aEventType == TMS_EVENT_EFFECT_VOL_CHANGED)
        {
        iObserver->EffectsEvent(*iParent, event);
        }
    }

