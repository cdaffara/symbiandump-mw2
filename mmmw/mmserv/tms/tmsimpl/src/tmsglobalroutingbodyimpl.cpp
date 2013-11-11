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
#include <tmsglobalroutingobsrvr.h>
#include "tmsutility.h"
#include "tmsproxy.h"
#include "tmsglobalroutingbodyimpl.h"

using namespace TMS;

TMSGlobalRoutingBodyImpl::TMSGlobalRoutingBodyImpl() :
    iObserver(NULL),
    iProxy(NULL),
    iParent(NULL)
    {
    }

TMSGlobalRoutingBodyImpl::~TMSGlobalRoutingBodyImpl()
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

gint TMSGlobalRoutingBodyImpl::Create(TMSGlobalRoutingBody*& bodyimpl)
    {
    gint ret(TMS_RESULT_INSUFFICIENT_MEMORY);
    TMSGlobalRoutingBodyImpl* self = new TMSGlobalRoutingBodyImpl();
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

gint TMSGlobalRoutingBodyImpl::PostConstruct()
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
        ret = iProxy->StartRoutingNotifier(); //starts TAR
        }
    RET_REASON_IF_ERR(ret);
    return ret;
    }

gint TMSGlobalRoutingBodyImpl::AddObserver(TMSGlobalRoutingObserver& obsrvr,
        gpointer user_data)
    {
    gint ret(TMS_RESULT_SUCCESS);
    if (!iObserver)
        {
        iObserver = &obsrvr;
        iUserData = user_data;
        if (iProxy)
            {
            ret = iProxy->SetMsgQueueNotifier(EMsgQueueGlobalRoutingType,
                    iObserver, iParent, iClientId);
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

gint TMSGlobalRoutingBodyImpl::RemoveObserver(TMSGlobalRoutingObserver& obsrvr)
    {
    gint ret(TMS_RESULT_SUCCESS);
    if (iProxy && (&obsrvr == iObserver))
        {
        ret = iProxy->RemoveMsgQueueNotifier(EMsgQueueGlobalRoutingType,
                iObserver);
        iObserver = NULL;
        ret = iProxy->CancelRoutingNotifier();
        }
    else
        {
        ret = TMS_RESULT_DOES_NOT_EXIST;
        }
    return ret;
    }

gint TMSGlobalRoutingBodyImpl::SetOutput(const TMSAudioOutput output)
    {
    gint ret(TMS_RESULT_SUCCESS);
    if (iProxy)
        {
        ret = iProxy->SetOutput(output);
        }
    else
        {
        ret = TMS_RESULT_DOES_NOT_EXIST;
        }
    return ret;
    }

gint TMSGlobalRoutingBodyImpl::GetOutput(TMSAudioOutput& output)
    {
    gint ret(TMS_RESULT_SUCCESS);
    if (iProxy)
        {
        ret = iProxy->GetOutput(output);
        }
    else
        {
        ret = TMS_RESULT_DOES_NOT_EXIST;
        }
    return ret;
    }

gint TMSGlobalRoutingBodyImpl::GetPreviousOutput(TMSAudioOutput& output)
    {
    gint ret(TMS_RESULT_SUCCESS);
    if (iProxy)
        {
        ret = iProxy->GetPreviousOutput(output);
        }
    else
        {
        ret = TMS_RESULT_DOES_NOT_EXIST;
        }
    return ret;
    }

gint TMSGlobalRoutingBodyImpl::GetAvailableOutputs(OutputVector& outputs)
    {
    gint ret(TMS_RESULT_SUCCESS);
    if (iProxy)
        {
        ret = iProxy->GetAvailableOutputs(outputs);
        }
    else
        {
        ret = TMS_RESULT_DOES_NOT_EXIST;
        }
    return ret;
    }

void TMSGlobalRoutingBodyImpl::SetParent(TMSGlobalRouting*& parent)
    {
    iParent = parent;
    }

