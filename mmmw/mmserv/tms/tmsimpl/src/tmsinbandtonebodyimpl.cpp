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
#include <tmsinbandtoneobsrvr.h>
#include "tmsutility.h"
#include "tmsproxy.h"
#include "tmsinbandtonebodyimpl.h"

using namespace TMS;

TMSInbandToneBodyImpl::TMSInbandToneBodyImpl() :
    iObserver(NULL),
    iProxy(NULL),
    iParent(NULL)
    {
    }

TMSInbandToneBodyImpl::~TMSInbandToneBodyImpl()
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

gint TMSInbandToneBodyImpl::Create(TMSInbandToneBody*& bodyimpl)
    {
    gint ret(TMS_RESULT_INSUFFICIENT_MEMORY);
    TMSInbandToneBodyImpl* self = new TMSInbandToneBodyImpl();
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

gint TMSInbandToneBodyImpl::PostConstruct()
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
    RET_REASON_IF_ERR(ret);
    return ret;
    }

gint TMSInbandToneBodyImpl::AddObserver(TMSInbandToneObserver& obsrvr,
        gpointer user_data)
    {
    gint ret(TMS_RESULT_SUCCESS);
    if (!iObserver)
        {
        iObserver = &obsrvr;
        iUserData = user_data;
        if (iProxy)
            {
            ret = iProxy->SetMsgQueueNotifier(EMsgQueueInbandToneType,
                    iObserver, iParent, iClientId);
            if (ret == TMS_RESULT_SUCCESS)
                {
                //ret = iProxy->StartInbandToneNotifier(); //TODO: remove?
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

gint TMSInbandToneBodyImpl::RemoveObserver(TMSInbandToneObserver& obsrvr)
    {
    gint ret(TMS_RESULT_SUCCESS);
    if (iProxy && (&obsrvr == iObserver))
        {
        ret = iProxy->RemoveMsgQueueNotifier(EMsgQueueInbandToneType,
                iObserver);
        iObserver = NULL;
        //iProxy->CancelInbandToneNotifier(); //TODO: remove?
        }
    else
        {
        ret = TMS_RESULT_DOES_NOT_EXIST;
        }
    return ret;
    }

gint TMSInbandToneBodyImpl::Start(TMSInbandToneType inbandtone)
    {
    gint ret(TMS_RESULT_SUCCESS);

    if (iProxy)
        {
        ret = iProxy->StartInbandTone(inbandtone);
        }
    else
        {
        ret = TMS_RESULT_DOES_NOT_EXIST;
        }
    return ret;
    }

gint TMSInbandToneBodyImpl::Stop()
    {
    gint ret(TMS_RESULT_SUCCESS);
    if (iProxy)
        {
        ret = iProxy->StopInbandTone();
        }
    else
        {
        ret = TMS_RESULT_DOES_NOT_EXIST;
        }
    return ret;
    }

void TMSInbandToneBodyImpl::SetParent(TMSInbandTone*& parent)
    {
    iParent = parent;
    }

