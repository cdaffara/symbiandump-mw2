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
#include "tmsproxy.h"
#include "tmsutility.h"
#include "tmsglobalgaineffectbodyimpl.h"

using namespace TMS;

TMSGlobalGainEffectBodyImpl::TMSGlobalGainEffectBodyImpl() :
    iObserver(NULL),
    iProxy(NULL),
    iParent(NULL)
    {
    }

TMSGlobalGainEffectBodyImpl::~TMSGlobalGainEffectBodyImpl()
    {
    if (iProxy)
        {
        iProxy->Close();
        delete iProxy;
        iProxy = NULL;
        }
    iObserver = NULL;
    iParent = NULL;
    iUserData = NULL;
    }

gint TMSGlobalGainEffectBodyImpl::Create(TMSGlobalGainEffectBody*& bodyimpl)
    {
    gint ret(TMS_RESULT_INSUFFICIENT_MEMORY);
    TMSGlobalGainEffectBodyImpl* self = new TMSGlobalGainEffectBodyImpl();
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

gint TMSGlobalGainEffectBodyImpl::PostConstruct()
    {
    gint ret(TMS_RESULT_SUCCESS);
    iClientId = 1;
    iProxy = new TMSProxy();
    if (!iProxy)
        {
        ret = TMS_RESULT_INSUFFICIENT_MEMORY;
        }
    RET_REASON_IF_ERR(ret);

    if (iProxy->Connect() != TMS_RESULT_SUCCESS)
        {
        delete iProxy;
        iProxy = NULL;
        ret = TMS_RESULT_FATAL_ERROR;
        }
    else
        {
        // Starts TAR if not started already;
        ret = iProxy->StartRoutingNotifier();
        }
    RET_REASON_IF_ERR(ret);
    return ret;
    }

gint TMSGlobalGainEffectBodyImpl::AddObserver(TMSEffectObserver& obsrvr,
        gpointer user_data)
    {
    gint ret(TMS_RESULT_SUCCESS);
    if (!iObserver)
        {
        iObserver = &obsrvr;
        iUserData = user_data;
        if (iProxy)
            {
            ret = iProxy->SetMsgQueueNotifier(EMsgQueueGlobalGainType,
                    iObserver, iParent, iClientId);
            if (ret == TMS_RESULT_SUCCESS)
                {
                ret = iProxy->StartGlobalEffectNotifier();
                }
            }
        else
            {
            ret = TMS_RESULT_DOES_NOT_EXIST;
            }
        }
    else
        {
        ret = TMS_RESULT_ALREADY_EXIST;
        }
    return ret;
    }

gint TMSGlobalGainEffectBodyImpl::RemoveObserver(TMSEffectObserver& obsrvr)
    {
    gint ret(TMS_RESULT_SUCCESS);
    if (iProxy && (&obsrvr == iObserver))
        {
        ret = iProxy->RemoveMsgQueueNotifier(EMsgQueueGlobalGainType,
                iObserver);
        iObserver = NULL;
        ret = iProxy->CancelGlobalEffectNotifier();
        ret = iProxy->CancelRoutingNotifier();
        }
    else
        {
        ret = TMS_RESULT_DOES_NOT_EXIST;
        }
    return ret;
    }

gint TMSGlobalGainEffectBodyImpl::GetLevel(guint& level)
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

gint TMSGlobalGainEffectBodyImpl::SetLevel(const guint level)
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

gint TMSGlobalGainEffectBodyImpl::GetMaxLevel(guint& level)
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

gint TMSGlobalGainEffectBodyImpl::GetType(TMSEffectType& effecttype)
    {
    gint ret(TMS_RESULT_SUCCESS);
    effecttype = TMS_EFFECT_GLOBAL_GAIN;
    return ret;
    }

void TMSGlobalGainEffectBodyImpl::SetParent(TMSEffect*& parent)
    {
    iParent = parent;
    }

