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
#include <tmsringtoneobsrvr.h>
#include "tmsutility.h"
#include "tmsproxy.h"
#include "tmsringtonebodyimpl.h"

using namespace TMS;

TMSRingToneBodyImpl::TMSRingToneBodyImpl() :
    iObserver(NULL),
    iProxy(NULL),
    iParent(NULL)
    {
    }

TMSRingToneBodyImpl::~TMSRingToneBodyImpl()
    {
    if (iProxy)
        {
        iProxy->Close();
        }
    delete iProxy;
    iObserver = NULL;
    iParent = NULL;
    }

gint TMSRingToneBodyImpl::Create(TMSRingToneBody*& bodyimpl)
    {
    gint ret(TMS_RESULT_INSUFFICIENT_MEMORY);
    TMSRingToneBodyImpl* self = new TMSRingToneBodyImpl();
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

gint TMSRingToneBodyImpl::Create(TMSRingToneBody*& bodyimpl,
        RWindow& /*window*/, gint /*scrid*/)
    {
    gint ret(TMS_RESULT_INSUFFICIENT_MEMORY);
    TMSRingToneBodyImpl* self = new TMSRingToneBodyImpl();
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

gint TMSRingToneBodyImpl::PostConstruct()
    {
    gint ret(TMS_RESULT_SUCCESS);
    iClientId = 1;
    iProxy = new TMSProxy();
    if (!iProxy)
        {
        ret = TMS_RESULT_INSUFFICIENT_MEMORY;
        }
    else
        {
        if (iProxy->Connect() != TMS_RESULT_SUCCESS)
            {
            delete iProxy;
            iProxy = NULL;
            ret = TMS_RESULT_FATAL_ERROR;
            }
        }
    RET_REASON_IF_ERR(ret);
    return ret;
    }

gint TMSRingToneBodyImpl::AddObserver(TMSRingToneObserver& obsrvr,
        gpointer user_data)
    {
    gint ret(TMS_RESULT_SUCCESS);
    if (!iObserver)
        {
        iObserver = &obsrvr;
        iUserData = user_data;
        if (iProxy)
            {
            ret = iProxy->SetMsgQueueNotifier(EMsgQueueRingtoneType, iObserver,
                    iParent, iClientId);
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

gint TMSRingToneBodyImpl::RemoveObserver(TMSRingToneObserver& obsrvr)
    {
    gint ret(TMS_RESULT_SUCCESS);
    if (iProxy && (&obsrvr == iObserver))
        {
        ret = iProxy->RemoveMsgQueueNotifier(EMsgQueueRingtoneType, iObserver);
        iObserver = NULL;
        }
    else
        {
        ret = TMS_RESULT_DOES_NOT_EXIST;
        }
    return ret;
    }

gint TMSRingToneBodyImpl::Init(const TMSRingToneType type, GString* str,
        GString* tts)
    {
    gint ret(TMS_RESULT_SUCCESS);
    if (iProxy)
        {
        ret = iProxy->InitRT(type, str, tts);
        }
    else
        {
        ret = TMS_RESULT_DOES_NOT_EXIST;
        }
    return ret;
    }

gint TMSRingToneBodyImpl::Deinit()
    {
    gint ret(TMS_RESULT_SUCCESS);
    if (iProxy)
        {
        ret = iProxy->DeinitRT();
        }
    else
        {
        ret = TMS_RESULT_DOES_NOT_EXIST;
        }
    return ret;
    }

gint TMSRingToneBodyImpl::Play()
    {
    gint ret(TMS_RESULT_SUCCESS);
    if (iProxy)
        {
        ret = iProxy->PlayRT();
        }
    else
        {
        ret = TMS_RESULT_DOES_NOT_EXIST;
        }
    return ret;
    }

gint TMSRingToneBodyImpl::Stop()
    {
    gint ret(TMS_RESULT_SUCCESS);
    if (iProxy)
        {
        ret = iProxy->StopRT();
        }
    else
        {
        ret = TMS_RESULT_DOES_NOT_EXIST;
        }
    return ret;
    }

gint TMSRingToneBodyImpl::Pause()
    {
    gint ret(TMS_RESULT_SUCCESS);
    if (iProxy)
        {
        ret = iProxy->PauseRT();
        }
    else
        {
        ret = TMS_RESULT_DOES_NOT_EXIST;
        }
    return ret;
    }

gint TMSRingToneBodyImpl::Mute()
    {
    gint ret(TMS_RESULT_SUCCESS);
    if (iProxy)
        {
        ret = iProxy->MuteRT();
        }
    else
        {
        ret = TMS_RESULT_DOES_NOT_EXIST;
        }
    return ret;
    }

void TMSRingToneBodyImpl::SetParent(TMSRingTone*& parent)
    {
    iParent = parent;
    }

